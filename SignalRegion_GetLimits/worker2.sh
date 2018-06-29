#!/bin/sh

#analyzeLightBSM 800 127 FastSim T5qqqqHg
executable=$1
gluinoMass=$2
nlspMass=$3
anaArg=$4
outName=$5
currDir=$(pwd)

#root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/T5bbbbZg_1600_1000_SkimmedSorted.root
#eos root://cmseos.fnal.gov ls /store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/${outName}_${gluinoMass}_${nlspMass}_SkimmedSorted.root
echo "root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/${outName}_${gluinoMass}_${nlspMass}_SkimmedSorted.root" > runFileList.txt
#inFileLoc="/home/work/vhegde/public/LPC_EOS/GMSB_skims_ST_RA2b_TreesV12/SignalRegion_v1/skimmed_SortedSignal/"
#ls -1 ${inFileLoc}*${outName}_${gluinoMass}_${nlspMass}_SkimmedSorted.root > runFileList.txt
######################################
# SETUP CMSSW STUFF...
######################################
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc491
cmsrel CMSSW_7_4_7
cd CMSSW_7_4_7/src 
cmsenv
tar -xf ${currDir}/higgsCombine.tar
cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
scram b clean
scram b -j4

cd $currDir
pwd
outRootFile="${anaArg}_${outName}_${gluinoMass}_${nlspMass}.root"
./$executable runFileList.txt $outRootFile ${anaArg}

rm runFileList.txt

echo "ls"
ls
echo "making datacards"
mkdir dataCards
#root -l -q -b 'makeDatacard_SBins.C('${gluinoMass}','${nlspMass}',"'${outRootFile}'")'
root -l -q -b 'makeDatacard_SBinsV7.C('${gluinoMass}','${nlspMass}',"'${outRootFile}'")'

echo "ls dataCards"
ls dataCards

combineCards.py dataCards/*.txt > dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt
echo "calculating limit"
mH="$(echo "${gluinoMass}+${nlspMass}*0.0001" | bc)"
echo $mH
#combine -M Asymptotic dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -t -1 -n ${outName}_${gluinoMass}_${nlspMass} -m ${mH}
combine -M Asymptotic dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -n ${outName}_${gluinoMass}_${nlspMass} -m ${mH}
#combine -M ProfileLikelihood dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -t 400 -n ${outName}_${gluinoMass}_${nlspMass} -m ${mH}
#combine -M ProfileLikelihood dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -t -1 --significance --expectSignal 1 -n ${outName}_${gluinoMass}_${nlspMass} -m ${mH}
#combine -M HybridNew --frequentist --testStat LHC dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -H ProfileLikelihood --fork 4 -n ${outName}_${gluinoMass}_${nlspMass} -m ${mH}
#combine -M ProfileLikelihood FastSim_T5bbbbZg_1600_150_bin31.txt -t 10
rm dataCard_*.txt
#rm ${outRootFile}