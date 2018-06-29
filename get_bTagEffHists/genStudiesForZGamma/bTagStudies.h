#ifndef bTagStudies_H
#define bTagStudies_H

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

class bTagStudies : public NtupleVariables{

 public:
  bTagStudies(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~bTagStudies();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);

  TH1D *h_cutFlow;
  TH1D *h_nGenbs_0l,*h_nGenbs_1l,*h_nGenbs_2l;
  TH1D *h_BTags_0l,*h_BTags_1l,*h_BTags_2l;
  TH1D *h_MET;
  TH1D *h_madHT;
  TFile *oFile;
  
};
#endif

#ifdef bTagStudies_cxx

void bTagStudies::BookHistogram(const char *outFileName) {
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);
  char name[100],title[100];

  oFile = new TFile(outFileName, "recreate");
  h_cutFlow = new TH1D("cutFlow","cut flow: 0:all events, 1:2 top selection, 2:tau veto, 3:veto W to b+X decay, 4:veto fake lepton events",10,-0.5,9.5);

  h_nGenbs_0l = new TH1D("nGenbs_0l","no. of gen b's for 0lepton events",12,-0.5,11.5);
  h_nGenbs_1l = new TH1D("nGenbs_1l","no. of gen b's for 1lepton events",12,-0.5,11.5);
  h_nGenbs_2l = new TH1D("nGenbs_2l","no. of gen b's for 2lepton events",12,-0.5,11.5);

  h_BTags_0l = new TH1D("BTags_0l","BTags for 0lepton events",12,-0.5,11.5);
  h_BTags_1l = new TH1D("BTags_1l","BTags for 1lepton events",12,-0.5,11.5);
  h_BTags_2l = new TH1D("BTags_2l","BTags for 2lepton events",12,-0.5,11.5);

  h_MET = new TH1D("MET","MET",200,0,2000);
  h_madHT = new TH1D("madHT","madHT",300,0,3000);
}


bTagStudies::bTagStudies(const TString &inputFileList, const char *outFileName, const char* dataset) {
  string nameData=dataset;//vvv
  //  TChain *tree = new TChain("PreSelection");
  TChain *tree = new TChain("TreeMaker2/PreSelection");
  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }
  
  NtupleVariables::Init(tree,nameData);
  char outflName[100];
  sprintf(outflName,"%s_%s",dataset,outFileName);
  BookHistogram(outflName);
}

Bool_t bTagStudies::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t bTagStudies::LoadTree(Long64_t entry) {
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

bTagStudies::~bTagStudies() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

