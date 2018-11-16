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
#include"TLegend.h"
#include"TLatex.h"
#include"TCanvas.h"
#include"THStack.h"
#include"TStyle.h"

const int nfiles=2;
TFile *f[nfiles];
TH1D *h1[nfiles];
TLegend *lg1;
int col[11]={kRed,kAzure,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};
vector<TString> fNames,histName,legName;
vector<int> rebin;
TString xAxisName="pT^{Gen #gamma} (GeV)";
//TString xAxisName="(#vec#tilde{G_{1}}+#vec#tilde{G_{2}}).pT (GeV)";
//TString xAxisName="GenMET (GeV)";
//TString xAxisName="GenHT (GeV)";
//TString xAxisName="GenJets";
//TString xAxisName="pT^{#tilde{g}} (GeV)";
//TString xAxisName="pT^{NLSP} (GeV)";
TString yAxisName="";
TLatex textOnTop,intLumiE;
double intLumi=35.9;

void makeRatioPlot();
void plotManyHists(TString name){
  //-------------------------------------
  gStyle->SetOptStat(0);

  fNames.push_back("t5bbbbZg_1800_150.root"); 
  histName.push_back(name);
  legName.push_back("CMS");
  rebin.push_back(2);
  //-------------------------------------
  //  fNames.push_back("/home/vinay/work/Physics/SummerProj18/DelphesAnalyzer/delphes_t5bbbbZg_1800_150.root");
  fNames.push_back("/home/vinay/work/Physics/SummerProj18/DelphesAnalyzer/b.root");
  histName.push_back(name);
  legName.push_back("Private");
  rebin.push_back(2);
  //-------------------------------------
  // fNames.push_back("/home/vinay/work/Physics/SummerProj18/DelphesAnalyzer/delphes_t5bbbbZg_1800_150.root");
  // histName.push_back("GenphoPt");
  // legName.push_back("NLSP_150");
  // rebin.push_back(5);
  //-------------------------------------
  // fNames.push_back("");
  // histName.push_back("");
  // legName.push_back("");
  // rebin.push_back(1);
  //-------------------------------------
  if(fNames.size()!=nfiles){cout<<"check no. of files and filenames"<<endl; return;}

  TCanvas *c1=new TCanvas("canvasA",yAxisName+" vs "+xAxisName,1500,850);
  c1->SetLeftMargin(0.11);
  c1->SetBottomMargin(0.13);
  lg1=new TLegend(0.65, 0.88,  0.83, 0.65);
  for(int i=0;i<nfiles;i++){
    f[i] = new TFile(fNames[i]);
    h1[i] = (TH1D*)f[i]->FindObjectAny(histName[i]);
    h1[i]->Rebin(rebin[i]);
    h1[i]->Scale(1.0/h1[i]->Integral());
    h1[i]->SetTitle(0);
    h1[i]->SetLineWidth(2);
    h1[i]->SetLineColor(col[i]);
    lg1->AddEntry(h1[i],legName[i],"lep");
    if(i==0){
      h1[i]->GetXaxis()->SetTitle(xAxisName);
      h1[i]->GetXaxis()->SetTitleSize(0.05);
      h1[i]->GetXaxis()->SetLabelSize(0.05);
      h1[i]->GetYaxis()->SetTitle(yAxisName);
      h1[i]->GetYaxis()->SetTitleSize(0.05);
      h1[i]->GetYaxis()->SetLabelSize(0.05);
      h1[i]->Draw("histe");
    }
    else h1[i]->Draw("sames histe");
  }
  
  //  lg1->Draw();
  char name2[100];
  textOnTop.SetTextSize(0.05);
  intLumiE.SetTextSize(0.05);
  //  textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Simulation}}");
  // intLumiE.SetTextSize(0.05);
  // sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  // intLumiE.DrawLatexNDC(0.7,0.91,name2);
  
  if(nfiles==2) makeRatioPlot();
  // c1->SaveAs("a.png");
}

void makeRatioPlot(){
  TString name = legName[0]+"Vs"+legName[1]+"_"+histName[0];
  TCanvas *c2=new TCanvas(name,yAxisName+" vs "+xAxisName,1500,850);
  c2->SetLeftMargin(0.11);
  c2->SetBottomMargin(0.13);
  TLegend *lg2=new TLegend(0.7, 0.90,  0.90, 0.65);
  lg2->AddEntry(h1[0],legName[0],"lep");
  lg2->AddEntry(h1[1],legName[1],"lep");

  TPad *p_top=new TPad("CB_top","CB_top",0,0.45,1,1);
  TPad *p_bot=new TPad("CB_bot","CB_bot",0,0.05,1,0.45);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  p_top->SetBottomMargin(0.01);
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.3);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();

  p_top->cd();
  p_top->SetTickx();p_top->SetTicky();
  
  h1[0]->GetYaxis()->SetLabelSize(0.08);
  h1[0]->GetYaxis()->SetTitleSize(0.08);

  h1[0]->Draw("histe");
  h1[1]->Draw("sames histe");
  lg2->Draw();

  p_bot->cd();
  TH1D *h1_cp=(TH1D*)h1[0]->Clone("h1cp");
  h1_cp->Divide(h1[1]);
  h1_cp->GetXaxis()->SetLabelSize(0.1);
  h1_cp->GetXaxis()->SetTitleSize(0.12);
  h1_cp->GetXaxis()->SetTitle(xAxisName);
  h1_cp->GetYaxis()->SetLabelSize(0.1);
  h1_cp->Draw();
  h1_cp->GetYaxis()->SetRangeUser(0.001,1.99);
  h1_cp->GetYaxis()->SetNdivisions(505);
  h1_cp->GetYaxis()->SetTitle(legName[0]+"/"+legName[1]);
  h1_cp->GetYaxis()->SetTitleSize(0.11);
  h1_cp->GetYaxis()->SetTitleOffset(0.35);

  //  for(int i=0;i<nfiles;i++){
    // f[i] = new TFile(fNames[i]);
    // h1[i] = (TH1D*)f[i]->FindObjectAny(histName[i]);
    // h1[i]->Rebin(rebin[i]);
    // h1[i]->SetTitle(0);
    // h1[i]->SetLineWidth(2);
    // h1[i]->SetLineColor(col[i]);
    // lg1->AddEntry(h1[i],legName[i],"lep");
    //if(i==0){
      // h1[i]->GetXaxis()->SetTitle(xAxisName);
      // h1[i]->GetXaxis()->SetTitleSize(0.06);
      // h1[i]->GetXaxis()->SetLabelSize(0.06);
      // h1[i]->GetYaxis()->SetTitle(yAxisName);
      // h1[i]->GetYaxis()->SetTitleSize(0.06);
      // h1[i]->GetYaxis()->SetLabelSize(0.06);
      //h1[i]->Draw("histe");
  //   }
  //   else h1[i]->Draw("sames histe");
  // }
  
}
