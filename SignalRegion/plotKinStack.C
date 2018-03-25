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
const int nfiles=11,nBG=7;    //Specify no. of files
TFile *f[nfiles];
//int col[11]={kTeal+9,kGreen,kYellow,kOrange,kPink+1,kMagenta+2,kBlue,kCyan,kRed,kBlue+2,kMagenta};  //Specify Colors b's
////int col[11]={kTeal+9,kGreen,kYellow,kOrange,kPink+1,kPink-2,kBlue,kCyan,kRed,kBlue+2,kMagenta};  //Specify Colors b's
//int col[11]={kPink-2,kTeal+9,kGreen,kYellow,kOrange,kBlue,kCyan,kRed,kBlue+2,kMagenta,kPink+1};  //Specify Colors b's
int col[11]={kPink+1,kTeal+9,kGreen,kYellow,kOrange,kBlue,kCyan,kRed,kBlue+2,kMagenta,kCyan};  //Specify Colors b's

TCanvas *c_cA=new TCanvas("kinVar","plot of a kin var",1500,900);

void decorate(TH1D*,int,const char*);
void decorate(THStack*,int,const char*);
void drawlegend(TH1D*,int,const char*);
void printInt(TH1D*,int,const char*);
TLegend *legend1=new TLegend(0.4501, 0.65,  0.87, 0.88);

