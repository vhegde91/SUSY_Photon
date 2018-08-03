#!/bin/sh
verNum="v3"
#ls ../Limits_Signif_GGM_M1M3_SbinV7_$verNum/higgsCombineGGM*Asymptotic*.root > runList_GGM_M1M3_SbinV7_Obs_$verNum.txt
#ls ../Limits_Signif_GGM_M1M3_SbinV7_$verNum/higgsCombineExpSignif_GGM*.root > runList_signif_GGM_M1M3_SbinV7_Exp_$verNum.txt
#ls ../Limits_Signif_GGM_M1M3_SbinV7_$verNum/higgsCombineObsSignif_GGM*.root > runList_signif_GGM_M1M3_SbinV7_Obs_$verNum.txt
#ls ../Limits_Signif_GGM_M1M3_SbinV7_$verNum/higgsCombinePhoLep_GGM*.root > runList_GGM_PhoLep_M1M3_SbinV7_Obs_$verNum.txt
#ls ../Limits_Signif_GGM_M1M3_SbinV7_$verNum/higgsCombinePhoLep_PhoJet_GGM*.root > runList_GGM_PhoLepPhoJet_M1M3_SbinV7_Obs_$verNum.txt

./plotlimit runList_GGM_M1M3_SbinV7_Obs_$verNum.txt GGM_M1M3_LimitPlots_Obs_V7_$verNum.root GGM_M1M3
./plotlimit runList_signif_GGM_M1M3_SbinV7_Exp_$verNum.txt GGM_M1M3_Signif_Exp_V7_$verNum.root GGM_M1M3
./plotlimit runList_signif_GGM_M1M3_SbinV7_Obs_$verNum.txt GGM_M1M3_Signif_Obs_V7_$verNum.root GGM_M1M3
./plotlimit runList_GGM_PhoLep_M1M3_SbinV7_Obs_$verNum.txt GGM_M1M3_PhoLep_LimitPlots_Obs_V7_$verNum.root GGM_M1M3
./plotlimit runList_GGM_PhoLepPhoJet_M1M3_SbinV7_Obs_$verNum.txt GGM_M1M3_PhoLepPhoJet_LimitPlots_Obs_V7_$verNum.root GGM_M1M3
