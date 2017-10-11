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
int col[10]={kRed,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};
vector<TString> fNames,histName,legName;
vector<int> rebin;
TString xAxisName="#";
TString yAxisName="Events";
bool saveHist=0;

void makeRatioPlot();
void plotManyHists(){
  //-------------------------------------
  gStyle->SetOptStat(0);

  fNames.push_back("HLR_gjets_qcd.root"); 
  histName.push_back("HLratio_1D");
  legName.push_back("BestPho");
  rebin.push_back(1);
  //-------------------------------------
  // fNames.push_back("CS_WJets_FR_NoTrg_v2.root");
  // histName.push_back("Qmulti_Ele");
  // legName.push_back("W+Jets");
  // rebin.push_back(1);
  //-------------------------------------
  fNames.push_back("HLR_GJetsQCD_idFail.root");
  histName.push_back("HLratio_1D");
  legName.push_back("NotGoodPho");
  rebin.push_back(1);
  //-------------------------------------
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
  lg1=new TLegend(0.7, 0.90,  0.90, 0.65);
  for(int i=0;i<nfiles;i++){
    f[i] = new TFile(fNames[i]);
    h1[i] = (TH1D*)f[i]->FindObjectAny(histName[i]);
    h1[i]->Rebin(rebin[i]);
    h1[i]->SetTitle(0);
    h1[i]->SetLineWidth(2);
    h1[i]->SetLineColor(col[i]);
    lg1->AddEntry(h1[i],legName[i],"lep");
    if(i==0){
      h1[i]->GetXaxis()->SetTitle(xAxisName);
      h1[i]->GetXaxis()->SetTitleSize(0.06);
      h1[i]->GetXaxis()->SetLabelSize(0.06);
      h1[i]->GetYaxis()->SetTitle(yAxisName);
      h1[i]->GetYaxis()->SetTitleSize(0.06);
      h1[i]->GetYaxis()->SetLabelSize(0.06);
      h1[i]->Draw("histe");
    }
    else h1[i]->Draw("sames histe");
  }
  lg1->Draw();
  if(nfiles==2) makeRatioPlot();
  // c1->SaveAs("a.png");
}

void makeRatioPlot(){
  TCanvas *c2=new TCanvas("canvasB",yAxisName+" vs "+xAxisName,1500,850);
  c2->SetLeftMargin(0.11);
  c2->SetBottomMargin(0.13);
  lg1=new TLegend(0.7, 0.90,  0.90, 0.65);
  TPad *p_top=new TPad("CB_top","CB_top",0,0.4,1,1);
  TPad *p_bot=new TPad("CB_bot","CB_bot",0,0.0,1,0.4);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  // p_top->SetBottomMargin(0);
  // p_bot->SetTopMargin(0);
  // p_bot->SetBottomMargin(0.3);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();

  p_top->cd();
  p_top->SetTickx();p_top->SetTicky();
  h1[0]->Draw("histe");
  h1[1]->Draw("sames histe");
  lg1->AddEntry(h1[0],legName[0],"lep");
  lg1->AddEntry(h1[1],legName[1],"lep");
  lg1->Draw();

  p_bot->cd();
  TH1D *h1_cp=(TH1D*)h1[0]->Clone("h1cp");
  h1_cp->Divide(h1[1]);
  h1_cp->Draw();

  TString ratioYName = legName[0]+"/"+legName[1];
  h1_cp->SetTitle(ratioYName);
  TFile *fout;
  if(saveHist){
    fout = new TFile("a_Sbin.root","recreate");
    fout->cd();
    h1_cp->Write();
  }
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
