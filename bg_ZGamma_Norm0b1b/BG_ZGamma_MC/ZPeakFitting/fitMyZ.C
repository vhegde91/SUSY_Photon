#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
using namespace std;
double fitfunc(double*,double*);
Double_t mybw(Double_t*, Double_t*);
Double_t background(Double_t *x, Double_t *par) {
  // return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
  return par[0] * exp(par[1]*x[0] + par[2]);
}
// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  //  return background(x,par) + lorentzianPeak(x,&par[3]);
  return background(x,par) + mybw(x,&par[3]);
}


void fitMyZ(){
  // TFile *f1 = new TFile("CS_ZDYToLLG.root");
  //  TFile *f1 = new TFile("DCS_ZGToLL_NoZMassWindow.root");
  TFile *f1 = new TFile("DCS_ZGToLL_NoZMassWindow_v2.root");
  TH1D *h_zmass=(TH1D*)f1->FindObjectAny("ZMass");
  h_zmass->Rebin(4);
  int nBins = h_zmass->GetNbinsX();
  double data[nBins];
  h_zmass->SetLineColor(kBlack);
  h_zmass->SetMarkerStyle(20);
  h_zmass->Draw();

  //TF1 *ft1=new TF1("ft1",fitfunc,0,500,3);
  //TF1 *ft1=new TF1("ft1",mybw,0,500,3);
  TF1 *ft1=new TF1("ft1",fitFunction,0,500,6);
  ft1->SetNpx(5000);
  // ft1->SetParameter(0,1.0);  // ft1->SetParName(0,"const");  
  // ft1->SetParameter(1,-0.002); //  ft1->SetParName(2,"mean");  
  // ft1->SetParameter(2,0.2);  //   ft1->SetParName(1,"sigma");
  // ft1->SetParameter(3,2.0);
  // ft1->SetParameter(4,90.0);
  // ft1->SetParameters(1,1,1,1,1,1);
  // h_zmass->Fit("ft1","0");
  ft1->SetParameter(4,2); // width
  ft1->SetParameter(5,90);   // peak
  h_zmass->Fit("ft1","V+","ep",10,150);

  // improve the picture:
  TF1 *backFcn = new TF1("backFcn",background,0,500,3);
  backFcn->SetLineColor(kGreen);
  TF1 *signalFcn = new TF1("signalFcn",mybw,0,500,3);
  signalFcn->SetLineColor(kBlue);
  signalFcn->SetNpx(5000);
  Double_t par[6];

  // writes the fit results into the par array
  ft1->GetParameters(par);

  backFcn->SetParameters(par);
  backFcn->Draw("same");

  signalFcn->SetParameters(&par[3]);
  signalFcn->Draw("same");

  TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
  legend->SetTextFont(72);
  legend->SetTextSize(0.04);
  legend->AddEntry(h_zmass,"Data","lpe");
  legend->AddEntry(backFcn,"Background fit","l");
  legend->AddEntry(signalFcn,"Signal(Z) fit","l");
  legend->AddEntry(ft1,"Global Fit","l");
  legend->Draw();
}


//------------------------
double fitfunc(double *x, double *par){
  //  return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
  double arg = 0;
  if (par[2] != 0) arg = (x[0] - par[1])/par[2];
  return par[0]*TMath::Exp(-0.5*arg*arg)/
     (TMath::Sqrt(2*TMath::Pi())*par[2]); // par[0] is constant
}

Double_t mybw(Double_t* x, Double_t* par)
{
  Double_t arg1 = 14.0/22.0; // 2 over pi
  Double_t arg2 = par[1]*par[1]*par[2]*par[2]; //Gamma=par[1]  M=par[2]
  Double_t arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]));
  Double_t arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]));
  return par[0]*arg1*arg2/(arg3 + arg4);
}

