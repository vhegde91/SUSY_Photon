#!/bin/bash
./multijet runList_GJets.txt GJets.root GJets
./multijet runList_QCD.txt QCD.root QCD

hadd -f gjets_qcd.root GJets.root QCD.root
