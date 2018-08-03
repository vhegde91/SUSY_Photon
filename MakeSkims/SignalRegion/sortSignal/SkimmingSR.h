#ifndef SkimmingSR_H
#define SkimmingSR_H

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

class SkimmingSR : public NtupleVariables{

 public:
  //  SkimmingSR(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  SkimmingSR(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~SkimmingSR();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char*);
  void     storeBTagEff();   
  //Variables defined

  vector<float> xbins = {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,800,99999}, ybins = {0.0,0.8,1.6,2.4};
  TH2F *d_eff_b[930], *n_eff_b[930];
  TH2F *d_eff_c[930], *n_eff_c[930];
  TH2F *d_eff_udsg[930], *n_eff_udsg[930];

  TTree *newTree2[930];
  TFile *oFile;
  
};
#endif
#ifdef SkimmingSR_cxx

void SkimmingSR::BookHistogram(const char* dataset) {
  TH1::SetDefaultSumw2(1);
  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
  int nP=0,mGl=0,mNLSP=0;
  TString dirName;
  TFile *fMass = TFile::Open("GGM_M1M3_MassScan.root");
  TH2D *h2=(TH2D*)fMass->Get("MGlMNLSP");
  char outflName[100];
  sprintf(outflName,"%s_Tree.root",dataset);
  oFile = new TFile(outflName, "recreate");
  for(int i=1;i<=h2->GetNbinsX();i++){
    mGl=h2->GetXaxis()->GetBinCenter(i);
    for(int j=1;j<=h2->GetNbinsY();j++){
      if(h2->GetBinContent(i,j) < 0.1) continue;
      mNLSP=h2->GetYaxis()->GetBinCenter(j);
      oFile->cd();
      dirName="GGM_M1M3_"+to_string(mGl)+"_"+to_string(mNLSP);
      oFile->mkdir(dirName);
      gDirectory->cd(dirName);
      d_eff_b[nP] = new TH2F("d_eff_b","d_eff_b",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
      n_eff_b[nP] = new TH2F("n_eff_b","n_eff_b",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  
      d_eff_c[nP] = new TH2F("d_eff_c","d_eff_c",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
      n_eff_c[nP] = new TH2F("n_eff_c","n_eff_c",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  
      d_eff_udsg[nP] = new TH2F("d_eff_udsg","d_eff_udsg",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
      n_eff_udsg[nP] = new TH2F("n_eff_udsg","n_eff_udsg",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0])); 
      
      //      newTree2[nP] = new TTree("PreSelection","PreSelection");
      nP++;
    }
  }
  cout<<"Number of directories needed:"<<nP<<endl;
}


SkimmingSR::SkimmingSR(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

  BookHistogram(dataset);
}

Bool_t SkimmingSR::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t SkimmingSR::LoadTree(Long64_t entry) {
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

SkimmingSR::~SkimmingSR() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  /* oFile->cd(); */
  /* d_eff_b->Write(); */
  /* n_eff_b->Write(); */
  /* d_eff_c->Write(); */
  /* n_eff_c->Write(); */
  /* d_eff_udsg->Write(); */
  /* n_eff_udsg->Write(); */

  oFile->Write();
  oFile->Close();

}

#endif

