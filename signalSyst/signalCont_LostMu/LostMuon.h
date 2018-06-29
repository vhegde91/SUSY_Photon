#ifndef LostMuon_H
#define LostMuon_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "NtupleVariables.h"
#include "TH1F.h"
#include "TH2.h"
#include "TH3.h"
#include <TH2Poly.h>
#include <TProfile.h>
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

class LostMuon : public NtupleVariables{

 public:
  LostMuon(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~LostMuon();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);
  TLorentzVector getBestPhoton();
  bool check_eMatchedtoGamma();
  double getGendRLepPho();
  void print(Long64_t);
  void findObjMatchedtoG(TLorentzVector);
  //Variables defined
  int bestPhotonIndxAmongPhotons=-100;
  double gendRLepPho=1000.;
  TLorentzVector bestPhoton;//(0.,0.,0.,0.);
  int getBinNoV4(int);
  int getBinNoV7(int);
    
  float HT_PtCut=30;
  float MHT_PtCut=30;//keep MHT_PtCut <= HT_PtCut and <= Njets_PtCut
  float Njets_PtCut=30;

  float HT_EtaCut=2.4;
  float MHT_EtaCut=5;
  float Njets_EtaCut=2.4;
  double wt=0,lumiInfb=35.86;//36.814;
  bool isSignal=false;
  //vector<double> HTBinLowEdge ={0,500,550,600,650,700,750,800,900,1000,1100,1200,1400,1600,1800,2000,2400,2800,3200};
  //  vector<double> HTBinLowEdge ={0,100,200,300,400,500,600,700,800,900,1000,1200,1500,2000,2500};
  //  vector<double> MHTBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,500};
  vector<double> STBinLowEdge ={0,300,360,420,500,600,700,850,1000,1200,1500,2000,2500,3000};
  //  vector<double> METBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,450,600,750,900,1200};
  //  vector<double> METBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,450,500};//org
  vector<double> METBinLowEdge={0,20,40,60,80,100,125,160,200,270,350,500,600};
  vector<double> METBinLowEdge2bJ={0,20,40,60,80,100,125,160,200,270,350,500};
  //  vector<double> METBinLowEdgeMidNJ={0,20,40,60,80,100,120,160,200,270,350};
  //  vector<double> METBinLowEdgeHighNJ={0,20,40,60,80,100,120,160,200,250};
  vector<double> METBinLowEdgeV4_njLow={0,100,125,160,200,270,350,450,750,900};//{0,100,200,270,350,450,750,900};
  vector<double> METBinLowEdgeV4={0,100,125,160,200,270,350,450,750};
  vector<double> METBinLowEdgeV7_njLow={0,100,200,270,350,450,750,900};
  vector<double> METBinLowEdgeV7={0,100,200,270,350,450,750};

  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  //vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,200,250,300,350,400,500,600};
  vector<double> nHadJLow0b={0,2,3,4,5,7,15};//0b
  vector<double> nHadJLow1b={0,2,5,7,15};//>=1b

  vector<double> METBinLowEdge5={0,100,150,5000};//0b,1b,>=2b
  /* vector<double> nHadJLow2={0,2,4,15}; */
  /* vector<double> METBinLowEdge5={0,100,150,5000}; */ //ob
    /* vector<double> METBinLowEdge5={0,100,150,5000};//=1b */
  /* vector<double> METBinLowEdge5={0,100,150,5000};//>=2b */
  vector<double> nHadJLow={0,2,3,4,5,15};

  vector<double> METBinLowEdge2={0,100,140,180,250,5000};//{0,100,140,180,5000};//{0,100,120,160,200,270,5000};
  vector<double> METBinLowEdge4={0,100,140,200,5000};
  // vector<double> dPhi1LowEdge={0,0.3,2,2.5,2.8,3.2};
  vector<double> dPhi1LowEdge={0,2.1, 2.7, 2.9, 3.2};
  vector<double> dPhi2LowEdge={0,0.9, 2.0, 2.6, 3.2};
  vector<double> STLowEdge={0,500,600,800,1500,5000};
  vector<double> STLowEdge2={0,500,550,600,720,5000};//{0,500,550,620,800,5000};
  vector<double> STLowEdge4={0,500,600,720,900,5000};//{0,500,550,620,800,5000};
  vector<double> MuPtLowEdge={0,10,25,45,100,1000};
  vector<double> MuActLowEdge={0,0.05,0.15,50};
  vector<double> jet1PtLowEdge={0,30,300,400,600,1500};

  vector<double> STInclLowEdge={0,500,10000};
  vector<double> METInclLowEdge={0,100,5000};
  vector<double> MET_NJ2or3LowEdge={0,100,140,200,5000};
  vector<double> MET_NJ4LowEdge={0,100,150,5000};
  vector<double> ST_NJ5or6LowEdge={0,500,700,10000};
  vector<double> ST_NJ7toInfLowEdge={0,500,800,10000};
  
  //histograms
  TH1I *h_RunNum;
  TH1D *h_intLumi;
  TH1D *h_ST;
  TH1D *h_MET;
  TH1D *h_nHadJets;
  TH1D *h_BTags;
  TH1D *h_METvBin;

