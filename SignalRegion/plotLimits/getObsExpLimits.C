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
void removePoint(TGraph*,double,double);
TGraph *cleanupDiagonal(TGraph*,TString);
void getObsExpLimits(TString fName){
  //---------------- set styles if not running on my desktop ---------------
  //---------------- otherwise you do not need this part -------------------
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetHistLineWidth(2);
  gStyle->SetTitleSize(0.05,"X");
  gStyle->SetLabelSize(0.05,"X");
  gStyle->SetTitleSize(0.05,"Y");
  gStyle->SetLabelSize(0.05,"Y");
  gStyle->SetTitleSize(0.05,"Z");
  gStyle->SetLabelSize(0.05,"Z");
  gStyle->SetLineScalePS(1.5);
  gROOT->ForceStyle();
  //---------------- end of settings ----------------------------
  //---------------- set styles for all computers ----------------
  gStyle->SetOptStat(0);
  gStyle->SetTitle(0);
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  bool cleanDiagnl = 1;
  bool xsecULinfb = 1;

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
  //double zMin = 0.000005, zMax = 0.09;
  //  double zMin = 0.4, zMax = 20;//EMHT, STgamma
  //double zMin = 0.0004, zMax = 0.002;//gluinos
  double zMin = 0.0004, zMax = 0.004;//gluinos tttt //appr
  //   double zMin = 0.00001, zMax = 0.004;
  //  double zMin = 0.0006, zMax = 0.01;//stop
  int nDivAxis = 505;
  TString modelName=f1->GetName();
  if(modelName.Contains("T5bbbb") || modelName.Contains("T5qqqq")){ xMin = 1450; xMax = 2450; nDivAxis = 510;}
  if(modelName.Contains("T5tttt")                              )  { xMin = 1450; xMax = 2450; nDivAxis = 510;}
  if(modelName.Contains("T6tt")                                )  { xMin =  800; xMax = 1451; nDivAxis = 510; yMin = 0.; yMax = 2000.;/* zMin = 0.00005; zMax = 1.0;*/}
  if(modelName.Contains("GGM_M1M3")                            )  { xMin = 1000; xMax = 2400; nDivAxis = 510; yMin = 0.; yMax = 2100.; zMin = 0.0008; zMax = 0.02;}
  //  if(modelName.Contains("T6tt")                                )  { xMin =  800; xMax = 1451; nDivAxis = 510; yMin = 0.; yMax = 2000.; zMin = 0.001; zMax = 1.0;}
  TPaveText *decayMode = new TPaveText(0.15,0.7, 0.8,0.9,"NDC");
  decayMode->SetShadowColor(0);   decayMode->SetFillColor(0); 
  decayMode->SetLineWidth(3);    //decayMode->SetBorderColor(kBlack);
  TLegend *legText = new TLegend(0.17,0.72, 0.45,0.82);

  TH2D *h2_rMedian=(TH2D*)f1->FindObjectAny("mGlmNLSP_median");//median exp 50%
  TH2D *h2_r16pc=(TH2D*)f1->FindObjectAny("mGlmNLSP_16pc");//median exp 16%
  TH2D *h2_r84pc=(TH2D*)f1->FindObjectAny("mGlmNLSP_84pc");//meadian exp 84%
  TH2D *h2_XsecULexp=(TH2D*)f1->FindObjectAny("mGlmNLSP_XsecULexp");//exp limit

  TH2D *h2_rObs=(TH2D*)f1->FindObjectAny("mGlmNLSP_r");//obs limit
  TH2D *h2_rXsecUp=(TH2D*)f1->FindObjectAny("mGlmNLSP_r_XsecUp");//obs limit+theory xsec up
  TH2D *h2_rXsecDn=(TH2D*)f1->FindObjectAny("mGlmNLSP_r_XsecDn");//obs limit+theory xsec down
  TH2D *h2_XsecUL=(TH2D*)f1->FindObjectAny("mGlmNLSP_XsecUL");//UL on xsec based on obs limit

  cout<<"Z axis to fb? "<<xsecULinfb<<endl;
  if(xsecULinfb){
    h2_XsecUL->Scale(1000);
    h2_XsecULexp->Scale(1000);
    zMin = zMin*1000;
    zMax = zMax*1000;
  }

  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  //------------------------------------------------
  TGraph2D *gr2d_rMedian = new TGraph2D(h2_rMedian);
  TGraph2D *gr2d_16pc = new TGraph2D(h2_r16pc);
  TGraph2D *gr2d_84pc = new TGraph2D(h2_r84pc);  
  TGraph2D *gr2dXsecExp = new TGraph2D(h2_XsecULexp);
  gr2dXsecExp->SetNpx(250);  gr2dXsecExp->SetNpy(250);

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
  //  removePoint(gr1d_XsecDn,2000.,700.);
  //  removePoint(gr1d_Median,2100.,900.);
  if(cleanDiagnl && (modelName.Contains("T5bbbb") || modelName.Contains("T5qqqq"))){
    gr1d_Median = cleanupDiagonal(gr1d_Median,modelName);
    gr1d_16pc   = cleanupDiagonal(gr1d_16pc,modelName);
    gr1d_84pc   = cleanupDiagonal(gr1d_84pc,modelName);
    gr1d_Obs    = cleanupDiagonal(gr1d_Obs,modelName);
    gr1d_XsecUp = cleanupDiagonal(gr1d_XsecUp,modelName);
    gr1d_XsecDn = cleanupDiagonal(gr1d_XsecDn,modelName);
  }
  TCanvas *c3=new TCanvas("c3","c3",1300,1000);
  //c3->SetBottomMargin(0.12);
  c3->SetLeftMargin(0.15);
  c3->SetRightMargin(0.2);
  //------------------------------------------------
  //  gr1d_Median->Print("all");
  gr1d_Median->SetLineColor(kRed);
  gr1d_Median->SetLineWidth(5);gr1d_Median->SetLineStyle(7);
  gr1d_16pc->SetLineColor(kRed);
  gr1d_16pc->SetLineWidth(2);gr1d_16pc->SetLineStyle(7);
  gr1d_84pc->SetLineColor(kRed);
  gr1d_84pc->SetLineWidth(2);gr1d_84pc->SetLineStyle(7);

  gr1d_Obs->SetLineColor(kBlack); //gr1d_Obs->SetLineStyle(2);
  gr1d_Obs->SetLineWidth(5);
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
  h2_XsecUL->GetZaxis()->SetMoreLogLabels();
  if(!(modelName.Contains("T6ttZg")))  h2_XsecUL->SetTitle(";m_{#tilde{g}} (GeV);m_{#tilde{#chi}_{1}^{0}} (GeV)");
  else                     h2_XsecUL->SetTitle(";m_{ #tilde{t} } (GeV);m_{#tilde{#chi}_{1}^{0}} (GeV)");
  h2_XsecUL->GetYaxis()->SetTitleOffset(1.5);

  if(modelName.Contains("GGM_M1M3")) h2_XsecUL->SetTitle(";M3 (GeV);M1 (GeV)");

  TH2D *h2_temp=(TH2D*)h2_XsecUL->Clone("h2_temp"); // a dummy histogram with all axes settings, bin content = 0.
  for(int i=0;i<=h2_temp->GetNbinsX();i++)
    for(int j=0;j<=h2_temp->GetNbinsY();j++)
      h2_temp->SetBinContent(i,j,0); 
  if(xsecULinfb){
    h2_temp->Draw();
  }
  else h2_XsecUL->Draw();
  cout<<modelName<<endl;
  
  gr2dXsec->Draw("same COLZ"); 
  gr1d_Median->Draw("L"); 
  gr1d_16pc->Draw("L");   
  gr1d_84pc->Draw("L");
  gr1d_Obs->Draw("L");
  gr1d_XsecUp->Draw("L");
  gr1d_XsecDn->Draw("L");
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
  else if(modelName.Contains("GGM_M1M3")){
    decayMode->AddText("#bf{GGM M1M3}");
    modelName="GGM_M1M3";}
  else decayMode->AddText(modelName);
  decayMode->AddText(" ");
  decayMode->AddText(" ");
  decayMode->Draw();
  
  legText->SetTextSize(0.04);
  legText->SetBorderSize(0);
  legText->SetFillColor(0);
  legText->AddEntry(gr1d_XsecDn," ","l");
  legText->AddEntry(gr1d_Obs,"Observed #pm 1 #sigma_{theory}","l");
  legText->AddEntry(gr1d_XsecDn," ","l");

  legText->AddEntry(gr1d_16pc," ","l");
  legText->AddEntry(gr1d_Median,"Expected #pm 1 #sigma_{experiment}","l");
  legText->AddEntry(gr1d_84pc," ","l");
  legText->Draw();
  //-----------------------------------------------
  // gr2dXsec->GetZaxis()->SetTitleSize(0.05);
  // gr2dXsec->GetZaxis()->SetTitleOffset(1.25);
  gr2dXsec->SetMinimum(zMin);
  gr2dXsec->SetMaximum(zMax);
  if(xsecULinfb) gr2dXsec->GetZaxis()->SetTitle("95% CL upper limit on cross section (fb)");
  else gr2dXsec->GetZaxis()->SetTitle("95% CL upper limit on cross section (pb)");
  gr2dXsec->GetZaxis()->SetTitleOffset(1.2);
  gr2dXsecExp->SetMinimum(zMin);
  gr2dXsecExp->SetMaximum(zMax);
  if(xsecULinfb) gr2dXsecExp->GetZaxis()->SetTitle("95% CL expected upper limit on cross section (fb)");
  else gr2dXsecExp->GetZaxis()->SetTitle("95% CL expected upper limit on cross section (pb)");
  gr2dXsecExp->GetZaxis()->SetTitleOffset(1.2);
  //  gr2dXsec->GetZaxis()->SetMoreLogLabels(1);
  
 // TPaveText *pt = new TPaveText(.05,.1,.95,.8);
 //   pt->AddText("A TPaveText can contain severals line of text.");
 //  pt->Draw();
  
  c3->Update();
  fout->cd();
  // gr2dXsec->Write();

  TH2D *h2_XsecULdense = (TH2D*)gr2dXsec->GetHistogram();
  h2_XsecULdense->SetTitle(0);
  h2_XsecULdense->GetXaxis()->SetTitle(h2_XsecUL->GetXaxis()->GetTitle());
  h2_XsecULdense->GetYaxis()->SetTitle(h2_XsecUL->GetYaxis()->GetTitle());
  h2_XsecULdense->Write("obs_XsecLimit");

  TH2D *h2_XsecULexpdense = (TH2D*)gr2dXsecExp->GetHistogram();
  h2_XsecULexpdense->SetTitle(0);
  h2_XsecULexpdense->GetXaxis()->SetTitle(h2_XsecUL->GetXaxis()->GetTitle());
  h2_XsecULexpdense->GetYaxis()->SetTitle(h2_XsecUL->GetYaxis()->GetTitle());
  h2_XsecULexpdense->Write("exp_XsecLimit");

  //  h2_XsecUL->Write();
  TGraph *gr1d_Median_cp=(TGraph*)gr1d_Median->Clone("exp");  gr1d_Median_cp->Write();
  TGraph *gr1d_16pccp=(TGraph*)gr1d_16pc->Clone("exp1dn");   gr1d_16pccp->Write();
  TGraph *gr1d_84pccp=(TGraph*)gr1d_84pc->Clone("exp1up");   gr1d_84pccp->Write();
  TGraph *gr1d_Obs_cp=(TGraph*)gr1d_Obs->Clone("obs");    gr1d_Obs_cp->Write();
  TGraph *gr1d_XsecUp_cp=(TGraph*)gr1d_XsecUp->Clone("obs_XsecUp");    gr1d_XsecUp_cp->Write();
  TGraph *gr1d_XsecDn_cp=(TGraph*)gr1d_XsecDn->Clone("obs_XsecDn");    gr1d_XsecDn_cp->Write();

  char name3[100];
  textOnTop.SetTextSize(0.05);
  intLumiE.SetTextSize(0.05);
  //  textOnTop.DrawLatexNDC(0.16,0.91,"CMS #it{#bf{Preliminary}}");
  textOnTop.DrawLatexNDC(0.16,0.91,"CMS");
  sprintf(name3,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.57,0.91,name3);
  c3->SaveAs(modelName+"_exclusion.pdf");
  //  c3->SaveAs(modelName+"_exclusion.png");
}


