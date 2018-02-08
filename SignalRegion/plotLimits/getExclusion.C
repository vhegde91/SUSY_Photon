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
void getExclusion(TString fName){
  TFile *f1=new TFile(fName);
  TFile *fout =new TFile("Excl_"+fName,"recreate");
  TH2D *h2_rvalue=(TH2D*)f1->FindObjectAny("mGlmNLSP_median");
  TH2D *h2_r16pc=(TH2D*)f1->FindObjectAny("mGlmNLSP_16pc");
  TH2D *h2_r84pc=(TH2D*)f1->FindObjectAny("mGlmNLSP_84pc");
  TH2D *h2_XsecUL=(TH2D*)f1->FindObjectAny("mGlmNLSP_XsecUL");
  //  TH2D *h2_empty=new TH2D("h2","h2",110,1400,2500,300,0,3000);
  //  gStyle->SetPalette(kTemperatureMap);
  TCanvas *c3=new TCanvas("c3","c3",1200,1500);
  //c3->SetBottomMargin(0.12);
  c3->SetLeftMargin(0.15);
  c3->SetRightMargin(0.2);
  TGraph2D *gr2d_r = new TGraph2D(h2_rvalue);
  TGraph2D *gr2dXsec = new TGraph2D(h2_XsecUL);
  //  TGraph2D *gr2dXsec = new TGraph2D(h2_rvalue);
  gr2dXsec->SetNpx(500);
  gr2dXsec->SetNpy(500);
  TList *list =gr2d_r->GetContourList(1.0);
  TGraph *gr1d=(TGraph*)list->First();
  //-----------------------
  TGraph2D *gr2d_16pc = new TGraph2D(h2_r16pc);
  TGraph2D *gr2d_84pc = new TGraph2D(h2_r84pc);  
  TList *list16pc =gr2d_16pc->GetContourList(1.0);
  TList *list84pc =gr2d_84pc->GetContourList(1.0);
  TGraph *gr1d_16pc=(TGraph*)list16pc->First();
  TGraph *gr1d_84pc=(TGraph*)list84pc->First();
  //----------------------
  int k=0;
  // gStyle->Reset();
  gr1d->SetLineColor(kRed);
  gr1d->SetLineWidth(2);
  gr1d_16pc->SetLineColor(kRed);
  gr1d_16pc->SetLineWidth(2);gr1d_16pc->SetLineStyle(2);
  gr1d_84pc->SetLineColor(kRed);
  gr1d_84pc->SetLineWidth(2);gr1d_84pc->SetLineStyle(2);

  //  gr2dXsec->SetTitle(";m_{#tilde{g}}(GeV);m_{#tilde{#chi}_{1}^{0}}(GeV)");
  gr2dXsec->GetXaxis()->SetTitleSize(0.05);
  gr2dXsec->GetXaxis()->SetLabelSize(0.05);
  gr2dXsec->GetYaxis()->SetTitleSize(0.05);
  gr2dXsec->GetYaxis()->SetLabelSize(0.05);
  c3->SetLogz();
  //gr2dXsec->GetXaxis()->SetLabelSize(0.4);
  //  gr2dXsec->SetMaximum(1.0);
  gr2dXsec->SetMinimum(0.0);
  //  h2_empty->Draw("text");
  gr2dXsec->Draw("COLZ");
  gr1d->Draw("C");
  gr1d_16pc->Draw("C");
  gr1d_84pc->Draw("C");
  //---------- for profile likely----
  // TFile *f2 = new TFile("T5bbbbZg_MedExp_ProfileLikelyToy400_SbinV7.root");
  // TH2D *h2_medProf=(TH2D*)f2->FindObjectAny("mGlmNLSP_medProfL");
  // TGraph2D *gr2d_medProf = new TGraph2D(h2_medProf);  
  // TList *list_medProf =gr2d_medProf->GetContourList(1.0);
  // TGraph *gr1d_medProf=(TGraph*)list_medProf->First();
  // gr1d_medProf->SetLineWidth(2);
  // gr1d_medProf->Draw("C");
  //---------------------------------
  // gr2dXsec->GetXaxis()->SetRangeUser(1600,2400);
  gr2dXsec->GetZaxis()->SetTitle("95% UL on #sigma[pb]");
  gr2dXsec->GetZaxis()->SetTitleSize(0.05);
  gr2dXsec->GetZaxis()->SetTitleOffset(1.1);
  gr2dXsec->SetMinimum(0.00001);
  gr2dXsec->SetMaximum(0.1);
  TString modelName=f1->GetName();
  if(modelName.Contains("T5qqqqHg")) modelName="T5qqqqHg";
  else if(modelName.Contains("T5ttttZg")) modelName="T5ttttZg";
  else if(modelName.Contains("T5bbbbZg")) modelName="T5bbbbZg";

  gr2dXsec->SetTitle(";m_{#tilde{g}}[GeV];m_{#tilde{#chi}_{1}^{0}}[GeV]");
  gr2dXsec->SetTitle(modelName);
  // gr2dXsec->GetXaxis()->SetTitleSize(0.05);
  // gr2dXsec->GetXaxis()->SetLabelSize(0.05);
  // gr2dXsec->GetYaxis()->SetTitleSize(0.05);
  // gr2dXsec->GetYaxis()->SetLabelSize(0.05);

 
  // TPaveText *text1=new TPaveText(1950,2200,2100,2400,"NB");
  // text1->AddText(modelName);   text1->Draw();

  // TString binType=f1->GetName();
  // if(binType.Contains("SbinV1")) binType="high NJ bins only";
  // else binType="NJ, NbJ bins";
  // TPaveText *text2=new TPaveText(1450,2100,1700,2400,"NB");
  // text2->AddText(binType);   text2->Draw();

  c3->Update();
  fout->cd();
  gr2dXsec->Write();

  h2_XsecUL->Write();
  TGraph *gr1d_cp=(TGraph*)gr1d->Clone("exp");    gr1d_cp->Write();
  TGraph *gr1d_16pccp=(TGraph*)gr1d_16pc->Clone("exp16pc");   gr1d_16pccp->Write();
  TGraph *gr1d_84pccp=(TGraph*)gr1d_84pc->Clone("exp84pc");   gr1d_84pccp->Write();

  char name3[100];
  textOnTop.SetTextSize(0.04);
  intLumiE.SetTextSize(0.04);
  textOnTop.DrawLatexNDC(0.15,0.91,"CMS #it{#bf{Preliminary}}");
  //  intLumiE.SetTextSize(0.04);
  sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.63,0.91,name3);

}
