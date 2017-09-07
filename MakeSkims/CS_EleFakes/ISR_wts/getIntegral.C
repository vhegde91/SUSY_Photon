void setLastBinAsOverFlow(TH1D*);
void getIntegral(){
  gStyle->SetOptStat(0);
  int col[5]={kRed,kBlue,kBlue,kBlue,kBlue};
  int nf=5;
  TFile *f[nf];
  f[0]=new TFile("TTJets_Tune_Summer16.root");
  //  f[1]=new TFile("TTJets_HT-600ToInf_Summer16.root");
  f[1]=new TFile("TTJets_HT-600to800_Summer16.root");
  f[2]=new TFile("TTJets_HT-800to1200_Summer16.root");
  f[3]=new TFile("TTJets_HT-1200to2500_Summer16.root");
  f[4]=new TFile("TTJets_HT-2500toInf_Summer16.root");

  cout<<"[600,800)\t [800,1200)\t [1200,2500)\t [2500,Inf)"<<endl;  
  for(int i=0;i<nf;i++){
    //    cout<<"***************************"<<endl;
    if(i==0) cout<<"No ISR Wt:"<<endl;
    TH1D *h1=(TH1D*)f[i]->FindObjectAny("madHT_NoCut");
    //    cout<<f[i]->GetName()<<endl;
    cout<<h1->Integral(h1->FindBin(600),h1->FindBin(799.999))<<"\t"
	<<h1->Integral(h1->FindBin(800),h1->FindBin(1199.999))<<"\t"
	<<h1->Integral(h1->FindBin(1200),h1->FindBin(2499.999))<<"\t"
	<<h1->Integral(h1->FindBin(2500),h1->FindBin(h1->GetNbinsX()+1))<<endl;

    h1=(TH1D*)f[i]->FindObjectAny("madHT_NoCut_ISRWtd");
    //    cout<<f[i]->GetName()<<endl;
    cout<<h1->Integral(h1->FindBin(600),h1->FindBin(799.999))<<"\t"
	<<h1->Integral(h1->FindBin(800),h1->FindBin(1199.999))<<"\t"
	<<h1->Integral(h1->FindBin(1200),h1->FindBin(2499.999))<<"\t"
	<<h1->Integral(h1->FindBin(2500),h1->FindBin(h1->GetNbinsX()+1))<<endl;

    h1=(TH1D*)f[i]->FindObjectAny("madHT_NoCut_ISRWtd_D");
    //    cout<<f[i]->GetName()<<endl;
    cout<<h1->Integral(h1->FindBin(600),h1->FindBin(799.999))<<"\t"
	<<h1->Integral(h1->FindBin(800),h1->FindBin(1199.999))<<"\t"
	<<h1->Integral(h1->FindBin(1200),h1->FindBin(2499.999))<<"\t"
	<<h1->Integral(h1->FindBin(2500),h1->FindBin(h1->GetNbinsX()+1))<<endl<<endl;
    TString tstr=h1->GetName();
    if(tstr=="madHT_NoCut_ISRWtd_D"){
      h1->SetLineColor(col[i]);
      setLastBinAsOverFlow(h1);
      h1->Rebin(25);
      if(i==0) h1->Draw("histe");
      else h1->Draw("histe same");
    }
    
  }

}


void setLastBinAsOverFlow(TH1D* h_hist){
  double lastBinCt =h_hist->GetBinContent(h_hist->GetNbinsX()),overflCt =h_hist->GetBinContent(h_hist->GetNbinsX()+1);
  double lastBinErr=h_hist->GetBinError(h_hist->GetNbinsX()),  overflErr=h_hist->GetBinError(h_hist->GetNbinsX()+1);

  if(lastBinCt!=0 && overflCt!=0)
    lastBinErr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );

  else if(lastBinCt==0 && overflCt!=0)
    lastBinErr = overflErr;
  else if(lastBinCt!=0 && overflCt==0)
    lastBinErr = lastBinErr;
  else lastBinErr=0;

  lastBinCt = lastBinCt+overflCt;
  h_hist->SetBinContent(h_hist->GetNbinsX(),lastBinCt);
  h_hist->SetBinError(h_hist->GetNbinsX(),lastBinErr);

}
