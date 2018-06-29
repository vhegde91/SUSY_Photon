#!/bin/sh    
#root -l -q 'splitRunList.C("Summer16.DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa.txt",11)'
#root -l -q 'splitRunList.C("Summer16.DiPhotonJetsBox_M40_80-Sherpa.txt",20)'

root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.txt",20)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.txt",20)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-800To1200_13TeV-madgraph.txt",20)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-600To800_13TeV-madgraph.txt",20)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt",20)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt",20)'
root -l -q 'splitRunList.C("Summer16.ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt",20)'

#root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1.txt",10)'
#root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1.txt",5)'
#root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1.txt",5)'
#root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",5)'
#root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",5)'
#root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
#root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'

#root -l -q 'splitRunList.C("Summer16.ZGTo2NuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.txt",5)'
#root -l -q 'splitRunList.C("Summer16.ZGTo2NuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.txt",5)'
#root -l -q 'splitRunList.C("Summer16.ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext1.txt",5)'
#root -l -q 'splitRunList.C("Summer16.ZGTo2LG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.txt",5)'

root -l -q 'splitRunList.C("Summer16.ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph.txt",5)'
root -l -q 'splitRunList.C("Summer16.ZNuNuGJets_MonoPhoton_PtG-40to130_TuneCUETP8M1_13TeV-madgraph.txt",5)'
root -l -q 'splitRunList.C("Summer16.ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph.txt",1)'