Important files in this directory are: SkimmingFakeRate.cc, SkimmingFakeRate.h, NtupleVariables.cc, NtupleVariables.h and Makefile.

Compile code: make clean; make

Use the executable(skimmingFakeRate) for skimming the tree obtained from MINIAOD:

1. Make RA2 tree using TreeMaker recipie. 

2. Before transfering the o/p tree to eos area, run the skimming code and delete un-skimmed root file. Add the following lines in TreeMaker/Production/test/condorSub/jobExecCondor.sh before " # copy output to eos "
```
for rawTree in *.root
do
    echo $rawTree > inFile4Skimming.txt
    echo "Skimming " $rawTree
    echo "./skimmingFakeRate inFile4Skimming.txt ${rawTree} Skimmed"
    ./skimmingFakeRate inFile4Skimming.txt $rawTree Skimmed
    rm $rawTree
    rm inFile4Skimming.txt
    echo "DONEEE Skimming"
done
```

Other files are example files for condor job submissions. These files are not used. These files will be useful if we have non-skimmed RA2 trees with trigger objects.