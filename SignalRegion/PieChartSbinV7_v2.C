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
#include"TPie.h"
//int col[11]={kTeal+9,kGreen,kYellow,kOrange,kPink+1,kMagenta+2,kCyan,kBlue,kRed,kMagenta,kBlue+2};  //Specify Colors b's
//int col[]={kGray,kTeal+9,kOrange,kPink+1,kYellow,kCyan,kBlue,kRed,kBlue,kMagenta,kCyan};
//int col[]={kGray,kTeal+9,kBlue,kPink+1,kYellow,kCyan,kBlue,kRed,kBlue,kMagenta,kCyan};//v1
int col[]={kGray,kTeal+9,kOrange,kRed,kCyan-1,kCyan,kBlue,kMagenta+2,kPink+1,kMagenta,kBlack};
char name[100];
TLatex textOnTop,intLumiE;
char name2[100];
double intLumi=35.9;
TString getLegName(TString);
void PieChartSbinV7_v2(){
  TFile *f[6];
  // gStyle->SetPadColor(kBlack);
  // gStyle->SetPadBorderMode
  //  gStyle->SetPadBorderSize(2);
  int ii=0;
  ii = 2; f[ii] = new TFile("TTGJets.root"); col[ii] = kGray;
  ii = 3; f[ii] = new TFile("TTJetsHT.root"); col[ii] = kTeal+9;
  ii = 0; f[ii] = new TFile("ZGZJ_NuNuG.root"); col[ii] = kOrange;
  ii = 1; f[ii] = new TFile("WGJetsToLNuG.root"); col[ii] = kRed;
  ii = 4; f[ii] = new TFile("WJetsToLNu.root"); col[ii] = kCyan-1;
  ii = 5; f[ii] = new TFile("GJetsQCD.root"); col[ii] = kCyan;
  int nHists=6; //== no. of files
  //  vector<TString> histName={"AllSBins_v7_ZGPred","AllSBins_v7_LElePred","AllSBins_v7_LMuPred","AllSBins_v7_FRPred","AllSBins_v7_MultiJPred"};
  TString histName = "AllSBins_v7_CD";
  bool drawUnc=0;
  int nLastBins=1;//one less than no. of bins to include
  
  TH1D *h_hist[nHists];
  TCanvas *c_cA=new TCanvas("name","name",1100,1000);
  //  c_cA->SetTopMargin(2);
  TPad *pad[6];
  // pad[0]=new TPad("LB","LB",0.001,0.0, 0.332,0.4);
  // pad[1]=new TPad("MB","MB",0.333,0.0, 0.665,0.4);  
  // pad[2]=new TPad("RB","RB",0.666,0.0, 1.000,0.4);
  // pad[3]=new TPad("LT","LT",0.001,0.4, 0.332,0.8);
  // pad[4]=new TPad("MT","MT",0.333,0.4, 0.665,0.8);
  // pad[5]=new TPad("RT","RT",0.666,0.4, 1.000,0.8);

  pad[0]=new TPad("LB","LB",0.2,  0.1, 0.432,0.4);
  pad[1]=new TPad("MB","MB",0.433,0.1, 0.665,0.4);  
  pad[2]=new TPad("RB","RB",0.666,0.1, 0.898,0.4);
  pad[3]=new TPad("LT","LT",0.2,  0.4, 0.432,0.7);
  pad[4]=new TPad("MT","MT",0.433,0.4, 0.665,0.7);
  pad[5]=new TPad("RT","RT",0.666,0.4, 0.898,0.7);
  // pad[0]->SetBorderSize(6); //pad[0]->SetHighLightColor(kBlue);
  // pad[0]->SetBorderColor(6);
  // TPave *pv1 = new TPave(0.19,0.1,0.9,0.81);
  // pv1->SetFillStyle(0);
  // pv1->SetShadowColor(0);
  // pv1->Draw();
  TLegend *legend=new TLegend(0.25, 0.72,  0.93, 0.8);
  double intgrlRange1[nHists];
  double intgrlRange2[nHists];
  double intgrlRange3[nHists];
  double intgrlRange4[nHists];
  double intgrlRange5[nHists];
  double intgrlRange6[nHists];
  double err=0,temp=0;
  for(int i=0;i<nHists;i++){
    h_hist[i]=(TH1D*)f[i]->FindObjectAny(histName);
    h_hist[i]->SetLineColor(kBlack);
    h_hist[i]->SetLineWidth(1);
    h_hist[i]->SetFillColor(col[i]);
    legend->AddEntry(h_hist[i],getLegName(f[i]->GetName()),"f");
    if(drawUnc){
      intgrlRange1[i]=err; temp=h_hist[i]->IntegralAndError(6-nLastBins-1,6,err); //include one extra bin here
      intgrlRange2[i]=err; temp=h_hist[i]->IntegralAndError(11-nLastBins,11,err);
      intgrlRange3[i]=err; temp=h_hist[i]->IntegralAndError(16-nLastBins,16,err);
      intgrlRange4[i]=err; temp=h_hist[i]->IntegralAndError(21-nLastBins,21,err);
      intgrlRange5[i]=err; temp=h_hist[i]->IntegralAndError(26-nLastBins,26,err);
      intgrlRange6[i]=err; temp=h_hist[i]->IntegralAndError(31-nLastBins,31,err);
    }
    else{
      // intgrlRange1[i]=h_hist[i]->Integral(6-nLastBins-1,6); //include one extra bin here
      // intgrlRange2[i]=h_hist[i]->Integral(11-nLastBins,11);
      // intgrlRange3[i]=h_hist[i]->Integral(16-nLastBins,16);
      // intgrlRange4[i]=h_hist[i]->Integral(21-nLastBins,21);
      // intgrlRange5[i]=h_hist[i]->Integral(26-nLastBins,26);
      // intgrlRange6[i]=h_hist[i]->Integral(31-nLastBins,31);

      // intgrlrange1[i]=h_hist[i]->Integral(4,6); //include one extra bin here
      // intgrlRange2[i]=h_hist[i]->Integral(10,11);
      // intgrlRange3[i]=h_hist[i]->Integral(15,16);
      // intgrlRange4[i]=h_hist[i]->Integral(20,21);
      // intgrlRange5[i]=h_hist[i]->Integral(25,26);
      // intgrlRange6[i]=h_hist[i]->Integral(30,31);

      intgrlRange1[i]=h_hist[i]->Integral(2,3);
      intgrlRange2[i]=h_hist[i]->Integral(8,9);
      intgrlRange3[i]=h_hist[i]->Integral(12,13);
      intgrlRange4[i]=h_hist[i]->Integral(18,19);
      intgrlRange5[i]=h_hist[i]->Integral(23,24);
      intgrlRange6[i]=h_hist[i]->Integral(28,29);
      // cout<<f[i]->GetName()<<" "<<intgrlRange1[i]<<endl;
      // cout<<f[i]->GetName()<<" "<<intgrlRange2[i]<<endl;
    }
  }
  TPie *pie[6];
  pie[0]= new TPie("pie1","",nHists,intgrlRange1,col);
  pie[1]= new TPie("pie2","",nHists,intgrlRange2,col);
  pie[2]= new TPie("pie3","",nHists,intgrlRange3,col);
  
  pie[3]= new TPie("pie4","",nHists,intgrlRange4,col);
  pie[4]= new TPie("pie5","",nHists,intgrlRange5,col);
  pie[5]= new TPie("pie6","",nHists,intgrlRange6,col);
  //  pie[0]->Draw();
  legend->SetNColumns(3);
  legend->SetBorderSize(0);
  legend->Draw();
  //  gStyle->SetTextSize(20);
  for(int i=0;i<6;i++){
    c_cA->cd();   
    pad[i]->SetTitle(0);
    pad[i]->Draw();
    pad[i]->cd();
    //h_hist[i]->Draw();
    //    pie[i]->SetTextSize(40);
    pie[i]->Draw();
    pie[i]->SetLabelFormat(0);
  }
 
  c_cA->cd();
  TPave *pv1 = new TPave(0.19,0.1,0.9,0.81);
  pv1->SetFillStyle(0);
  pv1->SetShadowColor(0);
  TLatex latex;
  latex.DrawLatex(0.24,0.05,"2 - 4 jets");
  latex.DrawLatex(0.48,0.05,"5 - 6 jets");
  latex.DrawLatex(0.73,0.05,"#geq 7 jets");

  latex.SetTextAngle(90);
  latex.DrawLatex(0.1612022,0.1697192,"0 b tags"); //latex.DrawLatex(0.05,0.25,"0 b tags");
  latex.SetTextAngle(90);
  latex.DrawLatex(0.1612022,0.4371184,"#geq 1 b tags"); //  latex.DrawLatex(0.03,0.55,"#geq 1 b tags");

  TLine *line0 = new TLine(0.19, 0.705,  0.9, 0.705);
  line0->Draw();

  pv1->Draw();


  textOnTop.SetTextSize(0.035);
  intLumiE.SetTextSize(0.035);
  textOnTop.DrawLatexNDC(0.19,0.825,"CMS #it{#bf{Simulation Supplementary}}");
  sprintf(name2,"#bf{(13 TeV)}");
  intLumiE.DrawLatexNDC(0.8,0.825,name2);
  TLatex Tl;
  Tl.SetTextSize(0.03);
  Tl.DrawLatexNDC(0.6,0.825,"#bf{arXiv:xxxx.xxxxx}");
  //    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 )

   //  c_cA->SaveAs(histName+name);
  //pdfcrop --clip supp_Sim_Pie.pdf a.pdf --margins 10
}

TString getLegName(TString fname){
  if(fname.Contains("ZGZJ")){fname="Z(#nu#bar{#nu}) + #gamma";}
  else if(fname.Contains("DYJetsToLL")){fname="DY(l^{+}l^{-})";}
  else if(fname.Contains("WJetsToLNu")){fname="W(l#nu) + jets";}
  else if(fname.Contains("RareProcess")){}
  else if(fname.Contains("TTJetsHT")){fname="t #bar{t}";}
  else if(fname.Contains("WGJetsToLNuG")){fname="W(l#nu) + #gamma";}
  else if(fname.Contains("ZGJetsToNuNuG")){fname="Z(#nu#bar{#nu}) + #gamma";}
  else if(fname.Contains("TTGJets")){fname="t #bar{t} + #gamma";}
  //  else if(fname.Contains("QCD")){fname="QCD";}
  else if(fname.Contains("GJets")){fname="#gamma + jets";}
  else if(fname.Contains("Run2016")){fname="Data";}

  return fname;
}
