#include<iostream>
#include<iomanip>
#include"TH1.h"
#include"TROOT.h"
#include"TH2.h"
#include"TFile.h"
#include"TDirectory.h"
#include"TTree.h"
#include"TBrowser.h"
#include"TF1.h"
#include<string>
#include<vector>
#include"TGraphErrors.h"
#include"TGraph.h"
#include"TLegend.h"
#include"TLatex.h"
#include"TCanvas.h"
#include"THStack.h"
#include"TStyle.h"


void make1DFm2DHists(){

  TString finName = "DMS_Run2016_FR_ST200_BGSub_v2.root",foutName = "1D_"+finName;
  TFile *fin=new TFile(finName);
  TFile *fout=new TFile(foutName,"RECREATE");
  vector<TString> histNames1D,histNames2D;
  //------------------- Pho ------------------ 
  histNames2D.push_back("ST_ZMass_Pho");
  histNames1D.push_back("ST_Pho");

  histNames2D.push_back("nHadJets_ZMass_Pho");
  histNames1D.push_back("nHadJets_Pho");

  histNames2D.push_back("ElefPt_ZMass_Pho");
  histNames1D.push_back("F_ElePt_Pho");

  histNames2D.push_back("ElefPtvBin_ZMass_Pho");
  histNames1D.push_back("F_ElePtvBin_Pho");

  histNames2D.push_back("QMultJetElef_ZMass_Pho");
  histNames1D.push_back("QMultJetElef_Pho");
  //------------------- Ele ------------------ 
  histNames2D.push_back("ST_ZMass_Ele");
  histNames1D.push_back("ST_Ele");

  histNames2D.push_back("nHadJets_ZMass_Ele");
  histNames1D.push_back("nHadJets_Ele");

  histNames2D.push_back("ElefPt_ZMass_Ele");
  histNames1D.push_back("F_ElePt_Ele");

  histNames2D.push_back("ElefPtvBin_ZMass_Ele");
  histNames1D.push_back("F_ElePtvBin_Ele");

  histNames2D.push_back("QMultJetElef_ZMass_Ele");
  histNames1D.push_back("QMultJetElef_Ele");

  TH1D *h1;
  TH2D *h2d;
  for(int i=0;i<histNames2D.size();i++){
    h2d=(TH2D*)fin->FindObjectAny(histNames2D[i]);
    //    h2d->Draw("colz");
    h1=(TH1D*)h2d->ProjectionX(histNames1D[i]);
    //    h1->Draw("text");
    fout->cd();
    h1->Write();
    delete h1;
  }

}
