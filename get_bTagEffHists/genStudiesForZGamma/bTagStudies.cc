#define bTagStudies_cxx
#include "bTagStudies.h"
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

  bTagStudies ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void bTagStudies::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0, evtSurvived = 0;
  int decade = 0;
  //  TTree *newtree = fChain->CloneTree(0);

  TString s_data=data;

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

    if(s_data.Contains("madHT0to600") && madHT > 600) continue;
    //    if(madHT>600) continue;
    
    int nTop = 0, nAntiTop = 0, topIndx=-100, antiTopIndx=-100, nLightLepsFromWZ = 0, nGenbs = 0, nTaus = 0;
    double wt =1000*Weight;
    bool isWdecayTob=false, hasFakeLep = false;

    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if((*GenParticles_PdgId)[i] ==  6){ nTop++; topIndx = i;}
	if((*GenParticles_PdgId)[i] == -6){ nAntiTop++; antiTopIndx = i;}
	if(abs((*GenParticles_PdgId)[i]) == 5) nGenbs++;
	if(abs((*GenParticles_PdgId)[i]) == 5 && (abs((*GenParticles_ParentId)[i]) == 24) ) isWdecayTob = true;
	if( (abs((*GenParticles_PdgId)[i]) == 11 || abs((*GenParticles_PdgId)[i]) == 13) && (abs((*GenParticles_ParentId)[i]) == 24) ) nLightLepsFromWZ++;
	if( (abs((*GenParticles_PdgId)[i]) == 15) && (abs((*GenParticles_ParentId)[i]) == 24) ) nTaus++;
      }
    }
    h_cutFlow->Fill(0);

    if(nTop!=1 || nAntiTop!=1) continue;
    h_cutFlow->Fill(1);
    
    if(nTaus!=0) continue;
    h_cutFlow->Fill(2);

    if(isWdecayTob) continue;
    h_cutFlow->Fill(3);

    int nrecoLep = Electrons->size()+Muons->size();
    vector<TLorentzVector> ele, mu;
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
        if( abs((*GenParticles_PdgId)[i]) == 11 && (abs((*GenParticles_ParentId)[i]) == 24) ) ele.push_back((*GenParticles)[i]);
        if( abs((*GenParticles_PdgId)[i]) == 13 && (abs((*GenParticles_ParentId)[i]) == 24) ) mu.push_back((*GenParticles)[i]);
      }
    }
    if(Electrons->size() > ele.size() || Muons->size() > mu.size() || Electrons->size() > 2 || Muons->size() > 2 || nrecoLep > 2) hasFakeLep = true;

    double mindR1=1000,mindR2=1000;
    if(Electrons->size()==1){
      for(int i=0;i<ele.size();i++){
	if(mindR1 > ele[i].DeltaR((*Electrons)[0])) mindR1 = ele[i].DeltaR((*Electrons)[0]);
      }
      if(mindR1 > 0.3) hasFakeLep = true;
    }
    if(Electrons->size()==2){
      for(int i=0;i<ele.size();i++){
	if(mindR1 > ele[i].DeltaR((*Electrons)[0])) mindR1 = ele[i].DeltaR((*Electrons)[0]);
	if(mindR2 > ele[i].DeltaR((*Electrons)[1])) mindR2 = ele[i].DeltaR((*Electrons)[1]);
      }
      if(mindR1 > 0.3 || mindR2 > 0.3) hasFakeLep = true;
    }    
    mindR1=1000,mindR2=1000;
    if(Muons->size()==1){
      for(int i=0;i<mu.size();i++){
	if(mindR1 > mu[i].DeltaR((*Muons)[0])) mindR1 = mu[i].DeltaR((*Muons)[0]);
      }
      if(mindR1 > 0.3) hasFakeLep = true;
    }
    if(Muons->size()==2){
      for(int i=0;i<mu.size();i++){
	if(mindR1 > mu[i].DeltaR((*Muons)[0])) mindR1 = mu[i].DeltaR((*Muons)[0]);
	if(mindR2 > mu[i].DeltaR((*Muons)[1])) mindR2 = mu[i].DeltaR((*Muons)[1]);
      }
      if(mindR1 > 0.3 || mindR2 > 0.3) hasFakeLep = true;
    }
    if(hasFakeLep) continue;
    h_cutFlow->Fill(4);
    // if(nGenbs%2!=0){
    //   for(int i=0;i<GenParticles->size();i++){
    // 	cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<" Px:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<" Pt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
    //   }
    // }
    if(nrecoLep==0){
      h_nGenbs_0l->Fill(nGenbs,wt);
      h_BTags_0l->Fill(BTags,wt);   
    }
    else if(nrecoLep==1){
      h_nGenbs_1l->Fill(nGenbs,wt);
      h_BTags_1l->Fill(BTags,wt);   
    }
    else if(nrecoLep==2){
      h_nGenbs_2l->Fill(nGenbs,wt);
      h_BTags_2l->Fill(BTags,wt);
    }

    h_MET->Fill(MET,wt);
    h_madHT->Fill(madHT,wt);
    evtSurvived++;


  } // loop over entries
  cout<<"Events survived: "<<evtSurvived<<endl;
  //  newtree->AutoSave();
}
