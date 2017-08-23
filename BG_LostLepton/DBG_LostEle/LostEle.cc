#define LostEle_cxx
#include "LostEle.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{

  if (argc < 2) {
    cerr << "Please give 3 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];

  LostEle ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void LostEle::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  string s_data=data;
  
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  
  int evtSurvived=0;
  //get 2d histogram========================================
  TFile *f_LP=new TFile("LstEle_CS_TTWZ_LostEle_v2.root");
  //TFile *f_LP=new TFile("LstEle_CS_TTW_LostEle_v2.root");
  TH2D *h2_LP;TH1D *h_LP;
  bool do_prediction=1;
  cout<<"Doing prediction from file |"<<f_LP->GetName()<<"|? "<<do_prediction<<endl;
  
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" <<endl;
    decade = k;
    // cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    bool process=true;
    if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0)) continue;
   
    //About photons
    bestPhoton=getBestPhoton();
    if(bestPhoton.Pt() <= 100) continue;
    if(check_eMatchedtoGamma()) continue;
    //leptons
    if(Muons->size()>0) continue;//veto muons for lost electron estimation
    if(Electrons->size()>1) continue;//exactly one RECO electron or no electron(lost)
    if(isoMuonTracks!=0 || isoPionTracks!=0) continue;
    double mt_ele=0,mt_pho=0,mt_elepho=0;
    if(Electrons->size()==1){
      mt_ele=sqrt(2*(*Electrons)[0].Pt()*MET*(1-cos(DeltaPhi(METPhi,(*Electrons)[0].Phi()))));
      if(mt_ele>100) continue;
      if( ((*Electrons)[0].Pt() < 10) || abs((*Electrons)[0].Eta()) > 2.5 ) continue;
      //      if(!(*Electrons_tightID)[0]) continue;
    }
    mt_pho=sqrt(2*bestPhoton.Pt()*MET*(1-cos(DeltaPhi(METPhi,bestPhoton.Phi()))));
    if(Electrons->size()==1) mt_elepho=sqrt(2*(bestPhoton+(*Electrons)[0]).Pt()*MET*(1-cos(DeltaPhi(METPhi,(bestPhoton+(*Electrons)[0]).Phi()))));
    //calulate ST and HadJets by cleaning the matching jet.
    bool hadJetID=true;
    int minDRindx=-100,phoMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0,remJetPt=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= HT_EtaCut) ){
	double dR=bestPhoton.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= HT_EtaCut) ){
	if( !(minDR < 0.3 && i==minDRindx) ){
	  hadJets.push_back((*Jets)[i]);
	  if(hadJetID) hadJetID=(*Jets_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) phoMatchingJetIndx=minDRindx;
   
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
    }
    sortTLorVec(&hadJets);
    
    //ST and HadJets have been determined. Now calulate dPhi b/w MET and leading HadJets.
    double dphi1=3.8,dphi2=3.8,dphi3=3.8,dphi4=3.8,dphiPho_MET=3.8;
    if(bestPhoton.Pt()>0.1) dphiPho_MET=abs(DeltaPhi(METPhi,bestPhoton.Phi()));
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));
    if(hadJets.size() > 2 ) dphi3 = abs(DeltaPhi(METPhi,(hadJets)[2].Phi()));
    if(hadJets.size() > 3 ) dphi4 = abs(DeltaPhi(METPhi,(hadJets)[3].Phi()));

    //+++++++++++++++++++++++++ data only +++++++++++++++++++++++++++++
    wt=1;
    if((Electrons->size()==0) && (Muons->size()==0)) continue;//0-lepton is SR. Blind this region.
    //Trigger related
    bool passHT600Photon90Trigger = false;//(ST>800 && bestPhoton.Pt()>100);
    bool passPhoton165HE10Trigger = false;//(bestPhoton.Pt()>190);
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back();
      if( trgName=="HLT_Photon90_CaloIdL_PFHT600_v" && (*TriggerPass)[i]==1 ) passHT600Photon90Trigger = true;
      else if( trgName=="HLT_Photon165_HE10_v" && (*TriggerPass)[i]==1 ) passPhoton165HE10Trigger = true;
    }
    if(!(passPhoton165HE10Trigger || passHT600Photon90Trigger)) continue;
    //++++++++++++++++++++++ data only ends +++++++++++++++++++++++++++

    //---------------------- MC only -------------------------
    /*    wt=Weight*1000.0*lumiInfb;
    //Trigger related
    wt=wt*0.98;
    //done with trigger efficiencies

    TLorentzVector genPho1,genEle1,neutr;
    int leadGenPhoIdx=-100;
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if((abs((*GenParticles_PdgId)[i])==22) && ((abs((*GenParticles_ParentId)[i])<=25) || ((*GenParticles_ParentId)[i]==2212) ) && (*GenParticles_Status)[i]==1 ){
	  if(genPho1.Pt() < (*GenParticles)[i].Pt()){
	    leadGenPhoIdx = i;
	    genPho1 = ((*GenParticles)[i]);
	  }
	}
	if( (abs((*GenParticles_PdgId)[i])==13) && (abs((*GenParticles_ParentId)[i])<=25) && (*GenParticles_Status)[i]==1 ){
	  if(genEle1.Pt() < ((*GenParticles)[i]).Pt()) genEle1 = ((*GenParticles)[i]);
	}
	if( (abs((*GenParticles_PdgId)[i])==14) && (abs((*GenParticles_ParentId)[i])<=25) && (*GenParticles_Status)[i]==1 ){
	  neutr = ((*GenParticles)[i]);
	}
      }
    }
    double invMelePhoNu=0.0;
    invMelePhoNu = (neutr+genEle1+genPho1).M();

    gendRLepPho=1000;
    gendRLepPho = getGendRLepPho();

    if( (s_data=="WG") || (s_data=="TTG") || (s_data=="TG") || (s_data=="ZG") ){
      if(jentry<3) cout<<"Prompt";
    }
    else if( s_data=="WJets" ){
      if(hasGenPromptPhoton && gendRLepPho > 0.5 && madMinPhotonDeltaR > 0.5) continue;
      if(jentry<3) cout<<"Non-Prompt, dR(pho,q/g/lep) < 0.5 ";
    }
    else if( (s_data=="TTJets") || (s_data=="SingleTop") || (s_data=="ZJets") ){
      if(hasGenPromptPhoton && gendRLepPho > 0.3 && madMinPhotonDeltaR > 0.3) continue;
      if(jentry<3) cout<<"Non-Prompt, dR(pho,q/g/lep) < 0.3 ";
    }
    
    int nGenMu=0,nGenEle=0,nGenTau=0,nGenMuFmTau=0,nGenEleFmTau=0;
    vector<TLorentzVector> genEle;   
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if( abs((*GenParticles_PdgId)[i])==11 && (abs((*GenParticles_ParentId)[i])<=24) && ((*GenParticles_Status)[i]==1) ) {nGenEle++; genEle.push_back((*GenParticles)[i]);}//electrons
	else if( abs((*GenParticles_PdgId)[i])==13 && (abs((*GenParticles_ParentId)[i])<=24) && ((*GenParticles_Status)[i]==1) ) {
	  nGenMu++;
	}//muons
	else if( abs((*GenParticles_PdgId)[i])==15 && (abs((*GenParticles_ParentId)[i])<=24) ) {nGenTau++;}//taus
      }
    }
    if(nGenMu==0 && nGenEle==0 && nGenTau==0) continue;//to reject W->qq' type of events

    if(Electrons->size()==0){
      if(isoMuonTracks!=0 || isoElectronTracks!=0 || isoPionTracks!=0) continue;
      if(nGenEle==0) continue;
      if(nGenMu!=0) continue;
    }
    if(nGenEle==0) {TLorentzVector v1;genEle.push_back(v1);}
    sortTLorVec(&genEle);
    //check if the photon is real or fake
    bool realPho=true;
    int matche=0,matchp=0;
    double minDR_Pho_GenObj=1000;
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
	if(dr1 < 0.1 && (abs((*GenParticles_PdgId)[i])==11) && (abs((*GenParticles_ParentId)[i])<=24) ) {matche=1;realPho=false;}
	if(minDR_Pho_GenObj > dr1) minDR_Pho_GenObj=dr1;
      }
    }
    h_minDR_Pho_GenObj->Fill(minDR_Pho_GenObj,wt);
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
	if( dr1<0.1 && (abs((*GenParticles_PdgId)[i])==22) && 
	    ( ((*GenParticles)[i].Pt()/bestPhoton.Pt()) > 0.9) && ( ((*GenParticles)[i].Pt()/bestPhoton.Pt()) < 1.1) )
	  { matchp=1;realPho=true;}
      }
    }
    if(Electrons->size()==0){
      if(matche==1 && matchp==0) continue;//if reco photon matched to gen e and not matched to gen photon, it is fake.
    }//photon has been identified. It is a real photon and it is matched to gen photon with dR(genPho,RecoPho) < 0.1 and Pts are within 10%.
    //---------------------- MC only ends-------------------------
    */
    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) )  continue;
    process = process && ST>500 && MET > 100 && nHadJets >=2 && dphi1 > 0.3 && dphi2 > 0.3 && bestPhoton.Pt() > 100;

    if(process && hadJetID){
      evtSurvived++;
      double wt_org=wt;
      h_RunNum->Fill(RunNum);
      h_intLumi->Fill(lumiInfb);
      //      print(jentry);
      h_ST->Fill(ST,wt);
      h_MET->Fill(MET,wt);
      h_nHadJets->Fill(nHadJets,wt);
      h_BTags->Fill(BTags,wt);
      h_HT->Fill(HT,wt);
      h_MHT->Fill(MHT,wt);
      h_nJets->Fill(NJets,wt);
      h_METvBin->Fill(MET,wt);
      //h_madHT->Fill(madHT,wt);//MC only
      h_nVtx->Fill(NVtx,wt);
      //      findObjMatchedtoG(bestPhoton);//MC only
      int nEleMultJ=0;
      for(int i=0;i<Jets->size();i++){
	if((*Jets)[i].Pt()>30.0){ nEleMultJ = nEleMultJ + (*Jets_electronMultiplicity)[i]; }
      }
      int searchRegion=0;
      if     (nHadJets >= 2 && nHadJets <= 4 ) searchRegion=1;
      else if(nHadJets == 5 || nHadJets == 6 ) searchRegion=2;
      else if(nHadJets >= 7                  ) searchRegion=3;
      int sBin2=-100,m_i=0;
      if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      sBin2 = 0;
      else if(nHadJets >= 5 && BTags==0)                  sBin2 = 7;
      else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) sBin2 = 14;
      else if(nHadJets >= 5 && BTags==1)                  sBin2 = 21;
      else if(BTags>=2)                                   sBin2 = 28;
      for(int i=0;i<METBinLowEdge.size()-1;i++){
        if(METBinLowEdge[i]<99.99) continue;
        m_i++;
        if(MET>=METBinLowEdge[i] && MET<METBinLowEdge[i+1]){ sBin2 = sBin2+m_i;break; }
        else if(MET>=METBinLowEdge[METBinLowEdge.size()-1]){ sBin2 = sBin2+7  ;break; }
      }
      if(BTags>=2 && sBin2==35) sBin2=34;
      //      if(Electrons->size()==0){//MC only
      //+++++++++++++++++++++++++ data only +++++++++++++++++++++++++++++
      if(Electrons->size()==1){
	if(do_prediction){
          double parX=MET,parY=nHadJets;
          double tf=0;
          TString name;
          // if(nHadJets==2 || nHadJets==3)      name="LostProb_1";                                                                           
          // else if(nHadJets==4)                name="LostProb_2";                                                                           
          // else if(nHadJets==5 || nHadJets==6) name="LostProb_3";                                                                           
          // else if(nHadJets>=7)                name="LostProb_4";                                                                           
          if(BTags==0)      name="LostProb_0";
          else if(BTags==1) name="LostProb_1";
          else if(BTags>=2) name="LostProb_2";
          h2_LP=(TH2D*)f_LP->FindObjectAny(name);
          if(h2_LP) tf=h2_LP->GetBinContent(h2_LP->FindBin(parX,parY));
          else cout<<"hist not found"<<endl;
          wt=tf*wt;
        }
	//++++++++++++++++++++++ data only ends +++++++++++++++++++++++++++	
        h_nVtx_Ele0->Fill(NVtx,wt);
	h_ST_Ele0->Fill(ST,wt);
	h_MET_Ele0->Fill(MET,wt);
	h_nHadJets_Ele0->Fill(nHadJets,wt);
	h_BTags_Ele0->Fill(BTags,wt);
	h_METvBin_Ele0->Fill(MET,wt);
	h_BestPhotonPt_Ele0->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_Ele0->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_Ele0->Fill(bestPhoton.Phi(),wt);
	h_mTpho_Ele0->Fill(mt_pho,wt);

	h_EleMultInJets_Ele0->Fill(nEleMultJ,wt);

	h_dPhi_PhoMET_Ele0->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_Ele0->Fill(dphi1,wt);
	h_dphi_METjet2_Ele0->Fill(dphi2,wt);

	h_Jet1Pt_Ele0->Fill(hadJets[0].Pt(),wt);
	h_Jet2Pt_Ele0->Fill(hadJets[1].Pt(),wt);
	h_isoEleTrack_Ele0->Fill(isoElectronTracks,wt);
	h_isoMuTrack_Ele0->Fill(isoMuonTracks,wt);
	h_isoPiTrack_Ele0->Fill(isoPionTracks,wt);
	//-----------------------MC only ------------------------------
	/*	h_nGenEle_Ele0->Fill(nGenEle,wt);//MC only
	h_nGenEle_Ele0->Fill(nGenEle,wt);//MC only
	h_nGenTau_Ele0->Fill(nGenTau,wt);//MC only

	h_GenEleEta_Ele0->Fill(genEle[0].Eta(),wt);//MC only
	h_GenElePt_Ele0->Fill(genEle[0].Pt(),wt);//MC only
	h_photonPrompt_Ele0->Fill(!((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]),wt);//MC only

	h_nGenEleFmTau_Ele0->Fill(nGenEleFmTau,wt);//MC only
	h_nGenMuFmTau_Ele0->Fill(nGenMuFmTau,wt);//MC only
	if(matche==1 && matchp==0) h_fakePhoPt_Ele0->Fill(bestPhoton.Pt(),wt);//MC only
	for(int i=0;i<GenParticles->size();i++){
	  if(abs((*GenParticles_PdgId)[i]) == 11){
	    h_genEleParent_Ele0->Fill(abs((*GenParticles_ParentId)[i]),wt);//MC only
	  }
	  }*/
	//-----------------------------------------------------
	//print(jentry);
	h2_METnHadJ_Ele0->Fill(MET,nHadJets,wt);
	h2_dPhi1nHadJ_Ele0->Fill(dphi1,nHadJets,wt);
	h2_dPhi2nHadJ_Ele0->Fill(dphi2,nHadJets,wt);
	h2_dPhi1MET_Ele0->Fill(dphi1,MET,wt);
	h2_STMET_Ele0->Fill(ST,MET,wt);
	h2_dPhi1dPhi2_Ele0->Fill(dphi1,dphi2,wt);
	h2_STHadJ_Ele0->Fill(ST,nHadJets,wt);
	h2_METJet1Pt_Ele0->Fill(MET,hadJets[0].Pt(),wt);
	h2_R_PhoPtJetPtVsDR_Ele0->Fill(minDR,((*Jets)[phoMatchingJetIndx].Pt())/bestPhoton.Pt(),wt);

	h3_STMETnHadJ_Ele0->Fill(ST,MET,nHadJets,wt);
	h2_hadJbTag_Ele0->Fill(nHadJets,BTags,wt);

	//	if(ST>5000 || MET>5000) cout<<"ST "<<ST<<" MET "<<MET<<endl;
	if(nHadJets==2 || nHadJets==3)      h2_STMET_NJ2or3_Ele0  ->Fill(ST,MET,wt);
	else if(nHadJets==4)                h2_STMET_NJ4_Ele0     ->Fill(ST,MET,wt);
	else if(nHadJets==5 || nHadJets==6) h2_STMET_NJ5or6_Ele0  ->Fill(ST,MET,wt);
	else if(nHadJets>=7)                h2_STMET_NJ7toInf_Ele0->Fill(ST,MET,wt);

	if(BTags==0)      h2_METNJ_0b_Ele0->Fill(MET,nHadJets,wt);
        else if(BTags==1) h2_METNJ_1b_Ele0->Fill(MET,nHadJets,wt);
        else if(BTags>=2) h2_METNJ_m2b_Ele0->Fill(MET,nHadJets,wt);     
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_Ele0_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! ";
	if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2_Ele0[0]->Fill(MET,wt);
	else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2_Ele0[1]->Fill(MET,wt);
	else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2_Ele0[2]->Fill(MET,wt);
	else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2_Ele0[3]->Fill(MET,wt);
        else if(BTags>=2)                                   h_MET_R_v2_Ele0[4]->Fill(MET,wt);
	h_SBins_Ele0->Fill(sBin2,wt);
	wt=wt_org;
      }//0 electron + photon events
      if(Electrons->size()==1){
	//----------------------------- MC only ---------------------------
	/*	if(do_prediction){
	  double parX=MET,parY=nHadJets;
	  double tf=0;
	  TString name;
	  // if(nHadJets==2 || nHadJets==3)      name="LostProb_1";
	  // else if(nHadJets==4)                name="LostProb_2";
	  // else if(nHadJets==5 || nHadJets==6) name="LostProb_3";
	  // else if(nHadJets>=7)                name="LostProb_4";
	  if(BTags==0)      name="LostProb_0";
          else if(BTags==1) name="LostProb_1";
          else if(BTags>=2) name="LostProb_2";
	  h2_LP=(TH2D*)f_LP->FindObjectAny(name);
	  if(h2_LP) tf=h2_LP->GetBinContent(h2_LP->FindBin(parX,parY));
	  else cout<<"hist not found"<<endl;
	  wt=tf*wt;
	  }*/
	//----------------------------------------------------------------
        h_nVtx_Ele1->Fill(NVtx,wt);
	h_ST_Ele1->Fill(ST,wt);
	h_MET_Ele1->Fill(MET,wt);
	h_nHadJets_Ele1->Fill(nHadJets,wt);
	h_BTags_Ele1->Fill(BTags,wt);
	h_METvBin_Ele1->Fill(MET,wt);
	h_BestPhotonPt_Ele1->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_Ele1->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_Ele1->Fill(bestPhoton.Phi(),wt);
	h_mTpho_Ele1->Fill(mt_pho,wt);
	h_dPhiEleMET->Fill(DeltaPhi((*Electrons)[0].Phi(),METPhi),wt);

	h_EleMultInJets_Ele1->Fill(nEleMultJ,wt);
        h_invMassPhoEle->Fill((bestPhoton+(*Electrons)[0]).M(),wt);

	h_ElePt->Fill((*Electrons)[0].Pt(),wt);
	h_EleEta->Fill((*Electrons)[0].Eta(),wt);
	h_ElePhi->Fill((*Electrons)[0].Phi(),wt);
	h_mTEle->Fill(mt_ele,wt);

	h_dPhi_PhoMET_Ele1->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_Ele1->Fill(dphi1,wt);
	h_dphi_METjet2_Ele1->Fill(dphi2,wt);
	h_dPhi_Ele_Photon->Fill(abs(bestPhoton.DeltaPhi((*Electrons)[0])),wt);

	h_Jet1Pt_Ele1->Fill(hadJets[0].Pt(),wt);	
	h_Jet2Pt_Ele1->Fill(hadJets[1].Pt(),wt);	
	h_isoEleTrack_Ele1->Fill(isoElectronTracks,wt);
	h_isoMuTrack_Ele1->Fill(isoMuonTracks,wt);
	h_isoPiTrack_Ele1->Fill(isoPionTracks,wt);
	h_dR_ElePho->Fill(bestPhoton.DeltaR((*Electrons)[0]),wt);
	h_mTPhoEleMET->Fill(mt_elepho,wt);
	//-------------------------------------- MC only -------------------------------------
	//	h_GenElePt_Ele1->Fill(genEle[0].Pt(),wt);//MC only
	/*	h_photonPrompt_Ele1->Fill(!((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]),wt);//MC only
	h_nGenEle_Ele1->Fill(nGenEle,wt);//MC only
	h_nGenMu_Ele1->Fill(nGenMu,wt);//MC only
	h_nGenTau_Ele1->Fill(nGenTau,wt);//MC only

	h_nGenEleFmTau_Ele1->Fill(nGenEleFmTau,wt);//MC only
	h_nGenMuFmTau_Ele1->Fill(nGenMuFmTau,wt);//MC only
	h_invMassElePhoNu->Fill(invMelePhoNu,wt);//MC only
	if(matche==1 && matchp==0) h_fakePhoPt_Ele1->Fill(bestPhoton.Pt(),wt);//MC only
	h2_GenElePtVsRECOElePt->Fill(genEle[0].Pt(),(*Electrons)[0].Pt(),wt);//MC only
*/
	//-------------------------------------------------------------------------------------
	h2_STvsElePt_Ele1->Fill((*Electrons)[0].Pt(),ST,wt);
	h2_METvsElePt_Ele1->Fill((*Electrons)[0].Pt(),MET,wt);

	h2_METnHadJ_Ele1->Fill(MET,nHadJets,wt);
	h2_dPhi1nHadJ_Ele1->Fill(dphi1,nHadJets,wt);
	h2_dPhi2nHadJ_Ele1->Fill(dphi2,nHadJets,wt);
	h2_dPhi1MET_Ele1->Fill(dphi1,MET,wt);
	h2_STMET_Ele1->Fill(ST,MET,wt);
	h2_dPhi1dPhi2_Ele1->Fill(dphi1,dphi2,wt);
	h2_STHadJ_Ele1->Fill(ST,nHadJets,wt);
	h2_METJet1Pt_Ele1->Fill(MET,hadJets[0].Pt(),wt);
	h2_RecoElePtRecoAct_Ele1->Fill((*Electrons)[0].Pt(),(*Electrons_MT2Activity)[0],wt);
	h2_R_PhoPtJetPtVsDR_Ele1->Fill(minDR,((*Jets)[phoMatchingJetIndx].Pt())/bestPhoton.Pt(),wt);

	h3_STMETnHadJ_Ele1->Fill(ST,MET,nHadJets,wt);
	
	h2_hadJbTag_Ele1->Fill(nHadJets,BTags,wt);
	if(nHadJets==2 || nHadJets==3)      h2_STMET_NJ2or3_Ele1  ->Fill(ST,MET,wt);
	else if(nHadJets==4)                h2_STMET_NJ4_Ele1     ->Fill(ST,MET,wt);
	else if(nHadJets==5 || nHadJets==6) h2_STMET_NJ5or6_Ele1  ->Fill(ST,MET,wt);
	else if(nHadJets>=7)                h2_STMET_NJ7toInf_Ele1->Fill(ST,MET,wt);

	if(BTags==0)      h2_METNJ_0b_Ele1->Fill(MET,nHadJets,wt);
        else if(BTags==1) h2_METNJ_1b_Ele1->Fill(MET,nHadJets,wt);
        else if(BTags>=2) h2_METNJ_m2b_Ele1->Fill(MET,nHadJets,wt);

	int minDReleIndx = -100;
	double minDRele=1000.0;
	for(int i=0;i<Jets->size();i++){
	  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
	    double dR=(*Electrons)[0].DeltaR((*Jets)[i]);
	    if(dR<minDRele){minDRele=dR;minDReleIndx=i;}
	  }
	}
	h2_R_ElePtJetPtVsDR->Fill( minDRele,(((*Jets)[minDReleIndx].Pt())/((*Electrons)[0].Pt())),wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_Ele1_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! "<<searchRegion;

	if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2_Ele1[0]->Fill(MET,wt);
	else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2_Ele1[1]->Fill(MET,wt);
	else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2_Ele1[2]->Fill(MET,wt);
	else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2_Ele1[3]->Fill(MET,wt);
        else if(BTags>=2)                                   h_MET_R_v2_Ele1[4]->Fill(MET,wt);
	h_SBins_Ele1->Fill(sBin2,wt);
	wt=wt_org;
      }//electron + photon events
    }
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
}