//TLegend *legend2=new TLegend(0.7, 0.9,  0.90, 0.65);
//TLegend *legend2=new TLegend(0.6, 0.90,  0.98, 0.45);
void setLastBinAsOverFlow(TH1D*);
void plotKinStack(){
  double sr_Integral=0,cr_Integral=0;
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  gStyle->SetTitle(0);
  TString varName = "AllSBins_v7_CD";//mindPhi1dPhi2
  TString xLabel = "Bin no.";//min(#Delta#Phi_{1},#Delta#Phi_{2})
  int rebin=1;

  f[0] = new TFile("ZGZJ_NuNuG.root");
  f[1] = new TFile("TTGJets.root");
  f[2] = new TFile("TTJetsHT.root");
  f[3] = new TFile("WGJetsToLNuG.root");
  f[4] = new TFile("WJetsToLNu.root");
  f[5] = new TFile("QCD.root");
  f[6] = new TFile("GJets.root");
  //  f[7] = new TFile("DCS_LDP_Run2016_Multijet_v2.root");
  f[7] = new TFile("T5bbbbZg_1600_150_FastSim.root");
  //f[8] = new TFile("T5qqqqHg_1600_1000_FastSim.root");
  f[8] = new TFile("T5bbbbZg_1600_1550_FastSim.root");
  f[9] = new TFile("T5qqqqHg_1600_150_FastSim.root");
  f[10] = new TFile("T5qqqqHg_1600_1550_FastSim.root");

  //  f[10] = new TFile("T1bbbb_ZG_mGl1600_NLSP150.root");
  // f[9] = new TFile("T1bbbb_ZG_mGl1600_NLSP1000.root");
  // f[10] = new TFile("T1bbbb_ZG_mGl1600_NLSP1550.root");


  gStyle->SetTextSize(2);
  THStack *hs_var=new THStack("var_Stack","MET Stacked");
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
    TH1D *h_intLumi=(TH1D*)f[i]->FindObjectAny("intLumi");
    if(i==0) {
      intLumi=h_intLumi->GetMean();
      sprintf(name2, "%.2f fb^{-1}",intLumi);
    }
    else{
      if(abs(intLumi-h_intLumi->GetMean())>0.0001)
	cout<<"Integarted lumi for "<<f[i]->GetName()<<" is "<<h_intLumi->GetMean()<<" and for other files it is different"<<endl;
    }
    
    TH1D *h_MET=(TH1D*)f[i]->FindObjectAny(varName);
    h_MET->Rebin(rebin);
    //    h_MET->GetYaxis()->SetRangeUser(100.5,20000);
    //    h_MET->SetMinimum(100);
    decorate(h_MET,i,f[i]->GetName());
    
    if(i<=(nBG-1))  hs_var->Add(h_MET);

    if(i==nBG-1) {
      c_cA->cd();
      hs_var->Draw("BAR HIST");
      hs_var->Draw("HIST");
      hs_var->SetMinimum(0.8);
      hs_var->SetMaximum(10000);
      decorate(hs_var,i,f[i]->GetName()); 
      //hs_var->GetYaxis()->SetRangeUser(100.5,20000);
    }
    if(i>=nBG){ 
      c_cA->cd(); 
      h_MET->SetMarkerStyle(20);
      h_MET->SetMarkerColor(col[i]);
      h_MET->SetLineColor(col[i]);
      h_MET->SetLineWidth(3);
      if(i>=9)  h_MET->SetLineStyle(2);
      h_MET->Draw("hist same");
      //      h_MET->GetYaxis()->SetRangeUser(0.5,20000);
      //      h_MET->GetYaxis()->SetRangeUser(100.5,20000);
    }
    drawlegend(h_MET,i,f[i]->GetName());
    if(i==nfiles-1){ 
      hs_var->GetXaxis()->SetTitleOffset(.90);
      hs_var->GetXaxis()->SetTitle(xLabel); hs_var->GetYaxis()->SetTitle("Events");hs_var->SetTitle(0);
    }
  }
  legend1->SetNColumns(2);
  legend1->SetBorderSize(0);
  c_cA->cd(); gPad->SetLogy();legend1->Draw();
  //  gPad->RedrawAxis();
  //  hs_var->GetXaxis()->SetTitle(xLabel);
 
  textOnTop.SetTextSize(0.05);
  intLumiE.SetTextSize(0.05);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
  intLumiE.SetTextSize(0.05);
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.7,0.91,name2);

  // TLine *line1=new TLine( 0.3,0.3,  0.3,10000);
  // line1->Draw();
  // TArrow *arrow1 = new TArrow(0.3,3000,1.2,3000,0.01,"|>");
  // arrow1->Draw();
  // TLatex Tl;
  // Tl.SetTextSize(0.04);
  // Tl.DrawLatex(.4,4000,"#bf{Signal Region}");
  //  c_cB->SaveAs("searchBins.png");
  cout<<"*****************************************************"<<endl;
  cout<<"Int Lumi(inv.fb) for file1:"<<setprecision(4)<<intLumi<<endl;
  //------------------------------------
  if(0){
    TLine *line1V7=new TLine( 7.5,0.5,  7.5,3000);
    TLine *line2V7=new TLine(14.5,0.5, 14.5,3000);
    TLine *line3V7=new TLine(21.5,0.5, 21.5,3000);
    TLine *line4V7=new TLine(28.5,0.5, 28.5,300);
    TLine *line5V7=new TLine(35.5,0.5, 35.5,300);
    TLine *line6V7=new TLine(42.5,0.5, 42.5,300);
    TLine *line7V7=new TLine(49.5,0.5, 49.5,300);
    TLine *line8V7=new TLine(56.5,0.5, 56.5,300);

    c_cA->cd(); c_cA->SetGridx(0); c_cA->SetGridy(0);
    line1V7->Draw();      line2V7->Draw();  line3V7->Draw();
    line4V7->Draw();      line5V7->Draw();  line6V7->Draw();
    line7V7->Draw();      line8V7->Draw();
    TArrow *arrow1 = new TArrow(0.5,1500,7.5,1500,0.01,"<|>");
    TArrow *arrow2 = new TArrow(7.5,1500,14.5,1500,0.01,"<|>");
    TArrow *arrow3 = new TArrow(14.5,1500,21.5,1500,0.01,"<|>");
    TArrow *arrow4 = new TArrow(21.5,150,28.5,150,0.01,"<|>");
    TArrow *arrow5 = new TArrow(28.5,150,35.5,150,0.01,"<|>");
    TArrow *arrow6 = new TArrow(35.5,150,42.5,150,0.01,"<|>");
    TArrow *arrow7 = new TArrow(42.5,150,49.5,150,0.01,"<|>");
    TArrow *arrow8 = new TArrow(49.5,150,56.5,150,0.01,"<|>");
    TArrow *arrow9 = new TArrow(56.5,150,63.5,150,0.01,"<|>");


    arrow1->Draw(); arrow2->Draw(); arrow3->Draw();
    arrow4->Draw(); arrow5->Draw(); arrow6->Draw();
    arrow7->Draw(); arrow8->Draw(); arrow9->Draw();
    TLatex Tl;
    Tl.SetTextSize(0.04);
    Tl.DrawLatex(3.,2000,"N^{0}_{2-4}");
    Tl.DrawLatex(10,2000,"N^{0}_{5-6}");
    Tl.DrawLatex(17,2000,"N^{0}_{#geq7}");
    Tl.DrawLatex(24,200,"N^{1}_{2-4}");
    Tl.DrawLatex(31,200,"N^{1}_{5-6}");
    Tl.DrawLatex(38,200,"N^{1}_{#geq7}");
    Tl.DrawLatex(45,200,"N^{#geq2}_{2-4}");
    Tl.DrawLatex(52,200,"N^{#geq2}_{5-6}");
    Tl.DrawLatex(59,200,"N^{#geq2}_{#geq7}");
  }
  //------------------------------------
    
}

