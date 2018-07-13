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

TLatex textOnTop,intLumiE;
double intLumi=35.9;
const int nfiles=3,nBGs=5,nHists=8,nSig=nHists-nBGs-1;
TFile *f[nfiles];
TFile *fout;
TH1D *h_hist[nHists];
//int col[11]={kPink-2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};  //Specify Colors   
int col[11]={kOrange,kGray,kRed,kTeal+9,kCyan,kYellow,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};
TLegend *legend=new TLegend(0.6, 0.88,  0.86, 0.65);
TString name;
TString getXaxisName(TString);
TString getLegName(TString);
double getCombBinContent(TH1D*,vector<int>);
double getCombBinError(TH1D*,vector<int>);
void myPrint(TH1D*);
void getMETincl();
void getMET0b();
void getMET1b();
void getNJ();
void getbtag();
void drawPlots(TH1D* h[],TString);
void combineBins(){
  gStyle->SetOptStat(0);
  f[0] = new TFile("SBinHists.root");
  f[1] = new TFile("FastSim_T5bbbbZG_1600_150.root");
  f[2] = new TFile("FastSim_T5bbbbZG_1600_1550.root");
  fout = new TFile("binsCombined.root","recreate");
  vector<string> name1;
  name1.push_back("AllSBins_v7");

  vector<TString> histName={"AllSBins_v7_ZGPred","AllSBins_v7_LElePred","AllSBins_v7_LMuPred","AllSBins_v7_FRPred","AllSBins_v7_MultiJPred","AllSBins_v7_Obs","AllSBins_v7_CD","AllSBins_v7_CD"};
  TH1D *h_totBG = (TH1D*)f[0]->Get("AllSBins_v7_TotalBG");
  h_totBG->SetFillStyle(3013);
  h_totBG->SetFillColor(1);
  // h_totBG->Draw("E2SAME");

  THStack *hs_BG = new THStack("predVsData","predVsData");
 
  for(int i=0;i<nHists;i++){
    if(i<nBGs+1){
      h_hist[i] = (TH1D*)f[0]->Get(histName[i]);
      h_hist[i]->SetLineColor(kBlack);
    }
    else{
      h_hist[i] = (TH1D*)f[i-nBGs]->Get(histName[i]);
      h_hist[i]->SetLineColor(col[i]);
    }
    if(i<nBGs) {
      h_hist[i]->SetFillColor(col[i]);
      h_hist[i]->SetLineWidth(1);
      // hs_BG->Add(h_hist[i]);
      // hs_BG->SetMinimum(0.5);
      // hs_BG->SetMaximum(100000);
      // hs_BG->SetTitle(";Bin Number;Events");
      legend->AddEntry(h_hist[i],getLegName(histName[i]),"f");
    }
    else{
      h_hist[i]->SetLineWidth(2);
      if(i==nBGs){
	h_hist[i]->SetMarkerStyle(20);
	//	h_hist[i]->Draw("X0E1");
	legend->AddEntry(h_hist[i],getLegName(histName[i]),"ep");
      }
      else{
	if(i==nBGs+1) legend->AddEntry(h_hist[i],getLegName(f[1]->GetName()),"lp");
	if(i==nBGs+2) legend->AddEntry(h_hist[i],getLegName(f[2]->GetName()),"lp");
      }
    }
  }
  getMETincl();
  getMET0b();
  getMET1b();
  getNJ();
  getbtag();
}