TLorentzVector LostEle::getBestPhoton(){
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;

  for(int iPho=0;iPho<Photons->size();iPho++){
    if( ((*Photons_fullID)[iPho]) && ((*Photons_hasPixelSeed)[iPho]<0.001) ) {
      goodPho.push_back( (*Photons)[iPho] );
      goodPhoIndx.push_back(iPho);
    }
  }
  int highPtIndx=-100;
  for(int i=0;i<goodPho.size();i++){
    if(i==0) highPtIndx=0;
    else if( (goodPho[highPtIndx].Pt()) < (goodPho[i].Pt()) ){highPtIndx=i;}
  }

  if(highPtIndx>=0){
    bestPhotonIndxAmongPhotons = goodPhoIndx[highPtIndx];
    return goodPho[highPtIndx];
  }
  else{
    bestPhotonIndxAmongPhotons = -100;
    TLorentzVector v0;return v0;
  }
}


bool LostEle::check_eMatchedtoGamma(){
  if(bestPhotonIndxAmongPhotons>=0){
    for(int i=0;i<Electrons->size();i++){
      if( (*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i] ) < 0.2){
	//	cout<<(*Electrons)[i].Pt()<<" "<<(*Electrons)[i].Eta()<<" "<<(*Electrons)[i].Phi()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Pt()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Eta()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Phi()<<" dR:"<<(*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i])<<endl;
	return true;
      }
    }
  }
  else
    return false;
}

