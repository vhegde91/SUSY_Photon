#!/bin/bash
./zgamma CS_runList_ZGToLLG_PtG130_LO.txt CS_ZGToLLG_LO_PtG190.root ZGToLLG
./zgamma CS_runList_DYJetsToLL_M-50.txt CS_DYJetsToLL_PtG190.root DYJetsToLL
hadd -f CS_ZDYToLLG_LO_PtG190.root CS_DYJetsToLL_PtG190.root CS_ZGToLLG_LO_PtG190.root

./zgamma CS_runList_ZGToNuNuG_PtG130_LO.txt CS_ZGToNuNuG_LO_PtG190.root ZGToNuNuG
./zgamma CS_runList_ZJetsToNuNu.txt CS_ZJetsToNuNu_PtG190.root ZJetsToNuNu
hadd -f CS_ZGZJToNuNuG_LO_PtG190.root CS_ZGToNuNuG_LO_PtG190.root CS_ZJetsToNuNu_PtG190.root

#./zgamma CS_runList_ZGToLLG_PtG130_LO.txt CS_LDP_ZGToLLG_LO_PtG190.root ZGToLLG
#./zgamma CS_runList_DYJetsToLL_M-50.txt CS_LDP_DYJetsToLL_PtG190.root DYJetsToLL
#hadd -f CS_LDP_ZDYToLLG_LO_PtG190.root CS_LDP_DYJetsToLL_PtG190.root CS_LDP_ZGToLLG_LO_PtG190.root

#./zgamma CS_runList_ZGToNuNuG_PtG130_LO.txt CS_LDP_ZGToNuNuG_LO_PtG190.root ZGToNuNuG
#./zgamma CS_runList_ZJetsToNuNu.txt CS_LDP_ZJetsToNuNu_PtG190.root ZJetsToNuNu
#hadd -f CS_LDP_ZGZJToNuNuG_LO_PtG190.root CS_LDP_ZGToNuNuG_LO_PtG190.root CS_LDP_ZJetsToNuNu_PtG190.root


#./zgamma CS_runList_ZGToLLG.txt CS_ZGToLLG.root ZGToLLG
#./zgamma CS_runList_DYJetsToLL_M-50.txt CS_DYJetsToLL.root DYJetsToLL
#hadd -f CS_ZDYToLLG.root CS_ZGToLLG.root CS_DYJetsToLL.root

#./zgamma CS_runList_ZGToNuNuG.txt CS_ZGToNuNuG.root ZGToNuNuG
#./zgamma CS_runList_ZJetsToNuNu.txt CS_ZJetsToNuNu.root ZJetsToNuNu
#hadd -f CS_ZGZJToNuNuG.root CS_ZGToNuNuG.root CS_ZJetsToNuNu.root
