#ifndef SignalSyst_H
#define SignalSyst_H

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

class SignalSyst : public NtupleVariables{

 public:
  SignalSyst(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data",const char* mGl="0",const char* mNLSP="0");
  ~SignalSyst();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *,const double,const double);
  void     BookHistogram(const char *);
  TLorentzVector getBestPhoton();
  int getBinNoV7(int,double);

  bool check_eMatchedtoGamma();
  void print(Long64_t);
  //Variables defined
  int bestPhotonIndxAmongPhotons=-100;
  float HT_PtCut=30;
  float MHT_PtCut=30;//keep MHT_PtCut <= HT_PtCut and <= Njets_PtCut
  float Njets_PtCut=30;

  float HT_EtaCut=2.4;
  float MHT_EtaCut=5;
  float Njets_EtaCut=2.4;
  double wt=0,lumiInfb=35.86;
  vector<double> STBinLowEdge ={0,300,360,420,500,600,700,850,1000,1200,1500,2000,2500,3000};
  vector<double> METBinLowEdge={0,100,125,160,200,270,350,450,750,900};
  vector<double> METBinLowEdge2={0,20,40,60,80,100,120,160,200,270,350,450,500};

  vector<double> METBinLowEdgeV4_njLow={0,100,125,160,200,270,350,450,750,900};//{0,100,200,270,350,450,750,900}; 
  vector<double> METBinLowEdgeV4={0,100,125,160,200,270,350,450,750};
  vector<double> METBinLowEdgeV7_njLow={0,100,200,270,350,450,750,900};
  vector<double> METBinLowEdgeV7={0,100,200,270,350,450,750};

  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  vector<double> METLowSimple={0,100,200,15000};

  vector<double> nJBinLow={0,2,5,7,20};
  vector<double> STBinLow2={0,400,500,600,700,800,950,1200,1500,1800,2400,3000,4000,15000};
  vector<double> dPhiBinLow={0,0.3,0.6,2,2.3,2.6,2.9,3.15};
  vector<TLorentzVector> allBestPhotons;
  //histograms
  TH1I *h_RunNum;
  TH1D *h_intLumi;
  TH1D *h_madHT;

  TH1D *h_ST_CD;
  TH1D *h_MET_CD;
  TH1D *h_nHadJets_CD;
  TH1D *h_BTags_CD;
  TH1D *h_nVtx_CD;
  TH1D *h_GenMET_CD;
  TH1D *h_METPhi_CD;

  TH1D *h_METvBin_CD;
  TH1D *h_SBins_v7_CD,*h_SBins_v7_genMET_CD,*h_SBins_v7_JECup_CD,*h_SBins_v7_JECdn_CD,*h_SBins_v7_ISRup_CD,*h_SBins_v7_ISRdn_CD,*h_SBins_v7_JERup_CD,*h_SBins_v7_JERdn_CD;

  TH1D *h_mindPhi1dPhi2_CD;

  TH1D *h_BestPhotonPt_CD;

  TH1D *h_ISRCorrFactors;
  TH1D *h_cutFlow;

  TH2D *h2_SBins_v7_CD_vs_ScaleIdx;

  TFile *oFile;
  
};
#endif

#ifdef SignalSyst_cxx

void SignalSyst::BookHistogram(const char *outFileName) {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
 
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);
  h_RunNum=new TH1I("runs","Run nos.",300000,0,300000);
  h_intLumi=new TH1D("intLumi","integrated luminosity in /fb",2500,25,50); 

  h_ISRCorrFactors = new TH1D("ISRCorrFactors","Bin 0: No ISR weights, Bin 1: ISR wts, 2: ISR wt + unc, 3: ISR wt - unc",5,-0.5,4.5);

  h_ST_CD=new TH1D("ST_CD","ST_CD",400,0,4000);
  h_MET_CD=new TH1D("MET_CD","MET_CD",200,0,2000);
  h_nHadJets_CD=new TH1D("nHadJets_CD","no. of jets(only hadronic jets,not counting photon)_CD",25,0,25);
  h_BTags_CD=new TH1D("nBTags_CD","no. of B tags_CD",10,0,10);
  h_nVtx_CD=new TH1D("nVtx_CD","no. of priary vertices_CD",50,0,50);
  h_GenMET_CD=new TH1D("GenMET_CD","GenMET_CD",200,0,2000);
  h_METPhi_CD=new TH1D("METPhi_CD","METPhi_CD",40,0,4);

  h_BestPhotonPt_CD=new TH1D("BestPhotonPt_CD","Pt of the Best Photon_CD",150,0,1500);
  h_METvBin_CD=new TH1D("METvarBin_CD","MET with variable bin size_CD",METBinLowEdgeV4_njLow.size()-1,&(METBinLowEdgeV4_njLow[0]));

  h_mindPhi1dPhi2_CD=new TH1D("mindPhi1dPhi2_CD","min(#Delta#Phi1,#Delta#Phi2)_CD",400,0,4);
  
  h_cutFlow=new TH1D("cutFlow","cut flow 0:all events, 1:passed 100GeV photon, 2:passed e/mu veto, 3:passed iso track veto, 4:passed PtRatio, 5:passed trigger like cuts, 6:passed dPhi and MET",11,-0.5,10.5);

  h2_SBins_v7_CD_vs_ScaleIdx = new TH2D("SBins_v7_CD_ScaleIdx","x:search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD, y: scale weight index",31,0.5,31.5,12,-0.5,11.5);
  h_madHT=new TH1D("madHT","madHT",300,0,3000);
  //---------------Search Bins ----------------------------
  h_SBins_v7_CD = new TH1D("AllSBins_v7_CD","search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);
  h_SBins_v7_ISRup_CD = new TH1D("AllSBins_v7_ISRup_CD","ISR up, search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);
  h_SBins_v7_ISRdn_CD = new TH1D("AllSBins_v7_ISRdn_CD","ISR down, search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);
  h_SBins_v7_genMET_CD = new TH1D("AllSBins_v7_genMET_CD","GenMET, search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);
  h_SBins_v7_JECup_CD = new TH1D("AllSBins_v7_JECup_CD","JECup, search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);
  h_SBins_v7_JECdn_CD = new TH1D("AllSBins_v7_JECdn_CD","JECdn, search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);
  h_SBins_v7_JERup_CD = new TH1D("AllSBins_v7_JERup_CD","JERup, search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);
  h_SBins_v7_JERdn_CD = new TH1D("AllSBins_v7_JERdn_CD","JERdn, search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);


}


SignalSyst::SignalSyst(const TString &inputFileList, const char *outFileName, const char* dataset, const char* mGl, const char* mNLSP) {
  string nameData=dataset;//vvv
  TChain *tree = new TChain("PreSelection");
  if(nameData=="signalH") tree = new TChain("TreeMaker2/PreSelection");//vvv
  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  if(nameData!="signalH") nameData="BG";
  if(nameData=="signalH") nameData="signal";
  cout<<"Treating the input files as "<<nameData<<" for setting tree branches"<<endl;
  NtupleVariables::Init(tree,nameData);

  char outflName[5000];
  sprintf(outflName,"%s_%s_%s_%s",dataset,mGl,mNLSP,outFileName);
  BookHistogram(outflName);
  
}

Bool_t SignalSyst::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t SignalSyst::LoadTree(Long64_t entry) {
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

SignalSyst::~SignalSyst() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

