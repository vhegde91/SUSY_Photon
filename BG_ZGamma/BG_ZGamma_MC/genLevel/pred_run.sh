#!/bin/bash
./zgamma CS_runList_ZGToLLG.txt CS_ZGToLLG_AppTF.root ZGToLLG
./zgamma CS_runList_DYJetsToLL_M-50.txt CS_DYJetsToLL_AppTF.root DYJetsToLL
hadd -f CS_ZDYToLLG_AppTF.root CS_ZGToLLG_AppTF.root CS_DYJetsToLL_AppTF.root

