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
#include"/home/vinay/root_includes.h"
const int nfiles=2;
TFile *f[nfiles];
TH1D *h1[nfiles];
TLegend *lg1;
int col[10]={kRed,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};
vector<TString> fNames,histName,legName;
//void setLastBinAsOverFlow(TH1D*);
vector<int> rebin;
TString xAxisName="Bin no.";
TString yAxisName="";
bool saveHist=0,drawnormalized=0;

void makeRatioPlot();
void plotManyHists(){
  //-------------------------------------
  gStyle->SetOptStat(0);

  // fNames.push_back("EW_Subtracted_DVS_2016_ST1000.root"); 
  // histName.push_back("AllSBins_v4_CD");
  // legName.push_back("Exp");
  // rebin.push_back(1);
  //-------------------------------------
  fNames.push_back("VS_GJetsQCD_ST1000_v2.root");
  histName.push_back("MET_CD");
  legName.push_back("VR");
  rebin.push_back(5);
  //-------------------------------------
  // fNames.push_back("EW_Subtracted_DVS_doubleR_2016_ST1000.root");
  // histName.push_back("AllSBins_v4_AB");
  // legName.push_back("Pred");
  // rebin.push_back(1);
  fNames.push_back("GJetsQCD_prompt.root");
  histName.push_back("MET_CD");
  legName.push_back("SR");
  rebin.push_back(5);
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
  c1->SetLogy();
  lg1=new TLegend(0.7, 0.90,  0.90, 0.65);
  for(int i=0;i<nfiles;i++){
    f[i] = new TFile(fNames[i]);
    h1[i] = (TH1D*)f[i]->FindObjectAny(histName[i]);
    // setLastBinAsOverFlow(h1[i]);
    // h1[i]->ClearUnderflowAndOverflow();
    h1[i]->Rebin(rebin[i]);
    if(drawnormalized) h1[i]->Scale(1.0/h1[i]->Integral());
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
  //  c2->SetBottomMargin(0.13);
  lg1=new TLegend(0.7, 0.90,  0.90, 0.65);
  TPad *p_top=new TPad("CB_top","CB_top",0,0.4,1,1);
  TPad *p_bot=new TPad("CB_bot","CB_bot",0,0.1,1,0.4);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  // p_top->SetBottomMargin(0);
  // p_bot->SetTopMargin(0);
  //  p_bot->SetBottomMargin(0.0);
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

  TString ratioYName = legName[0]+" / "+legName[1];
  //  h1_cp->SetTitle(ratioYName);
  h1_cp->GetYaxis()->SetTitle(ratioYName);
  //  h1_cp->GetXaxis()->SetTitle(xAxisName);
  h1_cp->GetXaxis()->SetTitleSize(0.08);
  h1_cp->GetXaxis()->SetLabelSize(0.12);
  //  h1_cp->GetYaxis()->SetTitle(yAxisName);
  h1_cp->GetYaxis()->SetTitleSize(0.08);
  h1_cp->GetYaxis()->SetLabelSize(0.12);
  h1_cp->Draw();
  TFile *fout;
  if(saveHist){
    fout = new TFile("phoPt_forVRwithSRsel_AB.root","recreate");
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
// void setLastBinAsOverFlow(TH1D* h_hist){
//   double lastBinCt =h_hist->GetBinContent(h_hist->GetNbinsX()),overflCt =h_hist->GetBinContent(h_hist->GetNbinsX()+1);
//   double lastBinErr=h_hist->GetBinError(h_hist->GetNbinsX()),  overflErr=h_hist->GetBinError(h_hist->GetNbinsX()+1);

//   if(lastBinCt!=0 && overflCt!=0)
//     lastBinErr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );

//   else if(lastBinCt==0 && overflCt!=0)
//     lastBinErr = overflErr;
//   else if(lastBinCt!=0 && overflCt==0)
//     lastBinErr = lastBinErr;
//   else lastBinErr=0;

//   lastBinCt = lastBinCt+overflCt;
//   h_hist->SetBinContent(h_hist->GetNbinsX(),lastBinCt);
//   h_hist->SetBinError(h_hist->GetNbinsX(),lastBinErr);

// }
