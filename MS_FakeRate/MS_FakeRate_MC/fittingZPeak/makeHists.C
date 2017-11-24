void makeHists(bool isdata){
  TFile *f1;
  if(isdata) f1 = new TFile("/home/vinay/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/MS_EleFakes_v2/tnpTrees/Run2016_03Feb2017_SingleElectron_tnpTree.root");
  //  else f1 = new TFile("/home/vinay/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/MS_EleFakes_v2/tnpTrees/MS_FR_DYJetsToLL_HT-100toInf_tnpTree.root");
  else f1 = new TFile("/home/vinay/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/MS_EleFakes_v2/tnpTrees/MS_FR_MC_BGSig_DYTTWG.root");

  if(isdata) cout<<"using data file"<<endl;
  if(!isdata) cout<<"using MC file"<<endl;
  TTree *t1 = (TTree*)f1->Get("tnpTree");
  int isBG;
  Char_t isPho;
  double ZMass_Ele,ZMass_Pho,ZMass,wt;
  t1->SetBranchAddress("ZMass",&ZMass);
  t1->SetBranchAddress("ZMass_Ele",&ZMass_Ele);
  t1->SetBranchAddress("ZMass_Pho",&ZMass_Pho);
  t1->SetBranchAddress("ProbeIsPhoton",&isPho);
  t1->SetBranchAddress("IsBGEvent",&isBG);
  t1->SetBranchAddress("EvtWt",&wt);

  TH1D *h_zmassPho=new TH1D("ZMass_Pho","ZMass for e-gamma",625,0,500);
  TH1D *h_zmassEle=new TH1D("ZMass_Ele","ZMass for e-e",625,0,500);

  Long64_t nentries = t1->GetEntries();
  for (Long64_t i=0;i<nentries;i++) {
    t1->GetEntry(i);
    //    if(isBG!=0 && !isdata) continue;
    wt=1;
    if(isPho==1) h_zmassPho->Fill(ZMass_Pho,wt);
    else h_zmassEle->Fill(ZMass_Ele,wt);
  }
  h_zmassPho->Draw();
  h_zmassEle->Draw("sames");
}
