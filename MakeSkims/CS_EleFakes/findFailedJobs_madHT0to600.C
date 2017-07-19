#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include"TFile.h"
#include"TEnv.h"

using namespace std;
void findFailedJobs_madHT0to600(string);
void findFailedJobs_madHT0to600(){
  //  string jdlStart="Spring16.GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.QCD_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RA2AnalysisTree";
  //  string jdlStart="Spring16.QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //string jdlStart="Spring16.QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //string jdlStart="Spring16.GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  //  string jdlStart="Spring16.ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph";
  //  string jdlStart="Spring16.ZJetsToNuNu_HT-1200To2500_13TeV-madgraph";
  //  string jdlStart="Spring16.ZJetsToNuNu_HT-800To1200_13TeV-madgraph";
  //  string jdlStart="Spring16.ZJetsToNuNu_HT-600To800_13TeV-madgraph";
  //  string jdlStart="Spring16.ZJetsToNuNu_HT-400To600_13TeV-madgraph";
  //  string jdlStart="Spring16.ZJetsToNuNu_HT-200To400_13TeV-madgraph";
  //  string jdlStart="Spring16.ZJetsToNuNu_HT-100To200_13TeV-madgraph";
  string jdlStart="Spring16.WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  // string jdlStart="Spring16.WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  // string jdlStart="Spring16.WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  // string jdlStart="Spring16.WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  // string jdlStart="Spring16.WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  // string jdlStart="Spring16.WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  // string jdlStart="Spring16.WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8";
  findFailedJobs_madHT0to600(jdlStart);
}
void findFailedJobs_madHT0to600(string arg){
  char jdlStart[200];
  sprintf(jdlStart,"%s",arg.c_str());
  //  sprintf(jdlStart,"signalRegionSkim_%s_",dataset);
  char ofileStart[300]="/eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/CS_EleFakes_v3/CS_FR_madHT0to600_";
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
    
    cout<<endl<<"hadd "<<ofileStart<<jdlStart<<".root "<<ofileStart<<jdlStart<<"_job*.root"<<endl;
    cout<<"rm "<<ofileStart<<jdlStart<<"_job*.root"<<endl;
    int choice1=10;
    cout<<endl<<"Enter what you want to do:"<<endl
       	<<"1: hadd only"<<endl
     	<<"any other no. to exit"<<endl;
    cin>>choice1;
    if(choice1==1){
      char cmd2[1000];
      sprintf(cmd2,"hadd -f %s%s.root %s%s_job*.root",ofileStart,jdlStart,ofileStart,jdlStart);
      system(cmd2);
    }
    if(choice1==1){
      cout<<endl<<"hadd successfully/unsuccessful. Enter what you want to do:"<<endl
	  <<"-1: remove individual job files"<<endl
	  <<"any other no. to exit"<<endl;
      cin>>choice1;
      if(choice1==-1){
	char cmd2[1000];
	cout<<endl<<"Removing job files"<<endl;
	sprintf(cmd2,"rm %s%s_job*.root",ofileStart,jdlStart);
	system(cmd2);
      }
    }
  }
}

/*
  //sprintf(ofileStart,"/eos/uscms/store/user/vhegde/GMSBstudies2/signalRegionSkim/72511201ccaf867b19de961d2d9534eb883666a7/signalRegionSkim_Spring15v2.%s_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_",dataset);
  //  sprintf(ofileStart,"/eos/uscms/store/user/vhegde/GMSBstudies2/signalRegionSkim/72511201ccaf867b19de961d2d9534eb883666a7/signalRegionSkim_Spring15v2.%s_13TeV-madgraph_",dataset);//for Z
  //sprintf(ofileStart,"/eos/uscms/store/user/vhegde/GMSBstudies2/signalRegionSkim/72511201ccaf867b19de961d2d9534eb883666a7/signalRegionSkim_Spring15v2.%s_13TeV-madgraphMLM-pythia8_",dataset);//for TTJets_TuneCUETP8M1
  sprintf(ofileStart,"/eos/uscms/store/user/vhegde/GMSBstudies2/signalRegionSkim/72511201ccaf867b19de961d2d9534eb883666a7/signalRegionSkim_Spring15v2.%s_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_",dataset);//for TTGJets


 */
