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

void signalContamination(int,int,TString);
void signalContamination(TString);
TLatex textOnTop,intLumiE;
double intLumi=35.9;
//int col[11]={kBlack,kMagenta+2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kPink+1,kBlue,kRed,kMagenta};
void signalContamination(TString fName){

  TString xName = "Bin number";

  double ymin = 0.0002,ymax=9;
  bool totBGHist=0; 

  // TString yName = "#frac{(1 #mu+#gamma Signal)TF}{Total BG}";
  // TString histName = "AllSBins_v7_TotalBG";
  // TString legName2 = "Total BG";   totBGHist=1; ymin = 0.002,ymax=20000;

  TString yName = "#frac{(1 #mu+#gamma Signal)TF}{#gamma Signal}";
  TString histName = "AllSBins_v7_Mu0";
  TString legName2 = "#gamma Signal";

  TString legName1 = "(1 #mu+#gamma Signal)TF";
  
  int lineColor = kCyan;
  gStyle->SetOptStat(0);
  TLegend *leg = new TLegend(0.35, 0.89,  0.65, 0.76);

  TString name = fName,name1 = fName;
  //  name = name.ReplaceAll(".root","")+yName+"_Vs_"+xName;
  name1 = name1.ReplaceAll(".root","");

  TCanvas *c1 = new TCanvas("c1",name1,1500,800);
  TPad *p_top=new TPad(name1+"_top",name1+"_top",0,0.5, 1,1.0);
  TPad *p_bot=new TPad(name1+"_bot",name1+"_bot",0,0.0, 1,0.5);
  p_top->SetRightMargin(0.05);
  p_bot->SetRightMargin(0.05);
  // p_top->SetLeftMargin(0.15);
  // p_bot->SetLeftMargin(0.15);

  p_top->Draw();p_top->SetGridy();p_top->SetLogy();
  p_top->SetBottomMargin(0);
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.3);
  p_bot->Draw();p_bot->SetGridy();
  p_top->SetGridx(0);p_bot->SetGridx(0);
  //  c1->SetLogy();

  TFile *f1 = new TFile(fName);
  TFile *f2 = new TFile("SBinHists.root");

  TH1D *h1 = (TH1D*)f1->Get("AllSBins_v7_Mu1");
  TH1D *hTF = (TH1D*)f2->Get("LMuTFs_Final");//LMuTauTFs ,LMuTFs_Final
  TH1D *h2;
  if(histName=="AllSBins_v7_TotalBG") h2 = (TH1D*)f2->Get(histName);
  else h2 = (TH1D*)f1->Get(histName);

  h1->Multiply(hTF);

  TH1D *hRatio = (TH1D*)h1->Clone("ratio0mu1mu");
  hRatio->Divide(h2);
  //  hRatio->Smooth(1000);

  p_top->cd();
  h2->GetXaxis()->SetTitle(xName);
  h2->GetYaxis()->SetTitle("Events");
  h2->GetYaxis()->SetTitleSize(0.10);
  h2->GetYaxis()->SetLabelSize(0.10);
  h2->GetYaxis()->SetTitleOffset(0.5);
  h2->GetYaxis()->SetNdivisions(5);
  h2->SetTitle(0);
  //  if(totBGHist)
  h2->GetYaxis()->SetRangeUser(ymin,10*h2->GetMaximum());
  //    h1->Draw("histe");
  h1->SetLineColor(kBlue);
  h2->SetLineColor(kRed);
  h2->Draw("e1");
  h1->Draw("e1 same");

  p_bot->cd();
  hRatio->SetLineColor(kBlack);
  hRatio->SetTitle(";Bin number;");
  hRatio->GetXaxis()->SetLabelSize(0.10);
  hRatio->GetXaxis()->SetTitleSize(0.10);
  hRatio->GetYaxis()->SetLabelSize(0.10);
  hRatio->GetYaxis()->SetTitleSize(0.10);
  hRatio->GetYaxis()->SetTitleOffset(0.54);
  hRatio->GetYaxis()->SetTitle(yName);

  if(totBGHist){
    hRatio->GetYaxis()->SetRangeUser(5e-5,2);
    p_bot->SetLogy();
  }
  else{
    hRatio->GetYaxis()->SetRangeUser(0.0001,0.55);
  }

  hRatio->Draw("e0");
  hRatio->Draw("e1 same");

  TLine *line1V7=new TLine( 6.5,0.00005,  6.5,h2->GetMaximum());
  TLine *line2V7=new TLine(11.5,0.00005, 11.5,h2->GetMaximum());
  TLine *line3V7=new TLine(16.5,0.00005, 16.5,h2->GetMaximum());
  TLine *line4V7=new TLine(21.5,0.00005, 21.5,h2->GetMaximum());
  TLine *line5V7=new TLine(26.5,0.00005, 26.5,h2->GetMaximum());
  p_top->cd(); p_top->SetGridx(0);
  line1V7->Draw();      line2V7->Draw();        line3V7->Draw();        line4V7->Draw();        line5V7->Draw();
  p_bot->cd(); p_bot->SetGridx(0);
  line1V7->Draw();      line2V7->Draw();        line3V7->Draw();        line4V7->Draw();        line5V7->Draw();

  p_top->cd();
  leg->AddEntry(h1,legName1,"lpe");
  leg->AddEntry(h2,legName2,"lpe");
  // leg->SetBorderSize(0);
  leg->SetNColumns(2);
  leg->Draw();

  //  leg->Draw();
  p_top->cd();
  char name2[100];
  textOnTop.SetTextSize(0.08);
  intLumiE.SetTextSize(0.08);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.8,0.91,name2);

  cout<<name1<<endl;
  TFile *fout=new TFile("signalContLostMu_T5ttttZg_1800_150.root","recreate");
  hRatio->Write();
  // if(totBGHist) c1->SaveAs(name1+"_SignalCont_TotBG.pdf");
  // else c1->SaveAs(name1+"_SignalCont_SR.pdf");

}

