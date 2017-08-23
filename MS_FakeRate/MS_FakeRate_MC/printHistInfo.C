#include<iostream>
#include<iomanip>
#include"TH1.h"
#include"TROOT.h"
#include"TH2.h"
#include"TFile.h"
#include"TDirectory.h"
#include"TF1.h"
#include<string>
#include<vector>
#include"TLegend.h"
#include"TLatex.h"
#include"TCanvas.h"
#include"THStack.h"
#include"TStyle.h"

void printHistInfo(){
  TFile *f1=new TFile("FR_Hist_DMS_Run2016_FR_ST200.root");
  TH1D *h1=(TH1D*)f1->FindObjectAny("FakeRate_bin1_Pho");

  TFile *f2=new TFile("FR_Hist_MS_FR_DYJetsToLL_ST200_v2.root");
  TH1D *h2=(TH1D*)f2->FindObjectAny("FakeRate_bin1_Pho");

  TH1D *h1cp=(TH1D*)h1->Clone("h1cp");
  h1cp->Divide(h2);

  cout<<h1cp->GetTitle()<<endl;
  cout<<"------------------------"<<endl;
  cout<<"Bin Low edge\t Bin content\t Bin error"<<endl;
  cout<<"------------------------"<<endl;
  for(int i=0;i<=h1cp->GetNbinsX()+1;i++){
    cout<<h1cp->GetBinLowEdge(i)<<"\t"<<h1cp->GetBinContent(i)<<"\t"<<h1cp->GetBinError(i)<<endl;
  }
  h1cp->Draw("text");
}

void printHistInfo(TString iFname, TString histName){
  TFile *f=new TFile(iFname);
  TH1D *h1=(TH1D*)f->FindObjectAny(histName);
  int rebin=2;
  cout<<h1->GetTitle()<<endl;
  cout<<"------------------------"<<endl;
  cout<<"Bin Low edge\t Bin content\t Bin error"<<endl;
  cout<<"------------------------"<<endl;
  for(int i=0;i<=h1->GetNbinsX()+1;i++){
    cout<<h1->GetBinLowEdge(i)<<"\t"<<h1->GetBinContent(i)<<"\t"<<h1->GetBinError(i)<<endl;
  }

  cout<<"------------------------"<<endl;
  if(rebin!=1){
    cout<<"After rebinning"<<endl;
    cout<<"Bin Low edge\t Bin content\t Bin error"<<endl;
    cout<<"------------------------"<<endl;
    h1->Rebin(rebin);
    for(int i=0;i<=h1->GetNbinsX()+1;i++){
      cout<<h1->GetBinLowEdge(i)<<"\t"<<h1->GetBinContent(i)<<"\t"<<h1->GetBinError(i)<<endl;
    }
  }

}
