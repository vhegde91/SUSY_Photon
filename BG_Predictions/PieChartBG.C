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
//int col[11]={kTeal+9,kGreen,kYellow,kOrange,kPink+1,kMagenta+2,kCyan,kBlue,kRed,kMagenta,kBlue+2};  //Specify Colors b's
int col[11]={kPink-2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};  //Specify Colors
char name[100];
TString getLegName(TString);
void PieChartBG(){
  TFile *f=TFile::Open("SBinHists.root");
  
  vector<TString> histName={"AllSBins_v7_ZGPred","AllSBins_v7_LElePred","AllSBins_v7_LMuPred","AllSBins_v7_FRPred","AllSBins_v7_MultiJPred"};
  
  bool drawUnc=0;
  int nLastBins=1;
  int nHists=histName.size();
  TH1D *h_hist[nHists];
  TCanvas *c_cA=new TCanvas("name","name",1100,1000);
  TPad *pad[6];
  pad[0]=new TPad("LB","LB",0.001,0.0, 0.332,0.4);
  pad[1]=new TPad("MB","MB",0.333,0.0, 0.665,0.4);  
  pad[2]=new TPad("RB","RB",0.666,0.0, 1.000,0.4);
  pad[3]=new TPad("LT","LT",0.001,0.4, 0.332,0.8);
  pad[4]=new TPad("MT","MT",0.333,0.4, 0.665,0.8);
  pad[5]=new TPad("RT","RT",0.666,0.4, 1.000,0.8);

  TLegend *legend=new TLegend(0.1, 0.85,  0.9, 0.9);
  double intgrlRange1[nHists];
  double intgrlRange2[nHists];
  double intgrlRange3[nHists];
  double intgrlRange4[nHists];
  double intgrlRange5[nHists];
  double intgrlRange6[nHists];
  double err=0,temp=0;
  for(int i=0;i<nHists;i++){
    h_hist[i]=(TH1D*)f->FindObjectAny(histName[i]);
    h_hist[i]->SetLineColor(col[i]);
    h_hist[i]->SetFillColor(col[i]);
    legend->AddEntry(h_hist[i],getLegName(histName[i]),"f");
    if(drawUnc){
      intgrlRange1[i]=err; temp=h_hist[i]->IntegralAndError(6-nLastBins,6,err);
      intgrlRange2[i]=err; temp=h_hist[i]->IntegralAndError(11-nLastBins,11,err);
      intgrlRange3[i]=err; temp=h_hist[i]->IntegralAndError(16-nLastBins,16,err);
      intgrlRange4[i]=err; temp=h_hist[i]->IntegralAndError(21-nLastBins,21,err);
      intgrlRange5[i]=err; temp=h_hist[i]->IntegralAndError(26-nLastBins,26,err);
      intgrlRange6[i]=err; temp=h_hist[i]->IntegralAndError(31-nLastBins,31,err);
    }
    else{
      intgrlRange1[i]=h_hist[i]->Integral(6-nLastBins,6);
      intgrlRange2[i]=h_hist[i]->Integral(11-nLastBins,11);
      intgrlRange3[i]=h_hist[i]->Integral(16-nLastBins,16);
      intgrlRange4[i]=h_hist[i]->Integral(21-nLastBins,21);
      intgrlRange5[i]=h_hist[i]->Integral(26-nLastBins,26);
      intgrlRange6[i]=h_hist[i]->Integral(31-nLastBins,31);
    }
  }
  TPie *pie[6];
  pie[0]= new TPie("pie1","N_{2-4}^{0b}",nHists,intgrlRange1,col);
  pie[1]= new TPie("pie2","N_{5-6}^{0b}",nHists,intgrlRange2,col);
  pie[2]= new TPie("pie3","N_{#geq7}^{0b}",nHists,intgrlRange3,col);
  
  pie[3]= new TPie("pie4","N_{2-4}^{#geq1b}",nHists,intgrlRange4,col);
  pie[4]= new TPie("pie5","N_{5-6}^{#geq1b}",nHists,intgrlRange5,col);
  pie[5]= new TPie("pie6","N_{#geq7}^{#geq1b}",nHists,intgrlRange6,col);
  //  pie[0]->Draw();
  legend->SetNColumns(nHists);
  legend->SetBorderSize(0);
  legend->Draw();
  //  gStyle->SetTextSize(20);
  for(int i=0;i<6;i++){
    c_cA->cd();   
    pad[i]->Draw();
    pad[i]->cd();
    //h_hist[i]->Draw();
    //    pie[i]->SetTextSize(40);
    pie[i]->Draw();
    pie[i]->SetLabelFormat(0);
  }
 
 
  
  //    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 )

   //  c_cA->SaveAs(histName+name);

}

TString getLegName(TString fname){
  if(fname.Contains("ZGPred")){return "Z(#nu #bar{#nu})#gamma";}
  else if(fname.Contains("LEle")){return "Lost e";}
  else if(fname.Contains("LMu")){return "Lost #mu+ #tau_{had}";}
  else if(fname.Contains("FR")){return "e #rightarrow #gamma";}
  else if(fname.Contains("MultiJ") || fname.Contains("gjets")  ){return "#gamma+jets";}
  else if(fname.Contains("T5bbbbZG_1600_150")){return "T5bbbbZG_150";}
  else if(fname.Contains("T5bbbbZG_1600_1550")){return "T5bbbbZG_1550";}
  else if(fname.Contains("v7_Obs") ){return "Data";}
  else return fname;
}
