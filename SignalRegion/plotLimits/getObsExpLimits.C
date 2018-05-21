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
void getObsExpLimits(TString fName){
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
  double xMin = 1450, xMax = 2450;
  double yMin = 0, yMax = 3100;
  double zMin = 0.000005, zMax = 0.09;
  int nDivAxis = 505;
  TString modelName=f1->GetName();
  if(modelName.Contains("T5bbbb") || modelName.Contains("T5qqqq")){ xMin = 1450; xMax = 2450; nDivAxis = 510;}
  if(modelName.Contains("T5tttt")                              )  { xMin = 1450; xMax = 2450; nDivAxis = 510;}
  if(modelName.Contains("T6tt")                                )  { xMin =  800; xMax = 1451; nDivAxis = 510; yMin = 0.; yMax = 2000.; zMin = 0.001; zMax = 1.0;}

  TPaveText *decayMode = new TPaveText(0.15,0.7, 0.8,0.9,"NDC");
  decayMode->SetShadowColor(0);   decayMode->SetFillColor(0); 
  decayMode->SetLineWidth(3);    //decayMode->SetBorderColor(kBlack);
  TLegend *legText = new TLegend(0.17,0.72, 0.45,0.82);

  TH2D *h2_rMedian=(TH2D*)f1->FindObjectAny("mGlmNLSP_median");//median exp 50%
  TH2D *h2_r16pc=(TH2D*)f1->FindObjectAny("mGlmNLSP_16pc");//median exp 16%
  TH2D *h2_r84pc=(TH2D*)f1->FindObjectAny("mGlmNLSP_84pc");//meadian exp 84%

  TH2D *h2_rObs=(TH2D*)f1->FindObjectAny("mGlmNLSP_r");//obs limit
  TH2D *h2_rXsecUp=(TH2D*)f1->FindObjectAny("mGlmNLSP_r_XsecUp");//obs limit+theory xsec up
  TH2D *h2_rXsecDn=(TH2D*)f1->FindObjectAny("mGlmNLSP_r_XsecDn");//obs limit+theory xsec down
  TH2D *h2_XsecUL=(TH2D*)f1->FindObjectAny("mGlmNLSP_XsecUL");//UL on xsec based on obs limit

  TH2D *h2_temp=new TH2D("h2temp","h2temp",60,25,3025,300,5,3005);
  // gStyle->SetPadGridX(0);
  // gStyle->SetPadGridY(0);
  //------------------------------------------------
  TGraph2D *gr2d_rMedian = new TGraph2D(h2_rMedian);
  TGraph2D *gr2d_16pc = new TGraph2D(h2_r16pc);
  TGraph2D *gr2d_84pc = new TGraph2D(h2_r84pc);  

  TGraph2D *gr2d_rObs = new TGraph2D(h2_rObs);
  TGraph2D *gr2d_rXsecUp = new TGraph2D(h2_rXsecUp);
  TGraph2D *gr2d_rXsecDn = new TGraph2D(h2_rXsecDn);  
  TGraph2D *gr2dXsec = new TGraph2D(h2_XsecUL);
  gr2dXsec->SetNpx(250);  gr2dXsec->SetNpy(250);
  //------------------------------------------------
  TList *listMedian =gr2d_rMedian->GetContourList(1.0);
  TList *list16pc =gr2d_16pc->GetContourList(1.0);
  TList *list84pc =gr2d_84pc->GetContourList(1.0);

  TList *listObs =gr2d_rObs->GetContourList(1.0);
  TList *listXsecUp =gr2d_rXsecUp->GetContourList(1.0);
  TList *listXsecDn =gr2d_rXsecDn->GetContourList(1.0);
  //------------------------------------------------
  TGraph *gr1d_Median=(TGraph*)listMedian->First();
  TGraph *gr1d_16pc=(TGraph*)list16pc->First();
  TGraph *gr1d_84pc=(TGraph*)list84pc->First();

  TGraph *gr1d_Obs=(TGraph*)listObs->First();
  TGraph *gr1d_XsecUp=(TGraph*)listXsecUp->First();
  TGraph *gr1d_XsecDn=(TGraph*)listXsecDn->First();
  //------------------------------------------------

  TCanvas *c3=new TCanvas("c3","c3",1300,1000);
  //c3->SetBottomMargin(0.12);
  c3->SetLeftMargin(0.15);
  c3->SetRightMargin(0.2);
  //------------------------------------------------
  gr1d_Median->SetLineColor(kRed);
  gr1d_Median->SetLineWidth(3);gr1d_Median->SetLineStyle(7);
  gr1d_16pc->SetLineColor(kRed);
  gr1d_16pc->SetLineWidth(2);gr1d_16pc->SetLineStyle(7);
  gr1d_84pc->SetLineColor(kRed);
  gr1d_84pc->SetLineWidth(2);gr1d_84pc->SetLineStyle(7);

  gr1d_Obs->SetLineColor(kBlack);
  gr1d_Obs->SetLineWidth(3);
  gr1d_XsecUp->SetLineColor(kBlack);
  gr1d_XsecUp->SetLineWidth(2);//gr1d_XsecUp->SetLineStyle(2);
  gr1d_XsecDn->SetLineColor(kBlack);
  gr1d_XsecDn->SetLineWidth(2);//gr1d_XsecDn->SetLineStyle(2);
  //------------------------------------------------

  c3->SetLogz();
  h2_XsecUL->GetXaxis()->SetRangeUser(xMin,xMax);
  h2_XsecUL->GetYaxis()->SetRangeUser(yMin,yMax);
  if(!modelName.Contains("T6ttZg")) gr2dXsec->GetYaxis()->SetNdivisions(505);
  h2_XsecUL->SetMinimum(zMin);
  h2_XsecUL->SetMaximum(zMax);
  h2_XsecUL->Draw();
  cout<<modelName<<endl;
  if(!(modelName.Contains("T6ttZg")))  h2_XsecUL->SetTitle(";m_{#tilde{g}}(GeV);m_{#tilde{#chi}_{1}^{0}}(GeV)");
  else                     h2_XsecUL->SetTitle(";m_{ #tilde{t} }(GeV);m_{#tilde{#chi}_{1}^{0}}(GeV)");

  //  gr2dXsec->SetHistogram(h2_XsecUL);
  gr2dXsec->Draw("same COLZ");
  gr1d_Median->Draw("C"); 
  gr1d_16pc->Draw("C");   
  gr1d_84pc->Draw("C");
  gr1d_Obs->Draw("C");
  gr1d_XsecUp->Draw("C");
  gr1d_XsecDn->Draw("C");
  gPad->RedrawAxis();
  //-----------------------------------------------
  if(modelName.Contains("T5qqqqHg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow q #bar{q} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/H #tilde{G}  NLO+NLL Exclusion}");
    modelName="T5qqqqHg";}
  else if(modelName.Contains("T5ttttZg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow t #bar{t} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G}  NLO+NLL Exclusion}");
  modelName="T5ttttZg";}
  else if(modelName.Contains("T5bbbbZg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G}  NLO+NLL Exclusion}");
  modelName="T5bbbbZg";}
  else if(modelName.Contains("T6ttZg")){
    decayMode->AddText("#bf{pp #rightarrow #tilde{t} #tilde{t}, #tilde{t} #rightarrow t #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G}  NLO+NLL Exclusion}");
  modelName="T6ttZg";}
  else decayMode->AddText(modelName);
  decayMode->AddText(" ");
  decayMode->AddText(" ");
  decayMode->Draw();

  legText->SetTextSize(0.04);
  legText->SetBorderSize(0);
  legText->SetFillColor(0);
  legText->AddEntry(gr1d_XsecDn,"","l");
  legText->AddEntry(gr1d_Obs,"Observed #pm 1#sigma_{theory}","l");
  legText->AddEntry(gr1d_XsecDn,"","l");

  legText->AddEntry(gr1d_16pc,"","l");
  legText->AddEntry(gr1d_Median,"Expected #pm 1#sigma_{experiment}","l");
  legText->AddEntry(gr1d_84pc,"","l");
  legText->Draw();
  //-----------------------------------------------
  // gr2dXsec->GetZaxis()->SetTitleSize(0.05);
  // gr2dXsec->GetZaxis()->SetTitleOffset(1.25);
  gr2dXsec->SetMinimum(zMin);
  gr2dXsec->SetMaximum(zMax);
  gr2dXsec->GetZaxis()->SetTitle("95% CL upper limit on cross section(pb)");
  gr2dXsec->GetZaxis()->SetTitleOffset(1.2);
  
 // TPaveText *pt = new TPaveText(.05,.1,.95,.8);
 //   pt->AddText("A TPaveText can contain severals line of text.");
 //  pt->Draw();
  
  c3->Update();
  fout->cd();
  gr2dXsec->Write();

  h2_XsecUL->Write();
  TGraph *gr1d_Median_cp=(TGraph*)gr1d_Median->Clone("exp");    gr1d_Median_cp->Write();
  TGraph *gr1d_16pccp=(TGraph*)gr1d_16pc->Clone("exp16pc");   gr1d_16pccp->Write();
  TGraph *gr1d_84pccp=(TGraph*)gr1d_84pc->Clone("exp84pc");   gr1d_84pccp->Write();
  TGraph *gr1d_Obs_cp=(TGraph*)gr1d_Obs->Clone("obs");    gr1d_Obs_cp->Write();

  char name3[100];
  textOnTop.SetTextSize(0.05);
  intLumiE.SetTextSize(0.05);
  textOnTop.DrawLatexNDC(0.16,0.91,"CMS #it{#bf{Preliminary}}");
  sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.57,0.91,name3);
  c3->SaveAs(modelName+"_exclusion.pdf");
  //  c3->SaveAs(modelName+"_exclusion.png");
}
