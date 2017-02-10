#!/bin/sh

#xrdcp $datasetName'_'$outputFileTag root://cmseos.fnal.gov//store/user/vhegde/temp1/
for i in $( ls | grep .root ) ; do 
    xrdcp $i root://cmseos.fnal.gov//store/user/vhegde/GMSB_skims_RA2b_TreesV9
done