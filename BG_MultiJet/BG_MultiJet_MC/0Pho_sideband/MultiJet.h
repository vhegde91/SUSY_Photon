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
  //vector<double> HTBinLowEdge ={0,500,550,600,650,700,750,800,900,1000,1100,1200,1400,1600,1800,2000,2400,2800,3200};
  //  vector<double> HTBinLowEdge ={0,100,200,300,400,500,600,700,800,900,1000,1200,1500,2000,2500};
  //  vector<double> MHTBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,500};
  vector<double> STBinLowEdge ={0,300,360,420,500,600,700,850,1000,1200,1500,2000,2500,3000};
  vector<double> METBinLowEdge={0,20,40,60,80,100,120,160,200,270,350,450,600,750,900,1200};
  vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,180,200,220,250,280,320,380,450,550,650,750};
  //vector<double> BestPhotonPtBinLowEdge={0,100,120,140,160,200,250,300,350,400,500,600};
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

  TH1D *h_HT_AB,*h_HT_CD,*h_HT_[4];
  TH1D *h_MHT_AB,*h_MHT_CD,*h_MHT_[4];
  TH1D *h_nJets_AB,*h_nJets_CD,*h_nJets_[4];

  TH1D *h_STvBin_AB,*h_STvBin_CD,*h_STvBin_[4];
  TH1D *h_METvBin_AB,*h_METvBin_CD,*h_METvBin_[4];

  TH1D *h_jet1Pt_AB,*h_jet1Pt_CD,*h_jet1Pt_[4];
  TH1D *h_jet2Pt_AB,*h_jet2Pt_CD,*h_jet2Pt_[4];

  TH1D *h_dPhi_METjet1_AB,*h_dPhi_METjet1_CD,*h_dPhi_METjet1_[4];
  TH1D *h_dPhi_METjet2_AB,*h_dPhi_METjet2_CD,*h_dPhi_METjet2_[4];
  TH1D *h_dPhi_METjet3_AB,*h_dPhi_METjet3_CD,*h_dPhi_METjet3_[4];
  TH1D *h_dPhi_METjet4_AB,*h_dPhi_METjet4_CD,*h_dPhi_METjet4_[4];

  TH1D *h_BestPhotonPt;
  TH1D *h_BestPhotonPtvBin;
  TH1D *h_BestG_Eta;
  TH1D *h_BestG_Phi;
  TH1D *h_dPhi_METBestPhoton;
  TH2D *h2_PtPhotonvsMET;
  TH1D *h_nBestPho;
  TH1D *h_BestPho2Pt;
  TH1D *h_2ndPhoGenMatch;

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

  TH1D *h_GenMET;
  TH1D *h_GmatchedObj;
  TH1D *h_PdgIdPhoParent;

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

    h_HT_[i]=new TH1D("HT_"+regName,"HT_"+regName,400,0,4000);
    h_MHT_[i]=new TH1D("MHT_"+regName,"MHT_"+regName,200,0,2000);
    h_nJets_[i]=new TH1D("nJets_"+regName,"nJets_"+regName,25,0,25);

    h_STvBin_[i]=new TH1D("STvarBin_"+regName,"STvarBin_"+regName,STBinLowEdge.size()-1,&(STBinLowEdge[0]));
    h_METvBin_[i]=new TH1D("METvarBin_"+regName,"MET with variable bin size_"+regName,METBinLowEdge.size()-1,&(METBinLowEdge[0]));

    h_jet1Pt_[i]=new TH1D("jet1Pt_"+regName,"Leading Jet Pt_"+regName,200,0,2000);
    h_jet2Pt_[i]=new TH1D("jet2Pt_"+regName,"2nd Leading Jet Pt_"+regName,200,0,2000);

    h_dPhi_METjet1_[i]=new TH1D("dPhi_METjet1_"+regName,"dphi between MET Vec and Jet1_"+regName,40,0,4);
    h_dPhi_METjet2_[i]=new TH1D("dPhi_METjet2_"+regName,"dphi between MET Vec and Jet2_"+regName,40,0,4);
    h_dPhi_METjet3_[i]=new TH1D("dPhi_METjet3_"+regName,"dphi between MET Vec and Jet3_"+regName,40,0,4);
    h_dPhi_METjet4_[i]=new TH1D("dPhi_METjet4_"+regName,"dphi between MET Vec and Jet4_"+regName,40,0,4);
  }

  h_ST_AB=new TH1D("ST_AB","ST_AB",400,0,4000);
  h_MET_AB=new TH1D("MET_AB","MET_AB",200,0,2000);
  h_nHadJets_AB=new TH1D("nHadJets_AB","no. of jets(only hadronic jets,not counting photon)_AB",25,0,25);
  h_BTags_AB=new TH1D("nBTags_AB","no. of B tags_AB",10,0,10);
  h_GenMET_AB=new TH1D("GenMET_AB","GenMET_AB",200,0,2000);

  h_HT_AB=new TH1D("HT_AB","HT_AB",400,0,4000);
  h_MHT_AB=new TH1D("MHT_AB","MHT_AB",200,0,2000);
  h_nJets_AB=new TH1D("nJets_AB","nJets_AB",25,0,25);

  h_STvBin_AB=new TH1D("STvarBin_AB","STvarBin_AB",STBinLowEdge.size()-1,&(STBinLowEdge[0]));
  h_METvBin_AB=new TH1D("METvarBin_AB","MET with variable bin size_AB",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_jet1Pt_AB=new TH1D("jet1Pt_AB","Leading Jet Pt_AB",200,0,2000);
  h_jet2Pt_AB=new TH1D("jet2Pt_AB","2nd Leading Jet Pt_AB",200,0,2000);

  h_dPhi_METjet1_AB=new TH1D("dPhi_METjet1_AB","dphi between MET Vec and Jet1_AB",40,0,4);
  h_dPhi_METjet2_AB=new TH1D("dPhi_METjet2_AB","dphi between MET Vec and Jet2_AB",40,0,4);
  h_dPhi_METjet3_AB=new TH1D("dPhi_METjet3_AB","dphi between MET Vec and Jet3_AB",40,0,4);
  h_dPhi_METjet4_AB=new TH1D("dPhi_METjet4_AB","dphi between MET Vec and Jet4_AB",40,0,4);

  h_ST_CD=new TH1D("ST_CD","ST_CD",400,0,4000);
  h_MET_CD=new TH1D("MET_CD","MET_CD",200,0,2000);
  h_nHadJets_CD=new TH1D("nHadJets_CD","no. of jets(only hadronic jets,not counting photon)_CD",25,0,25);
  h_BTags_CD=new TH1D("nBTags_CD","no. of B tags_CD",10,0,10);
  h_GenMET_CD=new TH1D("GenMET_CD","GenMET_CD",200,0,2000);

  h_HT_CD=new TH1D("HT_CD","HT_CD",400,0,4000);
  h_MHT_CD=new TH1D("MHT_CD","MHT_CD",200,0,2000);
  h_nJets_CD=new TH1D("nJets_CD","nJets_CD",25,0,25);

  h_STvBin_CD=new TH1D("STvarBin_CD","STvarBin_CD",STBinLowEdge.size()-1,&(STBinLowEdge[0]));
  h_METvBin_CD=new TH1D("METvarBin_CD","MET with variable bin size_CD",METBinLowEdge.size()-1,&(METBinLowEdge[0]));

  h_jet1Pt_CD=new TH1D("jet1Pt_CD","Leading Jet Pt_CD",200,0,2000);
  h_jet2Pt_CD=new TH1D("jet2Pt_CD","2nd Leading Jet Pt_CD",200,0,2000);

  h_dPhi_METjet1_CD=new TH1D("dPhi_METjet1_CD","dphi between MET Vec and Jet1_CD",40,0,4);
  h_dPhi_METjet2_CD=new TH1D("dPhi_METjet2_CD","dphi between MET Vec and Jet2_CD",40,0,4);
  h_dPhi_METjet3_CD=new TH1D("dPhi_METjet3_CD","dphi between MET Vec and Jet3_CD",40,0,4);
  h_dPhi_METjet4_CD=new TH1D("dPhi_METjet4_CD","dphi between MET Vec and Jet4_CD",40,0,4);

  h_BestPhotonPt=new TH1D("BestPhotonPt","Pt of the Best Photon",150,0,1500);  
  h_BestPhotonPtvBin=new TH1D("BestPhotonPtvarBin","BestPhotonPt with variable bin size",BestPhotonPtBinLowEdge.size()-1,&(BestPhotonPtBinLowEdge[0]));
  h_BestG_Eta=new TH1D("BestG_Eta","Best Photon Eta",120,-6,6);
  h_BestG_Phi=new TH1D("BestG_Phi","Best Photon Phi",80,-4,4);
  h_nBestPho=new TH1D("nBestPhotons","no. of best photons",5,0,5);
  h_BestPho2Pt=new TH1D("BestPho2Pt","Pt of the second leading photon",1500,0,1500);
  h_2ndPhoGenMatch=new TH1D("2ndPhoGenMatch","Gen particle matched to 2nd leading photon",62,-0.75,30.25);
  h_dPhi_METBestPhoton=new TH1D("dPhi_METBestPhoton","dphi between MET and BestPhoton",40,0,4);
  h_GmatchedObj=new TH1D("GmatchedObj","Gen Obj close to Reco-Gamma",62,-0.75,30.25);
  h_PdgIdPhoParent = new TH1D("PdgIdPhoParent","PdgID of the Photon's parent",62,-0.75,30.25);
  h2_PtPhotonvsMET=new TH2D("BestPhotonPtvsMET","Best photon Pt vs MET",150,0,1500,200,0,2000);

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

