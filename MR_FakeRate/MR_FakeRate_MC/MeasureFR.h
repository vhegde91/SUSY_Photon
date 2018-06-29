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
  //  double ptLow2[8]={0,100,150,200,250,300,500,20000};//a good one
  double ptLow2[8]={0,100,140,190,240,300,500,20000};
  vector<double> ptLow3={0,100,130,190,300,20000};
  vector<double> ptLow4={0,100,120,150,200,250,400,600};
  double QMultLow[6]={0,2,4,7,10,100};
  vector<double> nVtxLow={0,15,20,25,30,100};
  double etaLow[6]={0, 0.5, 1.0, 1.48, 1.65, 3.0};
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
  TH1D *h_1Bin_Ele;
  TH1D *h_ST_Ele;
  TH1D *h_MET_Ele;
  TH1D *h_nHadJets_Ele;
  TH1D *h_BTags_Ele;
  TH1D *h_nJets_Ele;
  TH1D *h_METvBin_Ele;
  TH1D *h_ZMass_Ele;
  TH1D *h_METPhi_Ele;
  TH1D *h_nVtx_Ele;
  TH1D *h_isoEleTracks_Ele;
  TH1D *h_EleTracks_Ele;
  TH1D *h_MuTracks_Ele;
  TH1D *h_PiTracks_Ele;
  TH1D *h_nTracks_Ele;

  TH1D *h_ElefPt_Ele;
  TH1D *h_ElefEta_Ele;
  TH1D *h_ElefPhi_Ele;
  TH1D *h_ElefPtvBin_Ele;

  TH1D *h_ElerPt_Ele;
  TH1D *h_ElerEta_Ele;
  TH1D *h_ElerPhi_Ele;

  TH1D *h_dPhi1_Ele;
  TH1D *h_dPhi2_Ele;
  TH1D *h_dphi_elefMET_Ele;
  TH1D *h_dPhi_elerMET_Ele;
  TH1D *h_dphi_elef_eler_Ele;
  TH1D *h_mTPrbMET_Ele;

  TH1D *h_minDRJetElef_Ele;
  TH1D *h_QMultJetElef_Ele;
  TH1D *h_QMultJetEler_Ele;

  TH2D *h2_ElefPtQMultJet_Ele;
  TH2D *h2_ElefPtElerPt_Ele;
  //--------------------- ge(fake) ----------------
  TH1D *h_1Bin_Pho;
  TH1D *h_ST_Pho;
  TH1D *h_MET_Pho;
  TH1D *h_nHadJets_Pho;
  TH1D *h_BTags_Pho;
  TH1D *h_nJets_Pho;
  TH1D *h_METvBin_Pho;
  TH1D *h_ZMass_Pho;
  TH1D *h_METPhi_Pho;
  TH1D *h_nVtx_Pho;
  TH1D *h_isoEleTracks_Pho;
  TH1D *h_EleTracks_Pho;
  TH1D *h_MuTracks_Pho;
  TH1D *h_PiTracks_Pho;
  TH1D *h_nTracks_Pho;

  TH1D *h_ElefPt_Pho;
  TH1D *h_ElefEta_Pho;
  TH1D *h_ElefPhi_Pho;
  TH1D *h_ElefPtvBin_Pho;

  TH1D *h_ElerPt_Pho;
  TH1D *h_ElerEta_Pho;
  TH1D *h_ElerPhi_Pho;

  TH1D *h_minDRJetElef_Pho;
  TH1D *h_QMultJetElef_Pho;
  TH1D *h_QMultJetEler_Pho;

  TH1D *h_dPhi1_Pho;
  TH1D *h_dPhi2_Pho;
  TH1D *h_dphi_elefMET_Pho;
  TH1D *h_dPhi_elerMET_Pho;
  TH1D *h_dphi_elef_eler_Pho;
  TH1D *h_mTPrbMET_Pho;

  TH2D *h2_ElefPtQMultJet_Pho;
  TH2D *h2_ElefPtElerPt_Pho;
  TH1D *h_Pho2Pt;
  //----------------------------------------
  TH2D *h2_PhoPtVtx,*h2_ElePtVtx;
  TH2D *h2_PhoPtEtavBin,*h2_ElePtEtavBin;
  //---------------- search bins -----------------------                                                                        
  TH1D *h_GmatchedObj;
  TH1D *h_PdgIdPhoParent;

  TH1D *h_MET_Ele_R[3];
  TH1D *h_MET_Pho_R[3];

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
  h_1Bin_Ele=new TH1D("bin1_Ele","1 bin for ee",3,0,3);
  h_ST_Ele=new TH1D("ST_Ele","ST for ee events",400,0,4000);
  h_MET_Ele=new TH1D("MET_Ele","MET for ee events",200,0,2000);
  h_nHadJets_Ele=new TH1D("nHadJets_Ele","nHadJets for ee events",25,0,25);
  h_BTags_Ele=new TH1D("nBTags_Ele","no. of B-tags for ee events",10,0,10);
  h_nJets_Ele=new TH1D("nJets_Ele","no. of jets(not cleaned)",25,0,25);
  h_METvBin_Ele=new TH1D("METvBin_Ele","MET in variable bins for ee events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_ZMass_Ele=new TH1D("ZMass_Ele","ZMass for ee events",500,0,500);
  h_METPhi_Ele=new TH1D("METPhi_Ele","MET Phi for ee events",40,-4,4);
  h_nVtx_Ele=new TH1D("nVtx_Ele","no. of primary vertices for ee events",50,-0.5,49.5);
  h_isoEleTracks_Ele=new TH1D("isoEleTracks_Ele","no. of isolated e tracks for ee events",10,-0.5,9.5);

  h_EleTracks_Ele=new TH1D("nEleTracks_Ele","No. of e tracks for ee events",30,-0.5,29.5);
  h_MuTracks_Ele=new TH1D("nMuTracks_Ele","No. of #mu tracks for ee events",30,-0.5,29.5);
  h_PiTracks_Ele=new TH1D("nPiTracks_Ele","No. of #pi tracks for ee events",30,-0.5,29.5);
  h_nTracks_Ele=new TH1D("nTracks_Ele","No. of tracks for ee events",50,-0.5,49.5);

  h_ElefPt_Ele=new TH1D("F_ElePt_Ele","Electron_f Pt for ee events",150,0,1500);
  h_ElefEta_Ele=new TH1D("F_EleEta_Ele","Electron_f eta for ee events",120,-6,6);
  h_ElefPhi_Ele=new TH1D("F_ElePhi_Ele","Electron_f phi for ee events",40,-4,4);
  h_ElefPtvBin_Ele=new TH1D("F_ElePtvBin_Ele","Electron_f Pt var bins for ee events",ptLow4.size()-1,&(ptLow4[0]));

  h_ElerPt_Ele=new TH1D("R_ElePt_Ele","Electron_r Pt for ee events",150,0,1500);
  h_ElerEta_Ele=new TH1D("R_EleEta_Ele","Electron_r eta for ee events",120,-6,6);
  h_ElerPhi_Ele=new TH1D("R_ElePhi_Ele","Electron_r phi for ee events",40,-4,4);

  h_dPhi1_Ele=new TH1D("dPhi1_Ele","dPhi(jet1,MET) for ee events",40,0,4);
  h_dPhi2_Ele=new TH1D("dPhi2_Ele","dPhi(jet2,MET) for ee events",40,0,4);
  h_dphi_elefMET_Ele=new TH1D("dphi_elefMET_Ele","dPhi(MET,e_f) for ee events",40,0,4);
  h_dPhi_elerMET_Ele=new TH1D("dphi_elerMET_Ele","dPhi(MET,e_r) for ee events",40,0,4);
  h_dphi_elef_eler_Ele=new TH1D("dPhi_elef_eler_Ele","dPhi(e_r,e_f) for ee events",40,0,4);
  h_mTPrbMET_Ele=new TH1D("mTProbeMET_Ele","mT(probe em obj,MET) for ee events",200,0,2000);

  h_minDRJetElef_Ele=new TH1D("minDRJetElef_Ele","min dR(Jet,e_f) for ee events",1000,0,10);
  h_QMultJetElef_Ele=new TH1D("QMultJetElef_Ele","Q mult in jet closest to e_f",50,0,50);
  h_QMultJetEler_Ele=new TH1D("QMultJetEler_Ele","Q mult in jet closest to e_r",50,0,50);

  h2_ElefPtElerPt_Ele=new TH2D("ElefPtElerPt_Ele","x:F_ElePt vs R_ElePt for ee events",200,0,2000,200,0,2000);
  h2_ElefPtQMultJet_Ele=new TH2D("F_ElePtQMultJet_Ele","x:Pt of electron_f vs QMult in closest jet for ee events",7,ptLow2,sizeof(QMultLow)/sizeof(double)-1,QMultLow);
  //---------------------------- ge ------------------------------------
  h_1Bin_Pho=new TH1D("bin1_Pho","1 bin for ge",3,0,3);
  h_ST_Pho=new TH1D("ST_Pho","ST for e(->#gamma)e events",400,0,4000);
  h_MET_Pho=new TH1D("MET_Pho","MET for e(->#gamma)e events",200,0,2000);
  h_nHadJets_Pho=new TH1D("nHadJets_Pho","nHadJets for e(->#gamma)e events",25,0,25);
  h_BTags_Pho=new TH1D("nBTags_Pho","no. of B-tags for e(->#gamma)e events",10,0,10);
  h_nJets_Pho=new TH1D("nJets_Pho","no. of jets(not cleaned)",25,0,25);
  h_METvBin_Pho=new TH1D("METvBin_Pho","MET in variable bins for e(->#gamma)e events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_ZMass_Pho=new TH1D("ZMass_Pho","ZMass for e(->#gamma)e events",500,0,500);
  h_METPhi_Pho=new TH1D("METPhi_Pho","MET Phi for e(->#gamma)e events",40,-4,4);
  h_nVtx_Pho=new TH1D("nVtx_Pho","no. of primary vertices for e(->#gamma)e events",50,-0.5,49.5);
  h_isoEleTracks_Pho=new TH1D("isoEleTracks_Pho","no. of isolated e tracks for e(->#gamma)e events",10,-0.5,9.5);

  h_EleTracks_Pho=new TH1D("nEleTracks_Pho","No. of e tracks for ge events",30,-0.5,29.5);
  h_MuTracks_Pho=new TH1D("nMuTracks_Pho","No. of #mu tracks for ge events",30,-0.5,29.5);
  h_PiTracks_Pho=new TH1D("nPiTracks_Pho","No. of #pi tracks for ge events",30,-0.5,29.5);
  h_nTracks_Pho=new TH1D("nTracks_Pho","No. of tracks for ge events",50,-0.5,49.5);

  h_ElefPt_Pho=new TH1D("F_ElePt_Pho","Electron_f Pt for e(->#gamma)e events",150,0,1500);
  h_ElefEta_Pho=new TH1D("F_Eleeta_Pho","Electron_f eta for e(->#gamma)e events",120,-6,6);
  h_ElefPhi_Pho=new TH1D("F_ElePhi_Pho","Electron_f phi for e(->#gamma)e events",40,-4,4);
  h_ElefPtvBin_Pho=new TH1D("F_ElePtvBin_Pho","Electron_f Pt var bins for e(->#gamma)e events",ptLow4.size()-1,&(ptLow4[0]));

  h_ElerPt_Pho=new TH1D("R_ElePt_Pho","Electron_r Pt for e(->#gamma)e events",150,0,1500);
  h_ElerEta_Pho=new TH1D("R_EleEta_Pho","Electron_r eta for e(->#gamma)e events",120,-6,6);
  h_ElerPhi_Pho=new TH1D("R_ElePhi_Pho","Electron_r phi for e(->#gamma)e events",40,-4,4);

  h_dPhi1_Pho=new TH1D("dPhi1_Pho","dPhi(jet1,MET) for e(->#gamma)e events",40,0,4);
  h_dPhi2_Pho=new TH1D("dPhi2_Pho","dPhi(jet2,MET) for e(->#gamma)e events",40,0,4);
  h_dphi_elefMET_Pho=new TH1D("dphi_elefMET_Pho","dPhi(MET,photon) for e(->#gamma)e events",40,0,4);
  h_dPhi_elerMET_Pho=new TH1D("dphi_elerMET_Pho","dPhi(MET,e) for e(->#gamma)e events",40,0,4);
  h_dphi_elef_eler_Pho=new TH1D("dPhi_elef_eler_Pho","dPhi(photon,e) for e(->#gamma)e events",40,0,4);
  h_mTPrbMET_Pho=new TH1D("mTProbeMET_Pho","mT(probe em obj,MET) for ge events",200,0,2000);

  h_minDRJetElef_Pho=new TH1D("minDRJetElef_Pho","min dR(Jet,e_f) for e(->#gamma)e events",1000,0,10);
  h_QMultJetElef_Pho=new TH1D("QMultJetElef_Pho","Q mult in jet closest to e_f",50,0,50);
  h_QMultJetEler_Pho=new TH1D("QMultJetEler_Pho","Q mult in jet closest to e_r",50,0,50);

  h2_ElefPtElerPt_Pho=new TH2D("ElefPtElerPt_Pho","x:F_ElePt(Photon) vs R_ElePt for e(->#gamma)e events",200,0,2000,200,0,2000);
  h2_ElefPtQMultJet_Pho=new TH2D("F_ElePtQMultJet_Pho","x:Pt of electron_f vs QMult in closest jet for e(->#gamma)e events",7,ptLow2,sizeof(QMultLow)/sizeof(double)-1,QMultLow);

  h_GmatchedObj=new TH1D("GmatchedObj","Gen Obj close to Reco-Gamma",62,-0.75,30.25);
  h_PdgIdPhoParent = new TH1D("PdgIdPhoParent","PdgID of the Photon's parent",62,-0.75,30.25);
  h_Pho2Pt = new TH1D("Pho2Pt","2nd Photon Pt for e(->#gamma)e events",200,0,2000);
  //---------------------------------------------------
  h2_PhoPtVtx=new TH2D("PhoPtVtx","x:Photon Pt vs num of vertices for e(->#gamma)e events",ptLow3.size()-1,&(ptLow3[0]),nVtxLow.size()-1,&(nVtxLow[0]));
  h2_ElePtVtx=new TH2D("ElePtVtx","x:Electron Pt vs num of vertices for ee events",ptLow3.size()-1,&(ptLow3[0]),nVtxLow.size()-1,&(nVtxLow[0]));
  h2_PhoPtEtavBin=new TH2D("PhotonPtEtavBin","x:Photon |Eta| vs Photon Pt for e(->#gamma)e events",5,etaLow,7,ptLow2);
  h2_ElePtEtavBin=new TH2D("ElePtEtavBin","x:Electron |Eta| vs Electron Pt for ee events",5,etaLow,7,ptLow2);
  //********************************************************
  h_MET_Ele_R[0] = new TH1D("MET_Ele_R1","MET: NJ=2to4, for ee events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele_R[1] = new TH1D("MET_Ele_R2","MET: NJ=5or6, for ee events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Ele_R[2] = new TH1D("MET_Ele_R3","MET: NJ>=7,   for ee events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_MET_Pho_R[0] = new TH1D("MET_Pho_R1","MET: NJ=2to4, for e(->#gamma)e events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Pho_R[1] = new TH1D("MET_Pho_R2","MET: NJ=5or6, for e(->#gamma)e events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_Pho_R[2] = new TH1D("MET_Pho_R3","MET: NJ>=7,   for e(->#gamma)e events",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  
}


MeasureFR::MeasureFR(const TString &inputFileList, const char *outFileName, const char* dataset) {
  string nameData=dataset;//vvv
  TChain *tree = new TChain("PreSelection");
  //  tree = new TChain("TreeMaker2/PreSelection");//vvv
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

