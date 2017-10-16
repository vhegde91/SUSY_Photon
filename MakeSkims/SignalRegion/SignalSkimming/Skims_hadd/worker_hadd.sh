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

hadd -f TChiNG_$mGl\_$mNLSP\_SkimmedSorted.root `xrdfs root://cmseos.fnal.gov ls -u /store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/ | grep 'TChiNG_'$mGl'_'$mNLSP'_Spring16Fast.SMS-TChiNG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_job'`
xrdcp -f TChiNG_$mGl\_$mNLSP\_SkimmedSorted.root root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/
rm TChiNG_$mGl\_$mNLSP\_SkimmedSorted.root

#goodOPFile=0
root -l -b -q 'numEventsComp.C("root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/TChiNG_'$mGl'_'$mNLSP'_SkimmedSorted.root",'$mGl','$mNLSP')'

echo "processed. ls"
ls

