#!/bin/bash

for i in `seq 1 19`;
#for i in 1 18 3;
do  
#    ./run.txt $i -j4
#    root -l -q -b 'StackHists.C('$i')'
    root -l -q 'makeDatacard.C(1600,150,'$i')'
    root -l -q 'makeDatacard.C(1600,1000,'$i')'
    root -l -q 'makeDatacard.C(1600,1550,'$i')'
done
