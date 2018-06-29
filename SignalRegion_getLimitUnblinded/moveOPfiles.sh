#!/bin/sh    
dir=$1
model=$2
echo "Limits_Signif_${model}_SbinV7_$dir"
if [ ! -d "Limits_Signif_${model}_SbinV7_$dir" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
    echo "making directory Limits_Signif_${model}_SbinV7_$dir"
    mkdir Limits_Signif_${model}_SbinV7_$dir
    mv higgsCombine$model\_*.root Limits_Signif_$model\_SbinV7_$dir
    mv higgsCombine*Signif_$model\_*.root Limits_Signif_$model\_SbinV7_$dir
    ls -l Limits_Signif_$model\_SbinV7_$dir/*.root |wc -l
fi
