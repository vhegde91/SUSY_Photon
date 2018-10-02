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
bool saveCanvas=1,save2D=0;
void setLastBinAsOverFlow(TH1D*);
TString getXaxisName(TString);
TLatex textOnTop,intLumiE;
double intLumi=35.9;

double sr_Integral=0,cr_Integral=0;

void compKinematics(TString iFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
 
  TString saveName = iFname;
  saveName.ReplaceAll(".root","");
  saveName.ReplaceAll("t5","_t5");

  f[0] = new TFile(iFname);
  //  gStyle->SetOptStat("nemri");
  
  vector<string> name1;
  vector<int> rebin;
  name1.push_back("pPhoton"); rebin.push_back(2);
  name1.push_back("pLSP"); rebin.push_back(2);
  name1.push_back("ptPhoton"); rebin.push_back(2);
  name1.push_back("ptLSP"); rebin.push_back(2);
  name1.push_back("pBoson"); rebin.push_back(2);
  name1.push_back("ptBoson"); rebin.push_back(2);
  
  // name1.push_back("pxLSP"); rebin.push_back(2);
  // name1.push_back("ELSP"); rebin.push_back(2);
  // name1.push_back("MLSP"); rebin.push_back(2);

  // name1.push_back("pxPhoton"); rebin.push_back(2);
  // name1.push_back("EPhoton"); rebin.push_back(2);
  // name1.push_back("MPhoton"); rebin.push_back(2);

  // name1.push_back("pxBoson"); rebin.push_back(2);
  // name1.push_back("EBoson"); rebin.push_back(2);
  // name1.push_back("MBoson"); rebin.push_back(2);

  TString gravMassName;// = "m_{#tilde{G}} = 1 MeV";

  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  TCanvas *c_cB[name1.size()];
  //  TCanvas *c_cB=new TCanvas("closure_test","closure test",1500,800); c_cB->Divide(4,2);
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];

  for(int i=0;i<name1.size();i++){
    name=name1[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    p_top[i]=new TPad(name+"_top",name+"_top",0,0.4,1,1);
    p_bot[i]=new TPad(name+"_bot",name+"_bot",0,0.0,1,0.4);
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.3);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy();    
    //c_cB[i]=new TCanvas(name+"_2D",name+"_2D",1500,800);
    
    TH1D *h_histG,*h_histE,*h_histGcopy;
    TH2D *h2;
    for(int j=0;j<nfiles;j++){
      c_cA[i]->cd();
      //c_cB->cd(i+1);
      name=name1[i];
      h2=(TH2D*)f[j]->Get(name);

      h_histG=(TH1D*)h2->ProjectionX(name+"_Org",1,1);
      h_histG->Rebin(rebin[i]);
          
      h_histE=(TH1D*)h2->ProjectionX(name+"_New",4,4);
      h_histE->Rebin(rebin[i]);

      gravMassName = h2->GetYaxis()->GetBinLabel(4);

      if(h_histG && h_histE){
	sr_Integral=h_histG->Integral();
	cr_Integral=h_histE->Integral();
	
	setLastBinAsOverFlow(h_histG);
	setLastBinAsOverFlow(h_histE);

	h_histG->SetLineColor(kRed);
	h_histG->SetLineWidth(2);
	h_histG->SetMarkerStyle(21);
	h_histG->SetMarkerColor(h_histG->GetLineColor());
	h_histG->SetTitle(";;Events");
	h_histG->GetYaxis()->SetLabelSize(0.10);
	h_histG->GetYaxis()->SetTitleSize(0.10);
	h_histG->GetYaxis()->SetTitleOffset(0.45);
	h_histG->GetYaxis()->SetNdivisions(10);
	
	h_histE->SetLineColor(kGreen+2);
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

	legend[i]=new TLegend(0.55, 0.85,  0.82, 0.67);
	legend[i]->SetBorderSize(0);
	name=name1[i];
	legend[i]->AddEntry(h_histG,"Default, m(#tilde{G} = 1 GeV)","lp");
	legend[i]->AddEntry(h_histE,gravMassName,"lp");
	legend[i]->Draw();
	
	TString name = h_histG->GetName();
	
      }
    }
    
    TH1D *h_numr=(TH1D*)h_histG->Clone();
    
    h_numr->Divide(h_histE);
    TString xaxisName = getXaxisName(h_histG->GetName());

    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    //    h_numr->SetTitle(";;#frac{0#mu,Had#tau+#gamma}{1#mu+#gamma}");
    h_numr->SetTitle(0);
    h_numr->GetXaxis()->SetLabelSize(0.13);
    h_numr->GetXaxis()->SetTitle(xaxisName);
    h_numr->GetXaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetTitleOffset(1.0);

    h_numr->GetYaxis()->SetTitleOffset(0.29);
    h_numr->GetYaxis()->SetTitleSize(0.13);
    h_numr->GetYaxis()->SetLabelSize(0.13);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->GetYaxis()->SetTitle("#frac{NewMass}{1GeV}");
    h_numr->SetMaximum(1.90);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e1");
    h_numr->Draw("e0same");

    TString name = h_histG->GetName();
   
    c_cA[i]->cd();    p_top[i]->cd();
    char name2[100];
    textOnTop.SetTextSize(0.06);
    intLumiE.SetTextSize(0.06);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
    intLumiE.SetTextSize(0.06);
    sprintf(name2,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.73,0.91,name2);

    if(saveCanvas){name=h2->GetName()+saveName+"_1D.png";c_cA[i]->SaveAs(name);}
    // c_cB[i]->cd();
    // c_cB[i]->SetLeftMargin(0.2);
    // //    h2->RebinX(rebin[i]);
    // h2->GetXaxis()->SetTitle(h2->GetName());
    // c_cB[i]->SetLogz();
    // h2->Draw("colz");
    // if(save2D){name = h2->GetName()+saveName+".png";c_cB[i]->SaveAs(name);}
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
  else if(axname.Contains("pPhoton")) return "p^{#gamma} (GeV)";
  else if(axname.Contains("pLSP")) return " p^{#tilde{G}} (GeV)";
  else if(axname.Contains("ptPhoton")) return "p^{#gamma}_{T} (GeV)";
  else if(axname.Contains("ptLSP")) return " p^{#tilde{G}}_{T} (GeV)";
  else if(axname.Contains("pBoson")) return " p^{Z} (GeV)";
  else if(axname.Contains("ptBoson")) return "p^{Z}_{T} (GeV)";
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