double LostEle::getGendRLepPho(){//MC only
  TLorentzVector genPho1,genLep1;
  /*  int leadGenPhoIdx=-100;
  for(int i=0;i<GenParticles->size();i++){
    if((*GenParticles)[i].Pt()!=0){
      if((abs((*GenParticles_PdgId)[i])==22) && ((abs((*GenParticles_ParentId)[i])<=25) || ((*GenParticles_ParentId)[i]==2212) ) && (*GenParticles_Status)[i]==1 ){
	if(genPho1.Pt() < (*GenParticles)[i].Pt()){
	  leadGenPhoIdx = i;
	  genPho1 = ((*GenParticles)[i]);
	}
      }
      if( (abs((*GenParticles_PdgId)[i])==11 || abs((*GenParticles_PdgId)[i])==13 || abs((*GenParticles_PdgId)[i])==15 ) && (abs((*GenParticles_ParentId)[i])<=25) && (abs((*GenParticles_ParentId)[i])!=15) ){
	if(genLep1.Pt() < ((*GenParticles)[i]).Pt()) genLep1 = ((*GenParticles)[i]);
      }
    }
  }//for
  */
  if(genPho1.Pt() > 0. && genLep1.Pt() > 0.) return genLep1.DeltaR(genPho1);
  else return 1000.0;
}

