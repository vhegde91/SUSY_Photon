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
  TFile *f[10],*fl;
  TString fName[10],fOutName;
  TFile *fout;
  //  TH1D *hLElePredLDP, *hLMuPredLDP, *hFRPredLDP, *hZGPredLDP;

  void getLEleHist(int);
  void getLMuTauHist(int);
  void getFRHist(int);
  void getZGHist(int);
  void getMultiJHist(int);
  void getTotalBG(int);
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
  void assignPCuncAllBins(TH1D *h1,double pc){
    for(int i=1;i<=h1->GetNbinsX();i++){
      h1->SetBinError(i,pc*0.01*h1->GetBinContent(i));
      h1->SetBinContent(i,0.);
    }
  }
}c1;

void getBGHists(){
  c_getBGHists c1;
  c1.setFileNames();
  
  c1.getLEleHist(0);
  c1.getLMuTauHist(1);
  c1.getFRHist(2);
  c1.getZGHist(3);
  
  c1.getLEleHist(5);
  c1.getLMuTauHist(6);
  c1.getFRHist(7);
  c1.getZGHist(8);

  c1.getMultiJHist(4);

  c1.getTotalBG(9);
}

void c_getBGHists::setFileNames(){
  fName[0] = "DCS_Run2016_LostEle_v2.root";
  fName[1] = "DCS_Run2016_LostMuHadTau_v2.root";
  fName[2] = "DCS_Run2016_SingleEle_v2.root";
  fName[3] = "CS_ZGZJToNuNuG_LO.root";//CS_ZGZJToNuNuG.root";
  fName[4] = "DCS_LDP_Run2016_Multijet_v2.root";
  //fName[4] = "a.root";

  fName[5] = "DCS_LDP_Run2016_LostEle_v2.root";
  fName[6] = "DCS_LDP_Run2016_LostMuHadTau_v2.root";
  fName[7] = "DCS_LDP_Run2016_SingleEle_v2.root";
  fName[8] = "CS_LDP_ZGZJToNuNuG_LO.root";//CS_LDP_ZGZJToNuNuG.root";

  fName[9] = "DCS_LDP_Run2016_Multijet_v2.root";
  //fName[9] = "Slewed_DCS_LDP_Run2016_Multijet_v2.root";
  //fName[9] = "a.root";

  fOutName = "SBinHists.root";

  fout = new TFile(fOutName,"recreate");
}

