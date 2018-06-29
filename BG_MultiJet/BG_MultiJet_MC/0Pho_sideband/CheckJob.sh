#!/bin/bash
root -l -q 'findFailedJobs.C("runList_QCD")'
root -l -q 'findFailedJobs.C("runList_GJets")'

#root -l -q 'findFailedJobs.C("runList_QCD_HT500to700")'
#root -l -q 'findFailedJobs.C("runList_QCD_HT700to1000")'
#root -l -q 'findFailedJobs.C("runList_QCD_HT1000to1500")'
#root -l -q 'findFailedJobs.C("runList_QCD_HT1500to2000")'
#root -l -q 'findFailedJobs.C("runList_QCD_HT2000toInf")'

#root -l -q 'findFailedJobs.C("runList_SingleTop")'
#root -l -q 'findFailedJobs.C("runList_TTG")'
#root -l -q 'findFailedJobs.C("runList_TTJets")'
#root -l -q 'findFailedJobs.C("runList_WG")'
#root -l -q 'findFailedJobs.C("runList_WJets")'
#root -l -q 'findFailedJobs.C("runList_ZG")'
#root -l -q 'findFailedJobs.C("runList_ZJetsToNuNu")'
