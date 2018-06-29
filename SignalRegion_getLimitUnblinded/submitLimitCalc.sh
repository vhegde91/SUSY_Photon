#!/bin/sh
for i in T5bbbbZg T5qqqqHg T5ttttZg T6ttZg
do
    echo "********************** "$i" ***********************"
    ./calcLimit_$i.sh $i\_MassScan_subset.txt
done