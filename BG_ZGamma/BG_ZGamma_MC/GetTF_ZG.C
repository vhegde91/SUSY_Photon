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

void setLastBinAsOverFlow(TH1D*);

void GetTF_ZG(){
  TH1::SetDefaultSumw2(1);

  bool saveCanvas=0;
  vector<TString> legNames, name2;
  vector<int> rebin;
  double nunu_Integral=0,ll_Integral=0;
  //  TFile *fn=new TFile("DCS_ZGToLL.root");  
  //  TFile *fn=new TFile("DCS_ZGToLL_NoZMassWindow.root");  

  TFile *fn=new TFile("CS_ZGZJToNuNuG.root");
  TFile *fd=new TFile("CS_ZDYToLLG.root");
  //  TFile *fd=new TFile("CS_ZDYToLLG_NoZMassWindow.root");

  // TFile *fn=new TFile("Summer16.ZJetsToNuNu_HT-100ToInf_13TeV-madgraph.root");
  // TFile *fd=new TFile("Summer16.DYJetsToLL_M-50_HT-100ToInf.root");

  // TFile *fn=new TFile("Summer16.ZGTo2NuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root");
  // TFile *fd=new TFile("Summer16.ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext1.root");
  // TFile *fn=new TFile("Summer16.ZGTo2NuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root");
  // TFile *fd=new TFile("Summer16.ZGTo2LG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root");
  
  //  TFile *fd=new TFile("CS_ZDYToLLG_AppTF.root");
    
  name=fd->GetName();
  name="TF_"+name;
  TFile *fout=new TFile(name,"RECREATE");
  
  TString histToSave="nBTagsvarBin";
  // TFile *fd=new TFile("CS_ZGToLLG.root");

  name2.push_back("MET");  rebin.push_back(10);
  name2.push_back("METvarBin");  rebin.push_back(1);
  //  name2.push_back("METvarBin_TF");  rebin.push_back(1);
  name2.push_back("ST");  rebin.push_back(10);
  name2.push_back("BestPhotonPt");  rebin.push_back(5);
  name2.push_back("nHadJets");  rebin.push_back(1);
  name2.push_back("nBTags");  rebin.push_back(1);
  // name2.push_back("nGenbs");  rebin.push_back(1);
  // name2.push_back("bParent_PdgId");  rebin.push_back(1);
  //  name2.push_back("ZMass");  rebin.push_back(2);
  name2.push_back("nBTagsvarBin"); rebin.push_back(1);
  //  name2.push_back("AllSBins_ZG"); rebin.push_back(1);
  // name2.push_back("STvarBin"); rebin.push_back(1);
  // name2.push_back("MET_R1");  rebin.push_back(1);
  // name2.push_back("MET_R2");  rebin.push_back(1);
  // name2.push_back("MET_R3");  rebin.push_back(1);
  // name2.push_back("Ele1Pt");  rebin.push_back(5);
  // name2.push_back("Ele2Pt");  rebin.push_back(5);
  // name2.push_back("Mu1Pt");  rebin.push_back(5);
  // name2.push_back("Mu2Pt");  rebin.push_back(5);

  // name2.push_back("METPhi");  rebin.push_back(1);
  // name2.push_back("dPhi_METBestPhoton");  rebin.push_back(1);
  // name2.push_back("dPhi_METjet1");  rebin.push_back(1);
  // name2.push_back("dPhi_METjet2");  rebin.push_back(1);
  
  TString nameN="Z#gamma#rightarrow#nu#bar{#nu}#gamma";
  //TString nameN="Data";
  TString nameD="Z#gamma#rightarrow l^{+}l^{-}#gamma"; 
  TLegend *legend[name2.size()];
  gStyle->SetOptStat(0);
  
  txtTop="CMS #it{#bf{Simulation}}";
  txtLumi="#bf{35.9 fb^{-1}(13TeV)}";
  textOnTop.SetTextSize(0.035);
  intLumiE.SetTextSize(0.035);
  // textOnTop.DrawLatexNDC(0.1,0.91,txtTop);
  //  sprintf(name2,"#bf{35.9 fb^{-1}(13TeV)}");
  //  intLumiE.DrawLatexNDC(0.73,0.91,name2);
  TCanvas *c_cA[name2.size()];
  TPad *p_top[name2.size()],*p_bot[name2.size()];

  for(int i=0;i<name2.size();i++){
    name = name2[i]+"NuNu_LL";
    //    name = name2[i]+"DataMC_LL";
    c_cA[i] = new TCanvas(name,name,1500,1000);    
    p_top[i] =new TPad(name+"top",name+"top",0,0.4,1,1);
    p_bot[i] =new TPad(name+"bot",name+"bot",0,0.04,1,0.4);
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.3);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy();
    //    textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Simulation}}");
  }

  TH1D *h_num,*h_den,*h_numCp;
  for(int i=0;i<name2.size();i++){
    h_num=(TH1D*)fn->FindObjectAny(name2[i]);
    h_den=(TH1D*)fd->FindObjectAny(name2[i]);
    h_num->Rebin(rebin[i]);
    h_den->Rebin(rebin[i]);
    //    h_den->Scale(6.0);
    if(h_num && h_den){
      if(name2[i]=="nHadJets"){
	nunu_Integral=h_num->Integral();
	ll_Integral=h_den->Integral();
      }
      setLastBinAsOverFlow(h_num);
      setLastBinAsOverFlow(h_den);
      
      h_num->SetLineColor(kRed);
      h_num->SetLineWidth(2);
      h_num->SetMarkerStyle(21);
      h_num->SetMarkerColor(h_num->GetLineColor());
      h_num->SetTitle(";;Events");
      h_num->GetYaxis()->SetLabelSize(0.09);
      h_num->GetYaxis()->SetTitleSize(0.09);
      h_num->GetYaxis()->SetTitleOffset(0.45);
      
      //      h_den->SetLineColor(kBlue);//kMagenta+2
      h_den->SetLineColor(kMagenta+2);//MC
      h_den->SetLineWidth(2);
      h_den->SetMarkerStyle(20);
      h_den->SetMarkerColor(h_den->GetLineColor());

      c_cA[i]->cd();p_top[i]->cd();
      // textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Simulation}}");
      // textOnTop.DrawLatexNDC(0.1,0.91,txtTop);
      // intLumiE.DrawLatexNDC(0.73,0.91,txtLumi);
      // gPad->Update();
      p_top[i]->SetTickx();p_top[i]->SetTicky();
      h_num->Draw();
      h_den->Draw("same");

      legend[i]=new TLegend(0.70, 0.90,  0.90, 0.65);
      legend[i]->AddEntry(h_num,nameN,"lp");
      legend[i]->AddEntry(h_den,nameD,"lp");
      legend[i]->Draw();
      //---------------- for ratio ------------------
      h_numCp=(TH1D*)h_num->Clone("Ratio_NuNuToLL");
      h_numCp->Divide(h_den);
      if(name2[i]==histToSave){fout->cd();h_numCp->Write();}
      h_numCp->SetLineColor(kBlack);
      h_numCp->SetMarkerColor(kBlack);
      h_numCp->SetMaximum(2.0);////////////////////////////////////
      h_numCp->SetMinimum(0.01);

      h_numCp->GetXaxis()->SetTitle(name2[i]);
      h_numCp->GetXaxis()->SetLabelSize(0.15);
      h_numCp->GetXaxis()->SetTitleSize(0.16);
      h_numCp->GetXaxis()->SetTitleOffset(0.80);

      h_numCp->GetYaxis()->SetTitle("#frac{#nu#bar{#nu}}{l^{+}l^{-}}");
      //      h_numCp->GetYaxis()->SetTitle("#frac{Data}{MC(l^{+}l^{-})}");
      h_numCp->GetYaxis()->SetTitleOffset(0.28);
      h_numCp->GetYaxis()->SetTitleSize(0.165);
      h_numCp->GetYaxis()->SetLabelSize(0.16);
      h_numCp->GetYaxis()->SetNdivisions(505);

      c_cA[i]->cd();    p_bot[i]->cd();
      p_bot[i]->SetTickx();p_bot[i]->SetTicky();
      h_numCp->Draw("e0");
      c_cA[i]->cd();    p_top[i]->cd();
      char name3[100];
      textOnTop.SetTextSize(0.06);
      intLumiE.SetTextSize(0.06);
      textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Preliminary}}");
      intLumiE.SetTextSize(0.06);
      sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
      intLumiE.DrawLatexNDC(0.73,0.91,name3);

      if(saveCanvas){name="c_"+name2[i]+".png";c_cA[i]->SaveAs(name);}
    }
  }
  cout<<"# of events in NuNu: "<<nunu_Integral<<endl;
  cout<<"# of events in LL: "<<ll_Integral<<endl;
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
