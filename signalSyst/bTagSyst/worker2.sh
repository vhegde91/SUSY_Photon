#!/bin/sh

#analyzeLightBSM 800 127 FastSim_T5qqqqHg T5qqqqHg
executable=$1
gluinoMass=$2
nlspMass=$3
anaArg=$4
outName=$5
currDir=$(pwd)

echo "root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/${outName}_${gluinoMass}_${nlspMass}_SkimmedSorted.root" > runFileList.txt
######################################
# SETUP CMSSW STUFF...
######################################
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
scram p CMSSW CMSSW_8_0_25
cd CMSSW_8_0_25/src
eval `scramv1 runtime -sh`

echo "done setting up cmsenv"
pwd
cd -
pwd
tar -xf btagFiles.tar
#tar -xf ${currDir}/higgsCombine.tar
#cd $currDir
pwd
#outRootFile="${anaArg}_${outName}_${gluinoMass}_${nlspMass}.root"
outRootFile="${anaArg}_${gluinoMass}_${nlspMass}.root"
./$executable runFileList.txt $outRootFile ${anaArg}

rm runFileList.txt

echo "ls"
ls
