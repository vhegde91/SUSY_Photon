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
#include"TPie.h"
const int nfiles=8,nBG=8;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kOrange,kBlue,kTeal+9,kOrange-9,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta};  //Specify Colors
//int col[11]={kOrange,kBlue,kTeal+9,kOrange-9,kGray+1,kCyan,kMagenta+2,kPink+1,kRed,kMagenta,kBlue+2};  //Specify Colors
//int col[11]={kPink+1,kTeal+9,kOrange,kYellow,kBlue,kCyan,kGreen,kMagenta+2,kRed,kMagenta,kBlue+2};  //Specify Colors
int col[11]={kTeal+9,kGreen,kYellow,kOrange,kPink+1,kMagenta+2,kCyan,kBlue,kRed,kMagenta,kBlue+2};  //Specify Colors b's
char name[100];
void PieChartBG_SB(){
  TFile *f[nfiles];
  f[0] = new TFile("WGJetsToLNuG.root");
  f[1] = new TFile("TTGJets.root");
  f[2] = new TFile("TTJetsHT.root");
  f[3] = new TFile("WJetsToLNu.root");
  f[4] = new TFile("ZJetsToNuNu.root");
  f[5] = new TFile("ZGJetsToNuNuG.root");
  f[6] = new TFile("GJets.root");
  f[7] = new TFile("QCD.root");

  TString histName="AllSBins_v4";
  TCanvas *c_cA=new TCanvas("name","name",1200,1800);
  c_cA->Divide(2,3);
  const int nPieCharts=6;
  int loc[6]={5,3,1,6,4,2};
  TPie *pie3[nPieCharts];
  double intgrlRange[nfiles];
  gStyle->SetTitle(0);

  for(int r=1;r<=nPieCharts;r++){
    for(int i=0;i<nfiles;i++){
      TH1D *h_METvBin=(TH1D*)f[i]->FindObjectAny(histName);
      intgrlRange[i]=h_METvBin->Integral(r*7-1,r*7);//get intregral from last 2 bins in region r
      cout<<h_METvBin->GetTitle()<<" "<<f[i]->GetName()<<"\t"<<h_METvBin->Integral(r*7-1,r*7)<<endl;
      c_cA->cd(loc[r-1]);
      sprintf(name,"pie_%i",i);
      pie3[r] = new TPie(name,"",nfiles,intgrlRange,col);
      pie3[r]->SetLabelFormat(0);
      pie3[r]->Draw("nol");
    }
  }
  sprintf(name,"pieChart.png");

  c_cA->SaveAs(histName+name);

}

