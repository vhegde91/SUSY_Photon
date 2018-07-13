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
const int nfiles=8,nBG=6;    //Specify no. of files
TFile *f[nfiles];
//int col[11]={kTeal+9,kGreen,kYellow,kOrange,kPink+1,kMagenta+2,kBlue,kCyan,kRed,kBlue+2,kMagenta};  //Specify Colors b's
////int col[11]={kTeal+9,kGreen,kYellow,kOrange,kPink+1,kPink-2,kBlue,kCyan,kRed,kBlue+2,kMagenta};  //Specify Colors b's
//int col[11]={kPink-2,kTeal+9,kGreen,kYellow,kOrange,kBlue,kCyan,kRed,kBlue+2,kMagenta,kPink+1};  //Specify Colors b's
vector<int> col={kPink+1,kTeal+9,kYellow,kGray,kOrange,kCyan,kBlue,kRed,kBlue+2,kMagenta,kCyan};  //Specify Colors b's

TCanvas *c_cA=new TCanvas("kinVar","plot of a kin var",1500,900);

void decorate(TH1D*,int,const char*);
void decorate(THStack*,int,const char*);
void drawlegend(TH1D*,int,const char*);
void printInt(TH1D*,int,const char*);
TLegend *legend1=new TLegend(0.4, 0.75,  0.87, 0.88);
TLegend *legend2=new TLegend(0.35, 0.62,  0.82, 0.72);

