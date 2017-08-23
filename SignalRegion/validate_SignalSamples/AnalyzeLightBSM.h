#ifndef AnalyzeLightBSM_H
#define AnalyzeLightBSM_H

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

class AnalyzeLightBSM : public NtupleVariables{

 public:
  AnalyzeLightBSM(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~AnalyzeLightBSM();
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
  double wt=0,lumiInfb=35.86;//36.814;//35.862824;//36.814;
  bool isSignal=false;
  //vector<double> HTBinLowEdge ={0,500,550,600,650,700,750,800,900,1000,1100,1200,1400,1600,1800,2000,2400,2800,3200};
  //  vector<double> HTBinLowEdge ={0,100,200,300,400,500,600,700,800,900,1000,1200,1500,2000,2500};
  //  vector<double> MHTBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,500};
  vector<double> STBinLowEdge ={0,300,360,420,500,600,700,850,1000,1200,1500,2000,2500,3000};
  vector<double> METBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,450,600,750,900,1200};
  vector<double> METBinLowEdge2={0,20,40,60,80,100,120,160,200,270,350,450,500};
  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  //vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,200,250,300,350,400,500,600};
  vector<TLorentzVector> allBestPhotons;
  //histograms
  TH1D *h_mGl;
  TH1D *h_mNLSP;
  TH1D *h_mLSP;
  TH1D *h_xsec;
  TH1D *h_NumEvt;
  TH1D *h_Wt;
  TH1D *h_GlPt;
  TH1D *h_NLSP_Pt;
  TH1D *h_nPhoFmNLSP;
  TH1D *h_nZFmNLSP;
  TH1D *h_ZKids;
  TH1D *h_invMassGl;
  TH1D *h_invMassPhoLSP;
  TH1D *h_invMassZLSP;

  TH2D *h2_mGlmNLSP;

  TH1I *h_RunNum;
  TH1D *h_intLumi;
  TH1D *h_ST;
  TH1D *h_MET;
  TH1D *h_nHadJets;
  TH1D *h_BTags;
  TH1D *h_BestPhotonPt;

  TH1D *h_HT;
  TH1D *h_MHT;
  TH1D *h_MHTstar;
  TH1D *h_nJets;

  TH2D *h2_PtPhotonvsMET;
  TH2D *h2_1G_PtPhotonvsMET;
  TH2D *h2_More2G_PtPhotonvsMET;
  
  TH1D *h_STvBin;
  TH1D *h_METvBin;
  TH1D *h_BestPhotonPtvBin;
  TH1D *h_MHTvBin;
  TH1D *h_MET_R[3];
  TH1D *h_MET_Rall;

  TH1D *h_BestG_Eta;
  TH1D *h_BestG_Phi;

  TH1D *h_isoEleTrack;
  TH1D *h_isoMuTrack;
  TH1D *h_isoPiTrack;

  TH1D *h_otherPhotonsPt;
  TH1D *h_otherPhotonsEta;
  TH1D *h_otherPhotonsPhi;
  TH1D *h_nBestPho;
  TH1D *h_BestPho2Pt;
  TH1D *h_dPhi2Photons;
  TH1D *h_2ndPhoGenMatch;
  TH1D *h_madHT;
  TH1D *h_mTPhoMET;

  TH2D *h2_STvsHT;

  TH1D *h_dPhi_METBestPhoton;
  TH1D *h_dPhi_METjet1;
  TH1D *h_dPhi_METjet2;
  TH1D *h_dPhi_METjet3;
  TH1D *h_dPhi_METjet4;
  TH1D *h_dPhi_MET_jets;
	
  TH1D *h_minDR_phoJet;

  TH1D *h_hasGenPrompt;
  TH1D *h_photonPrompt;
  TH1D *h_fakePhoton;

  TH1D *h_ST_NoJID;
  TH1D *h_MET_NoJID;
  TH1D *h_nHadJets_NoJID;
  TH1D *h_BTags_NoJID;
  TH1D *h_BestPhotonPt_NoJID;
  TH2D *h2_PtPhotonvsMET_NoJID;

  TH1D *h_ST_FalseJID;
  TH1D *h_MET_FalseJID;
  TH1D *h_nHadJets_FalseJID;
  TH1D *h_BTags_FalseJID;
  TH1D *h_BestPhotonPt_FalseJID;
  TH2D *h2_PtPhotonvsMET_FalseJID;

