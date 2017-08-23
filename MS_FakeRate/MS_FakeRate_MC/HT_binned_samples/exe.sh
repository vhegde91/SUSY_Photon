#!/bin/bash                                                                                                                                   
for sample in 100to200 200to400 400to600 600to800 800to1200 1200to2500 2500toInf
do
    root -l -q -b 'GetFakeRate.C("MS_FR_DYJetsToLL_HT'$sample'_ST200_v2.root")'
    root -l -q 'printHistInfo.C("FR_Hist_MS_FR_DYJetsToLL_HT'$sample'_ST200_v2.root","FakeRate_QMultJetElef_Pho")'
done
