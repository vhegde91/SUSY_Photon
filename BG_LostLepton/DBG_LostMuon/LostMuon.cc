#define LostMuon_cxx
#include "LostMuon.h"
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

  LostMuon ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void LostMuon::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  string s_data=data;
  
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  
  int evtSurvived=0;
  //get 2d histogram========================================
  TFile *f_LP=new TFile("LstMu_CS_TTWZ_LostMuHadTau_v2.root");
  //  TFile *f_LP=new TFile("LstMu_LDP_CS_TTWZ_LostMuHadTau_v2.root");
  //  TFile *f_LP=new TFile("LstMu_CS_TTWZ_HadTauOnly_v2.root");
  //TFile *f_LP=new TFile("LstMu_CS_TTWZ_LostMuOnly_v2.root");

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
    if(Electrons->size()>0) continue;//veto electrons for lost muon estimation
    if(Muons->size()>1) continue;//exactly one RECO muon or no muon(lost)
    if(isoElectronTracks!=0 || isoPionTracks!=0) continue;
    double mt_mu=0,mt_pho=0,mt_mupho=0;
    if(Muons->size()==1){
      mt_mu=sqrt(2*(*Muons)[0].Pt()*MET*(1-cos(DeltaPhi(METPhi,(*Muons)[0].Phi()))));
      if(mt_mu>100) continue;
      if( ((*Muons)[0].Pt() < 10) || abs((*Muons)[0].Eta()) > 2.4 ) continue;
      //      if(!(*Muons_tightID)[0]) continue;
    }
    mt_pho=sqrt(2*bestPhoton.Pt()*MET*(1-cos(DeltaPhi(METPhi,bestPhoton.Phi()))));
    if(Muons->size()==1) mt_mupho=sqrt(2*(bestPhoton+(*Muons)[0]).Pt()*MET*(1-cos(DeltaPhi(METPhi,(bestPhoton+(*Muons)[0]).Phi()))));
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
    //    if(dphiPho_MET<0.5) continue;
    //+++++++++++++++++++++++++ data only +++++++++++++++++++++++++++++
    wt=1;
    if((Muons->size()==0) && (Electrons->size()==0)) continue;//0-lepton is SR. Blind this region.
    //Trigger related
    bool passHT600Photon90Trigger = false;//(ST>700 && bestPhoton.Pt()>100);
    bool passPhoton165HE10Trigger = false;//(bestPhoton.Pt()>180);
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back();
      if( trgName=="HLT_Photon90_CaloIdL_PFHT600_v" && (*TriggerPass)[i]==1 ) passHT600Photon90Trigger = true;
      else if( trgName=="HLT_Photon165_HE10_v" && (*TriggerPass)[i]==1 ) passPhoton165HE10Trigger = true;
    }
    if(!(passPhoton165HE10Trigger || passHT600Photon90Trigger)) continue;
    //++++++++++++++++++++++ data only ends +++++++++++++++++++++++++++

    //---------------------- MC only -------------------------
    /*  wt=Weight*1000.0*lumiInfb;
    //Trigger related
    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) )  continue;
    wt=wt*0.98;
    //done with trigger efficiencies

    TLorentzVector genPho1,genMu1,neutr;
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
	  if(genMu1.Pt() < ((*GenParticles)[i]).Pt()) genMu1 = ((*GenParticles)[i]);
	}
	if( (abs((*GenParticles_PdgId)[i])==14) && (abs((*GenParticles_ParentId)[i])<=25) && (*GenParticles_Status)[i]==1 ){
	  neutr = ((*GenParticles)[i]);
	}
      }
    }
    double invMmuPhoNu=0.0;
    invMmuPhoNu = (neutr+genMu1+genPho1).M();

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
    vector<TLorentzVector> genMu;   
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if( abs((*GenParticles_PdgId)[i])==13 && (abs((*GenParticles_ParentId)[i])<=24) && ((*GenParticles_Status)[i]==1) ) {
	  nGenMu++; genMu.push_back((*GenParticles)[i]);
	}//muons
	else if( abs((*GenParticles_PdgId)[i])==11 && (abs((*GenParticles_ParentId)[i])<=24) && ((*GenParticles_Status)[i]==1) ) {nGenEle++;}//electrons
	else if( abs((*GenParticles_PdgId)[i])==15 && (abs((*GenParticles_ParentId)[i])<=24) ) {nGenTau++;}//taus
      }
    }
    if(nGenMu==0 && nGenEle==0 && nGenTau==0) continue;//to reject W->qq' type of events

    if(Muons->size()==0){
      if(isoMuonTracks!=0 || isoElectronTracks!=0 || isoPionTracks!=0) continue;
      //      if(nGenMu==0) continue;//uncomment this line to do only lost muon, w/o including hadronic tau decays
      bool temp1=true;
      if(nGenMu>0) temp1=true; //comment out "if(nGenMu==0) continue;" and then 
                               //set  temp1=true for lost muon + had tau. set temp1=false for only hadtau.
      else if(nGenEle>0) temp1=false;
      else temp1=true;
      if(temp1==false) continue;
    }
    if(nGenMu==0) {TLorentzVector v1;genMu.push_back(v1);}
    sortTLorVec(&genMu);
    //check if the photon is real or fake
    bool realPho=true;
    int matche=0,matchp=0;
    double minDR_Pho_GenObj=1000;
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
	if(dr1 < 0.1 && (abs((*GenParticles_PdgId)[i])==11) && (abs((*GenParticles_ParentId)[i])<=24) ) {matche=1;realPho=false;}
	if(minDR_Pho_GenObj > dr1) minDR_Pho_GenObj=dr1;
	//	if((minDR_Pho_GenObj > genPho1.DeltaR((*GenParticles)[i])) && (abs((*GenParticles_PdgId)[i])==21) ) minDR_Pho_GenObj=genPho1.DeltaR((*GenParticles)[i]);
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
    if(Muons->size()==0){
      if(matche==1 && matchp==0) continue;//if reco photon matched to gen e and not matched to gen photon, it is fake.
    }//photon has been identified. It is a real photon and it is matched to gen photon with dR(genPho,RecoPho) < 0.1 and Pts are within 10%.
    //---------------------- MC only ends-------------------------
*/
    //    if(MET>200) continue;
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
      //      h_madHT->Fill(madHT,wt);//MC only
      h_nVtx->Fill(NVtx,wt);
      //      findObjMatchedtoG(bestPhoton);//MC only
      //      h_madMinPhotonDeltaR->Fill(madMinPhotonDeltaR,wt);//MC only
      //      h_mindRPhoLep->Fill(gendRLepPho,wt);//MC only
      int nMuMultJ=0;
      for(int i=0;i<Jets->size();i++){
	if((*Jets)[i].Pt()>30.0){ nMuMultJ = nMuMultJ + (*Jets_muonMultiplicity)[i]; }
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
      int sBin4=-100,m_i4=0;
      if(BTags==0){
        if(nHadJets>=2 && nHadJets<=4)     { sBin4=0;}
	else if(nHadJets==5 || nHadJets==6){ sBin4=8;}
        else if(nHadJets>=7)               { sBin4=15;}
      }
      else{
        if(nHadJets>=2 && nHadJets<=4)     { sBin4=22;}
        else if(nHadJets==5 || nHadJets==6){ sBin4=29;}
        else if(nHadJets>=7)               { sBin4=36;}
      }
      if(sBin4==0){
        for(int i=0;i<METBinLowEdgeV4_njLow.size()-1;i++){
          if(METBinLowEdgeV4_njLow[i]<99.99) continue;
          m_i4++;
          if(MET >= METBinLowEdgeV4_njLow[i] && MET < METBinLowEdgeV4_njLow[i+1]){ sBin4 = sBin4+m_i4;break; }
          else if(MET >= METBinLowEdgeV4_njLow[METBinLowEdgeV4_njLow.size()-1])  { sBin4 = 8         ;break; }
	}
      }
      else{
        for(int i=0;i<METBinLowEdgeV4.size()-1;i++){
          if(METBinLowEdgeV4[i]<99.99) continue;
          m_i4++;
          if(MET >= METBinLowEdgeV4[i] && MET < METBinLowEdgeV4[i+1]){ sBin4 = sBin4+m_i4;break; }
          else if(MET >= METBinLowEdgeV4[METBinLowEdgeV4.size()-1])  { sBin4 = sBin4+7   ;break; }
	}
      }
      //      if(Muons->size()==0){//MC only
      //+++++++++++++++++++++++++ data only +++++++++++++++++++++++++++++
      if(Muons->size()==1){
	if(do_prediction){
	  double parX=MET,parY=nHadJets;
	  double tf=0;
	  TString name;
	  // if(nHadJets==2 || nHadJets==3)      name="LostProb_1";
	  // else if(nHadJets==4)                name="LostProb_2";
	  // else if(nHadJets==5 || nHadJets==6) name="LostProb_3";
	  // else if(nHadJets>=7)                name="LostProb_4";
	  if(BTags==0)      name="LostProb_0";
	  else if(BTags>=1) name="LostProb_1";
	  //	  else if(BTags>=2) name="LostProb_2";
	  h2_LP=(TH2D*)f_LP->FindObjectAny(name);
	  if(h2_LP) tf=h2_LP->GetBinContent(h2_LP->FindBin(parX,parY));
	  else cout<<"hist not found"<<endl;
	  wt=tf*wt;
	}
	//++++++++++++++++++++++ data only ends +++++++++++++++++++++++++++	
	h_nVtx_Mu0->Fill(NVtx,wt);
	h_ST_Mu0->Fill(ST,wt);
	h_MET_Mu0->Fill(MET,wt);
	h_nHadJets_Mu0->Fill(nHadJets,wt);
	h_BTags_Mu0->Fill(BTags,wt);
	h_METvBin_Mu0->Fill(MET,wt);
	h_BestPhotonPt_Mu0->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_Mu0->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_Mu0->Fill(bestPhoton.Phi(),wt);
	h_mTpho_Mu0->Fill(mt_pho,wt);

	h_isoMuonTracks_Mu0->Fill(isoMuonTracks,wt);
	h_MuMultInJets_Mu0->Fill(nMuMultJ,wt);

	h_dPhi_PhoMET_Mu0->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_Mu0->Fill(dphi1,wt);
	h_dphi_METjet2_Mu0->Fill(dphi2,wt);

	h_Jet1Pt_Mu0->Fill(hadJets[0].Pt(),wt);
	h_Jet2Pt_Mu0->Fill(hadJets[1].Pt(),wt);
	h_isoEleTrack_Mu0->Fill(isoElectronTracks,wt);
	h_isoMuTrack_Mu0->Fill(isoMuonTracks,wt);
	h_isoPiTrack_Mu0->Fill(isoPionTracks,wt);
	//-----------------------MC only ------------------------------
	/*	h_nGenEle_Mu0->Fill(nGenEle,wt);//MC only
	h_nGenMu_Mu0->Fill(nGenMu,wt);//MC only
	h_nGenTau_Mu0->Fill(nGenTau,wt);//MC only

	h_GenMuEta_Mu0->Fill(genMu[0].Eta(),wt);//MC only
	h_GenMuPt_Mu0->Fill(genMu[0].Pt(),wt);//MC only
	h_photonPrompt_Mu0->Fill(!((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]),wt);//MC only
	h_gendRLepPho_Mu0->Fill(gendRLepPho,wt);//MC only
	h_madMinPhotonDeltaR_Mu0->Fill(madMinPhotonDeltaR,wt);//MC only

	h_nGenEleFmTau_Mu0->Fill(nGenEleFmTau,wt);//MC only
	h_nGenMuFmTau_Mu0->Fill(nGenMuFmTau,wt);//MC only
	if(matche==1 && matchp==0) h_fakePhoPt_Mu0->Fill(bestPhoton.Pt(),wt);//MC only
	for(int i=0;i<GenParticles->size();i++){
	  if(abs((*GenParticles_PdgId)[i]) == 13){
	    h_genMuParent_Mu0->Fill(abs((*GenParticles_ParentId)[i]),wt);//MC only
	  }
	}*/
	//-----------------------------------------------------
	//print(jentry);
	h2_METnHadJ_Mu0->Fill(MET,nHadJets,wt);
	h2_dPhi1nHadJ_Mu0->Fill(dphi1,nHadJets,wt);
	h2_dPhi2nHadJ_Mu0->Fill(dphi2,nHadJets,wt);
	h2_dPhi1MET_Mu0->Fill(dphi1,MET,wt);
	h2_STMET_Mu0->Fill(ST,MET,wt);
	h2_dPhi1dPhi2_Mu0->Fill(dphi1,dphi2,wt);
	h2_STHadJ_Mu0->Fill(ST,nHadJets,wt);
	h2_METJet1Pt_Mu0->Fill(MET,hadJets[0].Pt(),wt);
	h2_R_PhoPtJetPtVsDR_Mu0->Fill(minDR,((*Jets)[phoMatchingJetIndx].Pt())/bestPhoton.Pt(),wt);

	h3_STMETnHadJ_Mu0->Fill(ST,MET,nHadJets,wt);
	h2_hadJbTag_Mu0->Fill(nHadJets,BTags,wt);

	//	if(ST>5000 || MET>5000) cout<<"ST "<<ST<<" MET "<<MET<<endl;
	if(nHadJets==2 || nHadJets==3)      h2_STMET_NJ2or3_Mu0  ->Fill(ST,MET,wt);
	else if(nHadJets==4)                h2_STMET_NJ4_Mu0     ->Fill(ST,MET,wt);
	else if(nHadJets==5 || nHadJets==6) h2_STMET_NJ5or6_Mu0  ->Fill(ST,MET,wt);
	else if(nHadJets>=7)                h2_STMET_NJ7toInf_Mu0->Fill(ST,MET,wt);

	if(BTags==0)      h2_METNJ_0b_Mu0->Fill(MET,nHadJets,wt);
	else if(BTags>=1) h2_METNJ_1b_Mu0->Fill(MET,nHadJets,wt);
	//	else if(BTags>=2) h2_METNJ_m2b_Mu0->Fill(MET,nHadJets,wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_Mu0_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! ";
	if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2_Mu0[0]->Fill(MET,wt);
	else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2_Mu0[1]->Fill(MET,wt);
        else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2_Mu0[2]->Fill(MET,wt);
        else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2_Mu0[3]->Fill(MET,wt);
        else if(BTags>=2)                                   h_MET_R_v2_Mu0[4]->Fill(MET,wt);
	h_SBins_Mu0->Fill(sBin2,wt);
	h_SBins_v4_Mu0->Fill(sBin4,wt);
	wt=wt_org;
      }//0 muon + photon events
      if(Muons->size()==1){
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
	  else if(BTags>=1) name="LostProb_1";
	  //	  else if(BTags>=2) name="LostProb_2";
	  h2_LP=(TH2D*)f_LP->FindObjectAny(name);
	  if(h2_LP) tf=h2_LP->GetBinContent(h2_LP->FindBin(parX,parY));
	  else cout<<"hist not found"<<endl;
	  wt=tf*wt;
	}*/
	//----------------------------------------------------------------
	h_nVtx_Mu1->Fill(NVtx,wt);
	h_ST_Mu1->Fill(ST,wt);
	h_MET_Mu1->Fill(MET,wt);
	h_nHadJets_Mu1->Fill(nHadJets,wt);
	h_BTags_Mu1->Fill(BTags,wt);
	h_METvBin_Mu1->Fill(MET,wt);
	h_BestPhotonPt_Mu1->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_Mu1->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_Mu1->Fill(bestPhoton.Phi(),wt);
	h_mTpho_Mu1->Fill(mt_pho,wt);

	h_isoMuonTracks_Mu1->Fill(isoMuonTracks,wt);
	h_MuMultInJets_Mu1->Fill(nMuMultJ,wt);
        h_invMassPhoMu->Fill((bestPhoton+(*Muons)[0]).M(),wt);

	h_MuPt->Fill((*Muons)[0].Pt(),wt);
	h_MuEta->Fill((*Muons)[0].Eta(),wt);
	h_MuPhi->Fill((*Muons)[0].Phi(),wt);
	h_mTmu->Fill(mt_mu,wt);

	h_dPhi_PhoMET_Mu1->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_Mu1->Fill(dphi1,wt);
	h_dphi_METjet2_Mu1->Fill(dphi2,wt);
	h_dPhi_Muon_Photon->Fill(abs(bestPhoton.DeltaPhi((*Muons)[0])),wt);

	h_Jet1Pt_Mu1->Fill(hadJets[0].Pt(),wt);	
	h_Jet2Pt_Mu1->Fill(hadJets[1].Pt(),wt);	
	h_isoEleTrack_Mu1->Fill(isoElectronTracks,wt);
	h_isoMuTrack_Mu1->Fill(isoMuonTracks,wt);
	h_isoPiTrack_Mu1->Fill(isoPionTracks,wt);
	h_dR_MuPho->Fill(bestPhoton.DeltaR((*Muons)[0]),wt);
	h_mTPhoMuMET->Fill(mt_mupho,wt);
	//-------------------------------------- MC only -------------------------------------
	/*	h_GenMuPt_Mu1->Fill(genMu[0].Pt(),wt);//MC only
	h_photonPrompt_Mu1->Fill(!((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]),wt);//MC only
	h_nGenEle_Mu1->Fill(nGenEle,wt);//MC only
	h_nGenMu_Mu1->Fill(nGenMu,wt);//MC only
	h_nGenTau_Mu1->Fill(nGenTau,wt);//MC only
	
	h_gendRLepPho_Mu1->Fill(gendRLepPho,wt);//MC only
	h_madMinPhotonDeltaR_Mu1->Fill(madMinPhotonDeltaR,wt);//MC only

	h_nGenEleFmTau_Mu1->Fill(nGenEleFmTau,wt);//MC only
	h_nGenMuFmTau_Mu1->Fill(nGenMuFmTau,wt);//MC only
	h_invMassMuPhoNu->Fill(invMmuPhoNu,wt);//MC only
	if(matche==1 && matchp==0) h_fakePhoPt_Mu1->Fill(bestPhoton.Pt(),wt);//MC only
	h2_GenMuPtVsRECOMuPt->Fill(genMu[0].Pt(),(*Muons)[0].Pt(),wt);//MC only
	*/
	//-------------------------------------------------------------------------------------
	h2_STvsMuPt_Mu1->Fill((*Muons)[0].Pt(),ST,wt);
	h2_METvsMuPt_Mu1->Fill((*Muons)[0].Pt(),MET,wt);

	h2_METnHadJ_Mu1->Fill(MET,nHadJets,wt);
	h2_dPhi1nHadJ_Mu1->Fill(dphi1,nHadJets,wt);
	h2_dPhi2nHadJ_Mu1->Fill(dphi2,nHadJets,wt);
	h2_dPhi1MET_Mu1->Fill(dphi1,MET,wt);
	h2_STMET_Mu1->Fill(ST,MET,wt);
	h2_dPhi1dPhi2_Mu1->Fill(dphi1,dphi2,wt);
	h2_STHadJ_Mu1->Fill(ST,nHadJets,wt);
	h2_METJet1Pt_Mu1->Fill(MET,hadJets[0].Pt(),wt);
	h2_RecoMuPtRecoAct_Mu1->Fill((*Muons)[0].Pt(),(*Muons_MT2Activity)[0],wt);
	h2_R_PhoPtJetPtVsDR_Mu1->Fill(minDR,((*Jets)[phoMatchingJetIndx].Pt())/bestPhoton.Pt(),wt);

	h3_STMETnHadJ_Mu1->Fill(ST,MET,nHadJets,wt);
	
	h2_hadJbTag_Mu1->Fill(nHadJets,BTags,wt);
	if(nHadJets==2 || nHadJets==3)      h2_STMET_NJ2or3_Mu1  ->Fill(ST,MET,wt);
	else if(nHadJets==4)                h2_STMET_NJ4_Mu1     ->Fill(ST,MET,wt);
	else if(nHadJets==5 || nHadJets==6) h2_STMET_NJ5or6_Mu1  ->Fill(ST,MET,wt);
	else if(nHadJets>=7)                h2_STMET_NJ7toInf_Mu1->Fill(ST,MET,wt);

	if(BTags==0)      h2_METNJ_0b_Mu1->Fill(MET,nHadJets,wt);
	else if(BTags>=1) h2_METNJ_1b_Mu1->Fill(MET,nHadJets,wt);
	//	else if(BTags>=2) h2_METNJ_m2b_Mu1->Fill(MET,nHadJets,wt);
	int minDRmuIndx = -100;
	double minDRmu=1000.0;
	for(int i=0;i<Jets->size();i++){
	  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
	    double dR=(*Muons)[0].DeltaR((*Jets)[i]);
	    if(dR<minDRmu){minDRmu=dR;minDRmuIndx=i;}
	  }
	}
	h2_R_MuPtJetPtVsDR->Fill( minDRmu,(((*Jets)[minDRmuIndx].Pt())/((*Muons)[0].Pt())),wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_Mu1_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! "<<searchRegion;

	if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2_Mu1[0]->Fill(MET,wt);
	else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2_Mu1[1]->Fill(MET,wt);
        else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2_Mu1[2]->Fill(MET,wt);
        else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2_Mu1[3]->Fill(MET,wt);
        else if(BTags>=2)                                   h_MET_R_v2_Mu1[4]->Fill(MET,wt);
	h_SBins_Mu1->Fill(sBin2,wt);
	h_SBins_v4_Mu1->Fill(sBin4,wt);
	wt=wt_org;
      }//muon + photon events
    }
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
}