  TH1D *h_ST_Mu0;
  TH1D *h_MET_Mu0;
  TH1D *h_nHadJets_Mu0;
  TH1D *h_BTags_Mu0;
  TH1D *h_METvBin_Mu0;
  TH1D *h_BestPhotonPt_Mu0;
  TH2D *h2_mGlmNLSP_Mu0_bin[31];
  //-----------------------------------------
  TH1D *h_ST_Mu1;
  TH1D *h_MET_Mu1;
  TH1D *h_nHadJets_Mu1;
  TH1D *h_BTags_Mu1;
  TH1D *h_METvBin_Mu1;
  TH1D *h_BestPhotonPt_Mu1;
  TH2D *h2_mGlmNLSP_Mu1_bin[31];

  TH1D *h_SBins_v7_Mu0;
  TH1D *h_SBins_v7_Mu1;
  TFile *oFile;
 
};
#endif

#ifdef LostMuon_cxx

void LostMuon::BookHistogram(const char *outFileName) {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
 
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);
  h_RunNum=new TH1I("runs","Run nos.",300000,0,300000);
  h_intLumi=new TH1D("intLumi","integrated luminosity in /fb",2500,25,50);

  h_ST=new TH1D("ST","ST",400,0,4000);
  h_MET=new TH1D("MET","MET",200,0,2000);
  h_nHadJets=new TH1D("nHadJets","no. of jets(only hadronic jets,not counting photon)",25,0,25);
  h_BTags=new TH1D("nBTags","no. of B tags",10,0,10);
  h_METvBin=new TH1D("METvarBin","MET with variable bin size",METBinLowEdgeV4_njLow.size()-1,&(METBinLowEdgeV4_njLow[0]));

  h_ST_Mu0=new TH1D("ST_Mu0","ST for 0 Muon events",400,0,4000);
  h_MET_Mu0=new TH1D("MET_Mu0","MET for 0 Muon events",200,0,2000);
  h_nHadJets_Mu0=new TH1D("nHadJets_Mu0","no. of jets(only hadronic jets,not counting photon)  for 0 Muon events",25,0,25);
  h_BTags_Mu0=new TH1D("nBTags_Mu0","no. of B tags for 0 Muon events",10,0,10);
  h_METvBin_Mu0=new TH1D("METvarBin_Mu0","MET with variable bin size for 0 Muon events",METBinLowEdgeV4_njLow.size()-1,&(METBinLowEdgeV4_njLow[0]));
  h_BestPhotonPt_Mu0=new TH1D("BestPhotonPt_Mu0","Pt of the best photon for 0 Muon events",150,0,1500);
  for(int i=1;i<=31;i++){
    TString aa="mGlmNLSP_Mu0_bin_"+to_string(i);
    h2_mGlmNLSP_Mu0_bin[i-1]=new TH2D(aa,"x:mass of gluino, y:mass of NLSP, z:number of events in bin XX for 0 Mu events",300,5,3005,300,5,3005);
  }
  //****************************************************
  h_ST_Mu1=new TH1D("ST_Mu1","ST for 1 Muon events",400,0,4000);
  h_MET_Mu1=new TH1D("MET_Mu1","MET for 1 Muon events",200,0,2000);
  h_nHadJets_Mu1=new TH1D("nHadJets_Mu1","no. of jets(only hadronic jets,not counting photon)  for 1 Muon events",25,0,25);
  h_BTags_Mu1=new TH1D("nBTags_Mu1","no. of B tags for 1 Muon events",10,0,10);
  h_METvBin_Mu1=new TH1D("METvarBin_Mu1","MET with variable bin size for 1 Muon events",METBinLowEdgeV4_njLow.size()-1,&(METBinLowEdgeV4_njLow[0]));
  h_BestPhotonPt_Mu1=new TH1D("BestPhotonPt_Mu1","Pt of the best photon for 1 Muon events",150,0,1500);
  for(int i=1;i<=31;i++){
    TString aa="mGlmNLSP_Mu1_bin_"+to_string(i);
    h2_mGlmNLSP_Mu1_bin[i-1]=new TH2D(aa,"x:mass of gluino, y:mass of NLSP, z:number of events in bin XX for 1 Mu events",300,5,3005,300,5,3005);
  }
  //---------------- search bins -----------------------
  h_SBins_v7_Mu0 = new TH1D("AllSBins_v7_Mu0","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] for 0 Mu events",31,0.5,31.5);
  h_SBins_v7_Mu1 = new TH1D("AllSBins_v7_Mu1","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] for 1 Mu events",31,0.5,31.5);
}


LostMuon::LostMuon(const TString &inputFileList, const char *outFileName, const char* dataset) {
  string nameData=dataset;//vvv
  TChain *tree = new TChain("PreSelection");
  tree = new TChain("TreeMaker2/PreSelection");//vvv
  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  if(nameData!="signalH") nameData="BG";
  if(nameData=="signalH") nameData="signal";
  cout<<"Treating the input files as "<<nameData<<" for setting tree branches"<<endl;
  NtupleVariables::Init(tree,nameData);

  BookHistogram(outFileName);
  
}

Bool_t LostMuon::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t LostMuon::LoadTree(Long64_t entry) {
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

LostMuon::~LostMuon() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