//////////////////////////////////////////////////////
///////////////////// MET Inclusive //////////////////
//////////////////////////////////////////////////////
void getMETincl(){
  TH1D *h_met[nHists];
  vector<double> metBins={200,270,350,450,600};
  vector<int> bins;
  for(int i=0;i<nHists;i++){
    name = "met_"+to_string(i);
    h_met[i] = new TH1D(name,name,metBins.size()-1,&(metBins[0]));

    bins={2,8,13,18,23,28};
    h_met[i]->SetBinContent(1,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(1,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={3,9,14,19,24,29};
    h_met[i]->SetBinContent(2,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(2,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={4,10,15,20,25,30};
    h_met[i]->SetBinContent(3,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(3,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={5,6,11,16,21,26,31};
    h_met[i]->SetBinContent(4,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(4,getCombBinError(h_hist[i],bins));
  }
  drawPlots(h_met,"MET_Incl");
}

//////////////////////////////////////////////////////
///////////////////// MET 0b //////////////////
//////////////////////////////////////////////////////
void getMET0b(){
  TH1D *h_met[nHists];
  vector<double> metBins={200,270,350,450,600};
  vector<int> bins;
  for(int i=0;i<nHists;i++){
    name = "met0b_"+to_string(i);
    h_met[i] = new TH1D(name,name,metBins.size()-1,&(metBins[0]));

    bins.resize(0);
    bins={2,8,13};
    h_met[i]->SetBinContent(1,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(1,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={3,9,14};
    h_met[i]->SetBinContent(2,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(2,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={4,10,15};
    h_met[i]->SetBinContent(3,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(3,getCombBinError(h_hist[i],bins));
    // cout<<h_hist[i]->GetName()<<" "<<h_met[i]->GetBinContent(3)<<endl;
    // cout<<"sum:"<<h_hist[i]->GetBinContent(4)+h_hist[i]->GetBinContent(10)+h_hist[i]->GetBinContent(15)<<endl;

    bins.resize(0);
    bins={5,6,11,16};
    h_met[i]->SetBinContent(4,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(4,getCombBinError(h_hist[i],bins));
  }
  drawPlots(h_met,"MET_0b");
}
//////////////////////////////////////////////////////
///////////////////// MET >=1b //////////////////
//////////////////////////////////////////////////////
void getMET1b(){
  TH1D *h_met[nHists];
  vector<double> metBins={200,270,350,450,600};
  vector<int> bins;
  for(int i=0;i<nHists;i++){
    name = "met1b_"+to_string(i);
    h_met[i] = new TH1D(name,name,metBins.size()-1,&(metBins[0]));

    bins={18,23,28};
    h_met[i]->SetBinContent(1,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(1,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={19,24,29};
    h_met[i]->SetBinContent(2,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(2,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={20,25,30};
    h_met[i]->SetBinContent(3,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(3,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={21,26,31};
    h_met[i]->SetBinContent(4,getCombBinContent(h_hist[i],bins));
    h_met[i]->SetBinError(4,getCombBinError(h_hist[i],bins));
  }
  drawPlots(h_met,"MET_1b");
}

//////////////////////////////////////////////////////
///////////////////// NJets  //////////////////
//////////////////////////////////////////////////////
void getNJ(){
  TH1D *h_nj[nHists];
  vector<double> njBins={1.5,4.5,6.5,10.5};
  //  vector<double> njBins={2,5,7,10};
  vector<int> bins;
  for(int i=0;i<nHists;i++){
    name = "nj_"+to_string(i);
    h_nj[i] = new TH1D(name,name,njBins.size()-1,&(njBins[0]));

    bins={2,3,4,5,6,18,19,20,21};
    h_nj[i]->SetBinContent(1,getCombBinContent(h_hist[i],bins));
    h_nj[i]->SetBinError(1,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={8,9,10,11,23,24,25,26};
    h_nj[i]->SetBinContent(2,getCombBinContent(h_hist[i],bins));
    h_nj[i]->SetBinError(2,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={13,14,15,16,28,29,30,31};
    h_nj[i]->SetBinContent(3,getCombBinContent(h_hist[i],bins));
    h_nj[i]->SetBinError(3,getCombBinError(h_hist[i],bins));
  }
  drawPlots(h_nj,"NJ");
}
//////////////////////////////////////////////////////
///////////////////// btags  //////////////////
//////////////////////////////////////////////////////
void getbtag(){
  TH1D *h_bTag[nHists];
  vector<double> bTagBins={-0.5,0.5,1.5};
  vector<int> bins;
  for(int i=0;i<nHists;i++){
    name = "bTag_"+to_string(i);
    h_bTag[i] = new TH1D(name,name,bTagBins.size()-1,&(bTagBins[0]));

    bins={2,3,4,5,6,8,9,10,11,13,14,15,16};
    h_bTag[i]->SetBinContent(1,getCombBinContent(h_hist[i],bins));
    h_bTag[i]->SetBinError(1,getCombBinError(h_hist[i],bins));

    bins.resize(0);
    bins={18,19,20,21,23,24,25,26,28,29,30,31};
    h_bTag[i]->SetBinContent(2,getCombBinContent(h_hist[i],bins));
    h_bTag[i]->SetBinError(2,getCombBinError(h_hist[i],bins));
  }
  drawPlots(h_bTag,"bTags");
}

void drawPlots(TH1D* h1[],TString varName){
  TCanvas *cA = new TCanvas("c_"+varName,"c_"+varName,1100,900);
  TPad *p_top=new TPad("top","top",0,0.4,1,1);
  TPad *p_bot=new TPad("bot","bot",0,0.0,1,0.4);
  p_top->SetBottomMargin(0);
  p_top->SetLeftMargin(0.15);
  p_top->Draw();p_top->SetGridx(0);p_top->SetGridy(0);p_top->SetLogy();
  p_bot->SetTopMargin(0);
  p_bot->SetLeftMargin(0.15);
  p_bot->SetBottomMargin(0.3);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();
  p_top->cd();

  THStack *hs = new THStack(varName,varName);
  TH1D *h1Tot;
  for(int i=0;i<nHists;i++){
    if(i<nBGs){
      h1[i]->SetFillColor(h_hist[i]->GetFillColor());
      hs->Add(h1[i]);
      if(i==0) h1Tot = (TH1D*)h1[i]->Clone("sum_"+varName);
      else h1Tot->Add(h1[i]);
    }
    h1[i]->SetLineWidth(h_hist[i]->GetLineWidth());
    h1[i]->SetLineColor(h_hist[i]->GetLineColor());
    //    cout<<"*************** "<<h_hist[i]->GetName()<<" *****************"<<endl;
    //    myPrint(h1[i]);
  }
  hs->SetMinimum(8);
  hs->SetMaximum(1000);
  hs->Draw("hist");
  hs->SetTitle(";MET;Events");
  hs->GetYaxis()->SetTitleOffset(0.65);
  hs->GetYaxis()->SetTitleSize(0.075);
  hs->GetYaxis()->SetLabelSize(0.075);
  for(int i=nBGs;i<nHists;i++){
    //    if(histName[i].Contains("Obs")){
    if(i==nBGs){
      h1[i]->SetMarkerStyle(20);
      h1[i]->SetLineWidth(2);
      h1[i]->Draw("E1X0 same");
    }
    else{
      h1[i]->Draw("hist same");
    }
  }
  h1Tot->SetFillStyle(3013);
  h1Tot->SetFillColor(1);
  h1Tot->Draw("E2SAME");
  legend->Draw();
  gPad->RedrawAxis();
  //------------------ratio plot-----------------------
  TH1D *h_ratio = (TH1D*)h1[nBGs]->Clone("DataOverBG_"+varName);
  h_ratio->Divide(h1Tot);

  h_ratio->SetLineColor(kBlack);
  h_ratio->SetMarkerColor(kBlack);
  h_ratio->SetTitle(0);
  h_ratio->GetXaxis()->SetTitle(getXaxisName(varName));
  h_ratio->GetXaxis()->SetTitleOffset(0.95);
  h_ratio->GetXaxis()->SetTitleSize(0.11);
  h_ratio->GetXaxis()->SetLabelSize(0.11);

  //h_ratio->GetYaxis()->SetTitle("#frac{Data}{SM}");
  h_ratio->GetYaxis()->SetTitle("Obs. / Pred.");
  h_ratio->GetYaxis()->SetTitleOffset(0.5);
  h_ratio->GetYaxis()->SetTitleSize(0.11);
  h_ratio->GetYaxis()->SetLabelSize(0.11);
  h_ratio->GetYaxis()->SetNdivisions(505);
  h_ratio->SetMarkerStyle(20);
  h_ratio->SetMaximum(1.99);
  h_ratio->SetMinimum(0.01);
  cA->cd();    p_bot->cd();
  p_bot->SetTickx();p_bot->SetTicky();
  h_ratio->Draw("X0E1");
  p_top->cd();
  char name2[100];
  textOnTop.SetTextSize(0.06);
  intLumiE.SetTextSize(0.06);
  textOnTop.DrawLatexNDC(0.15,0.91,"CMS #it{#bf{Preliminary, Additional}}");
  sprintf(name2,"#bf{%0.1f fb^{-1} (13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.68,0.91,name2);
}
double getCombBinContent(TH1D* h1,vector<int> bin){
  double sum=0.0;
   for(int i=0;i<bin.size();i++){
    sum = sum+h1->GetBinContent(bin[i]);
  }
   //cout<<h1->GetName()<<" Sum: "<<sum<<endl;
  return sum;
}
double getCombBinError(TH1D* h1,vector<int> bin){
  double err=0.;
  for(int i=0;i<bin.size();i++){
    err = err+((h1->GetBinError(bin[i]))*(h1->GetBinError(bin[i])));
  }
  //  cout<<h1->GetName()<<" Err: "<<sqrt(err)<<endl;
  return sqrt(err);
}



TString getLegName(TString fname){
  if(fname.Contains("ZGPred")){return "Z(#nu#bar{#nu})#gamma";}
  else if(fname.Contains("LEle")){return "Lost e";}
  else if(fname.Contains("LMu")){return "Lost #mu + #tau_{had}";}
  else if(fname.Contains("FR")){return "Misidentified #gamma";}
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
  else if(axname.Contains("MET")) return "p_{T}^{miss} (GeV)";
  else return axname;

}

void myPrint(TH1D *h){
  cout<<"================ "<<h->GetName()<<" ==================="<<endl;
  for(int i=1;i<=h->GetNbinsX();i++){
    //cout<<i<<" "<<h->GetBinCenter(i)<<" "<<h->GetBinContent(i)<<" "<<h->GetBinError(i)<<endl;
    cout<<h->GetBinContent(i)<<" "<<h->GetBinError(i)<<endl;
  }
}
