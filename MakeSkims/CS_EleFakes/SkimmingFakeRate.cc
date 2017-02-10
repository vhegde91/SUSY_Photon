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

    if(s_data=="CS_FR_madHT0to600" && madHT>600 ) continue;//only for TTJets_Tune*, TTJets_SingleLept*, TTJets_Dilept*
    //select skimming parameters (1 of 2)
    h_selectBaselineYields_->Fill(0);
    if( Electrons->size() <= 1 && Muons->size()==0 )   h_selectBaselineYields_->Fill(1);
    else continue;
     
    //about electrons
    bool hasEle=0,hasPho=0;
    if(Electrons->size()==1){
      if( (*Electrons)[0].Pt()>100 ) hasEle=true;
    }
    //about photons
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt()>100) hasPho=true;
    //calulate ST and number of Hadronic Jets    
    TLorentzVector bestEMObj;
    if(hasEle && !hasPho)      { bestEMObj=(*Electrons)[0]; h_selectBaselineYields_->Fill(2); }
    else if(!hasEle && hasPho) { bestEMObj=bestPhoton;      h_selectBaselineYields_->Fill(2); }
    else continue; 
    /*    if(hasEle){      bestEMObj=(*Electrons)[0]; h_selectBaselineYields_->Fill(2); }
	  else if(hasPho){ bestEMObj=bestPhoton;      h_selectBaselineYields_->Fill(2); }
	  else continue;
    */
    //    if(hasEle){if((*Electrons)[0].Pt()<100) cout<<(*Electrons)[0].Pt();}
    int minDRindx=-100,emMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	double dR=bestEMObj.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	if( !(minDR < 0.3 && i==minDRindx) )
	  hadJets.push_back((*Jets)[i]);
      }
    }
    if( minDR<0.3 ) emMatchingJetIndx=minDRindx;
    //now hadJets contains all jets except the one matched to em obj. check whether there is energy near emobj or not. If yes then add it as a jet.
    if( emMatchingJetIndx>=0 ){
      if( ( ((*Jets)[emMatchingJetIndx].Pt()) > 1.1*(bestPhoton.Pt()) ) && ( ((*Jets)[emMatchingJetIndx] - bestPhoton).Pt() > 30) ){
	hadJets.push_back( (*Jets)[emMatchingJetIndx] - bestEMObj );
      }
    }      
    //hadJets contains all jets if jetPt/EMPt > 1.1, else it has only hadronic type of jets(emobj is not accounted for)
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    if( minDR<0.3 ) ST=ST+bestEMObj.Pt();
    //-----------------------------------------------------------------------

    //select skimming parameters (2 of 2)
    if( nHadJets >= 2 || NJets >= 2)  h_selectBaselineYields_->Fill(3);
    else continue;
    if( ST>500. || HT>500 )           h_selectBaselineYields_->Fill(4);
    else continue;
    if( MET>100. )                    h_selectBaselineYields_->Fill(5);
    else continue;
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
}
