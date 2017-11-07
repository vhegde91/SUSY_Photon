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


TFile *f_data = new TFile("DVS_2016_ST1000.root");
TFile *f_EWMC = new TFile("VS_EW_ST1000_v2.root");
TFile *f_MultiMC = new TFile("VS_GJetsQCD_ST1000_v2.root");
// TFile *f_data = new TFile("DVS_2016_ST1000_ProxyMayBeb.root");
// TFile *f_EWMC = new TFile("VS_EW_ST1000_ProxyMayBeb.root");
// TFile *f_MultiMC = new TFile("VS_GJetsQCD_ST1000_ProxyMayBeb.root");

vector<double> cbya,doubleR;
void calc_cbya_doubleR();

void vald_ABCD(){
  calc_cbya_doubleR();
  vector<TString> histNames={"AllSBins_v1_","AllSBins_v4","MET_","METvarBin_"};
  gStyle->SetOptStat(0);
  gStyle->SetTitle(0);

  TH1D *h_EW_AB = (TH1D*)f_EWMC->Get("AllSBins_v4_AB");
  TH1D *h_data_AB = (TH1D*)f_data->Get("AllSBins_v4_AB");
  TH1D *h_EW_CD = (TH1D*)f_EWMC->Get("AllSBins_v4_CD");
  TH1D *h_data_CD = (TH1D*)f_data->Get("AllSBins_v4_CD");
  
  h_data_AB->Add(h_EW_AB,-1);
  h_data_CD->Add(h_EW_CD,-1);
  //  h_data_CD->Draw();
  //  TH1D *h_pred=new TH1D("AllSBins_v4_Pred","All search bins v4 Prediction",31,0.5,31.5);
  TH1D *h_pred = (TH1D*)h_data_AB->Clone("Pred_AllSBins_v4");
  for(int i=1;i<=h_data_AB->GetNbinsX();i++){
    int HLR_index=-1;
    if(i<=6) HLR_index = 0;
    else if(i<=11) HLR_index = 1;
    else if(i<=16) HLR_index = 2;
    else if(i<=21) HLR_index = 3;
    else if(i<=26) HLR_index = 4;
    else if(i<=31) HLR_index = 5;
    else continue;
    
    double mulFac = cbya[HLR_index]*doubleR[HLR_index];
    if(i==1 || i==7 || i==12 || i==17 || i==22 || i==27) mulFac = cbya[HLR_index];
    double pred = mulFac*h_pred->GetBinContent(i);
    
    h_pred->SetBinContent(i,mulFac*h_pred->GetBinContent(i));
    h_pred->SetBinError(i,mulFac*h_pred->GetBinError(i));
  }

  TCanvas *c_cA = new TCanvas("AllSBins_v4_Exp_Pred","c1",1500,800);
  TPad *p_top = new TPad("c1_top","c1_top",0,0.4,1,1);
  TPad *p_bot = new TPad("c1_bot","c1_bot",0,0.0,1,0.4);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  p_top->SetBottomMargin(0);
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.3);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();
  TLegend *legend=new TLegend(0.7, 0.90,  0.85, 0.75);
  
  p_top->cd();
  h_pred->SetTitle(";Bin no.;Events");
  h_pred->SetLineColor(kCyan);
  h_pred->SetFillColor(h_pred->GetLineColor());
  h_pred->GetXaxis()->SetLabelSize(.07);
  h_pred->GetYaxis()->SetLabelSize(.09);
  h_pred->GetYaxis()->SetTitleSize(0.08);
  h_pred->Draw("BAR");
  h_data_CD->SetLineColor(kBlack);
  h_data_CD->SetMarkerStyle(20);
  h_data_CD->Draw("same");
  legend->AddEntry(h_data_CD,"Exp. Multijet","pl");
  legend->AddEntry(h_pred,"Pred. Multijet","f");
  legend->Draw();

  TH1D *h1_cp=(TH1D*)h_data_CD->Clone("h1cp_exp");
  h1_cp->Divide(h_pred);
  p_bot->cd();
  h1_cp->SetMaximum(2);
  h1_cp->SetMinimum(0.);

  h1_cp->SetTitle(";Bin no.;Exp/Pred");
  h1_cp->GetXaxis()->SetLabelSize(.13);
  h1_cp->GetYaxis()->SetLabelSize(.13);
  h1_cp->GetXaxis()->SetTitleSize(0.13);
  h1_cp->GetYaxis()->SetTitleSize(0.13);
  h1_cp->Draw();
  gPad->RedrawAxis();
}

