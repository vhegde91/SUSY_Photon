#!/bin/bash
#root -l -q 'splitRunList.C("runList_GJets.txt",1)'
#root -l -q 'splitRunList.C("runList_QCD.txt",3)'

root -l -q 'splitRunList.C("runList_SingleTop.txt",3)'
root -l -q 'splitRunList.C("runList_TTG.txt",3)'
root -l -q 'splitRunList.C("runList_TTJets.txt",3)'
root -l -q 'splitRunList.C("runList_WG.txt",3)'
root -l -q 'splitRunList.C("runList_WJets.txt",3)'
root -l -q 'splitRunList.C("runList_ZG.txt",3)'
root -l -q 'splitRunList.C("runList_ZJetsToNuNu.txt",3)'
