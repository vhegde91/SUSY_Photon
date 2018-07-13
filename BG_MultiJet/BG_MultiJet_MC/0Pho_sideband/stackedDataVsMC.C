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

const int nfiles=7;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kBlack,kCyan,kMagenta+2,kOrange,kYellow,kGreen,kTeal+9,kPink+1,kBlue,kRed};  //Specify Colors
int col[12]={kBlack,kCyan,kTeal+9,kPink+1,kYellow,kOrange,kAzure-3,kRed,kBlue+2,kGreen,kMagenta+2,kMagenta};  //Specify Colors b's
TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getLegName(TString);
TString getXaxisName(TString);
double data_Integral=0,mc_Integral=0;
TLatex textOnTop,intLumiE;
double intLumi=35.9;
void stackedDataVsMC(TString iFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  //  gStyle->SetOptStat("nemri"); 
  f[0] = new TFile(iFname);
  f[1] = new TFile("VS_TTG_WG_ZG_GJets_ST1000_v2.root");
  //  f[1] = new TFile("VS_GJetsQCD_ST1000_v2.root");
  f[2] = new TFile("VS_SingleTop_ST1000_v2.root");
  f[3] = new TFile("VS_ZJetsToNuNu_ST1000_v2.root");
  f[4] = new TFile("VS_TTJets_ST1000_v2.root");
  f[5] = new TFile("VS_WJets_ST1000_v2.root");
  //  f[6] = new TFile("VS_QCD_ST1000_v2.root");
  f[6] = new TFile("VS_QCD_ST1000NoHighWt_v2.root");
  //f[6] = new TFile("VS_PhoPtWtd_GJetsQCD_ST1000.root");
  //f[6] = new TFile("DVS_2016_ST1000_Pred_v3.root");
  // f[7] = new TFile("VS_T5bbbbZg_1600_150_ST1000_v2.root");
  // f[8] = new TFile("VS_T5bbbbZg_1600_1550_ST1000_v2.root");
  vector<string> name1;
  vector<int> rebin;
  name1.push_back("AllSBins_v1_AB"); rebin.push_back(1);
  name1.push_back("AllSBins_v4_AB"); rebin.push_back(1);
  name1.push_back("AllSBins_v1_CD"); rebin.push_back(1);
  name1.push_back("AllSBins_v4_CD"); rebin.push_back(1);

  name1.push_back("ST_CD"); rebin.push_back(10);
  name1.push_back("MET_CD"); rebin.push_back(5);
  name1.push_back("METvarBin_CD"); rebin.push_back(1);
  name1.push_back("MET_AB"); rebin.push_back(5);
  name1.push_back("METvarBin_AB"); rebin.push_back(1);
  name1.push_back("nHadJets_CD"); rebin.push_back(1);
  name1.push_back("nBTags_AB"); rebin.push_back(1);
  name1.push_back("BestPhotonPt_CD"); rebin.push_back(1);
  name1.push_back("PhoFracPhoJet_CD"); rebin.push_back(1);
  name1.push_back("dPhi_METBestPhoton_CD"); rebin.push_back(1);
  name1.push_back("dPhi_METjet1_CD"); rebin.push_back(1);
  name1.push_back("dPhi_METjet2_CD"); rebin.push_back(1);
  name1.push_back("mTPho_CD"); rebin.push_back(2);

  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];
  THStack *hs_hist[name1.size()];
  TH1D *h_sum[name1.size()];
  // gStyle->SetTitleYSize(0.08);
  // gStyle->SetTitleYOffset(0.68);
  for(int i=0;i<name1.size();i++){
    name=name1[i];//+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    p_top[i]=new TPad(name+"_top",name+"_top",0,0.4,1,1);
    p_bot[i]=new TPad(name+"_bot",name+"_bot",0,0.0,1,0.4);
    //p_top[i]->SetLeftMargin(0.11);
    // p_top[i]->SetBottomMargin(0.25);
    // p_bot[i]->SetBottomMargin(0.25);
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.3);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy();    
    name=name1[i]+"_Stack";
    hs_hist[i] = new THStack(name,name);
    legend[i]=new TLegend(0.75, 0.90,  0.85, 0.45);
  }
  //cout<<getLegName(f[0]->GetName());
  TH1D *h_histG,*h_histE,*h_histGcopy;
  for(int i=0;i<name1.size();i++){
    c_cA[i]->cd();
    name=name1[i];
    h_histG=(TH1D*)f[0]->FindObjectAny(name);
    h_histG->Rebin(rebin[i]);
    setLastBinAsOverFlow(h_histG);
    if(name1[i]=="nHadJets_Ele") data_Integral=h_histG->Integral();
    c_cA[i]->cd();p_top[i]->cd();
    //    p_top[i]->SetTickx();p_top[i]->SetTicky();
    h_histG->SetLineColor(col[0]);
    h_histG->SetLineWidth(2);
    h_histG->SetMarkerStyle(20);
    h_histG->SetMarkerColor(h_histG->GetLineColor());
    // h_histG->SetTitle(";;Events");
    // h_histG->GetYaxis()->SetTitleOffset(0.60);    
    // h_histG->GetYaxis()->SetTitleSize(0.08);
    // h_histG->GetYaxis()->SetLabelSize(0.08);
    //h_histG->Draw();
    legend[i]->AddEntry(h_histG,"Data","lep");
    //gPad->Update();
    // gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
      
    for(int p=1;p<nfiles;p++){  
      name=name1[i];
      h_histE=(TH1D*)f[p]->FindObjectAny(name);
      h_histE->Rebin(rebin[i]);
      setLastBinAsOverFlow(h_histE);
      if(name1[i]=="nHadJets_Ele"){
        mc_Integral+=h_histE->Integral();
        cout<<f[p]->GetName()<<" # events "<<h_histE->Integral()<<endl;
      }
      h_histE->SetLineColor(col[p]);
      // h_histE->SetLineWidth(2);
      h_histE->SetMarkerStyle(21);
      h_histE->SetMarkerColor(h_histE->GetLineColor());
      h_histE->SetFillColor(h_histE->GetLineColor());
      hs_hist[i]->Add(h_histE);
      if(p==1){
	name=name1[i]+"_Sum";
	h_sum[i] = (TH1D*)h_histE->Clone(name);
      }
      else h_sum[i]->Add(h_histE);
      legend[i]->AddEntry(h_histE,getLegName(f[p]->GetName()),"f");
    }
    c_cA[i]->cd();    p_top[i]->cd();
    hs_hist[i]->SetMinimum(0.8);
    hs_hist[i]->SetMaximum(h_histG->GetMaximum()*10);//Integral()*10);
    hs_hist[i]->Draw("BAR");
    h_histG->Draw("same");
    hs_hist[i]->SetTitle(";;Events");
    hs_hist[i]->GetYaxis()->SetTitleOffset(0.60);    
    hs_hist[i]->GetYaxis()->SetTitleSize(0.09);
    hs_hist[i]->GetYaxis()->SetLabelSize(0.09);
    
    c_cA[i]->Modified();
    c_cA[i]->Update();
    gPad->RedrawAxis();
    legend[i]->Draw();

    TH1D *h_numr=(TH1D*)h_histG->Clone();
    h_numr->Divide(h_sum[i]);
    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    h_numr->SetTitle(0);
    name=getXaxisName(name1[i]);
    h_numr->GetXaxis()->SetTitle(name);
    h_numr->GetXaxis()->SetTitleOffset(0.87);
    h_numr->GetXaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetLabelSize(0.14);

    h_numr->GetYaxis()->SetTitle("#frac{Data}{MC}");
    h_numr->GetYaxis()->SetTitleOffset(0.35);
    h_numr->GetYaxis()->SetTitleSize(0.13);
    h_numr->GetYaxis()->SetLabelSize(0.14);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->SetMaximum(2.0);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    p_bot[i]->SetTickx();p_bot[i]->SetTicky();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e0");

    c_cA[i]->cd();    p_top[i]->cd();

    char name2[100];
    textOnTop.SetTextSize(0.06);
    intLumiE.SetTextSize(0.06);
    textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Preliminary}}");
    intLumiE.SetTextSize(0.06);
    sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.73,0.91,name2);
    if(saveCanvas){name="c_"+name1[i]+".png";c_cA[i]->SaveAs(name);}
    
  }
  
  cout<<"Integral in Data "<<data_Integral<<endl
      <<"Integral in MC "<<mc_Integral<<endl;
}

