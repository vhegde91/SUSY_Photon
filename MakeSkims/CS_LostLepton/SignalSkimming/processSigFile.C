#include<iostream>
#include<iomanip>
#include<string>
#include"TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"

void processSigFile(string inTxtFile,string outRootFileName){
  string exeAna = "skimmingSR";
  string macro_datasetName = "GGM_M1M3";//"T6ttZg";//"TChiNG";//"T5bbbbZg";
  //  string outRootFileName = "T5qqqqHg.root";
  //////////////////////////////////////////
  ifstream infile(inTxtFile);
  string line;
  TString inFname;
  if(!infile.is_open()){cout<<"could not get "<<inTxtFile<<endl;return;}
  while (getline(infile, line)){
    std::istringstream iss(line);
    iss>>inFname;
  }
  TFile *_file0 = TFile::Open(inFname);
  TTree *macro_t1 = (TTree*)_file0->Get("TreeMaker2/PreSelection");
  //  TTree *macro_t1 = (TTree*)_file0->Get("PreSelection");
  ULong64_t macro_nentries1 = macro_t1->GetEntries();
  double macro_mGl,macro_mNLSP;
  macro_t1->SetBranchAddress("SusyMotherMass",&macro_mGl);
  macro_t1->SetBranchAddress("SusyLSPMass",&macro_mNLSP);
  macro_t1->SetBranchStatus("*",0);
  macro_t1->SetBranchStatus("SusyMotherMass",1);
  macro_t1->SetBranchStatus("SusyLSPMass",1);
  TH2D *macro_h2=new TH2D("macro_h2","macro_h2",3011,-0.5,3010.5,3011,-0.5,3010.5);
  for (ULong64_t i=0;i<macro_nentries1;i++) {
    macro_t1->GetEntry(i);
    macro_h2->Fill(macro_mGl,macro_mNLSP);
  }
  //  fout->cd();macro_h2->Write();
  //  cout<<inTxtFile;
  //  _file0->Close();
  //  delete macro_t1;
  //  delete _file0;
  // cout<<macro_h2->GetNbinsX()<<" "<<macro_h2->GetNbinsY()<<endl;
  // cout<<"xMean:"<<macro_h2->GetMean(1)<<" "<<macro_h2->GetMean(2)<<endl;
  //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  char macro_name[20000];
  int macro_tot=0;
  for(int i=0;i<macro_h2->GetNbinsX();i++){
    //    if(macro_h2->GetXaxis()->GetBinCenter(i)<1400) continue;
    for(int j=0;j<macro_h2->GetNbinsY();j++){
      if( (macro_h2->GetBinContent(i,j)) > 0.5){
	macro_tot+=macro_h2->GetBinContent(i,j);
	cout<<":::::::::::::::::::::::::::::::::::::::::::::"<<endl;
	cout<<"x:"<<macro_h2->GetXaxis()->GetBinCenter(i)<<" y:"<<macro_h2->GetYaxis()->GetBinCenter(j)<<" entries in this file:"<<macro_h2->GetBinContent(i,j)<<endl;
	sprintf(macro_name,"./%s %s %s %s %.0f %.0f",exeAna.c_str(),inTxtFile.c_str(),outRootFileName.c_str(),macro_datasetName.c_str(),macro_h2->GetXaxis()->GetBinCenter(i),macro_h2->GetYaxis()->GetBinCenter(j));
	//	cout<<macro_name<<endl;
	system(macro_name);
      }
    }//y
  }//x
  delete macro_h2;
  cout<<macro_tot<<endl;
}
