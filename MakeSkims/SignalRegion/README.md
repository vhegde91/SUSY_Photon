SkimmingSR.cc can skim MC BG. It can skim signal for a given mGluino and mNLSP.

For skimming signal dataset and sort out mass points:

1) getSignalInfo/

This derectory is for getting a histogram file which contains different mass points for a dataset and the number of events for each of themass points.
SigSampleInfo.cc: main code which fills mass scan histogram

2) SignalSkimming/

mGl_Xsecpb_pcUnc_absUnc.txt : Contains gluino masses, xec in pb, percent uncertainty and absolute unc on xsec in pb.

makeGlMassVsXsecHist.C : make a root file containing a histogram of xecs. X-axis: gluino mass, Y-axis: xsec in pb and ->GetBinError(i) gives abs unc on xsec.

splitRunList.C : root -l -q 'splitRunList.C("unskimmed_RA2TreeFileList.txt",1)'  
#give only one file per job. For each file listed in unskimmed_RA2TreeFileList.txt, creates a jdl file.

processSigFile.C : At the worker node, this macro is executed. This takes .txt file(containing RA2Tree file name) as input and creates a job for each of the mass points present in this RA2 tree.

SkimmingSR.cc : This code is for skimming the RA2 tree for a given mass point. The executable obtained from this needs 5 arguements: fileList.txt, outFile.root, datasetName, mGl, mNLSP

3)  SignalSkimming/Skims_hadd/  : To hadd skimmed files based on mass points.

makeJDL.C : make jdl file for each mass point and submit a job for each.

worker_hadd.sh : Takes mGl and mNLSP as arguements and adds all files of eos *mGl*mNLSP*job*.root files. xrdcp to added file.

numEventsComp.C : checks final skimmed file and removes haded files from eos. rm *mGl*mNLSP*job*.root from eos. It can also run interactively w/o any arguemts and checks if final skimmed files are ok.