TString getLegName(TString fname){

  if(fname.Contains("ZJetsToNuNu")){fname="Z(#rightarrow#nu#bar{#nu})+jets";}
  else if(fname.Contains("DYJetsToLL")){fname="DY( #rightarrow l^{+}l^{-})";}
  else if(fname.Contains("SingleTop")){fname="t+jets";}
  else if(fname.Contains("WJets")){fname="W(#rightarrowl#nu)+jets";}
  else if(fname.Contains("TTJets")){fname="t #bar{t}";}
  else if(fname.Contains("WGJetsToLNuG")){fname="W(#rightarrowl#nu)+#gamma";}
  else if(fname.Contains("ZGJetsToNuNuG")){fname="Z(#rightarrow #nu #bar{#nu})+ #gamma";}
  else if(fname.Contains("TTGJets")){fname="t #bar{t}+ #gamma";}
  else if(fname.Contains("QCD")){fname="QCD";}
  else if(fname.Contains("TTG_WG_ZG_GJets")){fname="X+#gamma +jets";}
  else if(fname.Contains("T5bbbbZg_1600_150")){fname="T5bbbb_ZG_150";}
  else if(fname.Contains("T5bbbbZg_1600_1000")){fname="T5bbbb_ZG_1000";}
  else if(fname.Contains("T5bbbbZg_1600_1550")){fname="T5bbbb_ZG_1550";}

  return fname;
  // if(fname=="CS_TTJets_FR_v2.root" || fname=="CS_TTJets_FR_NoTrg_v2.root" || fname.Contains("TTJets")) return "t#bar{t}";
  // else if(fname=="CS_WJets_FR_v2.root" || fname=="CS_WJets_FR_NoTrg_v2.root") return "W+Jets";
  // else if(fname=="CS_ST_FR_v2.root" || fname=="CS_ST_FR_NoTrg_v2.root") return "Single top";
  // else if(fname=="CS_QCD_FR_v2.root" || fname=="CS_QCD_FR_NoTrg_v2.root") return "QCD";
  // else return fname;
}

TString getXaxisName(TString axname){
  if(axname.Contains("nHadJets")) return "Jets";
  //  else if(axname.Contains("MET")) return "MET(GeV)";
  else if(axname.Contains("ST")) return "ST(GeV)";
  else if(axname.Contains("BTags")) return "b-Tags";
  else if(axname.Contains("ElePt")) return "e pT(GeV)";
  else if(axname.Contains("PhotonPt")) return "#gamma pT(GeV)";
  else if(axname.Contains("mT_")) return "mT(GeV)";
  else if(axname.Contains("AllSBin")) return "Bin Number";
  else if(axname.Contains("QMut") || axname.Contains("Qmut")) return "QMult";
  else return axname;

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

