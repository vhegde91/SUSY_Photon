!!Work in progress!!

```
#setup cmsenv
#get code from git
svn checkout https://github.com/vhegde91/SUSY_Photon/trunk/MakeSkims/MS_EleFakes
cd MS_EleFakes
make clean; make
voms-proxy-init --voms cms

#Interactive test
./skimmingFakeRate smallrunList.txt a.root MS_FR

#submit a job to condor
root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",20)'
#If job is finished
root -l -q 'findFailedJobs.C("Summer16.DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'

#submit many jobs to condor
./submitMany.sh
condor_q <username>
#If jobs are done
./Check_FailedJobsMany.sh

./cleaUpBatchFiles.sh

```
