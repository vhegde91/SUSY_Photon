#!/bin/sh
for sample in B C D E F G H
do
#    echo "grep $sample inputFiles.txt_main > $sample.txt"
#    grep $sample inputFiles.txt_main > $sample.txt
    echo "$sample "
    grep 'Run2016'$sample'-03Feb2017.*SingleMuon' ~/inputFileList_main.txt > Run2016$sample\_SingleMuon_03Feb2017.txt
done