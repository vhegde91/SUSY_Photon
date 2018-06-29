#!/bin/sh
input_Scan=$1
dataSetType="T5qqqqHg"

while read -a massP
do 
    echo ${massP[0]} ${massP[1]}
    hadd -f 'FastSim_SystA_'$dataSetType'_'${massP[0]}'_'${massP[1]}'.root' '*SMS*'$dataSetType'*pythia8_job*'${massP[0]}'_'${massP[1]}'.root'
done < $input_Scan

