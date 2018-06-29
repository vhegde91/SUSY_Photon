#!/bin/bash
j="ST1000_dphiG0p3"

hadd -f runList_GJetsQCD.root runList_GJets.root runList_QCD.root
for i in QCD GJets GJetsQCD
do
    mv runList_$i.root VS_doubleR_$i\_$j.root
done

#hadd -f runList_EW.root runList_SingleTop.root runList_TTG.root runList_TTJets.root runList_WG.root runList_WJets.root runList_ZG.root runList_ZJetsToNuNu.root

#for i in EW SingleTop TTG TTJets WG WJets ZG ZJetsToNuNu
#do
#    mv runList_$i.root VS_$i\_$j.root
#done