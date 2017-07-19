```
#setup cmsenv
#get code from git
svn checkout https://github.com/vhegde91/SUSY_Photon/trunk/MakeSkims/DCS_GJets
cd DCS_GJets
make clean; make
voms-proxy-init --voms cms

#Interactive test
./skimmingGJets smallrunList.txt a.root DCS_GJets

#submit a job to condor
root -l -q 'splitRunList.C("Run2016B_SinglePhoton_03Feb2017.txt",21)'
#If job is finished
root -l -q 'findFailedJobs.C("Run2016B_SinglePhoton_03Feb2017")'

#submit many jobs to condor
./submitMany.sh
condor_q <username>
#If jobs are done
./Check_FailedJobsMany.sh

./cleaUpBatchFiles.sh

```
