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

TString name;

void setLastBinAsOverFlow(TH1D*);
void searchBinsClosure(TString iFname){
  double sr_Integral=0,cr_Integral=0;
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
 
  TFile *f = new TFile(iFname);
  TCanvas *c_cA=new TCanvas("allSearchBins_LostMuClsr","all search bins closure for lost muon",2000,600);
  TPad *p_top=new TPad("top","top",0,0.25,1,1);
  TPad *p_bot=new TPad("bot","bot",0,0.05,1,0.25);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  p_top->SetBottomMargin(0);
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.2);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();

  TH1D *h_histG,*h_histE,*h_histGcopy,*h_temp;
  TH1D *h_CR=new TH1D("searchBins_Mu0","searchBins 0 Mu:ST(500,Inf)-[2,4][5,6],[7,inf)",21,0.5,21.5);
  TH1D *h_SR=new TH1D("searchBins_Mu1","searchBins 1 Mu:ST(500,Inf)-[2,4][5,6],[7,inf)",21,0.5,21.5);
  for(int i=1;i<=3;i++){
    name="MET_Mu0_R"+to_string(i);
    h_histG=(TH1D*)f->FindObjectAny(name);

    name="MET_Mu1_R"+to_string(i);
    h_histE=(TH1D*)f->FindObjectAny(name);

    setLastBinAsOverFlow(h_histG);
    setLastBinAsOverFlow(h_histE);
    sr_Integral+=h_histG->Integral();
    cr_Integral+=h_histE->Integral();
    for(int j=6;j<=h_histG->GetNbinsX();j++){
      cout<<name<<" i,j: "<<i<<","<<j<<" low "<<h_histG->GetBinLowEdge(j)<<" cont "<<h_histG->GetBinContent(j)<<" err "<<h_histG->GetBinError(j)<<" fill # "<<((i-1)*7)+j-5<<endl;
      h_SR->SetBinContent( ((i-1)*7)+j-5,h_histG->GetBinContent(j) );
      h_SR->SetBinError  ( ((i-1)*7)+j-5,h_histG->GetBinError(j) );
      h_CR->SetBinContent( ((i-1)*7)+j-5,h_histE->GetBinContent(j) );
      h_CR->SetBinError  ( ((i-1)*7)+j-5,h_histE->GetBinError(j) );

      h_SR->SetLineColor(kRed);
      h_SR->SetLineWidth(2);
      h_SR->SetMarkerStyle(21);
      h_SR->SetMarkerColor(h_SR->GetLineColor());
      h_SR->SetFillColor(h_SR->GetLineColor());
      h_SR->SetFillStyle(3003);

      h_CR->SetLineColor(kGreen+2);
      h_CR->SetLineWidth(2);
      h_CR->SetMarkerStyle(20);
      h_CR->SetMarkerColor(h_CR->GetLineColor());
      h_CR->GetYaxis()->SetLabelSize(0.05);
    }
  }
  TH1D *h_numr=(TH1D*)h_SR->Clone();
  h_numr->Divide(h_CR);
  h_numr->SetLineColor(kBlack);
  h_numr->SetMarkerColor(kBlack);
  //  h_numr->SetTitle(";;#frac{0MuG}{1MuG}");
  h_numr->SetTitle(";;Exp / Pred");
  h_numr->GetYaxis()->SetTitleOffset(0.14);
  h_numr->GetYaxis()->SetTitleSize(0.16);
  h_numr->GetXaxis()->SetLabelSize(0.2);
  h_numr->GetYaxis()->SetLabelSize(0.13);
  h_numr->GetYaxis()->SetNdivisions(505);
  h_numr->SetMaximum(2.0);
  h_numr->SetMinimum(0.01);
  c_cA->cd();    p_bot->cd();
  h_numr->Draw("e0");
  // c_cA->SetLogy();
  // c_cA->SetGridx();
 
  c_cA->cd();p_top->cd();
  h_CR->Draw();
  h_SR->Draw("BAR same");

  TLegend *legend=new TLegend(0.75, 0.90,  0.90, 0.75);
  legend->AddEntry(h_SR,"0 Mu (Exp.)","l");
  legend->AddEntry(h_CR,"1 Mu (Pred.)","l");
  legend->Draw();
  c_cA->SaveAs("a.png");

  cout<<"Integral in SR(0 Mu events) "<<sr_Integral<<endl
      <<"Integral in CR(1 Mu events) "<<cr_Integral<<endl;
      
}



void setLastBinAsOverFlow(TH1D* h_hist){
  double lastBinCt =h_hist->GetBinContent(h_hist->GetNbinsX()),overflCt =h_hist->GetBinContent(h_hist->GetNbinsX()+1);
  double lastBinErr=h_hist->GetBinError(h_hist->GetNbinsX()),  overflErr=h_hist->GetBinError(h_hist->GetNbinsX()+1);
  
  if(lastBinCt!=0 && overflCt!=0){
    lastBinErr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );
    lastBinCt = lastBinCt+overflCt;
    h_hist->SetBinContent(h_hist->GetNbinsX(),lastBinCt);
    h_hist->SetBinError(h_hist->GetNbinsX(),lastBinErr);
  }
  
}
