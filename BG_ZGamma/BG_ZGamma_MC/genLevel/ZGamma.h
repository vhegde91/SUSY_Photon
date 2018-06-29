#ifndef ZGamma_H
#define ZGamma_H

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

class ZGamma : public NtupleVariables{

 public:
  ZGamma(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~ZGamma();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);
  TLorentzVector getBestPhoton();
  bool check_eMatchedtoGamma();
  bool check_muMatchedtoGamma();
  bool hasNeutrinoBrother(int,int);
  double getGendRLepPho();
  void print(Long64_t);
  void findObjMatchedtoG(TLorentzVector);
  //Variables defined
  int bestPhotonIndxAmongPhotons=-100;
  double gendRLepPho=1000.;
  float HT_PtCut=30;
  float MHT_PtCut=30;//keep MHT_PtCut <= HT_PtCut and <= Njets_PtCut
  float Njets_PtCut=30;

  float HT_EtaCut=2.4;
  float MHT_EtaCut=5;
  float Njets_EtaCut=2.4;
  double wt=0,lumiInfb=35.86;
  bool isSignal=false;
  vector<double> METBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,450,500};
  vector<double> METBinLowEdge2={0,100,150,200,350,5000};
  vector<double> STBinLowEdge={0,500,700,1000,10000};
  vector<double> nBTagsBinLowEdge={0,1,2,10};
  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  vector <TLorentzVector> allBestPhotons;
  //histograms
  TH1D *h_intLumi;
  TH1D *h_genLeadPhoPt;
  TH1D *h_genLeadPhoEtaP;
  TH1D *h_genLeadPhoEtaM;
  TH1D *h_genLeadPhoPhi;
  TH2D *h2_genLeadPhoPtEta;

  TH1D *h_genLeadPhoPt_PtG100;
  TH1D *h_genLeadPhoEtaP_PtG100;
  TH1D *h_genLeadPhoEtaM_PtG100;
  TH1D *h_genLeadPhoPhi_PtG100;
  TH2D *h2_genLeadPhoPtEta_PtG100;

  TH1D *h_nGenEle,*h_nGenMu,*h_nGenTau,*h_nLep;
  TH1D *h_nGenEle_PtG100,*h_nGenMu_PtG100,*h_nGenTau_PtG100,*h_nLep_PtG100;

  TH1D *h_genEle1Pt;
  TH1D *h_genEle1EtaP;
  TH1D *h_genEle1EtaM;
  TH1D *h_genEle1Phi;
  
  TH1D *h_genMu1Pt;
  TH1D *h_genMu1EtaP;
  TH1D *h_genMu1EtaM;
  TH1D *h_genMu1Phi;
  
  TH1D *h_genTau1Pt;
  TH1D *h_genTau1EtaP;
  TH1D *h_genTau1EtaM;
  TH1D *h_genTau1Phi;

  TH1D *h_genEle1Pt_PtG100;
  TH1D *h_genEle1EtaP_PtG100;
  TH1D *h_genEle1EtaM_PtG100;
  TH1D *h_genEle1Phi_PtG100;
  
  TH1D *h_genMu1Pt_PtG100;
  TH1D *h_genMu1EtaP_PtG100;
  TH1D *h_genMu1EtaM_PtG100;
  TH1D *h_genMu1Phi_PtG100;
  
  TH1D *h_genTau1Pt_PtG100;
  TH1D *h_genTau1EtaP_PtG100;
  TH1D *h_genTau1EtaM_PtG100;
  TH1D *h_genTau1Phi_PtG100;


  TH1D *h_madMinPhotonDeltaR;
  TH1D *h_MinPhoLepDR;

  TH1D *h_MET;
  TH1D *h_BTags;
  TH1D *h_METvBin;
  TH1D *h_HT;
  TH1D *h_MHT;
  TH1D *h_nJets;
  TH1D *h_nJetsClean;
  TH1D *h_invMass;

  TH1D *h_bParent_PdgId;
  TH1D *h_bStatus;
  TH1D *h_leadGenPhoParent;
  TH1D *h_GmatchedObj;
  TH1D *h_PdgIdPhoParent;

  TH1D *h_nGenbs;
  TFile *oFile;
 
};
#endif

