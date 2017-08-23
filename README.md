There are different directories corresponding to a perticular process/BG estimation. These directories contaon analysis code which is written in <fileName>.cc file. 

## Description about directories
BG*: For estimation of a perticular background.

DBG*: This usually contains CS and prediction for that perticular BG.

MS*: Measurement sample. In this analysis, we measure fakerate(e->gamma) using tag & probe and samples/code related to these are here.

DMS*: Measurement sample in data.

SignalRegion: MC files for SR

-----------------------------------------------

BG_FakeRate and MS_FakeRate directories process histogram files which are obtained after processing trees. These trees are large and hence they are processed using LPC condor. Code for processing trees is here:

https://github.com/vhegde91/SUSY_Photon/tree/master/BG_FakeRate/BG_FakeRate_MC

https://github.com/vhegde91/SUSY_Photon/tree/master/MR_FakeRate

All other directories contain code which run on trees which are located in local desktop. These trees are small and hence they can be processed on desktop.