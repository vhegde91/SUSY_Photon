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

char name[100];
char name2[100];
TString name3;
TLatex textOnTop,intLumiE;
const int nfiles=6,nBG=5;    //Specify no. of files
TFile *f[nfiles];
int col[11]={kMagenta+2,kTeal+9,kGreen,kOrange,kCyan,kBlack,kYellow,kPink+1,kBlue,kRed,kMagenta};  //Specify Colors
//int col[11]={kTeal+9,kGreen,kYellow,kOrange,kPink+1,kMagenta+2,kCyan,kBlue,kRed,kMagenta,kBlue+2};  //Specify Colors b's
//char name[100],name2[100];
TCanvas *c_cA=new TCanvas("MET_incl","MET for NJ>=2",1500,850);
//TCanvas *c_cB=new TCanvas("MET_SBins","MET for all search bins",1500,850);

void decorate(TH1D*,int,const char*);
void decorate(THStack*,int,const char*);
void drawlegend(TH1D*,int,const char*);
void printInt(TH1D*,int,const char*);
TLegend *legend1=new TLegend(0.50, 0.9,  0.7, 0.65);
TLegend *legend2=new TLegend(0.7, 0.9,  0.90, 0.65);
//TLegend *legend2=new TLegend(0.6, 0.90,  0.98, 0.45);
void setLastBinAsOverFlow(TH1D*);
void searchBinsStack(){
  double sr_Integral=0,cr_Integral=0;
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);

  f[0] = new TFile("CS_LDP_ZGZJToNuNuG.root");
  f[1] = new TFile("DCS_LDP_Run2016_LostEle_v2.root");
  f[2] = new TFile("DCS_LDP_Run2016_LostMuHadTau_v2.root");
  f[3] = new TFile("DCS_LDP_Run2016_SingleEle_v2.root");
  f[4] = new TFile("gjets_qcd.root");
  f[5] = new TFile("DCS_LDP_Run2016_Multijet_v2.root");

  // f[0] = new TFile("CS_ZGZJToNuNuG.root");
  // f[1] = new TFile("DCS_Run2016_LostEle_v2.root");
  // f[2] = new TFile("DCS_Run2016_LostMuHadTau_v2.root");
  // f[3] = new TFile("DCS_Run2016_SingleEle_v2.root");
  // f[4] = new TFile("gjets_qcd.root");
  // f[5] = new TFile("DCS_LDP_Run2016_Multijet_v2.root");

  //vector<TString> histName = {"AllSBins_v4","AllSBins_v4_Ele0","AllSBins_v4_Mu0","AllSBins_v4_Pho","AllSBins_v4_AB","AllSBins_v4_AB"};
  //  vector<TString> histName = {"ST","ST_Ele0","ST_Mu0","ST_Pho","ST_AB","ST_AB"};
  vector<TString> histName = {"BestPhotonPt","BestPhotonPt_Ele0","BestPhotonPt_Mu0","BestPhotonPt","BestPhotonPt_AB","BestPhotonPt_AB"};
  //vector<TString> histName = {"AllSBins_v4","AllSBins_v4_Ele0","AllSBins_v4_Mu0","AllSBins_v4_Pho","AllSBins_v4_CD","AllSBins_v4_CD"};
  int rebin = 1;

  gStyle->SetTextSize(2);
  THStack *hs_MET=new THStack("MET_Stack","MET Stacked");
  THStack *hs_MET_SB=new THStack("MET_SB","MET for all search bins");
  //TH1D *h_R;
  TH1D *h_MET_R[nfiles];
  for(int i=0;i<nfiles;i++){
    sprintf(name,"hist_file%i",i);
    h_MET_R[i]=new TH1D(name,name,21,0.5,21.5);
  }
  vector<double> Bcnt;
  double intLumi=0.0;
  TLatex tl1;
  for(int i=0;i<nfiles;i++){
    TString fName = f[i]->GetName();
    TH1D *h_intLumi=(TH1D*)f[i]->FindObjectAny("intLumi");
    if(i==0) {
      intLumi=h_intLumi->GetMean();
      sprintf(name2, "%.2f fb^{-1}",intLumi);
    }
    else{
      if(abs(intLumi-h_intLumi->GetMean())>0.0001)
	cout<<"Integarted lumi for "<<f[i]->GetName()<<" is "<<h_intLumi->GetMean()<<" and for other files it is different"<<endl;
    }
    
    TH1D *h_MET;
    // if(fName=="DCS_Run2016_LostEle_v2.root") h_MET=(TH1D*)f[i]->FindObjectAny("AllSBins_Ele0");//MET_R1
    // else if(fName=="DCS_Run2016_LostMuHadTau_v2.root") h_MET=(TH1D*)f[i]->FindObjectAny("AllSBins_Mu0");//MET_R1
    // else{ cout<<"Don't know which hist to get from "<<f[i]->GetName()<<endl;break;}
    h_MET=(TH1D*)f[i]->FindObjectAny(histName[i]);
    h_MET->Rebin(rebin);
    decorate(h_MET,i,f[i]->GetName());
    
    if(i<=(nBG-1))  hs_MET->Add(h_MET);
    if(i==nBG-1) {
      c_cA->cd(); 
      //      hs_MET->Draw("BAR");
      hs_MET->Draw("HIST");
      hs_MET->Draw("e0 sames");
      decorate(hs_MET,i,f[i]->GetName()); 
    }
    if(i>=nBG){ 
      c_cA->cd(); 
      h_MET->Draw("e0 sames");
    }
    drawlegend(h_MET,i,f[i]->GetName());
    if(i==nfiles-1) hs_MET->SetTitle(";Bin No.;Events");

  }
  c_cA->cd(); gPad->SetLogy();legend1->Draw();//legend2->Draw();
  gPad->RedrawAxis();

  textOnTop.SetTextSize(0.05);
  intLumiE.SetTextSize(0.05);
  textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Preliminary}}");
  intLumiE.SetTextSize(0.05);
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.73,0.91,name2);
  c_cA->SaveAs("met_inclusive.png");
  //----------------------all search bins---------------------------- 
  cout<<"*****************************************************"<<endl;
  cout<<"Int Lumi(inv.fb) for file1:"<<setprecision(4)<<intLumi<<endl;
    
}