void signalContamination(int mGl, int mNLSP, TString fName){
  TString name;
  TFile *f1 = TFile::Open(fName);
  TH2D *h2_Mu0,*h2_Mu1;
  TString foutName;
  if(fName.Contains("T5ttttZg"))      foutName = "T5ttttZg_LostMu_"+to_string(mGl)+"_"+to_string(mNLSP)+"_.root";
  else if(fName.Contains("T5bbbbZg")) foutName = "T5bbbbZg_LostMu_"+to_string(mGl)+"_"+to_string(mNLSP)+"_.root";
  else if(fName.Contains("T5qqqqHg")) foutName = "T5qqqqHg_LostMu_"+to_string(mGl)+"_"+to_string(mNLSP)+"_.root";
  else if(fName.Contains("T6ttZg"))   foutName = "T6ttZg_LostMu_"+to_string(mGl)+"_"+to_string(mNLSP)+"_.root";
  else if(fName.Contains("GGM_M1M3"))   foutName = "GGM_M1M3_LostMu_"+to_string(mGl)+"_"+to_string(mNLSP)+"_.root";

  TFile *fout = new TFile(foutName,"recreate");

  TH1D *h_SBins_v7_Mu0 = new TH1D("AllSBins_v7_Mu0","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] for 0 Mu events",31,0.5,31.5);
  TH1D *h_SBins_v7_Mu1 = new TH1D("AllSBins_v7_Mu1","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] for 1 Mu events",31,0.5,31.5);

  for(int i=1;i<=h_SBins_v7_Mu0->GetNbinsX();i++){
    double entr=0., errEntr = 0.;
    //-------------- Mu0 ------------------
    name = "mGlmNLSP_Mu0_bin_"+to_string(i);
    h2_Mu0=(TH2D*)f1->Get(name);
    entr    = h2_Mu0->GetBinContent(h2_Mu0->GetXaxis()->FindBin(mGl),h2_Mu0->GetYaxis()->FindBin(mNLSP));
    errEntr = h2_Mu0->GetBinError  (h2_Mu0->GetXaxis()->FindBin(mGl),h2_Mu0->GetYaxis()->FindBin(mNLSP));
    // if(i==8) cout<<entr<<" "<<errEntr<<endl;
    // entr   += h2_Mu0->GetBinContent(h2_Mu0->GetXaxis()->FindBin(mGl-50),h2_Mu0->GetYaxis()->FindBin(mNLSP));
    // errEntr = sqrt(errEntr*errEntr + (h2_Mu0->GetBinError  (h2_Mu0->GetXaxis()->FindBin(mGl-50),h2_Mu0->GetYaxis()->FindBin(mNLSP)))*(h2_Mu0->GetBinError  (h2_Mu0->GetXaxis()->FindBin(mGl-50),h2_Mu0->GetYaxis()->FindBin(mNLSP))));
    // if(i==8) cout<<entr<<" "<<errEntr<<endl;
    // entr   += h2_Mu0->GetBinContent(h2_Mu0->GetXaxis()->FindBin(mGl+50),h2_Mu0->GetYaxis()->FindBin(mNLSP));
    // errEntr = sqrt(errEntr*errEntr + (h2_Mu0->GetBinError  (h2_Mu0->GetXaxis()->FindBin(mGl+50),h2_Mu0->GetYaxis()->FindBin(mNLSP)))*(h2_Mu0->GetBinError  (h2_Mu0->GetXaxis()->FindBin(mGl+50),h2_Mu0->GetYaxis()->FindBin(mNLSP))));
    // if(i==8) cout<<entr<<" "<<errEntr<<endl;
    h_SBins_v7_Mu0->SetBinContent(i,entr);
    h_SBins_v7_Mu0->SetBinError  (i,errEntr);
    //-------------- Mu1 ------------------
    name = "mGlmNLSP_Mu1_bin_"+to_string(i);
    h2_Mu1=(TH2D*)f1->Get(name);
    entr    = h2_Mu1->GetBinContent(h2_Mu1->GetXaxis()->FindBin(mGl),h2_Mu1->GetYaxis()->FindBin(mNLSP));
    errEntr = h2_Mu1->GetBinError  (h2_Mu1->GetXaxis()->FindBin(mGl),h2_Mu1->GetYaxis()->FindBin(mNLSP));
    // if(i==8) cout<<entr<<" "<<errEntr<<endl;
    // entr   += h2_Mu1->GetBinContent(h2_Mu1->GetXaxis()->FindBin(mGl-50),h2_Mu1->GetYaxis()->FindBin(mNLSP));
    // errEntr = sqrt(errEntr*errEntr + (h2_Mu1->GetBinError  (h2_Mu1->GetXaxis()->FindBin(mGl-50),h2_Mu1->GetYaxis()->FindBin(mNLSP)))*(h2_Mu1->GetBinError  (h2_Mu1->GetXaxis()->FindBin(mGl-50),h2_Mu1->GetYaxis()->FindBin(mNLSP))));
    // if(i==8) cout<<entr<<" "<<errEntr<<endl;
    // entr   += h2_Mu1->GetBinContent(h2_Mu1->GetXaxis()->FindBin(mGl+50),h2_Mu1->GetYaxis()->FindBin(mNLSP));
    // errEntr = sqrt(errEntr*errEntr + (h2_Mu1->GetBinError  (h2_Mu1->GetXaxis()->FindBin(mGl+50),h2_Mu1->GetYaxis()->FindBin(mNLSP)))*(h2_Mu1->GetBinError  (h2_Mu1->GetXaxis()->FindBin(mGl+50),h2_Mu1->GetYaxis()->FindBin(mNLSP))));
    // if(i==8) cout<<entr<<" "<<errEntr<<endl;
    h_SBins_v7_Mu1->SetBinContent(i,entr);
    h_SBins_v7_Mu1->SetBinError  (i,errEntr);
  }
  double err=0;
  cout<<"SR events:     "<<h_SBins_v7_Mu0->IntegralAndError(1,h_SBins_v7_Mu0->GetNbinsX(),err)<<" +/- "<<err<<endl;
  cout<<"pho+mu events: "<<h_SBins_v7_Mu1->IntegralAndError(1,h_SBins_v7_Mu1->GetNbinsX(),err)<<" +/- "<<err<<endl;
  
  fout->cd();
  h_SBins_v7_Mu0->Write();
  h_SBins_v7_Mu1->Write();
  fout->Close();
  signalContamination(foutName);
}
