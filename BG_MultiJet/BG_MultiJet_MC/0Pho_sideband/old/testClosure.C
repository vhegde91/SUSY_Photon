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

const int nfiles=1,nBG=1;    //Specify no. of files
TLatex textOnTop,intLumiE;
TFile *f[nfiles];
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kCyan,kBlue,kTeal+9,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
TString name;
void setLastBinAsOverFlow(TH1D*);

void testClosure(TString inFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  //  f[0] = new TFile("CS_WJetsToLNu.root");
  // f[0] = new TFile("CS_TTJets.root");
  //  f[0] = new TFile("CS_TTW_FR_clsr_FRDY.root");
  //f[0] = new TFile("CS_TTW_FR_clsr_EMPtQMult.root");
  //  f[0] = new TFile("CS_TTW_FR_clsr_EMPtQMult_FR_DY_IgoreTagE.root");
  f[0] = new TFile(inFname);
  //  f[0] = new TFile("CS_WJets_FR_clsr_EMPtQMult_FR_DY_IgoreTagE.root");

  //  gStyle->SetOptStat("nemri");
  double intLumi=35.9;  
  vector<string> name1,name2;
  vector<int> rebin;
  name1.push_back("ST_CD");          name2.push_back("ST_AB");      rebin.push_back(10);
  name1.push_back("STvarBin_CD");          name2.push_back("STvarBin_AB");      rebin.push_back(1);
  name1.push_back("MET_CD");         name2.push_back("MET_AB");     rebin.push_back(2);
  name1.push_back("GenMET_CD");         name2.push_back("GenMET_AB");     rebin.push_back(2);
  name1.push_back("nHadJets_CD");    name2.push_back("nHadJets_AB");rebin.push_back(1);
  name1.push_back("nJets_CD");      name2.push_back("nJets_AB");  rebin.push_back(1);
  //  name1.push_back("BestPhotonPt");    name2.push_back("ElePt");       rebin.push_back(10);
  name1.push_back("METvarBin_CD");   name2.push_back("METvarBin_AB");rebin.push_back(1);
  name1.push_back("jet1Pt_CD");   name2.push_back("jet1Pt_AB");rebin.push_back(5);
  name1.push_back("jet2Pt_CD");   name2.push_back("jet2Pt_AB");rebin.push_back(5);
  // name1.push_back("dPhi_METjet1_CD");   name2.push_back("dPhi_METjet1_AB");rebin.push_back(1);
  // name1.push_back("dPhi_METjet2_CD");   name2.push_back("dPhi_METjet2_AB");rebin.push_back(1);
  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  //  TCanvas *c_cB=new TCanvas("closure_test","closure test",1500,800); c_cB->Divide(4,2);
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];

  for(int i=0;i<name1.size();i++){
    name=name1[i]+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    p_top[i]=new TPad(name+"_top",name+"_top",0,0.35,1,1);
    p_bot[i]=new TPad(name+"_bot",name+"_bot",0,0.05,1,0.35);
    //    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->Draw();p_top[i]->SetGrid(1,1);p_top[i]->SetLogy();
    //    gStyle->SetGridStyle(1);
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.3);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy();    

    
    TH1D *h_histG,*h_histE,*h_histGcopy;
    for(int j=0;j<nfiles;j++){
      c_cA[i]->cd();
      //c_cB->cd(i+1);
      name=name1[i];
      h_histG=(TH1D*)f[j]->FindObjectAny(name);//h_histG->Rebin(2);
      h_histG->Rebin(rebin[i]);
      h_histG->Scale(1./h_histG->Integral());
          
      name=name2[i];
      h_histE=(TH1D*)f[j]->FindObjectAny(name);//h_histE->Rebin(2);
      h_histE->Rebin(rebin[i]);
      h_histE->Scale(1.0/h_histE->Integral());
     
      if(h_histG && h_histE){

	setLastBinAsOverFlow(h_histG);
	setLastBinAsOverFlow(h_histE);

	h_histG->SetLineColor(col[0]);
	h_histG->SetLineWidth(2);
	h_histG->SetMarkerStyle(21);
	h_histG->SetMarkerColor(h_histG->GetLineColor());
	h_histG->GetYaxis()->SetLabelSize(0.07);
	
	h_histE->SetLineColor(col[1]);
	h_histE->SetLineWidth(2);
	h_histE->SetMarkerStyle(20);
	h_histE->SetMarkerColor(h_histE->GetLineColor());

	c_cA[i]->cd();p_top[i]->cd();
	//c_cB->cd(i+1);p_top[i]->cd();
	h_histG->Draw();
	h_histE->Draw("sames");
	h_histG->SetTitle(0);
	legend[i]=new TLegend(0.75, 0.90,  0.90, 0.75);
	name=name1[i];
	legend[i]->AddEntry(h_histG,"CD(#gamma)","l");
	name=name2[i];
	legend[i]->AddEntry(h_histE,"AB(No #gamma)","l");
	legend[i]->Draw();
      }
    }

    char name3[100];
    textOnTop.SetTextSize(0.06);
    intLumiE.SetTextSize(0.06);
    textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Simulation}}");
    intLumiE.SetTextSize(0.06);
    sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.73,0.91,name3);
    
    TH1D *h_numr=(TH1D*)h_histG->Clone();
    // if(name1[i]=="MET_CD") {cout<<"numr Ele:"<<h_numr->GetBinContent(7)<<" numr error:"<<h_numr->GetBinError(7)<<" den pho:"<<h_histG->GetBinContent(7)<<" error:"<<h_histG->GetBinError(7)<<endl;
    // }
    h_numr->Divide(h_histE);
    //    if(name1[i]=="MET_CD") {cout<<"Ele:"<<h_numr->GetBinContent(7)<<" error:"<<h_numr->GetBinError(7)<<endl;}
    string xLabel=h_histE->GetName();
    xLabel.pop_back();    xLabel.pop_back();    xLabel.pop_back();    xLabel.pop_back();
    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    h_numr->SetTitle(";;#gamma/No #gamma");
    h_numr->GetYaxis()->SetTitleOffset(0.21);
    h_numr->GetYaxis()->SetTitleSize(0.14);
    h_numr->GetXaxis()->SetLabelSize(0.15);
    h_numr->GetXaxis()->SetTitle(xLabel.c_str());
    h_numr->GetXaxis()->SetTitleSize(0.16);
    h_numr->GetXaxis()->SetTitleOffset(0.70);
    h_numr->GetYaxis()->SetLabelSize(0.13);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->SetMaximum(2.);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e0");
  }

  gStyle->SetTextSize(2);
  //fout->cd();
  //  h2_histGcopy->Write();



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
/*


	double lastBinCt =h_histG->GetBinContent(h_histG->GetNbinsX()),overflCt =h_histG->GetBinContent(h_histG->GetNbinsX()+1);
	double lastBinErr=h_histG->GetBinError(h_histG->GetNbinsX()),  overflErr=h_histG->GetBinError(h_histG->GetNbinsX()+1);
	lastBinEr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );
	lastBinCt = lastBinCt+overflCt;
	h_histG->SetBinContent(h_histG->GetNbinsX(),h_histG->GetBinContent(h_histG->GetNbinsX())+h_histG->GetBinContent(h_histG->GetNbinsX()+1));
	h_histE->SetBinContent(h_histE->GetNbinsX(),h_histE->GetBinContent(h_histE->GetNbinsX())+h_histE->GetBinContent(h_histE->GetNbinsX()+1));
*/