#ifdef ZGamma_cxx

void ZGamma::BookHistogram(const char *outFileName) {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
 
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);

  h_intLumi=new TH1D("intLumi","Lumi in /fb",100,0,50);
  h_MET=new TH1D("MET","MET",200,0,2000);
  h_BTags=new TH1D("nBTags","no. of B tags",10,0,10);

  h_METvBin=new TH1D("METvarBin","MET with variable bin size",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_HT=new TH1D("HT","HT",400,0,4000);
  h_MHT=new TH1D("MHT","MHT",200,0,2000);
  h_nJets=new TH1D("nJets","nJets",25,0,25);
  h_nGenbs=new TH1D("nGenbs","no. of gen b-quarks",10,0,10);
  h_bParent_PdgId=new TH1D("bParent_PdgId","PdgId of b-quarks's parent",30,0,30);
  h2_genLeadPhoPtEta=new TH2D("genLeadPhoPtEta","x: leading gen Photon pt, eta of the photons",2000,0,2000,2000,-10,10);
  h_bStatus=new TH1D("bStatus","pythia status of b",150,0,150);
  h_leadGenPhoParent=new TH1D("leadGenPhoParent","Parent ID of leading gen Photon",30,0,30);
  h_invMass=new TH1D("invMass","inv mass of ll or nunu",150,0,150);

  h_GmatchedObj=new TH1D("GmatchedObj","Gen Obj close to Reco-Gamma",62,-0.75,30.25);
  h_PdgIdPhoParent = new TH1D("PdgIdPhoParent","PdgID of the Photon's parent",62,-0.75,30.25);

  h_nGenEle=new TH1D("nGenEle","no. of gen ele",5,0,5);
  h_nGenMu=new TH1D("nGenMu","no. of gen mu",5,0,5);
  h_nGenTau=new TH1D("nGenTau","no. of gen tau",5,0,5);
  h_nLep=new TH1D("nGenLep","no. of gen leptons",5,0,5);

  h_nGenEle_PtG100=new TH1D("nGenEle_PtG100","no. of gen ele (PhoPt>100)",5,0,5);
  h_nGenMu_PtG100=new TH1D("nGenMu_PtG100","no. of gen mu (PhoPt>100)",5,0,5);
  h_nGenTau_PtG100=new TH1D("nGenTau_PtG100","no. of gen tau (PhoPt>100)",5,0,5);
  h_nLep_PtG100=new TH1D("nGenLep_PtG100","no. of gen leptons (PhoPt>100)",5,0,5);

  h_genLeadPhoPt=new TH1D("genLeadPhoPt","leading gen photon Pt",200,0,2000);
  h_genLeadPhoEtaP=new TH1D("genLeadPhoEtaP","leading gen photon +Eta",600,0,6);
  h_genLeadPhoEtaM=new TH1D("genLeadPhoEtaM","leading gen photon -Eta",600,0,6);
  h_genLeadPhoPhi=new TH1D("genLeadPhoPhi","leading gen photon Phi",840,-4.2,4.2);

  h_genLeadPhoPt_PtG100=new TH1D("genLeadPhoPt_PtG100","leading gen photon Pt (PhoPt>100)",200,0,2000);
  h_genLeadPhoEtaP_PtG100=new TH1D("genLeadPhoEtaP_PtG100","leading gen photon +Eta (PhoPt>100)",600,0,6);
  h_genLeadPhoEtaM_PtG100=new TH1D("genLeadPhoEtaM_PtG100","leading gen photon -Eta (PhoPt>100)",600,0,6);
  h_genLeadPhoPhi_PtG100=new TH1D("genLeadPhoPhi_PtG100","leading gen photon Phi (PhoPt>100)",840,-4.2,4.2);
  h2_genLeadPhoPtEta_PtG100=new TH2D("genLeadPhoPtEta_PtG100","x: leading gen Photon pt, eta of the photons (PhoPt>100)",2000,0,2000,2000,-10,10);

  h_genEle1Pt=new TH1D("genEle1Pt","gen ele(1 ele events only) Pt",200,0,2000);
  h_genEle1EtaP=new TH1D("genEle1EtaP","gen ele(1 ele events only) +Eta",600,0,6);
  h_genEle1EtaM=new TH1D("genEle1EtaM","gen ele(1 ele events only) -Eta",600,0,6);
  h_genEle1Phi=new TH1D("genEle1Phi","gen ele(1 ele events only) Phi",840,-4.2,4.2);

  h_genMu1Pt=new TH1D("genMu1Pt","gen mu(1 mu events only) Pt",200,0,2000);
  h_genMu1EtaP=new TH1D("genMu1EtaP","gen mu(1 mu events only) +Eta",600,0,6);
  h_genMu1EtaM=new TH1D("genMu1EtaM","gen mu(1 mu events only) -Eta",600,0,6);
  h_genMu1Phi=new TH1D("genMu1Phi","gen mu(1 mu events only) Phi",840,-4.2,4.2);

  h_genTau1Pt=new TH1D("genTau1Pt","gen tau(1 tau events only) Pt",200,0,2000);
  h_genTau1EtaP=new TH1D("genTau1EtaP","gen tau(1 tau events only) +Eta",600,0,6);
  h_genTau1EtaM=new TH1D("genTau1EtaM","gen tau(1 tau events only) -Eta",600,0,6);
  h_genTau1Phi=new TH1D("genTau1Phi","gen tau(1 tau events only) Phi",840,-4.2,4.2);

  h_genEle1Pt_PtG100=new TH1D("genEle1Pt_PtG100","gen ele(1 ele events only) Pt (PhoPt>100)",200,0,2000);
  h_genEle1EtaP_PtG100=new TH1D("genEle1EtaP_PtG100","gen ele(1 ele events only) +Eta (PhoPt>100)",600,0,6);
  h_genEle1EtaM_PtG100=new TH1D("genEle1EtaM_PtG100","gen ele(1 ele events only) -Eta (PhoPt>100)",600,0,6);
  h_genEle1Phi_PtG100=new TH1D("genEle1Phi_PtG100","gen ele(1 ele events only) Phi (PhoPt>100)",840,-4.2,4.2);

  h_genMu1Pt_PtG100=new TH1D("genMu1Pt_PtG100","gen mu(1 mu events only) Pt (PhoPt>100)",200,0,2000);
  h_genMu1EtaP_PtG100=new TH1D("genMu1EtaP_PtG100","gen mu(1 mu events only) +Eta (PhoPt>100)",600,0,6);
  h_genMu1EtaM_PtG100=new TH1D("genMu1EtaM_PtG100","gen mu(1 mu events only) -Eta (PhoPt>100)",600,0,6);
  h_genMu1Phi_PtG100=new TH1D("genMu1Phi_PtG100","gen mu(1 mu events only) Phi (PhoPt>100)",840,-4.2,4.2);

  h_genTau1Pt_PtG100=new TH1D("genTau1Pt_PtG100","gen tau(1 tau events only) Pt (PhoPt>100)",200,0,2000);
  h_genTau1EtaP_PtG100=new TH1D("genTau1EtaP_PtG100","gen tau(1 tau events only) +Eta (PhoPt>100)",600,0,6);
  h_genTau1EtaM_PtG100=new TH1D("genTau1EtaM_PtG100","gen tau(1 tau events only) -Eta (PhoPt>100)",600,0,6);
  h_genTau1Phi_PtG100=new TH1D("genTau1Phi_PtG100","gen tau(1 tau events only) Phi (PhoPt>100)",840,-4.2,4.2);
}


ZGamma::ZGamma(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

Bool_t ZGamma::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t ZGamma::LoadTree(Long64_t entry) {
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

ZGamma::~ZGamma() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

