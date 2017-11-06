#!/bin/bash
#root -l -q 'findFailedJobs.C("runList_QCD")'
#root -l -q 'findFailedJobs.C("runList_GJets")'

root -l -q 'findFailedJobs.C("runList_SingleTop")'
root -l -q 'findFailedJobs.C("runList_TTG")'
root -l -q 'findFailedJobs.C("runList_TTJets")'
root -l -q 'findFailedJobs.C("runList_WG")'
root -l -q 'findFailedJobs.C("runList_WJets")'
root -l -q 'findFailedJobs.C("runList_ZG")'
root -l -q 'findFailedJobs.C("runList_ZJetsToNuNu")'
