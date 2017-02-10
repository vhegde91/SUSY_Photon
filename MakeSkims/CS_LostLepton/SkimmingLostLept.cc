#define SkimmingLostLept_cxx
#include "SkimmingLostLept.h"
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

  SkimmingLostLept ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingLostLept::EventLoop(const char *data,const char *inputFileList) {
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

    if(s_data=="CS_LostEle_madHT0to600" && madHT>600 ) continue;//putting a cut on madHT for SingleLept and DiLept samples of TTbar. Do not use for other samples.
    h_selectBaselineYields_->Fill(0);
    //---------------------------------- For Lost Muons -----------------------------------------------
    //should not be any electron. Need muons in the sample. Do not say anything about the muons.
    if( Electrons->size() == 0 )   h_selectBaselineYields_->Fill(1);
    else continue;
    //---------------------------------- For Lost Electrons -----------------------------------------------
    //should not be any Muon. Need electrons in the sample. Do not say anything about the electrons.
    /*    if( Muons->size() == 0 )   h_selectBaselineYields_->Fill(1);
    else continue;*/
    //---------------------------------------------------------------------------------

    //about photons
    TLorentzVector bestPhoton=getBestPhoton();
    /*  if(Photons->size() && (*Photons)[0].Pt()>100){
      cout<<"================bestPhotonPt:"<<bestPhoton.Pt()<<" "<<bestPhoton.Eta()<<" "<<bestPhoton.Phi()<<endl;
      for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
	cout<<iPhoton<<" Pt: "<<(*Photons)[iPhoton].Pt()<<" eta: "<<(*Photons)[iPhoton].Eta()<<" phi: "<<(*Photons)[iPhoton].Phi()<<" E: "<<(*Photons)[iPhoton].Energy()<<" "<<(*Photons_fullID)[iPhoton]<<" HT: "<<HT<<" NJ: "<<NJets<<" MET: "<<MET<<endl;
      }
    }
*/
    if(bestPhoton.Pt()<100) continue;
    int minDRindx=-100,phoMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	double dR=bestPhoton.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	if( !(minDR < 0.3 && i==minDRindx) )
	  hadJets.push_back((*Jets)[i]);
      }
    }
    if( minDR<0.3 ) phoMatchingJetIndx=minDRindx;
    //now hadJets contains all jets except the one matched to photon. check whether there is energy near photon or not. If yes then add it as a jet.
    if( phoMatchingJetIndx>=0 ){
      if( ( ((*Jets)[phoMatchingJetIndx].Pt()) > 1.1*(bestPhoton.Pt()) ) && ( ((*Jets)[phoMatchingJetIndx] - bestPhoton).Pt() > 30) ){
	hadJets.push_back( (*Jets)[phoMatchingJetIndx] - bestPhoton );
      }
    }      
    //hadJets contains all hadronic type of jets. If there is a matching jet and jetPt/PhotonPt > 1.1 and Pt of (TLorentz[matching jet] - TLorentz[photon]) is > 30,
    //then photon and hadJets are seperated. If there is no matching jet, then jet collection is used as it is in hadJets.
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    if( minDR<0.3 ) ST=ST+bestPhoton.Pt();//add the pt of photon if and only if there is a matching jet.
    //-----------------------------------------------------------------------

    //select skimming parameters (2 of 2)
    if( nHadJets >= 2 || NJets >= 2)  h_selectBaselineYields_->Fill(3);
    else continue;
    if( ST>500. || HT>500 )           h_selectBaselineYields_->Fill(4);
    else continue;
    if( MET>80. )                    h_selectBaselineYields_->Fill(5);
    else continue;
    //end of select skimming parameters
    /*  for(int i=0;i<GenParticles->size();i++){
      cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
    }
    for(int i=0;i<Jets->size();i++){
      if((*Jets)[i].Pt()>30 && abs((*Jets)[i].Eta())<2.4){
	cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<endl;
      }
    }
*/    
    //
    newtree->Fill();
 
  } // loop over entries
  //  newtree->AutoSave();
}

TLorentzVector SkimmingLostLept::getBestPhoton(){
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
