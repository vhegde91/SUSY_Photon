#!/bin/sh
#root -l -q 'findFailedJobs.C("Summer16.GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
#root -l -q 'findFailedJobs.C("Summer16.GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
#root -l -q 'findFailedJobs.C("Summer16.GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
#root -l -q 'findFailedJobs.C("Summer16.GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'

root -l -q 'findFailedJobs.C("Summer16.GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'
root -l -q 'findFailedJobs.C("Summer16.GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8")'