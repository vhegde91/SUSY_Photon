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
TFile *f[nfiles];
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
TString name;
bool saveCanvas=false;
void setLastBinAsOverFlow(TH1D*);
TString getXaxisName(TString);
TLatex textOnTop,intLumiE;
double intLumi=35.9;
double sr_Integral=0,cr_Integral=0;

void testClosure(TString iFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
 
  f[0] = new TFile(iFname);
  TFile *fout = new TFile("forPull.root","recreate");
  //  gStyle->SetOptStat("nemri");
  
  vector<string> name1,name2;
  vector<int> rebin;
  name1.push_back("ST_Ele0");          name2.push_back("ST_Ele1");      rebin.push_back(25);
  name1.push_back("MET_Ele0");         name2.push_back("MET_Ele1");     rebin.push_back(5);
  name1.push_back("nBTags_Ele0");       name2.push_back("nBTags_Ele1");     rebin.push_back(1);
  name1.push_back("nHadJets_Ele0");    name2.push_back("nHadJets_Ele1");rebin.push_back(1);
  name1.push_back("BestPhotonPt_Ele0");    name2.push_back("BestPhotonPt_Ele1");       rebin.push_back(10);
  //  name1.push_back("BestPhotonEta_Ele0");   name2.push_back("BestPhotonEta_Ele1");  rebin.push_back(10);
  name1.push_back("METvarBin_Ele0");   name2.push_back("METvarBin_Ele1");  rebin.push_back(1);
  name1.push_back("dPhi_METjet1_Pho_Ele0");   name2.push_back("dPhi_METjet1_Pho_Ele1");  rebin.push_back(4);
  name1.push_back("dPhi_METjet2_Pho_Ele0");   name2.push_back("dPhi_METjet2_Pho_Ele1");  rebin.push_back(4);
  name1.push_back("dPhi_METBestPhoton_Ele0");   name2.push_back("dPhi_METBestPhoton_Ele1");  rebin.push_back(4);
  name1.push_back("mTPho_Ele0");   name2.push_back("mTPho_Ele1");  rebin.push_back(10);
  // name1.push_back("MET_R1_v2_Ele0");   name2.push_back("MET_R1_v2_Ele1");  rebin.push_back(1);
  // name1.push_back("MET_R2_v2_Ele0");   name2.push_back("MET_R2_v2_Ele1");  rebin.push_back(1);
  // name1.push_back("MET_R3_v2_Ele0");   name2.push_back("MET_R3_v2_Ele1");  rebin.push_back(1);
  // name1.push_back("MET_R4_v2_Ele0");   name2.push_back("MET_R4_v2_Ele1");  rebin.push_back(1);
  // name1.push_back("MET_R5_v2_Ele0");   name2.push_back("MET_R5_v2_Ele1");  rebin.push_back(1);
  //  name1.push_back("AllSBins_Ele0");    name2.push_back("AllSBins_Ele1");  rebin.push_back(1);
  name1.push_back("AllSBins_v7_Ele0");    name2.push_back("AllSBins_v7_Ele1");  rebin.push_back(1);
  name1.push_back("AllSBins_v4_Ele0");    name2.push_back("AllSBins_v4_Ele1");  rebin.push_back(1);
  
  // name1.push_back("MET_Ele0_R1");         name2.push_back("MET_Ele1_R1");rebin.push_back(1);
  // name1.push_back("MET_Ele0_R2");         name2.push_back("MET_Ele1_R2");rebin.push_back(1);
  // name1.push_back("MET_Ele0_R3");         name2.push_back("MET_Ele1_R3");rebin.push_back(1);

  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  TLine *line1=new TLine( 8.5,0.01,  8.5,50);
  TLine *line2=new TLine(15.5,0.01, 15.5,50);
  TLine *line3=new TLine(22.5,0.01, 22.5,50);
  TLine *line4=new TLine(29.5,0.01, 29.5,50);
  TLine *line5=new TLine(36.5,0.01, 36.5,8 );
  TLine *line1V7=new TLine( 6.5,0.05,  6.5,100);
  TLine *line2V7=new TLine(11.5,0.05, 11.5,100);
  TLine *line3V7=new TLine(16.5,0.05, 16.5,100);
  TLine *line4V7=new TLine(21.5,0.05, 21.5,100);
  TLine *line5V7=new TLine(26.5,0.05, 26.5,10 );
  //  TCanvas *c_cB=new TCanvas("closure_test","closure test",1500,800); c_cB->Divide(4,2);
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];

  for(int i=0;i<name1.size();i++){
    name=name1[i]+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    p_top[i]=new TPad(name+"_top",name+"_top",0,0.4,1,1);
    p_bot[i]=new TPad(name+"_bot",name+"_bot",0,0.0,1,0.4);
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
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
      h_histG->Rebin(rebin[i]);//h_histG->Scale(1.0/h_histG->Integral());
          
      name=name2[i];
      h_histE=(TH1D*)f[j]->FindObjectAny(name);//h_histE->Scale(150.9/481.6);
      h_histE->Rebin(rebin[i]);//h_histE->Scale(1.0/h_histE->Integral());
     
      if(h_histG && h_histE){
	if(name1[i]=="nHadJets_Ele0"){
	  sr_Integral=h_histG->Integral();
	  cr_Integral=h_histE->Integral();
	}
	setLastBinAsOverFlow(h_histG);
	setLastBinAsOverFlow(h_histE);

	h_histG->SetLineColor(kRed);
	h_histG->SetLineWidth(2);
	h_histG->SetMarkerStyle(21);
	h_histG->SetMarkerColor(h_histG->GetLineColor());
	h_histG->SetTitle(";;Events");
	h_histG->GetYaxis()->SetLabelSize(0.10);
	h_histG->GetYaxis()->SetTitleSize(0.10);
	h_histG->GetYaxis()->SetNdivisions(10);
	h_histG->GetYaxis()->SetTitleOffset(0.45);
	
	h_histE->SetLineColor(kCyan+2);
	h_histE->SetLineWidth(2);
	h_histE->SetMarkerStyle(20);
	h_histE->SetMarkerColor(h_histE->GetLineColor());

	c_cA[i]->cd();p_top[i]->cd();
	//c_cB->cd(i+1);p_top[i]->cd();
	h_histG->Draw("e1");
	// h_histE->SetFillStyle(3004);
	// h_histE->SetFillColor(h_histE->GetLineColor());
 	h_histE->Draw("e1same");
	//	h_histE->Draw("L same");

	legend[i]=new TLegend(0.72, 0.87,  0.87, 0.67);
	name=name1[i];
	legend[i]->AddEntry(h_histG,"Exp(0e+#gamma)","lp");
	name=name2[i];
	legend[i]->AddEntry(h_histE,"Pred(1e+#gamma)","lp");
	legend[i]->Draw();
	TString name = h_histG->GetName();
	if(name.Contains("SBins_v4")){ 
	  line1->Draw();line2->Draw();line3->Draw();line4->Draw();line5->Draw();
	  p_top[i]->SetGridx(0);
	}
	else if(name.Contains("SBins_v7")){ 
	  line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
	  p_top[i]->SetGridx(0);
	}
      }
    }
    
    TH1D *h_numr=(TH1D*)h_histG->Clone();
    h_numr->Divide(h_histE);
    TString xaxisName = getXaxisName(h_histG->GetName());

    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    //    h_numr->SetTitle(";;#frac{0e+#gamma}{1e+#gamma}");
    h_numr->SetTitle(";;#frac{Exp}{Pred }");
    h_numr->GetXaxis()->SetLabelSize(0.13);
    h_numr->GetXaxis()->SetTitle(xaxisName);
    h_numr->GetXaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetTitleOffset(0.9);

    h_numr->GetYaxis()->SetTitleOffset(0.32);
    h_numr->GetYaxis()->SetTitleSize(0.13);
    h_numr->GetYaxis()->SetLabelSize(0.13);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->SetMaximum(1.90);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e0");
    h_numr->Draw("e1same");
    TString name = h_histG->GetName();
    if(name.Contains("SBins_v4")){ 
      line1->Draw();line2->Draw();line3->Draw();line4->Draw();line5->Draw();
      fout->cd();
      h_numr->Write();
      TH1D *h_pullHist = new TH1D("pull_lostEle","1D pull for lost e",50,-2.5,7.5);
      for(int p=1;p<=h_numr->GetNbinsX();p++){
	h_pullHist->Fill( (1.0-h_numr->GetBinContent(p))/h_numr->GetBinError(p));
      }
      h_pullHist->Write();
      //      p_bot[i]->SetGridx(0);
    }
    else if(name.Contains("SBins_v7")){
      line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
    }
    c_cA[i]->cd();    p_top[i]->cd();
    char name2[100];
    textOnTop.SetTextSize(0.06);
    intLumiE.SetTextSize(0.06);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
    intLumiE.SetTextSize(0.06);
    sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.73,0.91,name2);
    if(saveCanvas){name="c_"+name1[i]+name2[i]+".png";c_cA[i]->SaveAs(name);}
  }
  gStyle->SetTextSize(2);
  //fout->cd();
  //  h2_histGcopy->Write();
  cout<<"Integral in SR(0 Ele events) "<<sr_Integral<<endl
      <<"Integral in CR(1 Ele events) "<<cr_Integral<<endl;


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
