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
double sr_Integral=0,cr_Integral=0;
TH2D rebin2DHist(TH2D*,int,double*,int,double*);

void GetLostMuProb(TString iFname){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  f[0] = new TFile(iFname);
  //  gStyle->SetOptStat("nemri");
  sprintf(name,"TF_ZG_%s",f[0]->GetName());
  TFile *fout=new TFile(name,"RECREATE");

  vector<string> name1,name2,name1_2d,name2_2d,name1_3d,name2_3d,newName;
  vector<int> rebin;
  name1.push_back("ST_Mu0");          name2.push_back("ST_Mu1");       rebin.push_back(25);
  name1.push_back("MET_Mu0");         name2.push_back("MET_Mu1");      rebin.push_back(5);
  name1.push_back("METvarBin_Mu0");   name2.push_back("METvarBin_Mu1");rebin.push_back(1);
  name1.push_back("nHadJets_Mu0");    name2.push_back("nHadJets_Mu1");  rebin.push_back(1);
  name1.push_back("BestPhotonPt_Mu0");name2.push_back("BestPhotonPt_Mu1");  rebin.push_back(10);
  name1.push_back("nBTags_Mu0");       name2.push_back("nBTags_Mu1");     rebin.push_back(1);
  name1.push_back("GenMuons_MT2Activity_Mu0");  name2.push_back("GenMuons_MT2Activity_Mu1"); rebin.push_back(5);
  name1.push_back("GenMuPt_Mu0");  name2.push_back("GenMuPt_Mu1"); rebin.push_back(1);
  name1.push_back("nIsoMuonTracks_Mu0");  name2.push_back("nIsoMuonTracks_Mu1");rebin.push_back(1);
  name1.push_back("MuMultInJets_Mu0");  name2.push_back("MuMultInJets_Mu1");rebin.push_back(1);
  name1.push_back("dPhi_METjet1_Pho_Mu0");   name2.push_back("dPhi_METjet1_Pho_Mu1");  rebin.push_back(5);
  name1.push_back("dPhi_METjet2_Pho_Mu0");   name2.push_back("dPhi_METjet2_Pho_Mu1");  rebin.push_back(5);
  name1.push_back("dPhi_METBestPhoton_Mu0");   name2.push_back("dPhi_METBestPhoton_Mu1");  rebin.push_back(5);
  name1.push_back("jet1Pt_Mu0");   name2.push_back("jet1Pt_Mu1");  rebin.push_back(10);
  name1.push_back("jet2Pt_Mu0");   name2.push_back("jet2Pt_Mu1");  rebin.push_back(10);

  name1.push_back("MET_Mu0_R1");         name2.push_back("MET_Mu1_R1");rebin.push_back(1);
  name1.push_back("MET_Mu0_R2");         name2.push_back("MET_Mu1_R2");rebin.push_back(1);
  name1.push_back("MET_Mu0_R3");         name2.push_back("MET_Mu1_R3");rebin.push_back(1);

  //  name1_2d.push_back("METnHadJ_Mu0");   name2_2d.push_back("METnHadJ_Mu1");newName.push_back("1");
  //  name1_2d.push_back("STMET_Mu0");   name2_2d.push_back("STMET_Mu1");newName.push_back("A");
  //  name1_2d.push_back("STMET_2to3HadJ_Mu0");   name2_2d.push_back("STMET_2to3HadJ_Mu1");newName.push_back("1");
  for(int i=1;i<=4;i++){
    name1_2d.push_back("STMET_Mu0_R"+to_string(i));   name2_2d.push_back("STMET_Mu1_R"+to_string(i));newName.push_back(to_string(i));
  }
  // name1_2d.push_back("STMET_min4HadJ_Mu0");   name2_2d.push_back("STMET_min4HadJ_Mu1");newName.push_back("7");
  //  name1_2d.push_back("STHadJ_Mu0");   name2_2d.push_back("STHadJ_Mu1");newName.push_back("2");
  //  name1_2d.push_back("METJet1Pt_Mu0");   name2_2d.push_back("METJet1Pt_Mu1");newName.push_back("2");
  //name1_3d.push_back("STMETnHadJ_Mu0");   name2_3d.push_back("STMETnHadJ_Mu1");

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
	if(name1[i]=="nHadJets_Mu0"){
	  sr_Integral=h_histG->Integral();
	  cr_Integral=h_histE->Integral();
	}
	h_histG->Rebin(rebin[i]);
	h_histE->Rebin(rebin[i]);
	h_histG->SetBinContent(h_histG->GetNbinsX(),h_histG->GetBinContent(h_histG->GetNbinsX())+h_histG->GetBinContent(h_histG->GetNbinsX()+1));
	//	sprintf(name,"LostProb_%s",newName[i].c_str());cout<<name<<endl;
	h_histGcopy=(TH1D*)h_histG->Clone("LostProb");
	h_histE->SetBinContent(h_histE->GetNbinsX(),h_histE->GetBinContent(h_histE->GetNbinsX())+h_histE->GetBinContent(h_histE->GetNbinsX()+1));
	h_histGcopy->Divide(h_histE);
	h_histGcopy->SetLineWidth(2);

	if(name1[i]=="nHadJets_Mu0"){fout->cd(); h_histGcopy->Write();}
	/*	for(int k=1;k<=h_histGcopy->GetNbinsX();k++){
	  if(name1[i]=="ST_Mu0")  cout<<h_histGcopy->GetBinLowEdge(k)<<",";
	  }*/
	//	if(name1[i]=="nHadJets_Pho") cout<<h_histGcopy->GetNbinsX();
	//	if(name1[i]=="_Pho") {fout->cd();h_histGcopy->Write();}
	h_histGcopy->Draw();
	//	if(name1[i]=="ST_Pho" || name1[i]=="BestPhotonPt") h_histGcopy->Write();
      }
    }
  }
  
  TH2D *h2_histG,*h2_histE,*h2_histGcopy; 
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
	sprintf(name,"LostProb_%s",newName[i].c_str());
	//	h2_histGcopy=(TH2D*)h2_histG->Clone("LostProb");
	h2_histGcopy=(TH2D*)h2_histG->Clone(name);
	// h2_histGcopy->RebinX(2);
	// h2_histGcopy->RebinY(5);
	// h2_histE->RebinX(2);
	// h2_histE->RebinY(5);
	h2_histGcopy->Divide(h2_histE);
	//h2_hsum->Add(h2_histGcopy,h2_histE);
	//	h2_histGcopy->Divide(h2_hsum);
	h2_histGcopy->Draw("colz texte");
	//	h2_histGcopy->SetMaximum(0.02);
	gPad->Update();
	if(h2_histGcopy) h2_histGcopy->Write();
      }
    }
  }
  gStyle->SetTextSize(2);
  fout->cd();
  //if(h2_histGcopy) h2_histGcopy->Write();

  TH3D *h3_histG,*h3_histE,*h3_histGcopy; 
  for(int i=0;i<name1_3d.size();i++){
    sprintf(name,"%s_%s",name1_3d[i].c_str(),name2_3d[i].c_str());
    c_dphi[i+name1.size()]=new TCanvas(name,name,1500,800);//c_dphi[i]->Divide(4,2);
    for(int j=0;j<nfiles;j++){
      c_dphi[i+name1.size()]->cd();
      sprintf(name,"%s",name1_3d[i].c_str());
      h3_histG=(TH3D*)f[j]->FindObjectAny(name);
      sprintf(name,"%s",name2_3d[i].c_str());
      h3_histE=(TH3D*)f[j]->FindObjectAny(name);
      if(h3_histG && h3_histE){
	h3_histGcopy=(TH3D*)h3_histG->Clone("LostProb");
	h3_histGcopy->Divide(h3_histE);
	h3_histGcopy->Draw("colz texte");
	//	h3_histGcopy->SetMaximum(0.02);
	gPad->Update();
      }
    }
  }
  gStyle->SetTextSize(2);
  fout->cd();
  //  if(h3_histGcopy) h3_histGcopy->Write();

  cout<<"Integral in SR(0 Mu events) "<<sr_Integral<<endl
      <<"Integral in CR(1 Mu events) "<<cr_Integral<<endl;

}
