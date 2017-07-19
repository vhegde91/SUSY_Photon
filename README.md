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
