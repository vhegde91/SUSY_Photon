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

void setNegEntryBinsTo0(TH1D*);
void EW_subtract(){
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);

  const int nFiles = 2;
  TFile *f[nFiles];

  f[0] = new TFile("DVS_2016_ST1000.root");
  f[1] = new TFile("VS_EW_ST1000_v2.root");
  // f[0] = new TFile("DVS_doubleR_2016_ST1000.root");
  // f[1] = new TFile("VS_EW_ST1000_doubleR_CbyAdata.root");

  TString outFname = f[0]->GetName(); outFname = "EW_Subtracted_"+outFname;
  TFile *fout = new TFile(outFname,"recreate");

  vector<TString> histList = {"ST_","MET_","METvarBin_","nHadJets_","nBTags_",
			      "mTPho_","BestPhotonPt_",
			      "AllSBins_v1_","AllSBins_v4_"};
  vector<TString> histList2D = {"METnHadJ_0b_AB","METnHadJ_min1b_AB",
				"METnHadJ_0b_CD","METnHadJ_min1b_CD",
				"METnHadJ_0b_v2_AB","METnHadJ_min1b_v2_AB",
                                "METnHadJ_0b_v2_CD","METnHadJ_min1b_v2_CD",};
  
  TH1D *h0,*h1;
  for(int i=0;i<histList.size();i++){
    for(char reg='A';reg<='D';reg++){
      h0 = (TH1D*)f[0]->Get(histList[i]+reg);
      h1 = (TH1D*)f[1]->Get(histList[i]+reg);
      h0->Add(h1,-1);
      //setNegEntryBinsTo0(h0);
      fout->cd(); h0->Write();
    }

    h0 = (TH1D*)f[0]->Get(histList[i]+"AB");
    h1 = (TH1D*)f[1]->Get(histList[i]+"AB");
    h0->Add(h1,-1);
    //    setNegEntryBinsTo0(h0);
    fout->cd(); h0->Write();

    h0 = (TH1D*)f[0]->Get(histList[i]+"CD");
    h1 = (TH1D*)f[1]->Get(histList[i]+"CD");
    h0->Add(h1,-1);
    // setNegEntryBinsTo0(h0);
    fout->cd(); h0->Write();
  }

  TH2D *h2_0,*h2_1;
  for(int i=0;i<histList2D.size();i++){
    h2_0 = (TH2D*)f[0]->Get(histList2D[i]);
    h2_1 = (TH2D*)f[1]->Get(histList2D[i]);
    h2_0->Add(h2_1,-1);
    fout->cd(); h2_0->Write();
  }

}



void setNegEntryBinsTo0(TH1D* hist){
  for(int i=0;i<=hist->GetNbinsX()+1;i++){
    if(hist->GetBinContent(i) < 0) hist->SetBinContent(i,0);
  }
}
