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
void getSignificance(TString fName){
  bool obsSignif = 1;
  if(fName.Contains("Obs")) obsSignif = 1;
  else obsSignif = 0;
  TFile *f1=new TFile(fName);
  TFile *fout =new TFile("Excl_"+fName,"recreate");
  TH2D *h2_rvalue=(TH2D*)f1->FindObjectAny("mGlmNLSP_r");
 //---------------- set styles----------------
  gStyle->SetOptStat(0);
  gStyle->SetTitle(0);
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.50, 0.50, 1.00, 1.00, 1.00 };
  Double_t green[NRGBs] = { 0.50, 1.00, 1.00, 0.60, 0.50 };
  Double_t blue[NRGBs]  = { 1.00, 1.00, 0.50, 0.40, 0.50 };
  
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  //---------------------------------   
  //-----------cheat here------------
  // h2_rvalue->SetBinContent(h2_rvalue->GetXaxis()->FindBin(1750),h2_rvalue->GetYaxis()->FindBin(1450),4.85);
  // h2_rvalue->SetBinContent(h2_rvalue->GetXaxis()->FindBin(1750),h2_rvalue->GetYaxis()->FindBin(1500),4.86);
  // h2_rvalue->SetBinContent(h2_rvalue->GetXaxis()->FindBin(1750),h2_rvalue->GetYaxis()->FindBin(1520),4.88);
  //---------------------------------
  //  TCanvas *c3=new TCanvas("c3","c3",1500,1500);
  TCanvas *c3=new TCanvas("c3","c3",1600,1200);
  c3->SetLeftMargin(0.15);
  c3->SetRightMargin(0.2);
  // c3->SetBottomMargin(0.12);
  // c3->SetLeftMargin(0.12);
  // c3->SetRightMargin(0.12);
  TGraph2D *gr2d_r = new TGraph2D(h2_rvalue);
  TGraph2D *gr2dXsec = new TGraph2D(h2_rvalue);
  gr2dXsec->SetNpx(500);
  gr2dXsec->SetNpy(500);
  TList *list;
  TGraph *gr1d;
  if(!obsSignif){
    list =gr2d_r->GetContourList(5.0);
    gr1d=(TGraph*)list->First();
  }

  // TList *listUp =gr2d_r->GetContourList(1.0);
  // TGraph *gr1dUp=(TGraph*)list->First();

  int k=0;

  if(!obsSignif){
    gr1d->SetLineColor(kRed);
    gr1d->SetLineWidth(2);
  }

  gr2dXsec->SetTitle(";m_{#tilde{g}}(GeV);m_{#tilde{#chi}_{1}^{0}}(GeV)");
  gr2dXsec->GetXaxis()->SetTitleSize(0.05);
  gr2dXsec->GetXaxis()->SetLabelSize(0.05);
  gr2dXsec->GetYaxis()->SetTitleSize(0.05);
  gr2dXsec->GetYaxis()->SetLabelSize(0.05);
  //  c3->SetLogz();
  //gr2dXsec->GetXaxis()->SetLabelSize(0.4);
  //  gr2dXsec->SetMaximum(1.0);
  //h2_rvalue->Draw();
  gr2dXsec->SetMinimum(0.00001);
  //  gr2dXsec->SetMinimum(-1.0);
  if(!obsSignif) gr2dXsec->SetMaximum(10.);
  else gr2dXsec->SetMaximum(0.4);
  
  gr2dXsec->Draw("COLZ");
  if(!obsSignif)  gr1d->Draw("C");
  if(!obsSignif) gr2dXsec->GetZaxis()->SetTitle("Expected Significance");
  else gr2dXsec->GetZaxis()->SetTitle("Observed Significance");
  gr2dXsec->GetZaxis()->SetTitleSize(0.05);
  gr2dXsec->GetZaxis()->SetTitleOffset(1.1);

  TString modelName=f1->GetName();
  if(modelName.Contains("T5qqqqHg")) modelName="T5qqqqHg";
  else if(modelName.Contains("T5ttttZg")) modelName="T5ttttZg";
  else if(modelName.Contains("T5bbbbZg")) modelName="T5bbbbZg";
  else if(modelName.Contains("T6ttZg"  )) modelName="T6ttZg";

  if(modelName!="T6ttZg") gr2dXsec->SetTitle(";m_{#tilde{g}}(GeV);m_{#tilde{#chi}_{1}^{0}}(GeV)");
  else gr2dXsec->SetTitle(";m_{#tilde{ t }}(GeV);m_{#tilde{#chi}_{1}^{0}}(GeV)");
  //  gr2dXsec->SetTitle(modelName);
  // gr2dXsec->GetXaxis()->SetTitleSize(0.05);
  // gr2dXsec->GetXaxis()->SetLabelSize(0.05);
  // gr2dXsec->GetYaxis()->SetTitleSize(0.05);
  // gr2dXsec->GetYaxis()->SetLabelSize(0.05);

  // TString binType=f1->GetName();
  // if(binType.Contains("SbinV1")) binType="high NJ bins only";
  // else binType="NJ, NbJ bins";
  // TPaveText *text2=new TPaveText(1450,2100,1700,2400,"NB");
  // text2->AddText(binType);   text2->Draw();

  //----------------------
  //  TPaveText *decayMode = new TPaveText(0.15,0.7, 0.8,0.9,"NDC");
  TPaveText *decayMode = new TPaveText(0.15,0.8, 0.8,0.9,"NDC");
  decayMode->SetShadowColor(0);   decayMode->SetFillColor(0);
  decayMode->SetLineWidth(3);
  if(!obsSignif) decayMode->AddText(" ");
  if(modelName.Contains("T5qqqqHg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow q #bar{q} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/H #tilde{G}}");
    modelName="T5qqqqHg";}
  else if(modelName.Contains("T5ttttZg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow t #bar{t} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G}}");
    modelName="T5ttttZg";}
  else if(modelName.Contains("T5bbbbZg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G}}");
    modelName="T5bbbbZg";}
  else if(modelName.Contains("T6ttZg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{t} #tilde{t}, #tilde{t} #rightarrow t #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G}}");
    modelName="T6ttZg";}
  else decayMode->AddText(modelName);
  // decayMode->AddText(" ");
  decayMode->Draw();
  //TLegend *legText = new TLegend(0.17,0.72, 0.45,0.82);
  //  TLegend *legText = new TLegend(0.27,0.81, 0.55,0.84);
  TLegend *legText = new TLegend(0.27,0.89, 0.55,0.86);
  legText->SetTextSize(0.04);
  legText->SetBorderSize(0);
  legText->SetFillColor(0);
  if(!obsSignif){
    legText->AddEntry(gr1d,"Expected 5#sigma","l");
    legText->Draw();
  }

  //----------------------
  char name3[100];
  textOnTop.SetTextSize(0.04);
  intLumiE.SetTextSize(0.04);
  textOnTop.DrawLatexNDC(0.15,0.91,"CMS #it{#bf{Preliminary}}");
  sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.63,0.91,name3);

  c3->Update();
  fout->cd();
  gr2dXsec->Write();
  TGraph *gr1d_cp;
  if(!obsSignif){ 
    gr1d_cp=(TGraph*)gr1d->Clone("exp");
    gr1d_cp->Write();
  }
  if(obsSignif)c3->SaveAs(modelName+"_ObsSignif.pdf");
  else c3->SaveAs(modelName+"_ExpSignif.pdf");
}


