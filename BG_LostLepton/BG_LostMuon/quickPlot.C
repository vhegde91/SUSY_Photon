#include<iostream>
#include<iomanip>
#include"TH1.h"
#include"TROOT.h"
#include"TH2.h"
#include"TFile.h"
#include"TDirectory.h"
#include"TTree.h"
#include"TBrowser.h"
#include"TF1.h"
#include<string>
#include<vector>
#include"TGraphErrors.h"
#include"TGraph.h"
#include"TLegend.h"
#include"TLatex.h"
#include"TCanvas.h"
#include"THStack.h"
#include"TStyle.h"

void quickPlot(TString fName,TString histName){

  //  TString xName = "#Delta#Phi(#gamma,MET)";
  TString xName = "Gen #mu pT(GeV)";
  TString yName = "Gen #mu |#eta|";
  gStyle->SetOptStat(0);
  TString name = fName,name1 = fName;
  name = name.ReplaceAll(".root","")+yName+"_Vs_"+xName;
  name1 = name1.ReplaceAll(".root","");

  cout<<name<<endl;

  TCanvas *c1 = new TCanvas("c1",name,1500,800);
  TFile *f1 = new TFile(fName);
  TH1D *h1 = (TH1D*)f1->Get(histName);
  TH2D *h2 = (TH2D*)f1->Get(histName);
  if(h1){
    h1->GetXaxis()->SetTitle(xName);
    h1->GetYaxis()->SetTitle(yName);
    h1->SetTitle(0);
    h1->Draw("histe");
  }
  if(h2){
    //    h2->GetYaxis()->SetRangeUser(0.5,1.5);
    //    h2->SetTitle(fName.ReplaceAll(".root",""));
    h2->GetXaxis()->SetTitle(xName);
    h2->GetYaxis()->SetTitle(yName);
    h2->GetYaxis()->SetTitleOffset(0.9);
    h2->Draw("colz");
    c1->SetLogz();
  }
  if(!h1 && !h2) {cout<<"Hist Not found"<<endl;}


  double  ptl=10.1, pth=2000,etal=0.,etah=2.39999,intgrl=0.,e=0.,e2=0.;
  intgrl = h2->IntegralAndError(h2->GetXaxis()->FindBin(ptl),h2->GetXaxis()->FindBin(pth),h2->GetYaxis()->FindBin(etal),h2->GetYaxis()->FindBin(etah),e);
  cout<<"Within acceptance = "<<intgrl<<"#pm"<<e<<endl;
  
  ptl=10.1, pth=2000,etal=2.40001,etah=7.0,intgrl=0.,e=0.;
  intgrl = h2->IntegralAndError(h2->GetXaxis()->FindBin(ptl),h2->GetXaxis()->FindBin(pth),h2->GetYaxis()->FindBin(etal),h2->GetYaxis()->FindBin(etah),e);

  ptl=0, pth=9.99,etal=0.,etah=7.;
  intgrl += h2->IntegralAndError(h2->GetXaxis()->FindBin(ptl),h2->GetXaxis()->FindBin(pth),h2->GetYaxis()->FindBin(etal),h2->GetYaxis()->FindBin(etah),e2);

  cout<<"Out of acceptance = "<<intgrl<<"#pm"<<sqrt(e*e+e2*e2)<<endl;
  
  //  c1->SaveAs(name1+"_"+xName+".png");

}
