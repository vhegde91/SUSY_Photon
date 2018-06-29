#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include"TFile.h"
#include"TEnv.h"

using namespace std;

void findFailedJobs_v2(string arg){
  gEnv->SetValue("TFile.Recover", 0);
  char name1[400],name2[1000];
  vector<int> failedID;
  int totalJobs=0;
  string ofileStart = "/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/CS_GJets_v2/CS_MultiJets_madHT0to600";
  string ofileLoc = "/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/CS_GJets_v2/";

  for(int i=0;;i++){
    sprintf(name1,"%s_job%i.jdl",arg.c_str(),i);
    ifstream infile(name1);
    if(!infile) {cout<<"Total jdl="<<i<<endl;totalJobs=i;break;}
    sprintf(name2,"root://cmseos.fnal.gov/%s%s_job%i.root",ofileStart.c_str(),arg.c_str(),i);
    TFile *f1= TFile::Open(name2);
    if(!f1 || f1->IsZombie()) {
      failedID.push_back(i);
      delete f1;
    }
  }

  cout<<"-----------------------------------------------"<<endl;
  for(int i=0;i<failedID.size();i++){
    cout<<"condor_submit "<<arg<<"_job"<<failedID[i]<<".jdl"<<endl;
  }
  cout<<"-----------------------------------------------"<<endl;
  cout<<"Total failed "<<arg<<" jobs:"<<failedID.size()<<endl;

  if(failedID.size() == 0 && totalJobs!=0) {
    char cmd0[1000];
    sprintf(cmd0,"du -sh /eos/uscms%s%s_job0.root",ofileStart.c_str(),arg.c_str());
    system(cmd0);
    cout<<endl;
    string haddCMD = "hadd -f /eos/uscms"+ofileStart+arg+".root `xrdfs root://cmseos.fnal.gov ls -u "+ofileLoc+"| grep '"+arg+"_job'`";
    cout<<haddCMD<<endl;
    cout<<"rm "<<ofileStart<<arg<<"_job*.root"<<endl;
    int choice1=10;
    cout<<endl<<"Enter what you want to do:"<<endl
       	<<"1: hadd only"<<endl
     	<<"any other no. to exit"<<endl;
    cin>>choice1;
    if(choice1==1){
      char cmd2[1000];
      sprintf(cmd2,"%s",haddCMD.c_str());
      system(cmd2);
      //      cout<<cmd2;
      TString oFileName  = "root://cmseos.fnal.gov/"+ofileStart+arg+".root";
      TFile *f1= TFile::Open(oFileName);
      if( f1 && !(f1->IsZombie()) ) {
	cout<<endl<<"Enter what you want to do:"<<endl
	    <<"-1 to remove hadded(successfull or not) files"<<endl
	    <<"any other no. to exit"<<endl;
	cin>>choice1;
	if(choice1==-1){
	  cout<<endl<<"Removing job files"<<endl;
	  sprintf(cmd2,"rm /eos/uscms%s%s_job*.root",ofileStart.c_str(),arg.c_str());
	  system(cmd2);
	  //  cout<<cmd2<<endl;
	}
      }
      delete f1;
    }
  }
}
