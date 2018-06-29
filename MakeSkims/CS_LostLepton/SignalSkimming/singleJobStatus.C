void singleJobStatus(TString fName){
  TFile *f=TFile::Open(fName);
  TH1D *h1=(TH1D*)f->Get("selectBaselineYields_");
  cout<<h1->GetBinContent(1)<<endl;
}
