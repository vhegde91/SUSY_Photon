# SUSY search with photon, MET, Jets and b-jets

## Instructions
Setup CMSSW. Most of the code uses root only. So CMSSW is needed for using root. You can also use previously setup CMSSW(preferably CMMSW_8_0_25)
```
cmsrel CMSSW_8_0_25
cd CMSSW_8_0_25/src/
cmsenv

```
## Description about directories
### MakeSkims
Make skimmed trees from V12 RA2b trees. RA2b trees are located here:
```
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12/
root://cmseos.fnal.gov//store/user/vhegde/myProduction_V12/
```
About skimming in general:
Skimming parametrs are specified in XXX.cc file. Variable and functions used in XXX.cc file are defined in XXX.h file. In addition to these, two more files are needed: NtupleVariables.h and NtupleVariables.cc.
Code needs to be compiled using Makefile. This gives an executable xxx.
To make skim:
$ ./xxx inFile.txt outFile.root addInfo

inFile.txt: is a text fil containg list of root files on which skimming is done.
outFile.root: Ouptut skimmed tree. This name will be addInfo_outFile.root
addInfo: additional info about type of skimimg. For ex, LostMu for skiming lost muon studies. 