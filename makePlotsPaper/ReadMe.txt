#!/bin/sh
#Set up root version > 6. Works well for root setup from CMSSW_8_0_25
cp /uscms/home/vhegde/nobackup/Physics/Code_SUS_18_002/makePlotsPaper/* .

# getting exclusion plots
root -l -b -q 'getObsExpLimits.C("T5bbbbZg_LimitPlots_Obs_V7_v11.root")'
root -l -b -q 'getObsExpLimits.C("T5qqqqHg_LimitPlots_Obs_V7_v11.root")'
root -l -b -q 'getObsExpLimits.C("T5ttttZg_LimitPlots_Obs_V7_v11.root")'
root -l -b -q 'getObsExpLimits.C("T6ttZg_LimitPlots_Obs_V7_v11.root")'

#lost lepton closure plots
root -l -b -q testClosure.C

#double ratio data vs MC
root -l doubleR_dataMC_merged.C -b -q

#final results - Exp vs Pred
root -l -b -q 'dataPred_SBinsOnly.C("SBinHists.root")'
