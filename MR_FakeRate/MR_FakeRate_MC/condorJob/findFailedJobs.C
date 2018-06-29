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
void findFailedJobs(){
  string jdlStart="runList_MS_FR_DYJetsToLL";
  findFailedJobs(jdlStart);
}
void findFailedJobs(string arg){
  char jdlStart[200];
  sprintf(jdlStart,"%s",arg.c_str());
  //  sprintf(jdlStart,"signalRegionSkim_%s_",dataset);
  char ofileStart[300]=" ";
  char name1[200],name2[400];
  gEnv->SetValue("TFile.Recover", 0);
  vector<int> failedID;
  int totalJobs=0;
  for(int i=0;;i++){
    sprintf(name1,"%s_job%i.jdl",jdlStart,i);//cout<<name1<<endl;
    ifstream infile(name1);
    if(!infile) {cout<<"Total jdl="<<i<<endl;totalJobs=i;break;}
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
    cout<<endl<<"hadd -f "<<ofileStart<<jdlStart<<".root "<<ofileStart<<jdlStart<<"_job*.root"<<endl;
    cout<<"rm "<<ofileStart<<jdlStart<<"_job*.root"<<endl;
    int choice1=10;
    cout<<endl<<"Enter what you want to do:"<<endl
	<<"1: hadd -f"<<endl
     	<<"any other no. to exit"<<endl;
    cin>>choice1;
    if(choice1==1){
      char cmd2[1000];
      sprintf(cmd2,"hadd -f %s%s.root %s%s_job*.root",ofileStart,jdlStart,ofileStart,jdlStart);
      system(cmd2);
    }
    cout<<endl<<"Enter what you want to do:"<<endl
	<<"-1: Remove added files"<<endl
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
