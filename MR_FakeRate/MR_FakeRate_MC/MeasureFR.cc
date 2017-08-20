#define MeasureFR_cxx
#include "MeasureFR.h"
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

  MeasureFR ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void MeasureFR::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;
  // cout<<"--------------------------------"<<
  //     <<"warning::::This code might be double counting some events"
  //     <<"--------------------------------"<<endl;
  string s_data=data;
  bool mcSample=true;
  if(s_data=="Data") mcSample=false;    
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  int tagCharge=1;  
  int evtSurvived=0;
  bool ignoreTag=0,applSF=1;
  //---------------------- MC only -----------------------
  TFile* pufile = TFile::Open("PileupHistograms_0121_69p2mb_pm4p6.root","READ");
  //choose central, up, or down
  TH1* puhist = (TH1*)pufile->Get("pu_weights_down");
  if(ignoreTag) cout<<"********** Ignoring Tag and adding as MET ************"<<endl;
  cout<<"Applying SF? "<<applSF<<endl;
  //---------------------- MC only ends-----------------------
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
    wt=Weight*lumiInfb*1000.0*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));//------ MC only -----
    //cout<<CrossSection<<" NEvt:"<<NumEvents<<" Wt:"<<Weight<<endl;
    bool process=true;
    if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0)) continue;

    if( (Muons->size()!=0) || (Electrons->size()==0) || (Electrons->size()>2) ) continue;
    if(isoMuonTracks!=0 || isoPionTracks!=0) continue;

    bool passReqTrg=false;
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back(); trgName.pop_back();
      if( (trgName=="HLT_Ele27_WPTight_Gsf_v") || (trgName=="HLT_Ele27_WPTight_Gsf_") ){
    	if( (*TriggerPass)[i]==1 && (*TriggerPrescales)[i]==1 ){
    	  passReqTrg = true;
    	  break;
    	}
    	else if((*TriggerPass)[i]==-1 || (*TriggerPrescales)[i]!=1) 
    	  cout<<"AHHHHH, trigger pass:"<<(*TriggerPass)[i]<<" PS:"<<(*TriggerPrescales)[i]<<endl;
      }
    }
    //    if(!passReqTrg) continue;
    //	cout<<trgName<<" PS:"<<(*TriggerPrescales)[i]<<" Dec:"<<(*TriggerPass)[i]<<endl;
    //++++++++++++++++++++++++++ data only ++++++++++++++++++++++++++++++
    //       Trigger selections
    /*
      bool passReqTrg=false;
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back(); trgName.pop_back();
      if( (trgName=="HLT_Ele27_WPTight_Gsf_v") || (trgName=="HLT_Ele27_WPTight_Gsf_") ){
    	if( (*TriggerPass)[i]==1 && (*TriggerPrescales)[i]==1 ){
    	  passReqTrg = true;
    	  break;
    	}
    	else if((*TriggerPass)[i]==-1 || (*TriggerPrescales)[i]!=1) 
    	  cout<<"AHHHHH, trigger pass:"<<(*TriggerPass)[i]<<" PS:"<<(*TriggerPrescales)[i]<<endl;
      }
    }
    if(!passReqTrg) {cout<<"Trigger failed."<<endl;continue;}
   
    //match the electrons to L1 and HLT objects. Note that only those objects(pt > 10) associated with HLT_Ele27_WPTight_Gsf path are saved.
    vector<bool> l1MatchEle, hltMatchEle;
    for(int i=0;i<Electrons->size();i++){
      bool l1match=false,hltmatch=false;
      for(int j=0;j<L1EGObject->size();j++){
    	if((*Electrons)[i].DeltaR((*L1EGObject)[j]) < 0.2){
    	  l1match = true; break;
    	}
      }
      for(int j=0;j<HLTElectronObjects->size();j++){
    	if((*Electrons)[i].DeltaR((*HLTElectronObjects)[j]) < 0.2){
    	  hltmatch = true; break;
    	}
      }
      l1MatchEle.push_back(l1match);
      hltMatchEle.push_back(hltmatch);
    }
    for(i=0;i<Electrons->size();i++){
      cout<"Ept:"<<<(*Electrons)[i].Pt()<<" EEta:"<<(*Electrons)[i].Eta()<<" Ephi:"<<(*Electrons)[i].Phi()<<" TightID:"<<(*Electrons_tightID)[i]<<" L1Match:"<<l1MatchEle[i]<<" HLTMatch:"<<hltMatchEle[i]<<endl;
    }
*/
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++    

    //about photons
    bool hasPho = false;
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt()>100) hasPho=true;  
    bool eMatchedG=false;
    if(hasPho) eMatchedG = check_eMatchedtoGamma();

    //about electrons
    int oddEvtNum = -100, extIsoEleTracks=0;
    TLorentzVector elef,eler;
    if(Electrons->size()==2 && (*Electrons)[0].Pt() < (*Electrons)[1].Pt()) cout<<"!!!!!!unsorted electrons collection"<<endl;
    if(Electrons->size()==1){
      if(hasPho && eMatchedG) continue;
      elef = bestPhoton;
      TLorentzVector v1;
      v1.SetPtEtaPhiE(MET,0.,METPhi,0.);
      v1 = v1 + eler;
      double psudoMET = v1.Pt(), psudoMETPhi = v1.Phi();
      for(int i=0;i<TAPElectronTracks->size();i++){
	if( ((*Electrons)[0].DeltaR((*TAPElectronTracks)[i]) > 0.2)){
	  double psudomt = sqrt(2*((*TAPElectronTracks)[i].Pt()*psudoMET*(1-cos(DeltaPhi(psudoMETPhi,(*TAPElectronTracks)[i].Phi())))));
	  //	  if( ((*TAPElectronTracks)[i].Pt()>=5) && ((*TAPElectronTracks_trkiso)[i]<0.2) && psudomt < 100){ extIsoEleTracks++; }
	  if( ((*TAPElectronTracks)[i].Pt()>=5) && ((*TAPElectronTracks_trkiso)[i]<0.2)){ extIsoEleTracks++; }
	}
      }
      //      if(extIsoEleTracks>0) continue;
      if(!((*Electrons_tightID)[0])) continue;//tag e. tight ID
      eler = (*Electrons)[0];
      //      if((*Electrons_charge)[0]!=tagCharge) continue;
    }
    if(Electrons->size()==2){
      if( !((*Electrons_tightID)[0]) && !((*Electrons_tightID)[1]) ) continue;//LL ee
      else if( ((*Electrons_tightID)[0]) && !((*Electrons_tightID)[1]) ){//TL ee
      	eler = (*Electrons)[0];
      	elef = (*Electrons)[1];
	continue;//reject TL
      }
      else if( !((*Electrons_tightID)[0]) && ((*Electrons_tightID)[1]) ){//LT ee
      	eler = (*Electrons)[1];
      	elef = (*Electrons)[0];
	continue;//reject TL
      }
      else if( ((*Electrons_tightID)[0]) && ((*Electrons_tightID)[1]) ){//TT ee
      	if(((*Electrons)[0].Pt() > 100) && ((*Electrons)[1].Pt() < 100) ){// choose leading as probe
      	  elef = (*Electrons)[0];
      	  eler = (*Electrons)[1];
      	}
      	else if( ((*Electrons)[0].Pt() > 100) && ((*Electrons)[1].Pt() > 100) ){// choose tag ramdomly.
      	  oddEvtNum = EvtNum%2;
      	  if(EvtNum == 0) cout<<"Evt Num Invalid. Cannot choose tag electron randomly"<<endl;
      	  if( oddEvtNum == 0 ){//if event# is even, choose leading e as tag.
      	    eler = (*Electrons)[0];
      	    elef = (*Electrons)[1];
      	  }
      	  else if( oddEvtNum == 1 ){//if event# is odd, choose leading e as probe.
      	    eler = (*Electrons)[1];
      	    elef = (*Electrons)[0];
      	  }
      	  wt=wt*2.0;
      	}
      	h_LeadEleIsProbe->Fill(oddEvtNum,wt);
      }
      else cout<<"logic issue in choosing tag and probe from ee"<<endl;
      // if( !((*Electrons_tightID)[0]) && !((*Electrons_tightID)[1]) ) continue;//LL ee
      // else if( ((*Electrons_tightID)[0]) && (*Electrons_charge)[0]==tagCharge ){//leading Tag, req Q
      // 	eler = (*Electrons)[0];
      // 	elef = (*Electrons)[1];
      // }
      // else if( ((*Electrons_tightID)[1]) && (*Electrons_charge)[1]==tagCharge ){//2nd leading is tag, req Q
      // 	eler = (*Electrons)[1];
      // 	elef = (*Electrons)[0];
      // }
      // else continue;
    }
    if(ignoreTag){
      TLorentzVector v1;
      v1.SetPtEtaPhiE(MET,0.,METPhi,0.);
      v1 = v1 + eler;
      MET = v1.Pt();
      METPhi = v1.Phi();
    }
    if(elef.Pt() < 100) continue;//probe e or fake photon Pt > 100.
    if(eler.Pt() < 40.) continue;
    if(elef.DeltaR(eler) < 0.2) continue;
    //    if(!((*Electrons_tightID)[1])) continue;
    if((Electrons->size()==2) && hasPho) continue;//reject event if there are 2e and photon
    
    //---------------------- MC only -----------------------
    bool fakePhoton=false;
    if(Electrons->size()==1){
      for(int i=0;i<GenParticles->size();i++){
        if((*GenParticles)[i].Pt()!=0){
          double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
    	  if(dr1 < 0.2 && (abs((*GenParticles_PdgId)[i])==11) && (abs((*GenParticles_ParentId)[i])<=24) ){
    	    // if(dr1 < 0.2 && (abs((*GenParticles_PdgId)[i])==11) && (abs((*GenParticles_ParentId)[i])<=24) &&
    	    //  ( ((*GenParticles)[i].Pt()/bestPhoton.Pt()) > 0.5) && ((*GenParticles)[i].Pt()/bestPhoton.Pt()) < 1.5 ){
            fakePhoton=true;
          }
        }
      }
      //      if(!fakePhoton) continue;
      if(isoMuonTracks!=0 || isoPionTracks!=0) continue;
      //      print(jentry);
    }
    bool elerGenMatch=false,elefGenMatch=false;
    for(int i=0;i<GenParticles->size();i++){
      if( ((*GenParticles)[i].Pt()!=0) && (abs((*GenParticles_PdgId)[i])==11) && (abs((*GenParticles_ParentId)[i])==23) ){
    	if( eler.DeltaR((*GenParticles)[i]) < 0.2 ) elerGenMatch = true;
    	if( elef.DeltaR((*GenParticles)[i]) < 0.2 ) elefGenMatch = true;
      }
    }
    if(!elerGenMatch || !elefGenMatch) continue;
    //---------------------- MC only ends -----------------------
    //    if(elef.Pt() < eler.Pt()) continue;//if photon Pt < ele Pt, reject events. for ee events, this does nothing.
    TLorentzVector zvec;
    double zmass= -10;
    zvec =  elef + eler;
    zmass = zvec.M();
    if(zmass < 80 || zmass > 100) continue;

    //calulate ST and number of Hadronic Jets
    bool hadJetID = true;
    int minDRindx_f=-100,minDRindx_r=-100,nHadJets=0;
    double minDR_f=99999,minDR_r=9999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	if(minDR_f > elef.DeltaR((*Jets)[i]) ){ 
	  minDR_f = elef.DeltaR((*Jets)[i]);
 	  minDRindx_f = i;
	}
	if(minDR_r > eler.DeltaR((*Jets)[i]) ){ 
	  minDR_r = eler.DeltaR((*Jets)[i]);
 	  minDRindx_r = i;
	}
      }
    }  
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	if( !(minDR_f < 0.3 && i==minDRindx_f) && !(minDR_r < 0.3 && i==minDRindx_r) ){
	  hadJets.push_back((*Jets)[i]);
	  if(hadJetID){ hadJetID = (*Jets_ID)[i]; }
	}
      }
    }
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    if( minDR_f < 0.3 ) ST=ST+elef.Pt();
    //    if( minDR_r < 0.3 ) ST=ST+eler.Pt();
    sortTLorVec(&hadJets);

    //ST and HadJets have been determined. Now calulate dPhi b/w MET and leading HadJets.
    double dphi1=3.8,dphi2=3.8,dphi3=3.8,dphi4=3.8,dphiPho_MET=3.8,dphi_elef_eler=3.8,dphi_elefMET=3.8,dPhi_elerMET=3.8;
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));
    if(hadJets.size() > 2 ) dphi3 = abs(DeltaPhi(METPhi,(hadJets)[2].Phi()));
    if(hadJets.size() > 3 ) dphi4 = abs(DeltaPhi(METPhi,(hadJets)[3].Phi()));
    dphi_elef_eler = abs(eler.DeltaPhi(elef));
    dphi_elefMET   = abs(DeltaPhi(METPhi,elef.Phi()));
    dPhi_elerMET   = abs(DeltaPhi(METPhi,eler.Phi()));
    int qMultJetElef = 0, qMultJetEler =0;
    if(minDR_f < 0.3) qMultJetElef = (*Jets_chargedMultiplicity)[minDRindx_f];
    if(minDR_r < 0.3) qMultJetEler = (*Jets_chargedMultiplicity)[minDRindx_r];

	
    //apply baseline selections
    if(ignoreTag)
      process = process && ST > 200 && MET > 100 && nHadJets>=2 && dphi1 > 0.3 && dphi2 > 0.3 && elef.Pt() > 100;
    else{ 
      //process = process && ST>500 && MET < 100 && nHadJets >=2 && elef.Pt() > 100;
      process = process && ST > 200 && MET < 100 && nHadJets >=2 && elef.Pt() > 100;
    }
    //    process = process && ST>500 && MET > 100 && nHadJets >=2 && dphi1 > 0.3 && dphi2 > 0.3 && elef.Pt() > 100;
    if(process && hadJetID){
      evtSurvived++;
      h_RunNum->Fill(RunNum);
      h_intLumi->Fill(lumiInfb);
      h_ST->Fill(ST,wt);
      h_MET->Fill(MET,wt);
      h_nHadJets->Fill(nHadJets,wt);
      h_BTags->Fill(BTags,wt);
      h_METvBin->Fill(MET,wt);

      h_ZMass->Fill(zmass,wt);
      h_METPhi->Fill(METPhi,wt);
      int searchRegion=0;
      if     (nHadJets >= 2 && nHadJets <= 4 ) searchRegion=1;
      else if(nHadJets == 5 || nHadJets == 6 ) searchRegion=2;
      else if(nHadJets >= 7                  ) searchRegion=3;

      if(!hasPho){
	h_1Bin_Ele->Fill(1.0,wt);
	h_ST_Ele->Fill(ST,wt);
	h_MET_Ele->Fill(MET,wt);
	h_nHadJets_Ele->Fill(nHadJets,wt);
	h_BTags_Ele->Fill(BTags,wt);
	h_nJets_Ele->Fill(NJets,wt);
	h_METvBin_Ele->Fill(MET,wt);
	h_ZMass_Ele->Fill(zmass,wt);
	h_METPhi_Ele->Fill(METPhi,wt);
	h_nVtx_Ele->Fill(NVtx,wt);
	h_isoEleTracks_Ele->Fill(isoElectronTracks,wt);
	h_EleTracks_Ele->Fill(TAPElectronTracks->size(),wt);
	h_MuTracks_Ele->Fill(TAPMuonTracks->size(),wt);
	h_PiTracks_Ele->Fill(TAPPionTracks->size(),wt);
	h_nTracks_Ele->Fill(TAPElectronTracks->size()+TAPMuonTracks->size()+TAPPionTracks->size(),wt);

	h_ElefPt_Ele->Fill(elef.Pt(),wt);
	h_ElefEta_Ele->Fill(elef.Eta(),wt);
	h_ElefPhi_Ele->Fill(elef.Phi(),wt);
	h_ElefPtvBin_Ele->Fill(elef.Pt(),wt);

	h_ElerPt_Ele->Fill(eler.Pt(),wt);
	h_ElerEta_Ele->Fill(eler.Eta(),wt);
	h_ElerPhi_Ele->Fill(eler.Phi(),wt);

	h_dPhi1_Ele->Fill(dphi1,wt);
	h_dPhi2_Ele->Fill(dphi2,wt);
	h_dphi_elefMET_Ele->Fill(dphi_elefMET,wt);
	h_dPhi_elerMET_Ele->Fill(dPhi_elerMET,wt);
	h_dphi_elef_eler_Ele->Fill(dphi_elef_eler,wt);

	h_mTPrbMET_Ele->Fill(sqrt(2*(elef.Pt()*MET*(1-cos(DeltaPhi(METPhi,elef.Phi()))))),wt); 
	h_minDRJetElef_Ele->Fill(minDR_f,wt);
	h_QMultJetElef_Ele->Fill(qMultJetElef,wt);
	h_QMultJetEler_Ele->Fill(qMultJetEler,wt);

	h2_ElefPtQMultJet_Ele->Fill(elef.Pt(),qMultJetElef,wt);
	h2_ElefPtElerPt_Ele->Fill(elef.Pt(),eler.Pt(),wt);
	h2_ElePtVtx->Fill(elef.Pt(),NVtx,wt);
	h2_ElePtEtavBin->Fill(abs(elef.Eta()),wt);
	// if(qMultJetEler > 15 && qMultJetEler < 25){
	//   cout<<"QMult: "<<qMultJetEler<<" JetPt:"<<(*Jets)[minDRindx_r].Pt()<<" Eta:"<<(*Jets)[minDRindx_r].Eta()<<" Phi:"<<(*Jets)[minDRindx_r].Phi()<<endl;
	//   print(jentry);
	// }
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_Ele_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! "<<searchRegion;
	//    if(Muons->size()==0){
      }//ee events

      else{
	if(applSF){
	  // if(qMultJetElef < 2) wt = wt*1.065;//no BG sub
	  // else wt = wt*1.929;//no BG sub
	  if(qMultJetElef < 2) wt = wt*0.923;
	  else wt = wt*1.66;
	}
	findObjMatchedtoG(bestPhoton);
	if(allBestPhotons.size()>1) h_Pho2Pt->Fill(allBestPhotons[1].Pt(),wt);
	h_1Bin_Pho->Fill(1.0,wt);
	h_ST_Pho->Fill(ST,wt);
	h_MET_Pho->Fill(MET,wt);
	h_nHadJets_Pho->Fill(nHadJets,wt);
	h_BTags_Pho->Fill(BTags,wt);
	h_nJets_Pho->Fill(NJets,wt);
	h_METvBin_Pho->Fill(MET,wt);
	h_ZMass_Pho->Fill(zmass,wt);
	h_METPhi_Pho->Fill(METPhi,wt);
	h_nVtx_Pho->Fill(NVtx,wt);
	h_isoEleTracks_Pho->Fill(isoElectronTracks,wt);
	h_EleTracks_Pho->Fill(TAPElectronTracks->size(),wt);
	h_MuTracks_Pho->Fill(TAPMuonTracks->size(),wt);
	h_PiTracks_Pho->Fill(TAPPionTracks->size(),wt);
	h_nTracks_Pho->Fill(TAPElectronTracks->size()+TAPMuonTracks->size()+TAPPionTracks->size(),wt);

	h_ElefPt_Pho->Fill(elef.Pt(),wt);
	h_ElefEta_Pho->Fill(elef.Eta(),wt);
	h_ElefPhi_Pho->Fill(elef.Phi(),wt);
	h_ElefPtvBin_Pho->Fill(elef.Pt(),wt);

	h_ElerPt_Pho->Fill(eler.Pt(),wt);
	h_ElerEta_Pho->Fill(eler.Eta(),wt);
	h_ElerPhi_Pho->Fill(eler.Phi(),wt);
 
	h_dPhi1_Pho->Fill(dphi1,wt);
	h_dPhi2_Pho->Fill(dphi2,wt);
	h_dphi_elefMET_Pho->Fill(dphi_elefMET,wt);
	h_dPhi_elerMET_Pho->Fill(dPhi_elerMET,wt);
	h_dphi_elef_eler_Pho->Fill(dphi_elef_eler,wt);

	h_mTPrbMET_Pho->Fill(sqrt(2*(elef.Pt()*MET*(1-cos(DeltaPhi(METPhi,elef.Phi()))))),wt); 
	h_minDRJetElef_Pho->Fill(minDR_f,wt);
	h_QMultJetElef_Pho->Fill(qMultJetElef,wt);
	h_QMultJetEler_Pho->Fill(qMultJetEler,wt);

	h2_ElefPtQMultJet_Pho->Fill(elef.Pt(),qMultJetElef,wt);
	h2_ElefPtElerPt_Pho->Fill(elef.Pt(),eler.Pt(),wt);
	h2_PhoPtVtx->Fill(elef.Pt(),NVtx,wt);
	h2_PhoPtEtavBin->Fill(abs(elef.Eta()),wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_Pho_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! ";
      }//e-g events
    }
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
}


