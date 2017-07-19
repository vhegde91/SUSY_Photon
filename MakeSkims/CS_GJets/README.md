```
#setup cmsenv
#get code from git
svn checkout https://github.com/vhegde91/SUSY_Photon/trunk/MakeSkims/CS_GJets
cd CS_GJets
make clean; make
voms-proxy-init --voms cms

#Interactive test
./skimmingGJets smallrunList.txt a.root CS_GJets

#submit a job to condor
root -l -q 'splitRunList.C("Summer16.GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
#If job is finished
root -l -q 'findFailedJobs.C("Summer16.GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt")'

#submit many jobs to condor
./submitMany.sh
condor_q <username>
#If jobs are done
./Check_FailedJobsMany.sh

./cleaUpBatchFiles.sh

```
