void purityCalc(){
  //data inc, data 0b, data >=1b, mc incl, mc 0b, mc >=1b
  //  vector<double> den={31.+5., 31., 5., 32.409+0.111+5.332+0.4718, 32.409+0.111, 5.332+0.4718};
  vector<double> den={500, 30, 36, 31.+5.,            31.,        5.,       23.53+0.11, 2.55+0.47, 26.08+0.58};
  vector<double> num={480, 29, 35, 36-0.11101-0.4718, 31-0.11101, 5-0.4718, 23.53,      2.55,      26.08};
  //  vector<double> denErr={6.,sqrt(31.),sqrt(5.),sqrt(1.485*1.485+0.812*0.812+0.0961*0.0961+0.2272*0.2272),sqrt(1.485*1.485+0.0961*0.0961),sqrt(0.812*0.812+0.2272*0.2272)};
  vector<double> denErr={sqrt(500), sqrt(31), sqrt(36), 6.,               sqrt(31), sqrt(5) ,sqrt(0.53*0.53+0.11*0.11), sqrt(0.17*0.17+0.23*0.23), sqrt(0.56*0.56+0.25*0.25)};
  //  vector<double> numErr={sqrt(36+0.0961*0.0961+0.2272*0.2272),sqrt(31+0.0961*0.0961),sqrt(5+0.2272*0.2272),sqrt(1.485*1.485+0.812*0.812),1.485,0.8120};
  vector<double> numErr={sqrt(480), sqrt(30), sqrt(35), sqrt((1./36.)+(0.25*0.25)), sqrt((1./31.)+(0.096138*0.096138)), sqrt((1./5.)+(0.227165*0.227165)),0.53, 0.17, 0.56};
  
  TH1D *hNum=new TH1D("hNum","hNum",10,0.5,10.5);
  TH1D *hDen=new TH1D("hDen","hDen",10,0.5,10.5);
  // hNum->Sumw2(false);
  // hDen->Sumw2(false);
  for(int i=0;i<den.size();i++){
    hNum->SetBinContent(i+1,num[i]);
    hNum->SetBinError(i+1,numErr[i]);
    // cout<<hNum->GetBinContent(i+1)<<" "<<hNum->GetBinError(i+1)<<endl;
    hDen->SetBinContent(i+1,den[i]);
    hDen->SetBinError(i+1,denErr[i]);
    //    cout<<hDen->GetBinContent(i+1)<<" "<<hDen->GetBinError(i+1)<<endl;
  }
  hNum->SetBinContent(8,5-0.4718);
  // //  hNum->SetBinError(1,sqrt(36.34));
  hDen->SetBinContent(8,5);
  // //  hDen->SetBinError(1,sqrt(36));

  TGraphAsymmErrors *gr = new TGraphAsymmErrors(hNum,hDen,"cp");
  //  TGraphAsymmErrors *gr = new TGraphAsymmErrors(den.size(),&(den[0]),&(num[0]),&(numErr[0]),&(numErr[0]),&(denErr[0]),&(denErr[0]));
  gr->Draw("text");
  gr->Print("ALL");
  cout<<"================"<<endl;
  // double x1[]=gr->GetX();
  // for(int i=0;i<gr->GetXaxis()->GetNbins();i++){
  //   cout<<"Den: "<<den[i]<<" Num: "<<num[i]<<x1[i]<<endl;
  // }
  
}
void purityCalc(int i){
  vector<double> den={32.409+0.111,5.332+0.4718};
  vector<double> num={32.409,5.332};
  vector<double> denErr={sqrt(1.485*1.485+0.096*0.096), sqrt(0.812*0.812+0.227*0.227)};
  vector<double> numErr={1.485,0.812};
  TH1D *hNum=new TH1D("hNum","hNum",10,0.5,10.5);
  TH1D *hDen=new TH1D("hDen","hDen",10,0.5,10.5);
  for(int i=0;i<den.size();i++){
    hNum->SetBinContent(i+1,num[i]);
    hNum->SetBinError(i+1,numErr[i]);
    cout<<hNum->GetBinContent(i+1)<<" "<<hNum->GetBinError(i+1)<<endl;
    hDen->SetBinContent(i+1,den[i]);
    hDen->SetBinError(i+1,denErr[i]);
    cout<<hDen->GetBinContent(i+1)<<" "<<hDen->GetBinError(i+1)<<endl;
  }
  hNum->Rebin(2);
  hDen->Rebin(2);
  TGraphAsymmErrors *gr = new TGraphAsymmErrors(hNum,hDen);
  gr->Draw();
  gr->Print("ALL");
}
