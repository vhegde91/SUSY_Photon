#define FakeRateEst_cxx
#include "FakeRateEst.h"
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

  FakeRateEst ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void FakeRateEst::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  string s_data=data;
 
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  
  int evtSurvived=0;
  //get 2d histogram========================================
  //TFile *f_FR=new TFile("FR_Hist_MS_FR_DYJetsToLL_Truth.root");
  TFile *f_FR=new TFile("FR_Hist_CS_TTW_FR_v2.root");
  //  TFile *f_FR=new TFile("FR_Hist_CS_TTW_FR_NoTrgPuWt.root");
  //  TFile *f_FR=new TFile("FR_Hist_MS_FR_DYJetsToLL_v2.root");
  TH2D *h2_FR;
  if(f_FR) h2_FR=(TH2D*)f_FR->FindObjectAny("FakeRate");

  bool do_prediction=0;
  cout<<"Doing prediction from file |"<<f_FR->GetName()<<"|? "<<do_prediction<<endl;
  //---------------------- MC only -------------------------
  // TFile* pufile = TFile::Open("PileupHistograms_0121_69p2mb_pm4p6.root","READ");
  // //choose central, up, or down
  // TH1* puhist = (TH1*)pufile->Get("pu_weights_down");
  // // int promptFiles=1;
  // // if(s_data=="CS_FR") promptFiles=3;
  // // else if(s_data=="CS_FR_WG") promptFiles=2;
  // // else if(s_data=="CS_FR_TTG") promptFiles=1;
  // // cout<<"Doing prediction from file |"<<f_FR->GetName()<<"|? "<<do_prediction<<endl;
  // // cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  // // cout<<"Only events with "<<endl
  // //     <<"prompt best photon from first "<<promptFiles<<" line(s) in input runlist"<<endl
  // //     <<"non-prompt best photon from remaining files"<<endl
  // //     <<"will be read."<<endl;
  //---------------------- MC only ends-------------------------
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
    if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0)) continue;  
    bool process=true;

    if( Electrons->size()>1 || Muons->size()!=0 ) continue;
    if(isoMuonTracks!=0 || isoPionTracks!=0) continue;   
    //about electrons
    bool hasEle=0,hasPho=0;
    if(Electrons->size()==1){
      if( ((*Electrons)[0].Pt()>100) && (*Electrons_tightID)[0] ) hasEle=true;
    }
    //about photons
    TLorentzVector bestPhoton=getBestPhoton();
    bool eMatchedG=check_eMatchedtoGamma();
    if( !eMatchedG && bestPhoton.Pt()>100) hasPho=true;

    bool bestEMObjIsEle=0;
    TLorentzVector bestEMObj;
    if     ( hasEle && !hasPho) {bestEMObjIsEle=true;  bestEMObj = (*Electrons)[0];}
    //    else if(!hasEle &&  hasPho) {bestEMObjIsEle=false; bestEMObj = bestPhoton;}//---------MC only--------
    else continue;

    double mT_EMObj = 0.;
    mT_EMObj = sqrt(2*bestEMObj.Pt()*MET*(1-cos(DeltaPhi(METPhi,bestEMObj.Phi()))));
    if(bestEMObjIsEle && mT_EMObj>100) continue;
    //++++++++++++++++++++++ data only +++++++++++++++++++++++
    bool passTrig = false;
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back();
      if( trgName=="HLT_Photon90_CaloIdL_PFHT600_v" && (*TriggerPass)[i]==1 ) passTrig = true;
      else if( trgName=="HLT_Photon165_HE10_v" && (*TriggerPass)[i]==1 ) passTrig = true;
      wt=0.98/0.99;
      // if((*Electrons)[0].Pt() > 190.0){
      // 	if( (trgName=="HLT_Ele27_WPTight_Gsf_v" || trgName=="HLT_Ele27_WPTight_Gsf_" || trgName=="HLT_Photon165_HE10_v" || trgName=="HLT_Photon165_HE10_" ) ){
      // 	  if((*TriggerPass)[i]==1){
      // 	    passTrig = true;
      // 	    wt = 1.0*0.98/0.99;//0.98 for SR trig eff, 0.99 for (Ele27 || Pho165) eff
      // 	    break;
      // 	  }
      // 	  else if((*TriggerPass)[i]==-1 || (*TriggerPrescales)[i]!=1)
      // 	    cout<<"AHHHHHH TrigPass:"<<(*TriggerPass)[i]<<" PS:"<<(*TriggerPrescales)[i]<<endl;
      // 	}
      // }
      // else if((*Electrons)[0].Pt() > 100){
      // 	if( (trgName=="HLT_Ele27_WPTight_Gsf_v" || trgName=="HLT_Ele27_WPTight_Gsf_" ) ){
      // 	  if((*TriggerPass)[i]==1){
      // 	    passTrig = true;
      // 	    wt = 1.0*0.98/0.87;//0.98 for SR trig eff, 0.87 for Ele27 eff.
      // 	    break;
      // 	  }
      // 	  else if((*TriggerPass)[i]==-1 || (*TriggerPrescales)[i]!=1)
      // 	    cout<<"AHHHHHH TrigPass:"<<(*TriggerPass)[i]<<" PS:"<<(*TriggerPrescales)[i]<<endl;
      // 	}
      // }
    }
    if(!passTrig) continue;
    //++++++++++++++++++++++ data only ends +++++++++++++++++++++++
    //---------------------- MC only -------------------------
    // wt=Weight*lumiInfb*1000.0*0.98*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));  
    // //    wt=Weight*1000.0*lumiInfb;  
    // if(s_data=="madHT0to600" && madHT > 600) continue;

    // bool fakePhoton=0,showEnt=0;
    // if(!bestEMObjIsEle){
    //   double minDR_Pho_GenObj=1000;
    //   for(int i=0;i<GenParticles->size();i++){
    // 	if((*GenParticles)[i].Pt()!=0){
    // 	  double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
    // 	  if(dr1 < 0.2 && (abs((*GenParticles_PdgId)[i])==11) && (abs((*GenParticles_ParentId)[i])<=24) ){
    // 	    fakePhoton=true;
    // 	  }
    // 	  if(minDR_Pho_GenObj > dr1) minDR_Pho_GenObj=dr1;
    // 	}
    //   }
    //   h_minDR_Pho_GenObj->Fill(minDR_Pho_GenObj,wt);
    //   // for(int i=0;i<GenParticles->size();i++){
    //   // 	if((*GenParticles)[i].Pt()!=0){
    //   // 	  double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
    //   // 	  if( dr1<0.2 && (abs((*GenParticles_PdgId)[i])==22) && 
    //   // 	      ( ((*GenParticles)[i].Pt()/bestPhoton.Pt()) > 0.9) && ((*GenParticles)[i].Pt()/bestPhoton.Pt()) < 1.1 )  fakePhoton=false;
    //   // 	}
    //   // }
    //   if(!fakePhoton) continue;
    //   if(Electrons->size()!=0 || Muons->size()!=0 ) continue;
    //   if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;
    // }
    //---------------------- MC only ends-------------------------
    
    bool hadJetID=true;
    int minDRindx=-100,emObjMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0,remJetPt=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= HT_EtaCut) ){
	double dR=bestEMObj.DeltaR((*Jets)[i]);
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
    if( minDR<0.3 ) emObjMatchingJetIndx=minDRindx;
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestEMObj.Pt();
    }
    sortTLorVec(&hadJets);
    
    double dphi1=3.8,dphi2=3.8,dphi3=3.8,dphi4=3.8,dphiEM_MET=3.8;
    if(bestEMObj.Pt() >0.1) dphiEM_MET=abs(DeltaPhi(METPhi,bestEMObj.Phi()));
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));
    if(hadJets.size() > 2 ) dphi3 = abs(DeltaPhi(METPhi,(hadJets)[2].Phi()));
    if(hadJets.size() > 3 ) dphi4 = abs(DeltaPhi(METPhi,(hadJets)[3].Phi()));
    
    int qMultMatchJet = 0;
    if(minDR < 0.3) qMultMatchJet = (*Jets_chargedMultiplicity)[minDRindx];

    double minDRHadJ=100000;
    int minDRHadJIndx=-100;
    for(int i=0;i<hadJets.size();i++){
      if(minDRHadJ > bestEMObj.DeltaR((hadJets)[i])) minDRHadJ=bestEMObj.DeltaR(hadJets[i]);
    }

    process = process && ST>500 && nHadJets>=2 && MET>100 && !(dphi1 > 0.3 && dphi2 > 0.3) && bestEMObj.Pt()>100;
    if( !((ST>800 && bestEMObj.Pt()>100) || (bestEMObj.Pt()>190)) ) continue;

    if(process){
      evtSurvived++;
      double wt_org=wt;
      h_RunNum->Fill(RunNum);
      h_intLumi->Fill(lumiInfb);
      //      cout<<" "<<Electrons_MT2Activity->size()<<","<<Electrons->size();      
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

      int nTracksNearEM = 0;
      for(int i=0;i<TAPElectronTracks->size();i++){
	if(bestEMObj.DeltaR((*TAPElectronTracks)[i]) < 0.3) nTracksNearEM++;
      }
      for(int i=0;i<TAPMuonTracks->size();i++){
	if(bestEMObj.DeltaR((*TAPMuonTracks)[i]) < 0.3) nTracksNearEM++;
      }
      for(int i=0;i<TAPPionTracks->size();i++){
	if(bestEMObj.DeltaR((*TAPPionTracks)[i]) < 0.3) nTracksNearEM++;
      }
      
      if(hadJetID){
	//---------------------- MC only -------------------------
	// double genElePt=0;
	// for(int i=0;i<GenParticles->size();i++){
	//   if((*GenParticles)[i].Pt()!=0){
	//     double dr1=bestEMObj.DeltaR((*GenParticles)[i]);
	//     if(dr1 < 0.2 && (abs((*GenParticles_PdgId)[i])==11) && (abs((*GenParticles_ParentId)[i])<=24) ) {
	//       genElePt=(*GenParticles)[i].Pt();
	//     }
	//   }
	// }
	
	// double min_1=1000,genEleMT2Act=-100;
	// for(int i=0;i<GenElectrons->size();i++){
	//   if(min_1>bestEMObj.DeltaR((*GenElectrons)[i])){
	//     min_1=bestEMObj.DeltaR((*GenElectrons)[i]);
	//     genEleMT2Act=(*GenElectrons_MT2Activity)[i];
	//   }
	// }
	// //	cout<<endl;print(jentry);
	
	// double minGenDR=1000,minGenDR2=1000;
	// int minGenDRIndx=-100;
	// for(int i=0;i<GenJets->size();i++){
	//   if(minGenDR>bestEMObj.DeltaR((*GenJets)[i])) {minGenDR=bestEMObj.DeltaR((*GenJets)[i]); minGenDRIndx=i;}
	// }
	// for(int i=0;i<GenJets->size();i++){
	//   if(i!=minGenDRIndx && minGenDR2>bestEMObj.DeltaR((*GenJets)[i])) minGenDR2=bestEMObj.DeltaR((*GenJets)[i]);
	// }
	
      	// if(bestEMObjIsEle){
	//   if(do_prediction){
	//     //
	//     double parX=bestEMObj.Pt(),parY=qMultMatchJet;
	//     //	    double parX=bestEMObj.Pt(),parY=NVtx;
	//     double fakerate=0;
	//     if(h2_FR) fakerate=h2_FR->GetBinContent(h2_FR->FindBin(parX,parY));
	//     else{ cout<<"hist not found"<<endl; return;}
	//     //	    wt=(fakerate/(1-fakerate))*wt;
	//     wt=fakerate*wt;
	//   }
	  //---------------------- MC only ends-------------------------
	if(bestEMObjIsEle){ //+++++ Data only +++++
	  h_ST_Ele->Fill(ST,wt);
	  h_MET_Ele->Fill(MET,wt);
	  h_nHadJets_Ele->Fill(nHadJets,wt);
	  h_BTags_Ele->Fill(BTags,wt);
	  h_HT_Ele->Fill(HT,wt);
	  h_MHT_Ele->Fill(MHT,wt);
	  h_nJets_Ele->Fill(NJets,wt);
	  h_METvBin_Ele->Fill(MET,wt);
	  h_MET_Ele_Rall->Fill(MET,wt);
	  h_mT_Ele->Fill(mT_EMObj,wt);

	  if( searchRegion > 0 && searchRegion < 4){
	    h_MET_Ele_R[searchRegion-1]->Fill(MET,wt);
	  }
	  else cout<<"Event outside search region! ";

	  if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2_Ele[0]->Fill(MET,wt);
	  else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2_Ele[1]->Fill(MET,wt);
	  else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2_Ele[2]->Fill(MET,wt);
	  else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2_Ele[3]->Fill(MET,wt);
	  else if(BTags>=2)                                   h_MET_R_v2_Ele[4]->Fill(MET,wt);
	  h_SBins_Ele->Fill(sBin2,wt);

	  //	  h_madHT_Ele->Fill(madHT,wt);//------ MC only --------------
	  h_nVtx_Ele->Fill(NVtx,wt);

	  h_ElePt->Fill(bestEMObj.Pt(),wt);
	  h_EleEta->Fill(bestEMObj.Eta(),wt);
	  h_ElePhi->Fill(bestEMObj.Phi(),wt);
	  h_ElePtvBin->Fill(bestEMObj.Pt(),wt);

	  h_QMult2_Ele->Fill(qMultMatchJet,wt);
	  h_EleTracks_Ele->Fill(TAPElectronTracks->size(),wt);
	  h_MuTracks_Ele->Fill(TAPMuonTracks->size(),wt);
	  h_PiTracks_Ele->Fill(TAPPionTracks->size(),wt);
	  h_nTracks_Ele->Fill(TAPElectronTracks->size()+TAPMuonTracks->size()+TAPPionTracks->size(),wt);
	  h_nTracks0p3_Ele->Fill(nTracksNearEM-1,wt);

    	  h_minDR_Ele_Jet->Fill(minDR,wt);
	  h_minDRHadJ_Ele_HadJet->Fill(minDRHadJ,wt);
	  //	  h_dRGenJet2_Ele->Fill(minGenDR2,wt);//------ MC only --------------

	  h_PtJetNearEle->Fill((*Jets)[minDRindx].Pt(),wt);
	  h_JptEleptRatio->Fill( ((*Jets)[minDRindx].Pt())/(bestEMObj.Pt()),wt );
	  h_HadDR_EleptJptRatio->Fill( minDRHadJ*bestEMObj.Pt()/((*Jets)[minDRindx].Pt()),wt );

	  h2_ElePtST->Fill(bestEMObj.Pt(),ST,wt);
	  h2_ElePtEta->Fill(abs(bestEMObj.Eta()),bestEMObj.Pt(),wt);
	  h2_ElePtEtavBin->Fill(abs(bestEMObj.Eta()),bestEMObj.Pt(),wt);
	  h2_JetPtNearEleEtavBin->Fill( abs((*Jets)[minDRindx].Eta()),(*Jets)[minDRindx].Pt(),wt);
	  h2_minHadDR_QMult_Ele->Fill( minDRHadJ,qMultMatchJet,wt);

	  h2_JetPtNearEleMinDRHadJ->Fill(minDRHadJ,(*Jets)[minDRindx].Pt(),wt);//
	  h2_ElePtMinDRHadJ->Fill(minDRHadJ,bestEMObj.Pt(),wt);
	  h2_PtEleQHadFracJet->Fill(bestEMObj.Pt(),(*Jets_chargedHadronEnergyFraction)[minDRindx],wt);
	  h2_ElePtQMultJet->Fill(bestEMObj.Pt(),qMultMatchJet,wt);

	  h2_ElePtVtx->Fill(bestEMObj.Pt(),NVtx,wt);
	  h2_EleEtaPhi->Fill(bestEMObj.Eta(),bestEMObj.Phi(),wt);

	  h2_PtElevsMET->Fill( bestEMObj.Pt(),MET,wt);
	  //	  h2_RecoElePt_GenEPt->Fill(genElePt,bestEMObj.Pt(),wt);//------ MC only --------------
	  h2_JptEleptRatiovsDR->Fill(minDR,((*Jets)[minDRindx].Pt())/(bestEMObj.Pt()),wt);
	  h2_HadDRJptEptRatio_Vtx->Fill( minDRHadJ*(((*Jets)[minDRindx].Pt())/(bestEMObj.Pt())),NVtx,wt);

	  h3_ElePtEtaNJ->Fill(abs(bestEMObj.Eta()),nHadJets,bestEMObj.Pt(),wt);
	  h3_ElePtEtaVtx->Fill(abs(bestEMObj.Eta()),NVtx,bestEMObj.Pt(),wt);
	  h3_JetPtEleEtaMinHadDR->Fill( abs(bestEMObj.Eta()),(*Jets)[minDRindx].Pt(),minDRHadJ,wt);

	  h3_JetPtVtxMinHadDR_Ele->Fill(((*Jets)[minDRindx].Pt()),NVtx,minDRHadJ,wt);

	  h3_ElePtVtxMinHadDR->Fill( bestEMObj.Pt(),NVtx,minDRHadJ,wt );//
	  h3_ElePtEtaMinDRHadJ->Fill( bestEMObj.Pt(),abs(bestEMObj.Eta()),minDRHadJ,wt );//

	  if(remJetPt>0.1)h_RemjetPtNearEle->Fill(remJetPt,wt);

	  h_dPhi_METEle->Fill(dphiEM_MET,wt);
	  if(hadJets.size() > 0 ) h_dPhi_METjet1_Ele->Fill( dphi1,wt );
	  if(hadJets.size() > 1 ) h_dPhi_METjet2_Ele->Fill( dphi2,wt );
	  if(hadJets.size() > 2 ) h_dPhi_METjet3_Ele->Fill( dphi3,wt );
	  if(hadJets.size() > 3 ) h_dPhi_METjet4_Ele->Fill( dphi4,wt );

	  for(int i=0;i<hadJets.size() && i<4;i++){
	    h_jetPt_Ele[i] ->Fill(hadJets[i].Pt(),wt);
	    h_jetEta_Ele[i]->Fill(hadJets[i].Eta(),wt);
	    h_jetPhi_Ele[i]->Fill(hadJets[i].Phi(),wt);
	    h_dPhi_jet_Ele[i]->Fill(abs(bestEMObj.DeltaPhi(hadJets[i])),wt);
	  }
	  double eleMT2Act=-100,tempMin=10000;
	  for(int i=0;i<Electrons->size();i++){
	    if(tempMin>bestEMObj.DeltaR((*Electrons)[i])){
	      tempMin=bestEMObj.DeltaR((*Electrons)[i]);
	      eleMT2Act=(*Electrons_MT2Activity)[i];
	    }
	  }
	  h_EleMT2Activity->Fill(eleMT2Act,wt);
	  h2_nVtx_EleMT2Act->Fill(NVtx,eleMT2Act,wt);
	  //	  h_genEleMT2Act_Ele->Fill(genEleMT2Act,wt);//------ MC only --------------

	  h_CM_Ele->Fill(qMultMatchJet,wt);
	  h_NHF_Ele->Fill((*Jets_neutralHadronEnergyFraction)[minDRindx],wt);
	  h_NEMF_Ele->Fill((*Jets_neutralEmEnergyFraction)[minDRindx],wt);
	  h_CHF_Ele->Fill((*Jets_chargedHadronEnergyFraction)[minDRindx],wt);
	  h_CEMF_Ele->Fill((*Jets_chargedEmEnergyFraction)[minDRindx],wt);
	  h_PhoFrac_Ele->Fill((*Jets_photonEnergyFraction)[minDRindx],wt);

	  wt=wt_org;
	}//end of ele like
	//photon like
	//	else{//-------------- MC only ------------
	//+++++++++++++++++++++++++ data only +++++++++++++++++++++++++++++
      	if(bestEMObjIsEle){//get prediction from single e-CS
	  if(do_prediction){
	    //
	    double parX=bestEMObj.Pt(),parY=qMultMatchJet;
	    //	    double parX=bestEMObj.Pt(),parY=NVtx;
	    double fakerate=0;
	    if(h2_FR) fakerate=h2_FR->GetBinContent(h2_FR->FindBin(parX,parY));
	    else{ cout<<"hist not found"<<endl; return;}
	    //	    wt=(fakerate/(1-fakerate))*wt;
	    wt=fakerate*wt;
	  }
	  //++++++++++++++++++++++ data only ends +++++++++++++++++++++++++++
	  h_ST_Pho->Fill(ST,wt);
	  h_MET_Pho->Fill(MET,wt);
	  h_nHadJets_Pho->Fill(nHadJets,wt);
	  h_BTags_Pho->Fill(BTags,wt);
	  h_HT_Pho->Fill(HT,wt);
	  h_MHT_Pho->Fill(MHT,wt);
	  h_nJets_Pho->Fill(NJets,wt);
	  h_METvBin_Pho->Fill(MET,wt);
	  h_MET_Pho_Rall->Fill(MET,wt);
	  h_mT_Pho->Fill(mT_EMObj,wt);

	  if( searchRegion > 0 && searchRegion < 4){
	    h_MET_Pho_R[searchRegion-1]->Fill(MET,wt);
	  }
	  else cout<<"Event outside search region! ";

	  if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2_Pho[0]->Fill(MET,wt);
	  else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2_Pho[1]->Fill(MET,wt);
	  else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2_Pho[2]->Fill(MET,wt);
	  else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2_Pho[3]->Fill(MET,wt);
	  else if(BTags>=2)                                   h_MET_R_v2_Pho[4]->Fill(MET,wt);
	  h_SBins_Pho->Fill(sBin2,wt);

	  //	  h_madHT_Pho->Fill(madHT,wt);//------ MC only --------------
	  h_nVtx_Pho->Fill(NVtx,wt);

	  h_BestPhotonPt->Fill(bestPhoton.Pt(),wt);
	  h_BestPhotonEta->Fill(bestPhoton.Eta(),wt);
	  h_BestPhotonPhi->Fill(bestPhoton.Phi(),wt);
	  h_PhoPtvBin->Fill(bestEMObj.Pt(),wt);

	  h_EleTracks_Pho->Fill(TAPElectronTracks->size(),wt);
	  h_MuTracks_Pho->Fill(TAPMuonTracks->size(),wt);
	  h_PiTracks_Pho->Fill(TAPPionTracks->size(),wt);
	  h_nTracks_Pho->Fill(TAPElectronTracks->size()+TAPMuonTracks->size()+TAPPionTracks->size(),wt);
	  h_nTracks0p3_Pho->Fill(nTracksNearEM,wt);

	  h_QMult2_Pho->Fill(qMultMatchJet,wt);
	  h_minDR_Pho_Jet->Fill(minDR,wt);
	  h_minDRHadJ_Pho_HadJet->Fill(minDRHadJ,wt);
	  //	  h_dRGenJet2_Pho->Fill(minGenDR2,wt);//------ MC only --------------
	  
	  h_PtJetNearPho->Fill((*Jets)[minDRindx].Pt(),wt);
	  h_JptGptRatio->Fill( ((*Jets)[minDRindx].Pt())/(bestEMObj.Pt()),wt );
	  h_HadDR_GptJptRatio->Fill( minDRHadJ*bestEMObj.Pt()/((*Jets)[minDRindx].Pt()),wt );

	  //	  h_genEleMT2Act_Pho->Fill(genEleMT2Act,wt);//------ MC only --------------

          h2_PhoPtST->Fill(bestEMObj.Pt(),ST,wt);
	  h2_PhotonPtEta->Fill(abs(bestEMObj.Eta()),bestEMObj.Pt(),wt);
	  h2_PhotonPtEtavBin->Fill(abs(bestEMObj.Eta()),bestEMObj.Pt(),wt);
	  h2_JetPtNearPhoEtavBin->Fill( abs(bestEMObj.Eta()),(*Jets)[minDRindx].Pt(),wt);
	  h2_minHadDR_QMult_Pho->Fill( minDRHadJ,qMultMatchJet,wt);

	  h2_JetPtNearPhoMinDRHadJ->Fill(minDRHadJ,(*Jets)[minDRindx].Pt(),wt);//
	  h2_PhoPtMinDRHadJ->Fill(minDRHadJ,bestEMObj.Pt(),wt);
	  h2_PtPhoQHadFracJet->Fill(bestEMObj.Pt(),(*Jets_chargedHadronEnergyFraction)[minDRindx],wt);
	  h2_PhoPtQMultJet->Fill(bestEMObj.Pt(),qMultMatchJet,wt);

	  h2_PhoPtVtx->Fill(bestEMObj.Pt(),NVtx,wt);
	  h2_PhoEtaPhi->Fill(bestEMObj.Eta(),bestEMObj.Phi(),wt);
	  
	  h2_PtGvsMET->Fill( bestPhoton.Pt(),MET,wt);
	  //	  h2_RecoPhoPt_GenEPt->Fill(genElePt,bestEMObj.Pt(),wt);//------ MC only --------------
	  h2_JptGptRatiovsDR->Fill(minDR,((*Jets)[minDRindx].Pt())/(bestEMObj.Pt()),wt);
	  h2_HadDRJptGptRatio_Vtx->Fill( minDRHadJ*(((*Jets)[minDRindx].Pt())/(bestEMObj.Pt())),NVtx,wt);

	  h3_PhotonPtEtaNJ->Fill(abs(bestEMObj.Eta()),nHadJets,bestEMObj.Pt(),wt);
	  h3_PhoPtEtaVtx->Fill(abs(bestEMObj.Eta()),NVtx,bestEMObj.Pt(),wt);
	  h3_JetPtPhoEtaMinHadDR->Fill( abs(bestEMObj.Eta()),(*Jets)[minDRindx].Pt(),minDRHadJ,wt);
	  h3_JetPtVtxMinHadDR_Pho->Fill(((*Jets)[minDRindx].Pt()),NVtx,minDRHadJ,wt);

	  h3_PhoPtVtxMinHadDR->Fill( bestEMObj.Pt(),NVtx,minDRHadJ,wt );//
	  h3_PhoPtEtaMinDRHadJ->Fill( bestEMObj.Pt(),abs(bestEMObj.Eta()),minDRHadJ,wt );//
	  
	  if(remJetPt>0.1) h_RemjetPtNearPhoton->Fill(remJetPt,wt);
	  
	  h_dPhi_METG->Fill(dphiEM_MET,wt);
	  if(hadJets.size() > 0 ) h_dPhi_METjet1_Pho->Fill( dphi1,wt );
	  if(hadJets.size() > 1 ) h_dPhi_METjet2_Pho->Fill( dphi2,wt );
	  if(hadJets.size() > 2 ) h_dPhi_METjet3_Pho->Fill( dphi3,wt );
	  if(hadJets.size() > 3 ) h_dPhi_METjet4_Pho->Fill( dphi4,wt );

	  for(int i=0;i<hadJets.size() && i<4;i++){
	    h_jetPt_Pho[i] ->Fill(hadJets[i].Pt(),wt);
	    h_jetEta_Pho[i]->Fill(hadJets[i].Eta(),wt);
	    h_jetPhi_Pho[i]->Fill(hadJets[i].Phi(),wt);
	    h_dPhi_jet_Pho[i]->Fill(abs(bestEMObj.DeltaPhi(hadJets[i])),wt);
	  }
	  h_CM_Pho->Fill(qMultMatchJet,wt);
	  h_NHF_Pho->Fill((*Jets_neutralHadronEnergyFraction)[minDRindx],wt);
	  h_NEMF_Pho->Fill((*Jets_neutralEmEnergyFraction)[minDRindx],wt);
	  h_CHF_Pho->Fill((*Jets_chargedHadronEnergyFraction)[minDRindx],wt);
	  h_CEMF_Pho->Fill((*Jets_chargedEmEnergyFraction)[minDRindx],wt);
	  h_PhoFrac_Pho->Fill((*Jets_photonEnergyFraction)[minDRindx],wt);

	  // h_photon_pfQIsoRhoCorr->Fill( (*Photons_pfChargedIsoRhoCorr)[bestPhotonIndxAmongPhotons],wt);
	  // h_photon_pfNutIsoRhoCorr->Fill( (*Photons_pfNeutralIsoRhoCorr)[bestPhotonIndxAmongPhotons],wt);
	  
	  //	  findObjMatchedtoG(bestPhoton);//------ MC only --------------
	  // cout<<"------------------------------------------------------"<<endl;
	  // cout<<"Pt: "<<bestPhoton.Pt()<<" Eta:"<<bestPhoton.Eta()<<" Phi:"<<bestPhoton.Phi()<<endl;
	  // print(jentry);
	  wt=wt_org;
	}
		
	h_ST->Fill(ST,wt);
	h_MET->Fill(MET,wt);
	h_nHadJets->Fill(nHadJets,wt);
	h_BTags->Fill(BTags,wt);
	h_HT->Fill(HT,wt);
	h_MHT->Fill(MHT,wt);
	h_nJets->Fill(NJets,wt);
	h_METvBin->Fill(MET,wt);
	//	h_madHT->Fill(madHT,wt);//------ MC only --------------
	h_nVtx->Fill(NVtx,wt);

	h_EMObjPt->Fill(bestEMObj.Pt(),wt);
	h_EMObjEta->Fill(bestEMObj.Eta(),wt);
	h_EMObjPhi->Fill(bestEMObj.Phi(),wt);

	h2_EMObjPtEta->Fill(abs(bestEMObj.Eta()),bestEMObj.Pt(),wt);
	h2_PtEMObjvsMET->Fill(bestEMObj.Pt(),MET,wt);
	h_dPhi_METEMObj->Fill(dphiEM_MET,wt);

	if(hadJets.size() > 0 ) h_dPhi_METjet1->Fill( dphi1,wt );
	if(hadJets.size() > 1 ) h_dPhi_METjet2->Fill( dphi2,wt );
	if(hadJets.size() > 2 ) h_dPhi_METjet3->Fill( dphi3,wt );
	if(hadJets.size() > 3 ) h_dPhi_METjet4->Fill( dphi4,wt );

	for(int i=0;i<hadJets.size() && i<4;i++){
	  h_jetPt[i] ->Fill(hadJets[i].Pt(),wt);
	  h_jetEta[i]->Fill(hadJets[i].Eta(),wt);
	  h_jetPhi[i]->Fill(hadJets[i].Phi(),wt);
	}
	
      }
      for(int i=0;i<Jets->size();i++){
	if((*Jets)[i].Pt()>30 && (*Jets_ID)[i] ){
	  h_NM->Fill((*Jets_neutralMultiplicity)[i],wt);
	  if( abs((*Jets)[i].Eta())<3){h_NMEta3->Fill((*Jets_neutralMultiplicity)[i],wt);}
	  if( abs((*Jets)[i].Eta())>=3){h_NMHF->Fill((*Jets_neutralMultiplicity)[i],wt);}
	  h_CM->Fill((*Jets_chargedMultiplicity)[i],wt);
	  h_NHF->Fill((*Jets_neutralHadronEnergyFraction)[i],wt);
	  h_NEMF->Fill((*Jets_neutralEmEnergyFraction)[i],wt);
	  h_CHF->Fill((*Jets_chargedHadronEnergyFraction)[i],wt);
	  h_CEMF->Fill((*Jets_chargedEmEnergyFraction)[i],wt);

	  h_PhoMult->Fill((*Jets_photonMultiplicity)[i],wt);
	  h_PhoFrac->Fill((*Jets_photonEnergyFraction)[i],wt);
	  }
      }      
    }//process
    //}
    
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
  //  delete h2_FR,f_FR;
}


