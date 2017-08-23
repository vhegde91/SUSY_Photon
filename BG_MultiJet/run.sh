#!/bin/bash
./analyzeLightBSM runList_QCD.txt $1_QCD.root QCD
./analyzeLightBSM runList_GJets.txt $1_GJets.root GJets
hadd -f $1_MultiJet.root $1_QCD.root $1_GJets.root
#rm $1_QCD.root $1_GJets.root
