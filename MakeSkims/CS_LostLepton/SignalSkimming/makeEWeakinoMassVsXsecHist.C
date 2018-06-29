#include<iostream>
#include<iomanip>
#include<string>
#include"TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"

void makeEWeakinoMassVsXsecHist(){
  gStyle->SetOptStat(0);
  vector<double> mass,xsec_nc,absUnc_nc,xsec_cc,absUnc_cc;
  ifstream infile("mEWeakino_N2C1Xsecfb_absUnc_C1C1Xsecfb_absUnc.txt");
  string line;
  while (getline(infile, line)){
    std::istringstream iss(line);
    double mass1,xsec1,absUnc1,ccXsec1,ccabsUnc1;
    iss>>mass1;
    iss>>xsec1;
    iss>>absUnc1;

    iss>>ccXsec1;
    iss>>ccabsUnc1;

    mass.push_back(mass1);
    xsec_nc.push_back(xsec1);
    absUnc_nc.push_back(absUnc1);

    xsec_cc.push_back(ccXsec1);
    absUnc_cc.push_back(ccabsUnc1);
  }
  TH1D *h1 = new TH1D("mEWeakino_N2C1Xsec","x:mass of EWeakion in GeV, bin content(y-axis) is N2C1 xsec in pb and bin error is xsec uncertainty in pb",3011,-0.5,3010.5);
  TH1D *h2 = new TH1D("mEWeakino_C1C1Xsec","x:mass of EWeakion in GeV, bin content(y-axis) is C1C1 xsec in pb and bin error is xsec uncertainty in pb",3011,-0.5,3010.5);
  TH1D *h3;// = new TH1D("mEWeakino_SumN2C1_C1C1Xsec","x:mass of EWeakion in GeV, bin content(y-axis) is sum of N2C1 xsec(pb) and C1C1 xsec(pb) and bin error is xsec uncertainty in pb",3011,-0.5,3010.5);
  for(int i=0;i<mass.size();i++){
    h1->Fill(mass[i],xsec_nc[i]/1000.0);
    h1->SetBinError(h1->FindBin(mass[i]),absUnc_nc[i]/1000.0);

    h2->Fill(mass[i],xsec_cc[i]/1000.0);
    h2->SetBinError(h2->FindBin(mass[i]),absUnc_cc[i]/1000.0);
  }
  h3 = (TH1D*)h1->Clone("mEWeakino_SumN2C1_C1C1Xsec");
  h3->Add(h2);
  h3->SetTitle("x:mass of EWeakion in GeV, bin content(y-axis) is sum of N2C1 xsec(pb) and C1C1 xsec(pb) and bin error is xsec uncertainty in pb");

  h1->GetXaxis()->SetTitle("EWeakino Mass(GeV)");
  h2->GetXaxis()->SetTitle("EWeakino Mass(GeV)");
  h3->GetXaxis()->SetTitle("EWeakino Mass(GeV)");
  h1->GetYaxis()->SetTitle("#sigma_{N2C1}(pb)");
  h2->GetYaxis()->SetTitle("#sigma_{C1C1}(pb)");
  h3->GetYaxis()->SetTitle("#sigma_{N2C1+C1C1}(pb)");

  //  h1->Draw();
  TFile *f1=new TFile("EWeakino_Xsec.root","recreate");
  h1->Write();
  h2->Write();
  h3->Write();
  if(1){
    int np=mass.size();
    if(np!=xsec_nc.size() || np!=absUnc_nc.size() || np!=xsec_cc.size() || np!=absUnc_cc.size()){ cout<<"problem in getting mass and xsec"<<endl; return;}
    cout<<"mEWeakino"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<mass[i]<<",";
    }
    cout<<endl<<"N2C1 xsec"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<xsec_nc[i]<<",";
    }
    cout<<endl<<"N2C1 Uncxsec"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<absUnc_nc[i]<<",";
    }
    cout<<endl<<"C1C1 xsec"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<xsec_cc[i]<<",";
    }
    cout<<endl<<"C1C1 Uncxsec"<<endl<<"{";
    for(int i=0;i<np;i++){
      cout<<absUnc_cc[i]<<",";
    }
    cout<<endl;
  }
}
