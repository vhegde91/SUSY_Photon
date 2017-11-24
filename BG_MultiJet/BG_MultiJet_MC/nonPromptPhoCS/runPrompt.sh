#!/bin/bash
./multijet VS_runListQCD.txt QCD_prompt.root QCD
./multijet VS_runListGJets.txt GJets_prompt.root GJets

./multijet VS_runListZJetsToNuNu.txt ZJetsToNuNu_prompt.root ZJets
./multijet VS_runListZGToNuNuG.txt ZGJetsToNuNuG_prompt.root ZG

./multijet VS_runListWJetsToLNu.txt WJetsToLNu_prompt.root WJets
./multijet VS_runListWG.txt WGJetsToLNuG_prompt.root WG

./multijet VS_runListTTJets.txt TTJets_prompt.root TTJets
./multijet VS_runListTTG.txt TTGJets_prompt.root TTG

./multijet VS_runListT5bbbbZg_1600_150.txt T5bbbbZg_1600_150_prompt.root FastSim
./multijet VS_runListT5bbbbZg_1600_1550.txt T5bbbbZg_1600_1550_prompt.root FastSim

./multijet SR_runListT5bbbbZg_1800_150.txt T5bbbbZg_1800_150_prompt.root FastSim
./multijet SR_runListT5bbbbZg_1800_1750.txt T5bbbbZg_1800_1750_prompt.root FastSim

hadd -f GJetsQCD_prompt.root QCD_prompt.root GJets_prompt.root
hadd -f EW_ZWTT_prompt.root ZJetsToNuNu_prompt.root ZGJetsToNuNuG_prompt.root WJetsToLNu_prompt.root WGJetsToLNuG_prompt.root TTJets_prompt.root TTGJets_prompt.root
