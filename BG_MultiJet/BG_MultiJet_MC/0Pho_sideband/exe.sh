#!/bin/bash
j="METShift"
hadd -f runList_GJetsQCD.root runList_GJets.root runList_QCD.root
for i in QCD GJets GJetsQCD
do
    mv runList_$i.root VS_$i\_$j.root
done