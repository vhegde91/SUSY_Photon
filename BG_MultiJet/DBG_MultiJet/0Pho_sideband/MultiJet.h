#ifndef MultiJet_H
#define MultiJet_H

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

class MultiJet : public NtupleVariables{

 public:
  MultiJet(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~MultiJet();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);
  TLorentzVector getBestPhoton();
  //  TLorentzVector getIDFailPhoton();
  bool check_eMatchedtoGamma();
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
  //vector<double> HTBinLowEdge ={0,500,550,600,650,700,750,800,900,1000,1100,1200,1400,1600,1800,2000,2400,2800,3200};
  //  vector<double> HTBinLowEdge ={0,100,200,300,400,500,600,700,800,900,1000,1200,1500,2000,2500};
  //  vector<double> MHTBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,500};
  vector<double> STBinLowEdge ={0,300,360,420,500,600,700,850,1000,1200,1500,2000,2500,3000};
  vector<double> METBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,450,600,750,900,1200};
  vector<double> METBinLowEdge2={0,20,40,60,80,100,120,160,200,270,350,450,500};
  vector<double> METBinLowEdgeV4_njLow={0,100,200,270,350,450,750,900};
  vector<double> METBinLowEdgeV4={0,100,200,270,350,450,750};
  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  //vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,200,250,300,350,400,500,600};
  vector<double> METLowSimple={0,100,200,15000};
  vector<double> nJBinLow={0,1,2,3,4,5,6,7,20};
  vector<double> STBinLow2={0,400,500,600,700,800,950,1200,1500,1800,2400,3000,4000,15000};
  vector<double> dPhiBinLow={0,0.3,0.6,2,2.3,2.6,2.9,3.15};
  vector<TLorentzVector> allBestPhotons;
  //histograms
  TH1I *h_RunNum;
  TH1D *h_intLumi;
  TH1D *h_madHT;

  TH1D *h_ST_AB,*h_ST_CD,*h_ST_[4];
  TH1D *h_MET_AB,*h_MET_CD,*h_MET_[4];
  TH1D *h_nHadJets_AB,*h_nHadJets_CD,*h_nHadJets_[4];
  TH1D *h_BTags_AB,*h_BTags_CD,*h_BTags_[4];
  TH1D *h_GenMET_AB,*h_GenMET_CD,*h_GenMET_[4];
  TH1D *h_myHT_AB,*h_myHT_CD,*h_myHT_[4];
  TH1D *h_METPhi_AB,*h_METPhi_CD,*h_METPhi_[4];

  TH1D *h_HT_AB,*h_HT_CD,*h_HT_[4];
  TH1D *h_MHT_AB,*h_MHT_CD,*h_MHT_[4];
  TH1D *h_nJets_AB,*h_nJets_CD,*h_nJets_[4];

  TH1D *h_STvBin_AB,*h_STvBin_CD,*h_STvBin_[4];
  TH1D *h_METvBin_AB,*h_METvBin_CD,*h_METvBin_[4];
  TH1D *h_SBins_v1_AB,*h_SBins_v1_CD,*h_SBins_v1_[4];
  TH1D *h_SBins_v4_AB,*h_SBins_v4_CD,*h_SBins_v4_[4];

  TH1D *h_jet1Pt_AB,*h_jet1Pt_CD,*h_jet1Pt_[4];
  TH1D *h_jet2Pt_AB,*h_jet2Pt_CD,*h_jet2Pt_[4];
  TH1D *h_PtjetNearMET_AB,*h_PtjetNearMET_CD,*h_PtjetNearMET_[4];

  TH1D *h_dPhi_METjet1_AB,*h_dPhi_METjet1_CD,*h_dPhi_METjet1_[4];
  TH1D *h_dPhi_METjet2_AB,*h_dPhi_METjet2_CD,*h_dPhi_METjet2_[4];
  TH1D *h_dPhi_METjet3_AB,*h_dPhi_METjet3_CD,*h_dPhi_METjet3_[4];
  TH1D *h_dPhi_METjet4_AB,*h_dPhi_METjet4_CD,*h_dPhi_METjet4_[4];

  TH1D *h_BestPhotonPt_AB,*h_BestPhotonPt_CD,*h_BestPhotonPt_[4];
  TH1D *h_BestPhotonPtvBin_AB,*h_BestPhotonPtvBin_CD,*h_BestPhotonPtvBin_[4];
  TH1D *h_BestG_Eta_AB,*h_BestG_Eta_CD,*h_BestG_Eta_[4];
  TH1D *h_BestG_Phi_AB,*h_BestG_Phi_CD,*h_BestG_Phi_[4];
  TH1D *h_dPhi_METBestPhoton_AB,*h_dPhi_METBestPhoton_CD,*h_dPhi_METBestPhoton_[4];
  TH1D *h_dPhiPhotonJet1_AB,*h_dPhiPhotonJet1_CD,*h_dPhiPhotonJet1_[4];
  TH1D *h_QIsoRhoCorrEB_AB,*h_QIsoRhoCorrEB_CD,*h_QIsoRhoCorrEB_[4];
  TH1D *h_sieieEB_AB,*h_sieieEB_CD,*h_sieieEB_[4];
  TH1D *h_QIsoRhoCorrEE_AB,*h_QIsoRhoCorrEE_CD,*h_QIsoRhoCorrEE_[4];
  TH1D *h_sieieEE_AB,*h_sieieEE_CD,*h_sieieEE_[4];
  TH1D *h_PhoFracJet1_AB,*h_PhoFracJet1_CD,*h_PhoFracJet1_[4];
  TH1D *h_PhoFracPhoJet_AB,*h_PhoFracPhoJet_CD,*h_PhoFracPhoJet_[4];
  TH1D *h_NuEMFracPhoJet_AB,*h_NuEMFracPhoJet_CD,*h_NuEMFracPhoJet_[4];
  TH1D *h_mTPho_AB,*h_mTPho_CD,*h_mTPho_[4];
  TH1D *h_PhoHoverE_AB,*h_PhoHoverE_CD,*h_PhoHoverE_[4];

  TH2D *h2_PtPhotonvsMET_AB,*h2_PtPhotonvsMET_CD,*h2_PtPhotonvsMET_[4];
  TH2D *h2_dPhi1dPhi2_AB,*h2_dPhi1dPhi2_CD,*h2_dPhi1dPhi2_[4];
  TH2D *h2_NJST_AB,*h2_NJST_CD,*h2_NJST_[4];
  TH2D *h2_dPhiMETPho_NJ_AB,*h2_dPhiMETPho_NJ_CD,*h2_dPhiMETPho_NJ_[4];
  TH2D *h2_PhoPtPhoJetPt_AB,*h2_PhoPtPhoJetPt_CD,*h2_PhoPtPhoJetPt_[4];
  TH2D *h2_QIsoSieieEB_AB,*h2_QIsoSieieEB_CD,*h2_QIsoSieieEB_[4];
  TH2D *h2_QIsoSieieEE_AB,*h2_QIsoSieieEE_CD,*h2_QIsoSieieEE_[4];
  TH2D *h2_METnHadJ_0b_AB,*h2_METnHadJ_0b_CD;
  TH2D *h2_METnHadJ_min1b_AB,*h2_METnHadJ_min1b_CD;

  TH1D *h_nBestPho;
  TH1D *h_BestPho2Pt;
  TH1D *h_2ndPhoGenMatch;

  TH1D *h_jetPt[4];
  TH1D *h_jetEta[4];
  TH1D *h_jetPhi[4];

  TH1D *h_GmatchedObj_AB,*h_GmatchedObj_CD,*h_GmatchedObj_[4];
  TH1D *h_PdgIdPhoParent_AB,*h_PdgIdPhoParent_CD,*h_PdgIdPhoParent_[4];

  TH1D *h_nConsti;
  TH1D *h_NM;
  TH1D *h_NMEta3;
  TH1D *h_NMHF;
  TH1D *h_CM;
  TH1D *h_NHF;
  TH1D *h_NEMF;
  TH1D *h_CHF;
  TH1D *h_CEMF;

  TH1D *h_PhoMult;
  TH1D *h_PhoFrac;

  TH1D *h_GenMET;

  TFile *oFile;
  
};
#endif

