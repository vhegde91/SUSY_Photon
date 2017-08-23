void mergeTrees(){
  //  gSystem->Load("$ROOTSYS/test/libEvent");
  TFile *f1 = new TFile("Step1_Run2016B_SingleElectron_03Feb2017.root");
  TFile *f2 = new TFile("Step1_Run2016B_SinglePhoton_03Feb2017.root");
  TFile *fout = new TFile("mergedTree.root","recreate");

  TTree *t1 = (TTree*)f1->Get("PreSelection");
  TTree *t2 = (TTree*)f2->Get("PreSelection");
  
  ULong64_t evtNum1, evtNum2;
  t1->SetBranchAddress("EvtNum",&evtNum1);
  t2->SetBranchAddress("EvtNum",&evtNum2);

  Long64_t nentries1 = t1->GetEntries();
  Long64_t nentries2 = t2->GetEntries();

  for (Long64_t i=0;i<nentries1;i++) {
    t1->GetEntry(i);
    cout<<evtNum1<<" ";
  }
}
