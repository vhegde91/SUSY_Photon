#include<iostream>
#include<iomanip>
#include<string>
#include"TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"

void makeGGMxsecHist(){
  gStyle->SetOptStat(0);
  vector<double> massM1,massM3,xsec,absUnc;
  ifstream infile("GGM_mM1_mM3_Xsecpb_absUnc.txt");
  string line;
  while (getline(infile, line)){
    std::istringstream iss(line);
    double mass1M1,mass1M3,xsec1,absUnc1;
    iss>>mass1M1;
    iss>>mass1M3;
    iss>>xsec1;
    iss>>absUnc1;
    massM1.push_back(mass1M1);
    massM3.push_back(mass1M3);
    xsec.push_back(xsec1);
    absUnc.push_back(absUnc1);
  }
  TH2D *h2 = new TH2D("GGM_M1M3_xsec","GGM. x:mass of M3 in GeV, y:mass of M1 in GeV, z:xsec in pb with uncertainty in pb",3011,-0.5,3010.5,3011,-0.5,3010.5);
  for(int i=0;i<massM1.size();i++){
    h2->Fill(massM3[i],massM1[i],xsec[i]);
    h2->SetBinError(h2->GetXaxis()->FindBin(massM3[i]),h2->GetYaxis()->FindBin(massM1[i]),absUnc[i]);
  }
  h2->GetXaxis()->SetTitle("M3 Mass(GeV)");
  h2->GetYaxis()->SetTitle("M1 Mass(GeV)");
  h2->GetZaxis()->SetTitle("#sigma(pb)");
  h2->Draw("colz");
  TFile *f1=new TFile("GGM_mM1_mM3_Xsecpb_absUnc.root","recreate");
  h2->Write();
  if(0){
    int np=massM1.size();
    if(np!=xsec.size() || np!=massM3.size() || np!=absUnc.size()){ cout<<"problem in getting mass and xsec"<<endl; return;}
    cout<<"M1"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<massM1[i]<<",";
    }
    cout<<"M3"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<massM3[i]<<",";
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
