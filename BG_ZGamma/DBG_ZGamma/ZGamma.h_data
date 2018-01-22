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
  bool isSignal=false;
  vector<double> METBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,450,500};
  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  vector <TLorentzVector> allBestPhotons;
  //histograms
  TH1I *h_RunNum;
  TH1D *h_intLumi;
  TH1D *h_passPhoPtTrg;
  TH1D *h_passHTxPhoPtTrg;

  TH1D *h_ST;
  TH1D *h_MET;
  TH1D *h_nHadJets;
  TH1D *h_BTags;
  TH1D *h_METvBin;
  TH1D *h_BestPhotonPt;
  TH1D *h_BestPhotonEta;
  TH1D *h_BestPhotonPhi;
  TH1D *h_BestPhotonSietaieta;

  TH1D *h_HT;
  TH1D *h_MHT;
  TH1D *h_nJets;
  TH1D *h_nVtx;
  TH1D *h_ZMass;

  TH1D *h_METstar;
  TH1D *h_METPhi;
  TH1D *h_METOrg;
  TH1D *h_METclean;
  TH1D *h_METPhiclean;

  TH1D *h_ST_2Mu;
  TH1D *h_MET_2Mu;
  TH1D *h_nHadJets_2Mu;
  TH1D *h_BTags_2Mu;
  TH1D *h_METvBin_2Mu;
  TH1D *h_BestPhotonPt_2Mu;
  TH1D *h_BestPhotonEta_2Mu;
  TH1D *h_BestPhotonPhi_2Mu;
  TH1D *h_ZMass_2Mu;

  TH1D *h_METstar_2Mu;
  TH1D *h_METPhi_2Mu;
  TH1D *h_METOrg_2Mu;
  TH1D *h_METclean_2Mu;
  TH1D *h_METPhiclean_2Mu;

  TH1D *h_Mu1Pt;
  TH1D *h_Mu2Pt;
  TH1D *h_Mu1Eta;
  TH1D *h_Mu2Eta;
  TH1D *h_Mu1Phi;
  TH1D *h_Mu2Phi;

  TH1D *h_isoMuonTracks_2Mu;
  TH1D *h_MuMultInJets_2Mu;

  TH1D *h_dPhi_PhoMET_2Mu;
  TH1D *h_dphi_METjet1_2Mu;
  TH1D *h_dphi_METjet2_2Mu;
  TH1D *h_Jet1Pt_2Mu;
  TH1D *h_Jet2Pt_2Mu;
  TH1D *h_isoEleTrack_2Mu;
  TH1D *h_isoMuTrack_2Mu;
  TH1D *h_isoPiTrack_2Mu;
  TH1D *h_Pho2Pt_2Mu;

  TH2D *h2_R_Mu1PtJetPtVsDR;
  TH2D *h2_Mu1PtMatchJetPt;
  //-----------------------------------------
  TH1D *h_ST_2Ele;
  TH1D *h_MET_2Ele;
  TH1D *h_nHadJets_2Ele;
  TH1D *h_BTags_2Ele;
  TH1D *h_METvBin_2Ele;
  TH1D *h_BestPhotonPt_2Ele;
  TH1D *h_BestPhotonEta_2Ele;
  TH1D *h_BestPhotonPhi_2Ele;
  TH1D *h_ZMass_2Ele;

  TH1D *h_METstar_2Ele;
  TH1D *h_METPhi_2Ele;
  TH1D *h_METOrg_2Ele;
  TH1D *h_METclean_2Ele;
  TH1D *h_METPhiclean_2Ele;

  TH1D *h_Ele1Pt;
  TH1D *h_Ele2Pt;
  TH1D *h_Ele1Eta;
  TH1D *h_Ele2Eta;
  TH1D *h_Ele1Phi;
  TH1D *h_Ele2Phi;

  TH1D *h_isoMuonTracks_2Ele;
  TH1D *h_MuMultInJets_2Ele;

  TH1D *h_dPhi_PhoMET_2Ele;
  TH1D *h_dphi_METjet1_2Ele;
  TH1D *h_dphi_METjet2_2Ele;
  TH1D *h_Jet1Pt_2Ele;
  TH1D *h_Jet2Pt_2Ele;
  TH1D *h_isoEleTrack_2Ele;
  TH1D *h_isoMuTrack_2Ele;
  TH1D *h_isoPiTrack_2Ele;
  TH1D *h_Pho2Pt_2Ele;
  TH2D *h2_R_Ele1PtJetPtVsDR;
  TH2D *h2_Ele1PtMatchJetPt;
  //-------------------------------------------------
  TH1D *h_ST_1E1Mu;
  TH1D *h_MET_1E1Mu;
  TH1D *h_nHadJets_1E1Mu;
  TH1D *h_BTags_1E1Mu;
  TH1D *h_METvBin_1E1Mu;
  TH1D *h_BestPhotonPt_1E1Mu;
  TH1D *h_BestPhotonEta_1E1Mu;
  TH1D *h_BestPhotonPhi_1E1Mu;
  TH1D *h_ZMass_1E1Mu;

  TH1D *h_METstar_1E1Mu;
  TH1D *h_METPhi_1E1Mu;
  TH1D *h_METOrg_1E1Mu;
  TH1D *h_METclean_1E1Mu;
  TH1D *h_METPhiclean_1E1Mu;

  TH1D *h_ElePt_1E1Mu;
  TH1D *h_EleEta_1E1Mu;
  TH1D *h_ElePhi_1E1Mu;
  TH1D *h_MuPt_1E1Mu;
  TH1D *h_MuEta_1E1Mu;
  TH1D *h_MuPhi_1E1Mu;

  TH1D *h_isoMuonTracks_1E1Mu;
  TH1D *h_MuMultInJets_1E1Mu;

  TH1D *h_dPhi_PhoMET_1E1Mu;
  TH1D *h_dphi_METjet1_1E1Mu;
  TH1D *h_dphi_METjet2_1E1Mu;
  TH1D *h_Jet1Pt_1E1Mu;
  TH1D *h_Jet2Pt_1E1Mu;
  TH1D *h_isoEleTrack_1E1Mu;
  TH1D *h_isoMuTrack_1E1Mu;
  TH1D *h_isoPiTrack_1E1Mu;
  TH1D *h_Pho2Pt_1E1Mu;

  TH1D *h_MET_OSSF_R[3];
  TH1D *h_MET_2Mu_R[3];
  TH1D *h_MET_2Ele_R[3];
  TH1D *h_MET_OSOF_R[3];
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
  h_RunNum=new TH1I("runs","Run nos.",300000,0,300000);
  h_intLumi=new TH1D("intLumi","integrated luminosity in /fb",2500,25,50);
  h_passPhoPtTrg=new TH1D("passPhoPtTrg","passes HLT_Photon165_HE10_v",4,-1.5,2.5);
  h_passHTxPhoPtTrg=new TH1D("passHTxPhoPtTrg","passes HLT_Photon90_CaloIdL_PFHT600_v",4,-1.5,2.5);

  h_ST=new TH1D("ST","ST",400,0,4000);
  h_MET=new TH1D("MET","MET",200,0,2000);
  h_nHadJets=new TH1D("nHadJets","no. of jets(only hadronic jets,not counting photon)",25,0,25);
  h_BTags=new TH1D("nBTags","no. of B tags",10,0,10);
  h_METvBin=new TH1D("METvarBin","MET with variable bin size",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_BestPhotonPt=new TH1D("BestPhotonPt","Pt of the best photon di-lepton(OS) events",150,0,1500);
  h_BestPhotonEta=new TH1D("BestPhotonEta","Eta of the best photon di-lepton(OS) events",120,-6,6);
  h_BestPhotonPhi=new TH1D("BestPhotonPhi","Phi of the best photon di-lepton(OS) events",80,-4,4);
  h_BestPhotonSietaieta=new TH1D("BestPhotonSietaieta","best photon #sigma i#eta i#eta",100,0,0.1);
  h_HT=new TH1D("HT","HT",400,0,4000);
  h_MHT=new TH1D("MHT","MHT",200,0,2000);
  h_nJets=new TH1D("nJets","nJets",25,0,25);
  h_nVtx=new TH1D("nVtx","no. of vertices",50,0,50);
  h_ZMass=new TH1D("ZMass","Inv mass of Z for 2 lep(e-e or mu-mu) events",500,0,500);

  h_METstar=new TH1D("METstar","MET* = MET+lepton",200,0,2000);
  h_METPhi=new TH1D("METPhi","METPhi* = METPhi after adding lepton",40,-4,4);
  h_METOrg=new TH1D("METOrg","MET before adding leptons",200,0,2000);
  h_METclean=new TH1D("METclean","MET clean",200,0,2000);
  h_METPhiclean=new TH1D("METPhiclean","METPhi clean",40,-4,4);

  h_ST_2Mu=new TH1D("ST_2Mu","ST di-Mu(OS) events",400,0,4000);
  h_MET_2Mu=new TH1D("MET_2Mu","MET di-Mu(OS) events",200,0,2000);
  h_nHadJets_2Mu=new TH1D("nHadJets_2Mu","no. of jets(only hadronic jets,not counting photon)  di-Mu(OS) events",25,0,25);
  h_BTags_2Mu=new TH1D("nBTags_2Mu","no. of B tags di-Mu(OS) events",10,0,10);
  h_METvBin_2Mu=new TH1D("METvarBin_2Mu","MET with variable bin size di-Mu(OS) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_BestPhotonPt_2Mu=new TH1D("BestPhotonPt_2Mu","Pt of the best photon di-Mu(OS) events",150,0,1500);
  h_BestPhotonEta_2Mu=new TH1D("BestPhotonEta_2Mu","Eta of the best photon di-Mu(OS) events",120,-6,6);
  h_BestPhotonPhi_2Mu=new TH1D("BestPhotonPhi_2Mu","Phi of the best photon di-Mu(OS) events",80,-4,4);
  h_ZMass_2Mu=new TH1D("ZMass_2Mu","Inv mass of Z for mu-mu events",500,0,500);

  h_METstar_2Mu=new TH1D("METstar_2Mu","MET* = MET+Muons",200,0,2000);
  h_METPhi_2Mu=new TH1D("METPhi_2Mu","METPhi* = METPhi after adding 2 muons",40,-4,4);
  h_METOrg_2Mu=new TH1D("METOrg_2Mu","MET before adding 2 muons",200,0,2000);
  h_METclean_2Mu=new TH1D("METclean_2Mu","MET clean for mu-mu events",200,0,2000);
  h_METPhiclean_2Mu=new TH1D("METPhiclean_2Mu","METPhi clean for mu-mu events",40,-4,4);

  h_Mu1Pt=new TH1D("Mu1Pt","leading Muon Pt",200,0,2000);
  h_Mu2Pt=new TH1D("Mu2Pt","2nd Muon Pt",200,0,2000);
  h_Mu1Eta=new TH1D("Mu1Eta","leading Muon Eta",120,-6,6);
  h_Mu2Eta=new TH1D("Mu2Eta","2nd leading Muon Eta",120,-6,6);
  h_Mu1Phi=new TH1D("Mu1Phi","Leading Muon Phi",80,-4,4);
  h_Mu2Phi=new TH1D("Mu2Phi","2nd Muon Phi",80,-4,4);

  h_isoMuonTracks_2Mu=new TH1D("nIsoMuonTracks_2Mu","no. of isolated muon tracks di-Mu(OS) events",20,0,20);

  h_dPhi_PhoMET_2Mu=new TH1D("dPhi_METBestPhoton_2Mu","dphi between MET and BestPhoton di-Mu(OS) events",40,0,4);
  h_dphi_METjet1_2Mu=new TH1D("dPhi_METjet1_Pho_2Mu","dphi between MET Vec and Jet1_Pho di-Mu(OS) events",40,0.,4);
  h_dphi_METjet2_2Mu=new TH1D("dPhi_METjet2_Pho_2Mu","dphi between MET Vec and Jet2_Pho di-Mu(OS) events",40,0.,4);
  h_Jet1Pt_2Mu=new TH1D("jet1Pt_2Mu","leading jet Pt di-Mu(OS) events",200,0,2000);
  h_Jet2Pt_2Mu=new TH1D("jet2Pt_2Mu","2nd leading jet Pt di-Mu(OS) events",200,0,2000);
  h_isoEleTrack_2Mu=new TH1D("isoEleTrack_2Mu","isolated electron tracks di-Mu(OS) events",5,0,5);
  h_isoMuTrack_2Mu=new TH1D("isoMuTrack_2Mu","isolated muon tracks di-Mu(OS) events",5,0,5);
  h_isoPiTrack_2Mu=new TH1D("isoPiTrack_2Mu","isolated pion tracks di-Mu(OS) events",5,0,5);
  h_Pho2Pt_2Mu=new TH1D("Pho2Pt_2Mu","2nd Photon Pt di-Mu(OS) events",200,0,2000);

  h2_R_Mu1PtJetPtVsDR=new TH2D("R_Mu1PtJetPtVsDR","y:Ratio of closest jet Pt to leading Mu Pt vs x: dR with closest jet",100,0,10,100,0,10);
  h2_Mu1PtMatchJetPt=new TH2D("Mu1PtMatchJetPt","x: Leading Muon Pt vs Y: closest jet Pt(dR<0.3)",200,0,2000,200,0,2000);
  //****************************************************
  h_ST_2Ele=new TH1D("ST_2Ele","ST for di-ele(OS) events",400,0,4000);
  h_MET_2Ele=new TH1D("MET_2Ele","MET for di-ele(OS) events",200,0,2000);
  h_nHadJets_2Ele=new TH1D("nHadJets_2Ele","no. of jets(only hadronic jets,not counting photon)  for di-ele(OS) events",25,0,25);
  h_BTags_2Ele=new TH1D("nBTags_2Ele","no. of B tags for di-ele(OS) events",10,0,10);
  h_METvBin_2Ele=new TH1D("METvarBin_2Ele","MET with variable bin size for di-ele(OS) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_BestPhotonPt_2Ele=new TH1D("BestPhotonPt_2Ele","Pt of the best photon for di-ele(OS) events",150,0,1500);
  h_BestPhotonEta_2Ele=new TH1D("BestPhotonEta_2Ele","Eta of the best photon for di-ele(OS) events",120,-6,6);
  h_BestPhotonPhi_2Ele=new TH1D("BestPhotonPhi_2Ele","Phi of the best photon for di-ele(OS) events",80,-4,4);
  h_ZMass_2Ele=new TH1D("ZMass_2Ele","Inv mass of Z for e-e events",500,0,500);

  h_METstar_2Ele=new TH1D("METstar_2Ele","MET* = MET+Electrons",200,0,2000);
  h_METPhi_2Ele=new TH1D("METPhi_2Ele","METPhi* = METPhi after adding 2 electrons",40,-4,4);
  h_METOrg_2Ele=new TH1D("METOrg_2Ele","MET before adding 2 electrons",200,0,2000);
  h_METclean_2Ele=new TH1D("METclean_2Ele","MET clean for di-ele(OS) events",200,0,2000);
  h_METPhiclean_2Ele=new TH1D("METPhiclean_2Ele","METPhi clean for di-ele(OS) events",40,-4,4);

  h_Ele1Pt=new TH1D("Ele1Pt","leading Electron Pt",200,0,2000);
  h_Ele2Pt=new TH1D("Ele2Pt","2nd Electron Pt",200,0,2000);
  h_Ele1Eta=new TH1D("Ele1Eta","leading Electron Eta",120,-6,6);
  h_Ele2Eta=new TH1D("Ele2Eta","2nd leading Electron Eta",120,-6,6);
  h_Ele1Phi=new TH1D("Ele1Phi","Leading Electron Phi",80,-4,4);
  h_Ele2Phi=new TH1D("Ele2Phi","2nd Electron Phi",80,-4,4);

  h_isoMuonTracks_2Ele=new TH1D("nIsoMuonTracks_2Ele","no. of isolated muon tracks for di-ele(OS) events",20,0,20);

  h_dPhi_PhoMET_2Ele=new TH1D("dPhi_METBestPhoton_2Ele","dphi between MET and BestPhoton for di-ele(OS) events",40,0,4);
  h_dphi_METjet1_2Ele=new TH1D("dPhi_METjet1_Pho_2Ele","dphi between MET Vec and Jet1_Pho for di-ele(OS) events",40,0,4);
  h_dphi_METjet2_2Ele=new TH1D("dPhi_METjet2_Pho_2Ele","dphi between MET Vec and Jet2_Pho for di-ele(OS) events",40,0,4);
  h_Jet1Pt_2Ele=new TH1D("jet1Pt_2Ele","leading jet Pt for di-ele(OS) events",200,0,2000);
  h_Jet2Pt_2Ele=new TH1D("jet2Pt_2Ele","2nd leading jet Pt for di-ele(OS) events",200,0,2000);
  h_isoEleTrack_2Ele=new TH1D("isoEleTrack_2Ele","isolated electron tracks for di-ele(OS) events",5,0,5);
  h_isoMuTrack_2Ele=new TH1D("isoMuTrack_2Ele","isolated muon tracks for di-ele(OS) events",5,0,5);
  h_isoPiTrack_2Ele=new TH1D("isoPiTrack_2Ele","isolated pion tracks for di-ele(OS) events",5,0,5);
  h_Pho2Pt_2Ele=new TH1D("Pho2Pt_2Ele","2nd Photon Pt di-ele(OS) events",200,0,2000);

  h2_R_Ele1PtJetPtVsDR=new TH2D("R_Ele1PtJetPtVsDR","y:Ratio of closest jet Pt to leading Ele Pt vs x: dR with closest jet",100,0,10,100,0,10);
  h2_Ele1PtMatchJetPt=new TH2D("Ele1PtMatchJetPt","x: Leading Electron Pt vs Y: closest jet Pt(dR<0.3)",200,0,2000,200,0,2000);
  //----------------------------------------------------
  h_ST_1E1Mu=new TH1D("ST_1E1Mu","ST for 1e 1Mu events",400,0,4000);
  h_MET_1E1Mu=new TH1D("MET_1E1Mu","MET for 1e 1Mu events",200,0,2000);
  h_nHadJets_1E1Mu=new TH1D("nHadJets_1E1Mu","no. of jets(only hadronic jets,not counting photon)  for 1e 1Mu events",25,0,25);
  h_BTags_1E1Mu=new TH1D("nBTags_1E1Mu","no. of B tags for 1e 1Mu events",10,0,10);
  h_METvBin_1E1Mu=new TH1D("METvarBin_1E1Mu","MET with variable bin size for 1e 1Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_BestPhotonPt_1E1Mu=new TH1D("BestPhotonPt_1E1Mu","Pt of the best photon for 1e 1Mu events",150,0,1500);
  h_BestPhotonEta_1E1Mu=new TH1D("BestPhotonEta_1E1Mu","Eta of the best photon for 1e 1Mu events",120,-6,6);
  h_BestPhotonPhi_1E1Mu=new TH1D("BestPhotonPhi_1E1Mu","Phi of the best photon for 1e 1Mu events",80,-4,4);
  h_ZMass_1E1Mu=new TH1D("ZMass_1E1Mu","Inv mass of e-mu ",500,0,500);

  h_METstar_1E1Mu=new TH1D("METstar_1E1Mu","MET* = MET+Electron+Muon",200,0,2000);
  h_METPhi_1E1Mu=new TH1D("METPhi_1E1Mu","METPhi* = METPhi after adding 1 electron, 1 muon",40,-4,4);
  h_METOrg_1E1Mu=new TH1D("METOrg_1E1Mu","MET before adding 1 electron and 1 muon",200,0,2000);
  h_METclean_1E1Mu=new TH1D("METclean_1E1Mu","MET clean for 1e 1Mu events",200,0,2000);
  h_METPhiclean_1E1Mu=new TH1D("METPhiclean_1E1Mu","METPhi clean for 1e 1Mu events",40,-4,4);

  h_MuPt_1E1Mu=new TH1D("MuPt_1E1Mu","Muon Pt for 1e 1Mu events",200,0,2000);
  h_MuEta_1E1Mu=new TH1D("MuEta_1E1Mu","Muon Eta for 1e 1Mu events",120,-6,6);
  h_MuPhi_1E1Mu=new TH1D("MuPhi_1E1Mu","Muon Phi for 1e 1Mu events",80,-4,4);

  h_ElePt_1E1Mu=new TH1D("ElePt_1E1Mu","Electron Pt for 1e 1Mu events",200,0,2000);
  h_EleEta_1E1Mu=new TH1D("EleEta_1E1Mu","Electron Eta for 1e 1Mu events",120,-6,6);
  h_ElePhi_1E1Mu=new TH1D("ElePhi_1E1Mu","Electron Phi for 1e 1Mu events",80,-4,4);

  h_isoMuonTracks_1E1Mu=new TH1D("nIsoMuonTracks_1E1Mu","no. of isolated muon tracks for 1e 1Mu events",20,0,20);

  h_dPhi_PhoMET_1E1Mu=new TH1D("dPhi_METBestPhoton_1E1Mu","dphi between MET and BestPhoton for 1e 1Mu events",40,0,4);
  h_dphi_METjet1_1E1Mu=new TH1D("dPhi_METjet1_Pho_1E1Mu","dphi between MET Vec and Jet1_Pho for 1e 1Mu events",40,0,4);
  h_dphi_METjet2_1E1Mu=new TH1D("dPhi_METjet2_Pho_1E1Mu","dphi between MET Vec and Jet2_Pho for 1e 1Mu events",40,0,4);
  h_Jet1Pt_1E1Mu=new TH1D("jet1Pt_1E1Mu","leading jet Pt for 1e 1Mu events",200,0,2000);
  h_Jet2Pt_1E1Mu=new TH1D("jet2Pt_1E1Mu","2nd leading jet Pt for 1e 1Mu events",200,0,2000);
  h_isoEleTrack_1E1Mu=new TH1D("isoEleTrack_1E1Mu","isolated electron tracks for 1e 1Mu events",5,0,5);
  h_isoMuTrack_1E1Mu=new TH1D("isoMuTrack_1E1Mu","isolated muon tracks for 1e 1Mu events",5,0,5);
  h_isoPiTrack_1E1Mu=new TH1D("isoPiTrack_1E1Mu","isolated pion tracks for 1e 1Mu events",5,0,5);
  h_Pho2Pt_1E1Mu=new TH1D("Pho2Pt_1E1Mu","2nd Photon Pt 1e 1Mu events",200,0,2000);
  //---------------- search bins -----------------------
  h_MET_OSSF_R[0] = new TH1D("MET_R1","MET: NJ=2to4, OSSF(ee or #mu#mu) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_OSSF_R[1] = new TH1D("MET_R2","MET: NJ=5or6, OSSF(ee or #mu#mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_OSSF_R[2] = new TH1D("MET_R3","MET: NJ>=7,   OSSF(ee or #mu#mu) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_MET_OSOF_R[0] = new TH1D("MET_OSOF_R1","MET: NJ=2to4, OSOF(1e 1#mu) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_OSOF_R[1] = new TH1D("MET_OSOF_R2","MET: NJ=5or6, OSOF(1e 1#mu) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_OSOF_R[2] = new TH1D("MET_OSOF_R3","MET: NJ>=7,   OSOF(1e 1#mu) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_MET_2Mu_R[0] = new TH1D("MET_2Mu_R1","MET: NJ=2to4, di-Mu(OS) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_2Mu_R[1] = new TH1D("MET_2Mu_R2","MET: NJ=5or6, di-Mu(OS) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_2Mu_R[2] = new TH1D("MET_2Mu_R3","MET: NJ>=7,   di-Mu(OS) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  
  h_MET_2Ele_R[0] = new TH1D("MET_2Ele_R1","MET: NJ=2to4, for di-ele(OS) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_2Ele_R[1] = new TH1D("MET_2Ele_R2","MET: NJ=5or6, for di-ele(OS) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_2Ele_R[2] = new TH1D("MET_2Ele_R3","MET: NJ>=7,   for di-ele(OS) events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  
}


ZGamma::ZGamma(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

