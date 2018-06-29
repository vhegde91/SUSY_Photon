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
void PieChartBG(){
  TFile *f[nfiles];
  f[0] = new TFile("WGJetsToLNuG.root");
  f[1] = new TFile("TTGJets.root");
  f[2] = new TFile("TTJetsHT.root");
  f[3] = new TFile("WJetsToLNu.root");
  f[4] = new TFile("ZJetsToNuNu.root");
  f[5] = new TFile("ZGJetsToNuNuG.root");
  f[6] = new TFile("GJets.root");
  f[7] = new TFile("QCD.root");

  TString histName="MET_R2";
  TCanvas *c_cA=new TCanvas("name","name",500,500);
  
  double intgrlRange[nfiles];
  for(int i=0;i<nfiles;i++){
    TH1D *h_METvBin=(TH1D*)f[i]->FindObjectAny(histName);
    int minMETBin=1;
    for(int j=1;j<=h_METvBin->GetNbinsX();j++){
      if(h_METvBin->GetBinLowEdge(j)-350>0){minMETBin=j;break;}
    }
    intgrlRange[i]=h_METvBin->Integral(minMETBin-1,h_METvBin->GetNbinsX()+1);
    cout<<f[i]->GetName()<<"\t"<<h_METvBin->Integral(minMETBin-1,h_METvBin->GetNbinsX()+1)<<" "<<minMETBin<<endl;
    
  }
  TPie *pie3 = new TPie("pie3","Pie with tangential labels",nfiles,intgrlRange,col);
  for(int i=0;i<nfiles;i++){
    string name2=f[i]->GetName();for(int j=0;j<=5;j++) name2.pop_back();
    sprintf(name,"%s",name2.c_str());
    //pie3->GetSlice(i)->SetTitle(name);
    //pie3->GetSlice(i)->SetTitle(0);
  }
  pie3->SetTitle(0);
  pie3->SetLabelsOffset(-.2);
  pie3->SetLabelFormat("%val(%perc)");
  pie3->Draw("rs");
  sprintf(name,"pieChart.png");

  c_cA->SaveAs(histName+name);

}

