#ifndef FakeRateEst_H
#define FakeRateEst_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "NtupleVariables.h"
#include "TH1F.h"
#include "TH2.h"
#include "TH3.h"
#include <TProfile.h>
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

class FakeRateEst : public NtupleVariables{

 public:
  FakeRateEst(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~FakeRateEst();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);
  TLorentzVector getBestPhoton();
  bool check_eMatchedtoGamma();
  void print(Long64_t);
  void findObjMatchedtoG(TLorentzVector);
  double dR_PtEtaPhi(double,double,double,double,double,double);
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
  //vector<double> HTBinLowEdge ={0,500,550,600,650,700,750,800,900,1000,1100,1200,1400,1600,1800,2000,2400,2800,3200};
  //  vector<double> HTBinLowEdge ={0,100,200,300,400,500,600,700,800,900,1000,1200,1500,2000,2500};
  //  vector<double> MHTBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,500};
  vector<double> STBinLowEdge ={0,300,360,420,500,600,700,850,1000,1200,1500,2000,2500,3000};
  //  vector<double> METBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,450,600,750,900,1200};
  vector<double> METBinLowEdge={0,20,40,60,80,100,125,160,200,270,350,500,600};
  //  vector<double> METBinLowEdge2={0,20,40,60,80,100,120,160,200,270,350,450,500};//org
  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  //vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,200,250,300,350,400,500,600};
  vector<TLorentzVector> allBestPhotons;
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

  TH1D *h_EMObjPt;
  TH1D *h_EMObjEta;
  TH1D *h_EMObjPhi;

  TH2D *h2_EMObjPtEta;
  TH2D *h2_PtEMObjvsMET;
  TH1D *h_dPhi_METEMObj;
  TH1D *h_dPhi_METjet1;
  TH1D *h_dPhi_METjet2;
  TH1D *h_dPhi_METjet3;
  TH1D *h_dPhi_METjet4;

  TH1D *h_jetPt[4];
  TH1D *h_jetEta[4];
  TH1D *h_jetPhi[4];
  //for Photon and Ele like objects
  TH1D *h_ST_Pho,*h_MET_Pho,*h_nHadJets_Pho,*h_BTags_Pho,*h_METvBin_Pho,*h_HT_Pho,*h_MHT_Pho,*h_nJets_Pho,*h_madHT_Pho,*h_nVtx_Pho,*h_mT_Pho;
  TH1D *h_ST_Ele,*h_MET_Ele,*h_nHadJets_Ele,*h_BTags_Ele,*h_METvBin_Ele,*h_HT_Ele,*h_MHT_Ele,*h_nJets_Ele,*h_madHT_Ele,*h_nVtx_Ele,*h_mT_Ele;

  TH1D *h_MET_Pho_R[3],*h_MET_Ele_R[3];
  TH1D *h_MET_Pho_Rall,*h_MET_Ele_Rall;

  TH1D *h_dPhi_METjet1_Pho,*h_dPhi_METjet2_Pho,*h_dPhi_METjet3_Pho,*h_dPhi_METjet4_Pho;
  TH1D *h_dPhi_METjet1_Ele,*h_dPhi_METjet2_Ele,*h_dPhi_METjet3_Ele,*h_dPhi_METjet4_Ele;
  TH1D *h_genEleMT2Act_Pho,*h_genEleMT2Act_Ele,*h_EleMT2Activity;

  TH1D *h_EleTracks_Ele; TH1D *h_EleTracks_Pho;
  TH1D *h_MuTracks_Ele; TH1D *h_MuTracks_Pho;
  TH1D *h_PiTracks_Ele;  TH1D *h_PiTracks_Pho;
  TH1D *h_nTracks_Ele;  TH1D *h_nTracks_Pho;
  TH1D *h_nTracks0p3_Ele;  TH1D *h_nTracks0p3_Pho;

  TH1D *h_BestPhotonPt,*h_ElePt,*h_PhoPtvBin,*h_ElePtvBin;
  TH1D *h_BestPhotonEta,*h_EleEta;
  TH1D *h_BestPhotonPhi,*h_ElePhi;

  TH1D *h_minDR_Pho_Jet,*h_minDR_Ele_Jet;
  TH1D *h_dRGenJet2_Pho,*h_dRGenJet2_Ele;
  TH1D *h_minDRHadJ_Pho_HadJet,*h_minDRHadJ_Ele_HadJet;
  TH1D *h_PtJetNearPho,*h_PtJetNearEle;
  TH1D *h_JptGptRatio,*h_JptEleptRatio;
  TH1D *h_HadDR_GptJptRatio,*h_HadDR_EleptJptRatio;

