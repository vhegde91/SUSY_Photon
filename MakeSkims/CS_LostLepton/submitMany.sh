#!/bin/sh    
root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",21)'
root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",20)'
root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",8)'
root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'

root -l -q 'splitRunList.C("Summer16.WGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph.txt",10)'
root -l -q 'splitRunList.C("Summer16.WGJets_MonoPhoton_PtG-40to130_TuneCUETP8M1_13TeV-madgraph.txt",10)'

root -l -q 'splitRunList.C("Summer16.TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt",10)'

root -l -q 'splitRunList_madHT0to600.C("Summer16.TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList_madHT0to600.C("Summer16.TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList_madHT0to600.C("Summer16.TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'

root -l -q 'splitRunList.C("Summer16.TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'

root -l -q 'splitRunList.C("Summer16.ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext1.txt",10)'

root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1.txt",10)'
root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1.txt",5)'
root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1.txt",5)'
root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",5)'
root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",5)'
root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
root -l -q 'splitRunList.C("Summer16.DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'

root -l -q 'splitRunList.C("Summer16.ST_s-ch_t-ch_tW_4f_5f_13TeV.txt",10)'
root -l -q 'splitRunList.C("Summer16.TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8.txt",5)'

#root -l -q 'splitRunList.C("Summer16.GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
#root -l -q 'splitRunList.C("Summer16.GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",11)'
#root -l -q 'splitRunList.C("Summer16.GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",11)'
#root -l -q 'splitRunList.C("Summer16.GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'

#root -l -q 'splitRunList.C("Summer16.QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",25)'
#root -l -q 'splitRunList.C("Summer16.QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",25)'
#root -l -q 'splitRunList.C("Summer16.QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",50)'
#root -l -q 'splitRunList.C("Summer16.QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",20)'
#root -l -q 'splitRunList.C("Summer16.QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",13)'
#root -l -q 'splitRunList.C("Summer16.QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",10)'
#root -l -q 'splitRunList.C("Summer16.QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",13)'