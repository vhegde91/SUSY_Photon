#!/bin/bash
./analyzeLightBSM runList_ZJetsToNuNu.txt ZJetsToNuNu.root ZJets
./analyzeLightBSM runList_ZGJetsToNuNuG.txt ZGJetsToNuNuG.root ZG
./analyzeLightBSM runList_QCD.txt QCD.root QCD
./analyzeLightBSM runList_GJets.txt GJets.root GJets
./analyzeLightBSM runList_WJetsToLNu.txt WJetsToLNu.root WJets
./analyzeLightBSM runList_WGToLNuG.txt WGJetsToLNuG.root WG
./analyzeLightBSM runList_TTJets.txt TTJetsHT.root TTJets
./analyzeLightBSM runList_TTGJets.txt TTGJets.root TTG

./analyzeLightBSM runList_T5bbbbZg_1600_150.txt T5bbbbZg_1600_150_FastSim.root FastSim
./analyzeLightBSM runList_T5bbbbZg_1600_1000.txt T5bbbbZg_1600_1000_FastSim.root FastSim
./analyzeLightBSM runList_T5bbbbZg_1600_1550.txt T5bbbbZg_1600_1550_FastSim.root FastSim

./analyzeLightBSM runList_T5qqqqHg_1600_150.txt T5qqqqHg_1600_150_FastSim.root FastSim
./analyzeLightBSM runList_T5qqqqHg_1600_1000.txt T5qqqqHg_1600_1000_FastSim.root FastSim
./analyzeLightBSM runList_T5qqqqHg_1600_1550.txt T5qqqqHg_1600_1550_FastSim.root FastSim

./analyzeLightBSM runList_T1bbbb_ZG_mGl1600_NLSP150.txt T1bbbb_ZG_mGl1600_NLSP150.root SignalZ
./analyzeLightBSM runList_T1bbbb_ZG_mGl1600_NLSP1000.txt T1bbbb_ZG_mGl1600_NLSP1000.root SignalZ
./analyzeLightBSM runList_T1bbbb_ZG_mGl1600_NLSP1550.txt T1bbbb_ZG_mGl1600_NLSP1550.root SignalZ


#./analyzeLightBSM runList_ZJetsToNuNu.txt ZJetsToNuNu.root nonprompt
#./analyzeLightBSM runList_ZGJetsToNuNuG.txt ZGJetsToNuNuG.root prompt
#./analyzeLightBSM runList_QCD.txt QCD.root nonprompt
#./analyzeLightBSM runList_GJets.txt GJets.root prompt
#./analyzeLightBSM runList_WJetsToLNu.txt WJetsToLNu.root WJetsToLNu
#./analyzeLightBSM runList_WGToLNuG.txt WGJetsToLNuG.root WG
#./analyzeLightBSM runList_TTJets.txt TTJetsHT.root TTJets_HT
#./analyzeLightBSM runList_TTGJets.txt TTGJets.root TTGJets
