#define SkimmingTrig_cxx
#include "SkimmingTrig.h"
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

  SkimmingTrig ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingTrig::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  TTree *newtree = fChain->CloneTree(0);

  string s_data=data;

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

    h_selectBaselineYields_->Fill(0);
    
    // TLorentzVector bestPhoton = getBestPhoton();
    // if(bestPhotonIndxAmongPhotons<0) continue;
    // if( (*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons] > 0.001 ) continue;

    //    bool passTrg = false, passMETTrg = false;
    bool passTrg=false,passHTTrg=false,passPhoPtTrg=false,passHTPhoPtTrg=false;

    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back();
      
      // if( trgName=="HLT_PFMET90_PFMHT90_IDTight_v" && (*TriggerPass)[i]==1 )        { h_selectBaselineYields_->Fill(1); passTrg = true; passMETTrg = true;}
      // else if( trgName=="HLT_PFMET100_PFMHT100_IDTight_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(2); passTrg = true; passMETTrg = true;}
      // else if( trgName=="HLT_PFMET110_PFMHT110_IDTight_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(3); passTrg = true; passMETTrg = true;}
      // else if( trgName=="HLT_PFMET120_PFMHT120_IDTight_v" && (*TriggerPass)[i]==1 ) { h_selectBaselineYields_->Fill(4); passTrg = true; passMETTrg = true;}

      // else if( trgName=="HLT_Photon90_CaloIdL_PFHT600_v" && (*TriggerPass)[i]==1 )  { h_selectBaselineYields_->Fill(5); passTrg = true;}
      // else if( trgName=="HLT_Photon165_HE10_v" && (*TriggerPass)[i]==1 )            { h_selectBaselineYields_->Fill(6); passTrg = true;}

      // else if( trgName=="HLT_Ele15_IsoVVVL_PFHT350_v" && (*TriggerPass)[i]==1 )     { h_selectBaselineYields_->Fill(7); passTrg = true;}
      // else if( trgName=="HLT_Ele27_WPTight_Gsf_v" && (*TriggerPass)[i]==1 )         { h_selectBaselineYields_->Fill(8); passTrg = true;}
      // else if( trgName=="HLT_IsoMu24_v" && (*TriggerPass)[i]==1 )                   { h_selectBaselineYields_->Fill(9); passTrg = true;}
      // else if( trgName=="HLT_Mu15_IsoVVVL_PFHT350_v" && (*TriggerPass)[i]==1 )        { h_selectBaselineYields_->Fill(10); passTrg = true;}
      if( trgName=="HLT_PFHT300_v" && (*TriggerPass)[i]==1 )                  { h_selectBaselineYields_->Fill(1); passTrg = true;}
      else if( trgName=="HLT_PFHT350_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(2); passTrg = true;}
      else if( trgName=="HLT_PFHT400_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(3); passTrg = true;}
      else if( trgName=="HLT_PFHT475_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(4); passTrg = true;}
      else if( trgName=="HLT_PFHT600_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(5); passTrg = true;}
      else if( trgName=="HLT_PFHT650_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(6); passTrg = true;}
      else if( trgName=="HLT_PFHT800_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(7); passTrg = true;}
      else if( trgName=="HLT_PFHT900_v" && (*TriggerPass)[i]==1 )             { h_selectBaselineYields_->Fill(8); passTrg = true;}
      else if( trgName=="HLT_Photon165_HE10_v" && (*TriggerPass)[i]==1 )      { h_selectBaselineYields_->Fill(9); passTrg = true;}
      if(passTrg) break;
      //cout<<(*TriggerNames)[i]<<" "<<(*TriggerPass)[i]<<" "<<(*TriggerPrescales)[i]<<endl;
    }
    if(!passTrg) continue;

    newtree->Fill();
    
  } // loop over entries
  //  newtree->AutoSave();
}

TLorentzVector SkimmingTrig::getBestPhoton(){
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( (*Photons_fullID)[iPho] ) {
      goodPho.push_back( (*Photons)[iPho] );
      goodPhoIndx.push_back(iPho);
    }
  }
  
  int highPtIndx=-100;
  for(int i=0;i<goodPho.size();i++){
    if(i==0) highPtIndx=0;
    else if( (goodPho[highPtIndx].Pt()) < (goodPho[i].Pt()) ){highPtIndx=i;}
  }
  
  if(highPtIndx>=0){
    bestPhotonIndxAmongPhotons = goodPhoIndx[highPtIndx];
  }
  else bestPhotonIndxAmongPhotons = -100;
  if(highPtIndx==-100){TLorentzVector v0;return v0;}
  else return goodPho[highPtIndx];
}


/*
  int bestPhoIndx=-100;
  TLorentzVector v1;
  vector<TLorentzVector> goodPho;
  for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
    if( (*Photons_fullID)[iPhoton] ) goodPho.push_back( (*Photons)[iPhoton] );
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
*/
