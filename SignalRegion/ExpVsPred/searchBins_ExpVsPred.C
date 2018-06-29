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
#include"TPad.h"

char name[100];
char name2[100];
TString name3;
TLatex textOnTop,intLumiE;
const int nfiles=4,nBG=3,nSig=1;    //Specify no. of files
TFile *f[nfiles];
//int col[11]={kCyan+2,kGreen+2,kBlue,kRed,kGray+1,kCyan,kMagenta+2,kPink+1,kRed,kMagenta,kBlue+2};  //Specify Colors
int col[5]={kCyan+2,kOrange-9,kBlue,kRed,kGray+1};



void decorate(TH1D*,int,const char*);
void decorate(THStack*,int,const char*);
void drawlegend(TH1D*,int,const char*);
void printInt(TH1D*,int,const char*);
TLegend *legend=new TLegend(0.7, 0.90,  0.9, 0.7);
// TLegend *legend1=new TLegend(0.50, 0.9,  0.7, 0.70);
// TLegend *legend2=new TLegend(0.7, 0.9,  0.90, 0.65);
void setLastBinAsOverFlow(TH1D*);

void searchBins_ExpVsPred(){
  double sr_Integral=0,cr_Integral=0;
  double intLumi=36.814;
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  TCanvas *c_cA=new TCanvas("MET_SBins_ExpVsPred","MET for all search bins. Comparing exp vs CS pred",1500,850);
  TPad *p_top=new TPad("top","top",0,0.35,1,1);
  TPad *p_bot=new TPad("bot","bot",0,0.04,1,0.35);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  p_top->SetBottomMargin(0);
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.2);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();

  TString histName[nfiles];

  f[0] = new TFile("LostEle_SB.root");  histName[0] = "searchBins_Ele1";
  f[1] = new TFile("LostMuon_SB.root"); histName[1] = "searchBins_Mu1";
  f[2] = new TFile("FakeRate_SB.root"); histName[2] = "searchBins_Ele";
  f[3] = new TFile("LeptonicBG.root");  histName[3] = "searchBins_SR";
  //f[3] = new TFile("AllBG.root");       histName[3] = "allBG_SR";

  TH1D *h_MET_R = new TH1D(histName[nBG],histName[nBG],21,0.5,21.5);
  TH1D *h_sum = new TH1D("sumPred","summing up of all predictions",21,0.5,21.5);
  for(int j=1;j<=3;j++){
    name3="MET_R"+to_string(j);
    TH1D *h_R=(TH1D*)f[nBG]->FindObjectAny(name3);
    setLastBinAsOverFlow(h_R);
    //sr_Integral+=h_R->Integral();
    for(int k=6;k<=h_R->GetNbinsX();k++){
      h_MET_R->SetBinContent( ((j-1)*7)+k-5,h_R->GetBinContent(k) );
      h_MET_R->SetBinError  ( ((j-1)*7)+k-5,h_R->GetBinError(k) );
    }
  }
  

  gStyle->SetTextSize(2);
  //THStack *hs_MET=new THStack("MET_Stack","MET Stacked");
  THStack *hs_MET=new THStack("MET_SB","MET for all search bins. Obtained from BG predictions");
  //TH1D *h_R;
  for(int i=0;i<nfiles;i++){
    if(i<=(nBG-1)){
      TH1D *h_MET=(TH1D*)f[i]->FindObjectAny(histName[i]);
      h_sum->Add(h_MET);
      decorate(h_MET,i,f[i]->GetName());
      hs_MET->Add(h_MET);
      drawlegend(h_MET,i,f[i]->GetName());
    }
    if(i==nBG-1) {
      c_cA->cd();  p_top->cd();
      hs_MET->Draw("BAR");
      decorate(hs_MET,i,f[i]->GetName()); 
    }
    if(i>=nBG){ 
      c_cA->cd();  p_top->cd();
      decorate(h_MET_R,i,f[i]->GetName());
      h_MET_R->Draw("sames");
      drawlegend(h_MET_R,i,f[i]->GetName());
    }

    if(i==nfiles-1) hs_MET->SetTitle(";Bin No.;Events");
  }
  c_cA->cd(); p_top->cd();
  gPad->SetLogy();legend->Draw();
  gPad->RedrawAxis();
  gPad->Update();

  textOnTop.SetTextSize(0.06);
  intLumiE.SetTextSize(0.06);
  textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Simulation}}");
  intLumiE.SetTextSize(0.06);
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.73,0.91,name2);

  TH1D *hs_numr=(TH1D*)h_MET_R->Clone();
  c_cA->cd();    p_bot->cd();
  hs_numr->Draw("e0");
  hs_numr->Divide(h_sum);
  hs_numr->SetLineColor(kBlack);
  hs_numr->SetLineWidth(2);
  hs_numr->SetMarkerStyle(21);
  hs_numr->SetMarkerColor(kBlack);
  hs_numr->SetTitle(";Bin Number;Exp / Pred");
  hs_numr->GetYaxis()->SetTitleOffset(0.18);
  hs_numr->GetYaxis()->SetTitleSize(0.14);
  hs_numr->GetXaxis()->SetTitleSize(0.2);
  hs_numr->GetXaxis()->SetLabelSize(0.15);
  hs_numr->GetXaxis()->SetTitleOffset(0.67);
  hs_numr->GetYaxis()->SetLabelSize(0.13);
  hs_numr->GetYaxis()->SetNdivisions(505);
  hs_numr->SetMaximum(1.5);
  hs_numr->SetMinimum(0.5);
  gPad->Update();

  cout<<"*****************************************************"<<endl;
  cout<<"Int Lumi(inv.fb) for file1:"<<setprecision(4)<<intLumi<<endl;
  c_cA->SaveAs("a.png");  

}

