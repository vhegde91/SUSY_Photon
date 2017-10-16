#!/bin/sh    
for era in B C D E F G H
#for era in B
do
#    echo "grep 'Run2016B.*03Feb2017.*SingleElectron.*.root' ~/inputFileList_main.txt > Run2016$era\_SingleElectron_03Feb2017.txt"
#    grep 'Run2016'$era'.*03Feb2017.*SingleElectron.*.root' ~/inputFileList_main.txt > Run2016$era\_SingleElectron_03Feb2017.txt
#    echo "hadd /eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$era\_SingleEGamma_03Feb2017.root /eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$era\_SinglePhoton_03Feb2017.root /eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$era\_SingleElectron_03Feb2017.root"
#    hadd /eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$era\_SingleEGamma_03Feb2017.root /eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$era\_SinglePhoton_03Feb2017.root /eos/uscms/store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$era\_SingleElectron_03Feb2017.root
    echo "root -l -q 'removeOverlap2.C(\"root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$era\_SingleEGamma_03Feb2017.root\")'"
    root -l -q 'removeOverlap2.C("root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016'$era'_SingleEGamma_03Feb2017.root")'
    mv newTree2.root NoOverlap_Run2016$era\_SingleEGamma_03Feb2017.root
    xrdcp -f NoOverlap_Run2016$era\_SingleEGamma_03Feb2017.root root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/
    rm NoOverlap_Run2016$era\_SingleEGamma_03Feb2017.root

done
    