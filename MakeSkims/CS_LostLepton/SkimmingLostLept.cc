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

    //    CrossSection = 123.9;//for ZGToLLG Inclv
    //    CrossSection = 0.1585;//for ZGTo2LG PtG-130                                                                                
    //    Weight = CrossSection/NumEvents;
    if(s_data=="CS_LostEle_madHT0to600" && madHT>600 ) continue;//putting a cut on madHT for SingleLept and DiLept samples of TTbar. Do not use for other samples.
    h_selectBaselineYields_->Fill(0);
    storeBTagEff();
    //---------------------------------- For Lost Muons -----------------------------------------------
    //should not be any electron. Need muons in the sample. Do not say anything about the muons.
    // if( Electrons->size() == 0 )   h_selectBaselineYields_->Fill(1);
    // else continue;
    //---------------------------------- for Lost Electrons -----------------------------------------------
    //should not be any Muon. Need electrons in the sample. Do not say anything about the electrons.
    if( Muons->size() == 0 )   h_selectBaselineYields_->Fill(1);
    else continue;
    //---------------------------------------------------------------------------------

    //about photons
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt()<100) continue;
    h_selectBaselineYields_->Fill(2);
    
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
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    if( minDR<0.3 ) ST=ST+bestPhoton.Pt();//add the pt of photon if and only if there is a matching jet.
    //-----------------------------------------------------------------------

    //select skimming parameters (2 of 2)
    if( nHadJets >= 2 )  h_selectBaselineYields_->Fill(3);
    else continue;
    if( ST>500.)         h_selectBaselineYields_->Fill(4);
    else continue;
    if( MET>100. )       h_selectBaselineYields_->Fill(5);
    else continue;
    //end of select skimming parameters
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

void SkimmingLostLept::storeBTagEff(){
  double CSVv2WP = 0.8484;
  for(unsigned ja = 0; ja < Jets->size(); ++ja){
    //HT jet cuts
    if(!Jets_HTMask->at(ja)) continue;
    
    //fill by flavor
    int flav = abs(Jets_hadronFlavor->at(ja));
    double csv = Jets_bDiscriminatorCSV->at(ja);
    double pt = Jets->at(ja).Pt();
    //use abs(eta) for now
    double eta = fabs(Jets->at(ja).Eta());
    if(flav==5){
      d_eff_b->Fill(pt,eta);
      if(csv > CSVv2WP) n_eff_b->Fill(pt,eta);
    }
    else if(flav==4){
      d_eff_c->Fill(pt,eta);
      if(csv > CSVv2WP) n_eff_c->Fill(pt,eta);
    }
    else if(flav<4 || flav==21){
      d_eff_udsg->Fill(pt,eta);
      if(csv > CSVv2WP) n_eff_udsg->Fill(pt,eta);
    }
  }
}