void decorate(THStack *hs,int i,const char* fname){
  hs->SetMinimum(0.1);
  hs->SetMaximum(100000);
  hs->SetTitle(0);
  hs->GetXaxis()->SetLabelSize(.04);
  hs->GetYaxis()->SetLabelSize(.05);
  hs->GetXaxis()->SetTitleSize(0.05);
  hs->GetYaxis()->SetTitleSize(0.05);
  //  drawlegend(hist,i,fname);
  gPad->Update();
  gStyle->SetOptStat(0);
}
void decorate(TH1D* hist,int i,const char* fname){
  hist->SetLineColor(col[i]);
  if(i<nBG) {
    hist->SetFillColor(col[i]);
    hist->SetLineColor(kBlack);
    hist->SetLineWidth(1);
  }
  else{
    hist->SetLineWidth(2);
    hist->SetMarkerStyle(20);
  }
  hist->SetTitle(0);
  hist->GetXaxis()->SetLabelSize(.04);
  hist->GetYaxis()->SetLabelSize(.05);
  //hist->SetXLabelSize(0.05);
  hist->GetXaxis()->SetTitleSize(0.05);
  // drawlegend(hist,i,fname);
  gPad->Update();
  setLastBinAsOverFlow(hist);
  gStyle->SetOptStat(0);
  //Hlist.Add(hist);
  /*
  TPaveStats *st=(TPaveStats*)hist->FindObject("stats");
  st->SetLineColor(col[i]);
  st->SetTextColor(col[i]);
  if(i>5){
    st->SetX1NDC(0.55);
    st->SetX2NDC(0.75); 
    st->SetY1NDC(0.90-(i-6)*0.1);
    st->SetY2NDC(0.80-(i-6)*0.1);
  }
  else{
    st->SetX1NDC(0.75);
    st->SetX2NDC(0.98); 
    // st->SetY1NDC(0.90-i*0.2);
    //st->SetY2NDC(0.70-i*0.2);
    st->SetY1NDC(0.90-i*0.1);
    st->SetY2NDC(0.80-i*0.1);
  }
  */
}

void drawlegend(TH1D *hist,int i,const char* fname){
  gStyle->SetLegendBorderSize(0);
  //TLegend *legend=new TLegend(0.65,0.90-i*0.05,0.98,0.85-i*0.05);
  // TLegend *legend=new TLegend(0.65,0.90-i*0.2,0.98,0.86-i*0.2);
  //TLegend *legend;
  /*//legend with stat box
  if(i>5){
    legend=new TLegend(0.55,0.9-(i-6)*0.1,0.95,0.85-(i-6)*0.1);//for legend with stat box
  }
  else legend=new TLegend(0.55, 0.90-i*0.05,  0.98, 0.85-i*0.05);
  */
  //legend=new TLegend(0.55, 0.90-i*0.05,  0.98, 0.85-i*0.05);
  
  TString lName=fname;
  
  if(lName.Contains("ToNuNu")){lName="Z( #rightarrow #nu #bar{#nu})#gamma+jets";}
  else if(lName.Contains("LostEle")){lName="Lost e";}
  else if(lName.Contains("LostMuHadTau")){lName="Lost #mu+had #tau";}
  else if(lName.Contains("SingleEle")){lName="e #rightarrow #gamma";}
  // else if(lName.Contains("TTJetsHT")){lName="t #bar{t}";}
  // else if(lName.Contains("QCD")){lName="QCD";}
  else if(lName.Contains("GJets") || lName.Contains("gjets")  ){lName="#gamma +jets";}
  else if(lName.Contains("Run2016_Multijet") ){lName="Data";}
  // else if(lName.Contains("WGJetsToLNuG")){lName="W(#rightarrow l #nu)+ #gamma";}
  // else if(lName.Contains("ZGJetsToNuNuG")){lName="Z(#rightarrow #nu #bar{#nu})+ #gamma";}
  // else if(lName.Contains("TTGJets")){lName="t #bar{t}+ #gamma";}
  else if(lName.Contains("T1bbbb_ZG_mGl1600_NLSP150")){lName="T5bbbb_ZG_150";}
  else if(lName.Contains("T1bbbb_ZG_mGl1600_NLSP1000")){lName="T5bbbb_ZG_1000";}
  else if(lName.Contains("T1bbbb_ZG_mGl1600_NLSP1550")){lName="T5bbbb_ZG_1550";}

  //  const char *l_name=lName.c_str();
  if(i<6){
    if(i<nBG)legend1->AddEntry(hist,lName,"f");
    else legend1->AddEntry(hist,lName,"lp");
    legend1->SetTextSize(0.04);
  }
  else{
    if(i<nBG)legend2->AddEntry(hist,lName,"f");
    else legend2->AddEntry(hist,lName,"lp");
    legend2->SetTextSize(0.04);
  }
  // legend->SetTextColor(col[i]);
  //legend->SetLineColor(col[i]);
  //if(i==nfiles-1) legend->Draw();
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
