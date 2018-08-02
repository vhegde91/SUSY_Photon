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
#include"TGraphErrors.h"
#include"TGraph.h"
#include"TLegend.h"
#include"TLatex.h"
#include"TCanvas.h"
#include"TGraph2D.h"

TLatex textOnTop,intLumiE;
double intLumi = 35.9;
void overlayExcl(){
  const int nFiles=2;
  int col[7]={kRed,kBlue,kTeal+9,kBlack,kOrange,kCyan,kMagenta};
  gStyle->SetOptStat(0);
  TCanvas *c1=new TCanvas("c1","c1",1200,1000);
  c1->SetLogz();
  TFile *f[nFiles];
  // f[0]=new TFile("Excl_T5ttttZg_LimitPlots_Obs_V7_v8.root"); 
  // f[1]=new TFile("Excl_T5ttttZg_LimitPlots_Obs_V7_v9.root");

  // f[0]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV3.root"); 
  // f[1]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV5.root");
  // f[2]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV1.root");
  // f[3]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV2.root");
  // f[4]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV4.root");
  // f[5]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV6.root");

  //  f[6]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV7.root");

  f[0]=new TFile("Excl_T5bbbbZg_LimitPlots_Obs_V7_v11.root");
  //f[1]=new TFile("Excl_T6ttZg_LimitPlots_Obs_V7_v9.root");

  // f[0]=new TFile("Excl_GGM_M1M3_LimitPlots_Obs_V7_v3.root");
  // f[1]=new TFile("Excl_GGM_M1M3_PhoLep_LimitPlots_Obs_V7_v3.root");
  // f[2]=new TFile("Excl_GGM_M1M3_PhoLepPhoJet_LimitPlots_Obs_V7_v3.root");

  // f[0]=new TFile("Excl_T5ttttZg_LimitPlots_Obs_V7_v7.root"); 
  f[1]=new TFile("EMHT_Limits_Knut/T5bbbbZg_v20/saved_graphs1d_limit.root");
  //  f[1]=new TFile("Excl_T5bbbbZg_LimitPlots_Obs_V7_v9.root");
  //  f[1]=new TFile("Excl_T6ttZg_LimitPlots_Obs_V7_v8.root");
  // f[0]=new TFile("Excl_T5bbbbZg_LimitPlots_SbinV7.root");
  // f[1]=new TFile("Excl_T5bbbbZg_LimitPlots_SbinV7_ObsTotBG.root");
  // f[2]=new TFile("Excl_T5bbbbZg_LimitPlots_SbinV7_ObsTotBG_r.root");
  //  f[2]=new TFile("EMHT_Limits_Knut/T5bbbbZg_v20/saved_graphs1d_limit.root");
  // f[5]=new TFile("Excl_T5bbbbZg_LimitPlots_SbinV6.root");
  // f[0]=new TFile("Excl_T5ttttZg_LimitPlots_SbinV7_v2.root");
  // f[1]=new TFile("Excl_T5ttttZg_LimitPlots_SbinV7_HybridNew.root");
  // f[1]=new TFile("Excl_T5ttttZg_LimitPlots_SbinV4.root");
  // f[2]=new TFile("Excl_T5ttttZg_LimitPlots_SbinV4.root");
  //  f[2]=new TFile("EMHT_Limits_Knut/T5ttttZg_v20/saved_graphs1d_limit.root");
  //TFile *f1=new TFile("EMHT_Limits_Knut/T5ttttZg_v20/saved_graphs1d_limit.root");
  //  TLegend *leg = new TLegend(0.8,0.6,0.9,0.9);
  TLegend *leg = new TLegend(0.17,0.76,0.85,0.89);
  // TH2D *h2=(TH2D*)f[0]->FindObjectAny("mGlmNLSP_XsecUL");
  // h2->Draw("colz");
  TH2D *h2 = new TH2D("mGlmNLSPPlane","mGlmNLSPPlane",100,12.5,2512.5,250,5,2505);
  //  gStyle->SetPadLeftMargin(0.15);
  c1->SetLeftMargin(0.15);
  h2->GetXaxis()->SetTitleSize(0.05);
  h2->GetXaxis()->SetLabelSize(0.05);
  h2->GetYaxis()->SetTitleSize(0.05);
  h2->GetYaxis()->SetLabelSize(0.05);
  TString modelName=f[0]->GetName();
  if(modelName.Contains("T5qqqqHg")) modelName="T5qqqqHg";
  else if(modelName.Contains("T5ttttZg")) modelName="T5ttttZg";
  else if(modelName.Contains("T5bbbbZg")) modelName="T5bbbbZg";
  if(!modelName.Contains("GGM_M1M3")) h2->SetTitle(";m_{#tilde{g}} (GeV);m_{#tilde{#chi}_{1}^{0}} (GeV)");
  else if(modelName.Contains("GGM_M1M3")) h2->SetTitle(";M3 (GeV);M1 (GeV)");
  if(modelName.Contains("T6tt")) h2->SetTitle(";m_{ #tilde{t}} (GeV);m_{#tilde{#chi}_{1}^{0}} (GeV)");
  h2->SetTitle(modelName);
  h2->Draw();
  TGraph *gr[nFiles];
  for(int i=0;i<nFiles;i++){
    gr[i]=(TGraph*)f[i]->FindObjectAny("exp");
    gr[i]->SetLineColor(col[i]);
    gr[i]->Draw("same");
    TString fName=f[i]->GetName();
    if(fName.Contains("EMHT")) fName = "SUS-16-047, H_{T}^{#gamma}";
    if(fName.Contains("SbinV1")) fName = "(N_{2-4}, N_{5-6}, N_{#geq7}) x (N^{#geq0})";//"B1:N^{#geq0}_{2-4}, N^{#geq0}_{5-6}, N^{#geq0}_{#geq7},";
    if(fName.Contains("SbinV2")) fName = "(N_{2-4}, N_{#geq5}) x (N^{0}, N^{1}, N^{#geq2})";//"B2:N^{0}_{2-4}, N^{0}_{#geq5}, N^{1}_{2-4}, N^{1}_{#geq5}, N^{#geq2}_{2-4}, N^{#geq2}_{#geq5}";
    if(fName.Contains("SbinV3")) fName = "(N_{2-4}, N_{5-6}, N_{#geq7}) x (N^{0}, N^{1}, N^{#geq2})";
    if(fName.Contains("SbinV4")) fName = "(N_{2-4}, N_{5-6}, N_{#geq7}) x (N^{0}, N^{#geq1})";
    if(fName.Contains("SbinV5")) fName = "Merged MET";
    if(fName.Contains("SbinV6")) fName = "(N_{2-4}, N_{5-6}) x (N^{0}, N^{1}) + N_{#geq7} ";
    if(fName.Contains("v9")) fName = "Earlier";
    if(fName.Contains("v11")) fName = "#gamma + (b) jets, SUS-18-002";
    //    if(fName.Contains("V7_v2")) fName = "Asymptotic";//"Photon+(b)Jets";
    if(fName.Contains("HybridNew")) fName = "HybridNew";
    if(fName.Contains("GGM_M1M3")){
      if(fName.Contains("PhoLep") && !fName.Contains("PhoJet")) fName = "#gamma + lepton";
      else if(fName.Contains("PhoJet") &&  fName.Contains("PhoLep")) fName = "Combination #gamma + (lepton + (b)jet)";
      else fName = "#gamma + (b)jet";
    }

    // if(fName.Contains("EMHT")) fName = "HT#gamma";
    // if(fName.Contains("SbinV1")) fName = "V1:NJ bins, Incl b's";
    // if(fName.Contains("SbinV2")) fName = "V2:nj x (0b,1b,>=2b)";
    // if(fName.Contains("SbinV3")) fName = "V3:NJ x (0b,1b,>=2b)";
    // if(fName.Contains("SbinV4_mT100")) fName = "V4(mT>100):NJ x (0b,>=1b)";
    // if(fName.Contains("SbinV4")) fName = "V4:NJ x (0b,>=1b)";
    // if(fName.Contains("SbinV5")) fName = "V5:NJ x (0b,1b,>=2b), Merged MET";
    // if(fName.Contains("SbinV6")) fName = "V6:Incl b for NJ>=7";
    leg->AddEntry(gr[i],fName,"l");
    leg->SetBorderSize(0);
  }
  leg->SetNColumns(2);
  leg->Draw();
  char name3[100];
  textOnTop.SetTextSize(0.045);
  intLumiE.SetTextSize(0.045);
  //  textOnTop.DrawLatexNDC(0.16,0.91,"CMS #it{#bf{Simulation}}");
  textOnTop.DrawLatexNDC(0.16,0.91,"CMS #it{#bf{Preliminary}}");
  sprintf(name3,"#bf{%0.1f fb^{-1} (13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.65,0.91,name3);


}

// void overlayExcl(){
//   const int nFiles=5;
//   int col[5]={kRed,kBlue,kMagenta,kTeal+9,kBlack};
//   gStyle->SetOptStat(0);
//   TCanvas *c1=new TCanvas("c1","c1",1200,1000);
//   c1->SetLogz();
//   TFile *f[nFiles];
//   f[0]=new TFile("EMHT_Limits_Knut/T5qqqqHg_v20/saved_graphs1d_limit.root");
//   f[1]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV3.root");
//   f[2]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV4.root");
//   f[3]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV5.root");
//   f[4]=new TFile("Excl_T5qqqqHg_LimitPlots_SbinV6.root");
//   //  f[1]=new TFile("a.root");
//   // f[1]=new TFile("Excl_T5ttttZg_LimitPlots_SbinV4.root");
//   // f[2]=new TFile("Excl_T5ttttZg_LimitPlots_SbinV4.root");

//   //  TFile *f1=new TFile("EMHT_Limits_Knut/T5ttttZg_v20/saved_graphs1d_limit.root");
//   TLegend *leg = new TLegend(0.8,0.6,0.9,0.9);
//   TH2D *h2=(TH2D*)f[0]->FindObjectAny("obs_hist");
//   h2->Draw();
//   TGraph *gr[nFiles];
//   for(int i=0;i<nFiles;i++){
//     gr[i]=(TGraph*)f[i]->FindObjectAny("exp");
//     gr[i]->SetLineColor(col[i]);
//     gr[i]->Draw("same");
//     TString fName=f[i]->GetName();
//     if(fName.Contains("EMHT")) fName = "HT#gamma";
//     if(fName.Contains("SbinV1")) fName = "V1";
//     if(fName.Contains("SbinV2")) fName = "V2";
//     if(fName.Contains("SbinV3")) fName = "V3";
//     if(fName.Contains("SbinV4")) fName = "V4";
//     if(fName.Contains("SbinV5")) fName = "V5";
//     if(fName.Contains("SbinV6")) fName = "V6";
//     leg->AddEntry(gr[i],fName,"l");
//   }

//   leg->Draw();

// }