  TH2D *h2_PhoPtST,*h2_ElePtST;
  TH2D *h2_PhotonPtEta,*h2_ElePtEta;
  TH2D *h2_PhotonPtEtavBin,*h2_ElePtEtavBin;
  TH2D *h2_JetPtNearPhoEtavBin,*h2_JetPtNearEleEtavBin;

  TH2D *h2_JetPtNearPhoMinDRHadJ,*h2_JetPtNearEleMinDRHadJ;
  TH2D *h2_PhoPtMinDRHadJ,*h2_ElePtMinDRHadJ;
  TH2D *h2_PhoPtQMultJet,*h2_ElePtQMultJet;
  TH2D *h2_PhoEtaPhi,*h2_EleEtaPhi;
  TH2D *h2_minHadDR_QMult_Pho,*h2_minHadDR_QMult_Ele;

  TH2D *h2_nVtx_EleMT2Act;
  TH2D *h2_RecoPhoPt_GenEPt,*h2_RecoElePt_GenEPt;
  TH2D *h2_JptGptRatiovsDR,*h2_JptEleptRatiovsDR;
  TH2D *h2_HadDRJptGptRatio_Vtx,*h2_HadDRJptEptRatio_Vtx;
  TH2D *h2_PhoPtVtx,*h2_ElePtVtx;

  TH3D *h3_PhotonPtEtaNJ,*h3_ElePtEtaNJ;
  TH3D *h3_PhoPtEtaVtx,*h3_ElePtEtaVtx;
  TH3D *h3_JetPtPhoEtaMinHadDR,*h3_JetPtEleEtaMinHadDR;
  TH3D *h3_JetPtVtxMinHadDR_Pho,*h3_JetPtVtxMinHadDR_Ele;
  TH3D *h3_PhoPtVtxMinHadDR,*h3_ElePtVtxMinHadDR;
  TH3D *h3_PhoPtEtaMinDRHadJ,*h3_ElePtEtaMinDRHadJ;

  TH1D *h_photon_pfQIsoRhoCorr,*h_photon_pfNutIsoRhoCorr;
  //  double etaLow[5]={0, 0.6, 1.2, 1.8, 3};
  double etaLow[6]={0, 0.5, 1.0, 1.48, 1.65, 3.0};
  double ptLow[10]={0,100,125,150,185,225,300,400,900,2000};
  //  double nJEtaYLow[9]={0,2,3,4,5,6,7,8,50};
  double nJEtaYLow[9]={0,5,10,15,25,30,35,40,500};
  //  double ptLow2[5]={0,200,400,600,20000};
  //  double ptLow2[8]={0,100,150,200,250,300,500,20000};//a good one
  double ptLow2[8]={0,100,140,190,240,300,500,20000};
  vector<double> ptLow3={0,100,130,190,300,20000};
  //double minDRLow[6]={0,0.03,0.05,0.1,0.2,1};
  double minDRLow[6]={0,0.02,0.05,0.1,0.4,100};
  double minDR_RatioLow[6]={0,0.02,0.05,0.1,1,1000};
  double ptRatioLow[8]={0, 1, 1.1, 1.2, 1.3, 1.5, 2, 1000};
  //  double nVtxLow[10]={0,5,10,15,20,25,30,35,40,100};
  vector<double> nVtxLow={0,15,20,25,30,100};
  double QHadFracLow[8]={0,0.05,0.1,0.4,0.5,0.6,0.8,1.5};
  //  double QMultLow[6]={0,2,5,10,15,50};
  double QMultLow[6]={0,2,4,7,10,100};

  TH2D *h2_PtGvsMET,*h2_PtElevsMET;
  TH2D *h2_PtPhoQHadFracJet,*h2_PtEleQHadFracJet;
  TH1D *h_dPhi_METG,*h_dPhi_METEle;
  TH1D *h_RemjetPtNearPhoton,*h_RemjetPtNearEle;

  TH1D *h_jetPt_Pho[4], *h_jetPt_Ele[4];
  TH1D *h_jetEta_Pho[4],*h_jetEta_Ele[4];
  TH1D *h_jetPhi_Pho[4],*h_jetPhi_Ele[4];

  TH1D *h_dPhi_jet_Pho[4], *h_dPhi_jet_Ele[4];

  TH1D *h_minDR_Pho_GenObj;
  TH1D *h_GmatchedObj;
  TH1D *h_PdgIdPhoParent;

  TH1D *h_MET_R_v2_Ele[5];
  TH1D *h_MET_R_v2_Pho[5];
  TH1D *h_SBins_Ele;
  TH1D *h_SBins_Pho;

