#!/bin/sh   
root -l -q 'splitRunList.C("CS_runList_TTJets_HT600ToInf.txt",10)'
root -l -q 'splitRunList.C("CS_runList_WJets_HT100ToInf.txt",10)'
root -l -q 'splitRunList_madHT0to600.C("CS_runList_TTJets_Lept.txt",10)'

#root -l -q 'findFailedJobs.C("CS_runList_TTJets_HT600ToInf")'
#root -l -q 'findFailedJobs.C("CS_runList_WJets_HT100ToInf")'
#root -l -q 'findFailedJobs.C("CS_runList_TTJets_Lept")'
