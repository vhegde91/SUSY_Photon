#include"TFile.h"
#include"TDirectory.h"
#include"TTree.h"
#include"TBrowser.h"
#include"TF1.h"
#include"TH2.h"
#include<string>
#include<vector>
#include"TGraphErrors.h"
#include"TGraph.h"
#include"TLegend.h"
#include"TLatex.h"
#include"TCanvas.h"
#include"THStack.h"
#include"TStyle.h"

TFile *f_multiJetRaw = new TFile("DCS_LDP_Run2016_Multijet_v2.root");
TFile *f_ZNuNuG = new TFile("CS_ZGZJToNuNuG.root");
TFile *f_LostMu = new TFile("DCS_Run2016_LostMuHadTau_v2.root");
TFile *f_LostEle = new TFile("DCS_Run2016_LostEle_v2.root");
TFile *f_fakerate = new TFile("DCS_Run2016_SingleEle_v2.root");

TH1D *h_multiJetRaw = (TH1D*)f_multiJetRaw->Get("AllSBins_v7_CD");
TH1D *h_ZNuNuG = (TH1D*)f_ZNuNuG->Get("AllSBins_v7");
TH1D *h_LostMu = (TH1D*)f_LostMu->Get("AllSBins_v7_Mu0");
TH1D *h_LostEle = (TH1D*)f_LostEle->Get("AllSBins_v7_Ele0");
TH1D *h_fakerate = (TH1D*)f_fakerate->Get("AllSBins_v7_Pho");

TFile *f_ZNuNuG_LDP = new TFile("CS_LDP_ZGZJToNuNuG.root");
TFile *f_LostMu_LDP = new TFile("DCS_LDP_Run2016_LostMuHadTau_v2.root");
TFile *f_LostEle_LDP = new TFile("DCS_LDP_Run2016_LostEle_v2.root");
TFile *f_fakerate_LDP = new TFile("DCS_LDP_Run2016_SingleEle_v2.root");

TH1D *h_multiJetRaw_LDP = (TH1D*)f_multiJetRaw->Get("AllSBins_v7_AB");
TH1D *h_ZNuNuG_LDP = (TH1D*)f_ZNuNuG_LDP->Get("AllSBins_v7");
TH1D *h_LostMu_LDP = (TH1D*)f_LostMu_LDP->Get("AllSBins_v7_Mu0");
TH1D *h_LostEle_LDP = (TH1D*)f_LostEle_LDP->Get("AllSBins_v7_Ele0");
TH1D *h_fakerate_LDP = (TH1D*)f_fakerate_LDP->Get("AllSBins_v7_Pho");

TH1D *h_multiJet_LDP = (TH1D*)h_multiJetRaw_LDP->Clone("AllSBins_v7_AB");
TH1D *h_multiJet = (TH1D*)h_multiJetRaw->Clone("AllSBins_v7_CD");

double doubleRatio[6] = {0.291625, 0.466531, 0.398334, 0.241211, 0.344722, 0.474065};

void makeMultiJetPred(){
  h_multiJet_LDP->Add(h_ZNuNuG_LDP,-1);
  h_multiJet_LDP->Add(h_LostMu_LDP,-1);
  h_multiJet_LDP->Add(h_LostEle_LDP,-1);
  h_multiJet_LDP->Add(h_fakerate_LDP,-1);

  h_multiJet->Add(h_ZNuNuG,-1);
  h_multiJet->Add(h_LostMu,-1);
  h_multiJet->Add(h_LostEle,-1);
  h_multiJet->Add(h_fakerate,-1);

  int iRegion=0;    float hlRatio = 0;
  for(int i=1;i<=h_multiJet->GetNbinsX();i++){
    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 ){
      hlRatio = (h_multiJet->GetBinContent(i)) / (h_multiJet_LDP->GetBinContent(i));
      iRegion++;
      cout<<endl;
      //      cout<<i<<" LDP: "<<h_multiJet_LDP->GetBinContent(i)<<" hlR: "<<hlRatio<<" doubleR: "<<doubleRatio[iRegion-1]<<endl;
    }
    else{
      // if(i<7) iRegion = 1;
      // else if(i<12) iRegion = 2;
      // else if(i<17) iRegion = 3;
      // else if(i<22) iRegion = 4;
      // else if(i<27) iRegion = 5;
      // else          iRegion = 6;
      h_multiJet->SetBinContent(i, (h_multiJet_LDP->GetBinContent(i))*hlRatio*doubleRatio[iRegion-1]);
    }
    cout<<i<<" LDP: "<<h_multiJet_LDP->GetBinContent(i)<<" hlR: "<<hlRatio<<" doubleR: "<<doubleRatio[iRegion-1]<<endl;
    if(h_multiJet->GetBinCenter(i)!=i)cout<<i<<" "<<h_multiJet->GetBinCenter(i)<<endl;
  }

  TFile *fout = new TFile("DCS_Run2016_Multijet_Pred_v2.root","recreate");
  fout->cd();
  h_multiJet->Write();
  h_multiJet->Draw();
}
