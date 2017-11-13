#!/bin/sh
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt",10)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt",10)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt",10)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-600To800_13TeV-madgraph.txt",10)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-800To1200_13TeV-madgraph.txt",10)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.txt",11)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.txt",12)'

root -l -q 'splitRunList.C("Summer16.TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt",10)'
