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

const int nfiles=8;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
///int col[11]={kBlack,kPink-2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kPink+1,kBlue,kRed,kMagenta};  //Specify Colors
//int col[10]={kBlack,kMagenta+2,kOrange,kYellow,kGreen,kTeal+9,kPink+1,kCyan,kBlue,kRed};  //Specify Colors
int col[11]={kBlack,kPink-2,kTeal+9,kGreen,kYellow,kOrange,kBlue,kCyan,kRed,kBlue+2,kMagenta};
TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getLegName(TString);
TString getXaxisName(TString);
void printBinContents(TH1D*);
double data_Integral=0,mc_Integral=0;
TLatex textOnTop,intLumiE;
double intLumi=35.9;
void allBGPredVsData(TString iFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  //  gStyle->SetOptStat("nemri");
  vector<TString> suffixHist; 
  f[0] = new TFile(iFname);                                      suffixHist.push_back("");
  f[1] = new TFile("ZGZJ_NuNuG.root");                       suffixHist.push_back("");
  f[2] = new TFile("TTGJets.root");               suffixHist.push_back("");
  f[3] = new TFile("WGJetsToLNuG.root");             suffixHist.push_back("");
  f[4] = new TFile("TTJetsHT.root");          suffixHist.push_back("");
  f[5] = new TFile("WJetsToLNu.root");         suffixHist.push_back("");
  f[6] = new TFile("QCD.root");         suffixHist.push_back("");
  f[7] = new TFile("GJets.root");         suffixHist.push_back("");
  

  TFile *fout;
  vector<string> name1;
  vector<int> rebin;
  name1.push_back("mindPhi1dPhi2");   rebin.push_back(10);
  //  name1.push_back("AllSBins_v7");   rebin.push_back(1);


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
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.3);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy();    
    name=name1[i]+"_Stack";
    hs_hist[i] = new THStack(name,name);
    //    legend[i]=new TLegend(0.7, 0.88,  0.86, 0.45);
    legend[i]=new TLegend(0.312, 0.73,  0.9, 0.88);
    legend[i]->SetBorderSize(0);
  }
  //cout<<getLegName(f[0]->GetName());
  TH1D *h_histG,*h_histE,*h_histGcopy;
  for(int i=0;i<name1.size();i++){
    c_cA[i]->cd();
    name=name1[i]+suffixHist[0];
    h_histG=(TH1D*)f[0]->FindObjectAny(name);
    if(!h_histG) cout<<"Hist Not Found: "<<name<<" in file "<<f[0]->GetName()<<endl;
    h_histG->Rebin(rebin[i]);
    //setLastBinAsOverFlow(h_histG);
    if(name.Contains("v7")) data_Integral=h_histG->Integral();
    c_cA[i]->cd();p_top[i]->cd();
    p_top[i]->SetTickx();p_top[i]->SetTicky();
    h_histG->SetLineColor(kBlack);
    h_histG->SetLineWidth(2);
    h_histG->SetMarkerStyle(20);
    h_histG->SetMarkerColor(h_histG->GetLineColor());
    cout<<"--------------Data--------------------"<<endl;
    //    printBinContents(h_histG);
    // h_histG->SetTitle(";;Events");
    // h_histG->GetYaxis()->SetTitleOffset(0.60);    
    // h_histG->GetYaxis()->SetTitleSize(0.08);
    // h_histG->GetYaxis()->SetLabelSize(0.08);
    //h_histG->Draw();
    legend[i]->AddEntry(h_histG,"Data","ep");
    legend[i]->SetNColumns(4);
    //gPad->Update();
    // gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
      
    for(int p=1;p<nfiles;p++){  
      name=name1[i]+suffixHist[p];
     
      h_histE=(TH1D*)f[p]->FindObjectAny(name);
      if(!h_histE) cout<<"Hist Not Found: "<<name<<" in file "<<f[p]->GetName()<<endl;
      h_histE->Rebin(rebin[i]);
      //      setLastBinAsOverFlow(h_histE);
      //cout<<"--------------"<<getLegName(f[p]->GetName())<<"--------------------"<<endl;
      //      printBinContents(h_histE);
      if(name.Contains("v7")){
        mc_Integral+=h_histE->Integral();
	//        cout<<f[p]->GetName()<<" # events "<<h_histE->Integral()<<endl;
      }
      h_histE->SetLineColor(kBlack);
      h_histE->SetLineWidth(1);
      //      h_histE->SetMarkerStyle(21);
      //      h_histE->SetMarkerColor(h_histE->GetLineColor());
      //      h_histE->SetFillColor(h_histE->GetLineColor());
      h_histE->SetMarkerColor(col[p]);
      h_histE->SetFillColor(col[p]);
     
      hs_hist[i]->Add(h_histE);
      if(p==1){
	name=name1[i]+"_Sum";
	h_sum[i] = (TH1D*)h_histE->Clone(name);
      }
      else h_sum[i]->Add(h_histE);
      legend[i]->AddEntry(h_histE,getLegName(f[p]->GetName()),"f");
    }
    c_cA[i]->cd();    p_top[i]->cd();
    hs_hist[i]->SetMinimum(6);
    hs_hist[i]->SetMaximum(12000);
    //hs_hist[i]->Draw("BAR");
    hs_hist[i]->Draw("HISTE");
    h_histG->Draw("E1 X0 same");
    hs_hist[i]->SetTitle(";;Events");
    hs_hist[i]->GetYaxis()->SetTitleOffset(0.50);    
    hs_hist[i]->GetYaxis()->SetTitleSize(0.09);
    hs_hist[i]->GetYaxis()->SetLabelSize(0.09);
    
    c_cA[i]->Modified();
    c_cA[i]->Update();

    legend[i]->Draw();

    TH1D *h_numr=(TH1D*)h_histG->Clone();
    h_numr->Divide(h_sum[i]);
    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    h_numr->SetTitle(0); name=name1[i];
    h_numr->GetXaxis()->SetTitle(getXaxisName(name));
    h_numr->GetXaxis()->SetTitleOffset(0.87);
    h_numr->GetXaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetLabelSize(0.14);

    h_numr->GetYaxis()->SetTitle("#frac{Data}{MC}");
    h_numr->GetYaxis()->SetTitleOffset(0.35);
    h_numr->GetYaxis()->SetTitleSize(0.13);
    h_numr->GetYaxis()->SetLabelSize(0.14);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->SetMaximum(1.99);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    p_bot[i]->SetTickx();p_bot[i]->SetTicky();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("E1 X0");
    cout<<h_numr->GetName()<<endl;;
    //    printBinContents(h_numr);
    c_cA[i]->cd();    p_top[i]->cd();
    gPad->RedrawAxis();
    char name2[100];
    textOnTop.SetTextSize(0.08);
    intLumiE.SetTextSize(0.08);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
    sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.7,0.91,name2);
    TLine *line1=new TLine( 0.3,0.3,  0.3,10000);
    TLine *line2=new TLine( 0.3,0.01, 0.3,1.99);
    if(name1[i] =="mindPhi1dPhi2"){ line1->Draw(); p_bot[i]->cd();line2->Draw();}
    if(saveCanvas){name="c_"+name1[i]+".png";c_cA[i]->SaveAs(name);}
  }
  
  cout<<"Integral in Data "<<data_Integral<<endl
      <<"Integral in MC "<<mc_Integral<<endl;
}

