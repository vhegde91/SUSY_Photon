# HEPdataMaker

Converts root files into HEP data compatible YAML files.

## SUSY limit plots
For converting SUSY style exclusion plots into YAML files, use [limitPlotToYaml.py](./limitPlotToYaml.py) This script is taken from https://github.com/kkiesel/root-to-yaml

Install python-yaml package (only package you need, no need for hepdatalib) using

```
 sudo apt-get install python-yaml
```

An example root file (Excl_T5qqqqHg_LimitPlots_Obs_V7_v11.root)  is provided to to make YAML files from SUSY exclusion plots.

First you need to edit [config.cfg](./config.cfg) file according to your need. In this example, remove all sections except [T5qqqqHG], and change the path to file name.

Run the example:

```
python limitPlotToYaml.py config.cfg
```

## TH1 Histograms to YAML

Using [histToYaml.C](./histToYaml.C) you can convert some of the TH1D histograms in SBinHists.root to YAML a file.

```
root -l -q histToYaml.C
```

## Latex table to YAML

Use [latexTableToyaml.sh](./latexTableToyaml.sh) for converting a LateX table to YAML file. Only simple tables (no multi-row or multi-column) can be handled. Assumes first row in [cutFlowTable.txt](./cutFlowTable.txt) is table header and first column is independent variable. IMP: do not give Latex line breaks (\\ given at the end of a row in Latex).

```
./latexTableToyaml.sh > CutflowTable.yaml
```

## Making submission.yaml file

Once all the individual yaml files are created, list them in [fileNames.txt](./fileNames.txt) and you need to create a [submission.yaml](./submission.yaml) file. This is done by

```
root -l -q 'mySubmission_gen.C("fileNames.txt")'
```

## Automated generator

```
./generateYAMLtar.sh
```

## References

https://github.com/kkiesel/root-to-yaml

https://github.com/hatakeyamak/misc/tree/master/HEPData

Check your YAML file online: http://www.yamllint.com before uploading to HEPdata sandbox.

