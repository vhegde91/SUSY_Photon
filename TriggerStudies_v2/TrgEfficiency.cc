#define TrgEfficiency_cxx
#include "TrgEfficiency.h"
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

  TrgEfficiency ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void TrgEfficiency::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  TTree *newtree = fChain->CloneTree(0);

  string s_data=data;
  double nevts=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if(jentry>100) break;
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" <<endl;
    decade = k;
    //    cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    double wt = 1.0;

    if(!(HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0) ) continue;

    TLorentzVector bestPhoton = getBestPhoton();
    //    if(bestPhotonIndxAmongPhotons<0) continue;
    //if( (*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons] > 0.001 ) continue;
    for(int i=0;i<TriggerNames->size();i++){
      if((*TriggerPass)[i]==1) h_trgNum->Fill(i+1,wt);
      else if((*TriggerPass)[i]==-1) h_trgNum->Fill(i+1,-wt);
    }
    bool passTrg=false,passHTTrg=false,passPhoPtTrg=false,passHTPhoPtTrg=false;
    for(int i=0;i<nTrgs;i++) trgDec.push_back(false);
    for(int i=0;i<trgNames.size();i++){
      // cout<<"-------------------------------------------------------------"<<endl;
      // cout<<trgDec[i]<<" "<<trgNames[i]<<endl;
      // cout<<"-------------------------------------------------------------"<<endl;
      for(int j=0;j<TriggerNames->size();j++){
	//	cout<<(*TriggerPass)[j]<<" "<<(*TriggerNames)[j]<<endl;
	string trgName_j=(*TriggerNames)[j];
	trgName_j.pop_back();
	if( trgName_j == trgNames[i] ){
	  if((*TriggerPass)[j]==1) trgDec[i] = true;
	  else trgDec[i] = false;
	  break;	    
	}//if name mathes
      }//inner for
      // cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
      // cout<<trgDec[i]<<" "<<trgNames[i]<<endl;
      // cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    }//outer for
    
    for(int i=0;i<nDenTrgs;i++){
      if(trgDec[i]){passTrg=true;break;}
    }
    if(!passTrg) continue;
    /*    cout<<"=========================================================="<<endl;
    for(int j=0;j<TriggerNames->size();j++){
      cout<<(*TriggerPass)[j]<<" "<<(*TriggerNames)[j]<<endl;
    }
    cout<<"-------------------------------------------------------------"<<endl;
    for(int i=0;i<trgNames.size();i++){
      cout<<trgDec[i]<<" "<<trgNames[i]<<endl;
    }
    cout<<"PaasDecTrg"<<passTrg<<endl;
    cout<<"=========================================================="<<endl;
   */

    //calulate ST and nHadJets
    bool hadJetID=true;
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
	if( !(minDR < 0.3 && i==minDRindx) ){
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
    if( minDR<0.3 ) ST=ST+bestPhoton.Pt();//add the pt of photon if and only if there is a matching jet.
    sortTLorVec(&hadJets);
    //-----------------------------------------------------------------------

    if(hadJetID){
      h_intLumi->Fill(intLumi,wt);
      nevts++;
      for(int i=0;i<trgNames.size();i++){
	h_ST[i]->Fill(ST,wt);
	h_MET[i]->Fill(MET,wt);
	h_nHadJets[i]->Fill(nHadJets,wt);
	h_HT[i]->Fill(HT,wt);
	h_NJets[i]->Fill(NJets,wt);
	if(bestPhoton.Pt() > 0.5){
	  h_PhotonPt[i]->Fill(bestPhoton.Pt(),wt);
	  h_PhotonEta[i]->Fill(bestPhoton.Eta(),wt);
	  h_PhotonPhi[i]->Fill(bestPhoton.Phi(),wt);
	}
	if(Muons->size()==1){
	  h_Mu1Pt[i]->Fill((*Muons)[0].Pt(),wt);
	  h_Mu1Eta[i]->Fill((*Muons)[0].Eta(),wt);
	  h_Mu1Phi[i]->Fill((*Muons)[0].Phi(),wt);
	}
	if(Muons->size()==2){
	  h_Mu2Pt[i]->Fill((*Muons)[1].Pt(),wt);
	  h_Mu2Eta[i]->Fill((*Muons)[1].Eta(),wt);
	  h_Mu2Phi[i]->Fill((*Muons)[1].Phi(),wt);
	}
	if(Electrons->size()==1){
	  h_Ele1Pt[i]->Fill((*Electrons)[0].Pt(),wt);
	  h_Ele1Eta[i]->Fill((*Electrons)[0].Eta(),wt);
	  h_Ele1Phi[i]->Fill((*Electrons)[0].Phi(),wt);
	}
	if(Electrons->size()==2){
	  h_Ele2Pt[i]->Fill((*Electrons)[1].Pt(),wt);
	  h_Ele2Eta[i]->Fill((*Electrons)[1].Eta(),wt); 
	  h_Ele2Phi[i]->Fill((*Electrons)[1].Phi(),wt);
	}
      }
    }//hadJetID
  } // loop over entries
  cout<<"No. of entries survided: "<<nevts<<endl;
}

TLorentzVector TrgEfficiency::getBestPhoton(){
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( (*Photons_fullID)[iPho] && (*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons] < 0.001) {
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
