#define SkimmingGJets_cxx
#include "SkimmingGJets.h"
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

  SkimmingGJets ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingGJets::EventLoop(const char *data,const char *inputFileList) {
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
    // double progress = 10.0 * jentry / (1.0 * nentries);
    // int k = int (progress);
    // if (k > decade)
    //   cout << 10 * k << " %" <<endl;
    // decade = k;
    // cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    h_selectBaselineYields_->Fill(0);
    if( Electrons->size() == 0 && Muons->size()==0 )   h_selectBaselineYields_->Fill(1);
    else continue;//veto leptons

    //----------------------------------skiming parematers for data only------------------------------
    // bool passTrg=false;
    // for(int i=0;i<TriggerNames->size();i++){
    //   string trgName=(*TriggerNames)[i];
    //   trgName.pop_back();
    //   // if( trgName=="HLT_PFHT350_v" && (*TriggerPass)[i]==1 ) {passTrg = true;break;}          //JetHT
    //   // else if( trgName=="HLT_PFHT400_v" && (*TriggerPass)[i]==1 ) {passTrg = true;break;}     //JetHT
    //   // else if( trgName=="HLT_PFHT475_v" && (*TriggerPass)[i]==1 ) {passTrg = true;break;}     //JetHT
    //   // else if( trgName=="HLT_PFHT600_v" && (*TriggerPass)[i]==1 ) {passTrg = true;break;}     //JetHT
    //   if( trgName=="HLT_Photon165_HE10_v" && (*TriggerPass)[i]==1 ) {passTrg = true;break;}       //SinglePhoton
    //   else if( trgName=="HLT_Photon90_CaloIdL_PFHT600_v" && (*TriggerPass)[i]==1 ) {passTrg = true;break;}    //SinglePhoton
    // }
    // if(passTrg) h_selectBaselineYields_->Fill(7);
    // else continue;
    //about photons
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt()>=100) continue;              // veto photons for JetHT dataset.
    //    if(bestPhoton.Pt()<100) continue;        //requires a photon for Signgle photon dataset.
    h_selectBaselineYields_->Fill(2);
    //----------------------------------------------------------------
    double nHadJets = NJets, ST = HT;
    //select skimming parameters
    if( nHadJets >= 2 )  h_selectBaselineYields_->Fill(3);
    else continue;
    if( ST>950.)        h_selectBaselineYields_->Fill(4);
    else continue;
    if( MET>100 )        h_selectBaselineYields_->Fill(5);
    else continue;
    //end of select skimming parameters
    //
    newtree->Fill();
 
  } // loop over entries
  //  newtree->AutoSave();
}

TLorentzVector SkimmingGJets::getBestPhoton(){
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
