#!/bin/sh
model=$1
gluinoMass=$2
nlspMass=$3
outName=$1
echo "root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/${outName}_${gluinoMass}_${nlspMass}_SkimmedSorted.root" > runFileList.txt

outRootFile="FastSim_${model}_${gluinoMass}_${nlspMass}.root"
anaArg="FastSim_${model}"
########### run with genMET #################
./multijet runFileList.txt $outRootFile ${anaArg}_GenMET
mv $outRootFile genMET_$outRootFile
########### run with recoMET #################
./multijet runFileList.txt $outRootFile ${anaArg}

hadd -f genRecoMET.root $outRootFile genMET_$outRootFile
#mv genRecoMET.root $outRootFile
echo "getting bTag SF up file"
xrdcp root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/signalSystematics/hists_${outName}/FastSim_${outName}_bTagSFup_${gluinoMass}_${nlspMass}.root .
mv FastSim_${outName}_bTagSFup_${gluinoMass}_${nlspMass}.root bTagSFupFile.root

