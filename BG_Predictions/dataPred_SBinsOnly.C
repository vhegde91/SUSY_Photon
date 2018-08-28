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

const int nfiles=3,nBGs=5,nHists=8,nSig=nHists-nBGs-1;    //Specify no. of files
TFile *f[nfiles];
//int col[11]={kPink-2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};  //Specify Colors Good
//int col[11]={kGray,kTeal+9,kOrange,kPink-2,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};  //Specify Colors
int col[11]={kOrange,kGray,kRed,kTeal+9,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};  //Specify Colors
//int col[11]={kOrange,kCyan,kPink-2,kYellow,kGreen,kTeal+9,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};  //Specify Colors

TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getLegName(TString);
TString getXaxisName(TString);
void setSBinHist(TH1D*,TH1D*);
void printBinContents(TH1D*);
double data_Integral=0,mc_Integral=0;
TLatex textOnTop,intLumiE;
double intLumi=35.9;
void dataPred_SBinsOnly(TString iFname){
  //  gStyle->Reset();
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  //---------------- set styles if not running on my desktop ---------------
  //---------------- otherwise you do not need this part -------------------
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetHistLineWidth(2);
  gStyle->SetTitleSize(0.05,"X");
  gStyle->SetLabelSize(0.05,"X");
  gStyle->SetTitleSize(0.05,"Y");
  gStyle->SetLabelSize(0.05,"Y");
  gStyle->SetTitleSize(0.05,"Z");
  gStyle->SetLabelSize(0.05,"Z");
  gStyle->SetLineScalePS(1.5);
  gROOT->ForceStyle();
  //---------------- end of settings ----------------------------
  //  gStyle->SetOptStat("nemri");
  vector<TString> suffixHist; 
  f[0] = new TFile(iFname);
  // f[1] = new TFile("FastSim_T5bbbbZG_1600_150.root");
  // f[2] = new TFile("FastSim_T5bbbbZG_1600_1550.root");
  // f[1] = new TFile("FastSim_T5bbbbZG_1800_150.root");
  // f[2] = new TFile("FastSim_T5bbbbZG_1800_1750.root");
  f[1] = new TFile("FastSim_TChiWG_0_800.root");
  f[2] = new TFile("FastSim_TChiNG_0_800.root");
  
  TFile *fout;
  vector<string> name1;
  vector<int> rebin;
  name1.push_back("AllSBins_v7");   rebin.push_back(1);

  vector<TString> histName={"AllSBins_v7_ZGPred","AllSBins_v7_LElePred","AllSBins_v7_LMuPred","AllSBins_v7_FRPred","AllSBins_v7_MultiJPred","AllSBins_v7_Obs","AllSBins_v7_CD","AllSBins_v7_CD"};
  TH1D *h_hist[nHists],*h_histOrg[nHists];
  THStack *hs_BG = new THStack("predVsData","predVsData");
  //  TLegend *legend=new TLegend(0.15, 0.88,  0.86, 0.70);
  //TLegend *legend=new TLegend(0.15, 0.88,  0.5, 0.6);
  TLegend *legend=new TLegend(0.15, 0.88,  0.78, 0.72);
  TLegend *legendSig=new TLegend(0.3, 0.73,  0.84, 0.53);
  for(int i=0;i<nHists;i++){
    //    cout<<i<<" "<<i-nBGs<<" "<<histName[i]<<endl;
    name = histName[i]+to_string(i);
    h_hist[i] = new TH1D(name,name,25,0.5,25.5);
    if(i<nBGs+1){
      // h_hist[i] = (TH1D*)f[0]->Get(histName[i]);
      h_histOrg[i] = (TH1D*)f[0]->Get(histName[i]);
      setSBinHist(h_histOrg[i],h_hist[i]);
      h_hist[i]->SetLineColor(kBlack);
    }
    else{
      //      h_hist[i] = (TH1D*)f[i-nBGs]->Get(histName[i]);
      h_histOrg[i] = (TH1D*)f[i-nBGs]->Get(histName[i]);
      setSBinHist(h_histOrg[i],h_hist[i]);
      h_hist[i]->SetLineColor(col[i]);
    }
    //    else h_hist[i] = (TH1D*)f[0]->Get(histName[i]);
    if(i<nBGs) {
      h_hist[i]->SetFillColor(col[i]);
      h_hist[i]->SetLineWidth(1);
      hs_BG->Add(h_hist[i]);
      hs_BG->SetMinimum(0.5);
      hs_BG->SetMaximum(100000);
      hs_BG->SetTitle(";Bin Number;Events");
      legend->AddEntry(h_hist[i],getLegName(histName[i]),"f");
    }
    else{
      h_hist[i]->SetLineWidth(3);
      if(i==nBGs) legend->AddEntry(h_hist[i],getLegName(histName[i]),"ep");
      else{
	if(i==nBGs+1) legendSig->AddEntry(h_hist[i],getLegName(f[1]->GetName()),"lp");
	if(i==nBGs+2) legendSig->AddEntry(h_hist[i],getLegName(f[2]->GetName()),"lp");
      }
    }
    // cout<<h_hist[i]->GetName()<<endl;
  }
  legend->SetNColumns(4);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  // legend->SetTextFont(62);
  legendSig->SetMargin(0.1);
  legendSig->SetEntrySeparation(0.001);
  legendSig->SetBorderSize(0);
  //  legendSig->SetTextFont(62);
  legendSig->SetFillColor(0);
  //  gStyle->SetLegendFont(44);
  //  cout<<"outttttttt";
  TCanvas *cA = new TCanvas("c_predVsDataSBins","c_predVsDataSBins",1500,800);
  TPad *p_top=new TPad("top","top",0,0.4,1,1);
  TPad *p_bot=new TPad("bot","bot",0,0.0,1,0.4);
  p_top->SetBottomMargin(0);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.3);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();  
  p_top->cd();
  hs_BG->Draw("hist");
  hs_BG->GetYaxis()->SetTitleOffset(0.50);    
  hs_BG->GetYaxis()->SetTitleSize(0.09);
  hs_BG->GetYaxis()->SetLabelSize(0.09);
  hs_BG->GetYaxis()->SetTickLength(0.01668);

  for(int i=nBGs;i<nHists;i++){
    if(histName[i].Contains("Obs")){
      h_hist[i]->SetMarkerStyle(20);
      h_hist[i]->SetLineWidth(2);
      //     h_hist[i]->GetXAxis()->SetErrorX(0);
      h_hist[i]->Draw("E1X0 same");
    }
    else{ 
      h_hist[i]->Draw("hist same");
    }
  }
  TH1D *h_totBGorg = (TH1D*)f[0]->Get("AllSBins_v7_TotalBG");
  TH1D *h_totBG = new TH1D("totBG","totBG",25,0.5,25.5);
  setSBinHist(h_totBGorg,h_totBG);
  h_totBG->SetFillStyle(3013);//3013);
  h_totBG->SetFillColor(kGray+3);
  h_totBG->Draw("E2SAME");
  //h_totBG->Draw("e0 histe same");
  
  gPad->RedrawAxis();
  legend->Draw();  
  legendSig->Draw();
  
  TH1D *h_ratioOrg = (TH1D*)f[0]->Get("DataOverBG");
  TH1D *h_ratio = new TH1D("dataByBG","data over BG",25,0.5,25.5);
  setSBinHist(h_ratioOrg,h_ratio);

  h_ratio->SetLineColor(kBlack);
  h_ratio->SetMarkerColor(kBlack);
  h_ratio->SetTitle(0);
  h_ratio->GetXaxis()->SetTitle("Bin Number");
  h_ratio->GetXaxis()->SetTitleOffset(0.87);
  h_ratio->GetXaxis()->SetTitleSize(0.13);
  h_ratio->GetXaxis()->SetLabelSize(0.14);

  //  h_ratio->GetYaxis()->SetTitle("#frac{Data}{SM}");
  h_ratio->GetYaxis()->SetTitle("Obs. / Pred.");
  h_ratio->GetYaxis()->SetTitleOffset(0.33);
  h_ratio->GetYaxis()->SetTitleSize(0.13);
  h_ratio->GetYaxis()->SetLabelSize(0.14);
  h_ratio->GetYaxis()->SetNdivisions(505);
  h_ratio->GetYaxis()->SetTickLength(0.02155);
  h_ratio->SetMarkerStyle(20);
  h_ratio->SetMaximum(1.99);
  h_ratio->SetMinimum(0.01);
  cA->cd();    p_bot->cd();
  p_bot->SetTickx();p_bot->SetTicky();
  //    c_cB->cd(i+1);    p_bot[i]->cd();
  h_ratio->Draw("X0E1");

  TLine *line1V7=new TLine( 5.5,0.01,  5.5,1000);
  TLine *line2V7=new TLine( 9.5,0.01,  9.5,300);
  TLine *line3V7=new TLine(13.5,0.01, 13.5,300);
  TLine *line4V7=new TLine(17.5,0.01, 17.5,300);
  TLine *line5V7=new TLine(21.5,0.01, 21.5,300);
  p_top->cd(); p_top->SetGridx(0);  p_top->SetGridy(0);
  line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
  p_bot->cd(); p_bot->SetGridx(0);
  line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();

  p_top->cd();
  char name2[100];
  textOnTop.SetTextSize(0.07);
  intLumiE.SetTextSize(0.07);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
  //textOnTop.DrawLatexNDC(0.12,0.91,"CMS");
  intLumiE.SetTextSize(0.07);
  sprintf(name2,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.72,0.91,name2);

  TArrow *arrow1 = new TArrow(0.5,280, 5.5,280,0.01,"<|>");
  TArrow *arrow2 = new TArrow(5.5,80, 9.5,80,0.01,"<|>");
  TArrow *arrow3 = new TArrow( 9.5,80,13.5,80,0.01,"<|>");
  TArrow *arrow4 = new TArrow(13.5,80,17.5,80,0.01,"<|>");
  TArrow *arrow5 = new TArrow(17.5,80,21.5,80,0.01,"<|>");
  TArrow *arrow6 = new TArrow(21.5,80,25.5,80,0.01,"<|>");
  arrow1->Draw(); arrow2->Draw(); arrow3->Draw();
  arrow4->Draw(); arrow5->Draw(); arrow6->Draw();
  TLatex Tl;
  Tl.SetTextSize(0.06);
  Tl.DrawLatex(2.5,400,"N^{ 0}_{ 2-4}");
  Tl.DrawLatex(7.0,120,"N^{ 0}_{ 5-6}");
  Tl.DrawLatex(11.0,120,"N^{ 0}_{ #geq7}");
  Tl.DrawLatex(15.0,120,"N^{ #geq1}_{ 2-4}");
  Tl.DrawLatex(19.0,120,"N^{ #geq1}_{ 5-6}");
  Tl.DrawLatex(23.0,120,"N^{ #geq1}_{ #geq7}");

  // TLatex Tl2;
  // Tl2.SetTextSize(0.05);
  // Tl2.SetTextAngle(90);
  // Tl2.DrawLatex(1.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(7.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(12.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(17.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(22.1,1,"#bf{Sideband}");
  // Tl2.DrawLatex(27.1,1,"#bf{Sideband}");
  //  Tl2.DrawLatex(21.6,1,"#bf{Sideband}");

  cA->SaveAs("c_predVsDataSBins.pdf");
}