void c_getBGHists::getLEleHist(int i_f){
  bool isLDP = (fName[i_f].Contains("LDP"));
  fl = TFile::Open(fName[i_f]);
  TFile *flUnc;
  if(!isLDP)  flUnc=TFile::Open("DCS_Run2016_LostEle_TFUncSq_v2.root");
  else flUnc=TFile::Open("DCS_LDP_Run2016_LostEle_TFUncSq_v2.root");
  TH1D *hCS,*hTemp,*hTF,*hPred;
  //----- DCS hist
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Ele1");
  if(!isLDP) hCS = (TH1D*)hTemp->Clone("AllSBins_v7_LEleCS");
  else hCS = (TH1D*)hTemp->Clone("AllSBins_v7_LEleCS_LDP");
  //hCS->SetBinErrorOption(TH1::kPoisson);
  //-----get TF in SBins
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Ele0");
  if(!isLDP)  hTF = (TH1D*)hTemp->Clone("LEleTFs");
  else   hTF = (TH1D*)hTemp->Clone("LEleTFs_LDP");
  hTF->Divide(hCS);
  //-----get TF unc in SBins
  hTemp=(TH1D*)flUnc->Get("AllSBins_v7_Ele0");
  TH1D *hTFUnc=(TH1D*)hTemp->Clone("hTFUnc");
  hTFUnc->Divide((TH1D*)flUnc->Get("AllSBins_v7_Ele1"));
  for(int i=1;i<=hTF->GetNbinsX();i++){
    hTF->SetBinError(i,sqrt(hTFUnc->GetBinContent(i)));
  }
  if(!isLDP){
    hTF->SetBinContent( 6,0.384939);  hTF->SetBinError( 6,0.0355985);//0.362095);  hTF->SetBinError( 6,0.0328);//special case
    hTF->SetBinContent(13,hTF->GetBinContent(14));  hTF->SetBinError(13,hTF->GetBinError(14));
    hTF->SetBinContent(16,hTF->GetBinContent(15));  hTF->SetBinError(16,hTF->GetBinError(15));
    hTF->SetBinContent(20,hTF->GetBinContent(19));  hTF->SetBinError(20,hTF->GetBinError(19));
    hTF->SetBinContent(29,hTF->GetBinContent(28));  hTF->SetBinError(29,hTF->GetBinError(28));
  }
  else{
    hTF->SetBinContent( 6,0.174976);  hTF->SetBinError( 6,0.0334641);//6,0.231834);  hTF->SetBinError( 6,0.039783);
    hTF->SetBinContent(11,hTF->GetBinContent(10));  hTF->SetBinError(11,hTF->GetBinError(10));
    hTF->SetBinContent(14,hTF->GetBinContent(13));  hTF->SetBinError(14,hTF->GetBinError(13));
    hTF->SetBinContent(16,hTF->GetBinContent(15));  hTF->SetBinError(16,hTF->GetBinError(15));
    hTF->SetBinContent(21,hTF->GetBinContent(20));  hTF->SetBinError(21,hTF->GetBinError(20));
    hTF->SetBinContent(31,hTF->GetBinContent(30));  hTF->SetBinError(31,hTF->GetBinError(30));
  }
  //--------assign 12% unc for dR(l,photon)
  TH1D *hdRLepPho;
  if(!isLDP) hdRLepPho=(TH1D*)hTF->Clone("AllSBins_v7_UncTF_dRElePho");
  else hdRLepPho=(TH1D*)hTF->Clone("AllSBins_v7_UncTF_dRElePho_LDP");
  c1.assignPCuncAllBins(hdRLepPho,12.0);
  //---------------------
  //------assign 5% unc for pdf Scale weights
  TH1D *hPdf;
  if(!isLDP) hPdf=(TH1D*)hTF->Clone("AllSBins_v7_UncLEleTF_pdf");
  else hPdf=(TH1D*)hTF->Clone("AllSBins_v7_UncLEleTF_pdf_LDP");
  c1.assignPCuncAllBins(hPdf,5.0);
  //------assign 2% unc for b-tagging.
  // TH1D *hBTag;
  // if(!isLDP) hBTag=(TH1D*)hTF->Clone("AllSBins_v7_UncLEleTF_bTag");
  // else hBTag=(TH1D*)hTF->Clone("AllSBins_v7_UncLEleTF_bTag_LDP");
  // c1.assignPCuncAllBins(hBTag,2.0);
  //------assign 2% unc for lepton SF
  TH1D *hLepSF;
  if(!isLDP) hLepSF=(TH1D*)hTF->Clone("AllSBins_v7_UncLEleTF_LepSF");
  else hLepSF=(TH1D*)hTF->Clone("AllSBins_v7_UncLEleTF_LepSF_LDP");
  c1.assignPCuncAllBins(hLepSF,2.0);
  //------assign 2% unc for JEC and mT
  TH1D *hJEC;
  if(!isLDP) hJEC=(TH1D*)hTF->Clone("AllSBins_v7_UncLEleTF_JEC");
  else hJEC=(TH1D*)hTF->Clone("AllSBins_v7_UncLEleTF_JEC_LDP");
  c1.assignPCuncAllBins(hJEC,2.0);
  //-------get final TF with all unc added in quadrature
  TH1D *hTF_final;
  if(!isLDP) hTF_final=(TH1D*)hTF->Clone("LEleTFs_Final");
  else hTF_final=(TH1D*)hTF->Clone("LEleTFs_Final_LDP");
  hTF_final->Add(hdRLepPho);
  hTF_final->Add(hPdf);
  //  hTF_final->Add(hBTag);
  hTF_final->Add(hLepSF);
  hTF_final->Add(hJEC);
  //-----multiply DCS with final TF histogram with TF unc
  if(!isLDP) hPred=(TH1D*)hCS->Clone("AllSBins_v7_LElePred");
  else hPred=(TH1D*)hCS->Clone("AllSBins_v7_LElePred_LDP");
  // hPred->SetBinErrorOption(TH1::kPoisson);
  hPred->Multiply(hTF_final);
  for(int i=1;i<=hPred->GetNbinsX();i++){
    if(hPred->GetBinContent(i) < 0.00001)
      hPred->SetBinError(i,1.8*hTF_final->GetBinContent(i));
  }

  fout->cd();
  hCS->Write();
  hTF->Write();
  hdRLepPho->Write();
  hPdf->Write();
  //  hBTag->Write();
  hLepSF->Write();
  hJEC->Write();
  hTF_final->Write();
  hPred->Write();

  // c1.printContents(hCS);
  // c1.printContents(hTF);
  // c1.printContents(hdRLepPho);
  // c1.printContents(hPdf);
  //c1.printContents(hBTag);
  //  c1.printContents(hLepSF);
  // c1.printContents(hJEC);
  //  c1.printContents(hTF_final);
  //  c1.printContents(hPred);
}

