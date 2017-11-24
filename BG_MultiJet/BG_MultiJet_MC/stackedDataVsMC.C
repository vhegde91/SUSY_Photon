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

const int nfiles=2;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
int col[11]={kBlack,kMagenta+2,kTeal+9,kGreen,kOrange,kCyan,kYellow,kPink+1,kBlue,kRed,kMagenta};  //Specify Colors
//int col[10]={kBlack,kMagenta+2,kOrange,kYellow,kGreen,kTeal+9,kPink+1,kCyan,kBlue,kRed};  //Specify Colors
TString name;
bool saveCanvas=0,saveFile=1;
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
  vector<TString> suffixHist; 
  //  f[0] = new TFile(iFname);                                          suffixHist.push_back("_AB");
  // f[1] = new TFile("CS_LDP_ZGZJToNuNuG.root");                       suffixHist.push_back("");
  // f[2] = new TFile("DCS_LDP_Run2016_LostEle_v2.root");               suffixHist.push_back("_Ele0");
  // f[3] = new TFile("DCS_LDP_Run2016_LostMuHadTau_v2.root");          suffixHist.push_back("_Mu0");
  // f[4] = new TFile("DCS_LDP_Run2016_SingleEle_v2.root");             suffixHist.push_back("_Pho");
  // f[5] = new TFile("gjets_qcd.root");                                suffixHist.push_back("_AB");

  f[0] = new TFile(iFname);                                          suffixHist.push_back("_AB");
  // f[1] = new TFile("CS_c_ZGZJToNuNuG.root");                       suffixHist.push_back("");
  // f[2] = new TFile("DCS_c_Run2016_LostEle_v2.root");               suffixHist.push_back("_Ele0");
  // f[3] = new TFile("DCS_c_Run2016_LostMuHadTau_v2.root");          suffixHist.push_back("_Mu0");
  // f[4] = new TFile("DCS_c_Run2016_SingleEle_v2.root");             suffixHist.push_back("_Pho");
  f[1] = new TFile("gjets_qcd.root");                                suffixHist.push_back("_AB");

  //f[5] = new TFile("GJetsQCD_LDPSFs.root");                          suffixHist.push_back("_AB");

  TFile *fout;
  vector<string> name1;
  vector<int> rebin;
  name1.push_back("ST");  rebin.push_back(10);         //name2.push_back("ST_Mu1");     
  name1.push_back("MET");    rebin.push_back(5);     //name2.push_back("MET_Mu1");     
  //  name1.push_back("METPhi");    rebin.push_back(1);     //name2.push_back("MET_Mu1");     
  name1.push_back("nBTags");   rebin.push_back(1);    //name2.push_back("nBTags_Mu1");     
  name1.push_back("nHadJets");  rebin.push_back(1);  //name2.push_back("nHadJets_Mu1");
  //name1.push_back("AllSBins_v4");  rebin.push_back(1);  //name2.push_back("nHadJets_Mu1");
  name1.push_back("BestPhotonPt");   rebin.push_back(5);  //name2.push_back("BestPhotonPt_Mu1");      
  name1.push_back("BestPhotonEta");   rebin.push_back(2);
  name1.push_back("BestPhotonPhi");   rebin.push_back(2);
  name1.push_back("nVtx");   rebin.push_back(2);
  //name1.push_back("METvarBin");  rebin.push_back(1); //name2.push_back("METvarBin_Mu1");  
  //name1.push_back("dPhi_METjet1_Pho_Mu1"); rebin.push_back(2);  //name2.push_back("dPhi_METjet1_Pho_Mu1");  
  // name1.push_back("dPhi_METjet2_Pho_Mu1"); rebin.push_back(2);  //name2.push_back("dPhi_METjet2_Pho_Mu1");  
  // name1.push_back("dPhi_METBestPhoton_Mu1"); rebin.push_back(2);   //name2.push_back("dPhi_METBestPhoton_Mu1");  
  name1.push_back("mTPho");   rebin.push_back(2);
  name1.push_back("dPhi_METBestPhoton");   rebin.push_back(1);
  // name1.push_back("AllSBins_Mu1");   rebin.push_back(1);


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
    legend[i]=new TLegend(0.7, 0.88,  0.86, 0.45);
  }
  //cout<<getLegName(f[0]->GetName());
  TH1D *h_histG,*h_histE,*h_histGcopy;
  for(int i=0;i<name1.size();i++){
    c_cA[i]->cd();
    name=name1[i]+suffixHist[0];
    h_histG=(TH1D*)f[0]->FindObjectAny(name);
    if(!h_histG) cout<<"Hist Not Found: "<<name<<" in file "<<f[0]->GetName()<<endl;
    h_histG->Rebin(rebin[i]);
    setLastBinAsOverFlow(h_histG);
    if(name.Contains("nHadJets")) data_Integral=h_histG->Integral();
    c_cA[i]->cd();p_top[i]->cd();
    p_top[i]->SetTickx();p_top[i]->SetTicky();
    h_histG->SetLineColor(kBlack);
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
      name=name1[i]+suffixHist[p];
      if(name=="BestPhotonPt_Pho" && p==4) name = "BestPhotonPt";
      if(name=="BestPhotonEta_Pho" && p==4) name = "BestPhotonEta";
      if(name=="BestPhotonPhi_Pho" && p==4) name = "BestPhotonPhi";
      if(name=="dPhi_METBestPhoton_Pho" && p==4) name = "dPhi_METBestPhoton";
      if(name=="mTPho_Pho" && p==4) name = "mT_Pho";

      h_histE=(TH1D*)f[p]->FindObjectAny(name);
      if(!h_histE) cout<<"Hist Not Found: "<<name<<" in file "<<f[p]->GetName()<<endl;
      h_histE->Rebin(rebin[i]);
      setLastBinAsOverFlow(h_histE);
      if(name.Contains("nHadJets")){
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
    h_numr->Draw("e0");
    cout<<h_numr->GetName()<<endl;;
    c_cA[i]->cd();    p_top[i]->cd();
    gPad->RedrawAxis();
    char name2[100];
    textOnTop.SetTextSize(0.06);
    intLumiE.SetTextSize(0.06);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Preliminary}}");
    intLumiE.SetTextSize(0.06);
    sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.73,0.91,name2);
    if(saveCanvas){name="c_"+name1[i]+".png";c_cA[i]->SaveAs(name);}
    if(saveFile){
      name = h_numr->GetName();
      if(name.Contains("nHadJets")){
	fout = new TFile("Multijet_SF.root","recreate");
	fout->cd(); h_numr->Write();
      }
    }    
  }
  
  cout<<"Integral in Data "<<data_Integral<<endl
      <<"Integral in MC "<<mc_Integral<<endl;
}

