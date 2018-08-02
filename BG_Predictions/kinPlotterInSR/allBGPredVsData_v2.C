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

const int nfiles=8,nBGs=5,nHists=8,nSig=nHists-nBGs-1;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
//int col[11]={kPink-2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};  //Specify Colors
int col[11]={kOrange,kGray,kRed,kTeal+9,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};

TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getLegName(TString);
TString getXaxisName(TString);
void printBinContents(TH1D*);
double data_Integral=0,mc_Integral=0;
TLatex textOnTop,intLumiE;
double intLumi=35.9;
void allBGPredVsData_v2(){
  //  gStyle->Reset();
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  //  gStyle->SetOptStat("nemri");
  vector<TString> suffixHist; 
  
  f[0] = new TFile("CS_D_ZGZJToNuNuG_LO.root");
  f[1] = new TFile("DCS_D_Run2016_LostEle_v2.root");
  f[2] = new TFile("DCS_D_Run2016_LostMuHadTau_v2.root");
  f[3] = new TFile("DCS_D_Run2016_SingleEle_v2.root");
  //f[4] = new TFile("gjets_qcd_wtdToDataInNjbJets.root");
  f[4] = new TFile("SBinHists.root"); //for complete prediction
  f[5] = new TFile("DCS_LDP_Run2016_Multijet_v2.root");

  // f[6] = new TFile("../FastSim_T5bbbbZG_1800_150.root");
  // f[7] = new TFile("../FastSim_T5bbbbZG_1800_1550.root");
  // f[6] = new TFile("../forAddInfo/FastSim_T5ttttZg_1800_150.root");
  // f[7] = new TFile("../forAddInfo/FastSim_T5ttttZg_1800_1550.root");
  // f[6] = new TFile("../forAddInfo/FastSim_T5qqqqHg_1800_150.root");
  // f[7] = new TFile("../forAddInfo/FastSim_T5qqqqHg_1800_1750.root");
  f[6] = new TFile("../forAddInfo/FastSim_T6ttZg_1000_100.root");
  f[7] = new TFile("../forAddInfo/FastSim_T6ttZg_1000_900.root");
  
  TFile *fout;
  int rebin=1;
  TString xName;
  //TString name1=""; xName = "p_{T}^{#gamma} (GeV)"; rebin=10; //BestPhotonPt
  //TString name1=""; xName = "#Delta#Phi(p_{T}^{miss},#gamma)"; rebin=4;
  //  TString name1=""; xName = "mT(#gamma, p_{T}^{miss})"; rebin=10;
  TString name1="AllSBins_v7"; xName = "Bin number"; rebin=1;
  //TString name1="ST"; xName = "ST (GeV)"; rebin=25;
  //  TString name1="MET"; xName = "p_{T}^{miss} (GeV)"; rebin=10;
  //TString name1="nHadJets"; xName = "N_{Jets}"; rebin=1;
  //  TString name1="nBTags"; xName = "N_{ b tags}"; rebin=1;

  //vector<TString> histName={"BestPhotonPt","BestPhotonPt_Ele0","BestPhotonPt_Mu0","BestPhotonPt","BestPhotonPt_D","BestPhotonPt_D","BestPhotonPt_D","BestPhotonPt_D"};//photon Pt
  //vector<TString> histName={"dPhi_METBestPhoton","dPhi_METBestPhoton_Ele0","dPhi_METBestPhoton_Mu0","dPhi_METBestPhoton","dPhi_METBestPhoton_D","dPhi_METBestPhoton_D","dPhi_METBestPhoton_D","dPhi_METBestPhoton_D"};//dPhi(photon,MET)
  //vector<TString> histName={"mTPho","mTPho_Ele0","mTPho_Mu0","mT_Pho","mTPho_D","mTPho_D","mTPho_D","mTPho_D"};//mT(photon,MET)

  //  vector<TString> histName={"","_Ele0","_Mu0","_Pho","_D","_D","_D","_D"};
  vector<TString> histName={"","_Ele0","_Mu0","_Pho","_MultiJPred","_D","_D","_D"}; //for complete prediction
  for(int i=0;i<nHists;i++){
    histName[i] = name1+histName[i];
  }
  
  TH1D *hTemp = (TH1D*)f[0]->Get(histName[0]);
  TH1D *h_totBG = (TH1D*)hTemp->Clone("totalBG");
  h_totBG->Reset();
  h_totBG->Rebin(rebin);
  setLastBinAsOverFlow(h_totBG);

  TH1D *h_hist[nHists];
  THStack *hs_BG = new THStack("predVsData","predVsData");
  TLegend *legend=new TLegend(0.2, 0.88,  0.86, 0.70);
  for(int i=0;i<nHists;i++){
    h_hist[i] = (TH1D*)f[i]->Get(histName[i]);
    cout<<f[i]->GetName()<<" "<<h_hist[i]->GetNbinsX()<<endl;
    //------------------for complete prediction
    for(int j=1;j<=h_hist[i]->GetNbinsX();j++){
      if( j==1 || j==7 || j==12 || j==17 || j==22 || j==27 )h_hist[i]->SetBinError(j,0);
      if( j==1 || j==7 || j==12 || j==17 || j==22 || j==27 )h_hist[i]->SetBinContent(j,0);
    }
    //------------------for complete prediction ends
    h_hist[i]->Rebin(rebin);
    setLastBinAsOverFlow(h_hist[i]);

    if(i==0) h_hist[i]->Scale(1.13984);
    if(i<nBGs) h_totBG->Add(h_hist[i]);

    if(i<nBGs+1) h_hist[i]->SetLineColor(kBlack);
    else h_hist[i]->SetLineColor(col[i]);
    //    cout<<f[i]->GetName()<<" "<<h_hist[i]->Integral()<<endl;
    if(i<nBGs) {
      h_hist[i]->SetFillColor(col[i]);
      h_hist[i]->SetLineWidth(1);
      hs_BG->Add(h_hist[i]);
      hs_BG->SetMinimum(0.5);
      hs_BG->SetMaximum(4000);
      //      hs_BG->SetMaximum(100000);
      hs_BG->SetTitle(0);
      hs_BG->SetTitle(";;Events");
      // hs_BG->GetXaxis()->SetTitle(xName);
      legend->AddEntry(h_hist[i],getLegName(f[i]->GetName()),"f");
    }
    else{
      h_hist[i]->SetLineWidth(3);
      if(i==nBGs) legend->AddEntry(h_hist[i],getLegName(f[i]->GetName()),"ep");
      else{
	if(i==nBGs+1) legend->AddEntry(h_hist[i],getLegName(f[i]->GetName()),"lp");
	if(i==nBGs+2) legend->AddEntry(h_hist[i],getLegName(f[i]->GetName()),"lp");
      }
    }
  }
  legend->SetNColumns(4);
  legend->SetBorderSize(0);
  TCanvas *cA = new TCanvas("c_predVsData","c_predVsData",1500,800);
  TPad *p_top=new TPad("top","top",0,0.4,1,1);
  TPad *p_bot=new TPad("bot","bot",0,0.0,1,0.4);
  p_top->SetBottomMargin(0);
  p_top->Draw();p_top->SetGridx(0);p_top->SetGridy(0);p_top->SetLogy();
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.3);
  p_bot->Draw();p_bot->SetGridx(0);p_bot->SetGridy();  
  p_top->cd();
  hs_BG->Draw("hist");
  hs_BG->GetYaxis()->SetTitleOffset(0.50);    
  hs_BG->GetYaxis()->SetTitleSize(0.09);
  hs_BG->GetYaxis()->SetLabelSize(0.09);

  TH1D *h_ratio;
  for(int i=nBGs;i<nHists;i++){
    name = f[i]->GetName();
    if(name=="DCS_LDP_Run2016_Multijet_v2.root"){
      h_hist[i]->SetMarkerStyle(20);
      h_hist[i]->SetLineWidth(2);
      h_ratio = (TH1D*)h_hist[i]->Clone("DataOverBG");
      //     h_hist[i]->GetXAxis()->SetErrorX(0);
      h_hist[i]->Draw("E1X0 same");
    }
    else{ 
      h_hist[i]->Draw("hist same");
    }
  }
  h_totBG->SetFillStyle(3013);
  h_totBG->SetFillColor(1);
  h_totBG->Draw("E2SAME");

  h_ratio->Divide(h_totBG);

  gPad->RedrawAxis();
  legend->Draw();
  
  // for(int i=1;i<=h_ratio->GetNbinsX();i++){
  //   if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 )h_ratio->SetBinError(i,0);
  //   if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 )h_ratio->SetBinContent(i,0);
  // }
  h_ratio->SetLineColor(kBlack);
  h_ratio->SetMarkerColor(kBlack);
  h_ratio->SetTitle(0);
  h_ratio->GetXaxis()->SetTitle(xName);
  h_ratio->GetXaxis()->SetTitleOffset(0.97);
  h_ratio->GetXaxis()->SetTitleSize(0.13);
  h_ratio->GetXaxis()->SetLabelSize(0.14);

  h_ratio->GetYaxis()->SetTitle("Obs. / Pred.");
  h_ratio->GetYaxis()->SetTitleOffset(0.35);
  h_ratio->GetYaxis()->SetTitleSize(0.13);
  h_ratio->GetYaxis()->SetLabelSize(0.14);
  h_ratio->GetYaxis()->SetNdivisions(505);
  h_ratio->SetMarkerStyle(20);
  h_ratio->SetMaximum(1.99);
  h_ratio->SetMinimum(0.01);
  cA->cd();    p_bot->cd();
  p_bot->SetTickx();p_bot->SetTicky();
  //    c_cB->cd(i+1);    p_bot[i]->cd();
  h_ratio->Draw("X0E1");
  
  p_top->cd();
  char name2[100];
  textOnTop.SetTextSize(0.07);
  intLumiE.SetTextSize(0.07);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Supplementary}}");
  intLumiE.SetTextSize(0.07);
  sprintf(name2,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.73,0.91,name2);

  // TLine *line1V7=new TLine( 6.5,0.01,  6.5,1300);
  // TLine *line2V7=new TLine(11.5,0.01, 11.5,1300);
  // TLine *line3V7=new TLine(16.5,0.01, 16.5,1300);
  // TLine *line4V7=new TLine(21.5,0.01, 21.5,1300);
  // TLine *line5V7=new TLine(26.5,0.01, 26.5,1300);
  // p_top->cd(); p_top->SetGridx(0);
  // line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
  // p_bot->cd(); p_bot->SetGridx(0);
  // line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();

  // TArrow *arrow1 = new TArrow(0.5,1300,6.5,1300,0.01,"<|>");
  // TArrow *arrow2 = new TArrow(6.5,1300,11.5,1300,0.01,"<|>");
  // TArrow *arrow3 = new TArrow(11.5,1300,16.5,1300,0.01,"<|>");
  // TArrow *arrow4 = new TArrow(16.5,1300,21.5,1300,0.01,"<|>");
  // TArrow *arrow5 = new TArrow(21.5,1300,26.5,1300,0.01,"<|>");
  // TArrow *arrow6 = new TArrow(26.5,1300,31.5,1300,0.01,"<|>");
  // arrow1->Draw(); arrow2->Draw(); arrow3->Draw();
  // arrow4->Draw(); arrow5->Draw(); arrow6->Draw();
  // TLatex Tl;
  // Tl.SetTextSize(0.08);
  // Tl.DrawLatex(3.,2000,"N^{0}_{2-4}");
  // Tl.DrawLatex(8.5,2000,"N^{0}_{5-6}");
  // Tl.DrawLatex(13.5,2000,"N^{0}_{#geq7}");
  // Tl.DrawLatex(18.5,2000,"N^{#geq1}_{2-4}");
  // Tl.DrawLatex(23.5,2000,"N^{#geq1}_{5-6}");
  // Tl.DrawLatex(28.5,2000,"N^{#geq1}_{#geq7}");

  // TLatex Tl2;
  // Tl2.SetTextSize(0.05);
  // Tl2.SetTextAngle(90);
  // Tl2.DrawLatex(1.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(7.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(12.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(17.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(22.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(27.1,1,"#bf{Sideband}");
  // //  Tl2.DrawLatex(21.6,1,"#bf{Sideband}");

  // cA->SaveAs("c_predVsData.pdf");
}

