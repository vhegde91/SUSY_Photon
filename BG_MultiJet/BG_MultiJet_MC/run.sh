#!/bin/bash
./multijet runList_GJets.txt GJets.root GJets
./multijet runList_QCD.txt QCD.root QCD

hadd -f gjets_qcd.root GJets.root QCD.root

#./multijet runList_T1bbbb_ZG_mGl1600_NLSP150.txt FullSim_T1bbbbZG_1600_150.root SignalZ
#./multijet runList_T1bbbb_ZG_mGl1600_NLSP1550.txt FullSim_T1bbbbZG_1600_1550.root SignalZ

#./multijet runList_T5bbbbZg_1600_150.txt FastSim_T5bbbbZG_1600_150.root FastSim
#./multijet runList_T5bbbbZg_1600_1550.txt FastSim_T5bbbbZG_1600_1550.root FastSim
