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

TLatex textOnTop,intLumiE;
double intLumi=35.9;
void getCovCorrMatrix(){
  gStyle->SetOptStat(0);
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  bool isPaper = 0; 

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  // Double_t red[NRGBs]   = { 0.50, 0.50, 1.00, 1.00, 1.00 };
  Double_t red[NRGBs]   = { 0.0, 0.0, 1.00, 1.00, 1.00 };
  Double_t green[NRGBs] = { 0.50, 1.00, 1.00, 0.60, 0.50 };
  Double_t blue[NRGBs]  = { 1.00, 1.00, 0.50, 0.40, 0.50 };

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
   
  TFile *f = new TFile("mlfit_SUSY.root");
  //  TFile *f = new TFile("mlfit_SUSY20kToys.root");
  TH2D *h2_cov=(TH2D*)f->Get("shapes_prefit/overall_total_covar");  
  TH2D *h2_corr=(TH2D*)h2_cov->Clone("corr");
  for(int i=1;i<=h2_cov->GetNbinsX();i++){
    for(int j=1;j<=h2_cov->GetNbinsY();j++){
      h2_corr->SetBinContent(i,j,(h2_cov->GetBinContent(i,j)/sqrt(h2_cov->GetBinContent(i,i)*h2_cov->GetBinContent(j,j))));
      //      cout<<"Corr: "<<i<<" "<<j<<" "<<h2->GetBinContent(i,j)<<endl;
    }
  }
  
  TH2D *h2_corr2=new TH2D("Correlation","Correlation between search regions",25,0.5,25.5,25,0.5,25.5);
  TH2D *h2_cov2=new TH2D("Covariance","Covariance between search regions",25,0.5,25.5,25,0.5,25.5);

  TString xBin,yBin;
  for(int i=1;i<=h2_cov->GetNbinsX();i++){
    xBin = h2_cov->GetXaxis()->GetBinLabel(i);
    xBin.ReplaceAll("ch1_ch","");
    xBin.ReplaceAll("_0","");
    for(int j=1;j<=h2_cov->GetNbinsY();j++){
      yBin = h2_cov->GetYaxis()->GetBinLabel(j);
      yBin.ReplaceAll("ch1_ch","");
      yBin.ReplaceAll("_0","");
      //      cout<<atoi(xBin)<<" "<<atoi(yBin)<<endl;
      h2_corr2->SetBinContent(atoi(xBin),atoi(yBin),h2_corr->GetBinContent(i,j));
      h2_cov2->SetBinContent(atoi(xBin),atoi(yBin),h2_cov->GetBinContent(i,j));
    }
  }

  TFile *fPred=TFile::Open("../SBinHists.root");
  TH2D *h2_TotPred = (TH2D*)fPred->Get("AllSBins_v7_TotalBG");
  int sBin=0,indx=0;
  cout<<"Bin | sqrt(cov) | prefit unc | sqrt(cov)/prefit unc"<<endl;
  cout<<"---------------------------------"<<endl;
  for(int i=1;i<=h2_cov2->GetNbinsX();i++){
    for(int j=1;j<=h2_cov2->GetNbinsY();j++){
      if(i!=j) continue;
      indx++;
      if     (i==1 || i==6  || i==10 || i==14 || i==18 || i==22) indx++; 
      // sidebands i==1 || i==7  || i==12 || i==17 || i==22 || i==27
      //      cout<<i<<" "<<indx<<endl;
      cout<<i<<" "<<sqrt(h2_cov2->GetBinContent(i,j))<<" "<<h2_TotPred->GetBinError(indx)<<" "<<sqrt(h2_cov2->GetBinContent(i,j))/h2_TotPred->GetBinError(indx)<<endl;
    }
  }

  TCanvas *corr = new TCanvas("CorrelationMatrix","Corr",1300,1000);
  corr->SetRightMargin(0.18);
  h2_corr2->SetTitle(0);
  h2_corr2->GetZaxis()->SetTitle("Correlation between search regions");
  h2_corr2->GetXaxis()->SetTitle("Bin number");
  h2_corr2->GetYaxis()->SetTitle("Bin number");
  h2_corr2->GetYaxis()->SetTitleOffset(0.9);
  h2_corr2->Draw("colz");
  // gPad->Update();
  // gPad->RedrawAxis();

  char name2[100];
  textOnTop.SetTextSize(0.04);
  intLumiE.SetTextSize(0.04);
  if(isPaper) textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Supplementary}}");
  else textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
  sprintf(name2,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.64,0.91,name2);
  TLatex Tl;
  Tl.SetTextSize(0.04);
  if(isPaper) Tl.DrawLatexNDC(0.4,0.91,"#bf{arXiv:xxxx.xxxxx}");

  TCanvas *cov = new TCanvas("CovariancenMatrix","Cov",1300,1000);
  cov->SetRightMargin(0.18);
  cov->SetLogz();
  h2_cov2->Draw("colz");
  h2_cov2->SetTitle(0);
  h2_cov2->GetZaxis()->SetTitle("Covariance between search regions");
  h2_cov2->GetXaxis()->SetTitle("Bin number");
  h2_cov2->GetYaxis()->SetTitle("Bin number");
  h2_cov2->GetYaxis()->SetTitleOffset(0.9);
  if(isPaper) textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Supplementary}}");
  else textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
  intLumiE.DrawLatexNDC(0.64,0.91,name2);
  if(isPaper) Tl.DrawLatexNDC(0.4,0.91,"#bf{arXiv:xxxx.xxxxx}");

  corr->SaveAs("CorrelationMatrix.pdf");
  h2_corr2->SaveAs("CorrelationMatrix.root");

  cov->SaveAs("CovarianceMatrix.pdf");
  h2_cov2->SaveAs("CovarianceMatrix.root");
  
}
