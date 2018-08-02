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
//int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
int col[11]={kPink-2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};  //Specify Colors
//int col[10]={kBlack,kMagenta+2,kOrange,kYellow,kGreen,kTeal+9,kPink+1,kCyan,kBlue,kRed};  //Specify Colors
TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getLegName(TString);
TString getXaxisName(TString);
void printBinContents(TH1D*);
double data_Integral=0,mc_Integral=0;
TLatex textOnTop,intLumiE;
double intLumi=35.9;
void allBGPredVsData_v2(TString iFname){
  //  gStyle->Reset();
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  //  gStyle->SetOptStat("nemri");
  vector<TString> suffixHist; 
  f[0] = new TFile(iFname);
  f[1] = new TFile("FastSim_T5bbbbZG_1600_150.root");
  f[2] = new TFile("FastSim_T5bbbbZG_1600_1550.root");
  // f[1] = new TFile("FastSim_TChiNG_0_800.root");
  // f[2] = new TFile("FastSim_TChiWG_0_800.root");
  
  TFile *fout;
  vector<string> name1;
  vector<int> rebin;
  name1.push_back("AllSBins_v7");   rebin.push_back(1);

  vector<TString> histName={"AllSBins_v7_ZGPred","AllSBins_v7_LElePred","AllSBins_v7_LMuPred","AllSBins_v7_FRPred","AllSBins_v7_MultiJPred","AllSBins_v7_Obs","AllSBins_v7_CD","AllSBins_v7_CD"};
  TH1D *h_hist[nHists];
  THStack *hs_BG = new THStack("predVsData","predVsData");
  //  TLegend *legend=new TLegend(0.55, 0.88,  0.86, 0.55);
  TLegend *legend=new TLegend(0.2, 0.88,  0.86, 0.70);
  for(int i=0;i<nHists;i++){
    cout<<i<<" "<<i-nBGs<<" "<<histName[i]<<endl;
    if(i<nBGs+1){
      h_hist[i] = (TH1D*)f[0]->Get(histName[i]);
      h_hist[i]->SetLineColor(kBlack);
    }
    else{
      h_hist[i] = (TH1D*)f[i-nBGs]->Get(histName[i]);
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
      if(i==nBGs) legend->AddEntry(h_hist[i],getLegName(histName[i]),"lp");
      else{
	if(i==nBGs+1) legend->AddEntry(h_hist[i],getLegName(f[1]->GetName()),"lp");
	if(i==nBGs+2) legend->AddEntry(h_hist[i],getLegName(f[2]->GetName()),"lp");
      }
    }
    // cout<<h_hist[i]->GetName()<<endl;
  }
  legend->SetNColumns(4);
  legend->SetBorderSize(0);
  //  cout<<"outttttttt";
  TCanvas *cA = new TCanvas("c_predVsData","c_predVsData",1500,800);
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
  TH1D *h_totBG = (TH1D*)f[0]->Get("AllSBins_v7_TotalBG");
  h_totBG->SetFillStyle(3013);
  h_totBG->SetFillColor(1);
  h_totBG->Draw("E2SAME");
  //h_totBG->Draw("e0 histe same");
  
  gPad->RedrawAxis();
  legend->Draw();
  
  TH1D *h_ratio = (TH1D*)f[0]->Get("DataOverBG");
  for(int i=1;i<=h_ratio->GetNbinsX();i++){
    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 )h_ratio->SetBinError(i,0);
    //if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 )h_ratio->SetBinContent(i,0);
  }
  h_ratio->SetLineColor(kBlack);
  h_ratio->SetMarkerColor(kBlack);
  h_ratio->SetTitle(0);
  h_ratio->GetXaxis()->SetTitle("Bin Number");
  h_ratio->GetXaxis()->SetTitleOffset(0.87);
  h_ratio->GetXaxis()->SetTitleSize(0.13);
  h_ratio->GetXaxis()->SetLabelSize(0.14);

  h_ratio->GetYaxis()->SetTitle("#frac{Data}{SM}");
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

  TLine *line1V7=new TLine( 6.5,0.01,  6.5,1300);
  TLine *line2V7=new TLine(11.5,0.01, 11.5,1300);
  TLine *line3V7=new TLine(16.5,0.01, 16.5,1300);
  TLine *line4V7=new TLine(21.5,0.01, 21.5,1300);
  TLine *line5V7=new TLine(26.5,0.01, 26.5,1300);
  p_top->cd(); p_top->SetGridx(0);
  line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
  p_bot->cd(); p_bot->SetGridx(0);
  line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();

  p_top->cd();
  char name2[100];
  textOnTop.SetTextSize(0.07);
  intLumiE.SetTextSize(0.07);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
  intLumiE.SetTextSize(0.07);
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.73,0.91,name2);

  TArrow *arrow1 = new TArrow(0.5,1300,6.5,1300,0.01,"<|>");
  TArrow *arrow2 = new TArrow(6.5,1300,11.5,1300,0.01,"<|>");
  TArrow *arrow3 = new TArrow(11.5,1300,16.5,1300,0.01,"<|>");
  TArrow *arrow4 = new TArrow(16.5,1300,21.5,1300,0.01,"<|>");
  TArrow *arrow5 = new TArrow(21.5,1300,26.5,1300,0.01,"<|>");
  TArrow *arrow6 = new TArrow(26.5,1300,31.5,1300,0.01,"<|>");
  arrow1->Draw(); arrow2->Draw(); arrow3->Draw();
  arrow4->Draw(); arrow5->Draw(); arrow6->Draw();
  TLatex Tl;
  Tl.SetTextSize(0.08);
  Tl.DrawLatex(3.,2000,"N^{0}_{2-4}");
  Tl.DrawLatex(8.5,2000,"N^{0}_{5-6}");
  Tl.DrawLatex(13.5,2000,"N^{0}_{#geq7}");
  Tl.DrawLatex(18.5,2000,"N^{#geq1}_{2-4}");
  Tl.DrawLatex(23.5,2000,"N^{#geq1}_{5-6}");
  Tl.DrawLatex(28.5,2000,"N^{#geq1}_{#geq7}");

  TLatex Tl2;
  Tl2.SetTextSize(0.05);
  Tl2.SetTextAngle(90);
  Tl2.DrawLatex(1.1,1,"#bf{Sideband}");
  Tl2.DrawLatex(7.1,1,"#bf{Sideband}");
  Tl2.DrawLatex(12.1,1,"#bf{Sideband}");
  Tl2.DrawLatex(17.1,1,"#bf{Sideband}");
  Tl2.DrawLatex(22.1,1,"#bf{Sideband}");
  Tl2.DrawLatex(27.1,1,"#bf{Sideband}");
  //  Tl2.DrawLatex(21.6,1,"#bf{Sideband}");

  cA->SaveAs("c_predVsData.pdf");
}

TString getLegName(TString fname){
  if(fname.Contains("ZGPred")){return "Z(#nu #bar{#nu})#gamma";}
  else if(fname.Contains("LEle")){return "Lost e";}
  else if(fname.Contains("LMu")){return "Lost #mu+ #tau_{had}";}
  else if(fname.Contains("FR")){return "e #rightarrow #gamma";}
  else if(fname.Contains("MultiJ") || fname.Contains("gjets")  ){return "#gamma+jets";}
  else if(fname.Contains("T5bbbbZG_1600_150")){return "T5bbbbZG_150";}
  else if(fname.Contains("T5bbbbZG_1600_1550")){return "T5bbbbZG_1550";}
  else if(fname.Contains("TChiNG_0_800")){return "TChiNG_800";}
  else if(fname.Contains("TChiWG_0_800")){return "TChiWG_800";}
  else if(fname.Contains("v7_Obs") ){return "Data";}
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