void decorate(THStack *hs,int i,const char* fname){
  hs->SetMinimum(0.1);
  hs->SetTitle(0);
  hs->GetXaxis()->SetLabelSize(.04);
  hs->GetYaxis()->SetLabelSize(.06);
  hs->GetYaxis()->SetTitleOffset(.5);
  hs->GetXaxis()->SetTitleSize(0.05);
  hs->GetYaxis()->SetTitleSize(0.07);
  //  drawlegend(hist,i,fname);
  gPad->Update();
  gStyle->SetOptStat(0);
}
void decorate(TH1D* hist,int i,const char* fname){
  hist->SetLineColor(col[i]);
  hist->SetMarkerColor(col[i]);
  if(i<nBG) {
    hist->SetFillColor(col[i]);
    hist->SetFillStyle(1001);
  }
  else hist->SetLineWidth(2);
  hist->SetTitle(0);
  //  hist->GetXaxis()->SetLabelSize(.04);
  // hist->GetYaxis()->SetLabelSize(.05);
  //hist->SetXLabelSize(0.05);
  //hist->GetXaxis()->SetTitleSize(0.05);
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
  
  string lName=fname;
  lName.pop_back();lName.pop_back();lName.pop_back();lName.pop_back();lName.pop_back();
  
  if(lName=="ZJetsToNuNu"){lName="Z( #rightarrow #nu #bar{#nu})+jets";}
  else if(lName=="DYJetsToLL"){lName="DY( #rightarrow l^{+}l^{-})";}
  else if(lName=="WJetsToLNu"){lName="W( #rightarrow l  #nu)+jets";}
  else if(lName=="RareProcess"){}
  else if(lName=="TTJetsHT"){lName="t #bar{t}";}
  else if(lName=="QCD"){lName="QCD";}
  else if(lName=="GJets"){lName="#gamma +jets";}
  else if(lName=="WGJetsToLNuG"){lName="W(#rightarrow l #nu)+ #gamma";}
  else if(lName=="ZGJetsToNuNuG"){lName="Z(#rightarrow #nu #bar{#nu})+ #gamma";}
  else if(lName=="TTGJets"){lName="t #bar{t}+ #gamma";}
  else if(lName=="Sig/a100"){lName="1550_100";}
  else if(lName=="Sig/a1000"){lName="1550_1000";}
  else if(lName=="Sig/a1500"){lName="1550_1500";}

  const char *l_name=lName.c_str();
  if(i<nBG)legend->AddEntry(hist,l_name,"f");
  else legend->AddEntry(hist,l_name,"l");
  legend->SetTextSize(0.04);
  
  // legend->SetTextColor(col[i]);
  //legend->SetLineColor(col[i]);
  //if(i==nfiles-1) legend->Draw();
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
