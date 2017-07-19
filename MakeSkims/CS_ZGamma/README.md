This code processes both ZGToNuNuG and ZGToLLG. So comment or uncomment appropriate lines in the SkimmingZGamma.cc file
```
#setup cmsenv
#get code from git
svn checkout https://github.com/vhegde91/SUSY_Photon/trunk/MakeSkims/CS_ZGamma
cd CS_ZGamma
make clean; make
voms-proxy-init --voms cms

#Interactive test
./skimmingZGamma smallrunList.txt a.root CS_ZGamma

#submit a job to condor
root -l -q 'splitRunList.C("Summer16.ZGTo2NuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.txt",10)'
#If job is finished
root -l -q 'findFailedJobs.C("Summer16.ZGTo2NuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8")'

#submit many jobs to condor
./submitMany.sh
condor_q <username>
#If jobs are done
./Check_FailedJobsMany.sh

./cleaUpBatchFiles.sh

```
