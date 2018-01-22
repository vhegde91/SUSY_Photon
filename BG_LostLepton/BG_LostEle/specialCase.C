void specialCase(){

  TH1D *h1=new TH1D("h1","h1",5,0.5,5.5);
  h1->SetBinContent(1,0.615);h1->SetBinError(1,0.056);
  h1->SetBinContent(2,0.295);h1->SetBinError(2,0.023);
  h1->SetBinContent(3,0.361);h1->SetBinError(3,0.033);

  TH1D *h2=new TH1D("h2","h1",5,0.5,5.5);
  h2->SetBinContent(1,0.083864);h2->SetBinError(1,0.06186);
  h2->SetBinContent(2,0.311016);h2->SetBinError(2,0.102663);
  h2->SetBinContent(3,0.312469);h2->SetBinError(3,0.106629);
  
  TH1D *h3=(TH1D*)h1->Clone("h3");
  h3->Multiply(h2);
  h3->Rebin(5);
  
  TH1D *h4=(TH1D*)h2->Clone("h4");
  h4->Rebin(5);
  h3->Divide(h4);
  cout<<h3->GetBinContent(1)<<" "<<h3->GetBinError(1)<<endl;
}
