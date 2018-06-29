#ifndef bTagSF_Hists_H
#define bTagSF_Hists_H

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

class bTagSF_Hists : public NtupleVariables{

 public:
  bTagSF_Hists(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~bTagSF_Hists();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);
  TString dirName;
  TDirectory *dir;
  vector<float> xbins = {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,800,99999}, ybins = {0.0,0.8,1.6,2.4};
  //Variables defined
  TH2F *d_eff_b, *n_eff_b;
  TH2F *d_eff_c, *n_eff_c;
  TH2F *d_eff_udsg, *n_eff_udsg;

  TFile *oFile;
  
};
#endif

#ifdef bTagSF_Hists_cxx

void bTagSF_Hists::BookHistogram(const char *outFileName) {
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);
  char name[100],title[100];

  oFile = new TFile(outFileName, "recreate");
  dir = oFile->mkdir(dirName);
  dir->cd();

  d_eff_b = new TH2F("d_eff_b","d_eff_b",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_b = new TH2F("n_eff_b","n_eff_b",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));

  d_eff_c = new TH2F("d_eff_c","d_eff_c",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_c = new TH2F("n_eff_c","n_eff_c",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));

  d_eff_udsg = new TH2F("d_eff_udsg","d_eff_udsg",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_udsg = new TH2F("n_eff_udsg","n_eff_udsg",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  
}


bTagSF_Hists::bTagSF_Hists(const TString &inputFileList, const char *outFileName, const char* dataset) {
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
  dirName = dataset;
  BookHistogram(outflName);
}

Bool_t bTagSF_Hists::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t bTagSF_Hists::LoadTree(Long64_t entry) {
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

bTagSF_Hists::~bTagSF_Hists() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  dir->cd();
  oFile->Write();
  oFile->Close();

}

#endif

