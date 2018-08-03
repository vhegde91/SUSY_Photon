#!/bin/sh
input_Scan=$1
anaExe="pickMassPoint"
anaArg="FastSim_GGM_M1M3"
exeAtWorker="worker_sortSMS.sh"
fileListName="Spring16Fast.GGM_GravitinoLSP_M1-50to1500_M3-1000to2500_TuneCUETP8M1_13TeV_pythia8.txt"
#filesToTransfer="GJets.root,QCD.root,TTGJets.root,TTJetsHT.root,WGJetsToLNuG.root,WJetsToLNu.root,ZGJetsToNuNuG.root,ZJetsToNuNu.root,makeDatacard_SBins.C,${anaExe},combineCards.py,higgsCombine.tar"
filesToTransfer="${anaExe},GGM_M1M3_MassScan.root,T5bbbbZg_MassScan.root,T5qqqqHg_MassScan.root,T5ttttZg_MassScan.root,T6ttZg_MassScan.root,${fileListName}"

while read -a massP
do 
    echo ${massP[0]} ${massP[1]}
    jdl_file="condor_${anaArg}_${massP[0]}_${massP[1]}_job.jdl"
    log_prefix="condor_${anaArg}_${massP[0]}_${massP[1]}_job"
    echo "universe = vanilla">$jdl_file
    echo "Executable = $exeAtWorker">>$jdl_file
    echo "Should_Transfer_Files = YES">>$jdl_file
    echo "WhenToTransferOutput = ON_EXIT_OR_EVICT">>$jdl_file
    echo "Transfer_Input_Files = ${filesToTransfer}">>$jdl_file
    echo "Output = ${log_prefix}.stdout">>$jdl_file
    echo "Error = ${log_prefix}.stderr">>$jdl_file
    echo "Log = ${log_prefix}.condor">>$jdl_file
    echo "notification = never">>$jdl_file
    echo "Arguments = ${anaExe} ${fileListName} ${anaArg}_${massP[0]}_${massP[1]}.root ${anaArg} ${massP[0]} ${massP[1]}">>$jdl_file
    echo "Queue">>$jdl_file
    condor_submit $jdl_file
done < $input_Scan

