#!/bin/bash
for sample in 100to200 200to400 400to600 600to800 800to1200 1200to2500 2500toInf
do
    mv runList_MS_FR_DYJetsToLL_HT-$sample.root MS_FR_DYJetsToLL_HT$sample\_ST200_v2.root
done