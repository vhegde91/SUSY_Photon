#include<iostream>
#include<iomanip>
#include<string>
#include"TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
void numEventsComp(){
  //  TFile *f1= TFile::Open(inFile);
  //  TString fName1,fName="root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/TChiNG_";
  //  TString fName1,fName="root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal_bTagEff/T5bbbbZg_";
  TString fName1,fName="root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal_v2/T5ttttZg_";
  TFile *f1;
  //  TFile *f2= TFile::Open("TChiNG_MassScan.root");
  //  TFile *f2= TFile::Open("T5bbbbZg_MassScan.root");
  TFile *f2= TFile::Open("T5ttttZg_MassScan.root");
  TH1D *h1;
  TH2D *h2_mass = (TH2D*)f2->FindObjectAny("MGlMNLSP");
  cout<<"File location: "<<fName<<endl;

  double entries1=0,entries2=-100,totEvents1=0,totEvents2=0;
  for(int i=1;i<=h2_mass->GetNbinsX();i++){
    for(int j=1;j<=h2_mass->GetNbinsY();j++){
      entries2=h2_mass->GetBinContent(i,j);
      if(entries2<0.1) continue;
      int mp=h2_mass->GetXaxis()->GetBinCenter(i);
      int md=h2_mass->GetYaxis()->GetBinCenter(j);
      //      if(mp==800 && md==127) continue;
      fName1=fName+to_string(mp)+"_"+to_string(md)+"_SkimmedSorted.root";
      //      cout<<fName1<<endl;
      //      cout<<mp<<","<<md<<" ";
      f1=TFile::Open(fName1);
      if(!f1){ cout<<fName1<<" file open error"<<endl;continue;}
      h1=(TH1D*)f1->FindObjectAny("selectBaselineYields_");
      double entries1;
      if(h1) entries1=h1->GetBinContent(1);
      totEvents1+=entries1;
      totEvents2+=entries2;
      if(abs(entries2-entries1) > 0.1){
      	cout<<"mismatch in num events for (mGl,mNLSP)."<<h2_mass->GetXaxis()->GetBinCenter(i)<<","<<h2_mass->GetYaxis()->GetBinCenter(j)<<endl
      	    <<"# of produced evenets:"<<setprecision(9)<<entries2<<endl
      	    <<"# of processed events:"<<setprecision(9)<<entries1<<endl;
      }
    }
  }
  cout<<"Total events:"<<setprecision(9)<<totEvents1<<" Dataset has "<<setprecision(9)<<totEvents2<<" events"<<endl;
}
void numEventsComp(TString inFile,double mp,double md){
//bool numEventsComp(TString inFile,double mp,double md){
  TFile *f1= TFile::Open(inFile);
  //  TFile *f2= TFile::Open("TChiNG_MassScan.root");
  //  TFile *f2= TFile::Open("T5bbbbZg_MassScan.root");
  TFile *f2= TFile::Open("T5ttttZg_MassScan.root");
  TH1D *h1=(TH1D*)f1->FindObjectAny("selectBaselineYields_");
  TH2D *h2_mass = (TH2D*)f2->FindObjectAny("MGlMNLSP");
  double entries1=h1->GetBinContent(1),entries2=0;

  entries2=h2_mass->GetBinContent(h2_mass->GetXaxis()->FindBin(mp),h2_mass->GetYaxis()->FindBin(md));
  char cmd1[300];
  cout<<"org events:"<<entries2<<" processed events:"<<entries1<<endl;
  if(abs(entries2-entries1) < 0.1){
    cout<<"yyyy"<<endl;
    //    sprintf(cmd1,"rm /eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/TChiNG_%.0f_%.0f_*pythia8_job*.root",mp,md);
    //    sprintf(cmd1,"rm /eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal_bTagEff/T5ChiNG_%.0f_%.0f_*pythia8_job*.root",mp,md);
    //    sprintf(cmd1,"eos root://cmseos.fnal.gov rm /store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal_bTagEff/T5ChiNG_%.0f_%.0f_*pythia8_job*.root",mp,md);
    sprintf(cmd1,"eos root://cmseos.fnal.gov rm /store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/temp/T5ttttZg_%.0f_%.0f_*pythia8_job*.root",mp,md);
    cout<<cmd1<<endl;
    // sprintf(cmd1,"goodOPFile=1");
    system(cmd1);
    // sprintf(cmd1,"echo OP $goodOPFile");
    // system(cmd1);
    // return true;
  }
  else{
    cout<<"nnnn"<<endl;
    // sprintf(cmd1,"goodOPFile=0");
    // system(cmd1);
    // return false;
  }

}
