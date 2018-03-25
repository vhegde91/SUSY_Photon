#!/bin/bash
./multijet runList_ZJetsToNuNu.txt ZJetsToNuNu.root ZJets
#./multijet runList_ZGJetsToNuNuG.txt ZGJetsToNuNuG.root ZG
./multijet CS_runList_ZGToNuNuG.txt ZGJetsToNuNuG.root ZG
hadd -f ZGZJ_NuNuG.root ZJetsToNuNu.root ZGJetsToNuNuG.root
./multijet runList_QCD.txt QCD.root QCD
./multijet runList_GJets.txt GJets.root GJets
./multijet runList_WJetsToLNu.txt WJetsToLNu.root WJets
./multijet runList_WGToLNuG.txt WGJetsToLNuG.root WG
./multijet runList_TTJets.txt TTJetsHT.root TTJets
./multijet runList_TTGJets.txt TTGJets.root TTG

#./multijet runList_T5bbbbZg_1600_150.txt T5bbbbZg_1600_150_FastSim.root FastSim
#./multijet runList_T5bbbbZg_1600_1000.txt T5bbbbZg_1600_1000_FastSim.root FastSim
#./multijet runList_T5bbbbZg_1600_1550.txt T5bbbbZg_1600_1550_FastSim.root FastSim

#./multijet runList_T5qqqqHg_1600_150.txt T5qqqqHg_1600_150_FastSim.root FastSim
#./multijet runList_T5qqqqHg_1600_1000.txt T5qqqqHg_1600_1000_FastSim.root FastSim
#./multijet runList_T5qqqqHg_1600_1550.txt T5qqqqHg_1600_1550_FastSim.root FastSim

#./multijet runList_T1bbbb_ZG_mGl1600_NLSP150.txt T1bbbb_ZG_mGl1600_NLSP150.root SignalZ
#./multijet runList_T1bbbb_ZG_mGl1600_NLSP1000.txt T1bbbb_ZG_mGl1600_NLSP1000.root SignalZ
#./multijet runList_T1bbbb_ZG_mGl1600_NLSP1550.txt T1bbbb_ZG_mGl1600_NLSP1550.root SignalZ

