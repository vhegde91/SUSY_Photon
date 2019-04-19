B1;3409;0c## Skimming ntuples

Recommended to use CMSSW_10_1_7 for using root.
Skimming variables and cuts are defined in SkimmingSR.cc

Other files needed to skim: SkimmingSR.h, NtupleVariables.h, NtupleVariables.cc, Makefile and a text file containg name of root files which need to be skimmed (eg. Summer16MiniAODv3.WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt)

Make changes in the SkimmingSR.cc file and then

```
make
./skimmingSR smallrunList.txt outFile.root sample
```
Now you get a root file sample_outFile.root which is skimmed.

## Use condor to make skims
To submit jobs to condor and make skims for large datasets:

1. Make text files containg name of samples using getInputFileList.sh (edit the according to your neeed)
```
./getInputFileList.sh
```

2. Submit jobs to condor using submitMany.sh. Comment lines for which you do not want to submit jobs. This script calls splitRunList.C macro for submitting jobs. Setup proxy if not done before. 
Edit worker2.sh file to write the o/p files to your eos area. (line starts with xrdcp -f .......)
```
voms-proxy-init --voms cms
./submitMany.sh
```

3. Check the status of jobs
```
condor_q
```

4. If all jobs are finished, uncomment appropriate lines in Check_FailedJobsMany.sh
```
./Check_FailedJobsMany.sh
```
This shell script uses findFailedJobs.C and findFailedJobs.C helps in identifying failed jobs. If all jobs for a sample are successfull, then it gives an option to hadd the oputput files and remove added files. If jobs have failed, it will print out commands to resubmit failed jobs. *** IMP ** Make sure that the variable `ofileStart` in findFailedJobs.C is correctly initialized based on your edit done in worker2.sh and the variable name assigned to `datasetAna` in splitRunList.C .

5. If every job is successfully completed checked some of the condor logs (.stdout, .stderr and .condor), then clean up the log files:
```
./cleaUpBatchFiles.sh 

```