#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"TH1.h"
#include"TROOT.h"
#include"TH2.h"
#include"TFile.h"
#include"TDirectory.h"
#include"TTree.h"
#include"TBrowser.h"
#include"TF1.h"
#include<iomanip>
#include"TCanvas.h"
#include"TPaveStats.h"
#include"TLegend.h"
#include"TStyle.h"
using namespace std;

TLatex textOnTop,intLumiE;
double intLumi=35.9;
//int col[5]={kBlue,kRed,kTeal+9,kMagenta,kBlack};
int col[5]={kRed,kBlue,kTeal+9,kMagenta,kBlack};
int rebin=1;
void comp2Hists(){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  vector<TString> legNames;
  // TFile *fn=new TFile("DCS_Run2016_LostMuHadTau_v2.root");
  // TFile *fd=new TFile("DCS_Run2016_LostEle_v2.root");  
  TFile *fn=new TFile("CS_WG_LostMuHadTau_v2.root");
  TFile *fd=new TFile("CS_WG_LostEle_v2.root");  

  rebin=2;
  TString nameN="MuPt"; legNames.push_back("#mu + #gamma");
  TString nameD="ElePt"; legNames.push_back("e + #gamma");
  TString xLabel = "pT(GeV)";
  TString yLabel = "Events";
  TString ratioLabel = "#frac{#mu + #gamma}{e + #gamma}";

  TCanvas *c_cA=new TCanvas("histsComp","compare 2 hists",1500,850);
  TPad *p_top=new TPad("top","top",0,0.4,1,1);
  TPad *p_bot=new TPad("bot","bot",0,0.0,1,0.4);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  p_top->SetBottomMargin(0);
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.3);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();

  TH1D *h_num=(TH1D*)fn->FindObjectAny(nameN);
  TH1D *h_den=(TH1D*)fd->FindObjectAny(nameD);
  h_num->Rebin(rebin);
  h_den->Rebin(rebin);

  h_num->SetLineColor(col[0]);
  h_num->SetLineWidth(2);
  h_den->SetLineColor(col[1]);
  h_den->SetLineWidth(2);
  //  h_den->Scale(h_num->Integral()/h_den->Integral());
  TH1D *h_numC=(TH1D*)h_num->Clone("hnew");
  h_numC->Divide(h_den);

  c_cA->cd();    p_top->cd();
  //h_num->Scale(0.5*h_num->Integral());
  // h_num->Scale(1.0/h_num->Integral());
  // h_den->Scale(1.0/h_den->Integral());
  
  // h_num->GetXaxis()->SetRangeUser(100,2000);
  h_num->SetTitle(0);
  h_num->GetXaxis()->SetTitle(xLabel);
  h_num->GetYaxis()->SetTitle(yLabel);
  h_num->GetYaxis()->SetLabelSize(0.08);
  h_num->GetYaxis()->SetTitleSize(0.08);
  h_num->GetYaxis()->SetNdivisions(10);
  h_num->GetYaxis()->SetTitleOffset(0.5);
  h_num->Draw();   // h_num->Draw("e1same");  
  h_den->Draw("same");//   h_den->Draw("e1same");
  gPad->Update();

  // TPaveStats *stN=(TPaveStats*)h_num->FindObject("stats");
  // TPaveStats *stD=(TPaveStats*)h_den->FindObject("stats");
  // stN->SetTextColor(h_num->GetLineColor());
  // stD->SetTextColor(h_den->GetLineColor());

  TLegend *leg=new TLegend(0.5,0.9,0.7,0.7);
  leg->AddEntry(h_num,legNames[0]);
  leg->AddEntry(h_den,legNames[1]);
  leg->Draw();  

  c_cA->cd();    p_bot->cd();

  h_numC->SetLineColor(kBlack);  
  h_numC->SetTitle(0);
  h_numC->GetXaxis()->SetTitle(xLabel);
  h_numC->GetXaxis()->SetLabelSize(0.13);
  h_numC->GetXaxis()->SetTitleSize(0.13);
  h_numC->GetXaxis()->SetTitleOffset(0.9);

  h_numC->GetYaxis()->SetRangeUser(0.001,1.99);
  h_numC->GetYaxis()->SetTitle(ratioLabel);
  h_numC->GetYaxis()->SetTitleOffset(0.32);
  h_numC->GetYaxis()->SetTitleSize(0.13);
  h_numC->GetYaxis()->SetLabelSize(0.13);
  h_numC->GetYaxis()->SetNdivisions(505);

  h_numC->Draw();
  
  c_cA->cd();    p_top->cd();
  char name2[100];
  textOnTop.SetTextSize(0.06);
  intLumiE.SetTextSize(0.06);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.73,0.91,name2);
}
