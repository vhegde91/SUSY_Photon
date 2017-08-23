#!/bin/bash
for i in B C D E F G H
do
    echo $i
    hadd /media/vinay/Physics/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$i\_SingleEGamma_03Feb2017.root /media/vinay/Physics/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$i\_SingleElectron_03Feb2017.root  /media/vinay/Physics/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/DCS_EleFakes/Step1_Run2016$i\_SinglePhoton_03Feb2017.root
done