TLorentzVector LostMuon::getBestPhoton(){
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


bool LostMuon::check_eMatchedtoGamma(){
  if(bestPhotonIndxAmongPhotons>=0){
    for(int i=0;i<Electrons->size();i++){
      if( (*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i] ) < 0.1){
	//	cout<<(*Electrons)[i].Pt()<<" "<<(*Electrons)[i].Eta()<<" "<<(*Electrons)[i].Phi()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Pt()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Eta()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Phi()<<" dR:"<<(*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i])<<endl;
	return true;
      }
    }
  }
  else
    return false;
}

double LostMuon::getGendRLepPho(){//MC only
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
  }*/ //for
  if(genPho1.Pt() > 0. && genLep1.Pt() > 0.) return genLep1.DeltaR(genPho1);
  else return 1000.0;
}

void  LostMuon::findObjMatchedtoG(TLorentzVector bestPhoton){//MC only
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
  //find obj matched to muon
  dR=100;match=-100;
  if(Muons->size()==1){
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if(i==0){dR=(*GenParticles)[i].DeltaR((*Muons)[0]);}
	else if(dR > ((*GenParticles)[i].DeltaR( (*Muons)[0]) ) ){
	  dR= (*GenParticles)[i].DeltaR((*Muons)[0]);
	  match=i;
	}
      }
    }
    if(dR<0.1){
      h_MuMatchedObj->Fill(abs((*GenParticles_PdgId)[match]),wt);
      if(abs((*GenParticles_PdgId)[match])==13){
	h_PdgIdMuParent->Fill(abs((*GenParticles_ParentId)[match]),wt);
      }
    }
    else{
      h_MuMatchedObj->Fill(0.0,wt);
      h_PdgIdMuParent->Fill(0.0,wt);
    }
  }
  */  
}



void LostMuon::print(Long64_t jentry){
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
  /*  for(int i=0;i<GenJets->size();i++){
    cout<<"GenJetPt:"<<(*GenJets)[i].Pt()<<" JetEta:"<<(*GenJets)[i].Eta()<<" JetPhi:"<<(*GenJets)[i].Phi()<<endl;
  }
  
  for(int i=0;i<GenParticles->size();i++){
    // cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
  }*/
  //-------------------------------------------------------------------------
  cout<<"^^^^^^^^^^^^^^^^^^ Event ends ^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl<<endl;
}
