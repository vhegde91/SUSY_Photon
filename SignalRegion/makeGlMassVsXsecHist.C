#include<iostream>
#include<iomanip>
#include<string>
#include"TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"

void makeGlMassVsXsecHist(){
  gStyle->SetOptStat(0);
  vector<double> mass,xsec,pcUnc,absUnc;
  //  ifstream infile("mGl_Xsecpb_pcUnc_absUnc.txt");
  ifstream infile("mStop_Xsecpb_pcUnc_absUnc.txt");
  string line;
  while (getline(infile, line)){
    std::istringstream iss(line);
    double mass1,xsec1,pcUnc1,absUnc1;
    iss>>mass1;
    iss>>xsec1;
    iss>>pcUnc1;
    iss>>absUnc1;
    mass.push_back(mass1);
    xsec.push_back(xsec1);
    pcUnc.push_back(pcUnc1);
    absUnc.push_back(absUnc1);
  }
  //  TH1D *h1 = new TH1D("mGlXsec","x:mass of gluino in GeV, bin content(y-axis) is xsec in pb and bin error is xsec uncertainty in pb",3011,-0.5,3010.5);
  TH1D *h1 = new TH1D("mStopXsec","x:mass of sTop/sBottom in GeV, bin content(y-axis) is xsec in pb and bin error is xsec uncertainty in pb",3011,-0.5,3010.5);
  for(int i=0;i<mass.size();i++){
    h1->Fill(mass[i],xsec[i]);
    h1->SetBinError(h1->FindBin(mass[i]),absUnc[i]);
  }
  h1->GetXaxis()->SetTitle("Gluino Mass(GeV)");
  h1->GetYaxis()->SetTitle("#sigma(pb)");
  h1->Draw();
  //  TFile *f1=new TFile("mGl_Xsecpb_absUnc.root","recreate");
  TFile *f1=new TFile("mStop_Xsecpb_absUnc.root","recreate");
  h1->Write();
  if(0){
    int np=mass.size();
    if(np!=xsec.size() || np!=pcUnc.size() || np!=absUnc.size()){ cout<<"problem in getting mass and xsec"<<endl; return;}
    cout<<"mGl_1"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<mass[i]<<",";
    }
    cout<<endl<<"xsec_1"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<xsec[i]<<",";
    }
    cout<<endl<<"Uncxsec_1"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<absUnc[i]<<",";
    }
    cout<<endl;
  }
}