  //ele and pho type events
  TH1D *h_nConsti;
  TH1D *h_NM;
  TH1D *h_NMEta3;
  TH1D *h_NMHF;
  TH1D *h_CM,*h_CM_Pho,*h_CM_Ele;
  TH1D *h_NHF,*h_NHF_Pho,*h_NHF_Ele;
  TH1D *h_NEMF,*h_NEMF_Pho,*h_NEMF_Ele;
  TH1D *h_CHF,*h_CHF_Pho,*h_CHF_Ele;
  TH1D *h_CEMF,*h_CEMF_Pho,*h_CEMF_Ele;
  TH1D *h_PhoMult;
  TH1D *h_PhoFrac,*h_PhoFrac_Pho,*h_PhoFrac_Ele;
  TH2D *h2_PtGenReco;

  TH1D *h_temp;
  TFile *oFile;
  
};
#endif

#ifdef FakeRateEst_cxx

void FakeRateEst::BookHistogram(const char *outFileName) {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
 
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);
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

  h_EMObjPt=new TH1D("EMObjPt","Pt of the EM object(electron or photon)",150,0,1500);
  h_EMObjEta=new TH1D("EMObjEta","Eta of the EM object(electron or photon)",120,-6,6);
  h_EMObjPhi=new TH1D("EMObjPhi","Phi of the EM object(electron or photon)",80,-4,4);

  h2_EMObjPtEta=new TH2D("EMObjPtEta","x:EMObject |Eta| vs EMObject Pt",60,0,6,150,0,1500);
  h2_PtEMObjvsMET=new TH2D("EMObjPtvsMET","EM obj Pt vs MET",150,0,1500,200,0,2000);
  h_dPhi_METEMObj=new TH1D("dPhi_METEMObj","dphi between MET and EMObj",40,0,4);
  h_dPhi_METjet1=new TH1D("dPhi_METjet1","dphi between MET Vec and Jet1",40,0,4);
  h_dPhi_METjet2=new TH1D("dPhi_METjet2","dphi between MET Vec and Jet2",40,0,4);
  h_dPhi_METjet3=new TH1D("dPhi_METjet3","dphi between MET Vec and Jet3",40,0,4);
  h_dPhi_METjet4=new TH1D("dPhi_METjet4","dphi between MET Vec and Jet4",40,0,4);

  h_ST_Pho=new TH1D("ST_Pho","ST_Pho",400,0,4000);
  h_MET_Pho=new TH1D("MET_Pho","MET_Pho",200,0,2000);
  h_nHadJets_Pho=new TH1D("nHadJets_Pho","no. of jets(only hadronic jets,not counting photon)_Pho",25,0,25);
  h_BTags_Pho=new TH1D("nBTags_Pho","no. of B tags_Pho",10,0,10);
  h_METvBin_Pho=new TH1D("METvarBin_Pho","MET with variable bin size_Pho",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Pho_R[0] = new TH1D("MET_Pho_R1","MET: NJ=2to4 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Pho_R[1] = new TH1D("MET_Pho_R2","MET: NJ=5or6 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Pho_R[2] = new TH1D("MET_Pho_R3","MET: NJ>=7 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_mT_Pho = new TH1D("mT_Pho","mT(MET, photon)",200,0,2000);
  h_MET_Pho_Rall = new TH1D("MET_Pho_Rall","MET: NJets>=2 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_HT_Pho=new TH1D("HT_Pho","HT_Pho",400,0,4000);
  h_MHT_Pho=new TH1D("MHT_Pho","MHT_Pho",200,0,2000);

  h_nJets_Pho=new TH1D("nJets_Pho","nJets_Pho",25,0,25);
  h_madHT_Pho=new TH1D("madHT_Pho","madHT_Pho",400,0,4000);
  h_nVtx_Pho=new TH1D("nVtx_Pho","no. of vertices in photon like events",50,0,50);

  h_BestPhotonPt=new TH1D("BestPhotonPt","Pt of the best photon",150,0,1500);
  h_BestPhotonEta=new TH1D("BestPhotonEta","Eta of the best photon",120,-6,6);
  h_BestPhotonPhi=new TH1D("BestPhotonPhi","Phi of the best photon",80,-4,4);
  h_PhoPtvBin=new TH1D("PhoPtvBin","Photon Pt with variable binning",sizeof(ptLow)/sizeof(double)-1,ptLow);

  h_EleTracks_Pho=new TH1D("nEleTracks_Pho","No. of e tracks for Pho events",30,-0.5,29.5);
  h_MuTracks_Pho=new TH1D("nMuTracks_Pho","No. of #mu tracks for Pho events",30,-0.5,29.5);
  h_PiTracks_Pho=new TH1D("nPiTracks_Pho","No. of #pi tracks for Pho events",30,-0.5,29.5);
  h_nTracks_Pho=new TH1D("nTracks_Pho","No. of tracks for Pho events",50,-0.5,49.5);
  h_nTracks0p3_Pho=new TH1D("nTracks0p3_Pho","No. of tracks within dR < 0.3 for Pho events",50,-0.5,49.5);

  h_minDR_Pho_Jet=new TH1D("minDR_Pho_Jet","min dR b/w a jet and photon",100,0,1);
  h_minDRHadJ_Pho_HadJet=new TH1D("minDR_Pho_HadJet","minDR b/w photon and a HadJet",1000,0,10);
  h_dRGenJet2_Pho=new TH1D("dRGenJet2_Pho","mindR b/w gen jet and photon. Do not consider GenJet that's matched to photon",1000,1,10);

  h_PtJetNearPho=new TH1D("PtJetNearPho","Pt of the jet nearest to photon",150,0,1500);
  h_JptGptRatio=new TH1D("JptGptRatio","Ratio of jet Pt to photon Pt",100,0,10);
  h_HadDR_GptJptRatio=new TH1D("HadDR_GptJptRatio","dR(Hadjet,photon) * photonPt/jetPt",1000,0,1);

  h_dPhi_METjet1_Pho=new TH1D("dPhi_METjet1_Pho","dphi between MET Vec and Jet1_Pho",40,0,4);
  h_dPhi_METjet2_Pho=new TH1D("dPhi_METjet2_Pho","dphi between MET Vec and Jet2_Pho",40,0,4);
  h_dPhi_METjet3_Pho=new TH1D("dPhi_METjet3_Pho","dphi between MET Vec and Jet3_Pho",40,0,4);
  h_dPhi_METjet4_Pho=new TH1D("dPhi_METjet4_Pho","dphi between MET Vec and Jet4_Pho",40,0,4);
  h_dPhi_METG=new TH1D("dPhi_METBestPhoton","dphi between MET and BestPhoton",40,0,4);
  
  h_genEleMT2Act_Pho=new TH1D("genEleMT2Act_Pho","gen MT2 activity for electrons matced to photon",5000,0,50);

  h2_PhoPtST=new TH2D("PhoPtST","x:Photon Pt vs ST",150,0,1500,400,0,4000);
  h2_PhotonPtEta=new TH2D("PhotonPtEta","x:Photon |Eta| vs Photon Pt",60,0,6,150,0,1500);
  h2_PhotonPtEtavBin=new TH2D("PhotonPtEtavBin","x:Photon |Eta| vs Photon Pt",5,etaLow,7,ptLow2);
  h2_JetPtNearPhoEtavBin=new TH2D("JetPtNearPhoEtavBin","x:Jet |Eta| vs jet Pt nearest to photon",5,etaLow,7,ptLow2);

  h2_JetPtNearPhoMinDRHadJ=new TH2D("JetPtNearPhoMinDRHadJ","x:minDR b/w a HadJet and photon vs JetPt",sizeof(minDRLow)/sizeof(double)-1,minDRLow,7,ptLow2);
  h2_PhoPtMinDRHadJ=new TH2D("PhoPtMinDRHadJ","x:minDR b/w a HadJet and photon vs PhoPt",sizeof(minDRLow)/sizeof(double)-1,minDRLow,7,ptLow2);
  h2_PtPhoQHadFracJet=new TH2D("PtPhoQHadFracJet","x:Photon Pt vs charged hadron frac of the matching jet",7,ptLow2,sizeof(QHadFracLow)/sizeof(double)-1,QHadFracLow);
  h2_PhoPtQMultJet=new TH2D("PhoPtQMultJet","x: Photon Pt vs charged multiplicity in the matching jet",7,ptLow2,sizeof(QMultLow)/sizeof(double)-1,QMultLow);

  h2_PhoPtVtx=new TH2D("PhoPtVtx","x:Photon Pt vs num of vertices",ptLow3.size()-1,&(ptLow3[0]),nVtxLow.size()-1,&(nVtxLow[0]));
  h2_PhoEtaPhi=new TH2D("PhoEtaPhi","x:Photon Eta, photon phi",60,-6,6,80,-4,4);

  h2_JptGptRatiovsDR=new TH2D("JptGptRatiovsDR","x:minDR b/w jet and photon vs ratio of pt_jet to pt_photon",100,0,1,100,0,10);
  h2_PtGvsMET=new TH2D("PhotonPtvsMET","Photon Pt vs MET",150,0,1500,200,0,2000);
  h2_RecoPhoPt_GenEPt=new TH2D("RecoPhoPt_GenEPt","x:Pt of Gen Electron matching RECO photon vs RECO photon Pt",150,0,1500,150,0,1500);
  h2_HadDRJptGptRatio_Vtx=new TH2D("HadDRJptGptRatio_Vtx","x:HadDR*JetPt/PhotonPt vs NVtx",sizeof(minDR_RatioLow)/sizeof(double)-1,minDR_RatioLow,nVtxLow.size()-1,&(nVtxLow[0]));
  h2_minHadDR_QMult_Pho=new TH2D("minHadDR_QMult_Pho","x:minHad DR vs Qmult in a jet closest to the photon",1000,0,10,50,0,50);

  h3_PhotonPtEtaNJ=new TH3D("PhotonPtEtaNJ","x:Photon |Eta|, y:nHadJets, z: photon Pt",5,etaLow,8,nJEtaYLow,7,ptLow2);
  h3_PhoPtEtaVtx=new TH3D("PhoPtEtaVtx","x:Photon |Eta|, y:nVtx, z: Photon Pt",5,etaLow,nVtxLow.size()-1,&(nVtxLow[0]),7,ptLow2);
  h3_JetPtPhoEtaMinHadDR=new TH3D("JetPtPhoEtaMinHadDR","x:Photon |Eta|, y:matching JetPt, z:MinHadDR",5,etaLow,7,ptLow2,sizeof(minDRLow)/sizeof(double)-1,minDRLow);

  h3_JetPtVtxMinHadDR_Pho=new TH3D("JetPtVtxMinHadDR_Pho","x:Pt of jet close to photon, y:nVtx, z:dR b/w closest HadJet and photon",7,ptLow2,nVtxLow.size()-1,&(nVtxLow[0]),sizeof(minDRLow)/sizeof(double)-1,minDRLow);
  h3_PhoPtVtxMinHadDR=new TH3D("PhoPtVtxMinHadDR","x:Photon pT, y:Vtx, z:MinDR with HadJet",7,ptLow2,nVtxLow.size()-1,&(nVtxLow[0]),sizeof(minDRLow)/sizeof(double)-1,minDRLow);
  h3_PhoPtEtaMinDRHadJ=new TH3D("PhoPtEtaMinDRHadJ","x:Photon pT, y:photon |eta|, z:MinDR with HadJet",7,ptLow2,5,etaLow,sizeof(minDRLow)/sizeof(double)-1,minDRLow);

  h_RemjetPtNearPhoton=new TH1D("RemJetPtNearPhoton","Pt of the Photon matched jet. Photon NOT inluded",200,0,2000);
  h_photon_pfQIsoRhoCorr=new TH1D("photon_pfQIsoRhoCorr","photon pf charged Iso Rho Corrected",500,0,5);
  h_photon_pfNutIsoRhoCorr=new TH1D("photon_pfNutIsoRhoCorr","photon pf neutral Iso Rho Corrected",160,0,16);

  h_ST_Ele=new TH1D("ST_Ele","ST_Ele",400,0,4000);
  h_MET_Ele=new TH1D("MET_Ele","MET_Ele",200,0,2000);
  h_nHadJets_Ele=new TH1D("nHadJets_Ele","no. of jets(only hadronic jets,not counting photon)_Ele",25,0,25);
  h_BTags_Ele=new TH1D("nBTags_Ele","no. of B tags_Ele",10,0,10);
  h_mT_Ele = new TH1D("mT_Ele","mT(MET, electron)",200,0,2000);
  h_METvBin_Ele=new TH1D("METvarBin_Ele","MET with variable bin size_Ele",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele_R[0] = new TH1D("MET_Ele_R1","MET: NJ=2to4 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele_R[1] = new TH1D("MET_Ele_R2","MET: NJ=5or6 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele_R[2] = new TH1D("MET_Ele_R3","MET: NJ>=7 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele_Rall = new TH1D("MET_Ele_Rall","MET: NJets>=2 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_HT_Ele=new TH1D("HT_Ele","HT_Ele",400,0,4000);
  h_MHT_Ele=new TH1D("MHT_Ele","MHT_Ele",200,0,2000);
  h_nJets_Ele=new TH1D("nJets_Ele","nJets_Ele",25,0,25);
  h_madHT_Ele=new TH1D("madHT_Ele","madHT_Ele",400,0,4000);
  h_nVtx_Ele=new TH1D("nVtx_Ele","no. of vertices in electron events",50,0,50);

  h_ElePt=new TH1D("ElePt","Pt of the electron",150,0,1500);
  h_EleEta=new TH1D("EleEta","Eta of the electron",120,-6,6);
  h_ElePhi=new TH1D("ElePhi","Phi of the electron",80,-4,4);
  h_ElePtvBin=new TH1D("ElePtvBin","Electron Pt with variable binning",sizeof(ptLow)/sizeof(double)-1,ptLow);

  h_EleTracks_Ele=new TH1D("nEleTracks_Ele","No. of e tracks for Ele events",30,-0.5,29.5);
  h_MuTracks_Ele=new TH1D("nMuTracks_Ele","No. of #mu tracks for Ele events",30,-0.5,29.5);
  h_PiTracks_Ele=new TH1D("nPiTracks_Ele","No. of #pi tracks for Ele events",30,-0.5,29.5);
  h_nTracks_Ele=new TH1D("nTracks_Ele","No. of tracks for Ele events",50,-0.5,49.5);
  h_nTracks0p3_Ele=new TH1D("nTracks0p3_Ele","No. of tracks within dR < 0.3 for Ele events",50,-0.5,49.5);

  h_minDR_Ele_Jet=new TH1D("minDR_Ele_Jet","min dR b/w a jet and electron",100,0,1);
  h_PtJetNearEle=new TH1D("PtJetNearEle","Pt of the jet nearest to electron",150,0,1500);
  h_JptEleptRatio=new TH1D("JptEleptRatio","Ratio of jet Pt to electron Pt",100,0,10);
  h_HadDR_EleptJptRatio=new TH1D("HadDR_EleptJptRatio","dR(HadJet,elctron) * electronPt/jetPt",1000,0,1);
  h_minDRHadJ_Ele_HadJet=new TH1D("minDR_Ele_HadJet","minDR b/w electron and a HadJet",1000,0,10);
  h_dRGenJet2_Ele=new TH1D("dRGenJet2_Ele","mindR b/w gen jet and electron. Do not consider GenJet that's matched to electron",1000,1,10);
  
  h2_JetPtNearEleMinDRHadJ=new TH2D("JetPtNearEleMinDRHadJ","x:minDR b/w a HadJet and electron vs JetPt",sizeof(minDRLow)/sizeof(double)-1,minDRLow,7,ptLow2);
  h2_ElePtMinDRHadJ=new TH2D("ElePtMinDRHadJ","x:minDR b/w a HadJet and electron vs ElePt",sizeof(minDRLow)/sizeof(double)-1,minDRLow,7,ptLow2);
  h2_PtEleQHadFracJet=new TH2D("PtEleQHadFracJet","x:Electron Pt vs charged hadron frac of the matching jet",7,ptLow2,sizeof(QHadFracLow)/sizeof(double)-1,QHadFracLow);
  h2_ElePtQMultJet=new TH2D("ElePtQMultJet","x: Electron Pt vs charged multiplicity in the matching jet",7,ptLow2,sizeof(QMultLow)/sizeof(double)-1,QMultLow);

  h2_EleEtaPhi=new TH2D("EleEtaPhi","x:Electron Eta, electron phi",60,-6,6,80,-4,4);
  h2_ElePtVtx=new TH2D("ElePtVtx","x:Electron Pt vs num of vertices",ptLow3.size()-1,&(ptLow3[0]),nVtxLow.size()-1,&(nVtxLow[0]));

  h_dPhi_METjet1_Ele=new TH1D("dPhi_METjet1_Ele","dphi between MET Vec and Jet1_Ele",40,0,4);
  h_dPhi_METjet2_Ele=new TH1D("dPhi_METjet2_Ele","dphi between MET Vec and Jet2_Ele",40,0,4);
  h_dPhi_METjet3_Ele=new TH1D("dPhi_METjet3_Ele","dphi between MET Vec and Jet3_Ele",40,0,4);
  h_dPhi_METjet4_Ele=new TH1D("dPhi_METjet4_Ele","dphi between MET Vec and Jet4_Ele",40,0,4);
  h_dPhi_METEle=new TH1D("dPhi_METEle","dphi between MET and Electron",40,0,4);

  h_genEleMT2Act_Ele=new TH1D("genEleMT2Act_Ele","gen MT2 activity for electrons matced to electron",5000,0,50);
  h_EleMT2Activity=new TH1D("EleMT2Activity","selected ID Electrons MT2Activity",5000,0,50);

  h2_ElePtST=new TH2D("ElePtST","x:Electron Pt vs ST",150,0,1500,400,0,4000);
  h2_ElePtEta=new TH2D("ElePtEta","x:Electron |Eta| vs Electron Pt",60,0,6,150,0,1500);
  h2_ElePtEtavBin=new TH2D("ElePtEtavBin","x:Electron |Eta| vs Electron Pt",5,etaLow,7,ptLow2);
  h2_JetPtNearEleEtavBin=new TH2D("JetPtNearEleEtavBin","x:Jet |Eta| vs jet Pt nearest to electron",5,etaLow,7,ptLow2);
  h2_nVtx_EleMT2Act=new TH2D("nVtx_EleMT2","x:nVtx vs electron MT2 activity",50,0,50,5000,0,50);

  h2_JptEleptRatiovsDR=new TH2D("JptEptRatiovsDR","x:minDR b/w jet and electron vs ratio of pt_jet to pt_Ele",100,0,1,100,0,10);
  h2_PtElevsMET=new TH2D("ElePtvsMET","Electron Pt vs MET",150,0,1500,200,0,2000);
  h2_RecoElePt_GenEPt=new TH2D("RecoElePt_GenEPt","x:Pt of Gen Electron matching RECO electron vs RECO electron Pt",150,0,1500,150,0,1500);
  h2_HadDRJptEptRatio_Vtx=new TH2D("HadDRJptEptRatio_Vtx","x:dR*JetPt/ElectronPt vs NVtx",sizeof(minDR_RatioLow)/sizeof(double)-1,minDR_RatioLow,nVtxLow.size()-1,&(nVtxLow[0]));
  h2_minHadDR_QMult_Ele=new TH2D("minHadDR_QMult_Ele","x:minHad DR vs Qmult in a jet closest to the electron",1000,0,10,50,0,50);

  h3_ElePtEtaNJ=new TH3D("ElePtEtaNJ","x:Electron |Eta|, y:nHadJets, z: Electron Pt",5,etaLow,8,nJEtaYLow,7,ptLow2);
  h3_ElePtEtaVtx=new TH3D("ElePtEtaVtx","x:Electron |Eta|, y:nVtx, z: Electron Pt",5,etaLow,nVtxLow.size()-1,&(nVtxLow[0]),7,ptLow2);
  h3_JetPtEleEtaMinHadDR=new TH3D("JetPtEleEtaMinHadDR","x:Electron |Eta|, y:matching JetPt, z:MinHadDR",5,etaLow,7,ptLow2,sizeof(minDRLow)/sizeof(double)-1,minDRLow);
  h3_JetPtVtxMinHadDR_Ele=new TH3D("JetPtVtxMinHadDR_Ele","x:Pt of jet close to electron, y:nVtx, z:dR b/w closest HadJet and electron",7,ptLow2,nVtxLow.size()-1,&(nVtxLow[0]),sizeof(minDRLow)/sizeof(double)-1,minDRLow);
  h3_ElePtVtxMinHadDR=new TH3D("ElePtVtxMinHadDR","x:Electron pT, y:Vtx, z:MinDR with HadJet",7,ptLow2,nVtxLow.size()-1,&(nVtxLow[0]),sizeof(minDRLow)/sizeof(double)-1,minDRLow);
  h3_ElePtEtaMinDRHadJ=new TH3D("ElePtEtaMinDRHadJ","x:Electron pT, y:electron |eta|, z:MinDR with HadJet",7,ptLow2,5,etaLow,sizeof(minDRLow)/sizeof(double)-1,minDRLow);

  h_RemjetPtNearEle=new TH1D("RemJetPtNearEle","Pt of the Ele matched jet. Ele NOT inluded",200,0,2000);

  for(int i=0;i<4;i++){
    sprintf(name,"jet%iPt",i+1);    h_jetPt[i]=new TH1D(name,name,200,0,2000);
    sprintf(name,"jet%iEta",i+1);   h_jetEta[i]=new TH1D(name,name,120,-6,6);
    sprintf(name,"jet%iPhi",i+1);   h_jetPhi[i]=new TH1D(name,name,80,-4,4);
    sprintf(name,"jet%iPt_Pho",i+1);h_jetPt_Pho[i]=new TH1D(name,name,200,0,2000);
    sprintf(name,"jet%iPt_Ele",i+1);h_jetPt_Ele[i]=new TH1D(name,name,200,0,2000);
    sprintf(name,"jet%iEta_Pho",i+1);   h_jetEta_Pho[i]=new TH1D(name,name,120,-6,6);
    sprintf(name,"jet%iEta_Ele",i+1);   h_jetEta_Ele[i]=new TH1D(name,name,120,-6,6);
    sprintf(name,"jet%iPhi_Pho",i+1);   h_jetPhi_Pho[i]=new TH1D(name,name,80,-4,4);
    sprintf(name,"jet%iPhi_Ele",i+1);   h_jetPhi_Ele[i]=new TH1D(name,name,80,-4,4);

    sprintf(name,"dPhi_jet%i_Pho",i+1); h_dPhi_jet_Pho[i]=new TH1D(name,name,40,0,4);
    sprintf(name,"dPhi_jet%i_Ele",i+1); h_dPhi_jet_Ele[i]=new TH1D(name,name,40,0,4);
  }

  h_minDR_Pho_GenObj=new TH1D("minDR_Pho_GenObj","min DR b/w reco photon and gen objects",100,0,1);
  h_GmatchedObj=new TH1D("GmatchedObj","Gen Obj close to Reco-Gamma",62,-0.75,30.25);
  h_PdgIdPhoParent = new TH1D("PdgIdPhoParent","PdgID of the Photon's parent",62,-0.75,30.25);

  h_NM=new TH1D("NeuMuli","neutral multiplicity",50,0,50);
  h_NMEta3=new TH1D("NeuMultEta3","neutral multiplicity for |eta|<3",50,0,50);
  h_NMHF=new TH1D("NeuMultHF","neutral multiplicity for |eta|>=3",50,0,50);
  h_CM=new TH1D("Qmulti","charged multiplicity",50,0,50);
  h_NHF=new TH1D("NeuHadFrac","neutral hadron Frac",220,0,1.1);
  h_NEMF=new TH1D("NeuEMFrac","neutral EM Frac",220,0,1.1);
  h_CHF=new TH1D("QHadFrac","charged hadron Frac",220,0,1.1);
  h_CEMF=new TH1D("QEMFrac","charged EM Frac",220,0,1.1);

  h_PhoMult=new TH1D("Phomulti","photon multiplicity",50,0,50);
  h_PhoFrac=new TH1D("PhoFrac","Photon Frac",220,0,1.1);

  h_CM_Pho=new TH1D("Qmulti_Pho","charged multiplicity in jet matching Pho",50,0,50);
  h_NHF_Pho=new TH1D("NeuHadFrac_Pho","neutral hadron Frac in jet matching Pho",220,0,1.1);
  h_NEMF_Pho=new TH1D("NeuEMFrac_Pho","neutral EM Frac in jet matching Pho",220,0,1.1);
  h_CHF_Pho=new TH1D("QHadFrac_Pho","charged hadron Frac in jet matching Pho",220,0,1.1);
  h_CEMF_Pho=new TH1D("QEMFrac_Pho","charged EM Frac in jet matching Pho",220,0,1.1);
  h_PhoFrac_Pho=new TH1D("PhoFrac_Pho","Photon Frac in jet matching Pho",220,0,1.1);

  h_CM_Ele=new TH1D("Qmulti_Ele","charged multiplicity in jet matching Ele",50,0,50);
  h_NHF_Ele=new TH1D("NeuHadFrac_Ele","neutral hadron Frac in jet matching Ele",220,0,1.1);
  h_NEMF_Ele=new TH1D("NeuEMFrac_Ele","neutral EM Frac in jet matching Ele",220,0,1.1);
  h_CHF_Ele=new TH1D("QHadFrac_Ele","charged hadron Frac in jet matching Ele",220,0,1.1);
  h_CEMF_Ele=new TH1D("QEMFrac_Ele","charged EM Frac in jet matching Ele",220,0,1.1);
  h_PhoFrac_Ele=new TH1D("PhoFrac_Ele","Photon Frac in jet matching Ele",220,0,1.1);

  //------------------search bins-----------------------
  h_MET_R_v2_Ele[0] =new TH1D("MET_R1_v2_Ele","MET for 0b, NJ=2to4 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele[1] =new TH1D("MET_R2_v2_Ele","MET for 0b, NJ>=5 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele[2] =new TH1D("MET_R3_v2_Ele","MET for 1b, NJ=2to4 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele[3] =new TH1D("MET_R4_v2_Ele","MET for 1b, NJ>=5 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Ele[4] =new TH1D("MET_R5_v2_Ele","MET for b>=2 for electron events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));  

  h_MET_R_v2_Pho[0] =new TH1D("MET_R1_v2_Pho","MET for 0b, NJ=2to4 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Pho[1] =new TH1D("MET_R2_v2_Pho","MET for 0b, NJ>=5 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Pho[2] =new TH1D("MET_R3_v2_Pho","MET for 1b, NJ=2to4 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Pho[3] =new TH1D("MET_R4_v2_Pho","MET for 1b, NJ>=5 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R_v2_Pho[4] =new TH1D("MET_R5_v2_Pho","MET for b>=2 for photon like events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));  

  h_SBins_Ele = new TH1D("AllSBins_Ele","all search bins:(0b, NJ=2to4)(0b, NJ>=5)(1b, NJ=2to4)(1b, NJ>=5)(b>=2) for electron events",34,0.5,34.5);
  h_SBins_Pho = new TH1D("AllSBins_Pho","all search bins:(0b, NJ=2to4)(0b, NJ>=5)(1b, NJ=2to4)(1b, NJ>=5)(b>=2) for photon like events",34,0.5,34.5);
  h_temp=new TH1D("temp","temp",200,0,2000);
}


FakeRateEst::FakeRateEst(const TString &inputFileList, const char *outFileName, const char* dataset) {
  string nameData=dataset;//vvv
  TChain *tree = new TChain("PreSelection");
  //  tree = new TChain("TreeMaker2/PreSelection");//vvv
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

Bool_t FakeRateEst::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t FakeRateEst::LoadTree(Long64_t entry) {
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

FakeRateEst::~FakeRateEst() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

