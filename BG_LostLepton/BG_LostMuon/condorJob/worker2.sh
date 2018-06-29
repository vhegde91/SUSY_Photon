#!/bin/sh

executable=$1
inputFileTag=$2
outputFileTag=$3
#commitHash=$4
datasetName=$4
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
ls
tar -xf SF_Files.tar
ls
echo "RUNNING ANALYSIS"
pwd
./$executable $inputFileTag $outputFileTag $datasetName
echo "processed. ls"
ls
echo "COPYING OUTPUT"

rm -r PileupHistograms_0121_69p2mb_pm4p6.root TnP_NUM_MiniIsoTight_DENOM_MediumID_VAR_map_pt_eta.root Tracking_EfficienciesAndSF_BCDEFGH.root LstMu_CS_TTWZ_LostMuHadTau_v2.root btag
#xrdcp $datasetName'_'$outputFileTag root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_RA2b_TreesV9/
#rm $datasetName'_'$outputFileTag
#for i in $( ls | grep root ) ; do 
#    xrdcp $i root://cmseos.fnal.gov//store/user/vhegde/temp1/
#done