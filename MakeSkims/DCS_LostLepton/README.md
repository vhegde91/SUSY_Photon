This code skims for lost muon(also hadtau) and lost electron. Comment/uncomment appropriate lines in SkimmimgLostLept.cc

!!!!!!!!
Skimmed files contain SR like events as well. In main analysis code, add this line in the beginning:
```
if((Muons->size()==0) && (Electrons->size()==0)) continue;//0-lepton is SR. Blind this region.
```
!!!!!!!!

```
#setup cmsenv
#get code from git
svn checkout https://github.com/vhegde91/SUSY_Photon/trunk/MakeSkims/DCS_LostLepton
cd DCS_LostLepton
make clean; make
voms-proxy-init --voms cms

#Interactive test
./skimmingLostLept smallrunList.txt a.root DCS_LostLept

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
