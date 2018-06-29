#!/bin/bash
./zgamma CS_runList_ZGToLLG.txt CS_ZGToLLG_AppTF.root ZGToLLG
./zgamma CS_runList_DYJetsToLL_M-50.txt CS_DYJetsToLL_AppTF.root DYJetsToLL
hadd -f CS_ZDYToLLG_AppTF.root CS_ZGToLLG_AppTF.root CS_DYJetsToLL_AppTF.root

./zgamma CS_runList_ZGToNuNuG.txt CS_ZGToNuNuG_AppTF.root ZGToNuNuG
./zgamma CS_runList_ZJetsToNuNu.txt CS_ZJetsToNuNu_AppTF.root ZJetsToNuNu
hadd -f CS_ZGZJToNuNuG_AppTF.root CS_ZGToNuNuG_AppTF.root CS_ZJetsToNuNu_AppTF.root
