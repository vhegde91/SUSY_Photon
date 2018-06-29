#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include"TFile.h"
#include"TEnv.h"

using namespace std;
string getSampleName(string);
void findFailedJobs(string);
void findFailedJobs(string arg){
  char jdlStart[200];
  sprintf(jdlStart,"%s",arg.c_str());
  //  sprintf(jdlStart,"signalRegionSkim_%s_",dataset);
  //ZJetsToNuNu_HT_800To1200_Summer16.ZJetsToNuNu_HT-800To1200_13TeV-madgraph_job3.root
  string datasetAna = getSampleName(arg);
  char ofileStart[300];
  sprintf(ofileStart,"%s_",datasetAna.c_str());
  char ofileEnd[200]="_RA2AnalysisTree.root";
  char name1[200],name2[400];
  gEnv->SetValue("TFile.Recover", 0);
  vector<int> failedID;
  int totalJobs=0;
  for(int i=0;;i++){
    sprintf(name1,"%s_job%i.jdl",jdlStart,i);//cout<<name1<<endl;
    ifstream infile(name1);
    if(!infile) {cout<<"Total jdl="<<i<<endl;totalJobs=i;break;}
    //    sprintf(name2,"%s%i%s",ofileStart,i,ofileEnd);
    sprintf(name2,"%s%s_job%i.root",ofileStart,jdlStart,i);//cout<<name2<<endl;
    TFile *f1=new TFile(name2);
    if(f1->IsZombie() || !f1) {
      failedID.push_back(i);
    }
    delete f1;
  }
  cout<<"-----------------------------------------------"<<endl;
  for(int i=0;i<failedID.size();i++){
    cout<<"condor_submit "<<jdlStart<<"_job"<<failedID[i]<<".jdl"<<endl;
  }
  cout<<"-----------------------------------------------"<<endl;
  cout<<"Total failed "<<jdlStart<<" jobs:"<<failedID.size()<<endl;

  if(failedID.size() == 0 && totalJobs!=0) {
    char cmd0[1000];
    sprintf(cmd0,"du -sh %s%s_job0.root",ofileStart,jdlStart);
    cout<<"size of job0 file:";
    system(cmd0);

    cout<<endl<<"hadd Eff_bTag_"<<jdlStart<<".root "<<ofileStart<<jdlStart<<"_job*.root"<<endl;
    cout<<"rm "<<ofileStart<<jdlStart<<"_job*.root"<<endl;
    int choice1=10;
    cout<<endl<<"Enter what you want to do:"<<endl
     	<<"-1: hadd and rm added files"<<endl
       	<<"1: hadd only"<<endl
     	<<"any other no. to exit"<<endl;
    cin>>choice1;
    if(choice1==-1){
      char cmd2[1000];
      sprintf(cmd2,"hadd -f Eff_bTag_%s.root %s%s_job*.root",jdlStart,ofileStart,jdlStart);
      system(cmd2);
      sprintf(name2,"Eff_bTag_%s.root",jdlStart);
      TFile *f1=new TFile(name2);
      if( !(f1->IsZombie() || !f1) ) {
	cout<<endl<<"Removing job files"<<endl;
	sprintf(cmd2,"rm %s%s_job*.root",ofileStart,jdlStart);
	system(cmd2);
      }
      else cout<<"hadd might have failed. Did not remove files"<<endl;
      delete f1;
    }
    else if(choice1==1){
      char cmd2[1000];
      sprintf(cmd2,"hadd -f Eff_bTag_%s.root %s%s_job*.root",jdlStart,ofileStart,jdlStart);
      system(cmd2);
    }

  }
}
string getSampleName(string str1){
  TString tstr1 = str1;
  vector<string> sampleName2 = {"DYJetsToLL_M_50_HT_100to200",
				 "DYJetsToLL_M_50_HT_200to400",
				 "DYJetsToLL_M_50_HT_400to600",
				 "DYJetsToLL_M_50_HT_600to800",
				 "DYJetsToLL_M_50_HT_800to1200",
				 "DYJetsToLL_M_50_HT_1200to2500",
				 "DYJetsToLL_M_50_HT_2500toInf",
				 "GJets_DR_0p4_HT_100To200",
				 "GJets_DR_0p4_HT_200To400",
				 "GJets_DR_0p4_HT_400To600",
				 "GJets_DR_0p4_HT_600ToInf",
				 "GJets_HT_100To200",
				 "GJets_HT_200To400",
				 "GJets_HT_400To600",
				 "GJets_HT_600ToInf",
				 "QCD_HT1000to1500",
				 "QCD_HT1500to2000",
				 "QCD_HT2000toInf",
				 "QCD_HT200to300",
				 "QCD_HT300to500",
				 "QCD_HT500to700",
				 "QCD_HT700to1000",
				 "ST_s_ch_t_ch_tW_4f_5f_13TeV",
				 "TTGJets",
				 "TGJets",
				 "TTJets_DiLept",
				 "TTJets_HT_1200to2500",
				 "TTJets_HT_2500toInf",
				 "TTJets_HT_600to800",
				 "TTJets_HT_800to1200",
				 "TTJets_SingleLeptFromTbar",
				 "TTJets_SingleLeptFromT",
				 "TTJets",
				 "WGJets_MonoPhoton_PtG_130",
				 "WGJets_MonoPhoton_PtG_40to130",
				 "WJetsToLNu_HT_100To200",
				 "WJetsToLNu_HT_1200To2500",
				 "WJetsToLNu_HT_200To400",
				 "WJetsToLNu_HT_2500ToInf",
				 "WJetsToLNu_HT_400To600",
				 "WJetsToLNu_HT_600To800",
				 "WJetsToLNu_HT_800To1200",
				 "ZGTo2LG_PtG_130",
				 "ZGTo2LG",
				 "ZGTo2NuG_PtG_130",
				 "ZGTo2NuG",
				 "ZJetsToNuNu_HT_100To200",
				 "ZJetsToNuNu_HT_200To400",
				 "ZJetsToNuNu_HT_400To600",
				 "ZJetsToNuNu_HT_600To800",
				 "ZJetsToNuNu_HT_800To1200",
				 "ZJetsToNuNu_HT_1200To2500",
				 "ZJetsToNuNu_HT_2500ToInf"};

  vector<TString> sampleName_org = {"DYJetsToLL_M-50_HT-100to200",
				    "DYJetsToLL_M-50_HT-200to400",
				    "DYJetsToLL_M-50_HT-400to600",
				    "DYJetsToLL_M-50_HT-600to800",
				    "DYJetsToLL_M-50_HT-800to1200",
				    "DYJetsToLL_M-50_HT-1200to2500",
				    "DYJetsToLL_M-50_HT-2500toInf",
				    "GJets_DR-0p4_HT-100To200",
				    "GJets_DR-0p4_HT-200To400",
				    "GJets_DR-0p4_HT-400To600",
				    "GJets_DR-0p4_HT-600ToInf",
				    "GJets_HT-100To200",
				    "GJets_HT-200To400",
				    "GJets_HT-400To600",
				    "GJets_HT-600ToInf",
				    "QCD_HT1000to1500",
				    "QCD_HT1500to2000",
				    "QCD_HT2000toInf",
				    "QCD_HT200to300",
				    "QCD_HT300to500",
				    "QCD_HT500to700",
				    "QCD_HT700to1000",
				    "ST_s-ch_t-ch_tW_4f_5f_13TeV",
				    "TTGJets",
				    "TGJets",
				    "TTJets_DiLept",
				    "TTJets_HT-1200to2500",
				    "TTJets_HT-2500toInf",
				    "TTJets_HT-600to800",
				    "TTJets_HT-800to1200",
				    "TTJets_SingleLeptFromTbar",
				    "TTJets_SingleLeptFromT",
				    "TTJets",
				    "WGJets_MonoPhoton_PtG-130",
				    "WGJets_MonoPhoton_PtG-40to130",
				    "WJetsToLNu_HT-100To200",
				    "WJetsToLNu_HT-1200To2500",
				    "WJetsToLNu_HT-200To400",
				    "WJetsToLNu_HT-2500ToInf",
				    "WJetsToLNu_HT-400To600",
				    "WJetsToLNu_HT-600To800",
				    "WJetsToLNu_HT-800To1200",
				    "ZGTo2LG_PtG-130",
				    "ZGTo2LG",
				    "ZGTo2NuG_PtG-130",
				    "ZGTo2NuG",
				    "ZJetsToNuNu_HT-100To200",
				    "ZJetsToNuNu_HT-200To400",
				    "ZJetsToNuNu_HT-400To600",
				    "ZJetsToNuNu_HT-600To800",
				    "ZJetsToNuNu_HT-800To1200",
				    "ZJetsToNuNu_HT-1200To2500",
				    "ZJetsToNuNu_HT-2500ToInf"};
  for(int i=0;i<sampleName_org.size();i++){
    if(tstr1.Contains(sampleName_org[i])) return sampleName2[i];
  }
  return "Unknown_Sample";
}
