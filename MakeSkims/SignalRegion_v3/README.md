Refer to https://github.com/vhegde91/SUSY_Photon/tree/master/MakeSkims/SignalRegion for main SR skims.

This is a old version in which skimming was done with (HT > 500 || ST > 500). Now skims are done with ST > 500 only.

#setup cmsenv
#get code from git
svn checkout https://github.com/vhegde91/SUSY_Photon/trunk/MakeSkims/SignalRegion_v3
cd SignalRegion_v3
make clean; make
voms-proxy-init --voms cms

#Interactive test
./skimmingSR smallrunList.txt a.root SR

#submit a job to condor
root -l -q 'splitRunList.C("Summer16.GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
#If job is finished
root -l -q 'findFailedJobs.C("Summer16.GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'

#submit many jobs to condor
./submitMany.sh
condor_q <username>
#If jobs are done
./Check_FailedJobsMany.sh

./cleaUpBatchFiles.sh

```
