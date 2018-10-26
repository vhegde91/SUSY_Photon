#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include"TFile.h"
#include"TEnv.h"

using namespace std;
void findFailedJobs(string);
void findFailedJobs(string arg){
  char jdlStart[200];
  sprintf(jdlStart,"%s",arg.c_str());
  //  sprintf(jdlStart,"signalRegionSkim_%s_",dataset);
  //  char ofileStart[300]="/eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/T5qqqqHg_1800_127_UnSkimmed_";
  char ofileStart[300]="/eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/TChiWg_0_800_UnSkimmed_";
  //  char ofileStart[300]="/eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/GGM_M1M3_1000_50_UnSkimmed_";
  //char ofileStart[300]="T5ttttZg_1800_150_UnSkimmed_1800_150_";
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
    cout<<endl<<"hadd "<<ofileStart<<jdlStart<<".root "<<ofileStart<<jdlStart<<"_job*.root"<<endl;
    cout<<"rm "<<ofileStart<<jdlStart<<"_job*.root"<<endl;
    int choice1=10;
    cout<<endl<<"Enter what you want to do:"<<endl
     	<<"-1: hadd and rm added files"<<endl
       	<<"1: hadd only"<<endl
     	<<"any other no. to exit"<<endl;
    cin>>choice1;
    if(choice1==-1){
      char cmd2[1000];
      sprintf(cmd2,"hadd %s%s.root %s%s_job*.root",ofileStart,jdlStart,ofileStart,jdlStart);
      system(cmd2);
      sprintf(name2,"%s%s.root",ofileStart,jdlStart);
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
      sprintf(cmd2,"hadd %s%s.root %s%s_job*.root",ofileStart,jdlStart,ofileStart,jdlStart);
      system(cmd2);
    }

  }
}

/*
  //sprintf(ofileStart,"/eos/uscms/store/user/vhegde/GMSBstudies2/signalRegionSkim/72511201ccaf867b19de961d2d9534eb883666a7/signalRegionSkim_Spring15v2.%s_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_",dataset);
  //  sprintf(ofileStart,"/eos/uscms/store/user/vhegde/GMSBstudies2/signalRegionSkim/72511201ccaf867b19de961d2d9534eb883666a7/signalRegionSkim_Spring15v2.%s_13TeV-madgraph_",dataset);//for Z
  //sprintf(ofileStart,"/eos/uscms/store/user/vhegde/GMSBstudies2/signalRegionSkim/72511201ccaf867b19de961d2d9534eb883666a7/signalRegionSkim_Spring15v2.%s_13TeV-madgraphMLM-pythia8_",dataset);//for TTJets_TuneCUETP8M1
  sprintf(ofileStart,"/eos/uscms/store/user/vhegde/GMSBstudies2/signalRegionSkim/72511201ccaf867b19de961d2d9534eb883666a7/signalRegionSkim_Spring15v2.%s_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_",dataset);//for TTGJets


 */
