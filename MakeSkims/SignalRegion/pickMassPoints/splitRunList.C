#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<stdlib.h>

using namespace std;
void splitRunList(string infile,int nfPerJob){
  //------------ needed for condor files --------------
  string exeCondor  = "worker2.sh";
  string exeAna     = "pickMassPoint";
  int mGl = 1150, mNLSP = 150;
  string datasetAna = "T5ttttZg_"+to_string(mGl)+"_"+to_string(mNLSP)+"_UnSkimmed";
  string filesToTransfer = "T5bbbbZg_MassScan.root,T5qqqqHg_MassScan.root,T5ttttZg_MassScan.root";
  //---------------------------------------------------
  cout<<"executable at worker node : "<<exeCondor<<endl
      <<"Analysis executable : "<<exeAna<<endl
      <<"dataset name for analysis : "<<datasetAna<<endl
      <<"making trees for m(gluino) "<<mGl<<" and m(NLSP) "<<mNLSP<<endl;
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
    sprintf(name,"FileList_%s_%i_%i_job%i.txt",dataset.c_str(),mGl,mNLSP,jobid);
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
    sprintf(name,"%s_%i_%i_job%i.jdl",dataset.c_str(),mGl,mNLSP,i);
    sprintf(fileListName,"FileList_%s_%i_%i_job%i.txt",dataset.c_str(),mGl,mNLSP,i);
    sprintf(logFile,"%s_%i_%i_job%i",dataset.c_str(),mGl,mNLSP,i);
    outf.open(name);
    outf<<"universe = vanilla"<<endl
	<<"Executable = "<<exeCondor<<endl
	<<"Requirements = OpSys == \"LINUX\" && (Arch != \"DUMMY\" )"<<endl
	// <<"request_disk = 10000000"<<endl
	// <<"request_memory = 10000"<<endl
	<<"Should_Transfer_Files = YES"<<endl
	<<"WhenToTransferOutput = ON_EXIT_OR_EVICT"<<endl
	<<"Transfer_Input_Files = "<<filesToTransfer<<","<<exeAna<<","<<fileListName<<endl
      //	<<"PeriodicRemove = ( JobStatus == 2 ) && ( ( CurrentTime - EnteredCurrentStatus ) > 600 )"<<endl
	<<"Output = "<<logFile<<".stdout"<<endl
	<<"Error = "<<logFile<<".stderr"<<endl
	<<"Log = "<<logFile<<".condor"<<endl
	<<"notification = Error"<<endl
	<<"notify_user = vhegde@FNAL.GOV"<<endl
      //	<<"x509userproxy = $ENV(X509_USER_PROXY)"<<endl
	<<"Arguments = "<<exeAna<<" "<<fileListName<<" "<<logFile<<".root "<<datasetAna<<" "<<mGl<<" "<<mNLSP<<endl
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
    sprintf(name,"condor_submit %s_%i_%i_job%i.jdl",dataset.c_str(),mGl,mNLSP,i);
    system(name); 
  }
  
}

void splitRunList(){
  cout<<"Please specify the input txt file to use and no. of files per job"<<endl;
  cout<<"splitRunList(string infile,int nfPerJob)"<<endl;
}
