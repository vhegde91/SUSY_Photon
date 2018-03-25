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

TFile *f;
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getXaxisName(TString);
TString getLegName(TString);
TLatex textOnTop,intLumiE;
double intLumi=35.9;

double sr_Integral=0,cr_Integral=0;

void scaleUncComp(TString iFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
 
  f = new TFile(iFname);
  TCanvas *c_cA=new TCanvas("cA","cA",1500,800);
  TCanvas *c_cB=new TCanvas("cB","cB",1500,800);
  TH2D *h2_scale = (TH2D*)f->Get("SBins_v7_CD_ScaleIdx");
  for(int j=1;j<=h2_scale->GetNbinsY();j++){
    for(int i=1;i<=h2_scale->GetNbinsX();i++){
      if(i==1 || i==7 || i==12 || i==17 || i==22 || i==27) h2_scale->SetBinContent(i,j,0);
    }
  }
  for(int j=1;j<=h2_scale->GetNbinsY();j++){
    float intgr = h2_scale->Integral(1,31,j,j);
    if(intgr < 0.0001) continue;
    for(int i=1;i<=h2_scale->GetNbinsX();i++){
      //     h2_scale->SetBinContent(i,j,(h2_scale->GetBinContent(i,j))/intgr);
    }
  }
  // h2_scale->GetZaxis()->SetRangeUser(0.16,0.18);
  // c_cA->cd();
  // h2_scale->Draw("colz text");    
  
  // h2_scale->SetTitle(";Bin no.;Scale Index");
  // h2_scale->GetYaxis()->SetLabelSize(0.06);
  // h2_scale->GetYaxis()->SetTitleSize(0.06);
  // h2_scale->GetYaxis()->SetTitleOffset(0.60);
  
  c_cB->cd();
  TH1D *h1,*h0 = (TH1D*)h2_scale->ProjectionX("ScaleIndex_Ref",1,1);
  TString histName;
  for(int i=1;i<=h2_scale->GetNbinsY();i++){
    if(i>1) continue;
    if(h2_scale->Integral(1,31,i,i) < 0.001) continue;
    histName = "Scale_"+to_string(i-1);
    h1 = (TH1D*)h2_scale->ProjectionX(histName,i,i);
    //    for(int k=1;k<=h1->GetNbinsX();k++) h1->SetBinError(k,0.0001);
   
    h1->SetTitle(";Bin no.;Scale X / Scale 0");
    h1->GetYaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetTitleOffset(0.80);
    h1->GetYaxis()->SetRangeUser(0.85,1.15);
    // c_cA->cd();
    // if(i==1)
    //   h1->Draw("plc e1");
    // else 
    //   h1->Draw("plc e1 same");
    //   h1->Divide(h0);
    //   for(int k=1;k<=h1->GetNbinsX();k++) h1->SetBinError(k,0.0001);
    c_cB->cd();
    if(i==1)
      h1->Draw("plc e1");
    else 
      h1->Draw("plc e1 same");
  }
  c_cA->BuildLegend();
  c_cB->BuildLegend();
  char name3[100];
  textOnTop.SetTextSize(0.05);
  intLumiE.SetTextSize(0.05);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
  sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.7,0.91,name3);

  gStyle->SetTextSize(2);

}
TString getXaxisName(TString axname){
  if(axname.Contains("nHadJets")) return "Jets";
  else if(axname.Contains("ST")) return "ST(GeV)";
  else if(axname.Contains("BTags")) return "b-Tags";
  else if(axname.Contains("ElePt")) return "e pT(GeV)";
  else if(axname.Contains("PhotonPt")) return "#gamma pT(GeV)";
  else if(axname.Contains("mT_")) return "mT(GeV)";
  else if(axname.Contains("AllSBin")) return "Bin Number";
  else if(axname.Contains("dPhi_METjet1") || axname.Contains("dphi1_METjet1")) return "#Delta#Phi_{1}";
  else if(axname.Contains("dPhi_METjet2") || axname.Contains("dphi2_METjet2")) return "#Delta#Phi_{2}";
  else if(axname.Contains("dPhi_METBestPhoton") ) return "#Delta#Phi(MET,#gamma)";
  else if(axname.Contains("QMut") || axname.Contains("Qmut")) return "QMult";
  else if(axname.Contains("MET")) return "MET(GeV)";
  else return axname;

}

TString getLegName(TString legName1){
  if(legName1.Contains("JEC")) return "JEC";
  else if(legName1.Contains("JER")) return "JER";
  else if(legName1.Contains("JECup")) return "JEC Up";
  else if(legName1.Contains("JECdn")) return "JEC Down";
  else if(legName1.Contains("JERup")) return "JER Up";
  else if(legName1.Contains("JERdn")) return "JER Down";
  else if(legName1.Contains("ISRdn")) return "ISR Down";
  else if(legName1.Contains("ISRup")) return "ISR Up";
  else if(legName1.Contains("genMET")) return "Gen MET";
  else if(legName1.Contains("AllSBins_v7_CD")) return "Central";
  else return legName1;


}

void setLastBinAsOverFlow(TH1D* h_hist){
  double lastBinCt =h_hist->GetBinContent(h_hist->GetNbinsX()),overflCt =h_hist->GetBinContent(h_hist->GetNbinsX()+1);
  double lastBinErr=h_hist->GetBinError(h_hist->GetNbinsX()),  overflErr=h_hist->GetBinError(h_hist->GetNbinsX()+1);
  
  if(lastBinCt!=0 && overflCt!=0)
    lastBinErr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );
  
  else if(lastBinCt==0 && overflCt!=0)
    lastBinErr = overflErr;
  else if(lastBinCt!=0 && overflCt==0)
    lastBinErr = lastBinErr;
  else lastBinErr=0;

  lastBinCt = lastBinCt+overflCt;
  h_hist->SetBinContent(h_hist->GetNbinsX(),lastBinCt);
  h_hist->SetBinError(h_hist->GetNbinsX(),lastBinErr);
    
}