void c_getBGHists::getLMuTauHist(int i_f){
  bool isLDP = (fName[i_f].Contains("LDP"));
  fl = TFile::Open(fName[i_f]);
  TFile *flUnc;
  if(!isLDP) flUnc=TFile::Open("DCS_Run2016_LostMuHadTau_TFUncSq_v2.root");
  else flUnc=TFile::Open("DCS_LDP_Run2016_LostMuHadTau_TFUncSq_v2.root");
  TH1D *hCS,*hTemp,*hTF,*hPred;
  //----- DCS hist
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Mu1");
  if(!isLDP) hCS = (TH1D*)hTemp->Clone("AllSBins_v7_LMuCS");
  else hCS = (TH1D*)hTemp->Clone("AllSBins_v7_LMuCS_LDP");
  //hCS->SetBinErrorOption(TH1::kPoisson);
  //-----get TF in SBins
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Mu0");
  if(!isLDP)  hTF = (TH1D*)hTemp->Clone("LMuTauTFs");
  else   hTF = (TH1D*)hTemp->Clone("LMuTauTFs_LDP");
  hTF->Divide(hCS);
  //-----get TF unc in SBins
  hTemp=(TH1D*)flUnc->Get("AllSBins_v7_Mu0");
  TH1D *hTFUnc=(TH1D*)hTemp->Clone("hTFUnc");
  hTFUnc->Divide((TH1D*)flUnc->Get("AllSBins_v7_Mu1"));
  for(int i=1;i<=hTF->GetNbinsX();i++){
    hTF->SetBinError(i,sqrt(hTFUnc->GetBinContent(i)));
  }
  if(!isLDP){
    hTF->SetBinContent(13,hTF->GetBinContent(14));  hTF->SetBinError(13,hTF->GetBinError(14));
    hTF->SetBinContent(16,hTF->GetBinContent(14));  hTF->SetBinError(16,hTF->GetBinError(14));
    hTF->SetBinContent(26,hTF->GetBinContent(25));  hTF->SetBinError(26,hTF->GetBinError(25));
    hTF->SetBinContent(30,hTF->GetBinContent(29));  hTF->SetBinError(30,hTF->GetBinError(29));
    hTF->SetBinContent(31,hTF->GetBinContent(29));  hTF->SetBinError(31,hTF->GetBinError(29));
  }
  else{
    hTF->SetBinContent( 6,0.584661);  hTF->SetBinError( 6,0.0639891);//6,0.671757);  hTF->SetBinError( 6,0.103653);
    hTF->SetBinContent(14,hTF->GetBinContent(13));  hTF->SetBinError(14,hTF->GetBinError(13));
    hTF->SetBinContent(20,hTF->GetBinContent(19));  hTF->SetBinError(20,hTF->GetBinError(19));
    hTF->SetBinContent(29,hTF->GetBinContent(28));  hTF->SetBinError(29,hTF->GetBinError(28));
    hTF->SetBinContent(30,hTF->GetBinContent(28));  hTF->SetBinError(30,hTF->GetBinError(28));
    hTF->SetBinContent(31,hTF->GetBinContent(28));  hTF->SetBinError(31,hTF->GetBinError(28));
  }
  //------assign 1.5% unc for pdf Scale weights
  TH1D *hPdf;
  if(!isLDP) hPdf=(TH1D*)hTF->Clone("AllSBins_v7_UncLMuTF_pdf");
  else hPdf=(TH1D*)hTF->Clone("AllSBins_v7_UncLMuTF_pdf_LDP");
  c1.assignPCuncAllBins(hPdf,1.5);
  // //------assign 2% unc for b-tagging
  // TH1D *hBTag;
  // if(!isLDP) hBTag=(TH1D*)hTF->Clone("AllSBins_v7_UncLMuTF_bTag");
  // else hBTag=(TH1D*)hTF->Clone("AllSBins_v7_UncLMuTF_bTag_LDP");
  // c1.assignPCuncAllBins(hBTag,2.0);
  //------assign 1% unc for lepton SF
  TH1D *hLepSF;
  if(!isLDP) hLepSF=(TH1D*)hTF->Clone("AllSBins_v7_UncLMuTF_LepSF");
  else hLepSF=(TH1D*)hTF->Clone("AllSBins_v7_UncLMuTF_LepSF_LDP");
  c1.assignPCuncAllBins(hLepSF,3.0);
  //-------get final TF with all unc added in quadrature
  TH1D *hTF_final;
  if(!isLDP) hTF_final=(TH1D*)hTF->Clone("LMuTFs_Final");
  else hTF_final=(TH1D*)hTF->Clone("LMuTFs_Final_LDP");
  hTF_final->Add(hPdf);
  //  hTF_final->Add(hBTag);
  hTF_final->Add(hLepSF);
  //-----multiply DCS with final TF histogram with TF unc
  if(!isLDP) hPred=(TH1D*)hCS->Clone("AllSBins_v7_LMuPred");
  else hPred=(TH1D*)hCS->Clone("AllSBins_v7_LMuPred_LDP");
  // hPred->SetBinErrorOption(TH1::kPoisson);
  hPred->Multiply(hTF_final);
  for(int i=1;i<=hPred->GetNbinsX();i++){
    if(hPred->GetBinContent(i) < 0.00001)
      hPred->SetBinError(i,1.8*hTF_final->GetBinContent(i));
  }
  //---------------------
  fout->cd();
  hCS->Write();
  hTF->Write();
  hPdf->Write();
  //  hBTag->Write();
  hLepSF->Write();
  hTF_final->Write();
  hPred->Write();

  // c1.printContents(hCS);
  // c1.printContents(hTF);
  // c1.printContents(hPdf);
  // c1.printContents(hBTag);
  // c1.printContents(hLepSF);
  // c1.printContents(hTF_final);
  // c1.printContents(hPred);
}

