#!/bin/bash
./lostMuon CS_runList_WG_LostMu_v2.txt CS_WG_LostMuHadTau_v2.root WG
./lostMuon CS_runList_TTG_LostMu_v2.txt CS_TTG_LostMuHadTau_v2.root TTG

./lostMuon CS_runList_WJets_LostMu_v2.txt CS_WJets_LostMuHadTau_v2.root WJets
./lostMuon CS_runList_TTJets_LostMu_v2.txt CS_TTJets_LostMuHadTau_v2.root TTJets

./lostMuon CS_runList_TG_LostMu_v2.txt CS_TG_LostMuHadTau_v2.root TG
./lostMuon CS_runList_ZGToLLG_LostMu_v2.txt CS_ZGToLLG_LostMuHadTau_v2.root ZG
./lostMuon CS_runList_DYJetsToLL_LostMu_v2.txt CS_DYJetsToLL_LostMuHadTau_v2.root ZJets

hadd -f CS_TGZGDY_LostMuHadTau_v2.root CS_TG_LostMuHadTau_v2.root CS_ZGToLLG_LostMuHadTau_v2.root CS_DYJetsToLL_LostMuHadTau_v2.root


hadd -f CS_TTWZ_LostMuHadTau_v2.root CS_WG_LostMuHadTau_v2.root CS_TTG_LostMuHadTau_v2.root CS_WJets_LostMuHadTau_v2.root CS_TTJets_LostMuHadTau_v2.root CS_TGZGDY_LostMuHadTau_v2.root

#hadd -f CS_TTWZ_LostMuHadTau_odd.root CS_WG_LostMuHadTau_v2.root CS_TTG_LostMuHadTau_v2.root CS_WJets_LostMuHadTau_v2.root CS_TTJets_LostMuHadTau_v2.root CS_TGZGDY_LostMuHadTau_v2.root

#./lostMuon CS_runList_TTW_LostMu_v2.txt CS_TTW_LostMuHadTau_v2.root CS_TTW_LostMu


./lostMuon runList_T5ttttZg_1800_150.txt T5ttttZg_LostMu_1800_150.root FastSim
./lostMuon runList_T5ttttZg_1800_1550.txt T5ttttZg_LostMu_1800_1550.root FastSim
