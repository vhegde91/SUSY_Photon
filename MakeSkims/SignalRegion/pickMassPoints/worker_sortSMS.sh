#!/bin/sh
executable=$1
inputFileTag=$2
outputFileTag=$3
#commitHash=$4
datasetName=$4
momMass=$5
kidMass=$6
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
sort --random-sort $inputFileTag > runList.txt
./$executable runList.txt $outputFileTag $datasetName $momMass $kidMass
echo "processed. ls"
ls
echo "COPYING OUTPUT"

#xrdcp -f $datasetName'_'$outputFileTag root://cmseos.fnal.gov//store/user/vhegde/myProduction_V12/
mv $datasetName'_'$momMass'_'$kidMass'_'$outputFileTag $datasetName'_'$outputFileTag
xrdcp -f $datasetName'_'$outputFileTag root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/
#xrdcp -f $datasetName'_'$outputFileTag root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyPhotonMET/Run2ProductionV12/sorted_SMS/
rm $datasetName'_'$outputFileTag
rm runList.txt
rm $inputFileTag