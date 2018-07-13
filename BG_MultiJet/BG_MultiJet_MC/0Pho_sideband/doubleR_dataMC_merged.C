#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"TH1.h"
#include"TROOT.h"
#include"TH2.h"
#include"TFile.h"
#include"TDirectory.h"
#include"TTree.h"
#include"TBrowser.h"
#include"TF1.h"
#include<iomanip>
#include"TCanvas.h"
#include"TPaveStats.h"
#include"TLegend.h"
#include"TStyle.h"
#include"TString.h"
#include"TLatex.h"
using namespace std;

//int col[5]={kBlue,kMagenta,kRed,kTeal+9,kBlack};
int col[5]={kRed,kBlue,kTeal+9,kMagenta,kBlack};
int rebin=1;
TString name,txtTop,txtLumi;
TLatex textOnTop,intLumiE;
double intLumi=35.9;
TString getXaxisName(TString);
void setLastBinAsOverFlow(TH1D*);

void doubleR_dataMC_merged(){
  TH1::SetDefaultSumw2(1);

  bool saveCanvas=0;
  vector<TString> legNames, name2;
  vector<int> rebin;
  double nunu_Integral=0,ll_Integral=0;
  // double dRData[12] = {0.58973,0.50699,0.45871,0.42018,0.38011,0.36777,0.51577,0.45348,0.41057,0.36905,0.37032,0.33742};
  // double dRDataUnc[12] = {0.00782,0.00739,0.00832,0.01068,0.01501,0.01885,0.01280,0.01034,0.01034,0.01240,0.01711,0.02005};
  // double dRMC[12] = {0.52004,0.47255,0.44953,0.38988,0.35691,0.39278,0.43122,0.40502,0.38646,0.36713,0.35995,0.33724};
  // double dRMCUnc[12] = {0.01089,0.01008,0.01182,0.01369,0.01848,0.02445,0.01746,0.01325,0.01393,0.01610,0.02119,0.02243};

  double dRData[6] = {0.525550,0.406994,0.367770,0.452054,0.369393,0.337416};
  double dRDataUnc[6] = {0.004529,0.008697,0.018846,0.006319,0.010047,0.020047};

  double dRMC[6] = {0.483443,0.378652,0.392778,0.401730,0.363947,0.337237};
  double dRMCUnc[6] = {0.006280,0.011004,0.024448,0.008350,0.012786,0.022428};

  double dRatioUnc[6] = {0.09,0.07,0.07, 0.13,0.04,0.04};

  TString binLabel[6] = {"N^{ 0}_{ 2-4}","N^{ 0}_{ 5-6}","N^{ 0}_{ #geq7}","N^{ #geq1}_{ 2-4}","N^{ #geq1}_{ 5-6}","N^{ #geq1}_{ #geq7}"};

  TH1D *h_numCp;
  TH1D *h_num = new TH1D("Data","data",6,0.5,6.5);
  TH1D *h_den = new TH1D("MC","MC",6,0.5,6.5);
  TH1D *h_DratioUnc = new TH1D("DratioUnc","assigned unc on double ratio",6,0.5,6.5);

  TLine *line1 = new TLine(0.5,1.0,6.5,1.0);
  line1->SetLineStyle(2);

  gStyle->SetPadTickX(0);
  for(int i=1;i<=6;i++){
    h_num->SetBinContent(i,dRData[i-1]);
    h_num->SetBinError(i,dRDataUnc[i-1]);
    h_den->SetBinContent(i,dRMC[i-1]);
    h_den->SetBinError(i,dRMCUnc[i-1]);
    h_DratioUnc->SetBinContent(i,1);
    h_DratioUnc->SetBinError(i,dRatioUnc[i-1]);
  }
  //  return;
  //  name=fd->GetName();
  name="TF_"+name;
  
  TString histToSave="nHadJets";
  // TFile *fd=new TFile("CS_ZGToLLG.root");

  name2.push_back("Region");  rebin.push_back(5);
  TString nameN="Data";
  TString nameD="MC";
  TLegend *legend[name2.size()];
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);

  TCanvas *c_cA[name2.size()];
  TPad *p_top[name2.size()],*p_bot[name2.size()];

  for(int i=0;i<name2.size();i++){
    //    name = name2[i]+"NuNu_LL";
    //    name = name2[i]+"NLO_LO";
    name = "DoubleRatio_VR_DataMC";
    c_cA[i] = new TCanvas(name,name,1000,600);
    p_top[i] =new TPad(name+"top",name+"top",0,0.4,1,1);
    p_bot[i] =new TPad(name+"bot",name+"bot",0,0.0,1,0.4);
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy(0);//p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.35);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy(0);
    //    textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Simulation}}");
  }

  for(int i=0;i<name2.size();i++){
    // h_num=(TH1D*)fn->FindObjectAny(name2[i]);
    // h_den=(TH1D*)fd->FindObjectAny(name2[i]);
    if(h_num && h_den){
      if(nameN.Contains("Data")){
	h_num->SetLineColor(kBlack);
	h_num->SetMarkerStyle(20);
      }
      else{
	h_num->SetLineColor(kRed);
	h_num->SetMarkerStyle(21);
      }
      h_num->SetLineWidth(2);
      h_num->SetMarkerColor(h_num->GetLineColor());
      h_num->SetTitle(";;Double Ratio (#kappa)");
      h_num->GetYaxis()->SetLabelSize(0.09);
      h_num->GetYaxis()->SetTitleSize(0.09);
      h_num->GetYaxis()->SetTitleOffset(0.53);

      h_den->SetLineColor(kBlue);//kMagenta+2
      //      h_den->SetLineColor(kMagenta+2);//MC
      h_den->SetLineWidth(2);
      h_den->SetMarkerStyle(4);
      h_den->SetMarkerSize(1.12);
      h_den->SetMarkerColor(h_den->GetLineColor());
      h_num->GetYaxis()->SetNdivisions(5);
      h_num->GetYaxis()->SetNdivisions(505);

      h_num->GetXaxis()->SetNdivisions(10);
      h_den->GetXaxis()->SetNdivisions(10);
      // h_num->GetXaxis()->SetAxisColor(0);
      // h_num->GetXaxis()->SetNdivisions(0);
      //      for(int j=1;j<=h_numCp->GetNbinsX();j++){
      	// h_num->GetXaxis()->SetBinLabel(j,binLabel[j-1]);
	// h_den->GetXaxis()->SetBinLabel(j,binLabel[j-1]);
	//      } 
      
      c_cA[i]->cd();p_top[i]->cd();
      // for(int j=1;j<=h_numCp->GetNbinsX();j++){
      // 	h_num->GetXaxis()->SetBinLabel(j,binLabel[j-1]);
      // }
      // textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Simulation}}");
      // textOnTop.DrawLatexNDC(0.1,0.91,txtTop);
      // intLumiE.DrawLatexNDC(0.73,0.91,txtLumi);
      // gPad->Update();
      p_top[i]->SetTickx();
      p_top[i]->SetTicky();
      h_num->GetYaxis()->SetTickLength(0.07/3);
      h_num->Draw("E1X0");
      h_den->Draw("E1X0 same");

      legend[i]=new TLegend(0.74, 0.64,  0.94, 0.89);
      legend[i]->AddEntry(h_num,nameN,"elp");
      legend[i]->AddEntry(h_den,nameD,"elp");
      legend[i]->Draw();
      gPad->RedrawAxis();
      //---------------- for ratio ------------------
      h_numCp=(TH1D*)h_num->Clone("Ratio_DataMC");
      h_numCp->Divide(h_den);
      h_numCp->SetLineColor(kBlack);
      h_numCp->SetMarkerColor(kBlack);
      h_numCp->GetYaxis()->SetTickLength(0.097/3);
      // h_numCp->SetMaximum(1.99);
      // h_numCp->SetMinimum(0.001);
      h_numCp->SetMaximum(1.3);
      h_numCp->SetMinimum(0.7);

      h_numCp->GetXaxis()->SetTitle(getXaxisName(name2[i]));
      h_numCp->GetXaxis()->SetLabelSize(0.18);
      h_numCp->GetXaxis()->SetTitleSize(0.16);
      h_numCp->GetXaxis()->SetTitleOffset(0.90);
      h_numCp->GetXaxis()->SetAxisColor(kBlack);
      for(int j=1;j<=h_numCp->GetNbinsX();j++){
      	h_numCp->GetXaxis()->SetBinLabel(j,binLabel[j-1]);
      }
      h_numCp->GetYaxis()->SetTitle("#frac{Data}{MC}");
      h_numCp->GetYaxis()->SetTitle("Data / MC");
      h_numCp->GetYaxis()->CenterTitle(1);
      h_numCp->GetYaxis()->SetTitleOffset(0.35);
      h_numCp->GetYaxis()->SetTitleSize(0.13);
      h_numCp->GetYaxis()->SetLabelSize(0.14);
      h_numCp->GetYaxis()->SetNdivisions(505);

      c_cA[i]->cd();    p_bot[i]->cd();
      p_bot[i]->SetTickx();p_bot[i]->SetTicky();
      h_numCp->Draw("e1X0");
      h_DratioUnc->SetFillStyle(3013);
      h_DratioUnc->SetFillColor(1);
      h_DratioUnc->Draw("E2same");
      line1->Draw();

      c_cA[i]->cd();    p_top[i]->cd();       gPad->RedrawAxis();
      char name3[100];
      textOnTop.SetTextSize(0.07);
      intLumiE.SetTextSize(0.07);
      textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
      //textOnTop.DrawLatexNDC(0.12,0.91,"CMS");
      sprintf(name3,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
      intLumiE.DrawLatexNDC(0.71,0.91,name3);

      if(saveCanvas){name="c_"+name2[i]+".png";c_cA[i]->SaveAs(name);}
    }
  }
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
  else if(axname.Contains("MET")) return "p_{T}^{miss}(GeV)";
  else return axname;

}

