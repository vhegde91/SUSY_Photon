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
  
  int evtSurvived=0;

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
    wt=Weight*lumiInfb*1000.0;
    //cout<<CrossSection<<" NEvt:"<<NumEvents<<" Wt:"<<Weight<<endl;
    bool process=true;
    if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0)) continue;

    if( (Muons->size()!=0) || (Electrons->size()==0) || (Electrons->size()>2) ) continue;
    if(isoMuonTracks!=0 || isoPionTracks!=0) continue;

    //about photons
    bool hasPho = false;
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt()>100) hasPho=true;  
    bool eMatchedG=false;
    if(hasPho) eMatchedG = check_eMatchedtoGamma();
    if(hasPho && eMatchedG) continue;

    int ngenE=0,ignoreIdx=-100;
    TLorentzVector gene1,gene2;
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if( abs((*GenParticles_PdgId)[i])==11 && abs((*GenParticles_ParentId)[i])<=24){
	  if(ngenE==0) gene1 = (*GenParticles)[i];
	  else gene2 = (*GenParticles)[i];
	  ngenE++;
	}
      }
    }
    double dr2=10000.;
    if( (EvtNum%2)==0){
      for(int i=0;i<Electrons->size();i++){
	if(dr2 > gene1.DeltaR((*Electrons)[i])){
	  dr2=gene1.DeltaR((*Electrons)[i]);
	  if(dr2<0.2) ignoreIdx = i;
	}
      }
      if(hasPho && dr2 > bestPhoton.DeltaR(gene1) ){
	dr2 = bestPhoton.DeltaR(gene1); 
	if(dr2<0.2) ignoreIdx = -1;
      }
    }
    else{
      for(int i=0;i<Electrons->size();i++){
	if(dr2 > gene2.DeltaR((*Electrons)[i])){
	  dr2=gene2.DeltaR((*Electrons)[i]);
	  if(dr2<0.2) ignoreIdx = i;
	}
      }
      if(hasPho && dr2 > bestPhoton.DeltaR(gene2) ){
	dr2 = bestPhoton.DeltaR(gene2); 
	if(dr2<0.2) ignoreIdx = -1;
      }
    }
    TLorentzVector v1,elef,eler;
    vector<TLorentzVector> myElectrons;
    v1.SetPtEtaPhiE(MET,0.,METPhi,0.);
    if(ignoreIdx = -1){    v1=v1+bestPhoton; bestPhoton.SetPtEtaPhiE(0.,0.,0.,0.);}
    else if(ignoreIdx = 0) v1=v1+(*Electrons)[0];
    else if(ignoreIdx = 1) v1=v1+(*Electrons)[1];
    else continue;
    MET = v1.Pt();
    METPhi = v1.Phi();

    if(Electrons->size()>2) continue;
    for(int i=0;i<Electrons->size();i++){
      if(i!=ignoreIdx) myElectrons.push_back((*Electrons)[i]);
    }

    if(myElectrons.size()>1) continue;
    else if(myElectrons.size()==1) elef = myElectrons[0];
    else if(myElectrons.size()==0){
      elef = bestPhoton;
      if(isoElectronTracks!=0) continue;
    }

    if(elef.Pt()<100) continue;   
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
      // for(int i=0;i<GenParticles->size();i++){
      //   if((*GenParticles)[i].Pt()!=0){
      //     double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
      //     if( dr1<0.2 && (abs((*GenParticles_PdgId)[i])==22) &&
      //         ( ((*GenParticles)[i].Pt()/bestPhoton.Pt()) > 0.9) && ((*GenParticles)[i].Pt()/bestPhoton.Pt()) < 1.1 )  fakePhoton=false;
      //   }
      // }
      if(!fakePhoton) continue;
      if(isoMuonTracks!=0 || isoPionTracks!=0) continue;
      //      print(jentry);
    }

    //    if(elef.Pt() < eler.Pt()) continue;//if photon Pt < ele Pt, reject events. for ee events, this does nothing.
    TLorentzVector zvec;
    double zmass= -10;
    //    if(!fakePhoton) print(jentry);
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
      }
    }  
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	//	if( !(minDR_f < 0.3 && i==minDRindx_f) && !(minDR_r < 0.3 && i==minDRindx_r) ){
	if( !(minDR_f < 0.3 && i==minDRindx_f) ){
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
    //if( minDR_r < 0.3 ) ST=ST+eler.Pt();
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
    //vvvvvvvvvvvvvvvvvvvvvvvvv MC only vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    /*    double genPho1Pt=0.;
    for(int i=0;i<GenParticles->size();i++){
      if( (abs((*GenParticles_PdgId)[i])==22)
	  && ((abs((*GenParticles_ParentId)[i]) <= 100) ) 
	  && ( ((*GenParticles_Status)[i]==1) || (((*GenParticles_Status)[i]/10)==2) )){
	if(genPho1Pt < (*GenParticles)[i].Pt()) 
	  genPho1Pt = (*GenParticles)[i].Pt();
      }
    }
    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    wt=wt*0.98;*/
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    //++++++++++++++++++++++++++ data only ++++++++++++++++++++++++++++++
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
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++    

    //apply baseline selections
    //    process = process && ST>200 && MET > 100 && nHadJets >=2 && dphi1 > 0.3 && dphi2 > 0.3 && elef.Pt() > 100;
    //    process = process && ST>200 && nHadJets >=2 && elef.Pt() > 100;
    process = process && ST>500 && MET > 100 && nHadJets >=2 && dphi1 > 0.3 && dphi2 > 0.3 && elef.Pt() > 100;
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
	h_ST_ee->Fill(ST,wt);
	h_MET_ee->Fill(MET,wt);
	h_nHadJets_ee->Fill(nHadJets,wt);
	h_BTags_ee->Fill(BTags,wt);
	h_nJets_ee->Fill(NJets,wt);
	h_METvBin_ee->Fill(MET,wt);
	h_ZMass_ee->Fill(zmass,wt);
	h_METPhi_ee->Fill(METPhi,wt);
	h_nVtx_ee->Fill(NVtx,wt);
	h_isoEleTracks_ee->Fill(isoElectronTracks,wt);

	h_ElefPt_ee->Fill(elef.Pt(),wt);
	h_ElefEta_ee->Fill(elef.Eta(),wt);
	h_ElefPhi_ee->Fill(elef.Phi(),wt);

	h_ElerPt_ee->Fill(eler.Pt(),wt);
	h_ElerEta_ee->Fill(eler.Eta(),wt);
	h_ElerPhi_ee->Fill(eler.Phi(),wt);

	h_dPhi1_ee->Fill(dphi1,wt);
	h_dPhi2_ee->Fill(dphi2,wt);
	h_dphi_elefMET_ee->Fill(dphi_elefMET,wt);
	h_dPhi_elerMET_ee->Fill(dPhi_elerMET,wt);
	h_dphi_elef_eler_ee->Fill(dphi_elef_eler,wt);
 
	h_minDRJetElef_ee->Fill(minDR_f,wt);
	h_QMultJetElef_ee->Fill((*Jets_chargedMultiplicity)[minDR_f],wt);
	h_QMultJetEler_ee->Fill((*Jets_chargedMultiplicity)[minDR_r],wt);

	h2_ElefPtQMultJet_ee->Fill(elef.Pt(),(*Jets_chargedMultiplicity)[minDR_f],wt);
	h2_ElefPtElerPt_ee->Fill(elef.Pt(),eler.Pt(),wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_ee_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! "<<searchRegion;
	//    if(Muons->size()==0){
      }//ee events

      else{
	findObjMatchedtoG(bestPhoton);
	if(allBestPhotons.size()>1) h_Pho2Pt->Fill(allBestPhotons[1].Pt(),wt);
	h_ST_ge->Fill(ST,wt);
	h_MET_ge->Fill(MET,wt);
	h_nHadJets_ge->Fill(nHadJets,wt);
	h_BTags_ge->Fill(BTags,wt);
	h_nJets_ge->Fill(NJets,wt);
	h_METvBin_ge->Fill(MET,wt);
	h_ZMass_ge->Fill(zmass,wt);
	h_METPhi_ge->Fill(METPhi,wt);
	h_nVtx_ge->Fill(NVtx,wt);
	h_isoEleTracks_ge->Fill(isoElectronTracks,wt);

	h_ElefPt_ge->Fill(elef.Pt(),wt);
	h_ElefEta_ge->Fill(elef.Eta(),wt);
	h_ElefPhi_ge->Fill(elef.Phi(),wt);

	h_ElerPt_ge->Fill(eler.Pt(),wt);
	h_ElerEta_ge->Fill(eler.Eta(),wt);
	h_ElerPhi_ge->Fill(eler.Phi(),wt);
 
	h_dPhi1_ge->Fill(dphi1,wt);
	h_dPhi2_ge->Fill(dphi2,wt);
	h_dphi_elefMET_ge->Fill(dphi_elefMET,wt);
	h_dPhi_elerMET_ge->Fill(dPhi_elerMET,wt);
	h_dphi_elef_eler_ge->Fill(dphi_elef_eler,wt);

	h_minDRJetElef_ge->Fill(minDR_f,wt);
	h_QMultJetElef_ge->Fill((*Jets_chargedMultiplicity)[minDRindx_f],wt);
	h_QMultJetEler_ge->Fill((*Jets_chargedMultiplicity)[minDRindx_r],wt);

	h2_ElefPtQMultJet_ge->Fill(elef.Pt(),(*Jets_chargedMultiplicity)[minDR_f],wt);
	h2_ElefPtElerPt_ge->Fill(elef.Pt(),eler.Pt(),wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_ge_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! ";
      }//e-g events
    }
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
}


TLorentzVector MeasureFR::getBestPhoton(){
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
    // cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<</*"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<*/"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
  }
  
  for(int i=0;i<Photons->size();i++){
    double dR=0;//DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*Photons)[i].Eta(),(*Photons)[i].Phi() );
    //    cout<<jentry<<" i:"<<i<<" phoSize:"<<Photons->size()<<" Pt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" otherP:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" dR:"<<dR<<endl;
    cout<<"Photon Pt:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" E:"<<(*Photons)[i].Energy()<<endl;
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