void decorate(THStack *hs,int i,const char* fname){
  //  hs->SetMinimum(0.5);
  //hs->SetTitle(0);
  hs->GetXaxis()->SetLabelSize(.05);
  hs->GetYaxis()->SetLabelSize(.05);
  hs->GetXaxis()->SetTitleSize(0.05);
  hs->GetYaxis()->SetTitleSize(0.05);
  //  drawlegend(hist,i,fname);
  //  gPad->Update();
  gStyle->SetOptStat(0);
}
void decorate(TH1D* hist,int i,const char* fname){
  hist->SetLineColor(col[i]);
  if(i<nBG) {
    hist->SetFillColor(col[i]);
    hist->SetLineColor(kBlack);
    hist->SetLineWidth(1);
  }
  else hist->SetLineWidth(2);
  hist->SetTitle(0);
  hist->GetXaxis()->SetLabelSize(.06);
  hist->GetYaxis()->SetLabelSize(.06);
  //hist->SetXLabelSize(0.05);
  hist->GetXaxis()->SetTitleSize(0.06);
  // drawlegend(hist,i,fname);
  //  gPad->Update();
  setLastBinAsOverFlow(hist);
  gStyle->SetOptStat(0);
  //Hlist.Add(hist);
}

void drawlegend(TH1D *hist,int i,const char* fname){
  gStyle->SetLegendBorderSize(0);
 
  TString lName=fname;
  
  if(lName.Contains("ZGZJ")){lName="Z(#nu#bar{#nu})+#gamma";}
  else if(lName.Contains("DYJetsToLL")){lName="DY(l^{+}l^{-})";}
  else if(lName.Contains("WJetsToLNu")){lName="W(l#nu)+jets";}
  else if(lName.Contains("RareProcess")){}
  else if(lName.Contains("TTJetsHT")){lName="t #bar{t}";}
  else if(lName.Contains("QCD")){lName="QCD";}
  else if(lName.Contains("WGJetsToLNuG")){lName="W(l#nu)+ #gamma";}
  else if(lName.Contains("ZGJetsToNuNuG")){lName="Z(#nu#bar{#nu})+ #gamma";}
  else if(lName.Contains("TTGJets")){lName="t #bar{t}+ #gamma";}
  else if(lName.Contains("GJets")){lName="#gamma +jets";}
  else if(lName.Contains("Run2016")){lName="Data";}
  else if(lName.Contains("T5bbbbZg_1600_150")){lName="T5bbbbZg_150";}
  //  else if(lName.Contains("T5bbbbZg_1600_150")){lName="T5bbbbZG(1.6,0.15)";}
  //  else if(lName.Contains("T5bbbbZg_1600_150")){lName="#tilde{g}_{1600}#rightarrow b#bar{b}#tilde{#chi}_{1,150}^{0}";}
  else if(lName.Contains("T5bbbbZg_1600_1550")){lName="T5bbbbZg_1550";}
  else if(lName.Contains("T5qqqqHg_1600_1550")){lName="T5qqqqHg_1550";}
  else if(lName.Contains("T5qqqqHg_1600_150")){lName="T5qqqqHg_150";}

  // const char *l_name=lName.c_str();
  if(i<nBG)legend1->AddEntry(hist,lName,"f");
  else legend1->AddEntry(hist,lName,"l");
  // legend1->SetTextSize(0.04);
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
