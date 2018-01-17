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
  fName[3] = "CS_ZGZJToNuNuG.root";
  fName[4] = "DCS_LDP_Run2016_Multijet_v2.root";

  fName[5] = "DCS_LDP_Run2016_LostEle_v2.root";
  fName[6] = "DCS_LDP_Run2016_LostMuHadTau_v2.root";
  fName[7] = "DCS_LDP_Run2016_SingleEle_v2.root";
  fName[8] = "CS_LDP_ZGZJToNuNuG.root";

  fName[9] = "DCS_LDP_Run2016_Multijet_v2.root";

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
    // hTF->SetBinContent( 6,0.362095);  hTF->SetBinError( 6,0.121615);//special case
    hTF->SetBinContent( 6,0.362095);  hTF->SetBinError( 6,0.0328);//special case
    hTF->SetBinContent(13,0.337721);  hTF->SetBinError(13,0.0771929);
    hTF->SetBinContent(16,0.337721);  hTF->SetBinError(16,0.0771929);
    hTF->SetBinContent(20,0.560793);  hTF->SetBinError(20,0.0501785);
    hTF->SetBinContent(29,0.346311);  hTF->SetBinError(29,0.0597414);
  }
  else{
    hTF->SetBinContent( 6,0.231834);  hTF->SetBinError( 6,0.039783);
    hTF->SetBinContent(11,0.163806);  hTF->SetBinError(11,0.0287722);
    hTF->SetBinContent(14,0.168521);  hTF->SetBinError(14,0.0496167);
    hTF->SetBinContent(16,0.168521);  hTF->SetBinError(16,0.0496167);
    hTF->SetBinContent(21,0.272343);  hTF->SetBinError(21,0.0429325);
    hTF->SetBinContent(31,0.245341);  hTF->SetBinError(31,0.0636078);
  }
  //-----multiply DCS with TF histogram with TF unc
  if(!isLDP) hPred=(TH1D*)hCS->Clone("AllSBins_v7_LElePred");
  else hPred=(TH1D*)hCS->Clone("AllSBins_v7_LElePred_LDP");
  hPred->SetBinErrorOption(TH1::kPoisson);
  hPred->Multiply(hTF);
  for(int i=1;i<=hPred->GetNbinsX();i++){
    if(hPred->GetBinContent(i) < 0.00001)
      hPred->SetBinError(i,1.8*hTF->GetBinContent(i));
  }
  //---------------------
  fout->cd();
  hCS->Write();
  hTF->Write();
  hPred->Write();
  //  c1.printContents(hCS);
  //  c1.printContents(hTF);
  // c1.printContents(hPred);
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
    hTF->SetBinContent(13,0.703993);  hTF->SetBinError(13,0.098179);
    hTF->SetBinContent(16,0.750494);  hTF->SetBinError(16,0.0984693);
    hTF->SetBinContent(26,0.797014);  hTF->SetBinError(26,0.0704897);
    hTF->SetBinContent(30,0.706147);  hTF->SetBinError(30,0.0969159);
    hTF->SetBinContent(31,0.706147);  hTF->SetBinError(31,0.0969159);
  }
  else{
    hTF->SetBinContent( 6,0.671757);  hTF->SetBinError( 6,0.103653);
    hTF->SetBinContent(14,0.603732);  hTF->SetBinError(14,0.197632);
    hTF->SetBinContent(20,0.690634);  hTF->SetBinError(20,0.0959729);
    hTF->SetBinContent(29,0.536487);  hTF->SetBinError(29,0.103087);
    hTF->SetBinContent(30,0.536487);  hTF->SetBinError(30,0.103087);
    hTF->SetBinContent(31,0.536487);  hTF->SetBinError(31,0.103087);
  }

  //-----multiply DCS with TF histogram with TF unc
  if(!isLDP)  hPred=(TH1D*)hCS->Clone("AllSBins_v7_LMuPred");
  else hPred=(TH1D*)hCS->Clone("AllSBins_v7_LMuPred_LDP");
  hPred->Multiply(hTF);
  for(int i=1;i<=hPred->GetNbinsX();i++){
    if(hPred->GetBinContent(i) < 0.00001)
      hPred->SetBinError(i,1.8*hTF->GetBinContent(i));
  }
  //---------------------
  fout->cd();
  hCS->Write();
  hTF->Write();
  hPred->Write();
  //  if(isLDP) hLMuPredLDP = (TH1D*)hPred->Clone("hLMuPredLDP");
  // c1.printContents(hCS);
   // c1.printContents(hTF);
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
  //-----multiply DCS with FR histogram with FR unc
  if(!isLDP)  hPred=(TH1D*)hCS->Clone("AllSBins_v7_FRPred");
  else hPred=(TH1D*)hCS->Clone("AllSBins_v7_FRPred_LDP");
  hPred->Multiply(hFR);
  for(int i=1;i<=hPred->GetNbinsX();i++){
    if(hPred->GetBinContent(i) < 0.00001)
      hPred->SetBinError(i,1.8*hFR->GetBinContent(i));
  }
  
  fout->cd();
  hCS->Write();
  hFR->Write();
  hPred->Write();
  //  if(isLDP) hFRPredLDP = (TH1D*)hPred->Clone("hFRPredLDP");
  // c1.printContents(hCS);
  // c1.printContents(hFR);
  // c1.printContents(hPred);
}

