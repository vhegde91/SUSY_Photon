```
#setup cmsenv
#get code from git
svn checkout https://github.com/vhegde91/SUSY_Photon/trunk/MakeSkims/CS_LostLepton
cd CS_LostLepton
make clean; make
voms-proxy-init --voms cms

#Interactive test
./skimmingLostLept smallrunList.txt a.root CS_LostLept

#submit a job to condor
root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",21)'
#If job is finished
root -l -q 'findFailedJobs.C("Summer16.WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'

#submit many jobs to condor
./submitMany.sh
condor_q <username>
#If jobs are done
./Check_FailedJobsMany.sh

./cleaUpBatchFiles.sh

```
