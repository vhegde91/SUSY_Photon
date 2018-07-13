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
TString getXaxisName(TString axname);
void showClosure1(TString);

void showClosure(TString inFname){
  TFile *f1 = new TFile(inFname);
  TH1D *h_exp=(TH1D*)f1->Get("AllSBins_v4_CD");
  TH1D *h_pre=(TH1D*)f1->Get("AllSBins_v4_AB");

  TH1D *h_pre_cp=(TH1D*)f1->Get("AllSBins_v4_AB");

  TString foutName = "VS_doubleR_"+inFname;
  TFile *fout1=new TFile(foutName,"recreate");
  h_exp->Write();

  vector<float> doubleR,hlr;
  float temp1,temp2;
  
  for(int i=0;i<h_exp->GetNbinsX();i++){
    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 ){
      temp1 = h_exp->GetBinContent(i)/h_pre->GetBinContent(i);
      hlr.push_back(temp1);
    }
  }
  doubleR.push_back( (h_exp->Integral( 2 ,6)/h_pre->Integral( 2, 6)) / hlr[0]);
  doubleR.push_back( (h_exp->Integral( 8,11)/h_pre->Integral( 8,11)) / hlr[1]);
  doubleR.push_back( (h_exp->Integral(13,16)/h_pre->Integral(13,16)) / hlr[2]);

  doubleR.push_back( (h_exp->Integral(18,21)/h_pre->Integral(18,21)) / hlr[3]);
  doubleR.push_back( (h_exp->Integral(23,26)/h_pre->Integral(23,26)) / hlr[4]);
  doubleR.push_back( (h_exp->Integral(28,31)/h_pre->Integral(28,31)) / hlr[5]);

  cout<<"H/L ratio:"<<endl;  
  for(int i=0;i<hlr.size();i++)
    cout<<hlr[i]<<endl;
  cout<<"Double R:"<<endl;
  for(int i=0;i<doubleR.size();i++)
    cout<<doubleR[i]<<endl;

  TH1D *h_scale=(TH1D*)h_exp->Clone("scalinghist");
  
  int regNo=-1;
  for(int i=1;i<=h_scale->GetNbinsX();i++){
    h_scale->SetBinContent(i,0);
    h_scale->SetBinError(i,0);
    if(i<=6) regNo = 1;
    else if(i<=11) regNo = 2;
    else if(i<=16) regNo = 3;
    else if(i<=21) regNo = 4;
    else if(i<=26) regNo = 5;
    else if(i<=31) regNo = 6;
    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 )
      h_scale->SetBinContent(i,hlr[regNo-1]*1.);
    else h_scale->SetBinContent(i,hlr[regNo-1]*doubleR[regNo-1]);
  }

  h_pre_cp->Multiply(h_scale);
  fout1->cd();
  h_pre_cp->Write();
  fout1->Close();
  showClosure1(foutName);
  
}
void showClosure1(TString inFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  f[0] = new TFile(inFname);
  TFile *fout = new TFile("forPull.root","recreate");
  //  gStyle->SetOptStat("nemri");
  double intLumi=35.9;  
  vector<TString> name1,name2;
  vector<int> rebin;
  name1.push_back("AllSBins_v4_CD");   name2.push_back("AllSBins_v4_AB");rebin.push_back(1);
  
  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  //  TCanvas *c_cB=new TCanvas("closure_test","closure test",1500,800); c_cB->Divide(4,2);
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];

  for(int i=0;i<name1.size();i++){
    name=name1[i]+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    p_top[i]=new TPad(name+"_top",name+"_top",0,0.4,1,1);
    p_bot[i]=new TPad(name+"_bot",name+"_bot",0,0.0,1,0.4);
    //    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->Draw();p_top[i]->SetGrid(1,1);p_top[i]->SetLogy();
    //    gStyle->SetGridStyle(1);
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.3);
    p_bot[i]->Draw();p_bot[i]->SetGridy();//p_bot[i]->SetGridx();
    if(name1[i].Contains("AllSBins_v")){p_bot[i]->SetGridx(0);p_top[i]->SetGridx(0);}
    
    TH1D *h_histG,*h_histE,*h_histGcopy;
    for(int j=0;j<nfiles;j++){
      c_cA[i]->cd();
      //c_cB->cd(i+1);
      name=name1[i];
      h_histG=(TH1D*)f[j]->FindObjectAny(name);//h_histG->Rebin(2);
      h_histG->Rebin(rebin[i]);
      //      h_histG->Scale(1./h_histG->Integral());
          
      name=name2[i];
      h_histE=(TH1D*)f[j]->FindObjectAny(name);//h_histE->Rebin(2);
      h_histE->Rebin(rebin[i]);
      // h_histE->Scale(1.0/h_histE->Integral());
     
      if(h_histG && h_histE){

	setLastBinAsOverFlow(h_histG);
	setLastBinAsOverFlow(h_histE);

	h_histG->SetLineColor(col[0]);
	h_histG->SetLineWidth(2);
	h_histG->SetMarkerStyle(21);
	h_histG->SetMarkerColor(h_histG->GetLineColor());
	h_histG->GetYaxis()->SetLabelSize(0.09);
	h_histG->GetYaxis()->SetTitle("Events");
	h_histG->GetYaxis()->SetTitleOffset(0.55);
	h_histG->GetYaxis()->SetTitleSize(0.09);

	h_histE->SetLineColor(col[1]);
	h_histE->SetLineWidth(2);
	h_histE->SetMarkerStyle(20);
	h_histE->SetMarkerColor(h_histE->GetLineColor());

	c_cA[i]->cd();p_top[i]->cd();
	//c_cB->cd(i+1);p_top[i]->cd();
	h_histG->Draw("e1");
	h_histE->Draw("e1 same");
	h_histG->SetTitle(0);
	legend[i]=new TLegend(0.7, 0.7,  0.85, 0.88);
	name=name1[i];
	legend[i]->AddEntry(h_histG,"Expected","lep");
	name=name2[i];
	legend[i]->AddEntry(h_histE,"Prediction","lep");
	legend[i]->Draw();
      }
    }

    char name3[100];
    textOnTop.SetTextSize(0.07);
    intLumiE.SetTextSize(0.07);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
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
    //    h_numr->SetTitle(";;#frac{High #Delta#Phi}{Low #Delta#Phi}");
    h_numr->SetTitle(";;#frac{Exp}{Pred }");
    h_numr->GetYaxis()->SetTitleOffset(0.35);
    h_numr->GetYaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetLabelSize(0.14);
    h_numr->GetXaxis()->SetTitle(getXaxisName(h_histE->GetName()));
    h_numr->GetXaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetTitleOffset(0.87);
    h_numr->GetYaxis()->SetLabelSize(0.14);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->SetMaximum(1.99);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e0");
    h_numr->Draw("e1 same");
    TString name = h_histG->GetName();
    if(name.Contains("SBins_v4")){ 
      fout->cd();
      h_numr->Write();
      //      TH1D *h_pullHist = new TH1D("pull_multijet0Pho","1D pull for 0 photon VR",50,-2.5,7.5);
      TH1D *h_pullHist = new TH1D("pull_multijet0Pho","1D pull for 0 photon VR",10,-2.5,7.5);
      for(int p=1;p<=h_numr->GetNbinsX();p++){
	if( p==1 || p==7 || p==12 || p==17 || p==22 || p==27 ) continue;
	h_pullHist->Fill( (1.0-h_numr->GetBinContent(p))/h_numr->GetBinError(p));
	//h_pullHist->Fill( p,(1.0-h_numr->GetBinContent(p))/h_numr->GetBinError(p));
      }
      h_pullHist->Write();
    }
    
    TLine *line1V7=new TLine( 6.5,0.01,  6.5,1000000);
    TLine *line2V7=new TLine(11.5,0.01, 11.5,1000000);
    TLine *line3V7=new TLine(16.5,0.01, 16.5,1000000);
    TLine *line4V7=new TLine(21.5,0.01, 21.5,1000000);
    TLine *line5V7=new TLine(26.5,0.01, 26.5,30000);

    if(name1[i].Contains("AllSBins")){
      p_top[i]->cd(); p_top[i]->SetGridx(0);
      line1V7->Draw();      line2V7->Draw();        line3V7->Draw();        line4V7->Draw();    line5V7->Draw();
      p_bot[i]->cd(); p_bot[i]->SetGridx(0);
      line1V7->Draw();      line2V7->Draw();        line3V7->Draw();        line4V7->Draw();    line5V7->Draw();
    }
  }

  gStyle->SetTextSize(2);
  //fout->cd();
  //  h2_histGcopy->Write();



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
  else if(axname.Contains("dPhi_METjet1") || axname.Contains("dphi1_METjet1")) return "#De\
lta#Phi_{1}";
  else if(axname.Contains("dPhi_METjet2") || axname.Contains("dphi2_METjet2")) return "#De\
lta#Phi_{2}";
  else if(axname.Contains("dPhi_METBestPhoton") ) return "#Delta#Phi(MET,#gamma)";
  else if(axname.Contains("QMut") || axname.Contains("Qmut")) return "QMult";
  else if(axname.Contains("MET")) return "MET(GeV)";
  else return axname;

}