TString getLegName(TString fname){
  if(fname.Contains("ToNuNu")){return "Z( #rightarrow #nu #bar{#nu})#gamma+jets";}
  else if(fname.Contains("LostEle")){return "Lost e";}
  else if(fname.Contains("LostMuHadTau")){return "Lost #mu+had #tau";}
  else if(fname.Contains("SingleEle")){return "e #rightarrow #gamma";}
  else if(fname.Contains("GJets") || fname.Contains("gjets")  ){return "#gamma +jets";}
  else if(fname.Contains("Run2016_Multijet") ){return "Data";}
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



/*


	double lastBinCt =h_histG->GetBinContent(h_histG->GetNbinsX()),overflCt =h_histG->GetBinContent(h_histG->GetNbinsX()+1);
	double lastBinErr=h_histG->GetBinError(h_histG->GetNbinsX()),  overflErr=h_histG->GetBinError(h_histG->GetNbinsX()+1);
	lastBinEr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );
	lastBinCt = lastBinCt+overflCt;
	h_histG->SetBinContent(h_histG->GetNbinsX(),h_histG->GetBinContent(h_histG->GetNbinsX())+h_histG->GetBinContent(h_histG->GetNbinsX()+1));
	h_histE->SetBinContent(h_histE->GetNbinsX(),h_histE->GetBinContent(h_histE->GetNbinsX())+h_histE->GetBinContent(h_histE->GetNbinsX()+1));
*/