TString getLegName(TString fname){
  if(fname.Contains("ToNuNu")){return "Z(#nu #bar{#nu})#gamma+jets";}
  else if(fname.Contains("LostEle")){return "Lost e";}
  else if(fname.Contains("LostMuHadTau")){return "Lost #mu+had #tau";}
  else if(fname.Contains("SingleEle")){return "e #rightarrow #gamma";}
  else if(fname.Contains("Run2016_Multijet_Pred") || fname.Contains("gjets")  ){return "Multijet";}
  else if(fname.Contains("Run2016_Multijet") ){return "Data";}
  else if(fname.Contains("ZGZJ")){return "Z(#nu#bar{#nu})+#gamma";}
  else if(fname.Contains("DYJetsToLL")){return "DY(l^{+}l^{-})";}
  else if(fname.Contains("WJetsToLNu")){return "W(l#nu)+jets";}
  else if(fname.Contains("TTJetsHT")){return "t #bar{t}";}
  else if(fname.Contains("QCD")){return "QCD";}
  else if(fname.Contains("WGJetsToLNuG")){return "W(l#nu)+ #gamma";}
  else if(fname.Contains("ZGJetsToNuNuG")){return "Z(#nu#bar{#nu})+ #gamma";}
  else if(fname.Contains("TTGJets")){return "t #bar{t}+ #gamma";}
  else if(fname.Contains("GJets")){return "#gamma +jets";}
  else if(fname.Contains("Run2016")){return "Data";}
  else if(fname.Contains("T5bbbbZg_1600_150")){return "T5bbbb_ZG_150";}
  else if(fname.Contains("T5bbbbZg_1600_1550")){return "T5bbbb_ZG_1550";}
  else return fname;
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
  else if(axname.Contains("mindPhi1dPhi2")) return "min(#Delta#Phi_{1},#Delta#Phi_{2})";
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

void printBinContents(TH1D *h1){
  for(int i=1;i<=h1->GetNbinsX();i++){
    cout<<h1->GetBinContent(i)<<" "<<h1->GetBinError(i)<<endl;
  }
}