void  LostEle::findObjMatchedtoG(TLorentzVector bestPhoton){//MC only
  /*  double dR=100;
  int match=-100;
  for(int i=0;i<GenParticles->size();i++){
    if((*GenParticles)[i].Pt()!=0){
      if(i==0){dR=DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi() );}
      else if(dR > (DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi())) ){
	dR=(DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi()));
	match=i;
      }
    }
  }
  if(dR<0.1){
    h_GmatchedObj->Fill(abs((*GenParticles_PdgId)[match]),wt);
    if(abs((*GenParticles_PdgId)[match])==22)  h_PdgIdPhoParent->Fill(abs((*GenParticles_ParentId)[match]),wt);
  }
  else{
    h_GmatchedObj->Fill(0.0,wt);
    h_PdgIdPhoParent->Fill(0.0,wt);
  }
  //find obj matched to electron
  dR=100;match=-100;
  if(Electrons->size()==1){
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if(i==0){dR=(*GenParticles)[i].DeltaR((*Electrons)[0]);}
	else if(dR > ((*GenParticles)[i].DeltaR( (*Electrons)[0]) ) ){
	  dR= (*GenParticles)[i].DeltaR((*Electrons)[0]);
	  match=i;
	}
      }
    }
    if(dR<0.1){
      h_EleMatchedObj->Fill(abs((*GenParticles_PdgId)[match]),wt);
      if(abs((*GenParticles_PdgId)[match])==11){
	h_PdgIdEleParent->Fill(abs((*GenParticles_ParentId)[match]),wt);
      }
    }
    else{
      h_EleMatchedObj->Fill(0.0,wt);
      h_PdgIdEleParent->Fill(0.0,wt);
    }
  }
  */
}



