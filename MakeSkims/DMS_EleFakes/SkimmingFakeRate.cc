#define SkimmingFakeRate_cxx
#include "SkimmingFakeRate.h"
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

  SkimmingFakeRate ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingFakeRate::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  TTree *newtree = fChain->CloneTree(0);

  string s_data = data;
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

    //select skimming parameters (1 of 2)
    h_selectBaselineYields_->Fill(0);
    if( (Muons->size()!=0) || (Electrons->size()==0) || (Electrons->size()>2) ) continue;
    else h_selectBaselineYields_->Fill(1);

    bool passReqTrg=false;
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back();
      if( ((trgName=="HLT_Ele105_CaloIdVT_GsfTrkIdT_v") ||
	   (trgName=="HLT_Ele115_CaloIdVT_GsfTrkIdT_v") ||
	   (trgName=="HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v") ||
	   (trgName=="HLT_Ele15_IsoVVVL_PFHT350_v") ||
	   (trgName=="HLT_Ele15_IsoVVVL_PFHT400_v") ||
	   (trgName=="HLT_Ele15_IsoVVVL_PFHT600_v") ||
	   (trgName=="HLT_Ele25_eta2p1_WPTight_Gsf_v") ||
	   (trgName=="HLT_Ele27_WPTight_Gsf_v") ||
	   (trgName=="HLT_Ele27_eta2p1_WPLoose_Gsf_v") ||
	   (trgName=="HLT_Ele45_WPLoose_Gsf_v") ||
	   (trgName=="HLT_Ele50_IsoVVVL_PFHT400_v") ) &&
	  (*TriggerPass)[i]==1 )      { h_selectBaselineYields_->Fill(5); passReqTrg = true; }
      if(passReqTrg) break;
    }
    if(!passReqTrg) continue;
    /*
    bool hasPho=0;
    //about photons
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt()>100) hasPho=true;
    
    //about electrons
    TLorentzVector ele1,ele2,bestEMObj;
    ele1 = (*Electrons)[0];
    if(Electrons->size()==2){
      if( (*Electrons)[0].Pt() < (*Electrons)[1].Pt() ){
	ele1 = (*Electrons)[1];
	ele2 = (*Electrons)[0];
      }
      else{
	ele1 = (*Electrons)[0];
	ele2 = (*Electrons)[1];  
      }
    }
    
    if((Electrons->size()==2) && hasPho) continue;//reject event if there are 2e and photon

    if(Electrons->size()==1)
      bestEMObj = bestPhoton;
    else if(Electrons->size()==2)
      bestEMObj = ele2;
    else cout<<"!!!! #e"<<Electrons->size()<<" phoPt:"<<bestPhoton.Pt()<<endl;
    h_selectBaselineYields_->Fill(2);
    
    //calulate ST and number of Hadronic Jets
    int minDRindx=-100,minDRindxele1=-100,emMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,minDRele1=9999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	double dR=bestEMObj.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
	if(minDRele1 > ele1.DeltaR((*Jets)[i])){
	  minDRele1 = ele1.DeltaR((*Jets)[i]);
	  minDRindxele1 = i;
	}
      }
    }
    
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	if( !(minDR < 0.3 && i==minDRindx) && !(minDRele1 < 0.3 && i==minDRindxele1) )
	  hadJets.push_back((*Jets)[i]);
      }
    }
    if( minDR<0.3 ) emMatchingJetIndx=minDRindx;
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    if( minDR<0.3 ) ST=ST+bestEMObj.Pt();
    if( minDRele1<0.3 ) ST=ST+ele1.Pt();*/
    //-----------------------------------------------------------------------

    //select skimming parameters (2 of 2)
    // if( nHadJets >= 2 )  h_selectBaselineYields_->Fill(3);
    // else continue;
    // if( ST>500. )        h_selectBaselineYields_->Fill(4);
    // else continue;
    // if( MET>100. )       h_selectBaselineYields_->Fill(5);
    // else continue;
    //end of select skimming parameters

    newtree->Fill();
 
  } // loop over entries
  //  newtree->AutoSave();
}

TLorentzVector SkimmingFakeRate::getBestPhoton(){
  int bestPhoIndx=-100;
  TLorentzVector v1;
  vector<TLorentzVector> goodPho;
  for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
    if( ((*Photons_fullID)[iPhoton]) && ((*Photons_hasPixelSeed)[iPhoton]<0.001) ) goodPho.push_back( (*Photons)[iPhoton] );
  }
  
  if(goodPho.size()==0) return v1;
  else if(goodPho.size()==1) return goodPho[0];
  else{
    for(int i=0;i<goodPho.size();i++){
      if(i==0) bestPhoIndx=0;
      else if(goodPho[bestPhoIndx].Pt() < goodPho[i].Pt()) bestPhoIndx=i;
    }
    return goodPho[bestPhoIndx];
  }
}
