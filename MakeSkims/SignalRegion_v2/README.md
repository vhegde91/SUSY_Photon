Refer to https://github.com/vhegde91/SUSY_Photon/tree/master/MakeSkims/SignalRegion for main SR skims.

This is meant for comparing summer16 MC with spring16 MC.

#setup cmsenv
#get code from git
svn checkout https://github.com/vhegde91/SUSY_Photon/trunk/MakeSkims/SignalRegion_v2
cd SignalRegion_v2
make clean; make
voms-proxy-init --voms cms

#Interactive test
./skimmingSR smallrunList.txt a.root SR

#submit a job to condor
root -l -q 'splitRunList.C("spring16_gjets100to200.txt",10)'
#If job is finished
root -l -q 'findFailedJobs.C("spring16_gjets100to200")'

#submit many jobs to condor
./submitMany.sh
condor_q <username>
#If jobs are done
./Check_FailedJobsMany.sh

./cleaUpBatchFiles.sh

```
