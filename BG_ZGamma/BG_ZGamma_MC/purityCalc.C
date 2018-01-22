void purityCalc(){
  //data inc, data 0b, data >=1b, mc incl, mc 0b, mc >=1b
  vector<double> den={31.+5., 31., 5., 32.409+0.111+5.332+0.4718, 32.409+0.111, 5.332+0.4718};
  vector<double> num={36-0.11101-0.4718, 31-0.11101, 5-0.4718, 32.409+5.332, 32.409, 5.332};
  vector<double> denErr={6.,sqrt(31.),sqrt(5.),sqrt(1.485*1.485+0.812*0.812+0.0961*0.0961+0.2272*0.2272),sqrt(1.485*1.485+0.0961*0.0961),sqrt(0.812*0.812+0.2272*0.2272)};
  vector<double> numErr={sqrt(36+0.0961*0.0961+0.2272*0.2272),sqrt(31+0.0961*0.0961),sqrt(5+0.2272*0.2272),sqrt(1.485*1.485+0.812*0.812),1.485,0.8120};
  
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

  TGraphAsymmErrors *gr = new TGraphAsymmErrors(hNum,hDen,"pois");
  //  TGraphAsymmErrors *gr = new TGraphAsymmErrors(den.size(),&(den[0]),&(num[0]),&(numErr[0]),&(numErr[0]),&(denErr[0]),&(denErr[0]));
  gr->Draw();
  gr->Print("ALL");

  
}
