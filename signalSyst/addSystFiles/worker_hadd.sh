#!/bin/sh

mGl=$1
mNLSP=$2
#inputFileTag=$1
#outputFileTag=$2
#commitHash=$4
#datasetName=$4
currDir=$(pwd)
######################################
# SETUP CMSSW STUFF...
######################################
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
scram p CMSSW CMSSW_8_0_25
cd CMSSW_8_0_25/src
eval `scramv1 runtime -sh`
pwd

######################################
# SETUP PRIVATE STUFF...
######################################
echo "ls"
pwd
cd -
#cd $currDir
echo "RUNNING ANALYSIS"
pwd

hadd -f FastSim_T5qqqqHg_SystA_$mGl\_$mNLSP.root `xrdfs root://cmseos.fnal.gov ls -u /store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/temp/ | grep 'T5qqqqHg_'$mGl'_'$mNLSP'_Spring16Fast.SMS-T5qqqqHg_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_job'`

echo "----------------hadd done--------------"
ls
xrdcp -f FastSim_T5qqqqHg_SystA_$mGl\_$mNLSP.root root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/signalSystematics/hists_t5qqqqHg/
echo "-------------xrdcp done------------"
ls
rm FastSim_T5qqqqHg_SystA_$mGl\_$mNLSP.root

root -l -b -q 'numEventsComp.C("root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/signalSystematics/hists_t5qqqqHg/FastSim_T5qqqqHg_SystA_'$mGl'_'$mNLSP'.root",'$mGl','$mNLSP')'

echo "processed. ls"
ls

