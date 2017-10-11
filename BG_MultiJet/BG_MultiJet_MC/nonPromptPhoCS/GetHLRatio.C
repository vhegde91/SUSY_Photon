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
#include"TString.h"

const int nfiles=1,nBG=1;    //Specify no. of files
TLatex textOnTop,intLumiE;
TFile *f[nfiles];
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kCyan,kBlue,kTeal+9,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
TString name;
void setLastBinAsOverFlow(TH1D*);

void GetHLRatio(TString inFname){
  TH1::SetDefaultSumw2(1);
  //  gStyle->SetOptStat(0);
  f[0] = new TFile(inFname);

  TFile *fout=new TFile("HLR_"+inFname,"recreate");
  TString hist2Save="AllSBins_v1_";
  //  gStyle->SetOptStat("nemri");
  double intLumi=35.9;  
  vector<string> name1,name2,name1_2d,name2_2d;
  vector<int> rebin;
  name1.push_back("ST_C");          name2.push_back("ST_A");      rebin.push_back(10);
  name1.push_back("myHT_C");          name2.push_back("myHT_A");      rebin.push_back(25);
  name1.push_back("STvarBin_C");          name2.push_back("STvarBin_A");      rebin.push_back(1);
  name1.push_back("MET_CD");         name2.push_back("MET_AB");     rebin.push_back(1);
  //  name1.push_back("GenMET_C");         name2.push_back("GenMET_A");     rebin.push_back(2);
  //  name1.push_back("METPhi_C");         name2.push_back("METPhi_A");     rebin.push_back(10);
  name1.push_back("nHadJets_C");    name2.push_back("nHadJets_A");rebin.push_back(1);
  name1.push_back("nBTags_C");      name2.push_back("nBTags_A");  rebin.push_back(1);
  name1.push_back("BestPhotonPt_C");    name2.push_back("BestPhotonPt_A");       rebin.push_back(2);
  name1.push_back("METvarBin_CD");   name2.push_back("METvarBin_AB");rebin.push_back(1);
  // name1.push_back("jet1Pt_C");   name2.push_back("jet1Pt_A");rebin.push_back(5);
  // name1.push_back("jet2Pt_C");   name2.push_back("jet2Pt_A");rebin.push_back(5);
  //  name1.push_back("PtJetNearMET_C");   name2.push_back("PtJetNearMET_A");rebin.push_back(20);
  // name1.push_back("dPhi_METjet1_C");   name2.push_back("dPhi_METjet1_A");rebin.push_back(1);
  // name1.push_back("dPhi_METjet2_C");   name2.push_back("dPhi_METjet2_A");rebin.push_back(1);
  name1.push_back("dPhi_METBestPhoton_C");   name2.push_back("dPhi_METBestPhoton_A");rebin.push_back(2);
  name1.push_back("AllSBins_v1_CD");   name2.push_back("AllSBins_v1_AB");rebin.push_back(1);
  //  name1.push_back("dPhiPhotonJet1_C");   name2.push_back("dPhiPhotonJet1_A");rebin.push_back(1);

  name1_2d.push_back("NJST_C"); name2_2d.push_back("NJST_A");
  //  name1_2d.push_back("dPhiMETPho_NJ_C"); name2_2d.push_back("dPhiMETPho_NJ_A");

  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()+name1_2d.size()];
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
          
      name=name2[i];
      h_histE=(TH1D*)f[j]->FindObjectAny(name);//h_histE->Rebin(2);
      h_histE->Rebin(rebin[i]);
     
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
	legend[i]=new TLegend(0.7, 0.90,  0.90, 0.75);
	name=name1[i];
	legend[i]->AddEntry(h_histG,"CD(High #Delta#Phi)","l");
	name=name2[i];
	legend[i]->AddEntry(h_histE,"AB(Low #Delta#Phi)","l");
	legend[i]->Draw();
      }
    }

    char name3[100];
    textOnTop.SetTextSize(0.06);
    intLumiE.SetTextSize(0.06);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
    intLumiE.SetTextSize(0.06);
    sprintf(name3,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.73,0.91,name3);
    
    TH1D *h_numr=(TH1D*)h_histG->Clone();

    h_numr->Divide(h_histE);

    TString xLabel=h_histE->GetName();

    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    h_numr->SetTitle(";;#frac{CD}{AB}");
    h_numr->GetYaxis()->SetTitleOffset(0.28);
    h_numr->GetYaxis()->SetTitleSize(0.17);
    h_numr->GetXaxis()->SetLabelSize(0.17);
    h_numr->GetXaxis()->SetTitle(xLabel);
    h_numr->GetXaxis()->SetTitleSize(0.17);
    h_numr->GetXaxis()->SetTitleOffset(0.85);
    h_numr->GetYaxis()->SetLabelSize(0.17);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->SetMaximum(2.);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e0");
    TString hname1=h_numr->GetName();
    if(hname1.Contains(hist2Save)){
      TH1D *h_ratio=(TH1D*)h_numr->Clone("HLratio_1D");
      fout->cd();
      h_ratio->Write();
    }
  }

  TH2D *h2_histG,*h2_histE,*h2_histGcopy,*h2_histGcopy2; 
  for(int i=0;i<name1_2d.size();i++){
    //    sprintf(name,"%s_%s",name1_2d[i].c_str(),name2_2d[i].c_str());
    name=name1_2d[i]+"_"+name2_2d[i].c_str();
    c_cA[i+name1.size()]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    c_cA[i+name1.size()]->cd();
    for(int j=0;j<nfiles;j++){
      c_cA[i+name1.size()]->cd();
      //      sprintf(name,"%s",name1_2d[i].c_str());
      name=name1_2d[i];
      h2_histG=(TH2D*)f[j]->FindObjectAny(name);
      //      sprintf(name,"%s",name2_2d[i].c_str());
      name=name2_2d[i];
      h2_histE=(TH2D*)f[j]->FindObjectAny(name);
      if(h2_histG && h2_histE){
	h2_histGcopy=(TH2D*)h2_histG->Clone("HLRatio");
	h2_histGcopy2=(TH2D*)h2_histG->Clone("HLRatio2");
	// h2_histGcopy->RebinX(2);
	// h2_histGcopy->RebinY(5);
	// h2_histE->RebinX(2);
	// h2_histE->RebinY(5);
	h2_histGcopy->Divide(h2_histE);
	h2_histGcopy->Draw("colz texte");
	//	h2_histGcopy->SetMaximum(0.02);
	gPad->Update();
	fout->cd();
	h2_histGcopy->Write();
      }
    }
  }

  gStyle->SetTextSize(2);

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
