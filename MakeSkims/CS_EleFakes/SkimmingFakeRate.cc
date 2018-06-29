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
  bool applyISRWeights=1,applyWptWts=0;
  TFile* NLOWeightFile = new TFile("kfactors.root");
  TH1F* WJets_NLO = (TH1F*) NLOWeightFile->Get("WJets_012j_NLO/nominal");
  TH1F* WJets_LO = (TH1F*) NLOWeightFile->Get("WJets_LO/inv_pt");

  string s_data = data;
  cout<<"Appying ISR weights? "<<applyISRWeights<<endl;
  cout<<"Applying WPt Weights to WJets? "<<applyWptWts<<endl;
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

    //    CrossSection = 815.96;//ttbar incl
    if(madHT < 800) CrossSection = 2.61537118;//TTJets_HT600to800
    else if(madHT < 1200) CrossSection = 1.07722318;//TTJets_HT800to1200
    else if(madHT < 2500) CrossSection = 0.194972521;//TTJets_HT1200to2500
    else CrossSection = 0.0023234211;//TTJets_HT2500toInf

    Weight = CrossSection/NumEvents;

    h_MET_NoCut->Fill(MET,Weight*1000);
    h_HT_NoCut->Fill(HT,Weight*1000);
    h_NJets_NoCut->Fill(NJets,Weight*1000);
    h_madHT_NoCut->Fill(madHT,Weight*1000);

    if(applyISRWeights){
      double isrWt = getISRWt();
      vector<double> D_values={1.,0.7838,0.7600,0.7365,0.7254};
      	   //     vector<double> D_values={1.06963, 1.2915, 1.3086, 1.2959, 2.0147};
      if(madHT<600) isrWt = isrWt*D_values[0];
      else if(madHT < 800) isrWt = isrWt*D_values[1];
      else if(madHT < 1200) isrWt = isrWt*D_values[2];
      else if(madHT < 2500) isrWt = isrWt*D_values[3];
      else isrWt = isrWt*D_values[4];

      h_SumISRWts->Fill(1.0,isrWt*Weight);
      h_MET_NoCut_ISRWtd->Fill(MET,isrWt*Weight*1000);
      h_HT_NoCut_ISRWtd->Fill(HT,isrWt*Weight*1000);
      h_NJets_NoCut_ISRWtd->Fill(NJets,isrWt*Weight*1000);
      h_madHT_NoCut_ISRWtd->Fill(madHT,isrWt*Weight*1000);
    }
    if(applyWptWts){
      double Wpt=-999., wptWt=0;
      for( unsigned int p = 0 ; p < GenParticles->size() ; p++ ){
        if( abs(GenParticles_PdgId->at(p)) == 24 )
          Wpt = GenParticles->at(p).Pt();
      }
      if( Wpt>150. ){
        double LO = WJets_LO->GetBinContent( WJets_LO->FindBin(Wpt) );
        double NLO = WJets_NLO->GetBinContent( WJets_NLO->FindBin(Wpt) );
        if(LO==0) wptWt = 0;
        else wptWt = NLO/LO;
      }
      else
        wptWt =  (WJets_NLO->GetBinContent(1)/WJets_LO->GetBinContent(1));
      
      h_MET_NoCut_WptWtd->Fill(MET,wptWt*Weight*1000);
      h_HT_NoCut_WptWtd->Fill(HT,wptWt*Weight*1000);
      h_NJets_NoCut_WptWtd->Fill(NJets,wptWt*Weight*1000);
    }

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
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    if( minDR<0.3 ) ST=ST+bestEMObj.Pt();
    //-----------------------------------------------------------------------

    //select skimming parameters (2 of 2)
    if( nHadJets >= 2 )  h_selectBaselineYields_->Fill(3);
    else continue;
    if( ST>500. )        h_selectBaselineYields_->Fill(4);
    else continue;
    if( MET>100. )       h_selectBaselineYields_->Fill(5);
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

double SkimmingFakeRate::getISRWt(){
  vector<double> isrwt_arr={1., 0.920, 0.821, 0.715, 0.662, 0.561, 0.511};
  if(NJetsISR>=6) return isrwt_arr[6];
  else return isrwt_arr[NJetsISR];
  // double w[6]={0.920,0.821,0.715,0.662,0.561,0.511};
  // if( NJetsISR == 0 )
  //   return 1.0;
  // else if( NJetsISR >= 6 )
  //   return w[5];
  // else 
  //   return w[NJetsISR];
}