/*
  TH1D *h_den=(TH1D*)fd->FindObjectAny(nameD);
  h_num->Rebin(rebin);
  h_den->Rebin(rebin);

  h_num->SetLineColor(col[0]);
  h_num->SetLineWidth(2);
  h_den->SetLineColor(col[1]);
  h_den->SetLineWidth(2);
  //  h_den->Scale(h_num->Integral()/h_den->Integral());
  TH1D *h_numC=(TH1D*)h_num->Clone("hnew");
  h_numC->Divide(h_den);

  c_cA->cd();    p_top->cd();
  //h_num->Scale(0.5*h_num->Integral());
  // h_num->Scale(1.0/h_num->Integral());
  // h_den->Scale(1.0/h_den->Integral());
  
  // h_num->GetXaxis()->SetRangeUser(100,2000);

  h_num->Draw();  
  h_den->Draw("sames");
  gPad->Update();

  TPaveStats *stN=(TPaveStats*)h_num->FindObject("stats");
  TPaveStats *stD=(TPaveStats*)h_den->FindObject("stats");
  stN->SetTextColor(h_num->GetLineColor());
  stD->SetTextColor(h_den->GetLineColor());

  TLegend *leg=new TLegend(0.5,0.9,0.7,0.7);
  leg->AddEntry(h_num,legNames[0]);
  leg->AddEntry(h_den,legNames[1]);
  leg->Draw();  

  c_cA->cd();    p_bot->cd();
  h_numC->GetYaxis()->SetLabelSize(0.08);
  h_numC->GetXaxis()->SetLabelSize(0.08);
  //  h_numC->GetXaxis()->SetRangeUser(100,2000);
  h_numC->Draw();
  
}
*/
