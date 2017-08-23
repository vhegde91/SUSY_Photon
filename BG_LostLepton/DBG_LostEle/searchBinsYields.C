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

TString name;
TLatex textOnTop,intLumiE;

void setLastBinAsOverFlow(TH1D*);
void searchBinsYields(TString iFname){
  double data_Integral=0,mc_Integral=0;
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);

  TFile *fData = new TFile(iFname);
  TFile *fMC = new TFile("CS_TTW_LostEle.root");
  //  TFile *fMC = new TFile("CS_TTW_LostEle_NoGMatchE.root");

  double intLumi=-1,intLumiData=0,intLumiMC=-10;  

  TH1D *h_intLumi=(TH1D*)fData->FindObjectAny("intLumi");
  if(h_intLumi) intLumiData=h_intLumi->GetMean();

  h_intLumi=(TH1D*)fMC->FindObjectAny("intLumi");
  if(h_intLumi) intLumiMC=h_intLumi->GetMean();
  if(abs(intLumiData-intLumiMC)>0.001) cout<<endl<<"Int Lumi in Data: "<<intLumiData<<" Int Lumi in MC:"<<intLumiMC<<endl;
  else intLumi=intLumiMC;
  delete h_intLumi;

  TCanvas *c_cA=new TCanvas("allSearchBins_LostEleCS","CS yields in all search bins for lost electron",2000,900);
  TPad *p_top=new TPad("top","top",0,0.35,1,1);
  TPad *p_bot=new TPad("bot","bot",0,0.04,1,0.35);
  p_top->Draw();p_top->SetGridx();p_top->SetGridy();p_top->SetLogy();
  p_top->SetBottomMargin(0);
  p_bot->SetTopMargin(0);
  p_bot->SetBottomMargin(0.2);
  p_bot->Draw();p_bot->SetGridx();p_bot->SetGridy();

  TH1D *h_Data,*h_mc,*h_Datacopy,*h_temp;
  TH1D *h_CSmc=new TH1D("searchBins_Ele1mc","mc searchBins 1 Ele:NJ[2,4][5,6],[7,inf)",21,0.5,21.5);
  TH1D *h_CSdata=new TH1D("searchBins_Ele1data","data searchBins 1 Ele:NJ[2,4][5,6],[7,inf)",21,0.5,21.5);

  for(int i=1;i<=3;i++){
    name="MET_Ele1_R"+to_string(i);
    h_Data=(TH1D*)fData->FindObjectAny(name);

    name="MET_Ele1_R"+to_string(i);
    h_mc=(TH1D*)fMC->FindObjectAny(name);

    setLastBinAsOverFlow(h_Data);
    setLastBinAsOverFlow(h_mc);
    data_Integral+=h_Data->Integral();
    mc_Integral+=h_mc->Integral();
    /*    for(int i=1;i<=h_mc->GetNbinsX();i++){
      cout<<i<<" "<<h_mc->GetBinContent(i)<<"\t"<<h_mc->GetBinError(i)<<endl;
      }*/
    for(int j=6;j<=h_Data->GetNbinsX();j++){
      //      cout<<name<<" i,j: "<<i<<","<<j<<" low "<<h_Data->GetBinLowEdge(j)<<" cont "<<h_Data->GetBinContent(j)<<" err "<<h_Data->GetBinError(j)<<" fill # "<<((i-1)*7)+j-5<<endl;
      h_CSdata->SetBinContent( ((i-1)*7)+j-5,h_Data->GetBinContent(j) );
      h_CSdata->SetBinError  ( ((i-1)*7)+j-5,h_Data->GetBinError(j) );
      h_CSmc->SetBinContent( ((i-1)*7)+j-5,h_mc->GetBinContent(j) );
      h_CSmc->SetBinError  ( ((i-1)*7)+j-5,h_mc->GetBinError(j) );

      h_CSdata->SetLineColor(kBlack);
      h_CSdata->SetLineWidth(2);
      h_CSdata->SetMarkerStyle(20);
      h_CSdata->SetMarkerColor(h_CSdata->GetLineColor());
      h_CSdata->SetFillColor(h_CSdata->GetLineColor());
      //      h_CSdata->SetFillStyle(3003);

      h_CSmc->SetLineColor(kBlue);
      h_CSmc->SetLineWidth(2);
      h_CSmc->SetMarkerStyle(21);
      h_CSmc->SetMarkerColor(h_CSmc->GetLineColor());
      h_CSmc->GetYaxis()->SetLabelSize(0.09);
      h_CSmc->SetTitle(";;Events");
      h_CSmc->SetFillColor(h_CSmc->GetLineColor());
      //      h_CSmc->GetYaxis()->SetTitle("Events");
      h_CSmc->GetYaxis()->SetTitleSize(0.07);
      h_CSmc->GetYaxis()->SetTitleOffset(0.51);
    }
  }

  for(int i=1;i<=h_CSmc->GetNbinsX();i++){
    cout<<" "<<h_CSmc->GetBinContent(i)<<"\t"<<h_CSmc->GetBinError(i)<<endl;
  }
  cout<<"-----------------------------------"<<endl;
  for(int i=1;i<=h_CSdata->GetNbinsX();i++){
    cout<<" "<<h_CSdata->GetBinContent(i)<<"\t"<<h_CSdata->GetBinError(i)<<endl;
  }

  TH1D *h_numr=(TH1D*)h_CSdata->Clone();
  h_numr->Divide(h_CSmc);
  h_numr->SetLineColor(kBlack);
  h_numr->SetMarkerColor(kBlack);
  //  h_numr->SetTitle(";;#frac{0#e+#gamma}{1#e+#gamma}");
  h_numr->SetTitle(";Bin Number;Data / MC");
  h_numr->GetYaxis()->SetTitleOffset(0.14);
  h_numr->GetYaxis()->SetTitleSize(0.16);
  h_numr->GetXaxis()->SetTitleSize(0.2);
  h_numr->GetXaxis()->SetLabelSize(0.15);
  h_numr->GetXaxis()->SetTitleOffset(0.67);
  h_numr->GetYaxis()->SetLabelSize(0.13);
  h_numr->GetYaxis()->SetNdivisions(505);
  h_numr->SetMaximum(2.0);
  h_numr->SetMinimum(0.01);
  c_cA->cd();    p_bot->cd();
  h_numr->Draw("e0");
  // c_cA->SetLogy();
  // c_cA->SetGridx();
 
  c_cA->cd();p_top->cd();
  h_CSmc->Draw("BAR");
  h_CSdata->Draw("same");

  TLegend *legend=new TLegend(0.75, 0.90,  0.90, 0.75);
  legend->AddEntry(h_CSdata,"1e+#gamma (Data)","lep");
  legend->AddEntry(h_CSmc,"1e+#gamma (MC)","f");
  legend->Draw();
  
  char name2[100];  
  textOnTop.SetTextSize(0.06);
  intLumiE.SetTextSize(0.06);
  textOnTop.DrawLatexNDC(0.1,0.91,"CMS #it{#bf{Preliminary}}");
  intLumiE.SetTextSize(0.06);
  sprintf(name2,"#bf{%0.1f fb^{-1}(13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.73,0.91,name2);
  c_cA->SaveAs("a.png");

  cout<<"Events in Data "<<data_Integral<<endl
      <<"Integral in MC "<<mc_Integral<<endl;
  // TFile *fout=new TFile("LostEle_SB.root","RECREATE");                                                                            
  // h_CSmc->Write();
  // h_CSdata->Write();
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
