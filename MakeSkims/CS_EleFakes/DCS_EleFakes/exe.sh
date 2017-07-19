#!/bin/sh    
for era in B C D E F G H
do
    echo "grep 'Run2016B.*03Feb2017.*SingleElectron.*.root' ~/inputFileList_main.txt > Run2016$era\_SingleElectron_03Feb2017.txt"
    grep 'Run2016'$era'.*03Feb2017.*SingleElectron.*.root' ~/inputFileList_main.txt > Run2016$era\_SingleElectron_03Feb2017.txt

done
    