void printEvtNum(TTree *);
void removeOverlap(){
  TFile *f1 = new TFile("Step1_Run2016B_SingleElectron_03Feb2017.root");
  // TFile *f1 = new TFile("b.root");
 
  TFile *fout = new TFile("newTree.root","recreate");

  TTree *t1 = (TTree*)f1->Get("PreSelection");
  TTree *newt1 = t1->CloneTree(0);

  ULong64_t evtNum1=0, rept=0;
  t1->SetBranchAddress("EvtNum",&evtNum1);
  t1->SetBranchStatus("*",0);
  t1->SetBranchStatus("EvtNum",1);
  //  printEvtNum(t1);
  
  ULong64_t nentries1 = t1->GetEntries();
  int decade = 0;
  bool foundRepeat = false;
  for (ULong64_t i=0;i<nentries1;i++) {
    if(i>999) break;
    t1->GetEntry(i);
    ULong64_t evtNum_i = evtNum1;
    //  cout<<i<<" i "<<evtNum1<<endl;
    for(ULong64_t j=0;j<i;j++){
      t1->GetEntry(j);
      ULong64_t evtNum_j = evtNum1;
      //cout<<j<<" j "<<evtNum_j<<endl;
      if(evtNum_i == evtNum_j){
    	foundRepeat = true;
    	break;
      }
    }
    //    cout<<endl;
    if(!foundRepeat){
      t1->SetBranchStatus("*",1);
      t1->GetEntry(i);
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
  
  for (Long64_t i=0;i<nentries;i++) {
    if(i>20) break;
    t2->GetEntry(i);
    cout<<i<<" EvtNum: "<<evtNum<<endl;
  }
}

/*
double progress = 10.0 * i / (1.0 * nentries1);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" <<endl;
    decade = k;
*/