void LostEle::print(Long64_t jentry){
  cout<<"*********************************************************************************"<<endl;
  cout<<"Photons:"<<endl;
  for(int i=0;i<Photons->size();i++){
    double dR=0;//DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*Photons)[i].Eta(),(*Photons)[i].Phi() );
    //    cout<<jentry<<" i:"<<i<<" phoSize:"<<Photons->size()<<" Pt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" otherP:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" dR:"<<dR<<endl;
    cout<<"PhotonsPt:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<endl;
  }
  cout<<"bestPhoton Pt: "<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" E: "<<bestPhoton.Energy()<<endl;
  cout<<"Muons:"<<endl;
  for(int i=0;i<Muons->size();i++){
    cout<<"MuonPt: "<<(*Muons)[i].Pt()<<" Eta: "<<(*Muons)[i].Eta()<<" Phi: "<<(*Muons)[i].Phi()<<" M: "<<(*Muons)[i].M()<<endl;
  }
  cout<<"Electrons:"<<endl;
  for(int i=0;i<Electrons->size();i++){
    cout<<"ElePt: "<<(*Electrons)[i].Pt()<<" Eta: "<<(*Electrons)[i].Eta()<<" Phi: "<<(*Electrons)[i].Phi()<<" M: "<<(*Electrons)[i].M()<<endl;
  }
  cout<<"Jets:"<<endl; 
  for(int i=0;i<Jets->size();i++){
    cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<endl;
  }
  //------------------------- MC only -------------------------------------------------
  /*for(int i=0;i<GenJets->size();i++){
    cout<<"GenJetPt:"<<(*GenJets)[i].Pt()<<" JetEta:"<<(*GenJets)[i].Eta()<<" JetPhi:"<<(*GenJets)[i].Phi()<<endl;
  }
  
  for(int i=0;i<GenParticles->size();i++){
    // cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
  }*/
  //-------------------------------------------------------------------------
  cout<<"^^^^^^^^^^^^^^^^^^ Event ends ^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl<<endl;
}
