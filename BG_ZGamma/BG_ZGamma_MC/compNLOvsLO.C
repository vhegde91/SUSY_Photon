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

void compNLOvsLO(){
  TH1::SetDefaultSumw2(1);

  bool saveCanvas=0;
  vector<TString> legNames, name2;
  vector<int> rebin;
  double nunu_Integral=0,ll_Integral=0;

  //  TFile *fn=new TFile("CS_ZGToNuNuG_PtG130_NLO.root");
  //  TFile *fd=new TFile("CS_ZGToNuNuG_PtG130_LO.root");

  //  TFile *fd=new TFile("CS_ZGToNuNuG_PtG130_LO_reWtnJ_NLO.root");

  TFile *fn=new TFile("CS_ZGToNuNuG_LO_PtG150.root");//CS_ZGZJToNuNuG_LO_PtG150.root");
  TFile *fd=new TFile("CS_ZGToLLG_LO_PtG150.root");//CS_ZDYToLLG_LO_PtG150.root");

  // TFile *fn=new TFile("/home/vinay/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/CS_ZGamma/CS_ZGToNuNuG_Summer16.ZGTo2NuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root");
  // TFile *fd=new TFile("/home/vinay/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/CS_ZGamma/CS_ZJetsToNuNu_Summer16.ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph.root");
  

  name=fd->GetName();
  name="TF_"+name;
  TFile *fout=new TFile(name,"RECREATE");
  
  TString histToSave="AllSBins_v7";
  // TFile *fd=new TFile("CS_ZGToLLG.root");

  name2.push_back("MET");  rebin.push_back(5);
  name2.push_back("METvarBin");  rebin.push_back(1);
  name2.push_back("ST");  rebin.push_back(10);
  name2.push_back("BestPhotonPt");  rebin.push_back(5);
  name2.push_back("nHadJets");  rebin.push_back(1);
  name2.push_back("nBTags");  rebin.push_back(1);
  name2.push_back("AllSBins_v7");  rebin.push_back(1);

  // name2.push_back("LeadGenPhoPt");  rebin.push_back(100);
  // name2.push_back("BestPhotonPt");  rebin.push_back(10);
  
  //  TString nameN="Z#gamma#rightarrow#nu#bar{#nu}#gamma";
  TString nameN="NLO";
  //TString nameN="Data";
  //  TString nameD="Z#gamma#rightarrow l^{+}l^{-}#gamma"; 
  TString nameD="LO"; 
  //TString nameD="MC";
  TLegend *legend[name2.size()];
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);

  // txtTop="CMS #it{#bf{Simulation}}";
  // txtLumi="#bf{35.9 fb^{-1}(13TeV)}";
  // textOnTop.SetTextSize(0.035);
  // intLumiE.SetTextSize(0.035);
  // textOnTop.DrawLatexNDC(0.1,0.91,txtTop);
  //  sprintf(name2,"#bf{35.9 fb^{-1}(13TeV)}");
  //  intLumiE.DrawLatexNDC(0.73,0.91,name2);
  TCanvas *c_cA[name2.size()];
  TPad *p_top[name2.size()],*p_bot[name2.size()];

  for(int i=0;i<name2.size();i++){
    //    name = name2[i]+"NuNu_LL";
    name = name2[i]+"NLO_LO";
    c_cA[i] = new TCanvas(name,name,1500,800);
    p_top[i] =new TPad(name+"top",name+"top",0,0.4,1,1);
    p_bot[i] =new TPad(name+"bot",name+"bot",0,0.0,1,0.4);
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.35);
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
      h_num->SetTitle(";;Events");
      h_num->GetYaxis()->SetLabelSize(0.09);
      h_num->GetYaxis()->SetTitleSize(0.09);
      h_num->GetYaxis()->SetTitleOffset(0.45);
      
      //      h_den->SetLineColor(kPink-2);//kMagenta+2
      h_den->SetLineColor(kMagenta+2);//MC
      h_den->SetLineWidth(2);
      h_den->SetMarkerStyle(21);
      h_den->SetMarkerColor(h_den->GetLineColor());
      //  h_num->GetYaxis()->SetNdivisions(2);

      c_cA[i]->cd();p_top[i]->cd();
      // textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Simulation}}");
      // textOnTop.DrawLatexNDC(0.1,0.91,txtTop);
      // intLumiE.DrawLatexNDC(0.73,0.91,txtLumi);
      // gPad->Update();
      p_top[i]->SetTickx();p_top[i]->SetTicky();
      h_num->Draw("E1");
      h_den->Draw("E1 same");

      legend[i]=new TLegend(0.74, 0.64,  0.94, 0.89);
      legend[i]->AddEntry(h_num,nameN,"elp");
      legend[i]->AddEntry(h_den,nameD,"elp");
      legend[i]->Draw();
      gPad->RedrawAxis();
      //---------------- for ratio ------------------
      h_numCp=(TH1D*)h_num->Clone("Ratio_NuNuToLL");
      h_numCp->Divide(h_den);
      if(name2[i]==histToSave){fout->cd();h_numCp->Write();}
      h_numCp->SetLineColor(kBlack);
      h_numCp->SetMarkerColor(kBlack);
      h_numCp->SetMaximum(1.99);////////////////////////////////////
      h_numCp->SetMinimum(0.5);

      h_numCp->GetXaxis()->SetTitle(getXaxisName(name2[i]));
      h_numCp->GetXaxis()->SetLabelSize(0.15);
      h_numCp->GetXaxis()->SetTitleSize(0.16);
      h_numCp->GetXaxis()->SetTitleOffset(0.90);

      //      h_numCp->GetYaxis()->SetTitle("#frac{#nu#bar{#nu}}{l^{+}l^{-}}");
      //      h_numCp->GetYaxis()->SetTitle("#frac{Data}{MC}");
      h_numCp->GetYaxis()->SetTitle("#frac{NLO}{LO}");
      h_numCp->GetYaxis()->SetTitleOffset(0.35);
      h_numCp->GetYaxis()->SetTitleSize(0.13);
      h_numCp->GetYaxis()->SetLabelSize(0.14);
      h_numCp->GetYaxis()->SetNdivisions(505);

      c_cA[i]->cd();    p_bot[i]->cd();
      p_bot[i]->SetTickx();p_bot[i]->SetTicky();
      h_numCp->Draw("e1");
      c_cA[i]->cd();    p_top[i]->cd();
      char name3[100];
      textOnTop.SetTextSize(0.07);
      intLumiE.SetTextSize(0.07);
      textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
      sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
      intLumiE.DrawLatexNDC(0.73,0.91,name3);

      if(saveCanvas){name="c_"+name2[i]+".png";c_cA[i]->SaveAs(name);}
    }
  }
  cout<<"# of events in NLO: "<<nunu_Integral<<endl;
  cout<<"# of events in LO: "<<ll_Integral<<endl;
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
