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
#include<fstream>
#include<sstream>
#include"TGraphErrors.h"
#include"TGraph.h"
#include"TLegend.h"
#include"TLatex.h"

using namespace std;

void printToTable(TString fName){

  //high dPhi LL
  // TFile *f=new TFile(fName);
  // TH1D *h1=(TH1D*)f->Get("AllSBins_v7_LEleCS");
  // TH1D *h2=(TH1D*)f->Get("LEleTFs_Final");
  // TH1D *h3=(TH1D*)f->Get("AllSBins_v7_LElePred");

  // TH1D *h4=(TH1D*)f->Get("AllSBins_v7_LMuCS");
  // TH1D *h5=(TH1D*)f->Get("LMuTFs_Final");
  // TH1D *h6=(TH1D*)f->Get("AllSBins_v7_LMuPred");

  // for(int i=1;i<=h1->GetNbinsX();i++){
  //   //    cout<<i<<" & "<<h1->GetBinContent(i)<<" & "<<setprecision(3)<<h2->GetBinContent(i)<<" $pm$ "<<setprecision(2)<<h2->GetBinError(i)<<"   & "<<setprecision(3)<<h3->GetBinContent(i)<<" $pm$ "<<h3->GetBinError(i)<<" & "<<endl;
  //   printf("%d & %.0f  & %.3f  $\\pm$  %.3f  & %.1f $\\pm$ %.2f",i,h1->GetBinContent(i),h2->GetBinContent(i),h2->GetBinError(i),h3->GetBinContent(i),h3->GetBinError(i));
  //   printf(" & %.0f  & %.3f  $\\pm$  %.3f  & %.1f $\\pm$ %.2f    \\hline",h4->GetBinContent(i),h5->GetBinContent(i),h5->GetBinError(i),h6->GetBinContent(i),h6->GetBinError(i));
  //   cout<<endl;
  // }
  //low dPhi LL
  // TFile *f=new TFile(fName);
  // TH1D *h1=(TH1D*)f->Get("AllSBins_v7_LEleCS_LDP");
  // TH1D *h2=(TH1D*)f->Get("LEleTFs_Final_LDP");
  // TH1D *h3=(TH1D*)f->Get("AllSBins_v7_LElePred_LDP");

  // TH1D *h4=(TH1D*)f->Get("AllSBins_v7_LMuCS_LDP");
  // TH1D *h5=(TH1D*)f->Get("LMuTFs_Final_LDP");
  // TH1D *h6=(TH1D*)f->Get("AllSBins_v7_LMuPred_LDP");

  // for(int i=1;i<=h1->GetNbinsX();i++){
  //   //    cout<<i<<" & "<<h1->GetBinContent(i)<<" & "<<setprecision(3)<<h2->GetBinContent(i)<<" $pm$ "<<setprecision(2)<<h2->GetBinError(i)<<"   & "<<setprecision(3)<<h3->GetBinContent(i)<<" $pm$ "<<h3->GetBinError(i)<<" & "<<endl;
  //   printf("%d & %.0f  & %.3f  $\\pm$  %.3f  & %.1f $\\pm$ %.2f",i,h1->GetBinContent(i),h2->GetBinContent(i),h2->GetBinError(i),h3->GetBinContent(i),h3->GetBinError(i));
  //   printf(" & %.0f  & %.3f  $\\pm$  %.3f  & %.1f $\\pm$ %.2f    \\hline",h4->GetBinContent(i),h5->GetBinContent(i),h5->GetBinError(i),h6->GetBinContent(i),h6->GetBinError(i));
  //   cout<<endl;
  // }
  //  fakerate high dPhi
  // TFile *f=new TFile(fName);
  // TH1D *h1=(TH1D*)f->Get("AllSBins_v7_FRCS");
  // TH1D *h2=(TH1D*)f->Get("FRs_Final");
  // TH1D *h3=(TH1D*)f->Get("AllSBins_v7_FRPred");
  // for(int i=1;i<=h1->GetNbinsX();i++){
  //   printf("%d & %.0f  & %.4f  $\\pm$  %.4f  & %.2f $\\pm$ %.2f    \\hline",i,h1->GetBinContent(i),h2->GetBinContent(i),h2->GetBinError(i),h3->GetBinContent(i),h3->GetBinError(i));
  //   cout<<endl;
  // }

  //fakerate low dPhi
  // TFile *f=new TFile(fName);
  // TH1D *h1=(TH1D*)f->Get("AllSBins_v7_FRCS_LDP");
  // TH1D *h2=(TH1D*)f->Get("FRs_Final_LDP");
  // TH1D *h3=(TH1D*)f->Get("AllSBins_v7_FRPred_LDP");
  // for(int i=1;i<=h1->GetNbinsX();i++){
  //   printf("%d & %.0f  & %.4f  $\\pm$  %.4f  & %.2f $\\pm$ %.2f    \\hline",i,h1->GetBinContent(i),h2->GetBinContent(i),h2->GetBinError(i),h3->GetBinContent(i),h3->GetBinError(i));
  //   cout<<endl;
  // }
  
  //ZG high dPhi
  TFile *f=new TFile(fName);
  TH1D *h1=(TH1D*)f->Get("AllSBins_v7_ZGCS_LDP_StatUncOnly");
  TH1D *h2=(TH1D*)f->Get("AllSBins_v7_ZGPred_LDP");
  for(int i=1;i<=h1->GetNbinsX();i++){
    printf("%d & %.2f  $\\pm$  %.2f  & %.2f $\\pm$ %.2f    \\\\hline",i,h1->GetBinContent(i),h1->GetBinError(i),h2->GetBinContent(i),h2->GetBinError(i));
    cout<<endl;
  }

  //Multijet
  // TFile *f=new TFile(fName);
  // TH1D *h1=(TH1D*)f->Get("AllSBins_v7_rawMultiJCS");
  // TH1D *h2=(TH1D*)f->Get("AllSBins_v7_EWSum_LDP");
  // TH1D *h3=(TH1D*)f->Get("AllSBins_v7_MultiJCS");
  // TH1D *h4=(TH1D*)f->Get("AllSBins_v7_MultiJPred");

  // cout<<"$N_{jets}^{b-tag}$  & Bin & Low \\dphi Raw  & EW low \\dphi & Low \\dphi & High \\dphi \\\\ \\hline"<<endl;

  // for(int i=1;i<=h1->GetNbinsX();i++){
  //   if(i==1)       cout<<"\\multirow{6}{*}{$N_{2-4}^{0}$}"<<endl;
  //   if(i==7)       cout<<"\\multirow{5}{*}{$N_{5-6}^{0}$}"<<endl;
  //   if(i==12)      cout<<"\\multirow{5}{*}{$N_{\\geq7}^{0}$}"<<endl;
  //   if(i==17)      cout<<"\\multirow{5}{*}{$N_{2-4}^{\\geq1}$}"<<endl;
  //   if(i==22)      cout<<"\\multirow{5}{*}{$N_{5-6}^{\\geq1}$}"<<endl;
  //   if(i==27)      cout<<"\\multirow{5}{*}{$N_{\\geq7}^{\\geq1}$}"<<endl;

  //   printf("& %d   &  %.0f  &   %.2f $\\pm$ %.2f  &    %.2f $\\pm$ %.2f   & %.2f $\\pm$ %.2f \\\\ ",i,h1->GetBinContent(i),h2->GetBinContent(i),h2->GetBinError(i),h3->GetBinContent(i),h3->GetBinError(i),h4->GetBinContent(i),h4->GetBinError(i));
  //   cout<<endl;
  //   if(i==0 || i==6  || i==11 || i==16 || i==21 || i==26)      cout<<"\\hline"<<endl;
  // }
  // cout<<"\\hline"<<endl;
 

}
