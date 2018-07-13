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

const int nfiles=5;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kBlack,kPink-2,kOrange,kYellow,kGreen,kTeal+9,kPink+1,kCyan,kBlue,kRed};  //Specify Colors
int col[10]={kMagenta+2,kOrange,kYellow,kGreen,kTeal+9,kPink+1,kCyan,kBlue,kRed,kBlack};  //Specify Colors

//int col[10]={kBlack,kBlue,kOrange,kYellow,kGreen,kCyan,kPink+1,kTeal+9,kBlue,kRed};  //Specify Colors
//int col[10]={kBlack,kPink-2,kGreen,kYellow,kBlue,kCyan,kPink+1,kCyan,kBlue,kRed};  //Specify Colors

TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
void setMyRange(TH1D*,double,double);
void setMyRange(THStack*,double,double);
TString getLegName(TString);
TString getXaxisName(TString);
double data_Integral=0,mc_Integral=0;
TLatex textOnTop,intLumiE;
double intLumi=35.9;
void mcStacked(){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  //  gStyle->SetOptStat("nemri"); 
  f[0] = new TFile("CS_TGZGDY_LostMuHadTau_v2.root");
  f[1] = new TFile("CS_WJets_LostMuHadTau_v2.root");
  f[2] = new TFile("CS_TTJets_LostMuHadTau_v2.root");
  f[3] = new TFile("CS_TTG_LostMuHadTau_v2.root");
  f[4] = new TFile("CS_WG_LostMuHadTau_v2.root");
  
  vector<string> name1;
  vector<int> rebin;
  vector<double> xLow,xHigh;
  //  name1.push_back("ST_Mu1");  rebin.push_back(10);   xLow.push_back(0); xHigh.push_back(3000); 
  name1.push_back("MET_Mu1");    rebin.push_back(2);    xLow.push_back(0); xHigh.push_back(800); 
  name1.push_back("nBTags_Mu1");   rebin.push_back(1);  xLow.push_back(0); xHigh.push_back(8);  
  name1.push_back("nHadJets_Mu0");  rebin.push_back(1);  xLow.push_back(0); xHigh.push_back(12);
  name1.push_back("nHadJets_Mu1");  rebin.push_back(1);  xLow.push_back(0); xHigh.push_back(12);
  // name1.push_back("BestPhotonPt_Mu1");   rebin.push_back(5); xLow.push_back(0); xHigh.push_back(1000);
  // name1.push_back("METvarBin_Mu1");  rebin.push_back(1); xLow.push_back(-100000); xHigh.push_back(1000000);
  // name1.push_back("dR_MuPho");   rebin.push_back(2);     xLow.push_back(0); xHigh.push_back(6);
  // name1.push_back("MuPt");   rebin.push_back(5);         xLow.push_back(0); xHigh.push_back(500);
  // name1.push_back("MT_Mu");   rebin.push_back(10);       xLow.push_back(0); xHigh.push_back(300);
  // name1.push_back("AllSBins_v7_Mu1");   rebin.push_back(1);   xLow.push_back(-10000); xHigh.push_back(300000);

  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  THStack *hs_hist[name1.size()];
  TH1D *h_sum[name1.size()];
  // gStyle->SetTitleYSize(0.08);
  // gStyle->SetTitleYOffset(0.68);
  for(int i=0;i<name1.size();i++){
    name=name1[i];//+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    c_cA[i]->Draw();c_cA[i]->SetGridx();c_cA[i]->SetGridy();c_cA[i]->SetLogy();
    //   c_cA[i]->SetBottomMargin(0);
    name=name1[i]+"_Stack";
    hs_hist[i] = new THStack(name,name);
    legend[i]=new TLegend(0.7, 0.87,  0.85, 0.65);
    //    legend[i]=new TLegend(0.55, 0.88,  0.85, 0.7);
    //legend[i]->SetBorderSize(0);
  }
  //cout<<getLegName(f[0]->GetName());
  TH1D *h_histE;
  for(int i=0;i<name1.size();i++){
    c_cA[i]->cd();
    for(int p=0;p<nfiles;p++){  
      name=name1[i];
      h_histE=(TH1D*)f[p]->FindObjectAny(name);
      h_histE->Rebin(rebin[i]);
      setLastBinAsOverFlow(h_histE);
      setMyRange(h_histE,xLow[i],xHigh[i]);
      if(xLow[i] > -5000 && xHigh[i] < 5000) h_histE->GetXaxis()->SetRangeUser(xLow[i],xHigh[i]);
      if(name1[i]=="nHadJets_Mu1"){
        mc_Integral+=h_histE->Integral();
        cout<<f[p]->GetName()<<" # events "<<h_histE->Integral()<<endl;
      }
      h_histE->SetLineColor(kBlack);
      h_histE->SetLineWidth(1);

      h_histE->SetMarkerColor(col[p]);
      h_histE->SetFillColor(col[p]);

      hs_hist[i]->Add(h_histE);
      if(p==0){
	name=name1[i]+"_Sum";
	h_sum[i] = (TH1D*)h_histE->Clone(name);
      }
      else h_sum[i]->Add(h_histE);
      legend[i]->AddEntry(h_histE,getLegName(f[p]->GetName()),"f");
    }
    c_cA[i]->cd();    c_cA[i]->cd();
    hs_hist[i]->SetMinimum(0.8);
    hs_hist[i]->SetMaximum(5*hs_hist[i]->GetMaximum());
    hs_hist[i]->Draw("HIST");
    
    h_sum[i]->SetFillColor(1);
    h_sum[i]->SetFillStyle(3013);
    h_sum[i]->Draw("e2 same");

    if(xLow[i] > -5000 && xHigh[i] < 5000) hs_hist[i]->GetXaxis()->SetRangeUser(xLow[i],xHigh[i]);
    hs_hist[i]->SetTitle(0);
    hs_hist[i]->GetYaxis()->SetNdivisions(5);
    hs_hist[i]->GetYaxis()->SetTitle("Events");
    hs_hist[i]->GetYaxis()->SetTitleOffset(0.75);    
    hs_hist[i]->GetYaxis()->SetTitleSize(0.06);
    hs_hist[i]->GetYaxis()->SetLabelSize(0.06);
    
    hs_hist[i]->GetXaxis()->SetTitle(getXaxisName(name));
    hs_hist[i]->GetXaxis()->SetTitleOffset(0.9);
    hs_hist[i]->GetXaxis()->SetTitleSize(0.06);
    hs_hist[i]->GetXaxis()->SetLabelSize(0.06);

    c_cA[i]->Modified();
    c_cA[i]->Update();
    //    legend[i]->SetNColumns(3);
    legend[i]->Draw();

    c_cA[i]->cd();    c_cA[i]->cd(); gPad->RedrawAxis();
    char name2[100];
    textOnTop.SetTextSize(0.05);
    intLumiE.SetTextSize(0.05);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
    sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.7,0.91,name2);
    if(saveCanvas){name=name1[i]+".pdf";c_cA[i]->SaveAs(name);}
    
  }
  
  cout<<"Integral in Data "<<data_Integral<<endl
      <<"Integral in MC "<<mc_Integral<<endl;
}