TString getLegName(TString fname){
  if(fname.Contains("ZGPred")){return "Z(#nu#bar{#nu}) + #gamma";}
  else if(fname.Contains("LEle")){return "Lost e";}
  else if(fname.Contains("LMu")){return "Lost #mu + #tau_{had}";}
  else if(fname.Contains("FR")){return "Misidentified #gamma";}
  else if(fname.Contains("MultiJ") || fname.Contains("gjets")  ){return "#gamma + jets";}
  else if(fname.Contains("T5bbbbZG_1600_150")){return "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1600 GeV, m_{#tilde{#chi}_{1}^{0}} = 150 GeV)";}
  else if(fname.Contains("T5bbbbZG_1600_1550")){return "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1600 GeV, m_{#tilde{#chi}_{1}^{0}} = 1550 GeV)";}
  else if(fname.Contains("T5bbbbZG_1800_150")){return "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 150 GeV)";}
  else if(fname.Contains("T5bbbbZG_1800_1750")){return "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 1750 GeV)";}
  else if(fname.Contains("v7_Obs") ){return "Data";}
  else if(fname.Contains("TChiNG_0_800.root") ){return "TChiNG (m_{#chi_{1}^{0, #pm}} = 800 GeV)";}
  else if(fname.Contains("TChiWG_0_800.root") ){return "TChiWG (m_{#chi_{1}^{0, #pm}} = 800 GeV)";}
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

void setSBinHist(TH1D* ho,TH1D *hn){
  int i_bin=0;
  for(int i=1;i<=ho->GetNbinsX();i++){
    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 ) continue;
    i_bin++;
    hn->SetBinContent(i_bin,ho->GetBinContent(i));
    hn->SetBinError(i_bin,ho->GetBinError(i));
  }
}

/*
  TLatex Tl;
  Tl.SetTextSize(0.08);
  Tl.DrawLatex(0.5,1000,"#leftarrow     N^{0}_{2-4}   #rightarrow");
  Tl.DrawLatex(6.5,1000,"#leftarrow   N^{0}_{5-6} #rightarrow");
  Tl.DrawLatex(11.5,1000,"#leftarrow  N^{0}_{#geq7}  #rightarrow");
  Tl.DrawLatex(16.5,1000,"#leftarrow  N^{#geq1}_{2-4}  #rightarrow");
  Tl.DrawLatex(21.5,1000,"#leftarrow  N^{#geq1}_{5-6}  #rightarrow");
  Tl.DrawLatex(26.5,1000,"#leftarrow  N^{#geq1}_{#geq7}  #rightarrow");
*/
