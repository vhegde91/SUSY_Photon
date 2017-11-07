{
  TF1 *mygaus = new TF1("mygaus","TMath::Gaus(x,3,.5)",0,6);
  TF1 *mygaus2 = new TF1("mygaus2","TMath::Gaus(x,3.2,.5)",0,6);
  TH1D *h1=new TH1D("h1","h1",100,0,6);
  TH1D *h2=new TH1D("h2","h2",100,0,6);
  h1->FillRandom("mygaus",100000);
  h2->FillRandom("mygaus2",100000);
  h1->Draw();
  h2->SetLineColor(kRed);
  h2->Draw("sames");
  TH1D *h1c=(TH1D*)h1->Clone("h1c");
  h1c->Divide(h2);
  h1c->SetLineColor(kOrange);
  h1c->Draw("sames");
}
