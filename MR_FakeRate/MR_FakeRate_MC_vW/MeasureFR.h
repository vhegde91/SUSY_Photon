#ifndef MeasureFR_H
#define MeasureFR_H

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

class MeasureFR : public NtupleVariables{

 public:
  MeasureFR(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~MeasureFR();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);
  TLorentzVector getBestPhoton();
  bool check_eMatchedtoGamma();
  void print(Long64_t);
  void findObjMatchedtoG(TLorentzVector);
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
  /* vector<double> METBinLowEdge2={0,100,150,200,350,5000}; */
  /* vector<double> STBinLowEdge={0,500,700,1000,10000}; */
  /* vector<double> nBTagsBinLowEdge={0,1,2,10}; */
  /* vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750}; */
  double ptLow2[8]={0,100,150,200,250,300,500,20000};
  double QMultLow[6]={0,2,4,7,10,100};
  vector <TLorentzVector> allBestPhotons;
  //histograms
  TH1I *h_RunNum;
  TH1D *h_intLumi;
  TH1D *h_ST;
  TH1D *h_MET;
  TH1D *h_nHadJets;
  TH1D *h_BTags;
  TH1D *h_METvBin;
  TH1D *h_ZMass;
  TH1D *h_METPhi;
  //--------------------- ge(fake) ----------------
  TH1D *h_LeadEleIsProbe;
  TH1D *h_ST_ee;
  TH1D *h_MET_ee;
  TH1D *h_nHadJets_ee;
  TH1D *h_BTags_ee;
  TH1D *h_nJets_ee;
  TH1D *h_METvBin_ee;
  TH1D *h_ZMass_ee;
  TH1D *h_METPhi_ee;
  TH1D *h_nVtx_ee;
  TH1D *h_isoEleTracks_ee;

  TH1D *h_ElefPt_ee;
  TH1D *h_ElefEta_ee;
  TH1D *h_ElefPhi_ee;

  TH1D *h_ElerPt_ee;
  TH1D *h_ElerEta_ee;
  TH1D *h_ElerPhi_ee;

  TH1D *h_dPhi1_ee;
  TH1D *h_dPhi2_ee;
  TH1D *h_dphi_elefMET_ee;
  TH1D *h_dPhi_elerMET_ee;
  TH1D *h_dphi_elef_eler_ee;

  TH1D *h_minDRJetElef_ee;
  TH1D *h_QMultJetElef_ee;
  TH1D *h_QMultJetEler_ee;

  TH2D *h2_ElefPtQMultJet_ee;
  TH2D *h2_ElefPtElerPt_ee;
  //--------------------- ge(fake) ----------------
  TH1D *h_ST_ge;
  TH1D *h_MET_ge;
  TH1D *h_nHadJets_ge;
  TH1D *h_BTags_ge;
  TH1D *h_nJets_ge;
  TH1D *h_METvBin_ge;
  TH1D *h_ZMass_ge;
  TH1D *h_METPhi_ge;
  TH1D *h_nVtx_ge;
  TH1D *h_isoEleTracks_ge;

  TH1D *h_ElefPt_ge;
  TH1D *h_ElefEta_ge;
  TH1D *h_ElefPhi_ge;

  TH1D *h_ElerPt_ge;
  TH1D *h_ElerEta_ge;
  TH1D *h_ElerPhi_ge;

  TH1D *h_minDRJetElef_ge;
  TH1D *h_QMultJetElef_ge;
  TH1D *h_QMultJetEler_ge;

  TH1D *h_dPhi1_ge;
  TH1D *h_dPhi2_ge;
  TH1D *h_dphi_elefMET_ge;
  TH1D *h_dPhi_elerMET_ge;
  TH1D *h_dphi_elef_eler_ge;

  TH2D *h2_ElefPtQMultJet_ge;
  TH2D *h2_ElefPtElerPt_ge;
  TH1D *h_Pho2Pt;
  //---------------- search bins -----------------------                                                                        
  TH1D *h_GmatchedObj;
  TH1D *h_PdgIdPhoParent;

  TH1D *h_MET_ee_R[3];
  TH1D *h_MET_ge_R[3];

  TFile *oFile;
 
};
#endif

#ifdef MeasureFR_cxx

