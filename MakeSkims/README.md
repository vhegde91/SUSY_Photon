# Making Skims for SUSY search with photon, MET, Jets and b-jets

## Instructions
Setup CMSSW and get code from git if not done before.
```
cmsrel CMSSW_8_0_25
cd CMSSW_8_0_25/src/
cmsenv
voms-proxy-init --voms cms

git clone git@github.com:vhegde91/SUSY_Photon.git
# wget https://github.com/vhegde91/SUSY_Photon.git
cd MakeSkims
```
### Skims Input and Output
Make skimmed trees from V12 RA2b trees. RA2b trees are located here:
```
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12/
root://cmseos.fnal.gov//store/user/vhegde/myProduction_V12/
```

Skimmed files:
` root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/ `


CS_*: Control Sample.

DCS_*: Data Control Sample.

MS_*: Measurement Sample. For ex, sample for measuring fakerate using tag & probe.

DMS_*: Data Measurement Sample.

### Description About skimming:
Each of these directories contain code for skimming RA2b trees. Main skimming code is inside each of these directories with name SkimmingXXX.cc
Skimming parametrs are specified in SkimmingXXX.cc file. Variable and functions used in SkimmingXXX.cc file are defined in SkimmingXXX.h file. In addition to these, two more files are needed: NtupleVariables.h and NtupleVariables.cc.

Code needs to be compiled using Makefile. This gives an executable skimmingxxx.

Instructions specific to each skimming is given in each of the direcories. Below is some description in general.

#### To make skim interactively:
`
./skimmingxxx inFile.txt outFile.root addInfo
`
inFile.txt: is a text fil containg list of root files on which skimming is done.

outFile.root: Ouptut skimmed tree. Output file name will be addInfo_outFile.root

addInfo: additional info about type of skimimg. For ex, LostMu for skiming lost muon studies.	

#### Make skims using condor:

$ ./submitMany.sh

This submits jobs to condor using splitRunList.C script.

1) splitRunList.C: 

Modify 3 lines in this script depending on type of skim you are making:

`  string exeCondor  = "worker2.sh";//name of the shell script to run at worker node. `
`  string exeAna     = "skimmingxxx";//name of the executable you created using Makefile `
`  string datasetAna = "addInfo";//a name of for type of skim(ex: "CS_FR" ) `

1st arguement is name of the text file in which name of ALL root files is mentioned. 2nd is number of root files to process per job. For ex:

root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",21)'

If there are 210 file names in Summer16.WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt, then it submits 10 condor jobs with 21 root files to process per job.

2) worker2.sh

This is the shell script that runs at worker node. In this file, you have to metion where your condor output root files need to be written.

xrdcp -f $datasetName'_'$outputFileTag root://cmseos.fnal.gov//store/user/vhegde/myLocation/

3) findFailedJobs.C

Use this script once the jobs are done. This checks if the o/p root files are good/absent/corrupted. If a job has failed or o/p file is corrupted, then you will get a command to resubmit these jobs. If all jobs are successful, then you will get an option to hadd files. !! Sometimes files may be very big, so do not hadd !!!. Also it gives a option to remove added files.

Modify this line depending on type of skim and location where the o/p root files are located(location mentioned in worker2.sh)
`
char ofileStart[300]="/eos/uscms/store/user/vhegde/myLocation/addInfo_";
`
Arguement: 

Name of the text file in which name of ALL root files is mentioned w/o .txt. For ex: root -l -q 'findFailedJobs.C("Summer16.WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'

4) Check_FailedJobsMany.sh

Check the o/p root files from different jobs. It just runs findFailedJobs.C many times for different i/p files.

5) cleaUpBatchFiles.sh

Remove files related to condor: FileList*.txt, *.jdl, *.condor, *.stderr, *.stdout