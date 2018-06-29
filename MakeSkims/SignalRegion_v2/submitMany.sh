#!/bin/sh    
#root -l -q 'splitRunList.C("Summer16.GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
#root -l -q 'splitRunList.C("Summer16.GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",11)'
#root -l -q 'splitRunList.C("Summer16.GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
#root -l -q 'splitRunList.C("Summer16.GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",11)'

root -l -q 'splitRunList.C("spring16_gjets100to200.txt",10)'
root -l -q 'splitRunList.C("spring16_gjets200to400.txt",10)'
root -l -q 'splitRunList.C("spring16_gjets400to600.txt",10)'
root -l -q 'splitRunList.C("spring16_gjets600toInf.txt",10)'