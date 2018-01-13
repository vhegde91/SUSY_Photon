#ifndef LostEle_H
#define LostEle_H

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

class LostEle : public NtupleVariables{

 public:
  LostEle(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~LostEle();
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
  vector<double> METBinLowEdge={0,20,40,60,80,100,125,150,200,250,350,500,600};
  vector<double> METBinLowEdge2bJ={0,20,40,60,80,100,125,150,200,250,350,500};
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

  vector<double> METBinLowEdge5={0,100,150,5000};//0b,>=1b
 
  vector<double> nHadJLow={0,2,4,6,15};
  //  vector<double> nHadJLow={0,2,3,4,6,15};
  vector<double> nHadJLow2={0,2,4,15};
  vector<double> METBinLowEdge2={0,100,140,180,250,5000};//{0,100,140,180,5000};//{0,100,120,160,200,270,5000};
  vector<double> METBinLowEdge4={0,100,140,200,5000};
  // vector<double> dPhi1LowEdge={0,0.3,2,2.5,2.8,3.2};
  vector<double> dPhi1LowEdge={0,2.1, 2.7, 2.9, 3.2};
  vector<double> dPhi2LowEdge={0,0.9, 2.0, 2.6, 3.2};
  vector<double> STLowEdge={0,500,600,800,1500,5000};
  vector<double> STLowEdge2={0,500,550,600,720,5000};//{0,500,550,620,800,5000};
  vector<double> STLowEdge4={0,500,600,720,900,5000};//{0,500,550,620,800,5000};
  vector<double> ElePtLowEdge={0,10,25,45,100,1000};
  vector<double> EleActLowEdge={0,0.05,0.15,50};
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

  TH1D *h_nVtx_Ele0;
  TH1D *h_ST_Ele0;
  TH1D *h_MET_Ele0;
  TH1D *h_nHadJets_Ele0;
  TH1D *h_BTags_Ele0;
  TH1D *h_METvBin_Ele0;
  TH1D *h_BestPhotonPt_Ele0;
  TH1D *h_BestPhotonEta_Ele0;
  TH1D *h_BestPhotonPhi_Ele0;
  TH1D *h_mTpho_Ele0;

  TH1D *h_EleMultInJets_Ele0;

  TH1D *h_nGenEle_Ele0,*h_nGenMu_Ele0,*h_nGenTau_Ele0;
  TH1D *h_GenElectrons_MT2Activity_Ele0;

  TH1D *h_dPhi_PhoMET_Ele0;
  TH1D *h_dphi_METjet1_Ele0;
  TH1D *h_dphi_METjet2_Ele0;
  TH1D *h_Jet1Pt_Ele0;
  TH1D *h_Jet2Pt_Ele0;
  TH1D *h_isoEleTrack_Ele0;
  TH1D *h_isoMuTrack_Ele0;
  TH1D *h_isoPiTrack_Ele0;

  TH1D *h_GenEleEta_Ele0;
  TH1D *h_GenElePt_Ele0;
  TH1D *h_nGenEleFmTau_Ele0;
  TH1D *h_nGenMuFmTau_Ele0;
  TH1D *h_genEleParent_Ele0;
  TH1D *h_photonPrompt_Ele0;
  TH1D *h_fakePhoPt_Ele0;

  TH2D *h2_METnHadJ_Ele0;
  TH2D *h2_dPhi1nHadJ_Ele0;
  TH2D *h2_dPhi2nHadJ_Ele0;
  TH2D *h2_dPhi1MET_Ele0;
  TH2D *h2_STMET_Ele0;
  TH2D *h2_dPhi1dPhi2_Ele0;
  TH2D *h2_GenElePtGenAct_Ele0;

  TH2D *h2_STHadJ_Ele0;
  TH2D *h2_METJet1Pt_Ele0;
  TH2D *h2_RatioJetPhoPtVsPhoPt_Ele0;
  TH3D *h3_STMETnHadJ_Ele0;

  TH2D *h2_hadJbTag_Ele0;

  TH2D *h2_STMET_NJ2or3_Ele0;
  TH2D *h2_STMET_NJ4_Ele0;
  TH2D *h2_STMET_NJ5or6_Ele0;
  TH2D *h2_STMET_NJ7toInf_Ele0;

  TH2D *h2_METNJ_0b_Ele0;
  TH2D *h2_METNJ_1b_Ele0;
  //  TH2D *h2_METNJ_m2b_Ele0;
  TH2D *h2_R_PhoPtJetPtVsDR_Ele0;
  //-----------------------------------------
  TH1D *h_nVtx_Ele1;
  TH1D *h_ST_Ele1;
  TH1D *h_MET_Ele1;
  TH1D *h_nHadJets_Ele1;
  TH1D *h_BTags_Ele1;
  TH1D *h_METvBin_Ele1;
  TH1D *h_BestPhotonPt_Ele1;
  TH1D *h_BestPhotonEta_Ele1;
  TH1D *h_BestPhotonPhi_Ele1;
  TH1D *h_mTpho_Ele1;
  TH1D *h_dPhiEleMET;

  TH1D *h_isoMuonTracks_Ele1;
  TH1D *h_EleMultInJets_Ele1;
  TH1D *h_invMassPhoEle;

  TH1D *h_nGenEle_Ele1,*h_nGenMu_Ele1,*h_nGenTau_Ele1;
  TH1D *h_GenElectrons_MT2Activity_Ele1;
  TH1D *h_GenElePt_Ele1;
 
  TH1D *h_ElePt;
  TH1D *h_EleEta;
  TH1D *h_ElePhi;
  TH1D *h_mTEle;

  TH1D *h_dPhi_PhoMET_Ele1;
  TH1D *h_dphi_METjet1_Ele1;
  TH1D *h_dphi_METjet2_Ele1;
  TH1D *h_Jet1Pt_Ele1;
  TH1D *h_Jet2Pt_Ele1;
  TH1D *h_isoEleTrack_Ele1;
  TH1D *h_isoMuTrack_Ele1;
  TH1D *h_isoPiTrack_Ele1;
  TH1D *h_nGenEleFmTau_Ele1;
  TH1D *h_nGenMuFmTau_Ele1;
  TH1D *h_mTPhoEleMET;
  TH1D *h_invMassElePhoNu;  

  TH2D *h2_STvsElePt_Ele1;
  TH2D *h2_METvsElePt_Ele1;
  TH2D *h2_GenActVsRECOAct;
  TH2D *h2_GenElePtVsRECOElePt;

  TH1D *h_dPhi_Ele_Photon;
   
  TH1D *h_PdgIdPhoParent;
  TH1D *h_GmatchedObj;
  TH1D *h_PdgIdEleParent;
  TH1D *h_EleMatchedObj;
  TH1D *h_photonPrompt_Ele1;
  TH1D *h_fakePhoPt_Ele1;
  TH1D *h_dR_ElePho;
 
  TH2D *h2_METnHadJ_Ele1;
  TH2D *h2_dPhi1nHadJ_Ele1;
  TH2D *h2_dPhi2nHadJ_Ele1;
  TH2D *h2_dPhi1MET_Ele1;
  TH2D *h2_STMET_Ele1;
  TH2D *h2_RecoElePtRecoAct_Ele1;
  TH2D *h2_dPhi1dPhi2_Ele1;

  TH2D *h2_STHadJ_Ele1;
  TH2D *h2_METJet1Pt_Ele1;
  TH2D *h2_RatioJetPhoPtVsPhoPt_Ele1,*h2_RatioJetElePtVsElePt_Ele1;
  TH3D *h3_STMETnHadJ_Ele1;

  TH2D *h2_hadJbTag_Ele1;

  TH2D *h2_STMET_NJ2or3_Ele1;
  TH2D *h2_STMET_NJ4_Ele1;
  TH2D *h2_STMET_NJ5or6_Ele1;
  TH2D *h2_STMET_NJ7toInf_Ele1;

  TH2D *h2_METNJ_0b_Ele1;
  TH2D *h2_METNJ_1b_Ele1;
  //  TH2D *h2_METNJ_m2b_Ele1;

  TH2D *h2_R_PhoPtJetPtVsDR_Ele1;
  TH2D *h2_R_ElePtJetPtVsDR;

  TH1D *h_temp;
  TH1D *h_MET_Ele0_R[3];
  TH1D *h_MET_Ele1_R[3];
  TH1D *h_MET_R_v2_Ele0[5];
  TH1D *h_MET_R_v2_Ele1[5];

  TH2D *h2_SBinsv7VsnJ_Ele0,*h2_SBinsv7VsnJ_Ele1;
  TH1D *h_SBins_Ele0,*h_SBins_v1_Ele0,*h_SBins_v3_Ele0,*h_SBins_v4_Ele0,*h_SBins_v7_Ele0;
  TH1D *h_SBins_Ele1,*h_SBins_v1_Ele1,*h_SBins_v3_Ele1,*h_SBins_v4_Ele1,*h_SBins_v7_Ele1;
  TH1D *h_HTgammaSB;
  TFile *oFile;
 
};
#endif

#ifdef LostEle_cxx

void LostEle::BookHistogram(const char *outFileName) {

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
  h_HT=new TH1D("HT","HT",400,0,4000);
  h_MHT=new TH1D("MHT","MHT",200,0,2000);
  h_nJets=new TH1D("nJets","nJets",25,0,25);
  h_madHT=new TH1D("madHT","madHT",400,0,4000);
  h_nVtx=new TH1D("nVtx","no. of vertices",50,0,50);

  h_nVtx_Ele0=new TH1D("nVtx_Ele0","no. of vertices for 0 Electron events",50,0,50);
  h_ST_Ele0=new TH1D("ST_Ele0","ST for 0 Electron events",400,0,4000);
  h_MET_Ele0=new TH1D("MET_Ele0","MET for 0 Electron events",200,0,2000);
  h_nHadJets_Ele0=new TH1D("nHadJets_Ele0","no. of jets(only hadronic jets,not counting photon)  for 0 Electron events",25,0,25);
  h_BTags_Ele0=new TH1D("nBTags_Ele0","no. of B tags for 0 Electron events",10,0,10);
  h_METvBin_Ele0=new TH1D("METvarBin_Ele0","MET with variable bin size for 0 Electron events",METBinLowEdgeV4_njLow.size()-1,&(METBinLowEdgeV4_njLow[0]));
  h_BestPhotonPt_Ele0=new TH1D("BestPhotonPt_Ele0","Pt of the best photon for 0 Electron events",150,0,1500);
  h_BestPhotonEta_Ele0=new TH1D("BestPhotonEta_Ele0","Eta of the best photon for 0 Electron events",120,-6,6);
  h_BestPhotonPhi_Ele0=new TH1D("BestPhotonPhi_Ele0","Phi of the best photon for 0 Electron events",80,-4,4);
  h_mTpho_Ele0=new TH1D("mTPho_Ele0","mT b/w photon and MET for 0 Electron events",150,0,1500);

  h_EleMultInJets_Ele0=new TH1D("EleMultInJets_Ele0","Electron multiplicity in jets for 0 Ele events",20,0,20);

  h_nGenEle_Ele0=new TH1D("nGenEle_Ele0","number of gen electrons, electrons from W/tau(tau's parent is W), for 0 Electron events",5,-0.5,4.5);
  h_nGenMu_Ele0=new TH1D("nGenMu_Ele0","number of gen muons, muons from W/tau(tau's parent is W), for 0 Electron events",5,-0.5,4.5);
  h_nGenTau_Ele0=new TH1D("nGenTau_Ele0","number of gen taus, taus from W, for 0 Electron events",5,-0.5,4.5);
  
  h_GenElectrons_MT2Activity_Ele0=new TH1D("GenElectrons_MT2Activity_Ele0","Gen MT2 activity for 0 Ele events",100,0,0.5);

  h_dPhi_PhoMET_Ele0=new TH1D("dPhi_METBestPhoton_Ele0","dphi between MET and BestPhoton for 0 Electron events",40,0,4);
  h_dphi_METjet1_Ele0=new TH1D("dPhi_METjet1_Pho_Ele0","dphi between MET Vec and Jet1_Pho for 0 Electron events",40,0.3,4.3);
  h_dphi_METjet2_Ele0=new TH1D("dPhi_METjet2_Pho_Ele0","dphi between MET Vec and Jet2_Pho for 0 Electron events",40,0.3,4.3);
  h_Jet1Pt_Ele0=new TH1D("jet1Pt_Ele0","leading jet Pt for 0 Ele events",200,0,2000);
  h_Jet2Pt_Ele0=new TH1D("jet2Pt_Ele0","2nd leading jet Pt for 0 Ele events",200,0,2000);
  h_isoEleTrack_Ele0=new TH1D("isoEleTrack_Ele0","isolated electron tracks for 0 Ele events",5,0,5);
  h_isoMuTrack_Ele0=new TH1D("isoMuTrack_Ele0","isolated muon tracks for 0 Ele events",5,0,5);
  h_isoPiTrack_Ele0=new TH1D("isoPiTrack_Ele0","isolated pion tracks for 0 Ele events",5,0,5);

  h_GenEleEta_Ele0=new TH1D("GenEleEta_Ele0","Eta of the Gen electron for 0 RECO Ele events. Gen Eles are coming from W/tau(tau's parent is W)",120,-6,6);
  h_GenElePt_Ele0=new TH1D("GenElePt_Ele0","Pt of the Gen Electron for 0 RECO Ele events. Gen Eles are coming from W/tau(tau's parent is W)",150,0,1500);
  h_nGenEleFmTau_Ele0=new TH1D("nGenEleFmTau_Ele0","no. of gen electrons from tau for 0 Ele events",5,0,5);
  h_nGenMuFmTau_Ele0=new TH1D("nGenMuFmTau_Ele0","no. of gen muons from tau for 0 Ele events",5,0,5);
  h_genEleParent_Ele0=new TH1D("genEleParent_Ele0","PdgID of gen electron's parent for 0 Ele events",51,-0.5,50.5);
  h_photonPrompt_Ele0=new TH1D("photonPrompt_Ele0","is Photon prompt(1) or non-prompt(0) for 0 Ele events",3,0,3);
  h_fakePhoPt_Ele0=new TH1D("fakePhoPt_Ele0","Pt of the fake photon for 0 Ele events",200,0,2000);

  h2_METnHadJ_Ele0=new TH2D("METnHadJ_Ele0","x: MET vs nHadJets for 0 Ele events",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi1nHadJ_Ele0=new TH2D("dPhi1nHadJ_Ele0","x:dPhi MET and leading jet vs nHadJets for 0 Ele events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi2nHadJ_Ele0=new TH2D("dPhi2nHadJ_Ele0","x:dPhi MET and 2nd leading jet vs nHadJets for 0 Ele events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi1MET_Ele0=new TH2D("dPhi1MET_Ele0","x:dPhi MET and leading jet vs MET for 0 Ele events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),METBinLowEdge2.size()-1,&(METBinLowEdge2[0]));
  h2_STMET_Ele0=new TH2D("STMET_Ele0","x:ST vs MET for 0 Ele events",STLowEdge.size()-1,&(STLowEdge[0]),METBinLowEdge4.size()-1,&(METBinLowEdge4[0]));
  h2_dPhi1dPhi2_Ele0=new TH2D("dPhi1dPhi2_Ele0","x:dPhi1 vs dPhi2 for 0 Ele events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),dPhi2LowEdge.size()-1,&(dPhi2LowEdge[0]));
  h2_GenElePtGenAct_Ele0=new TH2D("GenElePtGenAct_Ele0","x:Pt of Gen Electron vs MT2 activity of Gen Electron",ElePtLowEdge.size()-1,&(ElePtLowEdge[0]),EleActLowEdge.size()-1,&(EleActLowEdge[0]));

  h2_STHadJ_Ele0=new TH2D("STHadJ_Ele0","x:ST vs HadJets for 0 Ele events",STLowEdge.size()-1,&(STLowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_METJet1Pt_Ele0=new TH2D("METJet1Pt_Ele0","x:MET vs leading JetPt for 0 Ele events",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),jet1PtLowEdge.size()-1,&(jet1PtLowEdge[0]));
  h3_STMETnHadJ_Ele0=new TH3D("STMETnHadJ_Ele0","x:ST,y:MET,z:nHadJets for 0 Ele events",STLowEdge2.size()-1,&(STLowEdge2[0]),METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),nHadJLow.size()-1,&(nHadJLow[0]));

  h2_hadJbTag_Ele0=new TH2D("hadJbTag_Ele0","x: hadJ vs BTag for 0 Ele events",1,0,100,1,0,100);

  h2_STMET_NJ2or3_Ele0 = new TH2D("STMET_Ele0_R1","x:ST vs MET for NJ=2 or 3 for 0 Ele events",STInclLowEdge.size()-1,&(STInclLowEdge[0]),MET_NJ2or3LowEdge.size()-1,&(MET_NJ2or3LowEdge[0]));
  h2_STMET_NJ4_Ele0 = new TH2D("STMET_Ele0_R2","x:ST vs MET for NJ=4 for 0 Ele events",STInclLowEdge.size()-1,&(STInclLowEdge[0]),MET_NJ4LowEdge.size()-1,&(MET_NJ4LowEdge[0]));
  h2_STMET_NJ5or6_Ele0 = new TH2D("STMET_Ele0_R3","x:ST vs MET for NJ=5 or 6 for 0 Ele events",ST_NJ5or6LowEdge.size()-1,&(ST_NJ5or6LowEdge[0]),METInclLowEdge.size()-1,&(METInclLowEdge[0]));
  h2_STMET_NJ7toInf_Ele0 = new TH2D("STMET_Ele0_R4","x:ST vs MET for NJ>=7 for 0 Ele events",ST_NJ7toInfLowEdge.size()-1,&(ST_NJ7toInfLowEdge[0]),METInclLowEdge.size()-1,&(METInclLowEdge[0]));

  h2_METNJ_0b_Ele0=new TH2D("METNJ_Ele0_R0","x:MET vs NJets for 0 b for 0 Ele events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow0b.size()-1,&(nHadJLow0b[0]));
  h2_METNJ_1b_Ele0=new TH2D("METNJ_Ele0_R1","x:MET vs NJets for 1 b for 0 Ele events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow1b.size()-1,&(nHadJLow1b[0]));
  //  h2_METNJ_m2b_Ele0=new TH2D("METNJ_Ele0_R2","x:MET vs NJets for >=2 b for 0 Ele events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLowm2b.size()-1,&(nHadJLowm2b[0]));

  h2_R_PhoPtJetPtVsDR_Ele0=new TH2D("R_PhoPtJetPtVsDR_Ele0","y:Ratio of closest jet Pt to Photon Pt vs x: dR with closest jet for 0 Ele events",100,0,10,100,0,10);
  h2_RatioJetPhoPtVsPhoPt_Ele0=new TH2D("RatioJetPhoPtVsPhoPt_Ele0","x: #gamma Pt, y:ratio of pT of jet matched to photon to photon Pt for 0 Ele events",150,0,1500,400,0,5);
  //****************************************************
  h_nVtx_Ele1=new TH1D("nVtx_Ele1","no. of vertices for 1 Electron events",50,0,50);
  h_ST_Ele1=new TH1D("ST_Ele1","ST for 1 Electron events",400,0,4000);
  h_MET_Ele1=new TH1D("MET_Ele1","MET for 1 Electron events",200,0,2000);
  h_nHadJets_Ele1=new TH1D("nHadJets_Ele1","no. of jets(only hadronic jets,not counting photon)  for 1 Electron events",25,0,25);
  h_BTags_Ele1=new TH1D("nBTags_Ele1","no. of B tags for 1 Electron events",10,0,10);
  h_METvBin_Ele1=new TH1D("METvarBin_Ele1","MET with variable bin size for 1 Electron events",METBinLowEdgeV4_njLow.size()-1,&(METBinLowEdgeV4_njLow[0]));
  h_BestPhotonPt_Ele1=new TH1D("BestPhotonPt_Ele1","Pt of the best photon for 1 Electron events",150,0,1500);
  h_BestPhotonEta_Ele1=new TH1D("BestPhotonEta_Ele1","Eta of the best photon for 1 Electron events",120,-6,6);
  h_BestPhotonPhi_Ele1=new TH1D("BestPhotonPhi_Ele1","Phi of the best photon for 1 Electron events",80,-4,4);
  h_mTpho_Ele1=new TH1D("mTPho_Ele1","mT b/w photon and MET for 1 Electron events",150,0,1500);
  h_dPhiEleMET=new TH1D("dPhiEleMET","dPhi(MET,e) for 1 Electron evnets",80,-4,4);

  h_GenElectrons_MT2Activity_Ele1=new TH1D("GenElectrons_MT2Activity_Ele1","Gen MT2 activity for 1 Ele events",100,0,0.5);
  h_ElePt=new TH1D("ElePt","Electron Pt",150,0,1500);
  h_EleEta=new TH1D("EleEta","Electron eta",120,-6,6);
  h_ElePhi=new TH1D("ElePhi","Electron Phi",80,-4,4);
  h_mTEle=new TH1D("MT_Ele","MT for 1 Ele events",500,0,500);

  h_EleMultInJets_Ele1=new TH1D("EleMultInJets_Ele1","Electron multiplicity in jets for 1 Ele events",20,0,20);
  h_invMassPhoEle=new TH1D("invMassPhoEle","Inv mass of photon and muon",200,0,2000);

  h_dPhi_PhoMET_Ele1=new TH1D("dPhi_METBestPhoton_Ele1","dphi between MET and BestPhoton for 1 Electron events",40,0,4);
  h_dphi_METjet1_Ele1=new TH1D("dPhi_METjet1_Pho_Ele1","dphi between MET Vec and Jet1_Pho for 1 Electron events",40,0.3,4.3);
  h_dphi_METjet2_Ele1=new TH1D("dPhi_METjet2_Pho_Ele1","dphi between MET Vec and Jet2_Pho for 1 Electron events",40,0.3,4.3);
  h_dPhi_Ele_Photon=new TH1D("dPhi_Ele_Photon","dphi b/w muon and best photon",40,0,4);
  h_Jet1Pt_Ele1=new TH1D("jet1Pt_Ele1","leading jet Pt for 1 Ele events",200,0,2000);
  h_Jet2Pt_Ele1=new TH1D("jet2Pt_Ele1","2nd leading jet Pt for 1 Ele events",200,0,2000);
  h_isoEleTrack_Ele1=new TH1D("isoEleTrack_Ele1","isolated electron tracks for 1 Ele events",5,0,5);
  h_isoMuTrack_Ele1=new TH1D("isoMuTrack_Ele1","isolated muon tracks for 1 Ele events",5,0,5);
  h_isoPiTrack_Ele1=new TH1D("isoPiTrack_Ele1","isolated pion tracks for 1 Ele events",5,0,5);

  h_GenElePt_Ele1=new TH1D("GenElePt_Ele1","Pt of the Gen Electron for 1 RECO Ele events. Gen Eles are coming from W/tau(tau's parent is W)",150,0,1500);
  h_nGenEle_Ele1=new TH1D("nGenEle_Ele1","number of gen electrons, electrons from W/tau(tau's parent is W), for 1 Electron events",5,-0.5,4.5);
  h_nGenMu_Ele1=new TH1D("nGenMu_Ele1","number of gen muons, muons from W/tau(tau's parent is W), for 1 Electron events",5,-0.5,4.5);
  h_nGenTau_Ele1=new TH1D("nGenTau_Ele1","number of gen taus, taus from W, for 1 Electron events",5,-0.5,4.5);
  h_fakePhoPt_Ele1=new TH1D("fakePhoPt_Ele1","Pt of the fake photon for 1 Ele events",200,0,2000);
  h_dR_ElePho=new TH1D("dR_ElePho","dR(e,#gamma) for 1 Electron events",100,0,10);
  h_mTPhoEleMET=new TH1D("mTPhoEleMET","mT(pho+Ele,MET)",500,0,500);
  h_invMassElePhoNu=new TH1D("invMassElePhoNu","inv mass of mu, leading photon and neutrino",500,0,500);


  h2_STvsElePt_Ele1=new TH2D("STvsElePt_Ele1","x: Electron Pt vs ST for 1 Electron events",150,0,1500,400,0,4000);
  h2_METvsElePt_Ele1=new TH2D("METvsElePt_Ele1","x:Electron Pt vs MET for 1 Electron events",150,0,1500,200,0,2000);

  h2_GenActVsRECOAct=new TH2D("GenActVsRECOAct","x:Gen Electron MT2 activity vs RECO Electron MT2 activity",100,0,0.5,100,0,0.5);
  h2_GenElePtVsRECOElePt=new TH2D("GenElePtVsRECOElePt","x:Gen muon Pt vs RECO muon Pt",150,0,1500,150,0,1500);

  h_minDR_Pho_GenObj=new TH1D("minDR_Pho_GenObj","min DR b/w reco photon and gen objects",100,0,1);
  h_GmatchedObj=new TH1D("GmatchedObj","Gen Obj close to Reco-Gamma",62,-0.75,30.25);
  h_PdgIdPhoParent = new TH1D("PdgIdPhoParent","PdgID of the Photon's parent",62,-0.75,30.25);

  h_EleMatchedObj =new TH1D("EleMatchedObj","Gen Obj close to Reco-Electron",62,-0.75,30.25);
  h_PdgIdEleParent = new TH1D("PdgIdEleParent","PdgID of the muon's parent",62,-0.75,30.25);
  h_photonPrompt_Ele1=new TH1D("photonPrompt_Ele1","is Photon prompt(1) or non-prompt(0) for 1 Ele events",3,0,3);

  h_nGenEleFmTau_Ele1=new TH1D("nGenEleFmTau_Ele1","no. of gen electrons from tau for 1 Ele events",5,0,5);
  h_nGenMuFmTau_Ele1=new TH1D("nGenMuFmTau_Ele1","no. of gen muons from tau for 1 Ele events",5,0,5);

  h2_METnHadJ_Ele1=new TH2D("METnHadJ_Ele1","x: MET vs nHadJets for 1 Ele events",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi1nHadJ_Ele1=new TH2D("dPhi1nHadJ_Ele1","x:dPhi MET and leading jet vs nHadJets for 1 Ele events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi2nHadJ_Ele1=new TH2D("dPhi2nHadJ_Ele1","x:dPhi MET and 2nd leading jet vs nHadJets for 1 Ele events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_dPhi1MET_Ele1=new TH2D("dPhi1MET_Ele1","x:dPhi MET and leading jet vs MET for 1 Ele events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),METBinLowEdge2.size()-1,&(METBinLowEdge2[0]));
  h2_STMET_Ele1=new TH2D("STMET_Ele1","x:ST vs MET for 1 Ele events",STLowEdge.size()-1,&(STLowEdge[0]),METBinLowEdge4.size()-1,&(METBinLowEdge4[0]));
  h2_dPhi1dPhi2_Ele1=new TH2D("dPhi1dPhi2_Ele1","x:dPhi1 vs dPhi2 for 1 Ele events",dPhi1LowEdge.size()-1,&(dPhi1LowEdge[0]),dPhi2LowEdge.size()-1,&(dPhi2LowEdge[0]));
  h2_RecoElePtRecoAct_Ele1=new TH2D("RecoElePtRecoAct_Ele1","x:Pt of RECO Electron vs MT2 activity of RECO Electron",ElePtLowEdge.size()-1,&(ElePtLowEdge[0]),EleActLowEdge.size()-1,&(EleActLowEdge[0]));

  h2_STHadJ_Ele1=new TH2D("STHadJ_Ele1","x:ST vs HadJets for 1 Ele events",STLowEdge.size()-1,&(STLowEdge[0]),nHadJLow.size()-1,&(nHadJLow[0]));
  h2_METJet1Pt_Ele1=new TH2D("METJet1Pt_Ele1","x:MET vs leading JetPt for 1 Ele events",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),jet1PtLowEdge.size()-1,&(jet1PtLowEdge[0]));
  h3_STMETnHadJ_Ele1=new TH3D("STMETnHadJ_Ele1","x:ST,y:MET,z:nHadJets for 1 Ele events",STLowEdge2.size()-1,&(STLowEdge2[0]),METBinLowEdge2.size()-1,&(METBinLowEdge2[0]),nHadJLow.size()-1,&(nHadJLow[0]));

  h2_hadJbTag_Ele1=new TH2D("hadJbTag_Ele1","x: hadJ vs BTag for 1 Ele events",1,0,100,1,0,100);

  h2_STMET_NJ2or3_Ele1 = new TH2D("STMET_Ele1_R1","x:ST vs MET for NJ=2 or 3 for 1 Ele events",STInclLowEdge.size()-1,&(STInclLowEdge[0]),MET_NJ2or3LowEdge.size()-1,&(MET_NJ2or3LowEdge[0]));
  h2_STMET_NJ4_Ele1 = new TH2D("STMET_Ele1_R2","x:ST vs MET for NJ=4 for 1 Ele events",STInclLowEdge.size()-1,&(STInclLowEdge[0]),MET_NJ4LowEdge.size()-1,&(MET_NJ4LowEdge[0]));
  h2_STMET_NJ5or6_Ele1 = new TH2D("STMET_Ele1_R3","x:ST vs MET for NJ=5 or 6 for 1 Ele events",ST_NJ5or6LowEdge.size()-1,&(ST_NJ5or6LowEdge[0]),METInclLowEdge.size()-1,&(METInclLowEdge[0]));
  h2_STMET_NJ7toInf_Ele1 = new TH2D("STMET_Ele1_R4","x:ST vs MET for NJ>=7 for 1 Ele events",ST_NJ7toInfLowEdge.size()-1,&(ST_NJ7toInfLowEdge[0]),METInclLowEdge.size()-1,&(METInclLowEdge[0]));
  h2_R_PhoPtJetPtVsDR_Ele1=new TH2D("R_PhoPtJetPtVsDR_Ele1","y:Ratio of closest jet Pt to Photon Pt vs x: dR with closest jet for 1 Ele events",100,0,10,100,0,10);

  h2_METNJ_0b_Ele1=new TH2D("METNJ_Ele1_R0","x:MET vs NJets for 0 b for 1 Ele events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow0b.size()-1,&(nHadJLow0b[0]));
  h2_METNJ_1b_Ele1=new TH2D("METNJ_Ele1_R1","x:MET vs NJets for 1 b for 1 Ele events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLow1b.size()-1,&(nHadJLow1b[0]));
  //  h2_METNJ_m2b_Ele1=new TH2D("METNJ_Ele1_R2","x:MET vs NJets for >=2 b for 1 Ele events",METBinLowEdge5.size()-1,&(METBinLowEdge5[0]),nHadJLowm2b.size()-1,&(nHadJLowm2b[0]));

  h2_R_ElePtJetPtVsDR=new TH2D("R_ElePtJetPtVsDR","y:Ratio of closest jet Pt to Ele Pt vs x: dR with closest jet",100,0,10,100,0,10);
  h2_RatioJetPhoPtVsPhoPt_Ele1=new TH2D("RatioJetPhoPtVsPhoPt_Ele1","x: #gamma Pt, y:ratio of pT of jet matched to photon to photon Pt for 1 Ele events",150,0,1500,400,0,5);
  h2_RatioJetElePtVsElePt_Ele1=new TH2D("RatioJetElePtVsElePt_Ele1","x: #gamma Pt, y:ratio of pT of jet matched to electron to electron Pt for 1 Ele events",150,0,1500,400,0,5);
  h_temp=new TH1D("temp","temp",200,0,2000);

  //---------------- search bins -----------------------
  h2_SBinsv7VsnJ_Ele0 = new TH2D("SBinsv7VsnJ_Ele0","x:search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] , y: nHadJets for 0 Ele events",31,0.5,31.5,nHadJLow0b.size()-1,&(nHadJLow0b[0]));
  h2_SBinsv7VsnJ_Ele1 = new TH2D("SBinsv7VsnJ_Ele1","x:search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] , y: nHadJets for 1 Ele events",31,0.5,31.5,nHadJLow0b.size()-1,&(nHadJLow0b[0]));

  h_MET_Ele0_R[0] = new TH1D("MET_Ele0_R1","MET: NJ=2to4, for 0 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele0_R[1] = new TH1D("MET_Ele0_R2","MET: NJ=5or6, for 0 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele0_R[2] = new TH1D("MET_Ele0_R3","MET: NJ>=7,   for 0 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  
  h_MET_Ele1_R[0] = new TH1D("MET_Ele1_R1","MET: NJ=2to4, for 1 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele1_R[1] = new TH1D("MET_Ele1_R2","MET: NJ=5or6, for 1 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele1_R[2] = new TH1D("MET_Ele1_R3","MET: NJ>=7,   for 1 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_MET_R_v2_Ele0[0] =new TH1D("MET_R1_v2_Ele0","MET for 0b, NJ=2to4 for 0 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele0[1] =new TH1D("MET_R2_v2_Ele0","MET for 0b, NJ>=5 for 0 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele0[2] =new TH1D("MET_R3_v2_Ele0","MET for 1b, NJ=2to4 for 0 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele0[3] =new TH1D("MET_R4_v2_Ele0","MET for 1b, NJ>=5 for 0 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele0[4] =new TH1D("MET_R5_v2_Ele0","MET for b>=2 for 0 Ele events",METBinLowEdge2bJ.size()-1,&(METBinLowEdge2bJ[0]));  

  h_MET_R_v2_Ele1[0] =new TH1D("MET_R1_v2_Ele1","MET for 0b, NJ=2to4 for 1 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele1[1] =new TH1D("MET_R2_v2_Ele1","MET for 0b, NJ>=5 for 1 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele1[2] =new TH1D("MET_R3_v2_Ele1","MET for 1b, NJ=2to4 for 1 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele1[3] =new TH1D("MET_R4_v2_Ele1","MET for 1b, NJ>=5 for 1 Ele events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele1[4] =new TH1D("MET_R5_v2_Ele1","MET for b>=2 for 1 Ele events",METBinLowEdge2bJ.size()-1,&(METBinLowEdge2bJ[0]));  
  
  h_SBins_Ele0 = new TH1D("AllSBins_Ele0","all search bins:(0b, NJ=2to4)(0b, NJ>=5)(1b, NJ=2to4)(1b, NJ>=5)(b>=2) for 0 Ele events",34,0.5,34.5);
  h_SBins_v1_Ele0 = new TH1D("AllSBins_v1_Ele0","search bins:(NJ=2to4) (NJ:5or6) (NJ>=7) for 0 Ele events",21,0.5,21.5);
  h_SBins_v3_Ele0 = new TH1D("AllSBins_v3_Ele0","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, 1b, >=2b] for 0 Ele events",63,0.5,63.5);

  h_SBins_Ele1 = new TH1D("AllSBins_Ele1","all search bins:(0b, NJ=2to4)(0b, NJ>=5)(1b, NJ=2to4)(1b, NJ>=5)(b>=2) for 1 Ele events",34,0.5,34.5);
  h_SBins_v1_Ele1 = new TH1D("AllSBins_v1_Ele1","search bins:(NJ=2to4) (NJ:5or6) (NJ>=7) for 1 Ele events",21,0.5,21.5);
  h_SBins_v3_Ele1 = new TH1D("AllSBins_v3_Ele1","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, 1b, >=2b] for 1 Ele events",63,0.5,63.5);

  h_SBins_v4_Ele0 = new TH1D("AllSBins_v4_Ele0","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] for 0 Ele events",43,0.5,43.5);
  h_SBins_v4_Ele1 = new TH1D("AllSBins_v4_Ele1","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] for 1 Ele events",43,0.5,43.5);

  h_SBins_v7_Ele0 = new TH1D("AllSBins_v7_Ele0","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] for 0 Ele events",31,0.5,31.5);
  h_SBins_v7_Ele1 = new TH1D("AllSBins_v7_Ele1","search bins: [ NJ:2-4, NJ:5or6, NJ>=7] x [0b, >=1b] for 1 Ele events",31,0.5,31.5);

  h_HTgammaSB = new TH1D("HTgammaSB","Search bins used in HT+gamma analysis",6,0.5,6.5);
}


LostEle::LostEle(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

Bool_t LostEle::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t LostEle::LoadTree(Long64_t entry) {
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

LostEle::~LostEle() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