TString getLegName(TString fname){
  if(fname.Contains("ZGZJToNuNuG")){return "Z(#nu#bar{#nu}) + #gamma";}
  else if(fname.Contains("LostEle")){return "Lost e";}
  else if(fname.Contains("LostMu")){return "Lost #mu + #tau_{had}";}
  else if(fname.Contains("SingleEle")){return "Misidentified #gamma";}
  else if(fname.Contains("SBinHists") || fname.Contains("gjets")  ){return "#gamma + jets";}
  else if(fname.Contains("T5bbbbZG_1600_150")){return "T5bbbbZG_150";}
  else if(fname.Contains("T5bbbbZG_1600_1550")){return "T5bbbbZG_1550";}
  else if(fname.Contains("T5bbbbZG_1800_150")){return "T5bbbbZG (1800,150)";}
  else if(fname.Contains("T5bbbbZG_1800_1750")){return "T5bbbbZG (1800,1750)";}
  else if(fname.Contains("TChiNG_0_800")){return "TChiNG_800";}
  else if(fname.Contains("TChiWG_0_800")){return "TChiWG_800";}
  else if(fname.Contains("DCS_LDP_Run2016") ){return "Data";}

  else if(fname.Contains("T5bbbbZg_1800_150")){return "T5bbbbZg (1800,150)";}
  else if(fname.Contains("T5bbbbZg_1800_1750")){return "T5bbbbZg (1800,1750)";}
                          
  else if(fname.Contains("T5ttttZg_1800_150")){return "T5ttttZg (1800,150)";}
  else if(fname.Contains("T5ttttZg_1800_1550")){return "T5ttttZg (1800,1550)";}

  else if(fname.Contains("T5qqqqHg_1800_150")){return "T5qqqqHg (1800,150)";}
  else if(fname.Contains("T5qqqqHg_1800_1750")){return "T5qqqqHg (1800,1750)";}

  else if(fname.Contains("T6ttZg_1000_100")){return "T6ttZg (1000,100)";}
  else if(fname.Contains("T6ttZg_1000_900")){return "T6ttZg (1000,900)";}

  else return fname;
}
TString getXaxisName(TString axname){
  if(axname.Contains("nHadJets")) return "Jets";
  else if(axname.Contains("ST")) return "ST(GeV)";
  else if(axname.Contains("BTags")) return "b-Tags";
  else if(axname.Contains("ElePt")) return "e pT(GeV)";
  else if(axname.Contains("PhotonPt")) return "#gamma pT(GeV)";
  else if(axname.Contains("mT")) return "mT_{#gamma,MET}(GeV)";
  else if(axname.Contains("AllSBin")) return "Bin Number";
  else if(axname.Contains("dPhi_METjet1") || axname.Contains("dphi1_METjet1")) return "#Delta#Phi_{1}";
  else if(axname.Contains("dPhi_METjet2") || axname.Contains("dphi2_METjet2")) return "#Delta#Phi_{2}";
  else if(axname.Contains("dPhi_METBestPhoton") ) return "#Delta#Phi(MET,#gamma)";
  else if(axname.Contains("QMut") || axname.Contains("Qmut")) return "QMult";
  else if(axname.Contains("MET")) return "MET(GeV)";
  else return axname;

}

void setLastBinAsOverFlow(TH1D* h_hist){
  double lastBinCt =h_hist->GetBinContent(h_hist->GetNbinsX()),overflCt =h_hist->GetBinContent(h_hist->GetNbinsX()+1);
  double lastBinErr=h_hist->GetBinError(h_hist->GetNbinsX()),  overflErr=h_hist->GetBinError(h_hist->GetNbinsX()+1);
  
  if(lastBinCt!=0 && overflCt!=0)
    lastBinErr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );
  
  else if(lastBinCt==0 && overflCt!=0)
    lastBinErr = overflErr;
  else if(lastBinCt!=0 && overflCt==0)
    lastBinErr = lastBinErr;
  else lastBinErr=0;

  lastBinCt = lastBinCt+overflCt;
  h_hist->SetBinContent(h_hist->GetNbinsX(),lastBinCt);
  h_hist->SetBinError(h_hist->GetNbinsX(),lastBinErr);
    
}

void printBinContents(TH1D *h1){
  for(int i=1;i<=h1->GetNbinsX();i++){
    cout<<h1->GetBinContent(i)<<" "<<h1->GetBinError(i)<<endl;
  }
}