void MeasureFR::BookHistogram(const char *outFileName) {

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
  h_ZMass=new TH1D("ZMass","Inv mass of Z for e-e or #gamma-e events",500,0,500);
  h_METPhi=new TH1D("METPhi","METPhi",40,-4,4);
  //---------------------------- ee ------------------------------------
  h_LeadEleIsProbe=new TH1D("LeadEleIsProbe","Is leading electron probe for events with 2 tight ele",3,0,3);
  h_ST_ee=new TH1D("ST_ee","ST for ee events",400,0,4000);
  h_MET_ee=new TH1D("MET_ee","MET for ee events",200,0,2000);
  h_nHadJets_ee=new TH1D("nHadJets_ee","nHadJets for ee events",25,-0.5,24.5);
  h_BTags_ee=new TH1D("nBTags_ee","no. of B-tags for ee events",10,-0.5,9.5);
  h_nJets_ee=new TH1D("nJets_ee","no. of jets(not cleaned)",25,-0.5,24.5);
  h_METvBin_ee=new TH1D("METvBin_ee","MET in variable bins for ee events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_ZMass_ee=new TH1D("ZMass_ee","ZMass for ee events",500,0,500);
  h_METPhi_ee=new TH1D("METPhi_ee","MET Phi for ee events",40,-4,4);
  h_nVtx_ee=new TH1D("nVtx_ee","no. of primary vertices for ee events",50,-0.5,49.5);
  h_isoEleTracks_ee=new TH1D("isoEleTracks_ee","no. of isolated e tracks for ee events",10,-0.5,9.5);

  h_ElefPt_ee=new TH1D("F_ElePt_ee","Electron_f Pt for ee events",200,0,2000);
  h_ElefEta_ee=new TH1D("F_EleEta_ee","Electron_f eta for ee events",120,-6,6);
  h_ElefPhi_ee=new TH1D("F_ElePhi_ee","Electron_f phi for ee events",40,-4,4);

  h_ElerPt_ee=new TH1D("R_ElePt_ee","Electron_r Pt for ee events",200,0,2000);
  h_ElerEta_ee=new TH1D("R_EleEta_ee","Electron_r eta for ee events",120,-6,6);
  h_ElerPhi_ee=new TH1D("R_ElePhi_ee","Electron_r phi for ee events",40,-4,4);

  h_dPhi1_ee=new TH1D("dPhi1_ee","dPhi(jet1,MET) for ee events",40,0,4);
  h_dPhi2_ee=new TH1D("dPhi2_ee","dPhi(jet2,MET) for ee events",40,0,4);
  h_dphi_elefMET_ee=new TH1D("dphi_elefMET_ee","dPhi(MET,e_f) for ee events",40,0,4);
  h_dPhi_elerMET_ee=new TH1D("dphi_elerMET_ee","dPhi(MET,e_r) for ee events",40,0,4);
  h_dphi_elef_eler_ee=new TH1D("dPhi_elef_eler_ee","dPhi(e_r,e_f) for ee events",40,0,4);

  h_minDRJetElef_ee=new TH1D("minDRJetElef_ee","min dR(Jet,e_f) for ee events",1000,0,10);
  h_QMultJetElef_ee=new TH1D("QMultJetElef_ee","Q mult in jet closest to e_f",100,0,100);
  h_QMultJetEler_ee=new TH1D("QMultJetEler_ee","Q mult in jet closest to e_r",100,0,100);

  h2_ElefPtElerPt_ee=new TH2D("ElefPtElerPt_ee","x:F_ElePt vs R_ElePt for ee events",200,0,2000,200,0,2000);
  h2_ElefPtQMultJet_ee=new TH2D("F_ElePtQMultJet_ee","x:Pt of electron_f vs QMult in closest jet for ee events",7,ptLow2,sizeof(QMultLow)/sizeof(double)-1,QMultLow);
  //---------------------------- ge ------------------------------------
  h_ST_ge=new TH1D("ST_ge","ST for e(->#gamma)e events",400,0,4000);
  h_MET_ge=new TH1D("MET_ge","MET for e(->#gamma)e events",200,0,2000);
  h_nHadJets_ge=new TH1D("nHadJets_ge","nHadJets for e(->#gamma)e events",25,-0.5,24.5);
  h_BTags_ge=new TH1D("nBTags_ge","no. of B-tags for e(->#gamma)e events",10,-0.5,9.5);
  h_nJets_ge=new TH1D("nJets_ge","no. of jets(not cleaned)",25,-0.5,24.5);
  h_METvBin_ge=new TH1D("METvBin_ge","MET in variable bins for e(->#gamma)e events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_ZMass_ge=new TH1D("ZMass_ge","ZMass for e(->#gamma)e events",500,0,500);
  h_METPhi_ge=new TH1D("METPhi_ge","MET Phi for e(->#gamma)e events",40,-4,4);
  h_nVtx_ge=new TH1D("nVtx_ge","no. of primary vertices for e(->#gamma)e events",50,-0.5,49.5);
  h_isoEleTracks_ge=new TH1D("isoEleTracks_ge","no. of isolated e tracks for e(->#gamma)e events",10,-0.5,9.5);

  h_ElefPt_ge=new TH1D("F_ElePt_ge","Electron_f Pt for e(->#gamma)e events",200,0,2000);
  h_ElefEta_ge=new TH1D("F_Eleeta_ge","Electron_f eta for e(->#gamma)e events",120,-6,6);
  h_ElefPhi_ge=new TH1D("F_ElePhi_ge","Electron_f phi for e(->#gamma)e events",40,-4,4);

  h_ElerPt_ge=new TH1D("R_ElePt_ge","Electron_r Pt for e(->#gamma)e events",200,0,2000);
  h_ElerEta_ge=new TH1D("R_EleEta_ge","Electron_r eta for e(->#gamma)e events",120,-6,6);
  h_ElerPhi_ge=new TH1D("R_ElePhi_ge","Electron_r phi for e(->#gamma)e events",40,-4,4);

  h_dPhi1_ge=new TH1D("dPhi1_ge","dPhi(jet1,MET) for e(->#gamma)e events",40,0,4);
  h_dPhi2_ge=new TH1D("dPhi2_ge","dPhi(jet2,MET) for e(->#gamma)e events",40,0,4);
  h_dphi_elefMET_ge=new TH1D("dphi_elefMET_ge","dPhi(MET,photon) for e(->#gamma)e events",40,0,4);
  h_dPhi_elerMET_ge=new TH1D("dphi_elerMET_ge","dPhi(MET,e) for e(->#gamma)e events",40,0,4);
  h_dphi_elef_eler_ge=new TH1D("dPhi_elef_eler_ge","dPhi(photon,e) for e(->#gamma)e events",40,0,4);

  h_minDRJetElef_ge=new TH1D("minDRJetElef_ge","min dR(Jet,e_f) for e(->#gamma)e events",1000,0,10);
  h_QMultJetElef_ge=new TH1D("QMultJetElef_ge","Q mult in jet closest to e_f",100,0,100);
  h_QMultJetEler_ge=new TH1D("QMultJetEler_ge","Q mult in jet closest to e_r",100,0,100);

  h2_ElefPtElerPt_ge=new TH2D("ElefPtElerPt_ge","x:F_ElePt(Photon) vs R_ElePt for e(->#gamma)e events",200,0,2000,200,0,2000);
  h2_ElefPtQMultJet_ge=new TH2D("F_ElePtQMultJet_ge","x:Pt of electron_f vs QMult in closest jet for e(->#gamma)e events",7,ptLow2,sizeof(QMultLow)/sizeof(double)-1,QMultLow);

  h_GmatchedObj=new TH1D("GmatchedObj","Gen Obj close to Reco-Gamma",62,-0.75,30.25);
  h_PdgIdPhoParent = new TH1D("PdgIdPhoParent","PdgID of the Photon's parent",62,-0.75,30.25);
  h_Pho2Pt = new TH1D("Pho2Pt","2nd Photon Pt for e(->#gamma)e events",200,0,2000);
  //********************************************************
  h_MET_ee_R[0] = new TH1D("MET_ee_R1","MET: NJ=2to4, for ee events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_ee_R[1] = new TH1D("MET_ee_R2","MET: NJ=5or6, for ee events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_ee_R[2] = new TH1D("MET_ee_R3","MET: NJ>=7,   for ee events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_MET_ge_R[0] = new TH1D("MET_ge_R1","MET: NJ=2to4, for e(->#gamma)e events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_ge_R[1] = new TH1D("MET_ge_R2","MET: NJ=5or6, for e(->#gamma)e events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_ge_R[2] = new TH1D("MET_ge_R3","MET: NJ>=7,   for e(->#gamma)e events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  
}


MeasureFR::MeasureFR(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

Bool_t MeasureFR::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t MeasureFR::LoadTree(Long64_t entry) {
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

MeasureFR::~MeasureFR() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

