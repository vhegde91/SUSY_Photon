#include<iostream>
#include<iomanip>
#include<string>
#include"TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"

void addFiles(){

  string Fpath="/eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/T5qqqqHg_";
  TFile *f1 = new TFile("T5qqqqHg_MassScan.root");
  TFile *f2;
  TH1D *h1;
  TH2D *h2_mass = (TH2D*)f1->FindObjectAny("MGlMNLSP");
  int nxbins = h2_mass->GetNbinsX();
  int nybins = h2_mass->GetNbinsY();
  char cmd[1000],name[200];
  double entries=-100;
  for(int nx=1;nx<=nxbins;nx++){
    //    if(h2_mass->GetXaxis()->GetBinCenter(nx)>1001) continue;
    if(h2_mass->GetXaxis()->GetBinCenter(nx)<1001) continue;
    if(h2_mass->GetXaxis()->GetBinCenter(nx)>1501) break;
    for(int ny=1;ny<=nybins;ny++){
      double mgl=h2_mass->GetXaxis()->GetBinCenter(nx);
      double mnlsp=h2_mass->GetYaxis()->GetBinCenter(ny);
      if(h2_mass->GetBinContent(nx,ny) < 0.1) continue;
      if(mnlsp==127 && mgl==1050) continue;
      //      if(mnlsp>150) continue;
      sprintf(cmd,"hadd -f T5qqqqHg_%.0f_%.0f_SkimmedSorted.root `xrdfsls -u /store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/ | grep 'T5qqqqHg_%.0f_%.0f_Spring16Fast.SMS-T5qqqqHg_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_job'`",mgl,mnlsp,mgl,mnlsp);
      cout<<cmd<<endl;
      system(cmd);      

      sprintf(cmd,"xrdcp -f T5qqqqHg_%.0f_%.0f_SkimmedSorted.root root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/",mgl,mnlsp);
      cout<<cmd<<endl;
      system(cmd);

      sprintf(cmd,"rm T5qqqqHg_*_SkimmedSorted.root");
      cout<<cmd<<endl;
      system(cmd);

      sprintf(name,"%s%.0f_%.0f_SkimmedSorted.root",Fpath.c_str(),mgl,mnlsp);
      f2 = TFile::Open(name);
      h1=(TH1D*)f2->FindObjectAny("selectBaselineYields_");
      entries=h1->GetBinContent(1);
      cout<<"org events:"<<h2_mass->GetBinContent(nx,ny)<<" processed events:"<<entries<<endl;
      if(abs(entries-h2_mass->GetBinContent(nx,ny)) < 0.1){
	cout<<"removing files..."<<endl;
	sprintf(cmd,"rm %s%.0f_%.0f_*T5qqqqHg*job*.root",Fpath.c_str(),mgl,mnlsp);
	cout<<cmd<<endl;
	system(cmd);
      }
      else{
	cout<<mgl<<"\t"<<mnlsp<<endl;
      }
    }
  }
}


//1050-127, 
      //`xrdfsls -u /store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/ | grep 'T5qqqqHg_1050_150_Spring16Fast.SMS-T5qqqqHg_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_job'`
      //      sprintf(cmd,"hadd -f %s%.0f_%.0f_SkimmedSorted.root %s%.0f_%.0f_*T5qqqqHg*job*.root",Fpath.c_str(),mgl,mnlsp,Fpath.c_str(),mgl,mnlsp);
