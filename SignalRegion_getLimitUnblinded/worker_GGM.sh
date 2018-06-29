#!/bin/sh

#multijet 800 127 FastSim T5bbbbZg
executable=$1
gluinoMass=$2
nlspMass=$3
anaArg=$4
outName=$5
currDir=$(pwd)

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
#outRootFile="${anaArg}_${outName}_${gluinoMass}_${nlspMass}.root"
outRootFile="SignalCont_GGM_M1M3_LostMu.root"

echo "ls"
ls
echo "making datacards"
mkdir dataCards
root -l -q -b 'makeDatacard_SBinsV7_GGM.C('${gluinoMass}','${nlspMass}',"'${outRootFile}'")'

echo "ls dataCards"
ls dataCards

combineCards.py dataCards/*.txt > dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt
echo "calculating limit"
mH="$(echo "${gluinoMass}+${nlspMass}*0.0001" | bc)"
echo $mH
combine -M Asymptotic dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -n ${outName}_${gluinoMass}_${nlspMass} -m ${mH}
#combine -M ProfileLikelihood --significance dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -n ObsSignif_${outName}_${gluinoMass}_${nlspMass} -m ${mH}
#combine -M ProfileLikelihood --significance dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -t -1 --expectSignal=1 -n ExpSignif_${outName}_${gluinoMass}_${nlspMass} -m ${mH}
##### get photon+lepton ana limits
tar xf M1M3_leppho.tar.gz
ls
cp M1M3_leppho/counting_GMSB_${nlspMass}_${gluinoMass}.txt .
combine -M Asymptotic counting_GMSB_${nlspMass}_${gluinoMass}.txt -n PhoLep_${outName}_${gluinoMass}_${nlspMass} -m ${mH}
###### get combined limits
combineCards.py dataCard_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt counting_GMSB_${nlspMass}_${gluinoMass}.txt > dataCard_CombinedPhoLep_PhoJet_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt
combine -M Asymptotic dataCard_CombinedPhoLep_PhoJet_${anaArg}_${outName}_${gluinoMass}_${nlspMass}.txt -n PhoLep_PhoJet_${outName}_${gluinoMass}_${nlspMass} -m ${mH}

#counting_GMSB_100_1000.txt
##########################
rm dataCard_*.txt
rm ${outRootFile}
rm FastSim_GGM_M1M3_*_.root
rm *.txt
#rm counting_*.txt
#rm dataCard_CombinedPhoLep_PhoJet*.txt