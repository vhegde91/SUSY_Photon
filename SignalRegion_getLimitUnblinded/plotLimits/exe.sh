#!/bin/sh
for i in T5bbbbZg T5qqqqHg T5ttttZg T6ttZg
do
    echo "********************** "$i" ***********************"
    ./plotlimit runList_signif_$i\_SbinV7_Obs_v5.txt $i\_Signif_Obs_V7_v5.root a
    ./plotlimit runList_signif_$i\_SbinV7_Exp_v5.txt $i\_Signif_Exp_V7_v5.root a
done