void c_getBGHists::getFRHist(int i_f){
  bool isLDP = (fName[i_f].Contains("LDP"));
  fl = TFile::Open(fName[i_f]);
  TFile *flUnc;
  if(!isLDP) flUnc=TFile::Open("DCS_Run2016_SingleEle_FRUncSq_v2.root");
  else flUnc=TFile::Open("DCS_LDP_Run2016_SingleEle_FRUncSq_v2.root");
  TH1D *hCS,*hTemp,*hFR,*hPred;
  //----- DCS hist
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Ele");
  if(!isLDP) hCS = (TH1D*)hTemp->Clone("AllSBins_v7_FRCS");
  else hCS = (TH1D*)hTemp->Clone("AllSBins_v7_FRCS_LDP");
  //hCS->SetBinErrorOption(TH1::kPoisson);
  //-----get FR in SBins
  hTemp = (TH1D*)fl->Get("AllSBins_v7_Pho");
  if(!isLDP)  hFR = (TH1D*)hTemp->Clone("FRs");
  else   hFR = (TH1D*)hTemp->Clone("FRs_LDP");
  hFR->Divide(hCS);
  //-----get FR unc in SBins
  hTemp=(TH1D*)flUnc->Get("AllSBins_v7_Pho");
  TH1D *hFRUnc=(TH1D*)hTemp->Clone("hFRUnc");
  hFRUnc->Divide((TH1D*)flUnc->Get("AllSBins_v7_Ele"));
  for(int i=1;i<=hFR->GetNbinsX();i++){
    hFR->SetBinError(i,sqrt(hFRUnc->GetBinContent(i)));
  }
  if(isLDP)
    hFR->SetBinContent(16,hFR->GetBinContent(11));  hFR->SetBinError(16,hFR->GetBinError(11));
  //------assign CS stats (+) 2% unc for trigger eff difference in SR and CR
  TH1D *hStat;
  if(!isLDP) hStat=(TH1D*)hCS->Clone("AllSBins_v7_FR_StatTrig");
  else hStat=(TH1D*)hCS->Clone("AllSBins_v7_FR_StatTrig_LDP");
  for(int i=1;i<=hStat->GetNbinsX();i++){
    hStat->SetBinError(i, sqrt((hStat->GetBinError(i))*(hStat->GetBinError(i)) + pow(hStat->GetBinContent(i)*0.02,2)));
  }
  //------assign 10% unc for fakerate SF
  TH1D *hFakeSF;
  if(!isLDP) hFakeSF=(TH1D*)hFR->Clone("AllSBins_v7_UncFR_SF");
  else hFakeSF=(TH1D*)hFR->Clone("AllSBins_v7_UncFR_SF_LDP");
  c1.assignPCuncAllBins(hFakeSF,10.0);
  //------assign 6% unc for PU reweighting
  TH1D *hPU;
  if(!isLDP) hPU=(TH1D*)hFR->Clone("AllSBins_v7_UncFR_PU");
  else hPU=(TH1D*)hFR->Clone("AllSBins_v7_UncFR_PU_LDP");
  c1.assignPCuncAllBins(hPU,6.0);
  //------assign 2% unc for ISR re-weighting
  TH1D *hISRWt;
  if(!isLDP) hISRWt=(TH1D*)hFR->Clone("AllSBins_v7_UncFR_ISRWt");
  else hISRWt=(TH1D*)hFR->Clone("AllSBins_v7_UncFR_ISRWt_LDP");
  c1.assignPCuncAllBins(hISRWt,2.0);
  //-------get final FR with all unc added in quadrature
  TH1D *hFR_final;
  if(!isLDP) hFR_final=(TH1D*)hFR->Clone("FRs_Final");
  else hFR_final=(TH1D*)hFR->Clone("FRs_Final_LDP");
  hFR_final->Add(hFakeSF);
  hFR_final->Add(hPU);
  hFR_final->Add(hISRWt);
  //-----multiply DCS with FR histogram with FR unc
  if(!isLDP)  hPred=(TH1D*)hStat->Clone("AllSBins_v7_FRPred");
  else hPred=(TH1D*)hStat->Clone("AllSBins_v7_FRPred_LDP");
  hPred->Multiply(hFR_final);
  for(int i=1;i<=hPred->GetNbinsX();i++){
    if(hPred->GetBinContent(i) < 0.00001)
      hPred->SetBinError(i,1.8*hFR->GetBinContent(i));
  }
  hStat->Multiply(hFR);
  
  fout->cd();
  hCS->Write();
  hFR->Write();
  hFakeSF->Write();
  hPU->Write();
  hStat->Write();
  hISRWt->Write();
  hFR_final->Write();
  hPred->Write();

  // c1.printContents(hCS);
  // c1.printContents(hFR);
  // c1.printContents(hFakeSF);
  // c1.printContents(hPU);
  //  c1.printContents(hStat);
  // c1.printContents(hISRWt);
  // c1.printContents(hFR_final);
  // c1.printContents(hPred);
}

