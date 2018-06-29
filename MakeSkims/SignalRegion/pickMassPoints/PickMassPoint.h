#ifndef PickMassPoint_H
#define PickMassPoint_H

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

class PickMassPoint : public NtupleVariables{

 public:
  //  PickMassPoint(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  PickMassPoint(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data",const char* mGl="0",const char* mNLSP="0");
  ~PickMassPoint();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  //  void     EventLoop(const char *,const char *);
  void     EventLoop(const char *,const char *,const double,const double);
  void     BookHistogram();
  void     storeBTagEff();   
  //Variables defined
  TH1F *h_selectBaselineYields_;
  TH1D *h_met0,*h_mht0,*h_st0,*h_ht0,*h_nj0,*h_nhadj,*h_phopt0,*h_nZFmNLSP,*h_nHFmNLSP,*h_nGFmNLSP;

  vector<float> xbins = {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,800,99999}, ybins = {0.0,0.8,1.6,2.4};
  TH2F *d_eff_b, *n_eff_b;
  TH2F *d_eff_c, *n_eff_c;
  TH2F *d_eff_udsg, *n_eff_udsg;

  TFile *oFile;
  
};
#endif

#ifdef PickMassPoint_cxx

void PickMassPoint::BookHistogram() {
  TH1::SetDefaultSumw2(1);
  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
  h_selectBaselineYields_ = new TH1F("selectBaselineYields_","selectBaselineYields_",10,-0.5,9.5);
  h_met0 = new TH1D("met0","MET before any selection",500,0,5000);
  h_mht0 = new TH1D("mht0","MHT before any selection",500,0,5000);
  h_st0 = new TH1D("st0","ST after best Photon selection",500,0,5000);
  h_ht0 = new TH1D("ht0","HT before any selection",500,0,5000);
  h_nj0 = new TH1D("nj0","NJ before any selection",50,0,50);
  h_nhadj = new TH1D("nhadj0","nHadjets after best Photon selection",50,0,50);
  h_phopt0 = new TH1D("phopt0","all photons Pt before any selection",500,0,5000);
  h_nZFmNLSP = new TH1D("nZFmNLSP","no. of Z from NLSP decay",5,-0.5,4.5);
  h_nHFmNLSP = new TH1D("nHFmNLSP","no. of H from NLSP decay",5,-0.5,4.5);
  h_nGFmNLSP = new TH1D("nGFmNLSP","no. of photon from NLSP decay",5,-0.5,4.5);

  d_eff_b = new TH2F("d_eff_b","d_eff_b",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_b = new TH2F("n_eff_b","n_eff_b",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  
  d_eff_c = new TH2F("d_eff_c","d_eff_c",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_c = new TH2F("n_eff_c","n_eff_c",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  
  d_eff_udsg = new TH2F("d_eff_udsg","d_eff_udsg",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_udsg = new TH2F("n_eff_udsg","n_eff_udsg",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0])); 

}


PickMassPoint::PickMassPoint(const TString &inputFileList, const char *outFileName, const char* dataset, const char* mGl, const char* mNLSP) {
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
  char outflName[2000];
  sprintf(outflName,"%s_%s_%s_%s",dataset,mGl,mNLSP,outFileName);
  oFile = new TFile(outflName, "recreate");
}

Bool_t PickMassPoint::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t PickMassPoint::LoadTree(Long64_t entry) {
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

PickMassPoint::~PickMassPoint() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  h_selectBaselineYields_->Write();
  h_met0->Write();
  h_mht0->Write();
  h_ht0->Write();
  h_nj0->Write();
  h_phopt0->Write();
  h_st0->Write();
  h_nhadj->Write();
  h_nZFmNLSP->Write();
  h_nHFmNLSP->Write();
  h_nGFmNLSP->Write();

  d_eff_b->Write();
  n_eff_b->Write();
  d_eff_c->Write();
  n_eff_c->Write();
  d_eff_udsg->Write();
  n_eff_udsg->Write();

  oFile->Write();
  oFile->Close();

}

#endif

