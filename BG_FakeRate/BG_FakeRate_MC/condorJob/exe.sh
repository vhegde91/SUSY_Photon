#!/bin/sh   
#root -l -q 'splitRunList.C("CS_runList_TT_FR.txt",2)'
#root -l -q 'splitRunList.C("CS_runList_WJets_FR.txt",2)'

root -l -q 'findFailedJobs.C("CS_runList_TT_FR")'
root -l -q 'findFailedJobs.C("CS_runList_WJets_FR")'