void c_getBGHists::getZGHist(int i_f){
  bool isLDP = (fName[i_f].Contains("LDP"));
  fl = TFile::Open(fName[i_f]);
  TH1D *h1 = (TH1D*)fl->Get("AllSBins_v7");
  TH1D *hCS = (TH1D*)h1->Clone("AllSBins_v7_ZGCS");
  c1.setNegContentsTo0(hCS);

  TH1D *hLLGmc = (TH1D*)hCS->Clone("MCLLG");
  TH1D *hLLGdata = (TH1D*)hCS->Clone("dataLLG");
  TH1D *hLLGpurity = (TH1D*)hCS->Clone("LLGpurity");
  TH1D *hNuNuGmc = (TH1D*)hCS->Clone("NuNuGMC");
  TH1D *hBtagSFUnc = (TH1D*)hCS->Clone("bTagSF_ZG");

  for(int i=1;i<=hLLGmc->GetNbinsX();i++){
    hLLGdata->SetBinContent(i,36.);
    hLLGdata->SetBinError(i,sqrt(hLLGdata->GetBinContent(i)));

    hLLGmc->SetBinContent(i,26.0816);//LLG MC yield
    hLLGmc->SetBinError(i,0.5556);//LLG MC yield Unc

    hNuNuGmc->SetBinContent(i,170.553961);//NuNuG MC yield
    hNuNuGmc->SetBinError(i,0.958296);//NuNuG MC yield Unc

    hLLGpurity->SetBinContent(i,0.97222);//purity of LLG sample
    hLLGpurity->SetBinError(i,0.02778);//unc on purity of LLG sample
    
    if(i<=16){
      hBtagSFUnc->SetBinContent(i,0);
      hBtagSFUnc->SetBinError(i,0.02*hCS->GetBinContent(i));
    }
    else{
      hBtagSFUnc->SetBinContent(i,0);
      hBtagSFUnc->SetBinError(i,0.06*hCS->GetBinContent(i));
    }
  }
  
  TH1D *hTFpurity = (TH1D*)hLLGpurity->Clone("ZGTFpurity");
  hTFpurity->Multiply(hNuNuGmc);
  hTFpurity->Divide(hLLGmc);
  //----------------- assign higher order unc ---------------------
  TH1D *h_highOrd;
  if(!isLDP) h_highOrd=(TH1D*)hCS->Clone("ZG_HighOrd");
  else h_highOrd=(TH1D*)hCS->Clone("ZG_HighOrd_LDP");
  
  for(int i=1;i<=h_highOrd->GetNbinsX();i++){
    double met_binError = 0.;
    if     (i==1 || i==7  || i==12 || i==17 || i==22 || i==27)      met_binError = 0.08;
    else if(i==2 || i==8  || i==13 || i==18 || i==23 || i==28)      met_binError = 0.18;
    else if(i==3 || i==9  || i==14 || i==19 || i==24 || i==29)      met_binError = 0.20;
    else if(i==4 || i==10 || i==15 || i==20 || i==25 || i==30)      met_binError = 0.25;
    else if(i==5 || i==11 || i==16 || i==21 || i==26 || i==31)      met_binError = 0.35;
    else if(i==6)                                                   met_binError = 0.40;
    h_highOrd->SetBinError(i, met_binError*h_highOrd->GetBinContent(i));
    h_highOrd->SetBinContent(i,0.);
  }
  //---------------------------------------------------------------
  if(!isLDP){
    TH1D *hTFfinal = (TH1D*)hCS->Clone("AllSBins_v7_ZGTFfinal");
    hTFfinal->Add(h_highOrd);//mc stat unc+high order unc
    hTFfinal->Multiply(hTFpurity);
    hTFfinal->Add(hBtagSFUnc);
    double intgl=hCS->Integral();

    for(int i=1;i<=hTFfinal->GetNbinsX();i++){
      hTFfinal->SetBinContent(i,hTFfinal->GetBinContent(i)/intgl);
      hTFfinal->SetBinError(i,hTFfinal->GetBinError(i)/intgl);
    }

    TH1D *hPred = (TH1D*)hTFfinal->Clone("AllSBins_v7_ZGPred");
    hPred->Multiply(hLLGdata);
    
    fout->cd(); 
    hLLGdata->Write();
    hTFpurity->Write();
    hCS->Write();
    h_highOrd->Write();
    hTFfinal->Write();
    hPred->Write();

    //    c1.printContents(hCS);
    //c1.printContents(hNuNuGmc);
    //c1.printContents(hLLGmc);
    //c1.printContents(hLLGpurity);
    //c1.printContents(h_highOrd);
    // c1.printContents(hTFpurity);
    //c1.printContents(hLLGdata);
    //c1.printContents(hTFfinal);
    //c1.printContents(hPred);
  }
  else{
    TH1D *hSF=(TH1D*)hLLGdata->Clone("ZGSF_LDP");
    for(int i=1;i<=hSF->GetNbinsX();i++){
      hSF->SetBinContent(i,30.);
      hSF->SetBinError(i,sqrt(hSF->GetBinContent(i)));
      hLLGmc->SetBinContent(i,26.0816);//LLG MC yield
      hLLGmc->SetBinError(i,0.5556);//LLG MC yield Unc
      hLLGpurity->SetBinContent(i,0.966667);//purity of LLG sample
      hLLGpurity->SetBinError(i,(1.-0.966667));//unc on purity of LLG sample
    }
    hSF->Divide(hLLGmc);
    hSF->Multiply(hLLGpurity);
    
    TH1D *hTemp = (TH1D*)fl->Get("AllSBins_v7");
    TH1D *hCS_MC = (TH1D*)hTemp->Clone("AllSBins_v7_ZGCS_LDP_StatUncOnly");
    TH1D *hCS = (TH1D*)hTemp->Clone("AllSBins_v7_ZGCS_LDP");
    hCS->Add(h_highOrd);
    hCS->Add(hBtagSFUnc);

    TH1D *hPred = (TH1D*)hCS->Clone("AllSBins_v7_ZGPred_LDP");
    hPred->Multiply(hSF);
   
    fout->cd();
    hCS_MC->Write();
    hCS->Write();
    hSF->Write();
    hPred->Write();
   
    //c1.printContents(hCS_MC);
    //c1.printContents(hCS);
    //c1.printContents(hSF);
    //c1.printContents(h_highOrd);
    //c1.printContents(hPred);
  }

}

