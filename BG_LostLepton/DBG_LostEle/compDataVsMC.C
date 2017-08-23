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

const int nfiles=2,nBG=1;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
TString name;
bool saveCanvas=false;
void setLastBinAsOverFlow(TH1D*);
double sr_Integral=0,cr_Integral=0;

void compDataVsMC(TString iFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
 
  f[0] = new TFile(iFname);
  f[1] = new TFile("CS_TTW_LostEle_v2.root");
  //  gStyle->SetOptStat("nemri");
  
  vector<string> name1;
  vector<int> rebin;
  name1.push_back("ST_Ele1");  rebin.push_back(10);         //name2.push_back("ST_Ele1");     
  name1.push_back("MET_Ele1");    rebin.push_back(5);     //name2.push_back("MET_Ele1");     
  name1.push_back("nBTags_Ele1");   rebin.push_back(1);    //name2.push_back("nBTags_Ele1");     
  name1.push_back("nHadJets_Ele1");  rebin.push_back(1);  //name2.push_back("nHadJets_Ele1");
  name1.push_back("BestPhotonPt_Ele1");   rebin.push_back(1);  //name2.push_back("BestPhotonPt_Ele1");      
  // name1.push_back("BestPhotonEta_Ele1"); rebin.push_back(5);  //name2.push_back("BestPhotonEta_Ele1");  
  name1.push_back("METvarBin_Ele1");  rebin.push_back(1); //name2.push_back("METvarBin_Ele1");  
  // name1.push_back("dPhi_METjet1_Pho_Ele1"); rebin.push_back(1);  //name2.push_back("dPhi_METjet1_Pho_Ele1");  
  // name1.push_back("dPhi_METjet2_Pho_Ele1"); rebin.push_back(1);  //name2.push_back("dPhi_METjet2_Pho_Ele1");  
  // name1.push_back("dPhi_METBestPhoton_Ele1"); rebin.push_back(1);   //name2.push_back("dPhi_METBestPhoton_Ele1");  
  // name1.push_back("dPhi_Electron_Photon"); rebin.push_back(1);
  name1.push_back("invMassPhoEle"); rebin.push_back(1);
  //  name1.push_back("nBestPho_Ele1"); rebin.push_back(1);
  name1.push_back("ElePt");   rebin.push_back(1);
  //  name1.push_back("EleEta");   rebin.push_back(5);
  // name1.push_back("MET_Ele1_R2");         name2.push_back("MET_Ele1_R2");rebin.push_back(1);
  // name1.push_back("MET_Ele1_R3");         name2.push_back("MET_Ele1_R3");rebin.push_back(1);

  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  //  TCanvas *c_cB=new TCanvas("closure_test","closure test",1500,800); c_cB->Divide(4,2);
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];

  for(int i=0;i<name1.size();i++){
    name=name1[i];//+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    p_top[i]=new TPad(name+"_top",name+"_top",0,0.35,1,1);
    p_bot[i]=new TPad(name+"_bot",name+"_bot",0,0.04,1,0.35);
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.2);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy();    

    
    TH1D *h_histG,*h_histE,*h_histGcopy;
    //for(int j=0;j<nfiles;j++){
    c_cA[i]->cd();
    name=name1[i];
    h_histG=(TH1D*)f[0]->FindObjectAny(name);//h_histG->Rebin(2);
    h_histG->Rebin(rebin[i]);//h_histG->Scale(1.0/h_histG->Integral());
          
    name=name1[i];
    h_histE=(TH1D*)f[1]->FindObjectAny(name);//h_histE->Scale(150.9/481.6);
    h_histE->Rebin(rebin[i]);//h_histE->Scale(1.0/h_histE->Integral());
     
    if(h_histG && h_histE){
      if(name1[i]=="nHadJets_Ele1"){
	sr_Integral=h_histG->Integral();
	cr_Integral=h_histE->Integral();
      }
      setLastBinAsOverFlow(h_histG);
      setLastBinAsOverFlow(h_histE);

      h_histG->SetLineColor(kBlack);
      h_histG->SetLineWidth(2);
      h_histG->SetMarkerStyle(20);
      h_histG->SetMarkerColor(h_histG->GetLineColor());
      h_histG->SetTitle(";;Events");
	
      h_histE->SetLineColor(kBlue);
      h_histE->SetLineWidth(2);
      h_histE->SetMarkerStyle(21);
      h_histE->SetMarkerColor(h_histE->GetLineColor());
      h_histE->SetFillColor(h_histE->GetLineColor());
      h_histE->GetYaxis()->SetLabelSize(0.08);
      h_histE->GetYaxis()->SetTitleSize(0.08);
      h_histE->GetYaxis()->SetTitleOffset(0.45);

      c_cA[i]->cd();p_top[i]->cd();
      //c_cB->cd(i+1);p_top[i]->cd();
      h_histE->Draw("BAR");
      h_histG->Draw("same");
      // h_histE->SetFillStyle(3004);
      // h_histE->SetFillColor(h_histE->GetLineColor());
      //	h_histE->Draw("L same");

      legend[i]=new TLegend(0.8, 0.90,  0.90, 0.7);
      //      name=name1[i];
      legend[i]->AddEntry(h_histG,"Data","lep");
      //      name=name1[i];
      legend[i]->AddEntry(h_histE,"MC","f");
      legend[i]->Draw();
    
    }
    
    TH1D *h_numr=(TH1D*)h_histG->Clone();
    //!!!!!TH1D *h_numr=(TH1D*)h_histG->Clone();
    // if(name1[i]=="MET_Pho") {cout<<"numr Ele:"<<h_numr->GetBinContent(7)<<" numr error:"<<h_numr->GetBinError(7)<<" den pho:"<<h_histG->GetBinContent(7)<<" error:"<<h_histG->GetBinError(7)<<endl;
    // }
    h_numr->Divide(h_histE);
    
    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    h_numr->SetTitle(";;#frac{Data}{MC}");
    h_numr->GetYaxis()->SetTitleOffset(0.29);
    h_numr->GetYaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetLabelSize(0.17);
    h_numr->GetYaxis()->SetLabelSize(0.16);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->SetMaximum(2.0);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e0");
    if(saveCanvas){name="c_"+name1[i]+".png";c_cA[i]->SaveAs(name);}
  }  
  gStyle->SetTextSize(2);
  //fout->cd();
  //  h2_histGcopy->Write();
  cout<<"Integral in SR(0 Ele events) "<<sr_Integral<<endl
      <<"Integral in CR(1 Ele events) "<<cr_Integral<<endl;
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