void removePoint(TGraph *gr0,double xVal,double yVal){
  gr0->Print("all");
  double *xArr=gr0->GetX();
  double *yArr=gr0->GetY();
  int nP=gr0->GetN(),ptPos=-100;
  double dist=100000;
  for(int i=0;i<nP;i++){
    if(sqrt((xVal-xArr[i])*(yVal-yArr[i])) < dist){
      dist = sqrt((xVal-xArr[i])*(yVal-yArr[i]));
      ptPos=i;
    }
  }
  if(ptPos > 0){
    cout<<"Removing point "<<ptPos<<" x="<<xArr[ptPos]<<" y="<<yArr[ptPos]<<" from "<<gr0->GetName()<<endl;
    gr0->RemovePoint(ptPos);
  }
}

TGraph *cleanupDiagonal(TGraph* gr,TString model){
  if(!(model.Contains("T5qqqqHg") || model.Contains("T5bbbbZg"))) return gr;
  cout<<"Cleaning up diagonal for "<<gr->GetName()<<" in "<<model<<endl;
  double *xArr=gr->GetX();
  double *yArr=gr->GetY();
  vector<double> newX,newY;
  for(int i=0;i<gr->GetN();i++){
    if(xArr[i] - yArr[i] > 10){
      newX.push_back(xArr[i]);
      newY.push_back(yArr[i]);
    }
  }
  TGraph *gr2=new TGraph(newX.size(),&(newX[0]),&(newY[0]));
  return gr2;
}
