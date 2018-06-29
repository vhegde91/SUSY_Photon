#include"TFile.h"
#include"TH1.h"

void divide2Hist(){
  
  TFile *f1=new TFile("SR_summer16_gjets400to600.root");
  TFile *f2=new TFile("SR_spring16_gjets400to600.root");
  //  TFile *f1=new TFile("SR_MHT100_summer16_gjets400to600.root");
  //  TFile *f2=new TFile("SR_MHT100_spring16_gjets400to600.root");
  

  TH1F *h1=(TH1F*)f1->FindObjectAny("selectBaselineYields_");
  TH1F *h2=(TH1F*)f2->FindObjectAny("selectBaselineYields_");

  TH1F *hnew=(TH1F*)h1->Clone("h1new");
  hnew->Divide(h2);
  hnew->Draw("text hist");

  TTree *t1 = (TTree*)f1->Get("PreSelection");
  TTree *t2 = (TTree*)f2->Get("PreSelection");
  double MET1,MET2;
  t1->SetBranchAddress("MET",&MET1);
  t2->SetBranchAddress("MET",&MET2);
  
  // vector<TLorentzVector> v1,v2;
  // t1->SetBranchAddress("Photons",&(v1[0]));
  // t2->SetBranchAddress("Photons",&(v2[0]));
  
  TH1D *ht1   = new TH1D("ht1","ht1 distribution",500,0,5000);
  TH1D *ht2   = new TH1D("ht2","ht2 distribution",500,0,5000);

  Long64_t nentries1 = t1->GetEntries();
  for (Long64_t i=0;i<nentries1;i++) {
    t1->GetEntry(i);
    ht1->Fill(MET1);
  }
  Long64_t nentries2 = t2->GetEntries();
  for (Long64_t i=0;i<nentries2;i++) {
    t2->GetEntry(i);
    ht2->Fill(MET2);
  }

  ht1->SetLineColor(kRed);
  ht2->SetLineColor(kBlue);
  // ht1->DrawNormalized("e0");
  // ht2->DrawNormalized("sames e0");
}
