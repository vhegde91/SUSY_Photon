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

class c_getBGHists{
public:
  TFile *f[5],*fl;
  TString fName[5],fOutName;
  TFile *fout;

  void getLEleHist();
  void getLMuTauHist();
  void getFRHist();
  void getZGHist();
  void getMultiJHist();
  int getSBinNumV7(int nHadJets,int BTags,double MET){
    vector<double> METBinLowEdgeV7={0,100,200,270,350,450,750};
    if(BTags==0 && nHadJets>=2 && nHadJets<=4 && MET > 750) return 6;
    int sBin=-100,m_i=0;
    if(BTags==0){
      if(nHadJets>=2 && nHadJets<=4)     { sBin=0;}
      else if(nHadJets==5 || nHadJets==6){ sBin=6;}
      else if(nHadJets>=7)               { sBin=11;}
    }
    else{
      if(nHadJets>=2 && nHadJets<=4)     { sBin=16;}
      else if(nHadJets==5 || nHadJets==6){ sBin=21;}
      else if(nHadJets>=7)               { sBin=26;}
    }
    for(int i=0;i<METBinLowEdgeV7.size()-1;i++){
      if(METBinLowEdgeV7[i]<99.99) continue;
      m_i++;
      if(MET >= METBinLowEdgeV7[i] && MET < METBinLowEdgeV7[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METBinLowEdgeV7[METBinLowEdgeV7.size()-1])  { sBin = sBin+5   ;break; }
    }
    return sBin;
  }
  void setFileNames();
  void printContents(TH1D*);
  void setNegContentsTo0(TH1D*);
}c1;

void getBGHists(){
  c_getBGHists c1;
  c1.setFileNames();
  c1.getLEleHist();
  c1.getLMuTauHist();
  c1.getFRHist();
  c1.getZGHist();
  c1.getMultiJHist();
}

void c_getBGHists::setFileNames(){
  fName[0] = "DCS_Run2016_LostEle_v2.root";
  fName[1] = "DCS_Run2016_LostMuHadTau_v2.root";
  fName[2] = "DCS_Run2016_SingleEle_v2.root";
  fName[3] = "CS_ZGZJToNuNuG.root";
  fName[4] = "DCS_LDP_Run2016_Multijet_v2.root";

  fOutName = "SBinHists.root";

  fout = new TFile(fOutName,"recreate");
}

void c_getBGHists::getLEleHist(){
  fl = TFile::Open(fName[0]);
  TFile *flUp=TFile::Open("DCS_Run2016_LostEle_TFup_v2.root");
  TH1D *hCS,*hTemp,*hTF,*hPred;
  //----- DCS hist
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Ele1");
  hCS = (TH1D*)hTemp->Clone("AllSBins_v7_LEleCS");
  //-----get TF in SBins
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Ele0");
  hTF = (TH1D*)hTemp->Clone("LEleTFs");
  hTF->Divide((TH1D*)fl->Get("AllSBins_v7_Ele1"));
  //-----get TF unc in SBins
  hTemp=(TH1D*)flUp->Get("AllSBins_v7_Ele0");
  TH1D *hTFUnc=(TH1D*)hTemp->Clone("hTFUnc");
  hTFUnc->Divide((TH1D*)flUp->Get("AllSBins_v7_Ele1"));
  hTFUnc->Add(hTF,-1);
  for(int i=1;i<=hTF->GetNbinsX();i++){
    hTF->SetBinError(i,hTFUnc->GetBinContent(i));
  }
  hTF->SetBinContent(6,0.);  hTF->SetBinError(6,0.);
  hTF->SetBinContent(13,0.337721);  hTF->SetBinError(13,0.0771929);
  hTF->SetBinContent(16,0.337721);  hTF->SetBinError(16,0.0771929);
  hTF->SetBinContent(20,0.560793);  hTF->SetBinError(20,0.0501785);
  hTF->SetBinContent(29,0.346311);  hTF->SetBinError(29,0.0597414);

  //-----multiply DCS with TF histogram with TF unc
  hPred=(TH1D*)hCS->Clone("AllSBins_v7_LElePred");
  hPred->Multiply(hTF);
  hPred->SetBinError(6,1.14*hTF->GetBinContent(6));
  hPred->SetBinError(13,1.14*hTF->GetBinContent(13));
  hPred->SetBinError(16,1.14*hTF->GetBinContent(16));
  hPred->SetBinError(20,1.14*hTF->GetBinContent(20));
  hPred->SetBinError(29,1.14*hTF->GetBinContent(29));
  //---------------------
  fout->cd();
  hCS->Write();
  hTF->Write();
  hPred->Write();

  c1.printContents(hTF);
  c1.printContents(hPred);
}

void c_getBGHists::getLMuTauHist(){
  fl = TFile::Open(fName[1]);
  TFile *flUp=TFile::Open("DCS_Run2016_LostMuHadTau_TFup_v2.root");
  TH1D *hCS,*hTemp,*hTF,*hPred;
  //----- DCS hist
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Mu1");
  hCS = (TH1D*)hTemp->Clone("AllSBins_v7_LMuCS");
  //-----get TF in SBins
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Mu0");
  hTF = (TH1D*)hTemp->Clone("LMuTFs");
  hTF->Divide((TH1D*)fl->Get("AllSBins_v7_Mu1"));
  //-----get TF unc in SBins
  hTemp=(TH1D*)flUp->Get("AllSBins_v7_Mu0");
  TH1D *hTFUnc=(TH1D*)hTemp->Clone("hTFUnc");
  hTFUnc->Divide((TH1D*)flUp->Get("AllSBins_v7_Mu1"));
  hTFUnc->Add(hTF,-1);
  for(int i=1;i<=hTF->GetNbinsX();i++){
    hTF->SetBinError(i,hTFUnc->GetBinContent(i));
  }
  hTF->SetBinContent(13,0.703993);  hTF->SetBinError(13,0.098179);
  hTF->SetBinContent(16,0.750494);  hTF->SetBinError(16,0.0984693);
  hTF->SetBinContent(26,0.797014);  hTF->SetBinError(26,0.0704897);
  hTF->SetBinContent(30,0.706147);  hTF->SetBinError(30,0.0969159);
  hTF->SetBinContent(31,0.706147);  hTF->SetBinError(31,0.0969159);

  //-----multiply DCS with TF histogram with TF unc
  hPred=(TH1D*)hCS->Clone("AllSBins_v7_LMuPred");
  hPred->Multiply(hTF);
  hPred->SetBinError(13,1.14*hTF->GetBinContent(13));
  hPred->SetBinError(16,1.14*hTF->GetBinContent(16));
  hPred->SetBinError(26,1.14*hTF->GetBinContent(26));
  hPred->SetBinError(30,1.14*hTF->GetBinContent(30));
  hPred->SetBinError(31,1.14*hTF->GetBinContent(31));
  //---------------------
  fout->cd();
  hCS->Write();
  hTF->Write();
  hPred->Write();

  // c1.printContents(hTF);
  // c1.printContents(hPred);
}

void c_getBGHists::getFRHist(){
  fl = TFile::Open(fName[2]);
  TH1D *h1 = (TH1D*)fl->Get("AllSBins_v7_Ele");
  TH1D *h1cp = (TH1D*)h1->Clone("AllSBins_v7_FRCS");
  fout->cd(); h1cp->Write();
  
  h1 = (TH1D*)fl->Get("AllSBins_v7_Pho");
  h1cp = (TH1D*)h1->Clone("AllSBins_v7_FRPred");
  fout->cd(); h1cp->Write();
  //  c1.printContents(h1cp);
  // TH2D *h2_num=(TH2D*)fl->Get("METNJ_Mu0_R0");
  // TH2D *h2_den=(TH2D*)fl->Get("METNJ_Mu1_R0");
}

void c_getBGHists::getZGHist(){
  fl = TFile::Open(fName[3]);
  TH1D *h1 = (TH1D*)fl->Get("AllSBins_v7");
  TH1D *h1cp = (TH1D*)h1->Clone("AllSBins_v7_ZGCS");
  c1.setNegContentsTo0(h1cp);
  fout->cd(); h1cp->Write();
  //c1.printContents(h1cp);
  
  // h1 = (TH1D*)fl->Get("AllSBins_v7_Pho");
  // h1cp = (TH1D*)h1->Clone("AllSBins_v7_FRPred");
  // fout->cd(); h1cp->Write();

}

void c_getBGHists::getMultiJHist(){
  fl = TFile::Open(fName[4]);
  TH1D *h1 = (TH1D*)fl->Get("AllSBins_v7_AB");
  TH1D *h1cp = (TH1D*)h1->Clone("AllSBins_v7_MultiJCS");
  fout->cd(); h1cp->Write();
  //  c1.printContents(h1cp);
  // h1 = (TH1D*)fl->Get("AllSBins_v7_Pho");
  // h1cp = (TH1D*)h1->Clone("AllSBins_v7_FRPred");
  // fout->cd(); h1cp->Write();
  //  c1.printContents(h1cp);
  // TH2D *h2_num=(TH2D*)fl->Get("METNJ_Mu0_R0");
  // TH2D *h2_den=(TH2D*)fl->Get("METNJ_Mu1_R0");
}


//------------------------------------------------
void c_getBGHists::setNegContentsTo0(TH1D *h1){
  for(int i=1;i<=h1->GetNbinsX();i++){
    if(h1->GetBinContent(i) < 0.00001){
      h1->SetBinContent(i,0);
      h1->SetBinError(i,0);
    }
  }
}
void c_getBGHists::printContents(TH1D *h1){
  bool showBinError=1;
  cout<<h1->GetName()<<endl;
  for(int i=1;i<=h1->GetNbinsX();i++){
    if(showBinError) cout<<h1->GetBinContent(i)<<" "<<h1->GetBinError(i)<<endl;
    else cout<<h1->GetBinContent(i)<<endl;
  }
}


/*
int c_getBGHists::setLostMuTF(int nj, int nb, double met){
  double tf = -1, errTF = -1;
  if(nb==0){
    if(met >=100 && met < 150){
      if( nj==2 ){ tf = 1.05233; errTF = 0.07050; }
      if( nj==3 ){ tf = 0.83991; errTF = 0.05982; }
      if( nj==4 ){ tf = 0.80807; errTF = 0.06198; }
      if( (nj==5 || nj==6) ){ tf = 0.66948; errTF = 0.06044; }
      if( nj>=7 ){ tf = 0.63424; errTF = 0.09774; }
    }else if(met >= 150){
      if( nj==2 ){ tf = 1.22766; errTF = 0.07688; }
      if( nj==3 ){ tf = 0.96841; errTF = 0.05229; }
      if( nj==4 ){ tf = 0.78345; errTF = 0.05006; }
      if( (nj==5 || nj==6) ){ tf = 0.65053; errTF = 0.04211; }
      if( nj>=7 ){ tf = 0.75049; errTF = 0.09847; }
    }
  }
  else{
    if(met >=100 && met < 150){
      if( (nj>=2 && nj<=4) ){ tf = 0.86572; errTF = 0.08514; }
      if( (nj==5 || nj==6) ){ tf = 0.69923; errTF = 0.07000; }
      if( nj>=7 )           { tf = 0.67469; errTF = 0.09585; }
    }else if(met >= 150){
      if( (nj>=2 && nj<=4) ){ tf = 1.10313; errTF = 0.09891; }
      if( (nj==5 || nj==6) ){ tf = 0.79701; errTF = 0.07049; }
      if( nj>=7 )           { tf = 0.70615; errTF = 0.09692; }
    }
  }

  if(tf > 0.00001) LMuHadTauTF = tf;
  if(errTF > 0.000001) UncLMuHadTauTF = errTF;

  if(LMuHadTauTF >= 0 && UncLMuHadTauTF >=0) return 1;
  else return -1;
}
*/
