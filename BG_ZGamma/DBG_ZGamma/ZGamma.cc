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
  bool ignoreLeps=true;
  if(ignoreLeps) cout<<"********** Adding leptons to MET ***********"<<endl;
  else cout<<"********** NOT adding leptons to MET. Leptons' Pt will be added to ST.  ***********"<<endl;
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
    wt=1.0;
    
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
    if(isoPionTracks!=0) continue;

    if( (Electrons->size()==2) ){
      if( (Muons->size()!=0) ) continue;//exactly 0 RECO muons and 2 RECO e
      if(isoMuonTracks!=0) continue;
      if( (*Electrons_charge)[0] == (*Electrons_charge)[1] ) continue;//opp sign same flav(OSSF)
    }
    else if( (Muons->size()==2) ){
      if(Electrons->size()!=0 ) continue;//exactly 2 RECO muons and 0 RECO e
      if(isoElectronTracks!=0) continue;
      if( (*Muons_charge)[0] == (*Muons_charge)[1] ) continue;//opp sign same flav(OSSF)
    }
    else if(Muons->size()==1 && Electrons->size()==1){//exactly 1 RECO muon and 1 RECO e
      if((*Muons_charge)[0] == (*Electrons_charge)[0]) continue;//opp sign same flav(OSOF)
    }
    else continue;
    TLorentzVector zvec;
    double zmass= -10;
    if(Muons->size()==2){
      zvec =  ((*Muons)[0] + (*Muons)[1]);
      zmass = zvec.M();
    }
    if(Electrons->size()==2 ){
      zvec =  ((*Electrons)[0] + (*Electrons)[1]);
      zmass = zvec.M();
    }
    if(Muons->size()==1 && Electrons->size()==1){
      zvec =  ((*Muons)[0] + (*Electrons)[0]);
      zmass = zvec.M();
    }
    //    if(zvec.M() < 80 || zvec.M() > 100) continue;
    // if(Muons->size()==1){
    //   print(0);
    //   cout<<zmass<<endl;
    // }
    TLorentzVector metstar,metvec;
    metvec.SetPtEtaPhiE(MET,0.0,METPhi,0.0);
    if(ignoreLeps)  metstar = metvec + zvec;
    else metstar = metvec;
    
    //Trigger selections
    bool passHT600Photon90Trigger = false;//(ST>700 && bestPhoton.Pt()>100);
    bool passPhoton165HE10Trigger = false;//(bestPhoton.Pt()>180);
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back();
      if( trgName=="HLT_Photon90_CaloIdL_PFHT600_v" && (*TriggerPass)[i]==1 ) passHT600Photon90Trigger = true;
      else if( trgName=="HLT_Photon165_HE10_v" && (*TriggerPass)[i]==1 ) passPhoton165HE10Trigger = true;
      //      cout<<(*TriggerNames)[i]<<" "<<(*TriggerPass)[i]<<" "<<(*TriggerPrescales)[i]<<endl;
      //      cout<<(*TriggerPrescales)[i]<<" "<<(*TriggerNames)[i]<<" "<<endl;
    }
    // cout<<"PhoHT: "<<passHT600Photon90Trigger<<" Pho: "<<passPhoton165HE10Trigger<<endl;
    //    cout<<"--------------------------------------------------------------------------"<<endl;
    if(!passPhoton165HE10Trigger && !passHT600Photon90Trigger) continue;

    //calulate ST and HadJets by cleaning the matching jet.
    bool hadJetID=true;
    int minDRindx=-100,phoMatchingJetIndx=-100,nHadJets=0, minDRindxl1=-100, minDRindxl2=-100;
    double minDR=99999, ST=0, minDRl1=10000, minDRl2=10000;
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
	else if((Electrons->size()==1) && (Muons->size()==1)){
          if( (*Muons)[0].DeltaR((*Jets)[i]) < minDRl1 ){ minDRl1 = (*Muons)[0].DeltaR((*Jets)[i]); minDRindxl1=i;}
          if( (*Electrons)[0].DeltaR((*Jets)[i]) < minDRl2 ){ minDRl2 = (*Electrons)[0].DeltaR((*Jets)[i]); minDRindxl2=i;}
	}
      }
    }//loop over jets
    if( minDR > 0.3 )  minDRindx   = -100;
    if( minDRl1 > 0.3) minDRindxl1 = -100;
    if( minDRl2 > 0.3) minDRindxl2 = -100;
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= HT_EtaCut) ){
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
    if(!ignoreLeps){
      if(Muons->size()==2){
	if(minDRl1 < 0.3) ST = ST + (*Muons)[0].Pt();
	if(minDRl2 < 0.3) ST = ST + (*Muons)[1].Pt();
      }
      else if(Electrons->size()==2){
	if(minDRl1 < 0.3) ST = ST + (*Electrons)[0].Pt();
	if(minDRl2 < 0.3) ST = ST + (*Electrons)[1].Pt();
      }
      else if(Muons->size()==1 && Electrons->size()==1){
	if(minDRl1 < 0.3) ST = ST + (*Muons)[0].Pt();
	if(minDRl2 < 0.3) ST = ST + (*Electrons)[0].Pt();
      }
    }
    sortTLorVec(&hadJets);
    
    //ST and HadJets have been determined. Now calulate dPhi b/w MET and leading HadJets.
    double dphi1=3.8,dphi2=3.8,dphi3=3.8,dphi4=3.8,dphiPho_MET=3.8;
    if(bestPhoton.Pt()>0.1) dphiPho_MET=abs(DeltaPhi(metstar.Phi(),bestPhoton.Phi()));
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi(metstar.Phi(),(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi(metstar.Phi(),(hadJets)[1].Phi()));
    if(hadJets.size() > 2 ) dphi3 = abs(DeltaPhi(metstar.Phi(),(hadJets)[2].Phi()));
    if(hadJets.size() > 3 ) dphi4 = abs(DeltaPhi(metstar.Phi(),(hadJets)[3].Phi()));

    // if(phoMatchingJetIndx>=0 && ((*Jets)[phoMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    // if(phoMatchingJetIndx<0) continue;  
    //if(!(passPhoton165HE10Trigger || passPhoton135MET100Trigger)) continue;
    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    //apply baseline selections
    process = process && ST>500 && metstar.Pt() > 100 && nHadJets >=2 && bestPhoton.Pt() > 100 && dphi1 > 0.3 && dphi2 > 0.3 ;
    //    process = process && ST>500 && nHadJets >=2 && bestPhoton.Pt() > 100 && dphi1 > 0.3 && dphi2 > 0.3 ;
    if(ignoreLeps && MET > 100) continue;

    if(process && hadJetID){
      evtSurvived++;
      h_RunNum->Fill(RunNum);
      h_intLumi->Fill(lumiInfb);
      h_passPhoPtTrg->Fill(passPhoton165HE10Trigger,wt);
      h_passHTxPhoPtTrg->Fill(passHT600Photon90Trigger,wt);
      // for(int i=0;i<hadJets.size();i++){
      // 	cout<<"HadJetPt:"<<hadJets[i].Pt()<<" eta:"<<hadJets[i].Eta()<<" phi:"<<hadJets[i].Phi()<<endl;
      // }
      // cout<<"METOrg: "<<MET<<" METPhi:"<<METPhi<<" METClean:"<<METclean<<" METPhiClean:"<<METPhiclean<<endl;
      // cout<<"ZPt:"<<zvec.Pt()<<" eta:"<<zvec.Eta()<<" phi:"<<zvec.Phi()<<" M:"<<zvec.M()<<endl;
      // cout<<" MET* Pt:"<<metstar.Pt()<<" MET* !!Eta:"<<metstar.Eta()<<" MET* Phi:"<<metstar.Phi()<<" MET* !!Energy:"<<metstar.Energy()<<endl;
      // cout<<"BestPhotonPt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<endl<<endl;
      // print(jentry);

      int searchRegion=0;
      if     (nHadJets >= 2 && nHadJets <= 4 ) searchRegion=1;
      else if(nHadJets == 5 || nHadJets == 6 ) searchRegion=2;
      else if(nHadJets >= 7                  ) searchRegion=3;

      if(Muons->size()==2 || Electrons->size()==2){
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
	h_METvBin->Fill(metstar.Pt(),wt);
	h_nVtx->Fill(NVtx,wt);
	h_ZMass->Fill(zmass,wt);

	h_METstar->Fill(metstar.Pt(),wt);
	h_METPhi->Fill(metstar.Phi(),wt);
	h_METOrg->Fill(MET,wt);
	h_METclean->Fill(METclean,wt);
	h_METPhiclean->Fill(METPhiclean,wt);      
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_OSSF_R[searchRegion-1]->Fill(metstar.Pt(),wt);
	}
	else cout<<"Event outside search region! ";
      }
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

	h_dPhi_PhoMET_2Mu->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_2Mu->Fill(dphi1,wt);
	h_dphi_METjet2_2Mu->Fill(dphi2,wt);

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

	h_dPhi_PhoMET_2Ele->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_2Ele->Fill(dphi1,wt);
	h_dphi_METjet2_2Ele->Fill(dphi2,wt);
	 
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
      }//e-e + photon events
      else if(Muons->size()==1 && Electrons->size()==1){
	h_ST_1E1Mu->Fill(ST,wt);
	h_MET_1E1Mu->Fill(metstar.Pt(),wt);
	h_nHadJets_1E1Mu->Fill(nHadJets,wt);
	h_BTags_1E1Mu->Fill(BTags,wt);
	h_METvBin_1E1Mu->Fill(metstar.Pt(),wt);
	h_BestPhotonPt_1E1Mu->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_1E1Mu->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_1E1Mu->Fill(bestPhoton.Phi(),wt);
	h_ZMass_1E1Mu->Fill(zmass,wt);

	h_METstar_1E1Mu->Fill(metstar.Pt(),wt);
	h_METPhi_1E1Mu->Fill(metstar.Phi(),wt);
	h_METOrg_1E1Mu->Fill(MET,wt);
	h_METclean_1E1Mu->Fill(METclean,wt);
	h_METPhiclean_1E1Mu->Fill(METPhiclean,wt);

	h_Ele1Pt->Fill((*Electrons)[0].Pt(),wt);
	h_Ele2Pt->Fill((*Electrons)[1].Pt(),wt);
	h_Ele1Eta->Fill((*Electrons)[0].Eta(),wt);
	h_Ele2Eta->Fill((*Electrons)[1].Eta(),wt);
	h_Ele1Phi->Fill((*Electrons)[0].Phi(),wt);
	h_Ele2Phi->Fill((*Electrons)[1].Phi(),wt);

	h_isoMuonTracks_1E1Mu->Fill(isoMuonTracks,wt);

	h_dPhi_PhoMET_1E1Mu->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_1E1Mu->Fill(dphi1,wt);
	h_dphi_METjet2_1E1Mu->Fill(dphi2,wt);
	 
	h_Jet1Pt_1E1Mu->Fill(hadJets[0].Pt(),wt);	
	h_Jet2Pt_1E1Mu->Fill(hadJets[1].Pt(),wt);	
	h_isoEleTrack_1E1Mu->Fill(isoElectronTracks,wt);
	h_isoMuTrack_1E1Mu->Fill(isoMuonTracks,wt);
	h_isoPiTrack_1E1Mu->Fill(isoPionTracks,wt);
	
	if(allBestPhotons.size()>1) h_Pho2Pt_1E1Mu->Fill(allBestPhotons[1].Pt(),wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_OSOF_R[searchRegion-1]->Fill(metstar.Pt(),wt);
	}
	else cout<<"Event outside search region! "<<searchRegion;
      }//e-mu (OSOF) events
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


bool ZGamma::check_eMatchedtoGamma(){
  if(bestPhotonIndxAmongPhotons>=0){
    for(int i=0;i<Electrons->size();i++){
      if( (*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i] ) < 0.4){
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
      if( (*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Muons)[i] ) < 0.4){
	return true;
      }
    }
  }
  else
    return false;
}


void ZGamma::print(Long64_t jentry){
  //cout<<endl;
  TLorentzVector v1,photo;
  for(int i=0;i<Photons->size();i++){
    double dR=0;//DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*Photons)[i].Eta(),(*Photons)[i].Phi() );
    //cout<<jentry<<" i:"<<i<<" phoSize:"<<Photons->size()<<" Pt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" otherP:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" dR:"<<dR<<endl;
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
