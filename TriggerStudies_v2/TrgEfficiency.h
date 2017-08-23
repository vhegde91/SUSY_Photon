#ifndef TrgEfficiency_H
#define TrgEfficiency_H

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

class TrgEfficiency : public NtupleVariables{

 public:
  TrgEfficiency(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~TrgEfficiency();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);
  TLorentzVector getBestPhoton();
  int bestPhotonIndxAmongPhotons = -100;   
  //Variables defined
  double intLumi = 35.86;
  vector<double> PhoPtBins={0,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350,360,370,380,400,450,500,550,600,700,900,1500};
  TH1D *h_intLumi;
  TH1D *h_trgNum;
  static const int nTrgs=10, nDenTrgs=4;
  vector<TString> trgNames={"HLT_PFMET90_PFMHT90_IDTight_v",
			    "HLT_PFMET100_PFMHT100_IDTight_v",
			    "HLT_PFMET110_PFMHT110_IDTight_v",
			    "HLT_PFMET120_PFMHT120_IDTight_v",
			    "HLT_Photon90_CaloIdL_PFHT600_v",
			    "HLT_Photon165_HE10_v",
			    "HLT_Ele15_IsoVVVL_PFHT350_v",
			    "HLT_Ele27_WPTight_Gsf_v",
			    "HLT_IsoTkMu24_v",
			    "HLT_Mu15_IsoVVVL_PFHT350_v"};
  vector<bool> trgDec;
    
  TH1D *h_ST[nTrgs];
  TH1D *h_MET[nTrgs];
  TH1D *h_nHadJets[nTrgs];
  TH1D *h_HT[nTrgs];
  TH1D *h_NJets[nTrgs];

  TH1D *h_PhotonPt[nTrgs];
  TH1D *h_PhotonEta[nTrgs];
  TH1D *h_PhotonPhi[nTrgs];

  TH1D *h_Mu1Pt[nTrgs];
  TH1D *h_Mu1Eta[nTrgs];
  TH1D *h_Mu1Phi[nTrgs];
  TH1D *h_Mu2Pt[nTrgs];
  TH1D *h_Mu2Eta[nTrgs];
  TH1D *h_Mu2Phi[nTrgs];

  TH1D *h_Ele1Pt[nTrgs];
  TH1D *h_Ele1Eta[nTrgs];
  TH1D *h_Ele1Phi[nTrgs];
  TH1D *h_Ele2Pt[nTrgs];
  TH1D *h_Ele2Eta[nTrgs];
  TH1D *h_Ele2Phi[nTrgs];

  TDirectory *TrgDir[nTrgs];
  TFile *oFile;
  
};
#endif

#ifdef TrgEfficiency_cxx

void TrgEfficiency::BookHistogram(const char *outFileName) {

  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);
  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
  
  h_intLumi = new TH1D("intLumi","integrated luminosity in /fb",2500,25,50); 
  h_trgNum = new TH1D("TrgNum","Trg numbers",200,-0.75,99.25);
  for(int i=0;i<nTrgs;i++){
    oFile->mkdir(trgNames[i]);
    oFile->cd(trgNames[i]);
    h_ST[i] = new TH1D("ST_"+trgNames[i],"ST_"+trgNames[i],500,0,5000);
    h_MET[i] = new TH1D("MET_"+trgNames[i],"MET_"+trgNames[i],200,0,2000);
    h_nHadJets[i] = new TH1D("nHadJets_"+trgNames[i],"nHadJets_"+trgNames[i],26,-0.5,25.5);
    h_HT[i] = new TH1D("HT_"+trgNames[i],"HT_"+trgNames[i],500,0,5000);
    h_NJets[i] = new TH1D("NJets_"+trgNames[i],"NJets_"+trgNames[i],26,-0.5,25.5);
    
    h_PhotonPt[i] = new TH1D("PhotonPt_"+trgNames[i],"PhotonPt_"+trgNames[i],200,0,2000);
    h_PhotonEta[i] = new TH1D("PhotonEta_"+trgNames[i],"PhotonEta_"+trgNames[i],120,-6,6);
    h_PhotonPhi[i] = new TH1D("PhotonPhi_"+trgNames[i],"PhotonPhi_"+trgNames[i],80,-4,4);

    h_Mu1Pt[i] = new TH1D("Mu1Pt_"+trgNames[i],"Mu1Pt_"+trgNames[i],200,0,2000);
    h_Mu1Eta[i] = new TH1D("Mu1Eta_"+trgNames[i],"Mu1Eta_"+trgNames[i],120,-6,6);
    h_Mu1Phi[i] = new TH1D("Mu1Phi_"+trgNames[i],"Mu1Phi_"+trgNames[i],80,-4,4);

    h_Mu2Pt[i] = new TH1D("Mu2Pt_"+trgNames[i],"Mu2Pt_"+trgNames[i],200,0,2000);
    h_Mu2Eta[i] = new TH1D("Mu2Eta_"+trgNames[i],"Mu2Eta_"+trgNames[i],120,-6,6);
    h_Mu2Phi[i] = new TH1D("Mu2Phi_"+trgNames[i],"Mu2Phi_"+trgNames[i],80,-4,4);

    h_Ele1Pt[i] = new TH1D("Ele1Pt_"+trgNames[i],"Ele1Pt_"+trgNames[i],200,0,2000);
    h_Ele1Eta[i] = new TH1D("Ele1Eta_"+trgNames[i],"Ele1Eta_"+trgNames[i],120,-6,6);
    h_Ele1Phi[i] = new TH1D("Ele1Phi_"+trgNames[i],"Ele1Phi_"+trgNames[i],80,-4,4);

    h_Ele2Pt[i] = new TH1D("Ele2Pt_"+trgNames[i],"Ele2Pt_"+trgNames[i],200,0,2000);
    h_Ele2Eta[i] = new TH1D("Ele2Eta_"+trgNames[i],"Ele2Eta_"+trgNames[i],120,-6,6);
    h_Ele2Phi[i] = new TH1D("Ele2Phi_"+trgNames[i],"Ele2Phi_"+trgNames[i],80,-4,4);
  }
 
}


TrgEfficiency::TrgEfficiency(const TString &inputFileList, const char *outFileName, const char* dataset) {
  string nameData=dataset;//vvv
  TChain *tree = new TChain("PreSelection");
  //  TChain *tree = new TChain("TreeMaker2/PreSelection");
  //if(nameData=="signalH") tree = new TChain("TreeMaker2/PreSelection");//vvv
  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  //if(nameData!="signalH") nameData="BG";
  //cout<<"Treating the input files as "<<nameData<<" for setting tree branches"<<endl;
  NtupleVariables::Init(tree,nameData);

  BookHistogram(outFileName);
}

Bool_t TrgEfficiency::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t TrgEfficiency::LoadTree(Long64_t entry) {
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

TrgEfficiency::~TrgEfficiency() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

