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

TLatex textOnTop,intLumiE;
double intLumi=35.9;
//int col[11]={kBlack,kMagenta+2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kPink+1,kBlue,kRed,kMagenta};
void quickPlot(TString fName,TString histName){

  TString xName = "Bin number";
  TString yName = "Events";
  TString legName = "#gamma+jets Pred";
  int lineColor = kCyan;
  gStyle->SetOptStat(0);
  TLegend *leg = new TLegend(0.7, 0.88,  0.86, 0.8);
  TString name = fName,name1 = fName;
  //  name = name.ReplaceAll(".root","")+yName+"_Vs_"+xName;
  name1 = name1.ReplaceAll(".root","");

  cout<<name<<endl;

  TCanvas *c1 = new TCanvas("c1",name,1500,800);
  c1->SetLogy();
  TFile *f1 = new TFile(fName);
  TH1D *h1 = (TH1D*)f1->Get(histName);
  TH2D *h2 = (TH2D*)f1->Get(histName);
  if(h1){
    h1->GetXaxis()->SetTitle(xName);
    h1->GetYaxis()->SetTitle(yName);
    h1->SetTitle(0);
    //    h1->Draw("histe");
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
  h1->SetLineColor(kBlack);
  //  h1->SetLineColor(kBlack);
  h1->Draw("e0 histe");
  h1->SetFillColor(lineColor);
  h1->Draw("same");
  leg->AddEntry(h1,legName,"f");
  leg->Draw();
  char name2[100];
  textOnTop.SetTextSize(0.04);
  intLumiE.SetTextSize(0.04);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
  intLumiE.SetTextSize(0.04);
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.73,0.91,name2);

  c1->SaveAs(name1+"_.png");

}
