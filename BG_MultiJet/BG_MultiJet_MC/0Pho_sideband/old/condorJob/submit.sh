#!/bin/bash
root -l -q 'splitRunList.C("runList_GJets.txt",1)'
root -l -q 'splitRunList.C("runList_QCD.txt",5)'