TLorentzVector MeasureFR::getBestPhoton(){
  // bool passIDVeryLoose=false,passIDLoose=false,passIsoVeryLoose=false,passIsoLoose=false;
  bool ematch=false;
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  allBestPhotons.resize(0);
  for(int iPho=0;iPho<Photons->size();iPho++){
    for(int i=0;i<Electrons->size();i++){
      if( (*Photons)[iPho].DeltaR( (*Electrons)[i] ) < 0.2){
	ematch = true; break;
      }
    }
    if( ((*Photons_fullID)[iPho]) && ((*Photons_hasPixelSeed)[iPho]<0.001) && !ematch) {
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


bool MeasureFR::check_eMatchedtoGamma(){
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

void  MeasureFR::findObjMatchedtoG(TLorentzVector bestPhoton){
  
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
}



void MeasureFR::print(Long64_t jentry){
  //cout<<endl;
  TLorentzVector v1,photo;
  for(int i=0;i<GenParticles->size();i++){
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<</*"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<*/"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
  }

  for(int i=0;i<GenJets->size();i++){
    cout<<"GenJetsPt:"<<(*GenJets)[i].Pt()<<" eta:"<<(*GenJets)[i].Eta()<<" phi:"<<(*GenJets)[i].Phi()<<endl;
  }
  
  for(int i=0;i<Photons->size();i++){
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
    // if(Muons->size()==2){
    //   cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<" energy:"<<(*Jets)[i].Energy()<<" dR with Muon: "<<(*Jets)[i].DeltaR((*Muons)[0])<<endl;
    // }
    // if(Electrons->size()==2){
    //   cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<" energy:"<<(*Jets)[i].Energy()<<" dR with electron: "<<(*Jets)[i].DeltaR((*Electrons)[0])<<endl;
    // }
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
