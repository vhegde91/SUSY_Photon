#!/bin/sh    
root -l -q 'splitRunList.C("Summer16.QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",25)'
root -l -q 'splitRunList.C("Summer16.QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",25)'
root -l -q 'splitRunList.C("Summer16.QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",50)'
root -l -q 'splitRunList.C("Summer16.QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",20)'
root -l -q 'splitRunList.C("Summer16.QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",13)'
root -l -q 'splitRunList.C("Summer16.QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",13)'
 
root -l -q 'splitRunList.C("Summer16.GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",11)'
root -l -q 'splitRunList.C("Summer16.GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",11)'
