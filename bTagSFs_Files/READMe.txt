Copy all the neccessary files from here to the required location.
/home/vinay/work/Physics/skims_Run2ProductionV12/bTagSFs_Files/

In Ana.cc file:
//open skim file
BTagCorrector btagcorr;
btagcorr.SetEffs(file);
btagcorr.SetCalib("btag/CSVv2_ichep.csv");
//if fastsim
btagcorr.SetFastSim(true);
btagcorr.SetCalibFastSim("btag/CSV_13TEV_Combined_14_7_2016.csv");
//inside event loop
vector<double> prob = btagcorr.GetCorrections(Jets,Jets_hadronFlavor,HTJetsMask);
//put event in each btag bin, weighted by prob[0], prob[1], prob[2], prob[3] for nb = 0, 1, 2, 3+
//instead, if cutting on nb, use method 1a in event loop to get event weight
//double corr = btagcorr.GetSimpleCorrection(Jets,Jets_hadronFlavor,HTJetsMask,Jets_bDiscriminatorCSV);