//TLegend *legend2=new TLegend(0.7, 0.9,  0.90, 0.65);
//TLegend *legend2=new TLegend(0.6, 0.90,  0.98, 0.45);
void setLastBinAsOverFlow(TH1D*);
void plotKinStack(){
  double sr_Integral=0,cr_Integral=0;
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  gStyle->SetTitle(0);
  double yMin=0.1,yMax = 1000;
  //TString varName = "BestPhotonPt"; TString xLabel = "p_{T}^{#gamma} (GeV)";   int rebin=10; yMin=0.4,yMax = 1000;
  //  TString varName = "MET"; TString xLabel = "p_{T}^{miss} (GeV)";   int rebin=10; yMin=0.1,yMax = 1000;
  //TString varName = "nHadJets"; TString xLabel = "Jets";   int rebin=1; yMin=0.1,yMax = 1000;
  //  TString varName = "nBTags"; TString xLabel = "b-tagged jet";   int rebin=1; yMin=0.1,yMax = 1000;
  //  TString varName = "ST"; TString xLabel = "ST (GeV)";   int rebin=25; yMin=0.1,yMax = 1000;
  TString varName = "mindPhi1dPhi2"; TString xLabel = "Min (#Delta#Phi_{1}, #Delta#Phi_{2})";   int rebin=10; yMin=0.1,yMax = 1000;
  //"mindPhi1dPhi2";//"AllSBins_v7_CD";
  //TString varName = "METvarBin";
  //  TString xLabel = "p_{T}^{miss} (GeV)";//min(#Delta#Phi_{1},#Delta#Phi_{2})

  f[0] = new TFile("TTGJets.root");
  f[1] = new TFile("TTJetsHT.root");
  f[2] = new TFile("ZGZJ_NuNuG.root");
  f[3] = new TFile("WGJetsToLNuG.root");
  f[4] = new TFile("WJetsToLNu.root");
  f[5] = new TFile("GJetsQCD.root");
  col.resize(0);
  col={kYellow,kTeal+9,kOrange,kGray,kCyan,kPink+1,kBlue,kRed,kBlue+2,kMagenta,kCyan};
  // f[0] = new TFile("GJetsQCD.root");
  // f[1] = new TFile("TTJetsHT.root");
  // f[2] = new TFile("TTGJets.root");
  // f[3] = new TFile("WGJetsToLNuG.root");
  // f[4] = new TFile("ZGZJ_NuNuG.root");
  // f[5] = new TFile("WJetsToLNu.root");
  
  //  f[7] = new TFile("DCS_LDP_Run2016_Multijet_v2.root");
  f[6] = new TFile("FastSim_T5bbbbZG_1800_150.root");
  //  f[8] = new TFile("T5qqqqHg_1600_1000_FastSim.root");
  f[7] = new TFile("FastSim_T5bbbbZG_1800_1750.root");
  // f[7] = new TFile("T5qqqqHg_1600_150_FastSim.root");
  // f[8] = new TFile("T5qqqqHg_1600_1550_FastSim.root");
  // f[7] = new TFile("FastSim_TChiNG_0_800.root");
  // f[8] = new TFile("FastSim_TChiWG_0_800.root");

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
      hs_var->SetMinimum(yMin);
      hs_var->SetMaximum(yMax);
      decorate(hs_var,i,f[i]->GetName()); 
      //      hs_var->GetYaxis()->SetRangeUser(yMin,yMax);
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
      hs_var->GetXaxis()->SetTitleOffset(1.0);
      hs_var->GetXaxis()->SetTitle(xLabel); hs_var->GetYaxis()->SetTitle("Events");hs_var->SetTitle(0);
      hs_var->GetYaxis()->SetTitleOffset(.90);
    }
  }
  legend1->SetNColumns(2);
  legend1->SetBorderSize(0);
  legend2->SetBorderSize(0);
  legend2->SetMargin(0.12);
  c_cA->cd(); gPad->SetLogy();legend1->Draw();
  c_cA->cd(); gPad->SetLogy();legend2->Draw();
  //  gPad->RedrawAxis();
  //  hs_var->GetXaxis()->SetTitle(xLabel);
 
  textOnTop.SetTextSize(0.04);
  intLumiE.SetTextSize(0.04);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation Supplementary}}");
  sprintf(name2,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.7,0.91,name2);

  TLine *line1=new TLine( 0.3,yMin,  0.3,yMax);
  line1->Draw();
  TArrow *arrow1 = new TArrow(0.3,200,1.2,200,0.01,"|>");
  arrow1->Draw();
  TLatex Tl;
  Tl.SetTextSize(0.04);
  Tl.DrawLatex(0.33,240,"#bf{Signal Region}");
  //  c_cB->SaveAs("searchBins.png");
  cout<<"*****************************************************"<<endl;
  cout<<"Int Lumi(inv.fb) for file1:"<<setprecision(4)<<intLumi<<endl;
  //------------------------------------
  c_cA->cd(); c_cA->SetGridx(0); c_cA->SetGridy(0);
  if(varName=="AllSBins_v7_CD"){
    TLine *line1V7=new TLine( 6.5,0.5,  6.5,3000);
    TLine *line2V7=new TLine(11.5,0.5, 11.5,3000);
    TLine *line3V7=new TLine(16.5,0.5, 16.5,1000);
    TLine *line4V7=new TLine(21.5,0.5, 21.5,300);
    TLine *line5V7=new TLine(26.5,0.5, 26.5,300);
    

    c_cA->cd(); c_cA->SetGridx(0); c_cA->SetGridy(0);
    line1V7->Draw();      line2V7->Draw();  line3V7->Draw();
    line4V7->Draw();      line5V7->Draw();

    TArrow *arrow1 = new TArrow( 0.5,1000, 6.5,1000,0.01,"<|>");
    TArrow *arrow2 = new TArrow( 6.5,1000,11.5,1000,0.01,"<|>");
    TArrow *arrow3 = new TArrow(11.5,650,16.5,650,0.01,"<|>");
    TArrow *arrow4 = new TArrow(16.5,300, 21.5,300,0.01,"<|>");
    TArrow *arrow5 = new TArrow(21.5,300, 26.5,300,0.01,"<|>");
    TArrow *arrow6 = new TArrow(26.5,300, 31.5,300,0.01,"<|>");

    arrow1->Draw(); arrow2->Draw(); arrow3->Draw();
    arrow4->Draw(); arrow5->Draw(); arrow6->Draw();

    TLatex Tl;
    Tl.SetTextSize(0.04);
    Tl.DrawLatex(3.,1500,"N^{ 0}_{ 2-4}");
    Tl.DrawLatex(8.5,1500,"N^{ 0}_{ 5-6}");
    Tl.DrawLatex(13.5,850,"N^{ 0}_{ #geq7}");
    Tl.DrawLatex(18.5,400,"N^{ #geq1}_{ 2-4}");
    Tl.DrawLatex(23.5,400,"N^{ #geq1}_{ 5-6}");
    Tl.DrawLatex(28.5,400,"N^{ #geq1}_{ #geq7}");
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
  
  if(lName.Contains("ZGZJ")){lName="Z(#nu#bar{#nu}) + #gamma";}
  else if(lName.Contains("DYJetsToLL")){lName="DY(l^{+}l^{-})";}
  else if(lName.Contains("WJetsToLNu")){lName="W(l#nu) + jets";}
  else if(lName.Contains("RareProcess")){}
  else if(lName.Contains("TTJetsHT")){lName="t #bar{t}";}
  else if(lName.Contains("WGJetsToLNuG")){lName="W(l#nu) + #gamma";}
  else if(lName.Contains("ZGJetsToNuNuG")){lName="Z(#nu#bar{#nu}) + #gamma";}
  else if(lName.Contains("TTGJets")){lName="t #bar{t} + #gamma";}
  //  else if(lName.Contains("QCD")){lName="QCD";}
  else if(lName.Contains("GJets")){lName="#gamma + jets";}
  else if(lName.Contains("Run2016")){lName="Data";}
  //  else if(lName.Contains("T5bbbbZg_1600_150")){lName="T5bbbbZg 1600, 150";}
  else if(lName.Contains("T5bbbbZg_1600_150")){lName = "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1600 GeV, m_{#tilde{#chi}_{1}^{0}} = 150 GeV)";}
  else if(lName.Contains("T5bbbbZg_1600_1550")){lName = "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1600 GeV, m_{#tilde{#chi}_{1}^{0}} = 1550 GeV)";}
  else if(lName.Contains("T5bbbbZG_1800_150")){lName = "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 150 GeV)";}
  else if(lName.Contains("T5bbbbZG_1800_1750")){lName = "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 1750 GeV)";}
  // else if(lName.Contains("T5bbbbZg_1600_150")){lName = "T5bbbbZG (m_{#tilde{g}} = 1600 GeV, m_{#tilde{#chi}_{1}^{0}} = 150 GeV)";}
  // else if(lName.Contains("T5bbbbZg_1600_1550")){lName = "T5bbbbZG (m_{#tilde{g}} = 1600 GeV, m_{#tilde{#chi}_{1}^{0}} = 1550 GeV)";}
  else if(lName.Contains("T5bbbbZg_1600_1550")){lName="T5bbbbZg 1600, 1550";}
  else if(lName.Contains("T5qqqqHg_1600_1550")){lName="T5qqqqHg_1550";}
  else if(lName.Contains("T5qqqqHg_1600_150")){lName="T5qqqqHg_150";}

  // const char *l_name=lName.c_str();
  if(i<nBG)legend1->AddEntry(hist,lName,"f");
  else legend2->AddEntry(hist,lName,"l");
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

/*
{
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
    }*/
