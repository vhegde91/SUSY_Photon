#define ZGamma_cxx
#include "ZGamma.h"
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

  ZGamma ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void ZGamma::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  string s_data=data;
  bool mcSample=true;
  if(s_data=="Data") mcSample=false;    
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  
  int evtSurvived=0;
  TFile *f_TF = new TFile("TF_CS_ZDYToLLG.root");
  //  TFile *f_TF = new TFile("TF_CS_ZGToNuNuG_PtG130_LO.root");
  TH1D *h_TF=(TH1D*)f_TF->FindObjectAny("Ratio_NuNuToLL");

  bool do_prediction=0,reweightLO=0;
  cout<<"Doing prediction from ZToLL sample from file |"<<f_TF->GetName()<<"|?"<<do_prediction<<endl;
  TFile* pufile = TFile::Open("PileupHistograms_0121_69p2mb_pm4p6.root","READ");
  //choose central, up, or down
  TH1* puhist = (TH1*)pufile->Get("pu_weights_down");
  
  TFile *f_ewCorr = TFile::Open("ewk_corr.root");
  TH1D *h_ewCorr = (TH1D*)f_ewCorr->FindObjectAny("znng-130-o");

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
    wt=Weight*lumiInfb*1000.0*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));
    //cout<<CrossSection<<" NEvt:"<<NumEvents<<" Wt:"<<Weight<<endl;
    bool process=true;
    if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0)) continue;

    //    About photons
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhotonIndxAmongPhotons<0) continue;
    bool bestPhoHasPxlSeed=true;
    if((*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons]<0.001) bestPhoHasPxlSeed=false;
    if( bestPhoHasPxlSeed ) continue;  
    bool eMatchedG=check_eMatchedtoGamma();
    if(eMatchedG) continue;
    if(check_muMatchedtoGamma()) continue;
    //cout<<"BestPhotonPt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<endl<<endl;
    /*
    double genPho1Pt=0.;
    for(int i=0;i<GenParticles->size();i++){
      if( (abs((*GenParticles_PdgId)[i])==22)
	  && ((abs((*GenParticles_ParentId)[i]) <= 100) ) 
	  && ( ((*GenParticles_Status)[i]==1) || (((*GenParticles_Status)[i]/10)==2) )){
	if(genPho1Pt < (*GenParticles)[i].Pt()) 
	  genPho1Pt = (*GenParticles)[i].Pt();
      }
    }*/
    TLorentzVector genPho1,genEle1,neutr;
    bool genPromt1=false;
    int leadGenPhoIdx=-100,nGenbs=0;
    vector<int> bParent;
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
        if((abs((*GenParticles_PdgId)[i])==22) && ((abs((*GenParticles_ParentId)[i])<=6) || ((*GenParticles_ParentId)[i]==2212) || (abs((*GenParticles_ParentId)[i])==21)) && (*GenParticles_Status)[i]==1 ){
          if(genPho1.Pt() < (*GenParticles)[i].Pt()){
            leadGenPhoIdx = i;
            genPho1 = ((*GenParticles)[i]);
          }
        }
	if( abs((*GenParticles_PdgId)[i])==5 ){ nGenbs++;bParent.push_back(abs((*GenParticles_ParentId)[i]));}
      }
    }
    if(leadGenPhoIdx>=0) genPromt1=true;
    
    gendRLepPho=1000;
    gendRLepPho = getGendRLepPho();

    h_madMinPhotonDeltaR->Fill(madMinPhotonDeltaR,wt);
    h_MinPhoLepDR->Fill(gendRLepPho,wt);

    TLorentzVector zvec;
    double zmass= -10;
    if((s_data=="ZGToLLG") || (s_data=="DYJetsToLL")){
      if(jentry<3) cout<<"Processing as Z(G)->LL(G) Decay"<<endl;
      if( (Electrons->size()==2) ){
	if( (Muons->size()!=0) ) continue;//exactly 0 RECO muons and 2 RECO e
	if(isoPionTracks!=0 || isoMuonTracks!=0) continue;
	if( (*Electrons_charge)[0] == (*Electrons_charge)[1] ) continue;//opp sign same flav(OSSF)
	zvec =  ((*Electrons)[0] + (*Electrons)[1]);
	zmass = zvec.M();
      }
      else if( (Muons->size()==2) ){
	if(Electrons->size()!=0 ) continue;//exactly 2 RECO muons and 0 RECO e
	if(isoPionTracks!=0 || isoElectronTracks!=0) continue;
	if( (*Muons_charge)[0] == (*Muons_charge)[1] ) continue;//opp sign same flav(OSSF)
	zvec =  ((*Muons)[0] + (*Muons)[1]);
	zmass = zvec.M();
      }
      else continue;
      if(zvec.M() < 80 || zvec.M() > 100) continue;
      if(MET > 100) continue;
    }
    TLorentzVector metvec;
    metstar = metvec;
    metvec.SetPtEtaPhiE(MET,0.0,METPhi,0.0);
    metstar = metvec + zvec;
    if((s_data=="ZGToNuNuG") || (s_data=="ZJetsToNuNu")){
      if(jentry<3) cout<<"Processing as Z(G)->NuNu(G) Decay"<<endl;
      if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;
      if(Electrons->size()!=0 || Muons->size()!=0) continue;
      do_prediction = false;
    }

    if((s_data=="ZJetsToNuNu") || (s_data=="DYJetsToLL")){
      if(jentry<3) cout<<"Processing with non-prompt photons"<<endl;
      if(hasGenPromptPhoton && madMinPhotonDeltaR > 0.5 && gendRLepPho > 0.5) continue;
      // if(hasGenPromptPhoton) continue;
    }
    else if((s_data=="ZGToLLG") || (s_data=="ZGToNuNuG")){
      if(jentry<3) cout<<"Processing with prompt photons"<<endl;
      //      if(!hasGenPromptPhoton) continue;
      //      if(!genPromt1) continue;
    }

    //Trigger selections
    bool passHT600Photon90Trigger = false;//(ST>700 && bestPhoton.Pt()>100);
    bool passPhoton165HE10Trigger = false;//(bestPhoton.Pt()>180);
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back();
      if( trgName=="HLT_Photon90_CaloIdL_PFHT600_v" && (*TriggerPass)[i]==1 ) passHT600Photon90Trigger = true;
      else if( trgName=="HLT_Photon165_HE10_v" && (*TriggerPass)[i]==1 ) passPhoton165HE10Trigger = true;
      // cout<<(*TriggerNames)[i]<<" "<<(*TriggerPass)[i]<<" "<<(*TriggerPrescales)[i]<<endl;
    }

    h_GenPho1Pt->Fill(genPho1.Pt(),wt); 
    
    //calulate ST and HadJets by cleaning the matching jet.
    bool hadJetID=true;
    int minDRindx=-100,phoMatchingJetIndx=-100,nHadJets=0, minDRindxl1=-100, minDRindxl2=-100;
    double minDR=99999, ST=0, remJetPt=0, minDRl1=10000, minDRl2=10000,mt_Pho=0.;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= HT_EtaCut) ){
	double dR=bestPhoton.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
	if(Muons->size()==2){
	  if( (*Muons)[0].DeltaR((*Jets)[i]) < minDRl1 ){ minDRl1 = (*Muons)[0].DeltaR((*Jets)[i]); minDRindxl1=i;}
	  if( (*Muons)[1].DeltaR((*Jets)[i]) < minDRl2 ){ minDRl2 = (*Muons)[1].DeltaR((*Jets)[i]); minDRindxl2=i;}
	}
	else if(Electrons->size()==2){
	  if( (*Electrons)[0].DeltaR((*Jets)[i]) < minDRl1 ){ minDRl1 = (*Electrons)[0].DeltaR((*Jets)[i]); minDRindxl1=i;}
	  if( (*Electrons)[1].DeltaR((*Jets)[i]) < minDRl2 ){ minDRl2 = (*Electrons)[1].DeltaR((*Jets)[i]); minDRindxl2=i;}
	}
      }
    }//loop over jets
    if( minDR > 0.3 )  minDRindx   = -100;
    if( minDRl1 > 0.3) minDRindxl1 = -100;
    if( minDRl2 > 0.3) minDRindxl2 = -100;
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <=  HT_EtaCut) ){
	if((i!=minDRindx) && (i!=minDRindxl1) && (i!=minDRindxl2)){
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
    
    double genJdR1=10000,genJdR2=10000;
    int genJdR1Idx=-100,genJdR2Idx=-100;
    for(int i=0;i<GenJets->size();i++){
      if(genJdR1 > bestPhoton.DeltaR((*GenJets)[i])){
	genJdR1=bestPhoton.DeltaR((*GenJets)[i]);
	genJdR1Idx=i;
      }
    }
    for(int i=0;i<GenJets->size();i++){
      if(i==genJdR1Idx) continue;
      if(genJdR2 > bestPhoton.DeltaR((*GenJets)[i])){
	genJdR2=bestPhoton.DeltaR((*GenJets)[i]);
	genJdR2Idx=i;
      }
    }
    //ST and HadJets have been determined. Now calulate dPhi b/w MET and leading HadJets.
    double dphi1=3.8,dphi2=3.8,dphi3=3.8,dphi4=3.8,dphiPho_MET=3.8,dphi_PhoLep1=3.8,dphi_PhoLep2=3.8,dRphoClstLep=1000.0;
    if(bestPhoton.Pt()>0.1) dphiPho_MET=abs(DeltaPhi(metstar.Phi(),bestPhoton.Phi()));
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi(metstar.Phi(),(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi(metstar.Phi(),(hadJets)[1].Phi()));
    if(hadJets.size() > 2 ) dphi3 = abs(DeltaPhi(metstar.Phi(),(hadJets)[2].Phi()));
    if(hadJets.size() > 3 ) dphi4 = abs(DeltaPhi(metstar.Phi(),(hadJets)[3].Phi()));
    mt_Pho = sqrt(2*bestPhoton.Pt()*metstar.Pt()*(1-cos(DeltaPhi(metstar.Phi(),bestPhoton.Phi()))));

    if(Muons->size()==2) { 
      dphi_PhoLep1 = abs(bestPhoton.DeltaPhi((*Muons)[0]));
      dphi_PhoLep2 = abs(bestPhoton.DeltaPhi((*Muons)[1]));
      for(int i=0;i<Muons->size();i++){
	if(dRphoClstLep > bestPhoton.DeltaR((*Muons)[i])) dRphoClstLep = bestPhoton.DeltaR((*Muons)[i]);
      }
    }
    else if(Electrons->size()==2){
      dphi_PhoLep1 = abs(bestPhoton.DeltaPhi((*Electrons)[0]));
      dphi_PhoLep2 = abs(bestPhoton.DeltaPhi((*Electrons)[1]));
      for(int i=0;i<Electrons->size();i++){
	if(dRphoClstLep > bestPhoton.DeltaR((*Electrons)[i])) dRphoClstLep = bestPhoton.DeltaR((*Electrons)[i]);
      }
    }

    if(phoMatchingJetIndx>=0 && ((*Jets)[phoMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    if(phoMatchingJetIndx<0) continue;
    
    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    wt=wt*0.98;
    //apply baseline selections
    //    process = process && ST>500 && MET > 100 && nHadJets >=2 && dphi1 > 0.3 && dphi2 > 0.3 && bestPhoton.Pt() > 100;
    //process = process && ST>500 &&  nHadJets >=2 && bestPhoton.Pt() > 100;//&& metstar.Pt() > 100 && dphi1 > 0.3 && dphi2 > 0.3 ;
    //    if(MET>200) continue;
    process = process && ST>500 && metstar.Pt()>10 && nHadJets >= 2 && (dphi1 > 0.3 && dphi2 > 0.3) && bestPhoton.Pt() > 100;
    //process = process && ST>500 && nHadJets >=2 && bestPhoton.Pt() > 100;
    if(bestPhoton.Pt() < 190) continue;
    //    if(BTags!=0) continue;
    //------------------ NNLO corr --------------------
    //    if(genPho1.Pt() < 175) continue;
    //    double ewCorr = h_ewCorr->GetBinContent(h_ewCorr->FindBin(bestPhoton.Pt()));
    //    if(bestPhoton.Pt()>1000) ewCorr = 0.5879;
    // double qcdCorr = 0.;
    // if(bestPhoton.Pt() < 190) qcdCorr = 1.44;
    // else if(bestPhoton.Pt() < 250) qcdCorr = 1.41;
    // else if(bestPhoton.Pt() < 400) qcdCorr = 1.35;
    // else if(bestPhoton.Pt() < 700) qcdCorr = 1.29;
    // else qcdCorr = 1.15;
    //    wt=wt*1.44*ewCorr;
    //    wt=wt*qcdCorr;//for LO sample
    //    wt=wt*1.14;//for NLO sample
    //--------------------------------------
    if(process && hadJetID){
      evtSurvived++;
      h_RunNum->Fill(RunNum);
      h_intLumi->Fill(lumiInfb);
      // for(int i=0;i<hadJets.size();i++){
      // 	cout<<"HadJetPt:"<<hadJets[i].Pt()<<" eta:"<<hadJets[i].Eta()<<" phi:"<<hadJets[i].Phi()<<endl;
      // }
      // cout<<"METOrg: "<<MET<<" METPhi:"<<METPhi<<" METClean:"<<METclean<<" METPhiClean:"<<METPhiclean<<endl;
      // cout<<"ZPt:"<<zvec.Pt()<<" eta:"<<zvec.Eta()<<" phi:"<<zvec.Phi()<<" M:"<<zvec.M()<<endl;
      // cout<<" MET* Pt:"<<metstar.Pt()<<" MET* !!Eta:"<<metstar.Eta()<<" MET* Phi:"<<metstar.Phi()<<" MET* !!Energy:"<<metstar.Energy()<<endl;
      // cout<<"BestPhotonPt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<endl<<endl;
      //print(jentry);
      double wt_org=wt,tf=0;
      if(do_prediction){
	//	double parX=metstar.Pt();
	double parX=BTags;
	double tf=0;
	if(h_TF){
	  //	  tf=(37.0/37.8433)*h_TF->GetBinContent(h_TF->FindBin(parX));//37 events in data. 37.8433 events in ZGLLG MC
	  tf=h_TF->GetBinContent(h_TF->FindBin(parX));
	  //	  cout<<"parX: "<<parX<<" TF: "<<tf<<" BinLowEdge: "<<h_TF->GetBinLowEdge(h_TF->FindBin(parX))<<" BinIndx: "<<(h_TF->FindBin(parX))<<endl;
	}
	else cout<<"hist not found"<<endl;
	wt=tf*wt;
      }
      if(reweightLO) wt=wt*h_TF->GetBinContent(h_TF->FindBin(nHadJets));      

      //-----------for NNNNN...LO correction syst---------
      // if(nHadJets==2) wt=wt*0.27*0.27;
      // else if(nHadJets==3) wt=wt*0.13*0.13;
      // else if(nHadJets==4) wt=wt*0.01;
      // else wt=wt*0.02*0.02;
      //-----------end NNNNN...LO correction syst---------
      
      int sBin2=-100,m_i=0;
      if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      sBin2 = 0;
      else if(nHadJets >= 5 && BTags==0)                  sBin2 = 7;
      else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) sBin2 = 14;
      else if(nHadJets >= 5 && BTags==1)                  sBin2 = 21;
      else if(BTags>=2)                                   sBin2 = 28;
      for(int i=0;i<METBinLowEdge.size()-1;i++){
        if(METBinLowEdge[i]<99.99) continue;
        m_i++;
        if(metstar.Pt()>=METBinLowEdge[i] && metstar.Pt()<METBinLowEdge[i+1]){ sBin2 = sBin2+m_i;break; }
        else if(metstar.Pt()>=METBinLowEdge[METBinLowEdge.size()-1]){ sBin2 = sBin2+7  ;break; }
      }
      if(BTags>=2 && sBin2==35) sBin2=34;

      int sBin4 = getBinNoV4(nHadJets),  sBin7 = getBinNoV7(nHadJets);
      //------------------------ Sbins----------------------------

      h_ST->Fill(ST,wt);
      h_MET->Fill(metstar.Pt(),wt);
      h_nHadJets->Fill(nHadJets,wt);
      h_BTags->Fill(BTags,wt);
      h_BestPhotonPt->Fill(bestPhoton.Pt(),wt);
      h_BestPhotonEta->Fill(bestPhoton.Eta(),wt);
      h_BestPhotonPhi->Fill(bestPhoton.Phi(),wt);
      h_BestPhotonSietaieta->Fill((*Photons_sigmaIetaIeta)[bestPhotonIndxAmongPhotons],wt);
      h_HT->Fill(HT,wt);
      h_MHT->Fill(MHT,wt);
      h_nJets->Fill(NJets,wt);
      h_nJetsClean->Fill(NJetsclean,wt);
      h_METvBin->Fill(metstar.Pt(),wt);
      h_METvBin_TF->Fill(metstar.Pt(),wt);
      h_STvBin->Fill(ST,wt);
      h_nBTagsvBin->Fill(BTags,wt);

      h_nVtx->Fill(NVtx,wt);
      h_ZMass->Fill(zmass,wt);
      h_ZPt->Fill(zvec.Pt(),wt);
      h_minDR_Pho_GenJ->Fill(genJdR1,wt);
      h_minDR2_Pho_GenJ->Fill(genJdR2,wt);

      h_METstar->Fill(metstar.Pt(),wt);
      h_METPhi->Fill(metstar.Phi(),wt);
      h_METOrg->Fill(MET,wt);
      h_METclean->Fill(METclean,wt);
      h_METPhiclean->Fill(METPhiclean,wt);

      h_dR_PhoClstLep->Fill(dRphoClstLep,wt);
      h_dPhi_PhoMET->Fill(dphiPho_MET,wt);
      h_mTPho->Fill(mt_Pho,wt);

      h_dphi_METjet1->Fill(dphi1,wt);
      h_dphi_METjet2->Fill(dphi2,wt);
      h_dphi_PhoLep1->Fill(dphi_PhoLep1,wt);
      h_dphi_PhoLep2->Fill(dphi_PhoLep2,wt);
      h2_METVsPhoPt->Fill(metstar.Pt(),bestPhoton.Pt(),wt);

      findObjMatchedtoG(bestPhoton);
      h_nGenbs->Fill(nGenbs,wt);
      for(int i=0;i<bParent.size();i++){
	if(bParent[i]==2212) bParent[i]=26;
      	h_bParent_PdgId->Fill(bParent[i],wt);
      }
      int searchRegion=0;
      if     (nHadJets >= 2 && nHadJets <= 4 ) searchRegion=1;
      else if(nHadJets == 5 || nHadJets == 6 ) searchRegion=2;
      else if(nHadJets >= 7                  ) searchRegion=3;
      
      //---------------- search bins -----------------------
      if( searchRegion > 0 && searchRegion < 4){
	h_MET_R[searchRegion-1]->Fill(metstar.Pt(),wt);
      }
      else cout<<"Event outside search region! "<<searchRegion;
      if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2[0]->Fill(metstar.Pt(),wt);
      else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2[1]->Fill(metstar.Pt(),wt);
      else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2[2]->Fill(metstar.Pt(),wt);
      else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2[3]->Fill(metstar.Pt(),wt);
      else if(BTags>=2)                                   h_MET_R_v2[4]->Fill(metstar.Pt(),wt);
      h_SBins->Fill(sBin2,wt);
      h_SBins_v4->Fill(sBin4,wt);
      h_SBins_v7->Fill(sBin7,wt);

      h2_SBinsv7VsnJ->Fill(sBin7,nHadJets,wt);
      //    if(Muons->size()==0){

      if(Muons->size()==2){
	h_ST_2Mu->Fill(ST,wt);
	h_MET_2Mu->Fill(metstar.Pt(),wt);
	h_nHadJets_2Mu->Fill(nHadJets,wt);
	h_BTags_2Mu->Fill(BTags,wt);
	h_METvBin_2Mu->Fill(metstar.Pt(),wt);
	h_BestPhotonPt_2Mu->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_2Mu->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_2Mu->Fill(bestPhoton.Phi(),wt);
	h_ZMass_2Mu->Fill(zmass,wt);

	h_METstar_2Mu->Fill(metstar.Pt(),wt);
	h_METPhi_2Mu->Fill(metstar.Phi(),wt);
	h_METOrg_2Mu->Fill(MET,wt);
	h_METclean_2Mu->Fill(METclean,wt);
	h_METPhiclean_2Mu->Fill(METPhiclean,wt);

	h_Mu1Pt->Fill((*Muons)[0].Pt(),wt);
	h_Mu2Pt->Fill((*Muons)[1].Pt(),wt);
	h_Mu1Eta->Fill((*Muons)[0].Eta(),wt);
	h_Mu2Eta->Fill((*Muons)[1].Eta(),wt);
	h_Mu1Phi->Fill((*Muons)[0].Phi(),wt);
	h_Mu2Phi->Fill((*Muons)[1].Phi(),wt);

	h_isoMuonTracks_2Mu->Fill(isoMuonTracks,wt);

	h_dR_PhoClstMu->Fill(dRphoClstLep,wt);
	h_dPhi_PhoMET_2Mu->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_2Mu->Fill(dphi1,wt);
	h_dphi_METjet2_2Mu->Fill(dphi2,wt);
	h_dphi_PhoMu1->Fill(dphi_PhoLep1,wt);
	h_dphi_PhoMu2->Fill(dphi_PhoLep2,wt);
	h_mTPho_2Mu->Fill(mt_Pho,wt);
      
	h_Jet1Pt_2Mu->Fill(hadJets[0].Pt(),wt);
	h_Jet2Pt_2Mu->Fill(hadJets[1].Pt(),wt);
	h_isoEleTrack_2Mu->Fill(isoElectronTracks,wt);
	h_isoMuTrack_2Mu->Fill(isoMuonTracks,wt);
	h_isoPiTrack_2Mu->Fill(isoPionTracks,wt);

	int minDRindx = -100;
	double minDR=1000.0;
	for(int i=0;i<Jets->size();i++){
	  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
	    double dR=(*Muons)[0].DeltaR((*Jets)[i]);
	    if(dR<minDR){minDR=dR;minDRindx=i;}
	  }
	}
    
	h2_R_Mu1PtJetPtVsDR->Fill( minDR,(((*Jets)[minDRindx].Pt())/((*Muons)[0].Pt())),wt);
	if(minDR<0.3) h2_Mu1PtMatchJetPt->Fill( (*Muons)[0].Pt(),(*Jets)[minDRindx].Pt(),wt );
	//	h2_R_Mu2PtJetPtVsDR->Fill(ratioMu2,dRMu2,wt);

	if(allBestPhotons.size()>1) h_Pho2Pt_2Mu->Fill(allBestPhotons[1].Pt(),wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_2Mu_R[searchRegion-1]->Fill(metstar.Pt(),wt);
	}
	else cout<<"Event outside search region! ";
	if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2_2Mu[0]->Fill(metstar.Pt(),wt);
        else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2_2Mu[1]->Fill(metstar.Pt(),wt);
        else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2_2Mu[2]->Fill(metstar.Pt(),wt);
        else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2_2Mu[3]->Fill(metstar.Pt(),wt);
        else if(BTags>=2)                                   h_MET_R_v2_2Mu[4]->Fill(metstar.Pt(),wt);
        h_SBins_2Mu->Fill(sBin2,wt);
	h_SBins_v4_2Mu->Fill(sBin4,wt);
	h_SBins_v7_2Mu->Fill(sBin7,wt);
      }
      else if(Electrons->size()==2){
	h_ST_2Ele->Fill(ST,wt);
	h_MET_2Ele->Fill(metstar.Pt(),wt);
	h_nHadJets_2Ele->Fill(nHadJets,wt);
	h_BTags_2Ele->Fill(BTags,wt);
	h_METvBin_2Ele->Fill(metstar.Pt(),wt);
	h_BestPhotonPt_2Ele->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_2Ele->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_2Ele->Fill(bestPhoton.Phi(),wt);
	h_ZMass_2Ele->Fill(zmass,wt);

	h_METstar_2Ele->Fill(metstar.Pt(),wt);
	h_METPhi_2Ele->Fill(metstar.Phi(),wt);
	h_METOrg_2Ele->Fill(MET,wt);
	h_METclean_2Ele->Fill(METclean,wt);
	h_METPhiclean_2Ele->Fill(METPhiclean,wt);

	h_Ele1Pt->Fill((*Electrons)[0].Pt(),wt);
	h_Ele2Pt->Fill((*Electrons)[1].Pt(),wt);
	h_Ele1Eta->Fill((*Electrons)[0].Eta(),wt);
	h_Ele2Eta->Fill((*Electrons)[1].Eta(),wt);
	h_Ele1Phi->Fill((*Electrons)[0].Phi(),wt);
	h_Ele2Phi->Fill((*Electrons)[1].Phi(),wt);

	h_isoMuonTracks_2Ele->Fill(isoMuonTracks,wt);

	h_dR_PhoClstEle->Fill(dRphoClstLep,wt);
	h_dPhi_PhoMET_2Ele->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_2Ele->Fill(dphi1,wt);
	h_dphi_METjet2_2Ele->Fill(dphi2,wt);
	h_dphi_PhoEle1->Fill(dphi_PhoLep1,wt);
	h_dphi_PhoEle2->Fill(dphi_PhoLep2,wt);
	h_mTPho_2Ele->Fill(mt_Pho,wt);
      	 
	h_Jet1Pt_2Ele->Fill(hadJets[0].Pt(),wt);	
	h_Jet2Pt_2Ele->Fill(hadJets[1].Pt(),wt);	
	h_isoEleTrack_2Ele->Fill(isoElectronTracks,wt);
	h_isoMuTrack_2Ele->Fill(isoMuonTracks,wt);
	h_isoPiTrack_2Ele->Fill(isoPionTracks,wt);
	
	int minDRindx = -100;
	double minDR=1000.0;
	for(int i=0;i<Jets->size();i++){
	  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
	    double dR=(*Electrons)[0].DeltaR((*Jets)[i]);
	    if(dR<minDR){minDR=dR;minDRindx=i;}
	  }
	}
    
	h2_R_Ele1PtJetPtVsDR->Fill( minDR,(((*Jets)[minDRindx].Pt())/((*Electrons)[0].Pt())),wt);
	if(minDR<0.3) h2_Ele1PtMatchJetPt->Fill( (*Electrons)[0].Pt(),(*Jets)[minDRindx].Pt(),wt );

	if(allBestPhotons.size()>1) h_Pho2Pt_2Ele->Fill(allBestPhotons[1].Pt(),wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_2Ele_R[searchRegion-1]->Fill(metstar.Pt(),wt);
	}
	else cout<<"Event outside search region! "<<searchRegion;
	if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2_2Ele[0]->Fill(metstar.Pt(),wt);
        else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2_2Ele[1]->Fill(metstar.Pt(),wt);
        else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2_2Ele[2]->Fill(metstar.Pt(),wt);
        else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2_2Ele[3]->Fill(metstar.Pt(),wt);
        else if(BTags>=2)                                   h_MET_R_v2_2Ele[4]->Fill(metstar.Pt(),wt);
        h_SBins_2Ele->Fill(sBin2,wt);
	h_SBins_v4_2Ele->Fill(sBin4,wt);
	h_SBins_v7_2Ele->Fill(sBin7,wt);
      }//e-e + photon events
    }
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
}


TLorentzVector ZGamma::getBestPhoton(){
  // bool passIDVeryLoose=false,passIDLoose=false,passIsoVeryLoose=false,passIsoLoose=false;
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  allBestPhotons.resize(0);
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( ((*Photons_fullID)[iPho]) && ((*Photons_hasPixelSeed)[iPho]<0.001) ) {
      goodPho.push_back( (*Photons)[iPho] );
      goodPhoIndx.push_back(iPho);
      allBestPhotons.push_back((*Photons)[iPho]);
    }
  }
  if(allBestPhotons.size()>1) sortTLorVec(&allBestPhotons);
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

int ZGamma::getBinNoV4(int nHadJets){
  int sBin=-100,m_i=0;
  if(BTags==0){
    if(nHadJets>=2 && nHadJets<=4)     { sBin=0;}
    else if(nHadJets==5 || nHadJets==6){ sBin=8;}
    else if(nHadJets>=7)               { sBin=15;}
  }
  else{
    if(nHadJets>=2 && nHadJets<=4)     { sBin=22;}
    else if(nHadJets==5 || nHadJets==6){ sBin=29;}
    else if(nHadJets>=7)               { sBin=36;}
  }
  if(sBin==0){
    for(int i=0;i<METBinLowEdgeV4_njLow.size()-1;i++){
      if(METBinLowEdgeV4_njLow[i]<99.99) continue;
      m_i++;
      if(metstar.Pt() >= METBinLowEdgeV4_njLow[i] && metstar.Pt() < METBinLowEdgeV4_njLow[i+1]){ sBin = sBin+m_i;break; }
      else if(metstar.Pt() >= METBinLowEdgeV4_njLow[METBinLowEdgeV4_njLow.size()-1])  { sBin = 8         ;break; }
    }
  }
  else{
    for(int i=0;i<METBinLowEdgeV4.size()-1;i++){
      if(METBinLowEdgeV4[i]<99.99) continue;
      m_i++;
      if(metstar.Pt() >= METBinLowEdgeV4[i] && metstar.Pt() < METBinLowEdgeV4[i+1]){ sBin = sBin+m_i;break; }
      else if(metstar.Pt() >= METBinLowEdgeV4[METBinLowEdgeV4.size()-1])  { sBin = sBin+7   ;break; }
    }
  }
  return sBin;
}

int ZGamma::getBinNoV7(int nHadJets){
  int sBin=-100,m_i=0;
  if(BTags==0){
    if(nHadJets>=2 && nHadJets<=4)     { sBin=0;}
    else if(nHadJets==5 || nHadJets==6){ sBin=6;}
    else if(nHadJets>=7)               { sBin=11;}
  }
  else{
    if(nHadJets>=2 && nHadJets<=4)     { sBin=16;}
    else if(nHadJets==5 || nHadJets==6){ sBin=21;}
    else if(nHadJets>=7)               { sBin=26;}
  }
  if(sBin==0){
    for(int i=0;i<METBinLowEdgeV7_njLow.size()-1;i++){
      if(METBinLowEdgeV7_njLow[i]<99.99) continue;
      m_i++;
      if(metstar.Pt() >= METBinLowEdgeV7_njLow[i] && metstar.Pt() < METBinLowEdgeV7_njLow[i+1]){ sBin = sBin+m_i;break; }
      else if(metstar.Pt() >= METBinLowEdgeV7_njLow[METBinLowEdgeV7_njLow.size()-1])  { sBin = 6         ;break; }
    }
  }
  else{
    for(int i=0;i<METBinLowEdgeV7.size()-1;i++){
      if(METBinLowEdgeV7[i]<99.99) continue;
      m_i++;
      if(metstar.Pt() >= METBinLowEdgeV7[i] && metstar.Pt() < METBinLowEdgeV7[i+1]){ sBin = sBin+m_i;break; }
      else if(metstar.Pt() >= METBinLowEdgeV7[METBinLowEdgeV7.size()-1])  { sBin = sBin+5   ;break; }
    }
  }
  return sBin;
}

bool ZGamma::check_eMatchedtoGamma(){
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

bool ZGamma::check_muMatchedtoGamma(){
  if(bestPhotonIndxAmongPhotons>=0){
    for(int i=0;i<Muons->size();i++){
      if( (*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Muons)[i] ) < 0.2){
	return true;
      }
    }
  }
  else
    return false;
}

double ZGamma::getGendRLepPho(){//MC only
  TLorentzVector genPho1,genLep1;
  int leadGenPhoIdx=-100;
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
  if(genPho1.Pt() > 0. && genLep1.Pt() > 0.) return genLep1.DeltaR(genPho1);
  else return 1000.0;
}

void  ZGamma::findObjMatchedtoG(TLorentzVector bestPhoton){
  
  double dR=100;
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
  //cout<<"Pt: "<<bestPhoton.Pt()<<" Eta:"<<bestPhoton.Eta()<<" Phi:"<<bestPhoton.Phi()<<" PdgID:"<<(*GenParticles_PdgId)[match]<<" Pt:"<<(*GenParticles)[match].Pt()<<" Eta:"<<(*GenParticles)[match].Eta()<<" Phi:"<<(*GenParticles)[match].Phi()<<" parentId:"<<(*GenParticles_ParentId)[match]<<endl;
  if(dR<0.1){
    h_GmatchedObj->Fill(abs((*GenParticles_PdgId)[match]),wt);
    if(abs((*GenParticles_PdgId)[match])==22)  h_PdgIdPhoParent->Fill(abs((*GenParticles_ParentId)[match]),wt);
  }
  else{
    h_GmatchedObj->Fill(0.0,wt);
    h_PdgIdPhoParent->Fill(0.0,wt);
  }
  //find obj matched to muon
  /*
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
	// if(abs((*GenParticles_ParentId)[match])!=24 && abs((*GenParticles_ParentId)[match])!=15){
	//   print(0);
	//   cout<<(*GenParticles_PdgId)[match]<<" Mu Pt: "<<(*Muons)[0].Pt()<<" eta: "<<(*Muons)[0].Eta()<<" phi: "<<(*Muons)[0].Phi()<<endl<<abs((*GenParticles_ParentId)[match])<<endl;
	//	}
      }
    }
    else{
      h_MuMatchedObj->Fill(0.0,wt);
      h_PdgIdMuParent->Fill(0.0,wt);
    }
  }*/
  
}



void ZGamma::print(Long64_t jentry){
  //cout<<endl;
  TLorentzVector v1,photo;
  for(int i=0;i<GenParticles->size();i++){
    // cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
  }
  
  for(int i=0;i<Photons->size();i++){
    double dR=0;//DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*Photons)[i].Eta(),(*Photons)[i].Phi() );
    //cout<<jentry<<" i:"<<i<<" phoSize:"<<Photons->size()<<" Pt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" otherP:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" dR:"<<dR<<endl;
  }

  for(int i=0;i<GenJets->size();i++){
    cout<<"GenJetsPt:"<<(*GenJets)[i].Pt()<<" eta:"<<(*GenJets)[i].Eta()<<" phi:"<<(*GenJets)[i].Phi()<<endl;
  }
  for(int i=0;i<Jets->size();i++){
    //  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
    if(Muons->size()==2){
      cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<" energy:"<<(*Jets)[i].Energy()<<" dR with Muon: "<<(*Jets)[i].DeltaR((*Muons)[0])<<endl;
    }
    if(Electrons->size()==2){
      cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<" energy:"<<(*Jets)[i].Energy()<<" dR with electron: "<<(*Jets)[i].DeltaR((*Electrons)[0])<<endl;
    }
  }

  for(int i=0;i<Muons->size();i++){
    if(i==0) cout<<"-------------------------------- Muons -------------------------------------------"<<endl;
    cout<<"MuonPt: "<<(*Muons)[i].Pt()<<" Eta: "<<(*Muons)[i].Eta()<<" Phi: "<<(*Muons)[i].Phi()<<" M: "<<(*Muons)[i].M()<<" E:"<<(*Muons)[i].Energy()<<endl;
  }
  for(int i=0;i<Electrons->size();i++){
    if(i==0) cout<<"-------------------------------- Electrons -------------------------------------------"<<endl;
    cout<<"ElectronPt: "<<(*Electrons)[i].Pt()<<" Eta: "<<(*Electrons)[i].Eta()<<" Phi: "<<(*Electrons)[i].Phi()<<" M: "<<(*Electrons)[i].M()<<" E:"<<(*Electrons)[i].Energy()<<endl;
  }
  cout<<"^^^^^^^^^^^^^^^^^^ Event ends ^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl<<endl;
}
