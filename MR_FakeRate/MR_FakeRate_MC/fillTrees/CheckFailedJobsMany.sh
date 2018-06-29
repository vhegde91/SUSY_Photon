#!/bin/bash

for sample in 100to200 200to400 400to600 600to800 800to1200 1200to2500 2500toInf                                                                                        
do                                                                                                                                                                      
    root -l -q 'findFailedJobs.C("runList_MS_FR_DYJetsToLL_HT-'$sample'")'                                                                                              
done

#root -l -q 'findFailedJobs.C("runList_MS_FR_TTGJets")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_TTJets_HT-600to800")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_TTJets_HT-800to1200")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_TTJets_HT-1200to2500")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_TTJets_HT-2500toInf")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_TTJets_DiLept")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_TTJets_SingleLeptFromT")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_TTJets_SingleLeptFromTbar")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WJetsToLNu_HT-100To200")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WJetsToLNu_HT-200To400")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WJetsToLNu_HT-400To600")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WJetsToLNu_HT-600To800")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WJetsToLNu_HT-800To1200")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WJetsToLNu_HT-1200To2500")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WJetsToLNu_HT-2500ToInf")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WGJets_MonoPhoton_PtG-130")'
#root -l -q 'findFailedJobs.C("runList_MS_FR_WGJets_MonoPhoton_PtG-40to130")'