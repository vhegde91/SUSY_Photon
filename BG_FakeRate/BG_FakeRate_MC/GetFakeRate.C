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
int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
char name[100];

TH2D rebin2DHist(TH2D*,int,double*,int,double*);

void GetFakeRate(TString iFname){
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);
  bool fakeFrac=true;
  gStyle->SetOptStat(0);
  double nume=1,numg=1;

  //f[0] = new TFile("CS_WJetsToLNu.root");
  //  f[0] = new TFile("CS_DYJetsToLL_FR_v2.root");
  // f[0] = new TFile("CS_TTJets_v2.root");
  f[0] = new TFile(iFname);
  //  f[0] = new TFile("CS_TTW_FR_v2.root");
  
  //f[0] = new TFile("CS_TTW_FR_clsr_EMPtQMult.root");
  //  gStyle->SetOptStat("nemri");
  sprintf(name,"FR_Hist_%s",f[0]->GetName());
  TFile *fout=new TFile(name,"RECREATE");

  vector<string> name1,name2,name1_2d,name2_2d;
  vector<int> rebin;
  name1.push_back("ST_Pho");          name2.push_back("ST_Ele");      rebin.push_back(25);
  // name1.push_back("HT_Pho");          name2.push_back("HT_Ele");      rebin.push_back(20);
  name1.push_back("MET_Pho");         name2.push_back("MET_Ele");     rebin.push_back(10);
  name1.push_back("nHadJets_Pho");    name2.push_back("nHadJets_Ele");rebin.push_back(1);
  name1.push_back("nBTags_Pho");    name2.push_back("nBTags_Ele");rebin.push_back(1);
  name1.push_back("BestPhotonPt");    name2.push_back("ElePt");       rebin.push_back(5);
  name1.push_back("BestPhotonEta");   name2.push_back("EleEta");      rebin.push_back(5);
  name1.push_back("BestPhotonPhi");   name2.push_back("ElePhi");      rebin.push_back(2);
  name1.push_back("METvarBin_Pho");   name2.push_back("METvarBin_Ele");rebin.push_back(1);
  name1.push_back("PhoPtvBin");   name2.push_back("ElePtvBin");rebin.push_back(1);
  name1.push_back("nVtx_Pho");name2.push_back("nVtx_Ele");rebin.push_back(5);
  // name1.push_back("PtJetNearPho");name2.push_back("PtJetNearEle");rebin.push_back(10);
  // name1.push_back("genEleMT2Act_Pho");name2.push_back("genEleMT2Act_Ele");rebin.push_back(10);
  //  name1.push_back("RemJetPtNearPhoton");name2.push_back("RemJetPtNearEle");rebin.push_back(2);
  // name1.push_back("dR_GptJptRatio");name2.push_back("dR_EleptJptRatio");rebin.push_back(5);
  // name1.push_back("JptGptRatio");name2.push_back("JptEleptRatio");rebin.push_back(1);
  name1.push_back("minDR_Pho_Jet");name2.push_back("minDR_Ele_Jet");rebin.push_back(2);
  //name1.push_back("minDR_Pho_HadJet");name2.push_back("minDR_Ele_HadJet");rebin.push_back(4);
  // name1.push_back("dPhi_METjet1_Pho");name2.push_back("dPhi_METjet1_Ele");rebin.push_back(4);
  name1.push_back("Qmulti_Pho");name2.push_back("Qmulti_Ele");rebin.push_back(2);
  name1.push_back("nEleTrks_Pho");name2.push_back("nEleTrks_Ele");rebin.push_back(1);
  name1.push_back("nEMuPiTrks_Pho");name2.push_back("nEMuPiTrks_Ele");rebin.push_back(1);
  //  name1.push_back("");name2.push_back("");rebin.push_back(1);
  name1.push_back("nEleTracks_Pho");name2.push_back("nEleTracks_Ele");rebin.push_back(1);
  name1.push_back("nMuTracks_Pho");name2.push_back("nMuTracks_Ele");rebin.push_back(1);
  name1.push_back("nPiTracks_Pho");name2.push_back("nPiTracks_Ele");rebin.push_back(1);
  name1.push_back("nTracks_Pho");name2.push_back("nTracks_Ele");rebin.push_back(5);
  name1.push_back("nTracks0p3_Pho");name2.push_back("nTracks0p3_Ele");rebin.push_back(1);
  name1.push_back("bin1_Pho");          name2.push_back("bin1_Ele");      rebin.push_back(1);
  // name1.push_back("QEMFrac_Pho");name2.push_back("QEMFrac_Ele");rebin.push_back(2);
  // name1.push_back("QHadFrac_Pho");name2.push_back("QHadFrac_Ele");rebin.push_back(5);
  // name1.push_back("NeuEMFrac_Pho");name2.push_back("NeuEMFrac_Ele");rebin.push_back(2);
  // name1.push_back("NeuHadFrac_Pho");name2.push_back("NeuHadFrac_Ele");rebin.push_back(2);
  // name1.push_back("PhoFrac_Pho");name2.push_back("PhoFrac_Ele");rebin.push_back(2);
  // name1_2d.push_back("PhotonPtEta");       name2_2d.push_back("ElePtEta");
  //  name1_2d.push_back("PhotonPtEtavBin");   name2_2d.push_back("ElePtEtavBin");
  //  name1_2d.push_back("JetPtNearPhoEtavBin");   name2_2d.push_back("JetPtNearEleEtavBin");
  //  name1_2d.push_back("JPtGPtRatioMinDR");   name2_2d.push_back("JPtEPtRatioMinDR");
  //  name1_2d.push_back("PhoPtMinDRHadJ");   name2_2d.push_back("ElePtMinDRHadJ");
  //name1_2d.push_back("minHadDR_QMult_Pho"); name2_2d.push_back("minHadDR_QMult_Ele");
  //  name1_2d.push_back("PhoPtVtx");   name2_2d.push_back("ElePtVtx");
  name1_2d.push_back("PhoPtQMultJet");   name2_2d.push_back("ElePtQMultJet");
  //  name1_2d.push_back("PhoEtaPhi");   name2_2d.push_back("EleEtaPhi");
  //name1_2d.push_back("PhoPtST");   name2_2d.push_back("ElePtST");
  //  name1.push_back("BestPhotonPhi");   name2.push_back("ElePhi");

  TCanvas *c_dphi[name1.size()+name1_2d.size()];
  for(int i=0;i<name1.size();i++){
    sprintf(name,"%s_%s",name1[i].c_str(),name2[i].c_str());
    c_dphi[i]=new TCanvas(name,name,1500,800);//c_dphi[i]->Divide(4,2);
    TH1D *h_histG,*h_histE,*h_histGcopy;
    for(int j=0;j<nfiles;j++){
      c_dphi[i]->cd();//c_dphi[i]->SetLogy();
      sprintf(name,"%s",name1[i].c_str());
      h_histG=(TH1D*)f[j]->FindObjectAny(name);//h_histG->Rebin(2);
      
      sprintf(name,"%s",name2[i].c_str());
      h_histE=(TH1D*)f[j]->FindObjectAny(name);//h_histE->Rebin(2);

      if(h_histG && h_histE){
	h_histG->Rebin(rebin[i]);
	h_histE->Rebin(rebin[i]);
	h_histG->SetBinContent(h_histG->GetNbinsX(),h_histG->GetBinContent(h_histG->GetNbinsX())+h_histG->GetBinContent(h_histG->GetNbinsX()+1));
	sprintf(name,"FakeRate_%s",name1[i].c_str());
	h_histGcopy=(TH1D*)h_histG->Clone(name);
	h_histE->SetBinContent(h_histE->GetNbinsX(),h_histE->GetBinContent(h_histE->GetNbinsX())+h_histE->GetBinContent(h_histE->GetNbinsX()+1));
	h_histGcopy->Divide(h_histE);
	h_histGcopy->SetLineWidth(2);
	if(name1[i]=="nHadJets_Pho"){ numg=h_histG->Integral(); nume=h_histE->Integral();}
	for(int k=1;k<=h_histGcopy->GetNbinsX();k++){
	  if(name1[i]=="dR_GptJptRatio")  cout<<h_histGcopy->GetBinContent(k)<<",";
	}
	for(int k=1;k<=h_histGcopy->GetNbinsX();k++){
	  if(name1[i]=="dR_GptJptRatio")  cout<<h_histGcopy->GetBinLowEdge(k)<<",";
	}
	/*	if(name1[i]=="nHadJets_Pho") cout<<h_histGcopy->GetNbinsX();
	if(name1[i]=="MET_Pho") {fout->cd();h_histGcopy->Write();}*/
	h_histGcopy->Draw();
	fout->cd();
	h_histGcopy->Write();
	//	if(name1[i]=="ST_Pho" || name1[i]=="BestPhotonPt") h_histGcopy->Write();
      }
    }
  }
  
  TH2D *h2_histG,*h2_histE,*h2_histGcopy,*h2_histGcopy2; 
  for(int i=0;i<name1_2d.size();i++){
    sprintf(name,"%s_%s",name1_2d[i].c_str(),name2_2d[i].c_str());
    c_dphi[i+name1.size()]=new TCanvas(name,name,1500,800);//c_dphi[i]->Divide(4,2);
    for(int j=0;j<nfiles;j++){
      c_dphi[i+name1.size()]->cd();
      sprintf(name,"%s",name1_2d[i].c_str());
      h2_histG=(TH2D*)f[j]->FindObjectAny(name);
      sprintf(name,"%s",name2_2d[i].c_str());
      h2_histE=(TH2D*)f[j]->FindObjectAny(name);
      if(h2_histG && h2_histE){
	h2_histGcopy=(TH2D*)h2_histG->Clone("FakeRate");
	h2_histGcopy2=(TH2D*)h2_histG->Clone("FakeRate2");
	// h2_histGcopy->RebinX(2);
	// h2_histGcopy->RebinY(5);
	// h2_histE->RebinX(2);
	// h2_histE->RebinY(5);
	if(fakeFrac)  h2_histGcopy->Divide(h2_histE);
        else{
          h2_histGcopy2->Add(h2_histE);
          h2_histGcopy->Divide(h2_histGcopy2);
        }
	//h2_hsum->Add(h2_histGcopy,h2_histE);
	//	h2_histGcopy->Divide(h2_hsum);
	h2_histGcopy->Draw("colz texte");
	h2_histGcopy->SetMaximum(0.02);
	gPad->Update();
      }
    }
  }
  gStyle->SetTextSize(2);
  fout->cd();
  if(h2_histGcopy) h2_histGcopy->Write();
  for(int i=1;i<=h2_histGcopy->GetXaxis()->GetNbins();i++){
    for(int j=1;j<=h2_histGcopy->GetYaxis()->GetNbins();j++){
      cout<<h2_histGcopy->GetXaxis()->GetBinLowEdge(i)<<"\t"<<h2_histGcopy->GetYaxis()->GetBinLowEdge(j)<<"\t"<<h2_histGcopy->GetBinContent(i,j)<<"\t"<<h2_histGcopy->GetBinError(i,j)<<"\t"<<h2_histGcopy->GetBinError(i,j)*100/h2_histGcopy->GetBinContent(i,j)<<endl;
    }
  }
  cout<<"# of e events: "<<nume<<endl
      <<"# of g events: "<<numg<<endl
      <<"Fake fraction:  "<<numg/nume<<endl;
    //      <<"Fake rate:      "<<numg/(numg+nume)<<endl;
}
