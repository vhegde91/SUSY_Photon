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

const int nfiles=8,nBG=8;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
char name[100];

void dPhiPlots(){

  f[0] = new TFile("CS_WG_LostMuHadTau_v2.root");
  f[1] = new TFile("CS_WJets_LostMuHadTau_v2.root");
  f[2] = new TFile("CS_TTJets_LostMuHadTau_v2.root");
  f[3] = new TFile("CS_TTG_LostMuHadTau_v2.root");
  f[4] = new TFile("CS_TG_LostMuHadTau_v2.root");
  f[5] = new TFile("CS_ZGToLLG_LostMuHadTau_v2.root");
  f[6] = new TFile("CS_DYJetsToLL_LostMuHadTau_v2.root");
  f[7] = new TFile("CS_TTWZ_LostMuHadTau_v2.root");
  
 
  // f[7] = new TFile("HG_NLSP150.root");
  //  f[8] = new TFile("HG_NLSP1000.root");
  //  f[8] = new TFile("HG_NLSP1550.root");
  //  f[5] = new TFile("NLSP100.root");
  //  f[6] = new TFile("NLSP1000.root");
  //f[7] = new TFile("NLSP1500.root");
  //  gStyle->SetOptStat("nemri");

  // c_dphi[0]=new TCanvas("dphi_METBestPhotonPt","dphi_METBestPhotonPt",1500,800);c_dphi[0]->Divide(4,2);
  // c_dphi[1]=new TCanvas("dphi_METjet1","dphi_METjet1",1500,800);c_dphi[1]->Divide(4,2);
  // c_dphi[2]=new TCanvas("dphi_METjet2","dphi_METjet2",1500,800);c_dphi[2]->Divide(4,2);
  // c_dphi[3]=new TCanvas("dphi_METjet3","dphi_METjet3",1500,800);c_dphi[3]->Divide(4,2);
  // c_dphi[4]=new TCanvas("dphi_MET4jet","dphi_MET4jet",1500,800);c_dphi[4]->Divide(4,2);

  vector<string> name1;
  name1.push_back("invMassPhoMu");
  name1.push_back("invMassMuPhoNu");
  name1.push_back("dR_MuPho");
  name1.push_back("MadMinPhotonDeltaR");
  name1.push_back("mindRPhoLep");
  
  TCanvas *c_dphi[name1.size()];
  for(int i=0;i<name1.size();i++){
    sprintf(name,"%s",name1[i].c_str());
    c_dphi[i]=new TCanvas(name,name,1500,800);c_dphi[i]->Divide(4,2);
    TH1D *h_hist0;
    TH2D *h2_hist2;
    for(int j=0;j<nfiles;j++){
      c_dphi[i]->cd(j+1);c_dphi[i]->SetLogy();
      h_hist0=(TH1D*)f[j]->FindObjectAny(name);
      if(h_hist0){
	h_hist0->SetBinContent(h_hist0->GetNbinsX(),h_hist0->GetBinContent(h_hist0->GetNbinsX())+h_hist0->GetBinContent(h_hist0->GetNbinsX()+1));
	h_hist0->SetTitle(f[j]->GetName());
	h_hist0->SetMarkerSize(1.8);
	//	h_hist0->Draw("HIST text");
	h_hist0->Draw("HIST");
      }
      else if(h2_hist2){
	h2_hist2->SetTitle(f[j]->GetName());
	h2_hist2->Draw("colz");
	c_dphi[i]->SetLogz();
	gPad->Update();
      }
    }
  }
 
  //  gStyle->SetTextSize(4);
  
}
