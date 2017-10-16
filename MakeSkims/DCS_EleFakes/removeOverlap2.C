#include<iostream>
#include"TFile.h"
#include"TTree.h"

void printEvtNum(TTree *);
void removeOverlap2(TString inFName){
  //  TFile *f1 = new TFile("Step1_Run2016B_SingleElectron_03Feb2017.root");
  //  TFile *f1 = new TFile("root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016B_SingleEGamma_03Feb2017.root");
  TFile *f1 = TFile::Open(inFName);


  //  vector<ULong64_t> a={1,2,2,3,4,5,3,5,5,6,7,8,9};
  TFile *fout = new TFile("newTree2.root","recreate");

  TTree *t1 = (TTree*)f1->Get("PreSelection");
  TTree *newt1 = t1->CloneTree(0);

  ULong64_t evtNum1=0, rept=0;
  vector<ULong64_t> evtNums;
  ULong64_t nentries1 = t1->GetEntries();
  
  t1->SetBranchAddress("EvtNum",&evtNum1);
  t1->SetBranchStatus("*",0);
  t1->SetBranchStatus("EvtNum",1);
  for (ULong64_t i=0;i<nentries1;i++) {
    t1->GetEntry(i);
    evtNums.push_back(evtNum1);
  }
  cout<<"Done getting Evt Nums. Total # of events = "<<nentries1<<endl;
  //printEvtNum(t1);
 
  for (ULong64_t i=0;i<nentries1;i++) {
    bool foundRepeat = false;
    //    if(i>a.size()) break;
    t1->GetEntry(i);
    //    ULong64_t evtNum_i = evtNum1;
    //  cout<<i<<" i "<<evtNum1<<endl;
    for(ULong64_t j=0;j<i;j++){
      //cout<<j<<" j "<<evtNum_j<<endl;
      if(evtNum1 == evtNums[j]){
	foundRepeat = true;
	break;
      }
    }
    //    cout<<endl;
    if(!foundRepeat){
      t1->SetBranchStatus("*",1);
      t1->GetEntry(i);
      //  evtNum1 = a[i];
      newt1->Fill();
    }
    else rept++;
  }
  fout->cd();
  newt1->Write();
  cout<<"Repeated events: "<<rept<<endl;
}

void printEvtNum(TTree *t2){
  ULong64_t evtNum;
  t2->SetBranchAddress("EvtNum",&evtNum);
  ULong64_t nentries = t2->GetEntries();
  
  for (ULong64_t i=0;i<nentries;i++) {
    if(i>20) break;
    t2->GetEntry(i);
    cout<<i<<" EvtNum: "<<evtNum<<endl;
  }
}
