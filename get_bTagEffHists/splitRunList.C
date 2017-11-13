#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<stdlib.h>

using namespace std;
TString getSampleName(string);
void splitRunList(string infile,int nfPerJob){
  //------------ needed for condor files --------------
  string exeCondor  = "worker2.sh";
  string exeAna     = "bTagSF_Hists";
  TString datasetAna = getSampleName(infile);
  //---------------------------------------------------
  cout<<"executable at worker node : "<<exeCondor<<endl
      <<"Analysis executable : "<<exeAna<<endl
      <<"dataset name for analysis : "<<datasetAna<<endl;
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
      //	<<"Requirements = OpSys == \"LINUX\" && (Arch != \"DUMMY\" )"<<endl
      // <<"request_disk = 10000000"<<endl
      // <<"request_memory = 10000"<<endl
	<<"Should_Transfer_Files = YES"<<endl
	<<"WhenToTransferOutput = ON_EXIT_OR_EVICT"<<endl
	<<"Transfer_Input_Files = "<<exeAna<<","<<fileListName<<endl
      //	<<"PeriodicRemove = ( JobStatus == 2 ) && ( ( CurrentTime - EnteredCurrentStatus ) > 600 )"<<endl
	<<"Output = "<<logFile<<".stdout"<<endl
	<<"Error = "<<logFile<<".stderr"<<endl
	<<"Log = "<<logFile<<".condor"<<endl
	<<"notification = Error"<<endl
      //<<"notify_user = vhegde@FNAL.GOV"<<endl
      //	<<"x509userproxy = $ENV(X509_USER_PROXY)"<<endl
	<<"Arguments = "<<exeAna<<" "<<fileListName<<" "<<logFile<<".root "<<datasetAna<<endl
	<<"+LENGTH=\"SHORT\""<<endl
	<<endl
	<<"Queue 1";
    outf.close();
  }
  //------------------------ submit to condor --------------------------------------
  int t1=100;
  cout<<"Do you want to submit "<<jobid<<" jobs? If yes enter 100"<<endl;
  //  cin>>t1;
  for(int i=0;i<jobid && t1==100;i++){
    sprintf(name,"condor_submit %s_job%i.jdl",dataset.c_str(),i);
    system(name); 
  }
  
}

void splitRunList(){
  cout<<"Please specify the input txt file to use and no. of files per job"<<endl;
  cout<<"splitRunList(string infile,int nfPerJob)"<<endl;
}

TString getSampleName(string str1){
  TString tstr1 = str1;
  vector<TString> sampleName2 = {"DYJetsToLL_M_50_HT_100to200",
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
