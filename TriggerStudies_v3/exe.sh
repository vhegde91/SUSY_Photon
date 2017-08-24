#!/bin/sh    
for i in B C D E F G H
do
    echo $i
    mv Run2016$i\_MET_03Feb2017.root Run2016$i\_MET_03Feb2017_Ele27_OR_Pho165_1TightEleOnly.root
done