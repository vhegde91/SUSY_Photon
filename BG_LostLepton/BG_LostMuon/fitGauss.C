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
#include <fstream>
#include <sstream>
#include"TGraphErrors.h"
#include"TGraph.h"
#include"TLegend.h"
#include"TLatex.h"
#include"TCanvas.h"
#include"TMultiGraph.h"
#include"TStyle.h"

using namespace std;

double fitfunc(double*,double*);
void fitGauss(){
  //  gStyle->SetOptStat(11122);
  gStyle->SetOptFit(1);
  TFile *f = TFile::Open("pull_TFodd.root");
  TH1D *h = (TH1D*)f->Get("pull_lostMuHadTau");
  //  h->Rebin(2);
  TF1 *ft = new TF1("ft","gaus",-2,5);
  //  TF1 *ft = new TF1("ft",fitfunc,-2,3,3);
  ft->SetParameters(6.,0.,1.0);
  ft->SetParLimits(0,3,14);
  ft->SetParLimits(1,-0.5,0.5);
  ft->SetParLimits(2,0.0,2.0);
  h->SetTitle(";Pull;Entries");
  h->Draw();
  h->Fit(ft,"R");
  cout<<ft->GetParameter(0)<<" "<<ft->GetParameter(1)<<" "<<ft->GetParameter(2)<<" "<<endl;

}


double fitfunc(double *x, double *p){
  return p[0]*exp(-0.5*((x[0]-p[1])/p[2])*((x[0]-p[1])/p[2]));
}