void calc_cbya_doubleR(){
  TH2D *h2_data_AB = (TH2D*)f_data->Get("METnHadJ_0b_v2_AB");
  TH2D *h2_EW_AB = (TH2D*)f_EWMC->Get("METnHadJ_0b_v2_AB");
  TH2D *h2_data_CD = (TH2D*)f_data->Get("METnHadJ_0b_v2_CD");
  TH2D *h2_EW_CD = (TH2D*)f_EWMC->Get("METnHadJ_0b_v2_CD");

  h2_data_AB->Add(h2_EW_AB,-1);
  h2_data_CD->Add(h2_EW_CD,-1);
  h2_data_CD->Divide(h2_data_AB);

  for(int i=1;i<=h2_data_CD->GetNbinsY();i++){
    double bincnt = h2_data_CD->GetBinContent(h2_data_CD->GetXaxis()->FindBin(105),i);
    if(abs(bincnt) < 0.00001) continue;
    cbya.push_back( bincnt );
  }
  //------------------------------------------------------
  h2_data_AB = (TH2D*)f_data->Get("METnHadJ_min1b_v2_AB");
  h2_EW_AB = (TH2D*)f_EWMC->Get("METnHadJ_min1b_v2_AB");
  h2_data_CD = (TH2D*)f_data->Get("METnHadJ_min1b_v2_CD");
  h2_EW_CD = (TH2D*)f_EWMC->Get("METnHadJ_min1b_v2_CD");

  h2_data_AB->Add(h2_EW_AB,-1);
  h2_data_CD->Add(h2_EW_CD,-1);
  h2_data_CD->Divide(h2_data_AB);

  for(int i=1;i<=h2_data_CD->GetNbinsY();i++){
    double bincnt = h2_data_CD->GetBinContent(h2_data_CD->GetXaxis()->FindBin(105),i);
    if(abs(bincnt) < 0.00001) continue;
    cbya.push_back( bincnt );
  }
  //------------------------------------------------------

  TH2D *h2_MultiMC_AB = (TH2D*)f_MultiMC->Get("METnHadJ_0b_v2_AB");
  TH2D *h2_MultiMC_CD= (TH2D*)f_MultiMC->Get("METnHadJ_0b_v2_CD");
  h2_MultiMC_CD->Divide(h2_MultiMC_AB);
  double num,den;
  for(int i=1;i<=h2_MultiMC_CD->GetNbinsY();i++){
    num = h2_MultiMC_CD->GetBinContent(h2_MultiMC_CD->GetXaxis()->FindBin(205),i);
    den = h2_MultiMC_CD->GetBinContent(h2_MultiMC_CD->GetXaxis()->FindBin(105),i);
    if(abs(num) < 0.0001 || abs(den) < 0.0001) continue;
    doubleR.push_back( num/den );
  }

  h2_MultiMC_AB = (TH2D*)f_MultiMC->Get("METnHadJ_min1b_v2_AB");
  h2_MultiMC_CD= (TH2D*)f_MultiMC->Get("METnHadJ_min1b_v2_CD");
  h2_MultiMC_CD->Divide(h2_MultiMC_AB);
  for(int i=1;i<=h2_MultiMC_CD->GetNbinsY();i++){
    num = h2_MultiMC_CD->GetBinContent(h2_MultiMC_CD->GetXaxis()->FindBin(205),i);
    den = h2_MultiMC_CD->GetBinContent(h2_MultiMC_CD->GetXaxis()->FindBin(105),i);
    if(abs(num) < 0.0001 || abs(den) < 0.0001) continue;
    doubleR.push_back( num/den );
  }

  if(doubleR.size()!=cbya.size()) cout<<"doubleR size:"<<doubleR.size()<<" C/A size:"<<cbya.size()<<endl;
  cout<<"C/A"<<endl;
  for(int i=0;i<cbya.size();i++){
    cout<<cbya[i]<<" ";
  }cout<<endl;
  cout<<"Double Ratio:"<<endl;
  for(int i=0;i<doubleR.size();i++){
    cout<<doubleR[i]<<" ";
  }cout<<endl;

}
