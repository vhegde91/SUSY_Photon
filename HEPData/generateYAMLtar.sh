#!/bin/bash
root -l -q histToYaml.C
python limitPlotToYaml.py config.cfg
echo "Not generating cov/corr matrix yaml files"
#python limitPlotToYaml.py config_cov.cfg
#python limitPlotToYaml.py config_corr.cfg
./latexTableToyaml.sh > CutflowTable.yaml
root -l -q 'mySubmission_gen.C("fileNames.txt")'
tar cf submitYAML.tar submission.yaml allBGpredObs.yaml T*.yaml Matrix_Co*.yaml CutflowTable.yaml
gzip submitYAML.tar 
