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


void getDoubleR(TString fName){
  TFile *f = TFile::Open(fName);
  TH2D *h2_HLR0b = (TH2D*)f->Get("HLRatio_0");
  TH2D *h2_HLR1b = (TH2D*)f->Get("HLRatio_1");

  TH1D *h_doubleR_num0 = (TH1D*)h2_HLR0b->ProjectionY("doubleR_num0",h2_HLR0b->GetXaxis()->FindBin(201),h2_HLR0b->GetXaxis()->FindBin(201));
  TH1D *h_doubleR_den0 = (TH1D*)h2_HLR0b->ProjectionY("doubleR_den0",h2_HLR0b->GetXaxis()->FindBin(101),h2_HLR0b->GetXaxis()->FindBin(101));

  TH1D *h_doubleR_0b = (TH1D*)h_doubleR_num0->Clone("doubleR_0b");
  h_doubleR_0b->Divide(h_doubleR_den0);



  TH1D *h_doubleR_num1 = (TH1D*)h2_HLR1b->ProjectionY("doubleR_num1",h2_HLR1b->GetXaxis()->FindBin(201),h2_HLR1b->GetXaxis()->FindBin(201));
  TH1D *h_doubleR_den1 = (TH1D*)h2_HLR1b->ProjectionY("doubleR_den1",h2_HLR1b->GetXaxis()->FindBin(101),h2_HLR1b->GetXaxis()->FindBin(101));

  TH1D *h_doubleR_1b = (TH1D*)h_doubleR_num1->Clone("doubleR_1b");
  h_doubleR_1b->Divide(h_doubleR_den1);

  h_doubleR_0b->Draw("text");
  h_doubleR_1b->Draw("same text");

}
