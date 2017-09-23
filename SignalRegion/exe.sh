#!/bin/bash
for i in 150 1000 1550
do
    echo "root -l -q 'makeDatacard_Simple.C(1600,$i,\"T1bbbb_ZG_mGl1600_NLSP$i.root\")'"
    root -l -q 'makeDatacard_Simple.C(1600,'$i',"T1bbbb_ZG_mGl1600_NLSP'$i'.root")'
    echo "root -l -q 'makeDatacard_Simple.C(1600,$i,\"HG_NLSP$i.root\")'"
    root -l -q 'makeDatacard_Simple.C(1600,'$i',"HG_NLSP'$i'.root")'
done

