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
int col[11]={kOrange,kBlue,kTeal+9,kOrange-9,kGray+1,kCyan,kMagenta+2,kPink+1,kRed,kMagenta,kBlue+2};  //Specify Colors
char name[100];
void PieChartBG(){
TString inFname[nfiles];
  inFname[0]="ZJetsToNuNu.root";
  inFname[1]="TTGJets.root";
  inFname[2]="WJetsToLNu.root";
  inFname[3]="TTJetsHT.root";
  inFname[4]="QCD.root";
  inFname[5]="GJets.root";
  inFname[6]="WGJetsToLNuG.root";
  inFname[7]=("ZGJetsToNuNuG.root");
  // inFname[6]="HG_NLSP150_Cut" +to_string(cutnum)+".root";
  // inFname[7]="HG_NLSP1000_Cut"+to_string(cutnum)+".root";
  // inFname[8]="HG_NLSP1550_Cut"+to_string(cutnum)+".root";

  TCanvas *c_cA=new TCanvas("name","name",500,500);
  for(int i=0;i<nfiles;i++){f[i] = new TFile(inFname[i]);}
  double intgrlRange[nfiles];
  for(int i=0;i<nfiles;i++){
    TH1D *h_METvBin=(TH1D*)f[i]->FindObjectAny("METvarBin");
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
    pie3->GetSlice(i)->SetTitle(0);
  }
  pie3->SetTitle(0);
  pie3->SetLabelsOffset(-.2);
  pie3->Draw();
  sprintf(name,"pieChart.png");
  c_cA->SaveAs(name);
  //  pie3->SetLabelFormat("%perc");
}

