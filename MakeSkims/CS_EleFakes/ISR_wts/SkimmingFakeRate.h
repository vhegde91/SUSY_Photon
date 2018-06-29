#ifndef SkimmingFakeRate_H
#define SkimmingFakeRate_H

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

class SkimmingFakeRate : public NtupleVariables{

 public:
  SkimmingFakeRate(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~SkimmingFakeRate();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram();
  TLorentzVector getBestPhoton();
  double getISRWt();   
  //Variables defined
  vector<double> madhtbin={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350,360,370,380,390,400,410,420,430,440,450,460,470,480,490,500,510,520,530,540,550,560,570,580,590,600,610,620,630,640,650,660,670,680,690,700,710,720,730,740,750,760,770,780,790,800,810,820,830,840,850,860,870,880,890,900,910,920,930,940,950,960,970,980,990,1000,1010,1020,1030,1040,1050,1060,1070,1080,1090,1100,1110,1120,1130,1140,1150,1160,1170,1180,1190,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2200,2500,3000,5000};
  TH1F *h_selectBaselineYields_;
  TH1D *h_MET_NoCut;
  TH1D *h_HT_NoCut;
  TH1D *h_NJets_NoCut;
  TH1D *h_nISRJets_NoCut;
  TH1D *h_madHT_NoCut;
  TH1D *h_madHT_NoCut_vBin;
  TH1D *h_nISRJets_madHT600To800;

  TH1D *h_SumISRWts;
  TH1D *h_MET_NoCut_ISRWtd;
  TH1D *h_HT_NoCut_ISRWtd;
  TH1D *h_NJets_NoCut_ISRWtd;
  TH1D *h_nISRJets_NoCut_ISRWtd;
  TH1D *h_madHT_NoCut_ISRWtd;
  TH1D *h_madHT_NoCut_ISRWtd_vBin;
  TH1D *h_nISRJets_madHT600To800_ISRWtd;

  TH1D *h_MET_NoCut_ISRWtd_D;
  TH1D *h_HT_NoCut_ISRWtd_D;
  TH1D *h_NJets_NoCut_ISRWtd_D;
  TH1D *h_nISRJets_NoCut_ISRWtd_D;
  TH1D *h_madHT_NoCut_ISRWtd_D;
  TH1D *h_madHT_NoCut_ISRWtd_D_vBin;
  TH1D *h_nISRJets_madHT600To800_ISRWtd_D;

  TH1D *h_MET_NoCut_WptWtd;
  TH1D *h_HT_NoCut_WptWtd;
  TH1D *h_NJets_NoCut_WptWtd;

  TFile *oFile;
  
};
#endif

#ifdef SkimmingFakeRate_cxx

void SkimmingFakeRate::BookHistogram() {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
  h_selectBaselineYields_ = new TH1F("selectBaselineYields_","selectBaselineYields_",9,0.5,9.5);
  h_MET_NoCut=new TH1D("MET_NoCut","MET No Cut",200,0,2000);
  h_HT_NoCut=new TH1D("HT_NoCut","HT NoCut",500,0,5000);
  h_NJets_NoCut=new TH1D("NJets_NoCut","NJets No Cut",25,0,25);
  h_nISRJets_NoCut=new TH1D("nISRJets_NoCut","nISRJets No Cut",25,0,25);
  h_madHT_NoCut=new TH1D("madHT_NoCut","mad HT No Cut",3000,0,3000);
  h_madHT_NoCut_vBin=new TH1D("madHT_NoCut_vBin","mad HT No Cut vBins",madhtbin.size()-1,&(madhtbin[0]));
  h_nISRJets_madHT600To800=new TH1D("nISRJets_madHT600To800","nISRJets_madHT600To800",25,0,25);

  h_SumISRWts=new TH1D("SumISRWts","sum of ISR weights. = ISR wt for evt1 + ISR wt for evt2 + ... + ISR wt for evtN. N is number of events",4,0.5,3.5);

  h_MET_NoCut_ISRWtd=new TH1D("MET_NoCut_ISRWtd","MET No Cut ISRWtd",200,0,2000);
  h_HT_NoCut_ISRWtd=new TH1D("HT_NoCut_ISRWtd","HT NoCut_ISRWtd",500,0,5000);
  h_NJets_NoCut_ISRWtd=new TH1D("NJets_NoCut_ISRWtd","NJets No Cut ISRWtd",25,0,25);
  h_nISRJets_NoCut_ISRWtd=new TH1D("nISRJets_NoCut_ISRWtd","nISRJets No Cut ISRWtd",25,0,25);
  h_madHT_NoCut_ISRWtd=new TH1D("madHT_NoCut_ISRWtd","mad HT No Cut ISR Wtd",3000,0,3000);
  h_madHT_NoCut_ISRWtd_vBin=new TH1D("madHT_NoCut_ISRWtd_vBin","mad HT No Cut ISR Wtd vBins",madhtbin.size()-1,&(madhtbin[0]));
  h_nISRJets_madHT600To800_ISRWtd=new TH1D("nISRJets_madHT600To800_ISRWtd","nISRJets_madHT600To800_ISRWtd",25,0,25);

  h_MET_NoCut_ISRWtd_D=new TH1D("MET_NoCut_ISRWtd_D","MET No Cut ISRWtd_D",200,0,2000);
  h_HT_NoCut_ISRWtd_D=new TH1D("HT_NoCut_ISRWtd_D","HT NoCut_ISRWtd_D",500,0,5000);
  h_NJets_NoCut_ISRWtd_D=new TH1D("NJets_NoCut_ISRWtd_D","NJets No Cut ISRWtd_D",25,0,25);
  h_nISRJets_NoCut_ISRWtd_D=new TH1D("nISRJets_NoCut_ISRWtd_D","nISRJets No Cut ISRWtd_D",25,0,25);
  h_madHT_NoCut_ISRWtd_D=new TH1D("madHT_NoCut_ISRWtd_D","mad HT No Cut ISR Wtd",3000,0,3000);
  h_madHT_NoCut_ISRWtd_D_vBin=new TH1D("madHT_NoCut_ISRWtd_D_vBin","mad HT No Cut ISR Wtd vBins",madhtbin.size()-1,&(madhtbin[0]));
  h_nISRJets_madHT600To800_ISRWtd_D=new TH1D("nISRJets_madHT600To800_ISRWtd_D","nISRJets_madHT600To800_ISRWtd_D",25,0,25);

  h_MET_NoCut_WptWtd=new TH1D("MET_NoCut_WptWtd","MET No Cut WPt reweighted",200,0,2000);
  h_HT_NoCut_WptWtd=new TH1D("HT_NoCut_WptWtd","HT No Cut WPt reweighted",500,0,5000);
  h_NJets_NoCut_WptWtd=new TH1D("NJets_NoCut_WptWtd","NJets No Cut WPt reweighted",25,0,25);
}


SkimmingFakeRate::SkimmingFakeRate(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

Bool_t SkimmingFakeRate::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t SkimmingFakeRate::LoadTree(Long64_t entry) {
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

SkimmingFakeRate::~SkimmingFakeRate() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  h_selectBaselineYields_->Write();
  h_MET_NoCut->Write();
  h_HT_NoCut->Write();
  h_NJets_NoCut->Write();
  h_nISRJets_NoCut->Write();
  h_madHT_NoCut->Write();
  h_madHT_NoCut_vBin->Write();
  h_nISRJets_madHT600To800->Write();

  h_SumISRWts->Write();
  h_MET_NoCut_ISRWtd->Write();
  h_HT_NoCut_ISRWtd->Write();
  h_NJets_NoCut_ISRWtd->Write();
  h_nISRJets_NoCut_ISRWtd->Write();
  h_madHT_NoCut_ISRWtd->Write();
  h_madHT_NoCut_ISRWtd_vBin->Write();
  h_nISRJets_madHT600To800_ISRWtd->Write();

  h_MET_NoCut_ISRWtd_D->Write();
  h_HT_NoCut_ISRWtd_D->Write();
  h_NJets_NoCut_ISRWtd_D->Write();
  h_nISRJets_NoCut_ISRWtd_D->Write();
  h_madHT_NoCut_ISRWtd_D->Write();
  h_madHT_NoCut_ISRWtd_D_vBin->Write();
  h_nISRJets_madHT600To800_ISRWtd_D->Write();

  h_MET_NoCut_WptWtd->Write();
  h_HT_NoCut_WptWtd->Write();
  h_NJets_NoCut_WptWtd->Write();

  oFile->Write();
  oFile->Close();

}

#endif

