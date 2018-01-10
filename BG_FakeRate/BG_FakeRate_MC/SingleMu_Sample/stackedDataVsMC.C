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
//int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
int col[10]={kBlack,kCyan,kMagenta+2,kOrange,kYellow,kGreen,kTeal+9,kPink+1,kBlue,kRed};  //Specify Colors
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
  //  f[1] = new TFile("CS_TTJets_FR_NoTrgNoEleMatchbJ_v2.root");
  // f[1] = new TFile("CS_QCD_FR_NoTrg_v2.root");
  // f[2] = new TFile("CS_ST_FR_NoTrg_v2.root");
  //  f[3] = new TFile("CS_TTJets_FR_NoTrg_v2.root");
  //  f[3] = new TFile("CS_TTJets_myISRWt_FR_NoTrg_v2.root");
  //  f[4] = new TFile("CS_WJets_FR_NoTrg_v2.root");

  f[1] = new TFile("SingMu_QCD_NoSRTrigReq.root");
  f[2] = new TFile("SingMu_ST_NoSRTrigReq.root");
  f[3] = new TFile("SingMu_TTJets_NoSRTrigReq.root");
  f[4] = new TFile("SingMu_WJets_NoSRTrigReq.root");
  vector<string> name1;
  vector<int> rebin;
  name1.push_back("ST_Ele");  rebin.push_back(20);         //name2.push_back("ST_Ele");     
  name1.push_back("MET_Ele");    rebin.push_back(5);     //name2.push_back("MET_Ele");     
  name1.push_back("nBTags_Ele");   rebin.push_back(1);    //name2.push_back("nBTags_Ele");     
  name1.push_back("nHadJets_Ele");  rebin.push_back(1);  //name2.push_back("nHadJets_Ele");
  name1.push_back("ElePt");   rebin.push_back(5);  //name2.push_back("BestPhotonPt_Ele");      
  name1.push_back("ElePtvBin");   rebin.push_back(1); 
  //  name1.push_back("EleEta"); rebin.push_back(2);  //name2.push_back("BestPhotonEta_Ele");  
  //  name1.push_back("ElePhi"); rebin.push_back(1);  //name2.push_back("BestPhotonEta_Ele");  
  name1.push_back("METvarBin_Ele");  rebin.push_back(1); //name2.push_back("METvarBin_Ele");  
  // name1.push_back("dPhi_METjet1_Ele"); rebin.push_back(2);  //name2.push_back("dPhi_METjet1_Pho_Ele");  
  // name1.push_back("dPhi_METjet2_Ele"); rebin.push_back(2);  //name2.push_back("dPhi_METjet2_Pho_Ele");  
  //  name1.push_back("Qmulti_Ele"); rebin.push_back(2);
  name1.push_back("QMult2_Ele"); rebin.push_back(2);
  //  name1.push_back("mT_Ele");   rebin.push_back(2);

  // name1.push_back("MET_R1_v2_Ele");   rebin.push_back(1);
  // name1.push_back("MET_R2_v2_Ele");   rebin.push_back(1);
  // name1.push_back("MET_R3_v2_Ele");   rebin.push_back(1);
  // name1.push_back("MET_R4_v2_Ele");   rebin.push_back(1);
  // name1.push_back("MET_R5_v2_Ele");   rebin.push_back(1);
  name1.push_back("AllSBins_Ele");   rebin.push_back(1);

  //  name1.push_back("nVtx_Ele");   rebin.push_back(5);
  // name1.push_back("nEleTracks_Ele");   rebin.push_back(1);
  // name1.push_back("nMuTracks_Ele");   rebin.push_back(1);
  // name1.push_back("nPiTracks_Ele");   rebin.push_back(1);
  // name1.push_back("nTracks_Ele");   rebin.push_back(1);

  // name1.push_back("MET_Ele_R2");         name2.push_back("MET_Ele_R2");rebin.push_back(1);
  // name1.push_back("MET_Ele_R3");         name2.push_back("MET_Ele_R3");rebin.push_back(1);

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
    legend[i]=new TLegend(0.8, 0.90,  0.90, 0.55);
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
      //      if(p==4) h_histE->Scale(1/1.21);
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
    hs_hist[i]->Draw("BAR");
    h_histG->Draw("same");
    p_top[i]->SetTickx();p_top[i]->SetTicky();
    hs_hist[i]->SetTitle(";;Events");
    hs_hist[i]->GetYaxis()->SetTitleOffset(0.60);    
    hs_hist[i]->GetYaxis()->SetTitleSize(0.09);
    hs_hist[i]->GetYaxis()->SetLabelSize(0.09);
    
    c_cA[i]->Modified();
    c_cA[i]->Update();

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
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e0");
    //    p_bot[i]->SetTickx();p_bot[i]->SetTicky();

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
  if(fname.Contains("TTJets")) return "t#bar{t}";
  else if(fname.Contains("WJets")) return "W+Jets";
  else if(fname.Contains("ST")) return "Single top";
  else if(fname.Contains("QCD")) return "QCD";
  else return fname;
}

TString getXaxisName(TString axname){
  if(axname.Contains("nHadJets")) return "Jets";
  else if(axname.Contains("MET")) return "MET(GeV)";
  else if(axname.Contains("ST")) return "ST(GeV)";
  else if(axname.Contains("BTags")) return "b-Tags";
  else if(axname.Contains("ElePt")) return "#mu pT(GeV)";
  else if(axname.Contains("EleEta")) return "#mu #eta";
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



/*


	double lastBinCt =h_histG->GetBinContent(h_histG->GetNbinsX()),overflCt =h_histG->GetBinContent(h_histG->GetNbinsX()+1);
	double lastBinErr=h_histG->GetBinError(h_histG->GetNbinsX()),  overflErr=h_histG->GetBinError(h_histG->GetNbinsX()+1);
	lastBinEr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );
	lastBinCt = lastBinCt+overflCt;
	h_histG->SetBinContent(h_histG->GetNbinsX(),h_histG->GetBinContent(h_histG->GetNbinsX())+h_histG->GetBinContent(h_histG->GetNbinsX()+1));
	h_histE->SetBinContent(h_histE->GetNbinsX(),h_histE->GetBinContent(h_histE->GetNbinsX())+h_histE->GetBinContent(h_histE->GetNbinsX()+1));
*/
