#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<stdlib.h>

using namespace std;
void splitRunList(string infile){
  //------------ needed for condor files --------------
  string exeCondor  = "worker2.sh";
  string exeAna     = "lostMuon";
  string filesToTransfer = "T5bbbbZg_MassScan.root,T5qqqqHg_MassScan.root,T5ttttZg_MassScan.root,T6ttZg_MassScan.root,PileupHistograms_0121_69p2mb_pm4p6.root,TnP_NUM_MiniIsoTight_DENOM_MediumID_VAR_map_pt_eta.root,Tracking_EfficienciesAndSF_BCDEFGH.root,files_btag.tar,GGM_M1M3_MassScan.root";
  string datasetAna = "FastSim_GGM_M1M3";
  TString temp1=infile;
  if(temp1.Contains("T5bbbbZg")) datasetAna = "FastSim_T5bbbbZg";
  else if(temp1.Contains("T5ttttZg")) datasetAna = "FastSim_T5ttttZg";
  else if(temp1.Contains("T5qqqqHg")) datasetAna = "FastSim_T5qqqqHg";
  else if(temp1.Contains("T6ttZg")) datasetAna = "FastSim_T6ttZg";
  else if(temp1.Contains("GGM_GravitinoLSP")) datasetAna = "FastSim_GGM_M1M3";
  //---------------------------------------------------
  cout<<"executable at worker node : "<<exeCondor<<endl
      <<"Analysis executable : "<<exeAna<<endl
      <<"Submits one job per file."<<endl;
  int nfPerJob = 1;
  //----------------- split the input files into smaller ones ------------------
  ifstream file(infile);
  if(!file){cout<<"Couldn't Open File "<<infile<<endl;}
  string str,dataset=infile;
  dataset.pop_back();  dataset.pop_back();  dataset.pop_back();  dataset.pop_back();
  vector<string> fname;
  while (std::getline(file, str))
    {
      fname.push_back(str);
    }
  file.close();

  int jobid=0;
  char name[200];
  ofstream outf;
  for(int i=0,j=0;i<fname.size();){
    sprintf(name,"FileList_%s_job%i.txt",dataset.c_str(),jobid);
    outf.open(name);
    for(j=0;j<nfPerJob && i<fname.size();j++){
      outf<<fname[i]<<endl;
      i++;
    }
    jobid++;
    outf.close();
  }

  //--------------------- make files for codor ------------------------------------
  char fileListName[200],logFile[200];
  for(int i=0;i<jobid;i++){
    sprintf(name,"%s_job%i.jdl",dataset.c_str(),i);
    sprintf(fileListName,"FileList_%s_job%i.txt",dataset.c_str(),i);
    sprintf(logFile,"%s_job%i",dataset.c_str(),i);
    outf.open(name);
    outf<<"universe = vanilla"<<endl
	<<"Executable = "<<exeCondor<<endl
	<<"Requirements = OpSys == \"LINUX\" && (Arch != \"DUMMY\" )"<<endl
	// <<"request_disk = 10000000"<<endl
	// <<"request_memory = 10000"<<endl
	<<"Should_Transfer_Files = YES"<<endl
	<<"WhenToTransferOutput = ON_EXIT_OR_EVICT"<<endl
	<<"Transfer_Input_Files = "<<exeAna<<","<<fileListName<<","<<filesToTransfer<<endl
      //	<<"PeriodicRemove = ( JobStatus == 2 ) && ( ( CurrentTime - EnteredCurrentStatus ) > 600 )"<<endl
	<<"Output = "<<logFile<<".stdout"<<endl
	<<"Error = "<<logFile<<".stderr"<<endl
	<<"Log = "<<logFile<<".condor"<<endl
	<<"notification = Error"<<endl
      //	<<"notify_user = vhegde@FNAL.GOV"<<endl
      //    	<<"x509userproxy = $ENV(X509_USER_PROXY)"<<endl
      //	<<"Arguments = "<<fileListName<<" "<<logFile<<".root "<<endl
        <<"Arguments = "<<exeAna<<" "<<fileListName<<" "<<logFile<<".root "<<datasetAna<<endl
	<<"+LENGTH=\"SHORT\""<<endl
	<<endl
	<<"Queue 1";
    outf.close();
  }
  //------------------------ submit to condor --------------------------------------
  int t1=0;
  cout<<"Do you want to submit "<<jobid<<" jobs? If yes enter 100"<<endl;
  cin>>t1;
  for(int i=0;i<jobid && t1==100;i++){
    sprintf(name,"condor_submit %s_job%i.jdl",dataset.c_str(),i);
    system(name); 
  }
  
}
