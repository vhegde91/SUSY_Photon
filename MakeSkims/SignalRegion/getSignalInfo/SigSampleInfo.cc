#define SigSampleInfo_cxx
#include "SigSampleInfo.h"
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

  SigSampleInfo ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SigSampleInfo::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  TTree *newtree = fChain->CloneTree(0);

  TString s_data = data;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" <<endl;
    decade = k;
    //cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //cout<<"SusyMotherMass:"<<SusyMotherMass<<" SusyLSPMass:"<<SusyLSPMass<<endl;

    if((SusyMotherMass < 0.00001 || SusyLSPMass < 0.00001) && !(s_data.Contains("TChi"))){
      cout<<"!!!Warning: SUSY Mom and LSP mass: "<<SusyMotherMass<<" , "<<SusyLSPMass<<endl;
      //      vector<int> moms={1000001,1000002,1000003,1000004,1000005,1000006,1000021,1000023,1000024};//in GGM 1000023 and 1000024 are moms
      int kid=1000022;
      vector<TLorentzVector> vMoms,vKids;
      vector<int> momIdx,kidIdx;
           
      for(int i=0;i<GenParticles->size();i++){
	if((*GenParticles)[i].Pt()!=0){    
	  if(abs((*GenParticles_PdgId)[i])>1000000 && (abs((*GenParticles_ParentId)[i])<=5 || abs((*GenParticles_ParentId)[i])==21)){
	    vMoms.push_back((*GenParticles)[i]);
	    momIdx.push_back(i);
	  }
	  if(abs((*GenParticles_PdgId)[i])==1000039) kidIdx.push_back((*GenParticles_ParentIdx)[i]);
	}
      }//loop over genParticles
      if(vMoms.size()!=2) cout<<"!!!!!!!!!!!!! expected 2 SUSY mothers. But found "<<vMoms.size()<<endl;
      if(kidIdx.size()!=2) cout<<"!!!!!!!!!!!!! expected 2 SUSY kids(NLSPs). But found "<<kidIdx.size()<<endl;
      else{
	vKids.push_back((*GenParticles)[kidIdx[0]]);
	vKids.push_back((*GenParticles)[kidIdx[1]]);
      }
      //      cout<<vMoms.size()<<" kids:"<<vKids.size()<<"     ";
      if(vMoms.size()!=2 || vKids.size()!=2 || (vMoms.size()==2 && (abs(vMoms[0].M()-vMoms[1].M()) > 3)) || (vKids.size()==2 && (abs(vKids[0].M()-vKids[1].M()) > 3)) ){
	for(int i=0;i<GenParticles->size();i++){
	  cout<<EvtNum<<" "<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<" E:"<<(*GenParticles)[i].Energy()<<" Mass:"<<(*GenParticles)[i].M()<<endl;
	}
      }
      if(vKids.size()!=2) cout<<"!!!!!!!!!!!!! expected 2 SUSY kids(NLSPs). But found "<<vKids.size()<<endl;
      if(vMoms.size()==2 && (abs(vMoms[0].M()-vMoms[1].M()) > 3) )cout<<"!!!!!!!!!!! Masses of Mom1 and Mom2 are not similar "<<vMoms[0].M()<<" "<<vMoms[1].M()<<endl;
      else if(SusyMotherMass < 0.00001) SusyMotherMass = vMoms[0].M();
      if(vKids.size()==2 && (abs(vKids[0].M()-vKids[1].M()) > 3) )cout<<"!!!!!!!!!!! Masses of Kid1 and Kid2 are not similar "<<vKids[0].M()<<" "<<vKids[1].M()<<endl;
      else if(SusyLSPMass < 0.000001) SusyLSPMass = vKids[0].M();
    }

    h_selectBaselineYields_->Fill(0);
    // h_met0->Fill(MET);
    // h_mht0->Fill(MHT);  
    // h_ht0->Fill(HT);
    // h_nj0->Fill(NJets);
    h2_MomKidMass->Fill(SusyMotherMass,SusyLSPMass);

    double ISRwt = 0;
    vector<double> isrwt_arr={1., 0.920, 0.821, 0.715, 0.662, 0.561, 0.511};
    if(NJetsISR>=6) ISRwt = isrwt_arr[6];
    else ISRwt = isrwt_arr[NJetsISR];

    h2_Nevts_NoISRWt->Fill(SusyMotherMass,SusyLSPMass,1.);
    h2_Nevts_ISRWt->Fill(SusyMotherMass,SusyLSPMass,1.0*ISRwt);

    h_selectBaselineYields_->Fill(1);
    
    //    newtree->Fill();
 
  } // loop over entries
  //  newtree->AutoSave();
}

TLorentzVector SigSampleInfo::getBestPhoton(){
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
