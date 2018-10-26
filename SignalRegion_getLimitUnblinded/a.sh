#!/bin/sh
echo "====== pho + b ====="
combine -M Asymptotic card_Pho_bJet_1800M3_200M1_diPhoVeto.txt
echo "====== pho + l ====="
combine -M Asymptotic M1M3_leppho/counting_GMSB_200_1800.txt
echo "====== pho + pho ====="
combine -M Asymptotic M1M3_dipho/counting_GGM_200_1800.txt

echo "==============================="
combineCards.py card_Pho_bJet_1800M3_200M1_diPhoVeto.txt M1M3_leppho/counting_GMSB_200_1800.txt M1M3_dipho/counting_GGM_200_1800.txt > a.txt
combine -M Asymptotic a.txt > temp.txt
#card_Pho_bJet_1800M3_200M1_diPhoVeto.txt M1M3_dipho/counting_GGM_200_1800.txt M1M3_leppho/counting_GMSB_200_1800.txt