#ifdef MultiJet_cxx

void MultiJet::BookHistogram(const char *outFileName) {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
 
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);
  h_RunNum=new TH1I("runs","Run nos.",300000,0,300000);
  h_intLumi=new TH1D("intLumi","integrated luminosity in /fb",2500,25,50); 

  h_ST_AB=new TH1D("ST_AB","ST_AB",400,0,4000);
  h_MET_AB=new TH1D("MET_AB","MET_AB",200,0,2000);
  h_nHadJets_AB=new TH1D("nHadJets_AB","no. of jets(only hadronic jets,not counting photon)_AB",25,0,25);
  h_BTags_AB=new TH1D("nBTags_AB","no. of B tags_AB",10,0,10);
  h_GenMET_AB=new TH1D("GenMET_AB","GenMET_AB",200,0,2000);
  h_METPhi_AB=new TH1D("METPhi_AB","METPhi_AB",40,0,4);
  h_myHT_AB=new TH1D("myHT_AB","HT: sum Pt of hadJets_AB",400,0,4000);

  h_QIsoRhoCorrEB_AB=new TH1D("QIsoRhoCorrEB_AB","Barrel Photon Q Isolatoin Rho Corrected_AB",200,0,100);
  h_sieieEB_AB=new TH1D("sieieEB_AB","#sigma_{i#etai#eta} for Barrel photon_AB",50,0,0.05);
  h_QIsoRhoCorrEE_AB=new TH1D("QIsoRhoCorrEE_AB","Endcap Photon Q Isolatoin Rho Corrected_AB",200,0,100);
  h_sieieEE_AB=new TH1D("sieieEE_AB","#sigma_{i#etai#eta} for Endcap photon_AB",50,0,0.05);

  h2_PtPhotonvsMET_AB=new TH2D("BestPhotonPtvsMET_AB","Best photon Pt vs MET_AB",150,0,1500,200,0,2000);
  h2_dPhi1dPhi2_AB=new TH2D("dPhi1dPhi2_AB","x:dPhi1 vs dPhi2_AB",40,0,4,40,0,4);
  h2_NJST_AB=new TH2D("NJST_AB","x:no. of hadJets vs ST_AB",nJBinLow.size()-1,&(nJBinLow[0]),STBinLow2.size()-1,&(STBinLow2[0]));
  h2_dPhiMETPho_NJ_AB=new TH2D("dPhiMETPho_NJ_AB","x:#Delta#Phi(MET, photon) vs nHadJets_AB",dPhiBinLow.size()-1,&(dPhiBinLow[0]),nJBinLow.size()-1,&(nJBinLow[0]));
  h2_PhoPtPhoJetPt_AB=new TH2D("PhoPtPhoJetPt_AB","x:Photon Pt vs matching Jet Pt_AB",200,0,2000,200,0,2000);
  h2_QIsoSieieEB_AB=new TH2D("QIsoSieieEB_AB","x:Q Isolatoin Rho Corrected vs #sigma_{i#etai#eta} for Barrel photon_AB",200,0,100,50,0,0.05);
  h2_QIsoSieieEE_AB=new TH2D("QIsoSieieEE_AB","x:Q Isolatoin Rho Corrected vs #sigma_{i#etai#eta} for Endcap photon_AB",200,0,100,50,0,0.05);
  h2_METnHadJ_0b_AB=new TH2D("METnHadJ_0b_AB","x:MET vs nHadJets_0b_AB",METLowSimple.size()-1,&(METLowSimple[0]),nJBinLow.size()-1,&(nJBinLow[0]));
  h2_METnHadJ_min1b_AB=new TH2D("METnHadJ_min1b_AB","x:MET vs nHadJets_min1b_AB",METLowSimple.size()-1,&(METLowSimple[0]),nJBinLow.size()-1,&(nJBinLow[0]));

  h_BestPhotonPt_AB=new TH1D("BestPhotonPt_AB","Pt of the Best Photon_AB",150,0,1500);
  h_BestPhotonPtvBin_AB=new TH1D("BestPhotonPtvarBin_AB","BestPhotonPt with variable bin size_AB",BestPhotonPtBinLowEdge.size()-1,&(BestPhotonPtBinLowEdge[0]));
  h_BestG_Eta_AB=new TH1D("BestG_Eta_AB","Best Photon Eta_AB",120,-6,6);
  h_BestG_Phi_AB=new TH1D("BestG_Phi_AB","Best Photon Phi_AB",80,-4,4);
  h_dPhi_METBestPhoton_AB=new TH1D("dPhi_METBestPhoton_AB","dphi between MET and BestPhoton_AB",40,0,4);
  h_PhoFracJet1_AB=new TH1D("PhoFracJet1_AB","Photon energy frac in leading jet, not matched to photon_AB",150,0,1.5);
  h_PhoFracPhoJet_AB=new TH1D("PhoFracPhoJet_AB","Photon energy frac in jet matched to photon_AB",150,0,1.5);
  h_NuEMFracPhoJet_AB=new TH1D("NuEMFracPhoJet_AB","Nuetral EM energy Fracction in jet matched to photon_AB",150,0,1.5);
  h_mTPho_AB=new TH1D("mTPho_AB","mT(#gamma,MET)_AB",150,0,1500);
  h_PhoHoverE_AB=new TH1D("PhoHoverE_AB","Photon H/E_AB",60,0,0.06);

  h_HT_AB=new TH1D("HT_AB","HT_AB",400,0,4000);
  h_MHT_AB=new TH1D("MHT_AB","MHT_AB",200,0,2000);
  h_nJets_AB=new TH1D("nJets_AB","nJets_AB",25,0,25);

  h_STvBin_AB=new TH1D("STvarBin_AB","STvarBin_AB",STBinLowEdge.size()-1,&(STBinLowEdge[0]));
  h_METvBin_AB=new TH1D("METvarBin_AB","MET with variable bin size_AB",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_jet1Pt_AB=new TH1D("jet1Pt_AB","Leading Jet Pt_AB",200,0,2000);
  h_jet2Pt_AB=new TH1D("jet2Pt_AB","2nd Leading Jet Pt_AB",200,0,2000);
  h_PtjetNearMET_AB=new TH1D("PtJetNearMET_AB","Pt of the jet with min dphi with MET_AB",200,0,2000);

  h_dPhi_METjet1_AB=new TH1D("dPhi_METjet1_AB","dphi between MET Vec and Jet1_AB",40,0,4);
  h_dPhi_METjet2_AB=new TH1D("dPhi_METjet2_AB","dphi between MET Vec and Jet2_AB",40,0,4);
  h_dPhi_METjet3_AB=new TH1D("dPhi_METjet3_AB","dphi between MET Vec and Jet3_AB",40,0,4);
  h_dPhi_METjet4_AB=new TH1D("dPhi_METjet4_AB","dphi between MET Vec and Jet4_AB",40,0,4);

  h_dPhiPhotonJet1_AB=new TH1D("dPhiPhotonJet1_AB","dphi(jet1,photon)_AB",40,0,4);

  h_GmatchedObj_AB=new TH1D("GmatchedObj_AB","Gen Obj close(dR<0.2) to Reco-Gamma_AB",62,-0.75,30.25);
  h_PdgIdPhoParent_AB = new TH1D("PdgIdPhoParent_AB","If reco#gamma is matched to gen#gamma, PdgID of the gen#gamma's parent_AB",62,-0.75,30.25);

  h_ST_CD=new TH1D("ST_CD","ST_CD",400,0,4000);
  h_MET_CD=new TH1D("MET_CD","MET_CD",200,0,2000);
  h_nHadJets_CD=new TH1D("nHadJets_CD","no. of jets(only hadronic jets,not counting photon)_CD",25,0,25);
  h_BTags_CD=new TH1D("nBTags_CD","no. of B tags_CD",10,0,10);
  h_GenMET_CD=new TH1D("GenMET_CD","GenMET_CD",200,0,2000);
  h_myHT_CD=new TH1D("myHT_CD","HT: sum Pt of hadJets_CD",400,0,4000);
  h_METPhi_CD=new TH1D("METPhi_CD","METPhi_CD",40,0,4);

  h_BestPhotonPt_CD=new TH1D("BestPhotonPt_CD","Pt of the Best Photon_CD",150,0,1500);
  h_BestPhotonPtvBin_CD=new TH1D("BestPhotonPtvarBin_CD","BestPhotonPt with variable bin size_CD",BestPhotonPtBinLowEdge.size()-1,&(BestPhotonPtBinLowEdge[0]));
  h_BestG_Eta_CD=new TH1D("BestG_Eta_CD","Best Photon Eta_CD",120,-6,6);
  h_BestG_Phi_CD=new TH1D("BestG_Phi_CD","Best Photon Phi_CD",80,-4,4);
  h_dPhi_METBestPhoton_CD=new TH1D("dPhi_METBestPhoton_CD","dphi between MET and BestPhoton_CD",40,0,4);
  h_QIsoRhoCorrEB_CD=new TH1D("QIsoRhoCorrEB_CD","Barrel Photon Q Isolatoin Rho Corrected_CD",200,0,100);
  h_sieieEB_CD=new TH1D("sieieEB_CD","#sigma_{i#etai#eta} for Barrel photon_CD",50,0,0.05);
  h_QIsoRhoCorrEE_CD=new TH1D("QIsoRhoCorrEE_CD","Endcap Photon Q Isolatoin Rho Corrected_CD",200,0,100);
  h_sieieEE_CD=new TH1D("sieieEE_CD","#sigma_{i#etai#eta} for Endcap photon_CD",50,0,0.05);
  h_PhoFracJet1_CD=new TH1D("PhoFracJet1_CD","Photon energy frac in leading jet, not matched to photon_CD",150,0,1.5);
  h_PhoFracPhoJet_CD=new TH1D("PhoFracPhoJet_CD","Photon energy frac in jet matched to photon_CD",150,0,1.5);
  h_NuEMFracPhoJet_CD=new TH1D("NuEMFracPhoJet_CD","Nuetral EM energy Fracction in jet matched to photon_CD",150,0,1.5);
  h_mTPho_CD=new TH1D("mTPho_CD","mT(#gamma,MET)_CD",150,0,1500);
  h_PhoHoverE_CD=new TH1D("PhoHoverE_CD","Photon H/E_CD",60,0,0.06);

  h2_PtPhotonvsMET_CD=new TH2D("BestPhotonPtvsMET_CD","Best photon Pt vs MET_CD",150,0,1500,200,0,2000);
  h2_dPhi1dPhi2_CD=new TH2D("dPhi1dPhi2_CD","x:dPhi1 vs dPhi2_CD",40,0,4,40,0,4);
  h2_NJST_CD=new TH2D("NJST_CD","x:no. of hadJets vs ST_CD",nJBinLow.size()-1,&(nJBinLow[0]),STBinLow2.size()-1,&(STBinLow2[0]));
  h2_dPhiMETPho_NJ_CD=new TH2D("dPhiMETPho_NJ_CD","x:#Delta#Phi(MET, photon) vs nHadJets_CD",dPhiBinLow.size()-1,&(dPhiBinLow[0]),nJBinLow.size()-1,&(nJBinLow[0]));
  h2_PhoPtPhoJetPt_CD=new TH2D("PhoPtPhoJetPt_CD","x:Photon Pt vs matching Jet Pt_CD",200,0,2000,200,0,2000);
  h2_QIsoSieieEB_CD=new TH2D("QIsoSieieEB_CD","x:Q Isolatoin Rho Corrected vs #sigma_{i#etai#eta} for Barrel photon_CD",200,0,100,50,0,0.05);
  h2_QIsoSieieEE_CD=new TH2D("QIsoSieieEE_CD","x:Q Isolatoin Rho Corrected vs #sigma_{i#etai#eta} for Endcap photon_CD",200,0,100,50,0,0.05);
  h2_METnHadJ_0b_CD=new TH2D("METnHadJ_0b_CD","x:MET vs nHadJets_0b_CD",METLowSimple.size()-1,&(METLowSimple[0]),nJBinLow.size()-1,&(nJBinLow[0]));
  h2_METnHadJ_min1b_CD=new TH2D("METnHadJ_min1b_CD","x:MET vs nHadJets_min1b_CD",METLowSimple.size()-1,&(METLowSimple[0]),nJBinLow.size()-1,&(nJBinLow[0]));

  h_HT_CD=new TH1D("HT_CD","HT_CD",400,0,4000);
  h_MHT_CD=new TH1D("MHT_CD","MHT_CD",200,0,2000);
  h_nJets_CD=new TH1D("nJets_CD","nJets_CD",25,0,25);

  h_STvBin_CD=new TH1D("STvarBin_CD","STvarBin_CD",STBinLowEdge.size()-1,&(STBinLowEdge[0]));
  h_METvBin_CD=new TH1D("METvarBin_CD","MET with variable bin size_CD",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_jet1Pt_CD=new TH1D("jet1Pt_CD","Leading Jet Pt_CD",200,0,2000);
  h_jet2Pt_CD=new TH1D("jet2Pt_CD","2nd Leading Jet Pt_CD",200,0,2000);
  h_PtjetNearMET_CD=new TH1D("PtJetNearMET_CD","Pt of the jet with min dphi with MET_CD",200,0,2000);

  h_dPhi_METjet1_CD=new TH1D("dPhi_METjet1_CD","dphi between MET Vec and Jet1_CD",40,0,4);
  h_dPhi_METjet2_CD=new TH1D("dPhi_METjet2_CD","dphi between MET Vec and Jet2_CD",40,0,4);
  h_dPhi_METjet3_CD=new TH1D("dPhi_METjet3_CD","dphi between MET Vec and Jet3_CD",40,0,4);
  h_dPhi_METjet4_CD=new TH1D("dPhi_METjet4_CD","dphi between MET Vec and Jet4_CD",40,0,4);

  h_dPhiPhotonJet1_CD=new TH1D("dPhiPhotonJet1_CD","dphi(jet1,photon)_CD",40,0,4);
  h_GmatchedObj_CD=new TH1D("GmatchedObj_CD","Gen Obj close(dR<0.2) to Reco-Gamma_CD",62,-0.75,30.25);
  h_PdgIdPhoParent_CD = new TH1D("PdgIdPhoParent_CD","If reco#gamma is matched to gen#gamma, PdgID of the gen#gamma's parent_CD",62,-0.75,30.25);

  for(int i=0;i<4;i++){
    TString regName;
    if(i==0) regName="A";
    if(i==1) regName="B";
    if(i==2) regName="C";
    if(i==3) regName="D";

    h_ST_[i]=new TH1D("ST_"+regName,"ST_"+regName,400,0,4000);
    h_MET_[i]=new TH1D("MET_"+regName,"MET_"+regName,200,0,2000);
    h_nHadJets_[i]=new TH1D("nHadJets_"+regName,"no. of jets(only hadronic jets,not counting photon)_"+regName,25,0,25);
    h_BTags_[i]=new TH1D("nBTags_"+regName,"no. of B tags_"+regName,10,0,10);
    h_GenMET_[i]=new TH1D("GenMET_"+regName,"GenMET_"+regName,200,0,2000);
    h_myHT_[i]=new TH1D("myHT_"+regName,"HT: sum Pt of hadJets_"+regName,400,0,4000);
    h_METPhi_[i]=new TH1D("METPhi_"+regName,"METPhi_"+regName,40,0,4);

    h_BestPhotonPt_[i]=new TH1D("BestPhotonPt_"+regName,"Pt of the Best Photon_"+regName,150,0,1500);
    h_BestPhotonPtvBin_[i]=new TH1D("BestPhotonPtvarBin_"+regName,"BestPhotonPt with variable bin size_"+regName,BestPhotonPtBinLowEdge.size()-1,&(BestPhotonPtBinLowEdge[0]));
    h_BestG_Eta_[i]=new TH1D("BestG_Eta_"+regName,"Best Photon Eta_"+regName,120,-6,6);
    h_BestG_Phi_[i]=new TH1D("BestG_Phi_"+regName,"Best Photon Phi_"+regName,80,-4,4);
    h_dPhi_METBestPhoton_[i]=new TH1D("dPhi_METBestPhoton_"+regName,"dphi between MET and BestPhoton_"+regName,40,0,4);
    h_dPhiPhotonJet1_[i]=new TH1D("dPhiPhotonJet1_"+regName,"dphi(jet1,photon)_"+regName,40,0,4);

    h_SBins_v1_[i] = new TH1D("AllSBins_v1_"+regName,"search bins:(NJ=2to4) (NJ:5or6) (NJ>=7)_"+regName,21,0.5,21.5);
    h_SBins_v4_[i] = new TH1D("AllSBins_v4_"+regName,"search bins:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_"+regName,31,0.5,31.5);

    h_QIsoRhoCorrEB_[i]=new TH1D("QIsoRhoCorrEB_"+regName,"Barrel Photon Q Isolatoin Rho Corrected_"+regName,200,0,100);
    h_sieieEB_[i]=new TH1D("sieieEB_"+regName,"#sigma_{i#etai#eta} for Barrel photon_"+regName,50,0,0.05);
    h_QIsoRhoCorrEE_[i]=new TH1D("QIsoRhoCorrEE_"+regName,"Endcap Photon Q Isolatoin Rho Corrected_"+regName,200,0,100);
    h_sieieEE_[i]=new TH1D("sieieEE_"+regName,"#sigma_{i#etai#eta} for Endcap photon_"+regName,50,0,0.05);
    h_mTPho_[i]=new TH1D("mTPho_"+regName,"mT(#gamma,MET)_"+regName,150,0,1500);
    h_PhoHoverE_[i]=new TH1D("PhoHoverE_"+regName,"Photon H/E_"+regName,60,0,0.06);
    h_PhoFracJet1_[i]=new TH1D("PhoFracJet1_"+regName,"Photon energy frac in leading jet, not matched to photon_"+regName,150,0,1.5);
    h_PhoFracPhoJet_[i]=new TH1D("PhoFracPhoJet_"+regName,"Photon energy frac in jet matched to photon_"+regName,150,0,1.5);
    h_NuEMFracPhoJet_[i]=new TH1D("NuEMFracPhoJet_"+regName,"Nuetral EM energy Fracction in jet matched to photon_"+regName,150,0,1.5);
  
    h2_PtPhotonvsMET_[i]=new TH2D("BestPhotonPtvsMET_"+regName,"Best photon Pt vs MET_"+regName,150,0,1500,200,0,2000);
    h2_dPhi1dPhi2_[i]=new TH2D("dPhi1dPhi2_"+regName,"x:dPhi1 vs dPhi2_"+regName,40,0,4,40,0,4);
    h2_NJST_[i]=new TH2D("NJST_"+regName,"x:no. of hadJets vs ST_"+regName,nJBinLow.size()-1,&(nJBinLow[0]),STBinLow2.size()-1,&(STBinLow2[0]));
    h2_dPhiMETPho_NJ_[i]=new TH2D("dPhiMETPho_NJ_"+regName,"x:#Delta#Phi(MET, photon) vs nHadJets_"+regName,dPhiBinLow.size()-1,&(dPhiBinLow[0]),nJBinLow.size()-1,&(nJBinLow[0]));
    h2_PhoPtPhoJetPt_[i]=new TH2D("PhoPtPhoJetPt_"+regName,"x:Photon Pt vs matching Jet Pt_"+regName,200,0,2000,200,0,2000);
    h2_QIsoSieieEB_[i]=new TH2D("QIsoSieieEB_"+regName,"x:Q Isolatoin Rho Corrected vs #sigma_{i#etai#eta} for Barrel photon_"+regName,200,0,100,50,0,0.05);
    h2_QIsoSieieEE_[i]=new TH2D("QIsoSieieEE_"+regName,"x:Q Isolatoin Rho Corrected vs #sigma_{i#etai#eta} for Endcap photon_"+regName,200,0,100,50,0,0.05);
 
    h_HT_[i]=new TH1D("HT_"+regName,"HT_"+regName,400,0,4000);
    h_MHT_[i]=new TH1D("MHT_"+regName,"MHT_"+regName,200,0,2000);
    h_nJets_[i]=new TH1D("nJets_"+regName,"nJets_"+regName,25,0,25);

    h_STvBin_[i]=new TH1D("STvarBin_"+regName,"STvarBin_"+regName,STBinLowEdge.size()-1,&(STBinLowEdge[0]));
    h_METvBin_[i]=new TH1D("METvarBin_"+regName,"MET with variable bin size_"+regName,METBinLowEdge.size()-1,&(METBinLowEdge[0]));

    h_jet1Pt_[i]=new TH1D("jet1Pt_"+regName,"Leading Jet Pt_"+regName,200,0,2000);
    h_jet2Pt_[i]=new TH1D("jet2Pt_"+regName,"2nd Leading Jet Pt_"+regName,200,0,2000);
    h_PtjetNearMET_[i]=new TH1D("PtJetNearMET_"+regName,"Pt of the jet with min dphi with MET_"+regName,200,0,2000);

    h_dPhi_METjet1_[i]=new TH1D("dPhi_METjet1_"+regName,"dphi between MET Vec and Jet1_"+regName,40,0,4);
    h_dPhi_METjet2_[i]=new TH1D("dPhi_METjet2_"+regName,"dphi between MET Vec and Jet2_"+regName,40,0,4);
    h_dPhi_METjet3_[i]=new TH1D("dPhi_METjet3_"+regName,"dphi between MET Vec and Jet3_"+regName,40,0,4);
    h_dPhi_METjet4_[i]=new TH1D("dPhi_METjet4_"+regName,"dphi between MET Vec and Jet4_"+regName,40,0,4);

    h_GmatchedObj_[i]=new TH1D("GmatchedObj_"+regName,"Gen Obj close(dR<0.2) to Reco-Gamma_"+regName,62,-0.75,30.25);
    h_PdgIdPhoParent_[i] = new TH1D("PdgIdPhoParent_"+regName,"If reco#gamma is matched to gen#gamma, PdgID of the gen#gamma's parent_"+regName,62,-0.75,30.25);
  }

  h_nBestPho=new TH1D("nBestPhotons","no. of best photons",5,0,5);
  h_BestPho2Pt=new TH1D("BestPho2Pt","Pt of the second leading photon",1500,0,1500);
  h_2ndPhoGenMatch=new TH1D("2ndPhoGenMatch","Gen particle matched to 2nd leading photon",62,-0.75,30.25);
   
  h_madHT=new TH1D("madHT","madHT",300,0,3000);
  for(int i=0;i<4;i++){
    sprintf(name,"jet%iPt",i+1);
    h_jetPt[i]=new TH1D(name,name,200,0,2000);
    sprintf(name,"jet%iEta",i+1);
    h_jetEta[i]=new TH1D(name,name,120,-6,6);
    sprintf(name,"jet%iPhi",i+1);
    h_jetPhi[i]=new TH1D(name,name,80,-4,4);
  }

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
  //---------------Search Bins ----------------------------
  h_SBins_v1_AB = new TH1D("AllSBins_v1_AB","search bins:(NJ=2to4) (NJ:5or6) (NJ>=7)_AB",21,0.5,21.5);
  h_SBins_v1_CD = new TH1D("AllSBins_v1_CD","search bins:(NJ=2to4) (NJ:5or6) (NJ>=7)_CD",21,0.5,21.5);
  h_SBins_v4_AB = new TH1D("AllSBins_v4_AB","search bins:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_AB",31,0.5,31.5);
  h_SBins_v4_CD = new TH1D("AllSBins_v4_CD","search bins:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);
}


MultiJet::MultiJet(const TString &inputFileList, const char *outFileName, const char* dataset) {
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

Bool_t MultiJet::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t MultiJet::LoadTree(Long64_t entry) {
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

MultiJet::~MultiJet() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

