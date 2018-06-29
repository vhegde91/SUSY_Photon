#!/bin/sh
arg=$1
echo $arg
if [ "$arg" == "sub" ]; then
#if [ "$arg" == "valid" ]; then
    root -l -q 'splitRunList.C("CS_runList_TTJets_FR.txt",2)'
    root -l -q 'splitRunList.C("CS_runList_WJets_FR.txt",2)'
fi

if [ "$arg" == "che" ]; then
    root -l -q 'findFailedJobs.C("CS_runList_TTJets_FR")'
    root -l -q 'findFailedJobs.C("CS_runList_WJets_FR")'
fi
