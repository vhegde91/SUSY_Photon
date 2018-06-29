#!/bin/sh

#executable=$1
inputFileTag=$1
outputFileTag=$2
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
root -l -b -q 'processSigFile.C("'$inputFileTag'","'$outputFileTag'")'
#./$executable $inputFileTag $outputFileTag $datasetName
echo "processed. ls"
ls
echo "COPYING OUTPUT"

rm *MassScan.root

for FILE in *.root
do
    xrdcp -f ${FILE} root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/temp/
    rm ${FILE}
done
