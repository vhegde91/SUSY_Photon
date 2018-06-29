#define SkimmingZToLL_cxx
#include "SkimmingZToLL.h"
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

  SkimmingZToLL ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingZToLL::EventLoop(const char *data,const char *inputFileList) {
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
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    h_selectBaselineYields_->Fill(0);

    if( ((Electrons->size() == 0) && (Muons->size() == 2)) ||
	((Electrons->size() == 2) && (Muons->size() == 0)) ||
	((Electrons->size() == 1) && (Muons->size() == 1)) )  h_selectBaselineYields_->Fill(1);//for ZGToLLG
    else continue;//for ZGToLLG

    //about photons
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt()<100) continue;
    h_selectBaselineYields_->Fill(2);
    //calulate ST and HadJets by cleaning the matching jet.
    bool hadJetID=true;
    int minDRindx=-100,phoMatchingJetIndx=-100,nHadJets=0, minDRindxl1=-100, minDRindxl2=-100;
    double minDR=99999, ST=0, minDRl1=10000, minDRl2=10000;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	double dR=bestPhoton.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
	if(Muons->size()==2){
	  if( (*Muons)[0].DeltaR((*Jets)[i]) < minDRl1 ){ minDRl1 = (*Muons)[0].DeltaR((*Jets)[i]); minDRindxl1=i;}
	  if( (*Muons)[1].DeltaR((*Jets)[i]) < minDRl2 ){ minDRl2 = (*Muons)[1].DeltaR((*Jets)[i]); minDRindxl2=i;}
	}
	else if(Electrons->size()==2){
	  if( (*Electrons)[0].DeltaR((*Jets)[i]) < minDRl1 ){ minDRl1 = (*Electrons)[0].DeltaR((*Jets)[i]); minDRindxl1=i;}
	  if( (*Electrons)[1].DeltaR((*Jets)[i]) < minDRl2 ){ minDRl2 = (*Electrons)[1].DeltaR((*Jets)[i]); minDRindxl2=i;}
	}
	else if((Electrons->size()==1) && (Muons->size()==1)){
	  if( (*Muons)[0].DeltaR((*Jets)[i]) < minDRl1 ){ minDRl1 = (*Muons)[0].DeltaR((*Jets)[i]); minDRindxl1=i;}
	  if( (*Electrons)[0].DeltaR((*Jets)[i]) < minDRl2 ){ minDRl2 = (*Electrons)[0].DeltaR((*Jets)[i]); minDRindxl2=i;}
	}
      }
    }//loop over jets
    if( minDR > 0.3 )  minDRindx   = -100;
    if( minDRl1 > 0.3) minDRindxl1 = -100;
    if( minDRl2 > 0.3) minDRindxl2 = -100;    
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	if((i!=minDRindx) && (i!=minDRindxl1) && (i!=minDRindxl2)){
	  hadJets.push_back((*Jets)[i]);
	  if(hadJetID) hadJetID=(*Jets_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) phoMatchingJetIndx=minDRindx;
    
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
    }
    //-----------------------------------------------------------------------
    
    //select skimming parameters (2 of 2)
    if( nHadJets >= 2 )  h_selectBaselineYields_->Fill(3);
    else continue;
    if( ST>200. )           h_selectBaselineYields_->Fill(4);
    else continue;
    // if( MET>100. )                    h_selectBaselineYields_->Fill(5);
    // else continue;
    //end of select skimming parameters
    newtree->Fill();
    
  } // loop over entries
  //  newtree->AutoSave();
}

TLorentzVector SkimmingZToLL::getBestPhoton(){
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
