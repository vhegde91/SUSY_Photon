#!/bin/bash
./multijet VS_runListQCD.txt QCD_idFail.root QCD
./multijet VS_runListGJets.txt GJets_idFail.root GJets
hadd -f GJetsQCD_idFail.root QCD_idFail.root GJets_idFail.root

./multijet VS_runListZJetsToNuNu.txt ZJetsToNuNu_idFail.root ZJets
./multijet VS_runListZGToNuNuG.txt ZGJetsToNuNuG_idFail.root ZG

./multijet VS_runListWJetsToLNu.txt WJetsToLNu_idFail.root WJets
./multijet VS_runListWG.txt WGJetsToLNuG_idFail.root WG

./multijet VS_runListTTJets.txt TTJets_idFail.root TTJets
./multijet VS_runListTTG.txt TTGJets_idFail.root TTG

./multijet VS_runListT5bbbbZg_1600_150.txt T5bbbbZg_1600_150_idFail.root FastSim
./multijet VS_runListT5bbbbZg_1600_1550.txt T5bbbbZg_1600_1550_idFail.root FastSim


hadd -f EW_ZWTT_idFail.root ZJetsToNuNu_idFail.root ZGJetsToNuNuG_idFail.root WJetsToLNu_idFail.root WGJetsToLNuG_idFail.root TTJets_idFail.root TTGJets_idFail.root
