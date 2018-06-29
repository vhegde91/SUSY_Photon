#!/bin/sh    
for i in B C D E F G H
do
    echo $i
    mv Run2016$i\_MET_03Feb2017.root Run2016$i\_MET_03Feb2017_HT800orHT900.root
done