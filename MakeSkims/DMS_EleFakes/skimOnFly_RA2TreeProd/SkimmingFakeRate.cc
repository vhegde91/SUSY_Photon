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

  string s_data = data;
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

    //select skimming parameters (1 of 2)
    h_selectBaselineYields_->Fill(0);

    if( Electrons->size()==0 ) continue;
    h_selectBaselineYields_->Fill(1);

    bool passReqTrg=false;
    for(int i=0;i<TriggerNames->size();i++){
      string trgName=(*TriggerNames)[i];
      trgName.pop_back();
      trgName.pop_back();
      if( ((trgName=="HLT_Ele27_WPTight_Gsf_v") || (trgName=="HLT_Ele27_WPTight_Gsf_")) && (*TriggerPass)[i]==1 )
	passReqTrg = true;
    }
    if(!passReqTrg) continue;
    h_selectBaselineYields_->Fill(2);

    TLorentzVector bestPhoton=getBestPhoton();
    bool hasHighPtEM = false;
    if(bestPhoton.Pt() > 100.){
      hasHighPtEM = true;
    }
    else{
      for(int i=0;i<Electrons->size();i++){
	if((*Electrons)[i].Pt() > 100.0){ hasHighPtEM = true; break;}
      }
    }
    if(Muons->size()==1) hasHighPtEM = true;
    if(hasHighPtEM) h_selectBaselineYields_->Fill(3);
    else continue;
    // if( (Muons->size()!=0) || (Electrons->size()==0) || (Electrons->size()>2) ) continue;
    // else h_selectBaselineYields_->Fill(1);

    //-----------------------------------------------------------------------

    //select skimming parameters (2 of 2)
    // if( nHadJets >= 2 )  h_selectBaselineYields_->Fill(3);
    // else continue;
    // if( ST>500. )        h_selectBaselineYields_->Fill(4);
    // else continue;
    // if( MET<100. )       h_selectBaselineYields_->Fill(5);
    // else continue;
    //end of select skimming parameters

    newtree->Fill();
 
  } // loop over entries
  //  newtree->AutoSave();
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
