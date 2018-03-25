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

void plot_SBin_v3(TString fName,TString histName){
  TLatex textOnTop,intLumiE;
  double intLumi = 35.9;
  //  TString xName = "#Delta#Phi(#gamma,MET)";
  TString xName = "Bin number";
  TString yName = "Events";
  TString legName = "Single e + #gamma data CS";
  gStyle->SetOptStat(0);
  TString name = fName,name1 = fName;
  name = name.ReplaceAll(".root","")+yName+"_Vs_"+xName;
  name1 = name1.ReplaceAll(".root","");

  cout<<name<<endl;

  TLegend *leg = new TLegend(0.5,0.8,0.88,0.88);
  TCanvas *c1 = new TCanvas("c1",name,1500,800);
  TFile *f1 = new TFile(fName);
  TH1D *h1 = (TH1D*)f1->Get(histName);
  TH2D *h2 = (TH2D*)f1->Get(histName);
  if(h1){
    h1->GetXaxis()->SetTitle(xName);
    h1->GetYaxis()->SetTitle(yName);
    h1->GetYaxis()->SetRangeUser(0.5,200);
    h1->SetLineColor(kBlack);
    h1->SetMarkerStyle(20);
    h1->SetFillColor(kGreen);
    h1->SetTitle(0);
    h1->Draw("BAR e1");
    leg->AddEntry(h1,legName,"lepf");
    c1->SetLogy();
    leg->SetBorderSize(0);
    leg->Draw();
  }
  else if(h2){
    h2->GetYaxis()->SetRangeUser(0.5,1.5);
    //    h2->SetTitle(fName.ReplaceAll(".root",""));
    h2->GetXaxis()->SetTitle(xName);
    h2->GetYaxis()->SetTitle(yName);
    h2->GetYaxis()->SetTitleOffset(0.9);
    h2->Draw("colz");
    c1->SetLogz();
  }
  if(!h1 && !h2) {cout<<"Hist Not found"<<endl;}

  TLine *line1V7=new TLine( 7.5,0.5,  7.5,200);
  TLine *line2V7=new TLine(14.5,0.5, 14.5,200);
  TLine *line3V7=new TLine(21.5,0.5, 21.5,200);
  TLine *line4V7=new TLine(28.5,0.5, 28.5,70);
  TLine *line5V7=new TLine(35.5,0.5, 35.5,70);
  TLine *line6V7=new TLine(42.5,0.5, 42.5,70);
  TLine *line7V7=new TLine(49.5,0.5, 49.5,70);
  TLine *line8V7=new TLine(56.5,0.5, 56.5,70);

  c1->cd(); c1->SetGridx(0); c1->SetGridy(0);
  line1V7->Draw();      line2V7->Draw();  line3V7->Draw();
  line4V7->Draw();      line5V7->Draw();  line6V7->Draw();
  line7V7->Draw();      line8V7->Draw();
  TArrow *arrow1 = new TArrow(0.5,110,7.5,110,0.01,"<|>");
  TArrow *arrow2 = new TArrow(7.5,110,14.5,110,0.01,"<|>");
  TArrow *arrow3 = new TArrow(14.5,110,21.5,110,0.01,"<|>");
  TArrow *arrow4 = new TArrow(21.5,40,28.5,40,0.01,"<|>");
  TArrow *arrow5 = new TArrow(28.5,40,35.5,40,0.01,"<|>");
  TArrow *arrow6 = new TArrow(35.5,40,42.5,40,0.01,"<|>");
  TArrow *arrow7 = new TArrow(42.5,12,49.5,12,0.01,"<|>");
  TArrow *arrow8 = new TArrow(49.5,12,56.5,12,0.01,"<|>");
  TArrow *arrow9 = new TArrow(56.5,12,63.5,12,0.01,"<|>");


  arrow1->Draw(); arrow2->Draw(); arrow3->Draw();
  arrow4->Draw(); arrow5->Draw(); arrow6->Draw();
  arrow7->Draw(); arrow8->Draw(); arrow9->Draw();
  TLatex Tl;
  Tl.SetTextSize(0.04);
  Tl.DrawLatex(3.,130,"N^{0}_{2-4}");
  Tl.DrawLatex(10,130,"N^{0}_{5-6}");
  Tl.DrawLatex(17,130,"N^{0}_{#geq7}");
  Tl.DrawLatex(24,45,"N^{1}_{2-4}");
  Tl.DrawLatex(31,45,"N^{1}_{5-6}");
  Tl.DrawLatex(38,45,"N^{1}_{#geq7}");
  Tl.DrawLatex(45,15,"N^{#geq2}_{2-4}");
  Tl.DrawLatex(52,15,"N^{#geq2}_{5-6}");
  Tl.DrawLatex(59,15,"N^{#geq2}_{#geq7}"); 

  char name3[100];
  textOnTop.SetTextSize(0.04);
  intLumiE.SetTextSize(0.04);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
  sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.72,0.91,name3);

  //  c1->SaveAs(name1+"_RpTvsdPhiG.png");

}
