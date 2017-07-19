#ifndef SkimmingLostLept_H
#define SkimmingLostLept_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "NtupleVariables.h"
#include "TH1F.h"
#include "TH2.h"
#include <TProfile.h>
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

class SkimmingLostLept : public NtupleVariables{

 public:
  SkimmingLostLept(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~SkimmingLostLept();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram();
  TLorentzVector getBestPhoton();
   
  //Variables defined
  TH1F *h_selectBaselineYields_;
  TFile *oFile;
  
};
#endif

#ifdef SkimmingLostLept_cxx

void SkimmingLostLept::BookHistogram() {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
  h_selectBaselineYields_ = new TH1F("selectBaselineYields_","selectBaselineYields_",9,0.5,9.5);
  
 
}


SkimmingLostLept::SkimmingLostLept(const TString &inputFileList, const char *outFileName, const char* dataset) {
  string nameData=dataset;//vvv
  //  TChain *tree = new TChain("PreSelection");
  TChain *tree = new TChain("TreeMaker2/PreSelection");
  //if(nameData=="signalH") tree = new TChain("TreeMaker2/PreSelection");//vvv
  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  //if(nameData!="signalH") nameData="BG";
  //cout<<"Treating the input files as "<<nameData<<" for setting tree branches"<<endl;
  NtupleVariables::Init(tree,nameData);

  BookHistogram();
  char outflName[100];
  sprintf(outflName,"%s_%s",dataset,outFileName);
  oFile = new TFile(outflName, "recreate");
}

Bool_t SkimmingLostLept::FillChain(TChain *chain, const TString &inputFileList) {

  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;

  if(!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  while(1) {
    infile >> buffer;
    if(!infile.good()) break;
    //std::cout << "Adding tree from " << buffer.c_str() << std::endl;                                                              
    chain->Add(buffer.c_str());
  }
  std::cout << "No. of Entries in this tree : " << chain->GetEntries() << std::endl;
  return kTRUE;
}

Long64_t SkimmingLostLept::LoadTree(Long64_t entry) {
  // Set the environment to read one entry                                                                                          
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }
  return centry;
}

SkimmingLostLept::~SkimmingLostLept() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  h_selectBaselineYields_->Write();
  oFile->Write();
  oFile->Close();

}

#endif

