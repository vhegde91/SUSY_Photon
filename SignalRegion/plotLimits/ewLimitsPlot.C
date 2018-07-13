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
void ewLimitsPlot(TString fName){
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
  TFile *f1=new TFile(fName);
  TFile *fout =new TFile("Excl_"+fName,"recreate");
  TFile *xsecFile = TFile::Open("TChiNG_MassScan.root");
  double xMin = 500, xMax = 1250;
  double yMin = 0.0001, yMax = 2;
  TH1D *h_xsec;
  //  double zMin = 0.00159844, zMax = 0.09;
  int nDivAxis = 505;
  TString modelName=f1->GetName();
  if(modelName.Contains("T5bbbb")){ xMin = 1450; xMax = 2450; nDivAxis = 510;}
  if(modelName.Contains("T5tttt"))  { xMin = 1450; xMax = 2450; nDivAxis = 510;}
  if(modelName.Contains("TChiNG")) h_xsec=(TH1D*)xsecFile->Get("mEWeakino_SumN2C1_C1C1Xsec");
  if(modelName.Contains("TChiWG")) h_xsec=(TH1D*)xsecFile->Get("mEWeakino_N2C1Xsec");

  TPaveText *decayMode = new TPaveText(0.15,0.7, 0.8,0.9,"NDC");
  decayMode->SetShadowColor(0);   decayMode->SetFillColor(0); 
  decayMode->SetLineWidth(3);    //decayMode->SetBorderColor(kBlack);
  TLegend *legText = new TLegend(0.17,0.72, 0.45,0.82);

  TH1D *h_rMedian=(TH1D*)(((TH2D*)f1->Get("mGlmNLSP_median"))->ProjectionY("exp",4,4));
  TH1D *h_r16pc=(TH1D*)(((TH2D*)f1->Get("mGlmNLSP_16pc"))->ProjectionY("exp1L",4,4));
  TH1D *h_r84pc=(TH1D*)(((TH2D*)f1->Get("mGlmNLSP_84pc"))->ProjectionY("exp1U",4,4));
  TH1D *h_r2p5pc=(TH1D*)(((TH2D*)f1->Get("mGlmNLSP_2p5pc"))->ProjectionY("exp2L",4,4));
  TH1D *h_r97p5pc=(TH1D*)(((TH2D*)f1->Get("mGlmNLSP_97p5pc"))->ProjectionY("exp2U",4,4));
  TH1D *h_rObs=(TH1D*)(((TH2D*)f1->Get("mGlmNLSP_r"))->ProjectionY("obs",4,4));
  for(int i=1;i<=h_rMedian->GetNbinsX();i++){
    if(h_rMedian->GetBinContent(i) < 0.0000001) continue;
    double xsec=(h_xsec->GetBinContent(h_xsec->FindBin(h_rMedian->GetBinCenter(i))));
    if(xsec < 0.0000000001) xsec = (h_xsec->GetBinContent(h_xsec->FindBin(h_rMedian->GetBinCenter(i)+5)));
    if(xsec < 0.0000000001) xsec = (h_xsec->GetBinContent(h_xsec->FindBin(h_rMedian->GetBinCenter(i)-5)));
    cout<<i<<" "<<h_rMedian->GetBinCenter(i)<<" "<<xsec<<endl;
    h_rMedian->SetBinContent(i,xsec*h_rMedian->GetBinContent(i));
    h_r16pc->SetBinContent(i,xsec*h_r16pc->GetBinContent(i));
    h_r84pc->SetBinContent(i,xsec*h_r84pc->GetBinContent(i));
    h_r2p5pc->SetBinContent(i,xsec*h_r2p5pc->GetBinContent(i));
    h_r97p5pc->SetBinContent(i,xsec*h_r97p5pc->GetBinContent(i));
    h_rObs->SetBinContent(i,xsec*h_rObs->GetBinContent(i));

    h_rMedian->SetBinError(i,0);
    h_r16pc->SetBinError(i,0);
    h_r84pc->SetBinError(i,0);
    h_r2p5pc->SetBinError(i,0);
    h_r97p5pc->SetBinError(i,0);
    h_rObs->SetBinError(i,0);
  }
  //  TGraph2D *gr2d_rMedian = new TGraph2D(h2_rMedian);
  //  gr2dXsec->SetNpx(250);  gr2dXsec->SetNpy(250);

  TCanvas *c3=new TCanvas("c3","c3",1300,1000);
  //c3->SetBottomMargin(0.12);
  c3->SetLeftMargin(0.15);
  c3->SetRightMargin(0.2);
  //------------------------------------------------
  //  gr1d_Median->Print("all");
  h_rMedian->SetLineColor(kRed);
  h_rMedian->SetLineWidth(3);h_rMedian->SetLineStyle(7);
  h_r16pc->SetLineColor(kRed);
  h_r16pc->SetLineWidth(2);h_r16pc->SetLineStyle(7);
  h_r84pc->SetLineColor(kRed);
  h_r84pc->SetLineWidth(2);h_r84pc->SetLineStyle(7);

  h_rObs->SetLineColor(kBlack); //h_rObs->SetLineStyle(2);
  h_rObs->SetLineWidth(3);
  //------------------------------------------------

  c3->SetLogy();
  cout<<modelName<<endl;
  // if(!(modelName.Contains("T6ttZg")))  h2_XsecUL->SetTitle(";m_{#tilde{g}} (GeV);m_{#tilde{#chi}_{1}^{0}} (GeV)");
  // else                     h2_XsecUL->SetTitle(";m_{ #tilde{t} } (GeV);m_{#tilde{#chi}_{1}^{0}} (GeV)");
  h_rMedian->Draw("L"); 
  // h_r16pc->Draw("same e0");   
  // h_r84pc->Draw("same e0");
  h_rObs->Draw("same e0");
  h_xsec->Draw("same e0");
  gPad->RedrawAxis();
  //-----------------------------------------------
  if(modelName.Contains("T5qqqqHg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow q #bar{q} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/H #tilde{G}  NLO+NLL Exclusion}");
    modelName="T5qqqqHg";}
  // decayMode->Draw();
  
  legText->SetTextSize(0.04);
  legText->SetBorderSize(0);
  legText->SetFillColor(0);
  // legText->AddEntry(gr1d_XsecDn,"","l");
  // legText->AddEntry(gr1d_Obs,"Observed #pm 1#sigma_{theory}","l");
  // legText->AddEntry(gr1d_XsecDn,"","l");

  // legText->AddEntry(gr1d_16pc,"","l");
  // legText->AddEntry(gr1d_Median,"Expected #pm 1#sigma_{experiment}","l");
  // legText->AddEntry(gr1d_84pc,"","l");
  //legText->Draw();
  //-----------------------------------------------
  
  c3->Update();
  fout->cd();
  //  TGraph *gr1d_Median_cp=(TGraph*)gr1d_Median->Clone("exp");    gr1d_Median_cp->Write();

  char name3[100];
  textOnTop.SetTextSize(0.05);
  intLumiE.SetTextSize(0.05);
  textOnTop.DrawLatexNDC(0.16,0.91,"CMS #it{#bf{Preliminary}}");
  sprintf(name3,"#bf{%0.1f fb^{-1} (13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.57,0.91,name3);
  c3->SaveAs(modelName+"_exclusion.pdf");
  //  c3->SaveAs(modelName+"_exclusion.png");
}