TString getLegName(TString fname){
  if(fname=="CS_TTG_LostMuHadTau_v2.root") return "t#bar{t}+#gamma";
  else if(fname=="CS_TTJets_LostMuHadTau_v2.root") return "t#bar{t}";
  else if(fname=="CS_WG_LostMuHadTau_v2.root") return "W#gamma";
  else if(fname=="CS_WJets_LostMuHadTau_v2.root") return "W+Jets";
  else if(fname=="CS_TG_LostMuHadTau_v2.root") return "t+#gamma";
  else if(fname=="CS_TGZGDY_LostMuHadTau_v2.root") return "t#gamma+Z#gamma+DY";
  else return fname;
}
TString getXaxisName(TString axname){
  if(axname.Contains("nHadJets")) return "Jets";
  else if(axname.Contains("ST")) return "ST(GeV)";
  else if(axname.Contains("BTags")) return "b tags";
  else if(axname.Contains("ElePt")) return "e pT(GeV)";
  else if(axname.Contains("MuPt")) return "#mu pT(GeV)";
  else if(axname.Contains("MuEta")) return "#mu #eta";
  else if(axname.Contains("PhotonPt")) return "#gamma pT(GeV)";
  else if(axname.Contains("PhotonEta")) return "#gamma #eta";
  else if(axname.Contains("PhotonPhi")) return "#gamma #Phi";
  else if(axname.Contains("mT")) return "mT(#gamma,p_{T}^{miss})(GeV)";
  else if(axname.Contains("dR_MuPho")) return "#DeltaR(#mu,#gamma)";
  else if(axname.Contains("AllSBin")) return "Bin Number";
  else if(axname.Contains("nVtx")) return "No. of primary vertices";
  else if(axname.Contains("dPhi_METjet1") || axname.Contains("dphi1_METjet1")) return "#Delta#Phi_{1}";
  else if(axname.Contains("dPhi_METjet2") || axname.Contains("dphi2_METjet2")) return "#Delta#Phi_{2}";
  else if(axname.Contains("dPhi_METBestPhoton") ) return "#Delta#Phi(MET,#gamma)";
  else if(axname.Contains("dPhi_Muon_Photon") ) return "#Delta#Phi(#mu,#gamma)";
  else if(axname.Contains("QMut") || axname.Contains("Qmut")) return "QMult";
  else if(axname.Contains("MT_Mu")) return "mT(#mu,p_{T}^{miss})(GeV)";
  else if(axname.Contains("MET")) return "p_{T}^{miss}(GeV)";
  else return axname;

}

void setMyRange(TH1D *h1,double xLow,double xHigh){
  double err=0;
  if(xHigh > 13000) return;
  if(xLow < 13000) return;
  int nMax=h1->FindBin(xHigh);
  h1->SetBinContent(nMax,h1->IntegralAndError(nMax,h1->GetNbinsX(),err));
  h1->SetBinError(nMax,err);
  for(int i=nMax+1;i<=h1->GetNbinsX()+1;i++){
    h1->SetBinContent(i,0);
    h1->SetBinError(i,0);
  }
  //  h1->GetXaxis()->SetRangeUser(xLow,xHigh);
}
void setMyRange(THStack *h1,double xLow,double xHigh){
  // double err=0;
  // if(xHigh > 13000) return;
  // int nMax=h1->FindBin(xHigh);
  // h1->SetBinContent(nMax,h1->IntegralAndError(nMax,h1->GetNbinsX(),err));
  // h1->SetBinError(nMax,err);
  // for(int i=nMax+1;i<=h1->GetNbinsX()+1;i++){
  //   h1->SetBinContent(i,0);
  //   h1->SetBinError(i,0);
  // }
  //h1->GetXaxis()->SetRangeUser(xLow,xHigh);
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
