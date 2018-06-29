void ratio1(){

  TCanvas *c=new TCanvas("name","name",1500,500);
  c->Divide(1,2);
  TFile *_file0 = TFile::Open("SR_spring16_gjets400to600.root");
  TFile *_file1 = TFile::Open("SR_summer16_gjets400to600.root");
  
  TH1D *h0=(TH1D*)_file0->FindObjectAny("met0");
  TH1D *h1=(TH1D*)_file1->FindObjectAny("met0");
  // h0->Rebin(2);
  // h1->Rebin(2);

  TH1D *hsmr=(TH1D*)h0->Clone("sumr");
  c->cd(1);
  h0->Draw();
  h0->DrawNormalized();
  h0->SetLineColor(kBlue); h0->SetLineWidth(2);
  h0->DrawNormalized();
  h1->SetLineColor(kRed);h1->SetLineWidth(2);
  h1->DrawNormalized("sames");

  c->cd(2);
  hsmr->Divide(h1);hsmr->SetLineWidth(2);
  hsmr->Draw();

}
