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

  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  //vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,200,250,300,350,400,500,600};
  vector<double> nHadJLow0b={0,2,3,4,5,15};//0b
  vector<double> nHadJLow1b={0,2,5,15};//=1b
  vector<double> nHadJLowm2b={0,2,15};//>=2b
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
  TH1D *h_HT;
  TH1D *h_MHT;
  TH1D *h_nJets;
  TH1D *h_madHT;
  TH1D *h_nVtx;
  TH1D *h_minDR_Pho_GenObj;
  TH1D *h_madMinPhotonDeltaR;
  TH1D *h_mindRPhoLep;

  TH1D *h_nVtx_Mu0;
  TH1D *h_ST_Mu0;
  TH1D *h_MET_Mu0;
  TH1D *h_nHadJets_Mu0;
  TH1D *h_BTags_Mu0;
  TH1D *h_METvBin_Mu0;
  TH1D *h_BestPhotonPt_Mu0;
  TH1D *h_BestPhotonEta_Mu0;
  TH1D *h_BestPhotonPhi_Mu0;
  TH1D *h_mTpho_Mu0;

  TH1D *h_isoMuonTracks_Mu0;
  TH1D *h_MuMultInJets_Mu0;

  TH1D *h_nGenEle_Mu0,*h_nGenMu_Mu0,*h_nGenTau_Mu0;
  TH1D *h_GenMuons_MT2Activity_Mu0;

  TH1D *h_dPhi_PhoMET_Mu0;
  TH1D *h_dphi_METjet1_Mu0;
  TH1D *h_dphi_METjet2_Mu0;
  TH1D *h_Jet1Pt_Mu0;
  TH1D *h_Jet2Pt_Mu0;
  TH1D *h_isoEleTrack_Mu0;
  TH1D *h_isoMuTrack_Mu0;
  TH1D *h_isoPiTrack_Mu0;

  TH1D *h_GenMuEta_Mu0;
  TH1D *h_GenMuPt_Mu0;
  TH1D *h_nGenEleFmTau_Mu0;
  TH1D *h_nGenMuFmTau_Mu0;
  TH1D *h_genMuParent_Mu0;
  TH1D *h_photonPrompt_Mu0;
  TH1D *h_fakePhoPt_Mu0;
  TH1D *h_gendRLepPho_Mu0;
  TH1D *h_madMinPhotonDeltaR_Mu0;

  TH2D *h2_METnHadJ_Mu0;
  TH2D *h2_dPhi1nHadJ_Mu0;
  TH2D *h2_dPhi2nHadJ_Mu0;
  TH2D *h2_dPhi1MET_Mu0;
  TH2D *h2_STMET_Mu0;
  TH2D *h2_dPhi1dPhi2_Mu0;
  TH2D *h2_GenMuPtGenAct_Mu0;

  TH2D *h2_STHadJ_Mu0;
  TH2D *h2_METJet1Pt_Mu0;
  TH3D *h3_STMETnHadJ_Mu0;

  TH2D *h2_hadJbTag_Mu0;

  TH2D *h2_STMET_NJ2or3_Mu0;
  TH2D *h2_STMET_NJ4_Mu0;
  TH2D *h2_STMET_NJ5or6_Mu0;
  TH2D *h2_STMET_NJ7toInf_Mu0;

  TH2D *h2_METNJ_0b_Mu0;
  TH2D *h2_METNJ_1b_Mu0;
  TH2D *h2_METNJ_m2b_Mu0;
  TH2D *h2_R_PhoPtJetPtVsDR_Mu0;
  //-----------------------------------------
  TH1D *h_nVtx_Mu1;
  TH1D *h_ST_Mu1;
  TH1D *h_MET_Mu1;
  TH1D *h_nHadJets_Mu1;
  TH1D *h_BTags_Mu1;
  TH1D *h_METvBin_Mu1;
  TH1D *h_BestPhotonPt_Mu1;
  TH1D *h_BestPhotonEta_Mu1;
  TH1D *h_BestPhotonPhi_Mu1;
  TH1D *h_mTpho_Mu1;

  TH1D *h_isoMuonTracks_Mu1;
  TH1D *h_MuMultInJets_Mu1;
  TH1D *h_invMassPhoMu;

  TH1D *h_nGenEle_Mu1,*h_nGenMu_Mu1,*h_nGenTau_Mu1;
  TH1D *h_GenMuons_MT2Activity_Mu1;
  TH1D *h_GenMuPt_Mu1;
 
  TH1D *h_MuPt;
  TH1D *h_MuEta;
  TH1D *h_MuPhi;
  TH1D *h_mTmu;

  TH1D *h_dPhi_PhoMET_Mu1;
  TH1D *h_dphi_METjet1_Mu1;
  TH1D *h_dphi_METjet2_Mu1;
  TH1D *h_Jet1Pt_Mu1;
  TH1D *h_Jet2Pt_Mu1;
  TH1D *h_isoEleTrack_Mu1;
  TH1D *h_isoMuTrack_Mu1;
  TH1D *h_isoPiTrack_Mu1;
  TH1D *h_nGenEleFmTau_Mu1;
  TH1D *h_nGenMuFmTau_Mu1;
  TH1D *h_mTPhoMuMET;
  TH1D *h_invMassMuPhoNu;  

  TH2D *h2_STvsMuPt_Mu1;
  TH2D *h2_METvsMuPt_Mu1;
  TH2D *h2_GenActVsRECOAct;
  TH2D *h2_GenMuPtVsRECOMuPt;

  TH1D *h_dPhi_Muon_Photon;
   
  TH1D *h_PdgIdPhoParent;
  TH1D *h_GmatchedObj;
  TH1D *h_PdgIdMuParent;
  TH1D *h_MuMatchedObj;
  TH1D *h_photonPrompt_Mu1;
  TH1D *h_fakePhoPt_Mu1;
  TH1D *h_dR_MuPho;
  TH1D *h_gendRLepPho_Mu1;
  TH1D *h_madMinPhotonDeltaR_Mu1;
 
  TH2D *h2_METnHadJ_Mu1;
  TH2D *h2_dPhi1nHadJ_Mu1;
  TH2D *h2_dPhi2nHadJ_Mu1;
  TH2D *h2_dPhi1MET_Mu1;
  TH2D *h2_STMET_Mu1;
  TH2D *h2_RecoMuPtRecoAct_Mu1;
  TH2D *h2_dPhi1dPhi2_Mu1;

  TH2D *h2_STHadJ_Mu1;
  TH2D *h2_METJet1Pt_Mu1;
  TH3D *h3_STMETnHadJ_Mu1;

  TH2D *h2_hadJbTag_Mu1;

  TH2D *h2_STMET_NJ2or3_Mu1;
  TH2D *h2_STMET_NJ4_Mu1;
  TH2D *h2_STMET_NJ5or6_Mu1;
  TH2D *h2_STMET_NJ7toInf_Mu1;

  TH2D *h2_METNJ_0b_Mu1;
  TH2D *h2_METNJ_1b_Mu1;
  TH2D *h2_METNJ_m2b_Mu1;

  TH2D *h2_R_PhoPtJetPtVsDR_Mu1;
  TH2D *h2_R_MuPtJetPtVsDR;

  TH1D *h_temp;
  TH1D *h_MET_Mu0_R[3];
  TH1D *h_MET_Mu1_R[3];
  TH1D *h_MET_R_v2_Mu0[5];
  TH1D *h_MET_R_v2_Mu1[5];
  TH1D *h_SBins_Mu0;
  TH1D *h_SBins_Mu1;
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
  h_METvBin=new TH1D("METvarBin","MET with variable bin size",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_HT=new TH1D("HT","HT",400,0,4000);
  h_MHT=new TH1D("MHT","MHT",200,0,2000);
  h_nJets=new TH1D("nJets","nJets",25,0,25);
  h_madHT=new TH1D("madHT","madHT",400,0,4000);
  h_nVtx=new TH1D("nVtx","no. of vertices",50,0,50);
  h_madMinPhotonDeltaR=new TH1D("MadMinPhotonDeltaR","madMinPhotonDeltaR",1000,0,10);
  h_mindRPhoLep=new TH1D("mindRPhoLep","min dR(GenPho,lepton)",1000,0,10);

  h_nVtx_Mu0=new TH1D("nVtx_Mu0","no. of vertices for 0 Muon events",50,0,50);
  h_ST_Mu0=new TH1D("ST_Mu0","ST for 0 Muon events",400,0,4000);
  h_MET_Mu0=new TH1D("MET_Mu0","MET for 0 Muon events",200,0,2000);
  h_nHadJets_Mu0=new TH1D("nHadJets_Mu0","no. of jets(only hadronic jets,not counting photon)  for 0 Muon events",25,0,25);
  h_BTags_Mu0=new TH1D("nBTags_Mu0","no. of B tags for 0 Muon events",10,0,10);
  h_METvBin_Mu0=new TH1D("METvarBin_Mu0","MET with variable bin size for 0 Muon events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_BestPhotonPt_Mu0=new TH1D("BestPhotonPt_Mu0","Pt of the best photon for 0 Muon events",150,0,1500);
  h_BestPhotonEta_Mu0=new TH1D("BestPhotonEta_Mu0","Eta of the best photon for 0 Muon events",120,-6,6);
  h_BestPhotonPhi_Mu0=new TH1D("BestPhotonPhi_Mu0","Phi of the best photon for 0 Muon events",80,-4,4);
  h_mTpho_Mu0=new TH1D("mTPho_Mu0","mT b/w photon and MET for 0 Muon events",500,0,500);

  h_isoMuonTracks_Mu0=new TH1D("nIsoMuonTracks_Mu0","no. of isolated muon tracks for 0 Mu events",20,0,20);
  h_MuMultInJets_Mu0=new TH1D("MuMultInJets_Mu0","Muon multiplicity in jets for 0 Mu events",20,0,20);

  h_nGenEle_Mu0=new TH1D("nGenEle_Mu0","number of gen electrons, electrons from W/tau(tau's parent is W), for 0 Muon events",5,-0.5,4.5);
  h_nGenMu_Mu0=new TH1D("nGenMu_Mu0","number of gen muons, muons from W/tau(tau's parent is W), for 0 Muon events",5,-0.5,4.5);
  h_nGenTau_Mu0=new TH1D("nGenTau_Mu0","number of gen taus, taus from W, for 0 Muon events",5,-0.5,4.5);
  
  h_GenMuons_MT2Activity_Mu0=new TH1D("GenMuons_MT2Activity_Mu0","Gen MT2 activity for 0 Mu events",100,0,0.5);

  h_dPhi_PhoMET_Mu0=new TH1D("dPhi_METBestPhoton_Mu0","dphi between MET and BestPhoton for 0 Muon events",40,0,4);
  h_dphi_METjet1_Mu0=new TH1D("dPhi_METjet1_Pho_Mu0","dphi between MET Vec and Jet1_Pho for 0 Muon events",40,0.3,4.3);
  h_dphi_METjet2_Mu0=new TH1D("dPhi_METjet2_Pho_Mu0","dphi between MET Vec and Jet2_Pho for 0 Muon events",40,0.3,4.3);
  h_Jet1Pt_Mu0=new TH1D("jet1Pt_Mu0","leading jet Pt for 0 Mu events",200,0,2000);
  h_Jet2Pt_Mu0=new TH1D("jet2Pt_Mu0","2nd leading jet Pt for 0 Mu events",200,0,2000);
  h_isoEleTrack_Mu0=new TH1D("isoEleTrack_Mu0","isolated electron tracks for 0 Mu events",5,0,5);
  h_isoMuTrack_Mu0=new TH1D("isoMuTrack_Mu0","isolated muon tracks for 0 Mu events",5,0,5);
  h_isoPiTrack_Mu0=new TH1D("isoPiTrack_Mu0","isolated pion tracks for 0 Mu events",5,0,5);

  h_GenMuEta_Mu0=new TH1D("GenMuEta_Mu0","Eta of the Gen muon for 0 RECO Mu events. Gen Mus are coming from W/tau(tau's parent is W)",120,-6,6);
  h_GenMuPt_Mu0=new TH1D("GenMuPt_Mu0","Pt of the Gen Muon for 0 RECO Mu events. Gen Mus are coming from W/tau(tau's parent is W)",150,0,1500);
  h_nGenEleFmTau_Mu0=new TH1D("nGenEleFmTau_Mu0","no. of gen electrons from tau for 0 Mu events",5,0,5);
  h_nGenMuFmTau_Mu0=new TH1D("nGenMuFmTau_Mu0","no. of gen muons from tau for 0 Mu events",5,0,5);
  h_genMuParent_Mu0=new TH1D("genMuParent_Mu0","PdgID of gen muon's parent for 0 Mu events",51,-0.5,50.5);
  h_photonPrompt_Mu0=new TH1D("photonPrompt_Mu0","is Photon prompt(1) or non-prompt(0) for 0 Mu events",3,0,3);
  h_fakePhoPt_Mu0=new TH1D("fakePhoPt_Mu0","Pt of the fake photon for 0 Mu events",200,0,2000);
  h_gendRLepPho_Mu0=new TH1D("gendRLepPho_Mu0","Gen dR(#gamma,lepton) for 0 Mu events",100,0,10);
  h_madMinPhotonDeltaR_Mu0=new TH1D("madMinPhotonDeltaR_Mu0","Gen dR(#gamma,q) for 0 Mu events",100,0,10);

  h2_METnHadJ_Mu0=new TH2D("METnHadJ_Mu0","x: MET vs nHadJets for 0 Mu events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi1nHadJ_Mu0=new TH2D("dPhi1nHadJ_Mu0","x:dPhi MET and leading jet vs nHadJets for 0 Mu events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi2nHadJ_Mu0=new TH2D("dPhi2nHadJ_Mu0","x:dPhi MET and 2nd leading jet vs nHadJets for 0 Mu events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi1MET_Mu0=new TH2D("dPhi1MET_Mu0","x:dPhi MET and leading jet vs MET for 0 Mu events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),METBinLowEdge2.size()-1,&(METBinLowEdge2[0]));
  h2_STMET_Mu0=new TH2D("STMET_Mu0","x:ST vs MET for 0 Mu events",STLowEdge.size()-1,&(STLowEdge[0]),METBinLowEdge4.size()-1,&(METBinLowEdge4[0]));
  h2_dPhi1dPhi2_Mu0=new TH2D("dPhi1dPhi2_Mu0","x:dPhi1 vs dPhi2 for 0 Mu events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),dPhi2LowEdge.size()-1,&(dPhi2LowEdge[0]));
  h2_GenMuPtGenAct_Mu0=new TH2D("GenMuPtGenAct_Mu0","x:Pt of Gen Muon vs MT2 activity of Gen Muon",MuPtLowEdge.size()-1,&(MuPtLowEdge[0]),MuActLowEdge.size()-1,&(MuActLowEdge[0]));

  h2_STHadJ_Mu0=new TH2D("STHadJ_Mu0","x:ST vs HadJets for 0 Mu events",STLowEdge.size()-1,&(STLowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_METJet1Pt_Mu0=new TH2D("METJet1Pt_Mu0","x:MET vs leading JetPt for 0 Mu events",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),jet1PtLowEdge.size()-1,&(jet1PtLowEdge[0]));
  h3_STMETnHadJ_Mu0=new TH3D("STMETnHadJ_Mu0","x:ST,y:MET,z:nHadJets for 0 Mu events",STLowEdge2.size()-1,&(STLowEdge2[0]),METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),nHadJLow.size()-1,&(nHadJLow[0]));

  h2_hadJbTag_Mu0=new TH2D("hadJbTag_Mu0","x: hadJ vs BTag for 0 Mu events",1,0,100,1,0,100);

  h2_STMET_NJ2or3_Mu0 = new TH2D("STMET_Mu0_R1","x:ST vs MET for NJ=2 or 3 for 0 Mu events",STInclLowEdge.size()-1,&(STInclLowEdge[0]),MET_NJ2or3LowEdge.size()-1,&(MET_NJ2or3LowEdge[0]));
  h2_STMET_NJ4_Mu0 = new TH2D("STMET_Mu0_R2","x:ST vs MET for NJ=4 for 0 Mu events",STInclLowEdge.size()-1,&(STInclLowEdge[0]),MET_NJ4LowEdge.size()-1,&(MET_NJ4LowEdge[0]));
  h2_STMET_NJ5or6_Mu0 = new TH2D("STMET_Mu0_R3","x:ST vs MET for NJ=5 or 6 for 0 Mu events",ST_NJ5or6LowEdge.size()-1,&(ST_NJ5or6LowEdge[0]),METInclLowEdge.size()-1,&(METInclLowEdge[0]));
  h2_STMET_NJ7toInf_Mu0 = new TH2D("STMET_Mu0_R4","x:ST vs MET for NJ>=7 for 0 Mu events",ST_NJ7toInfLowEdge.size()-1,&(ST_NJ7toInfLowEdge[0]),METInclLowEdge.size()-1,&(METInclLowEdge[0]));

  h2_METNJ_0b_Mu0=new TH2D("METNJ_Mu0_R0","x:MET vs NJets for 0 b for 0 Mu events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow0b.size()-1,&(nHadJLow0b[0]));
  h2_METNJ_1b_Mu0=new TH2D("METNJ_Mu0_R1","x:MET vs NJets for 1 b for 0 Mu events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow1b.size()-1,&(nHadJLow1b[0]));
  h2_METNJ_m2b_Mu0=new TH2D("METNJ_Mu0_R2","x:MET vs NJets for >=2 b for 0 Mu events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLowm2b.size()-1,&(nHadJLowm2b[0]));

  h2_R_PhoPtJetPtVsDR_Mu0=new TH2D("R_PhoPtJetPtVsDR_Mu0","y:Ratio of closest jet Pt to Photon Pt vs x: dR with closest jet for 0 Mu events",100,0,10,100,0,10);
  //****************************************************
  h_nVtx_Mu1=new TH1D("nVtx_Mu1","no. of vertices for 1 Muon events",50,0,50);
  h_ST_Mu1=new TH1D("ST_Mu1","ST for 1 Muon events",400,0,4000);
  h_MET_Mu1=new TH1D("MET_Mu1","MET for 1 Muon events",200,0,2000);
  h_nHadJets_Mu1=new TH1D("nHadJets_Mu1","no. of jets(only hadronic jets,not counting photon)  for 1 Muon events",25,0,25);
  h_BTags_Mu1=new TH1D("nBTags_Mu1","no. of B tags for 1 Muon events",10,0,10);
  h_METvBin_Mu1=new TH1D("METvarBin_Mu1","MET with variable bin size for 1 Muon events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_BestPhotonPt_Mu1=new TH1D("BestPhotonPt_Mu1","Pt of the best photon for 1 Muon events",150,0,1500);
  h_BestPhotonEta_Mu1=new TH1D("BestPhotonEta_Mu1","Eta of the best photon for 1 Muon events",120,-6,6);
  h_BestPhotonPhi_Mu1=new TH1D("BestPhotonPhi_Mu1","Phi of the best photon for 1 Muon events",80,-4,4);
  h_mTpho_Mu1=new TH1D("mTPho_Mu1","mT b/w photon and MET for 1 Muon events",500,0,500);

  h_GenMuons_MT2Activity_Mu1=new TH1D("GenMuons_MT2Activity_Mu1","Gen MT2 activity for 1 Mu events",100,0,0.5);
  h_MuPt=new TH1D("MuPt","Muon Pt",150,0,1500);
  h_MuEta=new TH1D("MuEta","Muon eta",120,-6,6);
  h_MuPhi=new TH1D("MuPhi","Muon Phi",80,-4,4);
  h_mTmu=new TH1D("MT_Mu","MT for 1 Mu events",500,0,500);

  h_isoMuonTracks_Mu1=new TH1D("nIsoMuonTracks_Mu1","no. of isolated muon tracks for 1 Mu events",20,0,20);
  h_MuMultInJets_Mu1=new TH1D("MuMultInJets_Mu1","Muon multiplicity in jets for 1 Mu events",20,0,20);
  h_invMassPhoMu=new TH1D("invMassPhoMu","Inv mass of photon and muon",200,0,2000);

  h_dPhi_PhoMET_Mu1=new TH1D("dPhi_METBestPhoton_Mu1","dphi between MET and BestPhoton for 1 Muon events",40,0,4);
  h_dphi_METjet1_Mu1=new TH1D("dPhi_METjet1_Pho_Mu1","dphi between MET Vec and Jet1_Pho for 1 Muon events",40,0.3,4.3);
  h_dphi_METjet2_Mu1=new TH1D("dPhi_METjet2_Pho_Mu1","dphi between MET Vec and Jet2_Pho for 1 Muon events",40,0.3,4.3);
  h_dPhi_Muon_Photon=new TH1D("dPhi_Muon_Photon","dphi b/w muon and best photon",40,0,4);
  h_Jet1Pt_Mu1=new TH1D("jet1Pt_Mu1","leading jet Pt for 1 Mu events",200,0,2000);
  h_Jet2Pt_Mu1=new TH1D("jet2Pt_Mu1","2nd leading jet Pt for 1 Mu events",200,0,2000);
  h_isoEleTrack_Mu1=new TH1D("isoEleTrack_Mu1","isolated electron tracks for 1 Mu events",5,0,5);
  h_isoMuTrack_Mu1=new TH1D("isoMuTrack_Mu1","isolated muon tracks for 1 Mu events",5,0,5);
  h_isoPiTrack_Mu1=new TH1D("isoPiTrack_Mu1","isolated pion tracks for 1 Mu events",5,0,5);

  h_GenMuPt_Mu1=new TH1D("GenMuPt_Mu1","Pt of the Gen Muon for 1 RECO Mu events. Gen Mus are coming from W/tau(tau's parent is W)",150,0,1500);
  h_nGenEle_Mu1=new TH1D("nGenEle_Mu1","number of gen electrons, electrons from W/tau(tau's parent is W), for 1 Muon events",5,-0.5,4.5);
  h_nGenMu_Mu1=new TH1D("nGenMu_Mu1","number of gen muons, muons from W/tau(tau's parent is W), for 1 Muon events",5,-0.5,4.5);
  h_nGenTau_Mu1=new TH1D("nGenTau_Mu1","number of gen taus, taus from W, for 1 Muon events",5,-0.5,4.5);
  h_fakePhoPt_Mu1=new TH1D("fakePhoPt_Mu1","Pt of the fake photon for 1 Mu events",200,0,2000);
  h_dR_MuPho=new TH1D("dR_MuPho","dR(#mu,#gamma) for 1 Muon events",100,0,10);
  h_mTPhoMuMET=new TH1D("mTPhoMuMET","mT(pho+Mu,MET)",500,0,500);
  h_invMassMuPhoNu=new TH1D("invMassMuPhoNu","inv mass of mu, leading photon and neutrino",500,0,500);
  h_gendRLepPho_Mu1=new TH1D("gendRLepPho_Mu1","Gen dR(#gamma,lepton) for 1 Mu events",100,0,10);
  h_madMinPhotonDeltaR_Mu1=new TH1D("madMinPhotonDeltaR_Mu1","Gen dR(#gamma,q) for 1 Mu events",100,0,10);

  h2_STvsMuPt_Mu1=new TH2D("STvsMuPt_Mu1","x: Muon Pt vs ST for 1 Muon events",150,0,1500,400,0,4000);
  h2_METvsMuPt_Mu1=new TH2D("METvsMuPt_Mu1","x:Muon Pt vs MET for 1 Muon events",150,0,1500,200,0,2000);

  h2_GenActVsRECOAct=new TH2D("GenActVsRECOAct","x:Gen Muon MT2 activity vs RECO Muon MT2 activity",100,0,0.5,100,0,0.5);
  h2_GenMuPtVsRECOMuPt=new TH2D("GenMuPtVsRECOMuPt","x:Gen muon Pt vs RECO muon Pt",150,0,1500,150,0,1500);

  h_minDR_Pho_GenObj=new TH1D("minDR_Pho_GenObj","min DR b/w reco photon and gen objects",100,0,1);
  h_GmatchedObj=new TH1D("GmatchedObj","Gen Obj close to Reco-Gamma",62,-0.75,30.25);
  h_PdgIdPhoParent = new TH1D("PdgIdPhoParent","PdgID of the Photon's parent",62,-0.75,30.25);

  h_MuMatchedObj =new TH1D("MuMatchedObj","Gen Obj close to Reco-Muon",62,-0.75,30.25);
  h_PdgIdMuParent = new TH1D("PdgIdMuParent","PdgID of the muon's parent",62,-0.75,30.25);
  h_photonPrompt_Mu1=new TH1D("photonPrompt_Mu1","is Photon prompt(1) or non-prompt(0) for 1 Mu events",3,0,3);

  h_nGenEleFmTau_Mu1=new TH1D("nGenEleFmTau_Mu1","no. of gen electrons from tau for 1 Mu events",5,0,5);
  h_nGenMuFmTau_Mu1=new TH1D("nGenMuFmTau_Mu1","no. of gen muons from tau for 1 Mu events",5,0,5);

  h2_METnHadJ_Mu1=new TH2D("METnHadJ_Mu1","x: MET vs nHadJets for 1 Mu events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi1nHadJ_Mu1=new TH2D("dPhi1nHadJ_Mu1","x:dPhi MET and leading jet vs nHadJets for 1 Mu events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi2nHadJ_Mu1=new TH2D("dPhi2nHadJ_Mu1","x:dPhi MET and 2nd leading jet vs nHadJets for 1 Mu events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi1MET_Mu1=new TH2D("dPhi1MET_Mu1","x:dPhi MET and leading jet vs MET for 1 Mu events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),METBinLowEdge2.size()-1,&(METBinLowEdge2[0]));
  h2_STMET_Mu1=new TH2D("STMET_Mu1","x:ST vs MET for 1 Mu events",STLowEdge.size()-1,&(STLowEdge[0]),METBinLowEdge4.size()-1,&(METBinLowEdge4[0]));
  h2_dPhi1dPhi2_Mu1=new TH2D("dPhi1dPhi2_Mu1","x:dPhi1 vs dPhi2 for 1 Mu events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),dPhi2LowEdge.size()-1,&(dPhi2LowEdge[0]));
  h2_RecoMuPtRecoAct_Mu1=new TH2D("RecoMuPtRecoAct_Mu1","x:Pt of RECO Muon vs MT2 activity of RECO Muon",MuPtLowEdge.size()-1,&(MuPtLowEdge[0]),MuActLowEdge.size()-1,&(MuActLowEdge[0]));

  h2_STHadJ_Mu1=new TH2D("STHadJ_Mu1","x:ST vs HadJets for 1 Mu events",STLowEdge.size()-1,&(STLowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_METJet1Pt_Mu1=new TH2D("METJet1Pt_Mu1","x:MET vs leading JetPt for 1 Mu events",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),jet1PtLowEdge.size()-1,&(jet1PtLowEdge[0]));
  h3_STMETnHadJ_Mu1=new TH3D("STMETnHadJ_Mu1","x:ST,y:MET,z:nHadJets for 1 Mu events",STLowEdge2.size()-1,&(STLowEdge2[0]),METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),nHadJLow.size()-1,&(nHadJLow[0]));

  h2_hadJbTag_Mu1=new TH2D("hadJbTag_Mu1","x: hadJ vs BTag for 1 Mu events",1,0,100,1,0,100);

  h2_STMET_NJ2or3_Mu1 = new TH2D("STMET_Mu1_R1","x:ST vs MET for NJ=2 or 3 for 1 Mu events",STInclLowEdge.size()-1,&(STInclLowEdge[0]),MET_NJ2or3LowEdge.size()-1,&(MET_NJ2or3LowEdge[0]));
  h2_STMET_NJ4_Mu1 = new TH2D("STMET_Mu1_R2","x:ST vs MET for NJ=4 for 1 Mu events",STInclLowEdge.size()-1,&(STInclLowEdge[0]),MET_NJ4LowEdge.size()-1,&(MET_NJ4LowEdge[0]));
  h2_STMET_NJ5or6_Mu1 = new TH2D("STMET_Mu1_R3","x:ST vs MET for NJ=5 or 6 for 1 Mu events",ST_NJ5or6LowEdge.size()-1,&(ST_NJ5or6LowEdge[0]),METInclLowEdge.size()-1,&(METInclLowEdge[0]));
  h2_STMET_NJ7toInf_Mu1 = new TH2D("STMET_Mu1_R4","x:ST vs MET for NJ>=7 for 1 Mu events",ST_NJ7toInfLowEdge.size()-1,&(ST_NJ7toInfLowEdge[0]),METInclLowEdge.size()-1,&(METInclLowEdge[0]));
  h2_R_PhoPtJetPtVsDR_Mu1=new TH2D("R_PhoPtJetPtVsDR_Mu1","y:Ratio of closest jet Pt to Photon Pt vs x: dR with closest jet for 1 Mu events",100,0,10,100,0,10);

  h2_METNJ_0b_Mu1=new TH2D("METNJ_Mu1_R0","x:MET vs NJets for 0 b for 1 Mu events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow0b.size()-1,&(nHadJLow0b[0]));
  h2_METNJ_1b_Mu1=new TH2D("METNJ_Mu1_R1","x:MET vs NJets for 1 b for 1 Mu events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow1b.size()-1,&(nHadJLow1b[0]));
  h2_METNJ_m2b_Mu1=new TH2D("METNJ_Mu1_R2","x:MET vs NJets for >=2 b for 1 Mu events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLowm2b.size()-1,&(nHadJLowm2b[0]));

  h2_R_MuPtJetPtVsDR=new TH2D("R_MuPtJetPtVsDR","y:Ratio of closest jet Pt to Mu Pt vs x: dR with closest jet",100,0,10,100,0,10);

  h_temp=new TH1D("temp","temp",200,0,2000);

  //---------------- search bins -----------------------
  h_MET_Mu0_R[0] = new TH1D("MET_Mu0_R1","MET: NJ=2to4, for 0 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Mu0_R[1] = new TH1D("MET_Mu0_R2","MET: NJ=5or6, for 0 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Mu0_R[2] = new TH1D("MET_Mu0_R3","MET: NJ>=7,   for 0 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  
  h_MET_Mu1_R[0] = new TH1D("MET_Mu1_R1","MET: NJ=2to4, for 1 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Mu1_R[1] = new TH1D("MET_Mu1_R2","MET: NJ=5or6, for 1 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Mu1_R[2] = new TH1D("MET_Mu1_R3","MET: NJ>=7,   for 1 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  
  h_MET_R_v2_Mu0[0] =new TH1D("MET_R1_v2_Mu0","MET for 0b, NJ=2to4 for 0 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Mu0[1] =new TH1D("MET_R2_v2_Mu0","MET for 0b, NJ>=5 for 0 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Mu0[2] =new TH1D("MET_R3_v2_Mu0","MET for 1b, NJ=2to4 for 0 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Mu0[3] =new TH1D("MET_R4_v2_Mu0","MET for 1b, NJ>=5 for 0 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Mu0[4] =new TH1D("MET_R5_v2_Mu0","MET for b>=2 for 0 Mu events",METBinLowEdge2bJ.size()-1,&(METBinLowEdge2bJ[0]));  

  h_MET_R_v2_Mu1[0] =new TH1D("MET_R1_v2_Mu1","MET for 0b, NJ=2to4 for 1 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Mu1[1] =new TH1D("MET_R2_v2_Mu1","MET for 0b, NJ>=5 for 1 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Mu1[2] =new TH1D("MET_R3_v2_Mu1","MET for 1b, NJ=2to4 for 1 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Mu1[3] =new TH1D("MET_R4_v2_Mu1","MET for 1b, NJ>=5 for 1 Mu events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Mu1[4] =new TH1D("MET_R5_v2_Mu1","MET for b>=2 for 1 Mu events",METBinLowEdge2bJ.size()-1,&(METBinLowEdge2bJ[0]));  

  h_SBins_Mu0 = new TH1D("AllSBins_Mu0","all search bins:(0b, NJ=2to4)(0b, NJ>=5)(1b, NJ=2to4)(1b, NJ>=5)(b>=2) for 0 Mu events",34,0.5,34.5);
  h_SBins_Mu1 = new TH1D("AllSBins_Mu1","all search bins:(0b, NJ=2to4)(0b, NJ>=5)(1b, NJ=2to4)(1b, NJ>=5)(b>=2) for 1 Mu events",34,0.5,34.5);
}


LostMuon::LostMuon(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

