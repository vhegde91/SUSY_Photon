#!/bin/sh    
root -l -q 'findFailedJobs.C("Summer16.QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'