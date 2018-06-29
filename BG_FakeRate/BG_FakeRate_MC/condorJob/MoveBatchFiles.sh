#!/bin/sh    
name="clsr_v3"
hadd -f a.root CS_runList_TTJets_FR.root CS_runList_WJets_FR.root
mv CS_runList_TTJets_FR.root ../CS_TTJets_FR_ISRWt_$name.root
mv CS_runList_WJets_FR.root ../CS_WJets_FR_$name.root
mv a.root ../CS_TTW_FR_ISRWt_$name.root