TLorentzVector FakeRateEst::getBestPhoton(){
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  allBestPhotons.resize(0);
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( (*Photons_fullID)[iPho] && ((*Photons_hasPixelSeed)[iPho]<0.001) ) {
      goodPho.push_back( (*Photons)[iPho] );
      goodPhoIndx.push_back(iPho);
      allBestPhotons.push_back((*Photons)[iPho]);
    }
  }
  
  int highPtIndx=-100;
  for(int i=0;i<goodPho.size();i++){
    if(i==0) highPtIndx=0;
    else if( (goodPho[highPtIndx].Pt()) < (goodPho[i].Pt()) ){highPtIndx=i;}
  }

  if(highPtIndx>=0){
    bestPhotonIndxAmongPhotons = goodPhoIndx[highPtIndx];
  }
  else bestPhotonIndxAmongPhotons = -100;
  if(highPtIndx==-100){TLorentzVector v0;return v0;}
  else return goodPho[highPtIndx];  
}


bool FakeRateEst::check_eMatchedtoGamma(){
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

void  FakeRateEst::findObjMatchedtoG(TLorentzVector bestPhoton){
  // double dR=100,mindr=1000;
  // int match=-100;
  // for(int i=0;i<GenParticles->size();i++){
  //   if((*GenParticles)[i].Pt()!=0){
  //     if(dR > (bestPhoton.DeltaR((*GenParticles)[i])) && ( (bestPhoton.Pt()/(*GenParticles)[i].Pt() > 0.5) && (bestPhoton.Pt()/(*GenParticles)[i].Pt()<1.5)) ){
  // 	dR=bestPhoton.DeltaR((*GenParticles)[i]);
  // 	match=i;
  //     }
  //   }
  // }
  // //cout<<"Pt: "<<bestPhoton.Pt()<<" Eta:"<<bestPhoton.Eta()<<" Phi:"<<bestPhoton.Phi()<<" PdgID:"<<(*GenParticles_PdgId)[match]<<" Pt:"<<(*GenParticles)[match].Pt()<<" Eta:"<<(*GenParticles)[match].Eta()<<" Phi:"<<(*GenParticles)[match].Phi()<<" parentId:"<<(*GenParticles_ParentId)[match]<<endl;
  // if(dR<0.2){
  //   h_GmatchedObj->Fill(abs((*GenParticles_PdgId)[match]),wt);
  //   if(abs((*GenParticles_PdgId)[match])==22)  h_PdgIdPhoParent->Fill(abs((*GenParticles_ParentId)[match]),wt);
  // }
  // else{
  //   h_GmatchedObj->Fill(0.0,wt);
  //   h_PdgIdPhoParent->Fill(0.0,wt);
  // }
}

void FakeRateEst::print(Long64_t jentry){
  //cout<<endl;
  TLorentzVector v1,photo;
  // for(int i=0;i<GenParticles->size();i++){
  //   cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<</*"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<*/"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
  // }

  // for(int i=0;i<GenJets->size();i++){
  //   if(i==0) cout<<"-------------------------------- GenJets -------------------------------------------"<<endl;
  //   cout<<"GenJetsPt:"<<(*GenJets)[i].Pt()<<" eta:"<<(*GenJets)[i].Eta()<<" phi:"<<(*GenJets)[i].Phi()<<endl;
  // }
  
  for(int i=0;i<Photons->size();i++){
    if(i==0) cout<<"-------------------------------- Photons -------------------------------------------"<<endl;
    double dR=0;//DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*Photons)[i].Eta(),(*Photons)[i].Phi() );
    //    cout<<jentry<<" i:"<<i<<" phoSize:"<<Photons->size()<<" Pt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" otherP:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" dR:"<<dR<<endl;
    cout<<"Photon Pt:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" E:"<<(*Photons)[i].Energy()<<" FullID:"<<(*Photons_fullID)[i]<<" Pixel Seed:"<<(*Photons_hasPixelSeed)[i]<<endl;
  }
  
  for(int i=0;i<TAPElectronTracks->size();i++){
    cout<<"Ele Track Pt:"<<(*TAPElectronTracks)[i].Pt()<<" Eta:"<<(*TAPElectronTracks)[i].Eta()<<" Phi:"<<(*TAPElectronTracks)[i].Phi()<<endl;
  }
  for(int i=0;i<TAPMuonTracks->size();i++){
    cout<<"Mu Track Pt:"<<(*TAPMuonTracks)[i].Pt()<<" Eta:"<<(*TAPMuonTracks)[i].Eta()<<" Phi:"<<(*TAPMuonTracks)[i].Phi()<<endl;
  }
  for(int i=0;i<TAPPionTracks->size();i++){
    cout<<"Pi Track Pt:"<<(*TAPPionTracks)[i].Pt()<<" Eta:"<<(*TAPPionTracks)[i].Eta()<<" Phi:"<<(*TAPPionTracks)[i].Phi()<<endl;
  }

  for(int i=0;i<Jets->size();i++){
    //  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
    cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<" energy:"<<(*Jets)[i].Energy()<<" QMult:"<<(*Jets_chargedMultiplicity)[i]<<endl;
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
