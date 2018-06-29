#!/bin/bash
./zgamma CS_runList_ZGToLLG.txt CS_ZGToLLG.root ZGToLLG
./zgamma CS_runList_DYJetsToLL_M-50.txt CS_DYJetsToLL.root DYJetsToLL
hadd -f CS_ZDYToLLG.root CS_ZGToLLG.root CS_DYJetsToLL.root

./zgamma CS_runList_ZGToNuNuG.txt CS_ZGToNuNuG.root ZGToNuNuG
./zgamma CS_runList_ZJetsToNuNu.txt CS_ZJetsToNuNu.root ZJetsToNuNu
hadd -f CS_ZGZJToNuNuG.root CS_ZGToNuNuG.root CS_ZJetsToNuNu.root
