#define SkimmingTrig_cxx
#include "SkimmingTrig.h"
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

  SkimmingTrig ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingTrig::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  TTree *newtree = fChain->CloneTree(0);

  string s_data=data;

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
    double wt=1.0;
    int reqTrgPS=-1;
    h_selectBaselineYields_->Fill(0);
    if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0)) continue;

    TLorentzVector bestPhoton = getBestPhoton();
    if(bestPhoton.Pt()>100) continue;
    // if(bestPhotonIndxAmongPhotons<0) continue;
    //    if( (*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons] > 0.001 ) continue;
    if(Electrons->size()!=1) continue;
    if(!(*Electrons_tightID)[0]) continue;

    int tightEleIdx = -100;
    TLorentzVector ele1,ele2;
    ele1=(*Electrons)[0];
    //    if((Electrons->size()==1) && (*Electrons_tightID)[0]) ele1=(*Electrons)[0];
    //    else continue;
    // if((Electrons->size()==1)) ele1=(*Electrons)[0];
    // else continue;
    //if(Electrons_tightID->size()!=2) continue;
    // for(int i=0;i<Electrons->size();i++){
    //   if((*Electrons_tightID)[i] && (ele1.Pt() < (*Electrons)[i].Pt()) ){ 
    // 	//      if((ele1.Pt() < (*Electrons)[i].Pt()) ){ 
    // 	ele1 = (*Electrons)[i];
    // 	tightEleIdx = i;
    // 	//	break; 
    //   }
    // }
    // if(tightEleIdx < 0) continue;
    // for(int i=0;i<Electrons->size();i++){
    //   if(i!=tightEleIdx && (*Electrons_tightID)[i]) ele2 = (*Electrons)[i];
    // }
    bool passTrg=false,passReqTrg=false;

    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i],trgName2=(*TriggerNames)[i];
      trgName.pop_back();
      trgName2.pop_back();trgName2.pop_back();

      int trigNum=90;
      for(int j=0;j<trigNames1.size();j++){
	if(trgName==trigNames1[j]){trigNum=j;break;}
	else if(trgName2==trigNames1[j]){trigNum=j;break;}
      }
      h_TrigDec->Fill(trigNum,(*TriggerPass)[i]);
      h2_TrigPS->Fill(trigNum,(*TriggerPrescales)[i],wt);

      if( trgName=="HLT_PFMET90_PFMHT90_IDTight_v" && (*TriggerPass)[i]==1 )        { h_selectBaselineYields_->Fill(1); passTrg = true;}
      else if( trgName=="HLT_PFMET100_PFMHT100_IDTight_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(2); passTrg = true;}
      else if( trgName=="HLT_PFMET110_PFMHT110_IDTight_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(3); passTrg = true;}
      else if( trgName=="HLT_PFMET120_PFMHT120_IDTight_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(4); passTrg = true;}
      // if( trgName=="HLT_IsoMu22_v" && (*TriggerPass)[i]==1 )        { h_selectBaselineYields_->Fill(17); passTrg = true;}
      // else if( trgName=="HLT_IsoMu24_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(18); passTrg = true;}
      // else if( trgName=="HLT_IsoTkMu22_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(19); passTrg = true;}
      // else if( trgName=="HLT_IsoTkMu24_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(20); passTrg = true;}
      // else if( trgName=="HLT_Mu50_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(29); passTrg = true;}

      // else if( trgName=="HLT_Ele15_IsoVVVL_PFHT350_v" && (*TriggerPass)[i]==1 )     { h_selectBaselineYields_->Fill(7); passTrg = true;}
      // else if( trgName=="HLT_Ele27_WPTight_Gsf_v" && (*TriggerPass)[i]==1 )         { h_selectBaselineYields_->Fill(8); passTrg = true;}
      // else if( trgName=="HLT_IsoMu24_v" && (*TriggerPass)[i]==1 )                   { h_selectBaselineYields_->Fill(9); passTrg = true;}
      // else if( trgName=="HLT_Mu15_IsoVVVL_PFHT350_v" && (*TriggerPass)[i]==1 )        { h_selectBaselineYields_->Fill(10); passTrg = true;}

      // if( trgName=="HLT_PFHT300_v" && (*TriggerPass)[i]==1 )                  { h_selectBaselineYields_->Fill(1); passTrg = true;}
      // else if( trgName=="HLT_PFHT350_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(2); passTrg = true;}
      // else if( trgName=="HLT_PFHT400_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(3); passTrg = true;}
      // else if( trgName=="HLT_PFHT475_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(4); passTrg = true;}
      // else if( trgName=="HLT_PFHT600_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(5); passTrg = true;}
      // else if( trgName=="HLT_PFHT650_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(6); passTrg = true;}
      // else if( trgName=="HLT_PFHT800_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(7); passTrg = true;}
      // else if( trgName=="HLT_PFHT900_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(8); passTrg = true;}
      
      // else if( trgName=="HLT_CaloJet500_NoJetID_v" ){
      // 	//if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(10); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( trgName=="HLT_Ele105_CaloIdVT_GsfTrkIdT_v" ){
      // 	//if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(11);
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( trgName=="HLT_Ele115_CaloIdVT_GsfTrkIdT_v" ){
      // 	//	if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(12); 
      // 	  passReqTrg = true; 
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( trgName=="HLT_Ele15_IsoVVVL_PFHT350_v" || trgName=="HLT_Ele15_IsoVVVL_PFHT350_"){
      // 	//if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(13); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( trgName=="HLT_Ele15_IsoVVVL_PFHT400_v" || trgName=="HLT_Ele15_IsoVVVL_PFHT400_"){
      // 	//if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(13); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( trgName=="HLT_Ele15_IsoVVVL_PFHT600_v" ){
      // 	//      	if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(13); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( trgName=="HLT_Ele25_eta2p1_WPTight_Gsf_v" ){
      // 	//if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(13); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      else if( trgName=="HLT_Ele27_WPTight_Gsf_v" || trgName=="HLT_Ele27_WPTight_Gsf_" ){
      	//if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      	if((*TriggerPass)[i]==1){
      	  h_selectBaselineYields_->Fill(13); 
      	  passReqTrg = true;
      	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      	}
      }
      // else if( trgName=="HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v" || trgName=="HLT_Ele15_IsoVVVL_PFHT350_PFMET50_" ){
      // 	//if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(5); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }

      // else if( trgName=="HLT_Ele27_eta2p1_WPLoose_Gsf_v" ){
      // 	//      	if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(13); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( (trgName=="HLT_Ele45_WPLoose_Gsf_v") || (trgName=="HLT_Ele45_WPLoose_Gsf_") ){
      // 	//      	if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(13); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( trgName=="HLT_Ele50_IsoVVVL_PFHT400_v" || trgName=="HLT_Ele50_IsoVVVL_PFHT400_" ){
      // 	//      	if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(13); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      else if( (trgName=="HLT_Photon165_HE10_v") ){
      	//if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
	if((*TriggerPass)[i]==1){
      	  h_selectBaselineYields_->Fill(13); 
      	  passReqTrg = true;
      	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      	}
      }
      // else if( trgName=="HLT_Photon90_CaloIdL_PFHT600_v" ){
      // 	//      	if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(14); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }
      // else if( (trgName=="HLT_Photon175_v") ){
      // 	//      	if((*TriggerPass)[i]==-1) cout<<trgName<<" is not run. Dec "<<(*TriggerPass)[i]<<endl;
      // 	if((*TriggerPass)[i]==1){
      // 	  h_selectBaselineYields_->Fill(13); 
      // 	  passReqTrg = true;
      // 	  if(reqTrgPS<0) reqTrgPS = (*TriggerPrescales)[i];
      // 	}
      // }

      //cout<<trgName<<" dec: "<<(*TriggerPass)[i]<<" PS: "<<(*TriggerPrescales)[i]<<endl;
      //      else if( ((trgName=="HLT_Ele15_IsoVVVL_PFHT350_v") || (trgName=="HLT_Ele15_IsoVVVL_PFHT400_v") || (trgName=="HLT_Ele15_IsoVVVL_PFHT600_v") || (trgName=="HLT_Ele50_IsoVVVL_PFHT400_v")) && (*TriggerPass)[i]==1 )      { h_selectBaselineYields_->Fill(11); passReqTrg = true; reqTrgPS = (*TriggerPrescales)[i];}
    }
    //    cout<<endl;
    //    cout<<" "<<reqTrgPS<<" ";
    if(!passTrg) continue;
    //    if(HT<800) continue;
    if(ele1.Pt()<10) continue;
    bool hadJetID=true;
    int minDRindx=-100,photonMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	double dR=ele1.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	if( !(minDR < 0.3 && i==minDRindx) ){
	  hadJets.push_back((*Jets)[i]);
	  if(hadJetID) hadJetID=(*Jets_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) photonMatchingJetIndx=minDRindx;
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+ele1.Pt();
    }
    if(nHadJets<2) continue;
    //    if(! ((ST > 500) && ((ST > 800 && ele1.Pt() > 100) || (ele1.Pt() > 190))) ) continue;
    if(!hadJetID) continue;
    // if(hadJetID){
    h_ST->Fill(ST,wt);
    h_MET->Fill(MET,wt);
    h_nHadJets->Fill(nHadJets,wt);
    h_BTags->Fill(BTags,wt);
    h_BestPhotonPt->Fill(bestPhoton.Pt(),wt);
    h_Ele1Pt->Fill(ele1.Pt(),wt);
    h_Ele1Eta->Fill(ele1.Eta(),wt);
    if(ele2.Pt()>0) h_Ele2Pt->Fill(ele2.Pt(),wt);

    h_HT->Fill(HT,wt);
    h_MHT->Fill(MHT,wt);
    h_nJets->Fill(NJets,wt);

    h_STvBin->Fill(ST,wt);
    h_BestPhotonPt_vBin->Fill(bestPhoton.Pt(),wt);
    h_BestPhotonPt_vBin2->Fill(bestPhoton.Pt(),wt);
    h_Ele1Pt_vBin->Fill(ele1.Pt(),wt);
    if(ele2.Pt()>0) h_Ele2Pt_vBin->Fill(ele2.Pt(),wt);

    h_HTvBin->Fill(HT,wt);
    h2_PhoPtST->Fill(bestPhoton.Pt(),ST,wt);

    if(passTrg && passReqTrg){
      h_Prescale_Req->Fill(reqTrgPS,wt);
      h_ST_Req->Fill(ST,wt);
      h_MET_Req->Fill(MET,wt);
      h_nHadJets_Req->Fill(nHadJets,wt);
      h_BTags_Req->Fill(BTags,wt);
      h_BestPhotonPt_Req->Fill(bestPhoton.Pt(),wt);
      h_Ele1Pt_Req->Fill(ele1.Pt(),wt);
      if(ele2.Pt()>0) h_Ele2Pt_Req->Fill(ele2.Pt(),wt);

      h_HT_Req->Fill(HT,wt);
      h_MHT_Req->Fill(MHT,wt);
      h_nJets_Req->Fill(NJets,wt);

      h_STvBin_Req->Fill(ST,wt);
      h_BestPhotonPt_vBin_Req->Fill(bestPhoton.Pt(),wt);
      h_BestPhotonPt_vBin2_Req->Fill(bestPhoton.Pt(),wt);
      h_Ele1Pt_vBin_Req->Fill(ele1.Pt(),wt);
      h_Ele1Eta_Req->Fill(ele1.Eta(),wt);
      if(ele2.Pt()>0) h_Ele2Pt_vBin_Req->Fill(ele2.Pt(),wt);

      h_HTvBin_Req->Fill(HT,wt);
      h2_PhoPtST_Req->Fill(bestPhoton.Pt(),ST,wt);
    }

    //    newtree->Fill();
    
  } // loop over entries
  //  newtree->AutoSave();
}

TLorentzVector SkimmingTrig::getBestPhoton(){
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( (*Photons_fullID)[iPho] && ((*Photons_hasPixelSeed)[iPho]<0.001) ) {
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
  }
  else bestPhotonIndxAmongPhotons = -100;
  if(highPtIndx==-100){TLorentzVector v0;return v0;}
  else return goodPho[highPtIndx];
}