  TH1D *h_jetsPt;
  TH1D *h_jetsEta;
  TH1D *h_jetsPhi;
  
  TH1D *h_jetPtNearPhoton;

  TH1D *h_jetPt[4];
  TH1D *h_jetEta[4];
  TH1D *h_jetPhi[4];

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
  TH2D *h2_PtGenReco;

  TH1D *h_my_ST;
  TH1D *h_my_MET;
  
  TH2D *h2_mMomvsmKid;
  TH1D *h_GenMET;
  TH2D *h2_mH_HKids;

  TH1D *h_GmatchedObj;
  TH1D *h_PdgIdPhoParent;

  TH2D *h2_MET_MHTstar;
  TH2D *h2_ptRatiovsDR;
  TH2D *h2_METvsdPhiJ1MET;

  TH1D *h_nGenLep;
  TH1D *h_BGType;
  TH1D *h_temp;

  TFile *oFile;
  
};
#endif

#ifdef AnalyzeLightBSM_cxx

void AnalyzeLightBSM::BookHistogram(const char *outFileName) {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  char name[100],title[100];
 
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);

  h_mGl = new TH1D("mGl","mass of gluino",200,0,2000);
  h_mNLSP = new TH1D("mNLSP","mass of NLSP",200,0,2000);
  h_mLSP = new TH1D("mLSP","mass of LSP",2000,0,2000);
  h_xsec = new TH1D("xsec","xsec in /pb",2,0,2);
  h_NumEvt = new TH1D("NumEvts","number events",2,0,2);
  h_Wt = new TH1D("evtWt","event weight",2,0,2);
  h_GlPt = new TH1D("GlPt","Gluino Pt",200,0,2000);
  h_NLSP_Pt = new TH1D("NLSP_Pt","NLSP Pt",200,0,2000);
  h_nPhoFmNLSP = new TH1D("nPhoFmNLSP","no. of photons from NLSP decay",5,0,5);
  h_nZFmNLSP = new TH1D("nZFmNLSP","no. of Z from NLSP decay",5,0,5);
  h_ZKids = new TH1D("ZKids","no. of Z kids",60,-0.75,29.25);
  h_invMassGl = new TH1D("invMassGl","Invariant mass of gluino: calculated from its daughters",200,0,2000);
  h_invMassPhoLSP = new TH1D("invMassPhoLSP","Invariant mass of Photon and LSP",200,0,2000);
  h_invMassZLSP = new TH1D("invMassZLSP","Invariant mass of Z and LSP",200,0,2000);

  h2_mGlmNLSP = new TH2D("mGlmNLSP","x:mass of gluino vs mass of NLSP",2001,-0.5,2000.5,2001,-0.5,2000.5);

  h_RunNum=new TH1I("runs","Run nos.",300000,0,300000);
  h_intLumi=new TH1D("intLumi","integrated luminosity in /fb",2500,25,50); 
  h_ST=new TH1D("ST","ST",400,0,4000);
  h_MET=new TH1D("MET","MET",200,0,2000);
  h_nHadJets=new TH1D("nHadJets","no. of jets(only hadronic jets,not counting photon)",25,0,25);
  h_BTags=new TH1D("nBTags","no. of B tags",10,0,10);
  h_BestPhotonPt=new TH1D("BestPhotonPt","Pt of the Best Photon",150,0,1500);

  h_HT=new TH1D("HT","HT",400,0,4000);
  h_MHT=new TH1D("MHT","MHT",200,0,2000);
  h_nJets=new TH1D("nJets","nJets",25,0,25);
  h_MHTstar=new TH1D("MHTstar","MHT calculated using ST like definition",200,0,2000);
  
  h2_PtPhotonvsMET=new TH2D("BestPhotonPtvsMET","Best photon Pt vs MET",150,0,1500,200,0,2000);
  h2_1G_PtPhotonvsMET=new TH2D("1G_PtPhotonvsMET","Best photon Pt vs MET for 1Photon events Only",150,0,1500,200,0,2000);
  h2_More2G_PtPhotonvsMET=new TH2D("More2G_PtPhotonvsMET","Best photon Pt vs MET for More than 1 Photons events",150,0,1500,200,0,2000);
  h2_mH_HKids=new TH2D("mH_HKids","mH and Higgs Kids",2000,0,200,10,0,10);  

  h_STvBin = new TH1D("STvarBin","STvarBin",STBinLowEdge.size()-1,&(STBinLowEdge[0]));
  h_METvBin=new TH1D("METvarBin","MET with variable bin size",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_BestPhotonPtvBin=new TH1D("BestPhotonPtvarBin","BestPhotonPt with variable bin size",BestPhotonPtBinLowEdge.size()-1,&(BestPhotonPtBinLowEdge[0]));
  h_MHTvBin=new TH1D("MHTvarBin","MHT with variable bin size",METBinLowEdge.size()-1,&(METBinLowEdge[0]));
  h_MET_R[0] = new TH1D("MET_R1","MET: NJ=2to4",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]));
  h_MET_R[1] = new TH1D("MET_R2","MET: NJ=5or6",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]));
  h_MET_R[2] = new TH1D("MET_R3","MET: NJ>=7",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]));
  h_MET_Rall = new TH1D("MET_Rall","MET: NJets>=2",METBinLowEdge2.size()-1,&(METBinLowEdge2[0]));

  h_BestG_Eta=new TH1D("BestG_Eta","Best Photon Eta",120,-6,6);
  h_BestG_Phi=new TH1D("BestG_Phi","Best Photon Phi",80,-4,4);

  h_isoEleTrack=new TH1D("isoEleTrack","isolated electron tracks",5,0,5);
  h_isoMuTrack=new TH1D("isoMuTrack","isolated Muon tracks",5,0,5);
  h_isoPiTrack=new TH1D("isoPiTrack","isolated Pion tracks",5,0,5);

  h_otherPhotonsEta=new TH1D("otherPhotonsEta","otherPhotonsEta",120,-6,6);
  h_otherPhotonsPhi=new TH1D("otherPhotonsPhi","otherPhotonsPhi",80,-4,4);
  h_madHT=new TH1D("madHT","madHT",300,0,3000);
  h_mTPhoMET=new TH1D("mTPhoMET","mT b/w bestPhoton and MET",200,0,2000);

  h_hasGenPrompt=new TH1D("has_genPromptPho","has a gen hard scattered photon with Pt>40GeV",3,-1,2);
  h_photonPrompt=new TH1D("isPromptPho","RECO photon is a prompt photon",3,-1,2);
  h_fakePhoton=new TH1D("fakePho","photon is fake - matched to a gen e and not matched to a gen Photon",3,-1,2);

  h2_STvsHT=new TH2D("STvsHT","x:ST vs HT",400,0,4000,400,0,4000);

  h_ST_NoJID=new TH1D("ST_NoJID","ST without JetID",400,0,4000);
  h_MET_NoJID=new TH1D("MET_NoJID","MET without JetID",200,0,2000);
  h_nHadJets_NoJID=new TH1D("nHadJets_NoJID","nHadJets without JetID",25,0,25);
  h_BTags_NoJID=new TH1D("nBTags_NoJID","no. of B tags without JetID",10,0,10);
  h_BestPhotonPt_NoJID=new TH1D("BestPhotonPt_NoJID","Pt of the Best Photon without JetID",150,0,1500);
  h2_PtPhotonvsMET_NoJID=new TH2D("BestPhotonPtvsMET_NoJID","Best photon Pt vs MET without JetID",150,0,1500,200,0,2000);

  h_ST_FalseJID=new TH1D("ST_FalseJID","ST-Failed JetID",400,0,4000);
  h_MET_FalseJID=new TH1D("MET_FalseJID","MET-Failed JetID",200,0,2000);
  h_nHadJets_FalseJID=new TH1D("nHadJets_FalseJID","nHadJets-Failed JetID",25,0,25);
  h_BTags_FalseJID=new TH1D("nBTags_FalseJID","no. of B tags-Failed JetID",10,0,10);
  h_BestPhotonPt_FalseJID=new TH1D("BestPhotonPt_FalseJID","Pt of the Best Photon-Failed JetID",150,0,1500);
  h2_PtPhotonvsMET_FalseJID=new TH2D("BestPhotonPtvsMET_FalseJID","Best photon Pt vs MET Failed JetID",150,0,1500,200,0,2000);
  
  h_jetsPt=new TH1D("jetsPt","Jets Pt",200,0,2000);
  h_jetsEta=new TH1D("jetsEta","Jets Eta",120,-6,6);
  h_jetsPhi=new TH1D("jetsPhi","Jets Phi",80,-4,4);
  for(int i=0;i<4;i++){
    sprintf(name,"jet%iPt",i+1);
    h_jetPt[i]=new TH1D(name,name,200,0,2000);
    sprintf(name,"jet%iEta",i+1);
    h_jetEta[i]=new TH1D(name,name,120,-6,6);
    sprintf(name,"jet%iPhi",i+1);
    h_jetPhi[i]=new TH1D(name,name,80,-4,4);
  }
  h_jetPtNearPhoton=new TH1D("jetPtNearPhoton","Pt of the Photon matched jet. Photon NOT inluded",200,0,2000);
  h_nBestPho=new TH1D("nBestPhotons","no. of best photons",5,0,5);
  h_BestPho2Pt=new TH1D("BestPho2Pt","Pt of the second leading photon",1500,0,1500);
  h_dPhi2Photons=new TH1D("dPhi2Photons","dPhi b/w 2 photons",80,-4,4);
  h_2ndPhoGenMatch=new TH1D("2ndPhoGenMatch","Gen particle matched to 2nd leading photon",62,-0.75,30.25);

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

  h_dPhi_METBestPhoton=new TH1D("dPhi_METBestPhoton","dphi between MET and BestPhoton",40,0,4);
  h_dPhi_METjet1=new TH1D("dPhi_METjet1","dphi between MET Vec and Jet1",40,0,4);
  h_dPhi_METjet2=new TH1D("dPhi_METjet2","dphi between MET Vec and Jet2",40,0,4);
  h_dPhi_METjet3=new TH1D("dPhi_METjet3","dphi between MET Vec and Jet3",40,0,4);
  h_dPhi_METjet4=new TH1D("dPhi_METjet4","dphi between MET Vec and Jet4",40,0,4);
  h_dPhi_MET_jets=new TH1D("dPhi_MET_jets","dphi between MET Vec and hadronic jets",40,0,4);

  h_minDR_phoJet=new TH1D("minDR_phoJet","min dR b/w best photon and jet",200,0,2);

  h2_mMomvsmKid=new TH2D("mMomvsmKid","mass of gluino(x) vs mass of NLSP(y)",501,-5,5005,501,-5,5005);
  h_GenMET=new TH1D("GenMET","GenMET",200,0,2000);
  h2_PtGenReco=new TH2D("PtGenJet_RecoJet","x_PtGenJet_RecoJet",200,0,2000,200,0,2000);

  h_my_ST=new TH1D("my_ST","my_ST",200,0,2000);
  h_my_MET=new TH1D("my_MET","my_MET",200,0,2000);

  h_GmatchedObj=new TH1D("GmatchedObj","Gen Obj close to Reco-Gamma",62,-0.75,30.25);
  h_PdgIdPhoParent = new TH1D("PdgIdPhoParent","PdgID of the Photon's parent",62,-0.75,30.25);

  h2_MET_MHTstar=new TH2D("MET_MHTstar","x:MET vs MHTstar(MHT definition is like ST definition) ",100,0,2000,100,0,2000);
  h2_ptRatiovsDR=new TH2D("ptRatiovsDR","x:minDR b/w jet and photon vs ratio of pt_jet to pt_photon",100,0,1,100,0,10);
  h2_METvsdPhiJ1MET=new TH2D("METvsdPhiJ1MET","x:dPhi b/w jet1 and MET vs MET",40,0,4,200,0,2000);

  h_nGenLep=new TH1D("nGenLep","no. of Gen leptons",5,0,5);
  h_BGType=new TH1D("typeOfBG","Type of BG: 1-elefake, 2-lostMu, 3-lostEle, 4-hadTau, 5-multijet",8,-0.5,7.5);
  h_temp=new TH1D("temp","temp",200,0,2000);
}


AnalyzeLightBSM::AnalyzeLightBSM(const TString &inputFileList, const char *outFileName, const char* dataset) {
  string nameData=dataset;//vvv
  TChain *tree = new TChain("PreSelection");
  tree = new TChain("TreeMaker2/PreSelection");//vvv
  //  if(nameData=="signalH") tree = new TChain("TreeMaker2/PreSelection");//vvv
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

Bool_t AnalyzeLightBSM::FillChain(TChain *chain, const TString &inputFileList) {

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

Long64_t AnalyzeLightBSM::LoadTree(Long64_t entry) {
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

AnalyzeLightBSM::~AnalyzeLightBSM() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

