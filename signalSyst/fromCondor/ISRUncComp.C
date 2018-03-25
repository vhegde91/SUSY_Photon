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

TFile *f[1];
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getXaxisName(TString);
TString getLegName(TString);
TLatex textOnTop,intLumiE;
double intLumi=35.9;

double sr_Integral=0,cr_Integral=0;

void ISRUncComp(TString iFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
 
  f[0] = new TFile(iFname);

  TString outFileName = "ISR_T5bbbbZg_1600_1000";
  //  TFile *fout = new TFile("signalSyst.root","recreate");
  //  gStyle->SetOptStat("nemri");
  
  vector<string> name1,name2;
  vector<int> rebin;
  //  name1.push_back("AllSBins_v7_CD");          name2.push_back("AllSBins_v7_CD");
  name1.push_back("AllSBins_v7_ISRwtNoBtagSF_CD");          name2.push_back("AllSBins_v7_NoISRWt_CD");

  TH1D *h_SBin_v7 = (TH1D*)f[0]->Get("AllSBins_v7_ISRwtNoBtagSF_CD");
  TH1D *h_isrWt = (TH1D*)h_SBin_v7->Clone("isrWt_SBins");
  TH1D *h_temp = (TH1D*)f[0]->Get("AllSBins_v7_NoISRWt_CD");
  h_isrWt->Divide(h_temp);
  //  h_isrWt->Draw();return;

  TH1D *h_isrWtSq = (TH1D*)f[0]->Get("AllSBins_v7_ISRUncSq_CD");

  for(int i=1;i<=h_isrWt->GetNbinsX();i++){
    h_isrWt->SetBinError(i, sqrt( (h_isrWtSq->GetBinContent(i)) / (h_SBin_v7->GetBinContent(i)) ));
    cout<<h_isrWt->GetBinContent(i)<<" +/- "<<h_isrWt->GetBinError(i)<<" %Unc: "<<100*h_isrWt->GetBinError(i)/h_isrWt->GetBinContent(i)<<endl;
  }

  //  h_isrWt->Draw();return;

  TH1D *h_histG, *h_histE;
  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  TLine *line1V7=new TLine( 6.5,0.35,  6.5,1.15);
  TLine *line2V7=new TLine(11.5,0.35, 11.5,1.15);
  TLine *line3V7=new TLine(16.5,0.35, 16.5,1.15);
  TLine *line4V7=new TLine(21.5,0.35, 21.5,1.15);
  TLine *line5V7=new TLine(26.5,0.35, 26.5,1.15);
  
  //  TCanvas *c_cB=new TCanvas("closure_test","closure test",1500,800); c_cB->Divide(4,2);
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];

  for(int i=0;i<name1.size();i++){
    name=name1[i]+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
  
    
    c_cA[i]->cd();
    //c_cB->cd(i+1);
    name=name1[i];
    //    h_histG=(TH1D*)f[0]->FindObjectAny(name);//h_histG->Rebin(2);
    //      h_histG->Scale(1/h_histG->Integral());
    name=name2[i];
    //    h_histE=(TH1D*)f[0]->FindObjectAny(name);//h_histE->Scale(150.9/481.6);
    //      h_histE->Scale(1/h_histE->Integral());
    
    h_isrWt->SetLineColor(kRed);
    h_isrWt->SetLineWidth(2);
    h_isrWt->SetMarkerStyle(21);
    h_isrWt->SetMarkerColor(h_isrWt->GetLineColor());
    h_isrWt->SetTitle(";Bin no.;ISR Weights");
    h_isrWt->GetYaxis()->SetLabelSize(0.05);
    h_isrWt->GetYaxis()->SetTitleSize(0.05);
    h_isrWt->GetYaxis()->SetTitleOffset(0.90);
    h_isrWt->GetYaxis()->SetRangeUser(0.35,1.2);
    //    h_isrWt->GetYaxis()->SetNdivisions(10);
    
    c_cA[i]->cd();//p_top[i]->cd();
    h_isrWt->Draw("e1");
    // legend[i]=new TLegend(0.35, 1.0,  0.65, 0.9);
    // legend[i]->SetNColumns(2);
    // name=name1[i];
    // legend[i]->AddEntry(h_isrWt,getLegName(f[0]->GetName()),"lp");
    // name=name2[i];
    // legend[i]->Draw();
    
    TString name = h_isrWt->GetName();
    line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
    c_cA[i]->SetGridx(0);	//p_top[i]->SetGridx(0);
         
    c_cA[i]->cd();   // p_top[i]->cd();
    char name2[100];
    textOnTop.SetTextSize(0.05);
    intLumiE.SetTextSize(0.05);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
    sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.73,0.91,name2);
  
    if(saveCanvas){
      name=outFileName+".png";
      c_cA[i]->SaveAs(name);
      name=outFileName+".pdf";
      c_cA[i]->SaveAs(name);
    }
  }

  gStyle->SetTextSize(2);
  //fout->cd();
//  h2_histGcopy->Write();
  cout<<"Integral in SR(0 Mu events) "<<sr_Integral<<endl
      <<"Integral in CR(1 Mu events) "<<cr_Integral<<endl;
  
  
}
TString getXaxisName(TString axname){
  if(axname.Contains("nHadJets")) return "Jets";
  else if(axname.Contains("ST")) return "ST(GeV)";
  else if(axname.Contains("BTags")) return "b-Tags";
  else if(axname.Contains("ElePt")) return "e pT(GeV)";
  else if(axname.Contains("PhotonPt")) return "#gamma pT(GeV)";
  else if(axname.Contains("mT_")) return "mT(GeV)";
  else if(axname.Contains("AllSBin")) return "Bin Number";
  else if(axname.Contains("dPhi_METjet1") || axname.Contains("dphi1_METjet1")) return "#Delta#Phi_{1}";
  else if(axname.Contains("dPhi_METjet2") || axname.Contains("dphi2_METjet2")) return "#Delta#Phi_{2}";
  else if(axname.Contains("dPhi_METBestPhoton") ) return "#Delta#Phi(MET,#gamma)";
  else if(axname.Contains("QMut") || axname.Contains("Qmut")) return "QMult";
  else if(axname.Contains("MET")) return "MET(GeV)";
  else return axname;

}

TString getLegName(TString legName1){
  if(legName1.Contains("bTagSFup")) return "SF up";
  //  else if(legName1.Contains("")) return "Central";
  else return "Central";
  
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
