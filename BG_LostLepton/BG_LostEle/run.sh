#!/bin/bash
./lostEle CS_runList_WG_LostEle_v2.txt CS_WG_LostEle_v2.root WG
./lostEle CS_runList_TTG_LostEle_v2.txt CS_TTG_LostEle_v2.root TTG

./lostEle CS_runList_WJets_LostEle_v2.txt CS_WJets_LostEle_v2.root WJets
./lostEle CS_runList_TTJets_LostEle_v2.txt CS_TTJets_LostEle_v2.root TTJets

./lostEle CS_runList_TG_LostEle_v2.txt CS_TG_LostEle_v2.root TG
./lostEle CS_runList_ZGToLLG_LostEle_v2.txt CS_ZGToLLG_LostEle_v2.root ZG
./lostEle CS_runList_DYJetsToLL_LostEle_v2.txt CS_DYJetsToLL_LostEle_v2.root ZJets

hadd -f CS_TGZGDY_LostEle_v2.root CS_TG_LostEle_v2.root CS_ZGToLLG_LostEle_v2.root CS_DYJetsToLL_LostEle_v2.root

hadd -f CS_TTWZ_LostEle_v2.root CS_WG_LostEle_v2.root CS_TTG_LostEle_v2.root CS_WJets_LostEle_v2.root CS_TTJets_LostEle_v2.root CS_TGZGDY_LostEle_v2.root

#hadd -f CS_TTW_LostEle_v2.root CS_WG_LostEle_v2.root CS_TTG_LostEle_v2.root CS_WJets_LostEle_v2.root CS_TTJets_LostEle_v2.root
