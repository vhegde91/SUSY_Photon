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

//int col[5]={kBlue,kRed,kTeal+9,kMagenta,kBlack};
int col[5]={kRed,kBlue,kTeal+9,kMagenta,kBlack};
int rebin=1;
void comp2Hists(){
  TH1::SetDefaultSumw2(1);
  
  vector<TString> legNames;
  // TFile *fd=new TFile("CS_DiPhoton.root");
  // TFile *fn=new TFile("CS_ZLLGJets_PtG130_LO.root");
  // TFile *fd=new TFile("CS_ZGToLLG_PtG130_NLO.root");
  TFile *fn=new TFile("DCS_ZGToLL.root");  
  //  TFile *fn=new TFile("CS_ZGZJToNuNuG.root");
  TFile *fd=new TFile("CS_ZDYToLLG.root");
  // TFile *fd=new TFile("CS_ZGToLLG.root");

  TString name2="BestPhotonEta";  rebin=5;
  TString nameN=name2; legNames.push_back("NuNu");
  TString nameD=name2; legNames.push_back("LL");
  // TString nameN="BestPhotonPt";
  // TString nameD="BestPhotonPt";
  // TString nameD="LeadGenPhoPt";
  //TString nameD="BestPho2Pt";
  //  TString nameD="MET2Pho";
  // TString nameN="LeadGenPhoPt";

  TCanvas *c_cA=new TCanvas("histsComp","compare 2 hists",1500,850);
  TPad *p_top=new TPad("top","top",0,0.35,1,1);
  TPad *p_bot=new TPad("bot","bot",0,0.04,1,0.35);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  p_top->SetBottomMargin(0);
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.2);
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

  h_num->Draw();  
  h_den->Draw("sames");
  gPad->Update();

  TPaveStats *stN=(TPaveStats*)h_num->FindObject("stats");
  TPaveStats *stD=(TPaveStats*)h_den->FindObject("stats");
  stN->SetTextColor(h_num->GetLineColor());
  stD->SetTextColor(h_den->GetLineColor());

  TLegend *leg=new TLegend(0.5,0.9,0.7,0.7);
  leg->AddEntry(h_num,legNames[0]);
  leg->AddEntry(h_den,legNames[1]);
  leg->Draw();  

  c_cA->cd();    p_bot->cd();
  h_numC->GetYaxis()->SetLabelSize(0.08);
  h_numC->GetXaxis()->SetLabelSize(0.08);
  //  h_numC->GetXaxis()->SetRangeUser(100,2000);
  h_numC->Draw();
  
}
