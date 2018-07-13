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
void bgPrediction(TString fName,TString histName){

  TString xName = "Bin number";
  TString yName = "Events";
  TString legName1 = "#gamma+jets Pred";

  int lineColor = kCyan;
  gStyle->SetOptStat(0);
  TLegend *leg = new TLegend(0.65, 0.88,  0.84, 0.75);
  TString name = fName,name1 = fName;
  //  name = name.ReplaceAll(".root","")+yName+"_Vs_"+xName;
  name1 = name1.ReplaceAll(".root","");

  cout<<name<<endl;

  TCanvas *c1 = new TCanvas("c1",name,1500,800);
  c1->SetLogy();
  TFile *f1 = new TFile(fName);
  TH1D *h1 = (TH1D*)f1->Get(histName);

  if(h1){
    h1->GetXaxis()->SetTitle(xName);
    h1->GetYaxis()->SetTitle(yName);
    h1->SetTitle(0);
    //    h1->Draw("histe");
  }
  
  h1->SetLineColor(kBlack);
  h1->SetFillColor(kCyan);
  h1->Draw("BAR histe0");

  //  h1->SetFillColor(lineColor);
  h1->Draw("same");
  leg->AddEntry(h1,legName1,"fpe");
  leg->Draw();
  char name2[100];
  textOnTop.SetTextSize(0.04);
  intLumiE.SetTextSize(0.04);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
  intLumiE.SetTextSize(0.04);
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.73,0.91,name2);

  //  c1->SaveAs(name1+"_.png");

}
