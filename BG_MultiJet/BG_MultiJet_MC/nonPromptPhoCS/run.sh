#!/bin/bash
./multijet runListQCD_v2.txt QCD_idFail.root QCD
./multijet runListGJets_v2.txt GJets_idFail.root GJets
hadd -f GJetsQCD_idFail.root QCD_idFail.root GJets_idFail.root