void c_getBGHists::getZGHist(int i_f){
  bool isLDP = (fName[i_f].Contains("LDP"));
  fl = TFile::Open(fName[i_f]);
  TH1D *h1 = (TH1D*)fl->Get("AllSBins_v7");
  TH1D *h1cp = (TH1D*)h1->Clone("AllSBins_v7_ZGCS");
  c1.setNegContentsTo0(h1cp);

  TH1D *hLLGmc = (TH1D*)h1cp->Clone("MCLLG");
  TH1D *hLLGdata = (TH1D*)h1cp->Clone("dataLLG");
  TH1D *hLLGpurity = (TH1D*)h1cp->Clone("LLGpurity");
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
  h1cp->SetBinContent(30,0.040508);
  h1cp->SetBinContent(31,0.0260939);
  h1cp->SetBinError(30,0.7*0.040508);
  h1cp->SetBinError(31,0.7*0.026094);

  h1cp->Divide(hLLGmc);
  h1cp->Multiply(hLLGpurity);

  TH1D *hPred = (TH1D*)h1cp->Clone("AllSBins_v7_ZGPred");
  hPred->Multiply(hLLGdata);
  if(!isLDP){
    fout->cd(); 
    h1cp->Write();
    hLLGdata->Write();
    hLLGmc->Write();
    hLLGpurity->Write();
    hPred->Write();
    // c1.printContents(h1cp);
    // c1.printContents(hLLGdata);
    // c1.printContents(hPred);
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

    fout->cd();
    hCS->Write();
    hSF->Write();
    hPred->Write();
    // c1.printContents(hCS);
    // c1.printContents(hSF);
    // c1.printContents(hPred);
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
  double dRatioUnc[6] = {0.042260, 0.108875, 0.108132, 0.0541237,0.0703203,0.339079};

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
  c1.printContents(h_MultiJPred);
    
}

void c_getBGHists::getTotalBG(int i_f){
  fl = TFile::Open(fName[i_f]);
  TH1D *hTemp = (TH1D*)fout->Get("AllSBins_v7_LElePred");
  TH1D *hTot = (TH1D*)hTemp->Clone("AllSBins_v7_TotalBG");
  hTot->Add((TH1D*)fout->Get("AllSBins_v7_LMuPred"));
  hTot->Add((TH1D*)fout->Get("AllSBins_v7_FRPred"));
  hTot->Add((TH1D*)fout->Get("AllSBins_v7_ZGPred"));
  hTot->Add((TH1D*)fout->Get("AllSBins_v7_MultiJPred"));
  
  hTemp = (TH1D*)fl->Get("AllSBins_v7_CD");
  TH1D *hData = (TH1D*)hTemp->Clone("AllSBins_v7_Obs");
  TH1D *hDataVsBG = (TH1D*)hData->Clone("DataOverBG");
  hDataVsBG->Divide(hTot);

  fout->cd();
  hTot->Write();
  hData->Write();
  hDataVsBG->Write();

  c1.printContents(hTot);
  c1.printContents(hData);
  c1.printContents(hDataVsBG);
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
