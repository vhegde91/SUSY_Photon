#define SkimmingFakeRate_cxx
#include "SkimmingFakeRate.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{

  if (argc < 2) {
    cerr << "Please give 3 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];

  SkimmingFakeRate ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingFakeRate::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  TTree *newtree = fChain->CloneTree(0);
  bool applyISRWeights=1,applyD=1;

  string s_data = data;
  cout<<"Appying ISR weights? "<<applyISRWeights<<endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" <<endl;
    decade = k;
    // cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // if(s_data== "TTJets_Tune") CrossSection = 815.96;//ttbar incl
    // else if(s_data== "TTJets_HT-600to800") CrossSection = 2.61537118;//TTJets_HT600to800
    // else if(s_data== "TTJets_HT-800to1200") CrossSection = 1.07722318;//TTJets_HT800to1200
    // else if(s_data== "TTJets_HT-1200to2500") CrossSection = 0.194972521;//TTJets_HT1200to2500
    // else if(s_data== "TTJets_HT-2500toInf") CrossSection = 0.0023234211;//TTJets_HT2500toInf

    //    Weight = CrossSection/NumEvents;

    h_MET_NoCut->Fill(MET,Weight*1000);
    h_HT_NoCut->Fill(HT,Weight*1000);
    h_NJets_NoCut->Fill(NJets,Weight*1000);
    h_nISRJets_NoCut->Fill(NJetsISR,Weight*1000);
    h_madHT_NoCut->Fill(madHT,Weight*1000);
    h_madHT_NoCut_vBin->Fill(madHT,Weight*1000);
    if(madHT > 600 && madHT < 800) h_nISRJets_madHT600To800->Fill(NJetsISR,Weight*1000);

    if(applyISRWeights){
      double isrWt = getISRWt();
      //      double isrWt = getISRWt()*1.0710;//for Incl sample only
      //      vector<double> D_values={1.,0.7838,0.7600,0.7365,0.7254};
      	   //     vector<double> D_values={1.06963, 1.2915, 1.3086, 1.2959, 2.0147};
      // if(madHT<600) isrWt = isrWt*D_values[0];
      // else if(madHT < 800) isrWt = isrWt*D_values[1];
      // else if(madHT < 1200) isrWt = isrWt*D_values[2];
      // else if(madHT < 2500) isrWt = isrWt*D_values[3];
      // else isrWt = isrWt*D_values[4];

      h_SumISRWts->Fill(1.0,isrWt*Weight*1000);
      h_MET_NoCut_ISRWtd->Fill(MET,isrWt*Weight*1000);
      h_HT_NoCut_ISRWtd->Fill(HT,isrWt*Weight*1000);
      h_NJets_NoCut_ISRWtd->Fill(NJets,isrWt*Weight*1000);
      h_nISRJets_NoCut_ISRWtd->Fill(NJetsISR,isrWt*Weight*1000);
      h_madHT_NoCut_ISRWtd->Fill(madHT,isrWt*Weight*1000);
      h_madHT_NoCut_ISRWtd_vBin->Fill(madHT,isrWt*Weight*1000);
      if(madHT > 600 && madHT < 800) h_nISRJets_madHT600To800_ISRWtd->Fill(NJetsISR,isrWt*Weight*1000);
    }
    if(applyISRWeights && applyD){
      double isrWt = 0.0, D_value=1.0;
      isrWt = getISRWt();
      if(s_data== "TTJets_Tune") D_value = 1.0697;//1.0710;
      else if(s_data== "TTJets_HT-600to800") D_value = 1.0150;//0.8394;
      else if(s_data== "TTJets_HT-800to1200") D_value = 0.9917;//0.8140;
      else if(s_data== "TTJets_HT-1200to2500") D_value = 0.9435;//0.7888;
      else if(s_data== "TTJets_HT-2500toInf") D_value = 0.95;//0.7769;      
    
      h_MET_NoCut_ISRWtd_D->Fill(MET,isrWt*D_value*Weight*1000);
      h_HT_NoCut_ISRWtd_D->Fill(HT,isrWt*D_value*Weight*1000);
      h_NJets_NoCut_ISRWtd_D->Fill(NJets,isrWt*D_value*Weight*1000);
      h_nISRJets_NoCut_ISRWtd_D->Fill(NJetsISR,isrWt*D_value*Weight*1000);
      h_madHT_NoCut_ISRWtd_D->Fill(madHT,isrWt*D_value*Weight*1000);
      h_madHT_NoCut_ISRWtd_D_vBin->Fill(madHT,isrWt*D_value*Weight*1000);
      if(madHT > 600 && madHT < 800) h_nISRJets_madHT600To800_ISRWtd_D->Fill(NJetsISR,isrWt*D_value*Weight*1000);
    }

    

    //    if(s_data=="CS_FR_madHT0to600" && madHT>600 ) continue;//only for TTJets_Tune*, TTJets_SingleLept*, TTJets_Dilept*
    //select skimming parameters (1 of 2)
    h_selectBaselineYields_->Fill(0);

    //    newtree->Fill();
 
  } // loop over entries
  //  newtree->AutoSave();
}

double SkimmingFakeRate::getISRWt(){
  vector<double> isrwt_arr={1., 0.920, 0.821, 0.715, 0.662, 0.561, 0.511};
  if(NJetsISR>=6) return isrwt_arr[6];
  else return isrwt_arr[NJetsISR];
}


TLorentzVector SkimmingFakeRate::getBestPhoton(){
  int bestPhoIndx=-100;
  TLorentzVector v1;
  vector<TLorentzVector> goodPho;
  for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
    if( ((*Photons_fullID)[iPhoton]) && ((*Photons_hasPixelSeed)[iPhoton]<0.001) ) goodPho.push_back( (*Photons)[iPhoton] );
  }
  
  if(goodPho.size()==0) return v1;
  else if(goodPho.size()==1) return goodPho[0];
  else{
    for(int i=0;i<goodPho.size();i++){
      if(i==0) bestPhoIndx=0;
      else if(goodPho[bestPhoIndx].Pt() < goodPho[i].Pt()) bestPhoIndx=i;
    }
    return goodPho[bestPhoIndx];
  }
}