void c_getBGHists::getMultiJHist(int i_f){
  fl = TFile::Open(fName[i_f]);
  TH1D *hTemp = (TH1D*)fl->Get("AllSBins_v7_AB");
  TH1D *hCSraw = (TH1D*)hTemp->Clone("AllSBins_v7_rawMultiJCS");

  hTemp = (TH1D*)fout->Get("AllSBins_v7_LElePred_LDP");
  TH1D *hEWSumLDP = (TH1D*)hTemp->Clone("AllSBins_v7_EWSum_LDP");
  hEWSumLDP->Add((TH1D*)fout->Get("AllSBins_v7_LMuPred_LDP"));
  hEWSumLDP->Add((TH1D*)fout->Get("AllSBins_v7_FRPred_LDP"));
  hEWSumLDP->Add((TH1D*)fout->Get("AllSBins_v7_ZGPred_LDP"));

  TH1D *hCS = (TH1D*)hCSraw->Clone("AllSBins_v7_MultiJCS");
  hCS->Add(hEWSumLDP,-1);
 
  for(int i=1;i<=hCS->GetNbinsX();i++){
    if(hCS->GetBinContent(i) < 0.00000001){
      hCS->SetBinContent(i,0);
      hCS->SetBinError(i,1.8);
    }
  }
  fout->cd(); 
  hCSraw->Write();
  hEWSumLDP->Write();
  hCS->Write();
  TH1D *h_pure = (TH1D*)hCSraw->Clone("MultiJLDPpurity");
  h_pure->Add(hEWSumLDP,-1);
  h_pure->Divide(hCSraw);
  for(int i=1;i<=h_pure->GetNbinsX();i++){
    if(h_pure->GetBinContent(i) < 0.00001) h_pure->SetBinContent(i,-1.);
    if(hCSraw->GetBinContent(i) < 0.00001){
      h_pure->SetBinContent(i,-1.);
      h_pure->SetBinError(i,0.0);
    }
    else
      h_pure->SetBinError(i,hEWSumLDP->GetBinError(i)/(hCSraw->GetBinContent(i)));
  }
  // setNegContentsTo0(h_pure);

  fout->cd();
  h_pure->Write();
  //  c1.printContents(h_pure);
  // c1.printContents(h_pureUncUp);
  // c1.printContents(h_pureUncDown);
  c1.printContents(hCSraw);
  c1.printContents(hEWSumLDP);
  c1.printContents(hCS);
  //----------------do predictions for HDP ----------------------
  hTemp->Reset();
  hTemp = (TH1D*)fout->Get("AllSBins_v7_LElePred");
  TH1D *hEWSum = (TH1D*)hTemp->Clone("AllSBins_v7_EWSum");
  hEWSum->Add((TH1D*)fout->Get("AllSBins_v7_LMuPred"));
  hEWSum->Add((TH1D*)fout->Get("AllSBins_v7_FRPred"));
  hEWSum->Add((TH1D*)fout->Get("AllSBins_v7_ZGPred"));
  
  hTemp = (TH1D*)fl->Get("AllSBins_v7_C");
  TH1D *hHLR = (TH1D*)hTemp->Clone("AllSBins_v7_HLRatio");
  hHLR->Add(hEWSum,-1);
  hHLR->Divide(hCS);

  hTemp->Reset();
  TH1D *h_doubleR = (TH1D*)hTemp->Clone("doubleRatio_MC");
  double dRatio[6]    = {0.291625, 0.466531, 0.398334, 0.241211, 0.344722, 0.474065};
  // double dRatioUnc[6] = {0.042260, 0.108875, 0.108132, 0.0541237,0.0703203,0.339079};
  double dRatioUnc[6] = {0.042260, 0.108875, 0.108132, 0.0541237,0.0703203,0.339079};
  double doubleRUncVR[6] = {0.09, 0.07, 0.07, 0.13, 0.04, 0.04};//% unc coming from VR. Multiply with double R.
  //add unc obtained from VR to double ratio
  for(int i=0;i<6;i++){
    //    cout<<dRatioUnc[i]<<" ";
    dRatioUnc[i] = sqrt((dRatioUnc[i]*dRatioUnc[i]) + (dRatio[i]*dRatio[i]*doubleRUncVR[i]*doubleRUncVR[i]));
    //    cout<<dRatioUnc[i]<<endl;
  }
  for(int i=1;i<=h_doubleR->GetNbinsX();i++){
    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 ){
      h_doubleR->SetBinContent(i,1.0);
      h_doubleR->SetBinError(i,0.);
    }
    else if(i<=6){
      h_doubleR->SetBinContent(i,dRatio[0]);
      h_doubleR->SetBinError(i,dRatioUnc[0]);
      hHLR->SetBinContent(i,hHLR->GetBinContent(1));
      hHLR->SetBinError(i,hHLR->GetBinError(1));
    }
    else if(i<=11){
      h_doubleR->SetBinContent(i,dRatio[1]);
      h_doubleR->SetBinError(i,dRatioUnc[1]);
      hHLR->SetBinContent(i,hHLR->GetBinContent(7));
      hHLR->SetBinError(i,hHLR->GetBinError(7));
    }
    else if(i<=16){
      h_doubleR->SetBinContent(i,dRatio[2]);
      h_doubleR->SetBinError(i,dRatioUnc[2]);
      hHLR->SetBinContent(i,hHLR->GetBinContent(12));
      hHLR->SetBinError(i,hHLR->GetBinError(12));
    }
    else if(i<=21){
      h_doubleR->SetBinContent(i,dRatio[3]);
      h_doubleR->SetBinError(i,dRatioUnc[3]);
      hHLR->SetBinContent(i,hHLR->GetBinContent(17));
      hHLR->SetBinError(i,hHLR->GetBinError(17));
    }
    else if(i<=26){
      h_doubleR->SetBinContent(i,dRatio[4]);
      h_doubleR->SetBinError(i,dRatioUnc[4]);
      hHLR->SetBinContent(i,hHLR->GetBinContent(22));
      hHLR->SetBinError(i,hHLR->GetBinError(22));
    }
    else{
      h_doubleR->SetBinContent(i,dRatio[5]);
      h_doubleR->SetBinError(i,dRatioUnc[5]);
      hHLR->SetBinContent(i,hHLR->GetBinContent(27));
      hHLR->SetBinError(i,hHLR->GetBinError(27));
    }
  }
  
  TH1D *h_MultiJPred = (TH1D*)hCS->Clone("AllSBins_v7_MultiJPred");
  h_MultiJPred->Multiply(h_doubleR);
  h_MultiJPred->Multiply(hHLR);

  fout->cd();

  hHLR->Write();
  h_doubleR->Write();
  h_MultiJPred->Write();
 
  
  c1.printContents(hHLR);
  c1.printContents(h_doubleR);
  c1.printContents(hEWSum);
  c1.printContents(h_MultiJPred);
    
}

