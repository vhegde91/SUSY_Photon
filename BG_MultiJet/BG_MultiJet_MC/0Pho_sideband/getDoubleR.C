void getDoubleR(TString fName){
  TFile *f=new TFile(fName);
  double intgr=0.,err=0;
  TH1D *hA=new TH1D("hA","hA",5,0,5);
  TH1D *hB=new TH1D("hB","hB",5,0,5);
  TH1D *hC=new TH1D("hC","hC",5,0,5);
  TH1D *hD=new TH1D("hD","hD",5,0,5);
  gStyle->SetMarkerSize(2);
  // name1_2d.push_back("METnHadJ_0b_CD"); name2_2d.push_back("METnHadJ_0b_AB");
  // name1_2d.push_back("METnHadJ_min1b_CD"); name2_2d.push_back("METnHadJ_min1b_AB");
  TH2D *h2AB=(TH2D*)f->Get("METnHadJ_0b_AB");
  TH2D *h2CD=(TH2D*)f->Get("METnHadJ_0b_CD");
  // TH2D *h2AB=(TH2D*)f->Get("METnHadJ_min1b_AB");
  // TH2D *h2CD=(TH2D*)f->Get("METnHadJ_min1b_CD");

  cout<<"Double ratio for "<<h2AB->GetName()<<endl;
 
  intgr=h2AB->IntegralAndError(h2AB->GetXaxis()->FindBin(101),h2AB->GetXaxis()->FindBin(101),
			 h2AB->GetYaxis()->FindBin(2.1),h2AB->GetYaxis()->FindBin(4.1),err);
  cout<<intgr<<" "<<err<<endl;
  hA->SetBinContent(1,intgr);
  hA->SetBinError(1,err);
  intgr=h2AB->IntegralAndError(h2AB->GetXaxis()->FindBin(101),h2AB->GetXaxis()->FindBin(101),
			 h2AB->GetYaxis()->FindBin(5.1),h2AB->GetYaxis()->FindBin(6.1),err);
  cout<<intgr<<" "<<err<<endl;
  hA->SetBinContent(2,intgr);
  hA->SetBinError(2,err);
  intgr=h2AB->IntegralAndError(h2AB->GetXaxis()->FindBin(101),h2AB->GetXaxis()->FindBin(101),
			 h2AB->GetYaxis()->FindBin(7.1),h2AB->GetYaxis()->FindBin(25),err);
  cout<<intgr<<" "<<err<<endl;
  hA->SetBinContent(3,intgr);
  hA->SetBinError(3,err);


  intgr=h2AB->IntegralAndError(h2AB->GetXaxis()->FindBin(201),h2AB->GetXaxis()->FindBin(201),
			 h2AB->GetYaxis()->FindBin(2.1),h2AB->GetYaxis()->FindBin(4.1),err);
  cout<<intgr<<" "<<err<<endl;
  hB->SetBinContent(1,intgr);
  hB->SetBinError(1,err);
  intgr=h2AB->IntegralAndError(h2AB->GetXaxis()->FindBin(201),h2AB->GetXaxis()->FindBin(201),
			 h2AB->GetYaxis()->FindBin(5.1),h2AB->GetYaxis()->FindBin(6.1),err);
  cout<<intgr<<" "<<err<<endl;
  hB->SetBinContent(2,intgr);
  hB->SetBinError(2,err);
  intgr=h2AB->IntegralAndError(h2AB->GetXaxis()->FindBin(201),h2AB->GetXaxis()->FindBin(201),
			 h2AB->GetYaxis()->FindBin(7.1),h2AB->GetYaxis()->FindBin(25),err);
  cout<<intgr<<" "<<err<<endl;
  hB->SetBinContent(3,intgr);
  hB->SetBinError(3,err);

  //  hB->Divide(hA);

  for(int i=1;i<=hA->GetNbinsX();i++){
    cout<<hB->GetBinContent(i)<<" "<<hB->GetBinError(i)<<endl;
  }
  //--------------get CD
  intgr=h2CD->IntegralAndError(h2CD->GetXaxis()->FindBin(101),h2CD->GetXaxis()->FindBin(101),
			 h2CD->GetYaxis()->FindBin(2.1),h2CD->GetYaxis()->FindBin(4.1),err);
  cout<<intgr<<" "<<err<<endl;
  hC->SetBinContent(1,intgr);
  hC->SetBinError(1,err);
  intgr=h2CD->IntegralAndError(h2CD->GetXaxis()->FindBin(101),h2CD->GetXaxis()->FindBin(101),
			 h2CD->GetYaxis()->FindBin(5.1),h2CD->GetYaxis()->FindBin(6.1),err);
  cout<<intgr<<" "<<err<<endl;
  hC->SetBinContent(2,intgr);
  hC->SetBinError(2,err);
  intgr=h2CD->IntegralAndError(h2CD->GetXaxis()->FindBin(101),h2CD->GetXaxis()->FindBin(101),
			 h2CD->GetYaxis()->FindBin(7.1),h2CD->GetYaxis()->FindBin(25),err);
  cout<<intgr<<" "<<err<<endl;
  hC->SetBinContent(3,intgr);
  hC->SetBinError(3,err);


  intgr=h2CD->IntegralAndError(h2CD->GetXaxis()->FindBin(201),h2CD->GetXaxis()->FindBin(201),
			 h2CD->GetYaxis()->FindBin(2.1),h2CD->GetYaxis()->FindBin(4.1),err);
  cout<<intgr<<" "<<err<<endl;
  hD->SetBinContent(1,intgr);
  hD->SetBinError(1,err);
  intgr=h2CD->IntegralAndError(h2CD->GetXaxis()->FindBin(201),h2CD->GetXaxis()->FindBin(201),
			 h2CD->GetYaxis()->FindBin(5.1),h2CD->GetYaxis()->FindBin(6.1),err);
  cout<<intgr<<" "<<err<<endl;
  hD->SetBinContent(2,intgr);
  hD->SetBinError(2,err);
  intgr=h2CD->IntegralAndError(h2CD->GetXaxis()->FindBin(201),h2CD->GetXaxis()->FindBin(201),
			 h2CD->GetYaxis()->FindBin(7.1),h2CD->GetYaxis()->FindBin(25),err);
  cout<<intgr<<" "<<err<<endl;
  hD->SetBinContent(3,intgr);
  hD->SetBinError(3,err);

  // hB->Divide(hA);
  // hD->Divide(hC);
  for(int i=1;i<=hD->GetNbinsX();i++){
    cout<<hD->GetBinContent(i)<<" "<<hD->GetBinError(i)<<endl;
  }
  //  h2CD->Draw("colz texte");
  cout<<"--------------- double R -------------"<<endl;
  //  hD->Divide(hB);

  hC->Divide(hA);
  hD->Divide(hB);
  hD->Divide(hC);
  hD->Draw("text");
  for(int i=1;i<=hD->GetNbinsX();i++){
    cout<<hD->GetBinContent(i)<<" "<<hD->GetBinError(i)<<endl;
  }
  
}
