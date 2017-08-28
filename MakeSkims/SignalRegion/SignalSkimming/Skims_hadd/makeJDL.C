#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include"TFile.h"
#include"TH2.h"
#include"TH1.h"

using namespace std;
void makeJDL(){
  //------------ needed for condor files --------------
  string exeCondor  = "worker_hadd.sh";
  string filesToTransfer = "numEventsComp.C,T5bbbbZg_MassScan.root,T5qqqqHg_MassScan.root,T5ttttZg_MassScan.root";
  string dataset="T5qqqqHg";

  int jobid=0,nfPerJob=1;
  char name[200];
  char fileListName[200],logFile[200];
  ofstream outf;
  // for(int i=0,j=0;i<fname.size();){
  //   sprintf(name,"FileList_%s_job%i.txt",dataset.c_str(),jobid);
  //   outf.open(name);
  //   for(j=0;j<nfPerJob && i<fname.size();j++){
  //     outf<<fname[i]<<endl;
  //     i++;
  //   }
  //   jobid++;
  //   outf.close();
  // }
  TFile *f1 = new TFile("T5qqqqHg_MassScan.root");
  TH2D *h2_mass = (TH2D*)f1->FindObjectAny("MGlMNLSP");
  int nxbins = h2_mass->GetNbinsX();
  int nybins = h2_mass->GetNbinsY();
  double entries=-100;
  for(int nx=1;nx<=nxbins;nx++){
    //    if(h2_mass->GetXaxis()->GetBinCenter(nx)>1001) continue;
    if(h2_mass->GetXaxis()->GetBinCenter(nx)<1001) continue;
    //    if(h2_mass->GetXaxis()->GetBinCenter(nx)>1501) break;
    for(int ny=1;ny<=nybins;ny++){
      double mgl=h2_mass->GetXaxis()->GetBinCenter(nx);
      double mnlsp=h2_mass->GetYaxis()->GetBinCenter(ny);
      int mGl1=mgl,mNLSP1=mnlsp;
      if(h2_mass->GetBinContent(nx,ny) < 0.1) continue;
      if(mnlsp==127 && mGl1==1050) continue;
      sprintf(name,"%s_job_%i_%i.jdl",dataset.c_str(),mGl1,mNLSP1);
      //sprintf(fileListName,"FileList_%s_job%i.txt",dataset.c_str(),mGl1,mNLSP1);
      sprintf(logFile,"%s_job_%i_%i",dataset.c_str(),mGl1,mNLSP1);
      outf.open(name);
      outf<<"universe = vanilla"<<endl
	  <<"Executable = "<<exeCondor<<endl
	  <<"Requirements = OpSys == \"LINUX\" && (Arch != \"DUMMY\" )"<<endl
	  <<"Should_Transfer_Files = YES"<<endl
	  <<"WhenToTransferOutput = ON_EXIT_OR_EVICT"<<endl
	  <<"Transfer_Input_Files = "<<filesToTransfer<<endl
	//	<<"PeriodicRemove = ( JobStatus == 2 ) && ( ( CurrentTime - EnteredCurrentStatus ) > 600 )"<<endl
	  <<"Output = "<<logFile<<".stdout"<<endl
	  <<"Error = "<<logFile<<".stderr"<<endl
	  <<"Log = "<<logFile<<".condor"<<endl
	  <<"notification = Error"<<endl
	  <<"notify_user = vhegde@FNAL.GOV"<<endl
	  <<"x509userproxy = $ENV(X509_USER_PROXY)"<<endl
	  <<"Arguments = "<<mGl1<<" "<<mNLSP1<<endl
	//    <<"Arguments = "<<exeAna<<" "<<fileListName<<" "<<logFile<<".root "<<datasetAna<<endl;
	  <<"+LENGTH=\"SHORT\""<<endl
	  <<endl
	  <<"Queue 1";
      outf.close();
      sprintf(name,"condor_submit %s_job_%i_%i.jdl",dataset.c_str(),mGl1,mNLSP1);
      system(name); 
    }
  }
}