void c_getBGHists::getTotalBG(int i_f){
  fl = TFile::Open(fName[i_f]);
  TH1D *hTemp = (TH1D*)fout->Get("AllSBins_v7_LElePred");
  TH1D *hTot = (TH1D*)hTemp->Clone("AllSBins_v7_TotalBG");
  TH1D *hPull = (TH1D*)hTemp->Clone("AllSBins_v7_Pull");
  hTot->Add((TH1D*)fout->Get("AllSBins_v7_LMuPred"));
  hTot->Add((TH1D*)fout->Get("AllSBins_v7_FRPred"));
  hTot->Add((TH1D*)fout->Get("AllSBins_v7_ZGPred"));
  hTot->Add((TH1D*)fout->Get("AllSBins_v7_MultiJPred"));
  
  hTemp = (TH1D*)fl->Get("AllSBins_v7_CD");
  TH1D *hData = (TH1D*)hTemp->Clone("AllSBins_v7_Obs");
  TH1D *hDataVsBG = (TH1D*)hData->Clone("DataOverBG");
  hDataVsBG->Divide(hTot);

  for(int i=1;i<=hPull->GetNbinsX();i++){
    if( i==1 || i==7 || i==12 || i==17 || i==22 || i==27 ) hPull->SetBinContent(i,0.);
    if(hDataVsBG->GetBinError(i) < 0.000001) hPull->SetBinContent(i, -100.);
    else hPull->SetBinContent(i, (1.0-(hDataVsBG->GetBinContent(i)))/(hDataVsBG->GetBinError(i)));
    hPull->SetBinError(i,0.);
  }

  fout->cd();
  hTot->Write();
  hData->Write();
  hDataVsBG->Write();
  hPull->Write();
  
  //c1.printContents(hTot);
  // c1.printContents(hData);
  //c1.printContents(hDataVsBG);
  //c1.printContents(hPull);
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
void c_getBGHists::getZGHist(int i_f){
  bool isLDP = (fName[i_f].Contains("LDP"));
  fl = TFile::Open(fName[i_f]);
  TH1D *h1 = (TH1D*)fl->Get("AllSBins_v7");
  TH1D *hCS = (TH1D*)h1->Clone("AllSBins_v7_ZGCS");
  c1.setNegContentsTo0(hCS);

  TH1D *hLLGmc = (TH1D*)hCS->Clone("MCLLG");
  TH1D *hLLGdata = (TH1D*)hCS->Clone("dataLLG");
  TH1D *hLLGpurity = (TH1D*)hCS->Clone("LLGpurity");
  
  for(int i=1;i<=hLLGmc->GetNbinsX();i++){
    if(i<=16){
      hLLGdata->SetBinContent(i,31.);
      hLLGdata->SetBinError(i,sqrt(hLLGdata->GetBinContent(i)));

      hLLGmc->SetBinContent(i,32.409096);//LLG MC yield
      hLLGmc->SetBinError(i,1.48495);//LLG MC yield Unc

      hLLGpurity->SetBinContent(i,0.98);//purity of LLG sample
      hLLGpurity->SetBinError(i,0.02);//unc on purity of LLG sample
    }
    else{
      hLLGdata->SetBinContent(i,5.);
      hLLGdata->SetBinError(i,sqrt(hLLGdata->GetBinContent(i)));

      hLLGmc->SetBinContent(i,5.332);//LLG MC yield
      hLLGmc->SetBinError(i,0.811977);//LLG MC yield Unc

      hLLGpurity->SetBinContent(i,0.89);//purity of LLG sample
      hLLGpurity->SetBinError(i,0.11);//unc on purity of LLG sample
    }
  }
  hCS->SetBinContent(30,0.0354253);
  hCS->SetBinContent(31,0.0228197);
  hCS->SetBinError(30,0.7*0.0354253);
  hCS->SetBinError(31,0.7*0.0228197);
  //----------------- assign higher order unc ---------------------
  TH2D *h2_SBinV7VsnJ=(TH2D*)fl->Get("SBinsv7VsnJ");
  TH1D *h_highOrd;
  if(!isLDP) h_highOrd=(TH1D*)hCS->Clone("ZG_HighOrd");
  else h_highOrd=(TH1D*)hCS->Clone("ZG_HighOrd_LDP");
  for(int i=1;i<=h_highOrd->GetNbinsX();i++){
    h_highOrd->SetBinError(i,0);
    if((i>=7 && i<=16) || (i>=22 && i<=31)){
      h_highOrd->SetBinError(i,0.02*h_highOrd->GetBinContent(i));
      h_highOrd->SetBinContent(i,0.);
    }//2% for nJ >=5
    else{
      double unc=0.;
      unc = sqrt(pow(0.27*(h2_SBinV7VsnJ->GetBinContent(h2_SBinV7VsnJ->GetXaxis()->FindBin(i), h2_SBinV7VsnJ->GetYaxis()->FindBin(2.1))),2)
		 + pow(0.13*(h2_SBinV7VsnJ->GetBinContent(h2_SBinV7VsnJ->GetXaxis()->FindBin(i), h2_SBinV7VsnJ->GetYaxis()->FindBin(3.1))),2)
		 + pow(0.10*(h2_SBinV7VsnJ->GetBinContent(h2_SBinV7VsnJ->GetXaxis()->FindBin(i), h2_SBinV7VsnJ->GetYaxis()->FindBin(4.1))),2));
      h_highOrd->SetBinError(i,unc);
      h_highOrd->SetBinContent(i,0.);
    }
  }
  
  //---------------------------------------------------------------
  TH1D *hTF = (TH1D*)hCS->Clone("AllSBins_v7_ZGTF");
  hTF->Divide(hLLGmc);
  hTF->Multiply(hLLGpurity);
    
  TH1D *hPred = (TH1D*)hTF->Clone("AllSBins_v7_ZGPred");
  hPred->Multiply(hLLGdata);
  hPred->Add(h_highOrd);//add higher order correction unc

  TH1D *hPureOverLLGmc = (TH1D*)hLLGpurity->Clone("PureOverLLG_MC");
  hPureOverLLGmc->Divide(hLLGmc);

  if(!isLDP){
    fout->cd(); 
    hCS->Write();
    hPureOverLLGmc->Write();
    hLLGdata->Write();
    hTF->Write();

    hPred->Write();
    hLLGpurity->Write();
    hLLGmc->Write();
    h_highOrd->Write();

    //    c1.printContents(hCS);
    // c1.printContents(hLLGpurity);
    // c1.printContents(hLLGmc);
    // c1.printContents(hLLGdata);
    // c1.printContents(hPureOverLLGmc);
    // c1.printContents(h_highOrd);
    //  c1.printContents(hPred);
    // c1.printContents(hTF);
  }
  else{
    TH1D *hSF=(TH1D*)hLLGdata->Clone("ZGSF_LDP");
    hSF->Divide(hLLGmc);
    hSF->Multiply(hLLGpurity);

    TH1D *hTemp = (TH1D*)fl->Get("AllSBins_v7");
    TH1D *hCS = (TH1D*)hTemp->Clone("AllSBins_v7_ZGCS_LDP");
    hCS->SetBinContent(26,0.026939); hCS->SetBinError(26,hCS->GetBinContent(26));

    hCS->SetBinContent(27,0.034524); hCS->SetBinError(27,hCS->GetBinContent(27));
    hCS->SetBinContent(29,0.000953); hCS->SetBinError(29,hCS->GetBinContent(29));
    hCS->SetBinContent(31,0.000986); hCS->SetBinError(31,hCS->GetBinContent(31));
   
    TH1D *hPred = (TH1D*)hCS->Clone("AllSBins_v7_ZGPred_LDP");
    hPred->Multiply(hSF);
    hPred->Add(h_highOrd);

    fout->cd();
    hCS->Write();
    hSF->Write();
    h_highOrd->Write();
    hPred->Write();
    // c1.printContents(hCS);
    // c1.printContents(hSF);
    // c1.printContents(h_highOrd);
    //    c1.printContents(hPred);
  }

}*/
