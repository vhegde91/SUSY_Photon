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

TString name;
TLatex textOnTop,intLumiE;
int col[5]={kCyan,kMagenta+2,kYellow+2,kRed,kMagenta};  //Specify Colors 
//int col[10]={kOrange,kBlue,kTeal+9,kOrange-9,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta};  //Specify Colors 
void setLastBinAsOverFlow(TH1D*);
void YieldComp(){
  double sr_Integral=0,cr_Integral=0;
  TPaveStats *st;
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat("ni");
 
  bool forCR=1;
  int nfiles=3;
  TFile *f[3];
  f[0] = new TFile("CS_TTW_LostEle.root");
  f[1] = new TFile("CS_TTW_LostEle_ST700.root");
  // f[2] = new TFile("CS_TTW_LostMu_MET250.root");
  f[2] = new TFile("CS_TTW_LostEle_Photon180.root");
  TH1D *h_CR[3],*h_SR[3];
  TString name1;
  for(int i=0;i<nfiles;i++){
    name1=f[i]->GetName();
    //    name1=f[i]->GetName()+"_CS";
    name1=name1+"_CR";
    h_CR[i]=new TH1D(name1,name1,21,0.5,21.5);
    name1=name1+"_SR";
    h_SR[i]=new TH1D(name1,name1,21,0.5,21.5);
  }

  TH1D *h_intLumi=(TH1D*)f[0]->FindObjectAny("intLumi");
  double intLumi=0;
  if(h_intLumi) intLumi=h_intLumi->GetMean();delete h_intLumi;
  
  TCanvas *c_cA=new TCanvas("allSearchBins_LostMuClsr","all search bins closure for lost muon",2000,900);
  TH1D *h_histG,*h_histE,*h_histGcopy,*h_temp;
  
  for(int p=0;p<nfiles;p++){
    for(int i=1;i<=3;i++){
      
      name="MET_Ele0_R"+to_string(i);
      h_histG=(TH1D*)f[p]->FindObjectAny(name);

      name="MET_Ele1_R"+to_string(i);
      h_histE=(TH1D*)f[p]->FindObjectAny(name);
      
      setLastBinAsOverFlow(h_histG);
      setLastBinAsOverFlow(h_histE);
      sr_Integral+=h_histG->Integral();
      cr_Integral+=h_histE->Integral();
      
      for(int j=6;j<=h_histG->GetNbinsX();j++){
	//      cout<<name<<" i,j: "<<i<<","<<j<<" low "<<h_histG->GetBinLowEdge(j)<<" cont "<<h_histG->GetBinContent(j)<<" err "<<h_histG->GetBinError(j)<<" fill # "<<((i-1)*7)+j-5<<endl;
	h_SR[p]->SetBinContent( ((i-1)*7)+j-5,h_histG->GetBinContent(j) );
	h_SR[p]->SetBinError  ( ((i-1)*7)+j-5,h_histG->GetBinError(j) );
	h_CR[p]->SetBinContent( ((i-1)*7)+j-5,h_histE->GetBinContent(j) );
	h_CR[p]->SetBinError  ( ((i-1)*7)+j-5,h_histE->GetBinError(j) );
     
	h_SR[p]->SetLineColor(col[p]);
	h_SR[p]->SetLineWidth(2);
	h_SR[p]->SetMarkerStyle(21);
	h_SR[p]->SetMarkerColor(h_SR[p]->GetLineColor());
	h_SR[p]->SetFillColor(h_SR[p]->GetLineColor());
	h_SR[p]->SetFillStyle(3001+p);
      	
	h_CR[p]->SetLineColor(col[p]);
	h_CR[p]->SetLineWidth(2);
	h_CR[p]->SetMarkerStyle(20);
	h_CR[p]->SetMarkerColor(h_CR[p]->GetLineColor());
	//h_CR[p]->GetYaxis()->SetLabelSize(0.09);
	h_CR[p]->SetTitle(";;Events");
	h_CR[p]->SetMinimum(0.1);
	//      h_CR[p]->GetYaxis()->SetTitle("Events");
	//	h_CR[p]->GetYaxis()->SetTitleSize(0.07);
	h_CR[p]->GetYaxis()->SetTitleOffset(0.51);
      }
    }
    if(forCR){
      if(p==0) h_CR[p]->Draw();
      else{
	h_CR[p]->Draw("sames");
	//      h_SR[p]->Draw("BAR sames");
      }
      gPad->Update();
      st=(TPaveStats*)h_CR[p]->FindObject("stats");
      st->SetTextColor(h_CR[p]->GetLineColor());
    }
    else{
      if(p==0) h_SR[p]->Draw();
      else{
	h_SR[p]->Draw("sames");
	//      h_SR[p]->Draw("BAR sames");
      }
      gPad->Update();
      st=(TPaveStats*)h_SR[p]->FindObject("stats");
      st->SetTextColor(h_SR[p]->GetLineColor());
    }
  }
  
  // for(int i=1;i<=h_CR[i]->GetNbinsX();i++){
  //    cout<<" "<<h_CR[i]->GetBinContent(i)<<"\t"<<h_CR[i]->GetBinError(i)<<endl;
  // }
  
  //  c_cA->cd();p_top->cd();
 

  TLegend *legend=new TLegend(0.65, 0.90,  0.90, 0.75);
   
}



void setLastBinAsOverFlow(TH1D* h_hist){
  double lastBinCt =h_hist->GetBinContent(h_hist->GetNbinsX()),overflCt =h_hist->GetBinContent(h_hist->GetNbinsX()+1);
  double lastBinErr=h_hist->GetBinError(h_hist->GetNbinsX()),  overflErr=h_hist->GetBinError(h_hist->GetNbinsX()+1);
  
  if(lastBinCt!=0 && overflCt!=0){
    lastBinErr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );
    lastBinCt = lastBinCt+overflCt;
    h_hist->SetBinContent(h_hist->GetNbinsX(),lastBinCt);
    h_hist->SetBinError(h_hist->GetNbinsX(),lastBinErr);
  }
  
}
