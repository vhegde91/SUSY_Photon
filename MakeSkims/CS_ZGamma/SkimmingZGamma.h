#ifndef SkimmingZGamma_H
#define SkimmingZGamma_H

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

class SkimmingZGamma : public NtupleVariables{

 public:
  SkimmingZGamma(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~SkimmingZGamma();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram();
  TLorentzVector getBestPhoton();
  void     storeBTagEff();
 
  //Variables defined
  TH1F *h_selectBaselineYields_;
  TH1D *h_LeadGenPhoPt;
  TH1D *h_LeadGenPhoPt_GenFilt;
  TH1D *h_LeadGenPhoMom;
  TH1D *h_LeadGenPhoMom_FdBstPho;
  TH1D *h_LeadGenPhoEta;
  TH1D *h_genMET;
  TH1D *h_nGenJets;

  TH1D *h_BestPhotonPt;
  TH1D *h_dR_PhoClstLep;
  TH1D *h_dphi_PhoLep1;
  TH1D *h_dphi_PhoLep2;
  TH1D *h_ZPt;
  TH1D *h_ZMass;

  TH1D *h_dR_PhoClstMu;
  TH1D *h_dphi_PhoMu1;
  TH1D *h_dphi_PhoMu2;

  TH1D *h_dR_PhoClstEle;
  TH1D *h_dphi_PhoEle1;
  TH1D *h_dphi_PhoEle2;

  vector<float> xbins = {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,800,99999}, ybins = {0.0,0.8,1.6,2.4};
  TH2F *d_eff_b, *n_eff_b;
  TH2F *d_eff_c, *n_eff_c;
  TH2F *d_eff_udsg, *n_eff_udsg;

  TFile *oFile;
  
};
#endif

#ifdef SkimmingZGamma_cxx

void SkimmingZGamma::BookHistogram() {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);

  char name[100],title[100];
  h_selectBaselineYields_ = new TH1F("selectBaselineYields_","selectBaselineYields_",10,-0.5,9.5);
  h_LeadGenPhoPt = new TH1D("LeadGenPhoPt","Leading gen photon Pt",2000,0,2000);
  h_LeadGenPhoPt_GenFilt= new TH1D("LeadGenPhoPt_GenFilt","Leading gen photon Pt after Gen Filter of G-Pt<130",2000,0,2000);
  h_LeadGenPhoMom=new TH1D("LeadGenPhoMom","PdgId of Mom of Leading Gen Photon",62,-0.75,30.25);
  h_LeadGenPhoMom_FdBstPho=new TH1D("LeadGenPhoMom_FdBstPho","PdgId of Mom of Leading Gen Photon after asking for a RECO photonPt>100GeV",62,-0.75,30.25);
  h_BestPhotonPt=new TH1D("BestPhotonPt","Pt of the best photon for dilepton events",150,0,1500);
  h_LeadGenPhoEta=new TH1D("LeadGenPhoEta","Leading gen Photon eta",2400,-6,6);
  h_genMET=new TH1D("genMET","gen MET",2000,0,2000);
  h_nGenJets=new TH1D("nGenJets","no. of gen jets, including photon",20,0,20);

  h_dR_PhoClstLep=new TH1D("dR_Pho_ClstLep","dR b/w photon and closest lepton",1000,0,10);
  h_dphi_PhoLep1=new TH1D("dPhi_PhoLep1","dphi b/w leading lepton and photon",40,0,4);
  h_dphi_PhoLep2=new TH1D("dPhi_PhoLep2","dphi b/w 2nd lepron and photon",40,0,4);
  h_ZPt=new TH1D("ZPt","ZPt for 2 lep events",2000,0,2000);
  h_ZMass=new TH1D("ZMass","Inv mass of Z for 2 lep(e-e or mu-mu) events",500,0,500);

  h_dR_PhoClstMu=new TH1D("dR_Pho_ClstMu","dR b/w photon and closest muon",1000,0,10);
  h_dphi_PhoMu1=new TH1D("dPhi_PhoMu1","dphi b/w leading muon and photon",40,0,4);
  h_dphi_PhoMu2=new TH1D("dPhi_PhoMu2","dphi b/w 2nd muon and photon",40,0,4);

  h_dR_PhoClstEle=new TH1D("dR_Pho_ClstEle","dR b/w photon and closest electron",1000,0,10);
  h_dphi_PhoEle1=new TH1D("dPhi_PhoEle1","dphi b/w leading electron and photon",40,0,4);
  h_dphi_PhoEle2=new TH1D("dPhi_PhoEle2","dphi b/w 2nd electron and photon",40,0,4);

  d_eff_b = new TH2F("d_eff_b","d_eff_b",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_b = new TH2F("n_eff_b","n_eff_b",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  
  d_eff_c = new TH2F("d_eff_c","d_eff_c",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_c = new TH2F("n_eff_c","n_eff_c",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  
  d_eff_udsg = new TH2F("d_eff_udsg","d_eff_udsg",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));
  n_eff_udsg = new TH2F("n_eff_udsg","n_eff_udsg",xbins.size()-1,&(xbins[0]), ybins.size()-1,&(ybins[0]));

}


SkimmingZGamma::SkimmingZGamma(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

Bool_t SkimmingZGamma::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t SkimmingZGamma::LoadTree(Long64_t entry) {
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

SkimmingZGamma::~SkimmingZGamma() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  h_selectBaselineYields_->Write();
  h_LeadGenPhoPt->Write();
  h_LeadGenPhoPt_GenFilt->Write();
  h_LeadGenPhoMom->Write();
  h_LeadGenPhoMom_FdBstPho->Write();

  h_BestPhotonPt->Write();
  h_dR_PhoClstLep->Write();
  h_dphi_PhoLep1->Write();
  h_dphi_PhoLep2->Write();
  h_ZPt->Write();
  h_ZMass->Write();

  h_dR_PhoClstMu->Write();
  h_dphi_PhoMu1->Write();
  h_dphi_PhoMu2->Write();

  h_dR_PhoClstEle->Write();
  h_dphi_PhoEle1->Write();
  h_dphi_PhoEle2->Write();

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

