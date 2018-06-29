#define SkimmingZGamma_cxx
#include "SkimmingZGamma.h"
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

  SkimmingZGamma ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingZGamma::EventLoop(const char *data,const char *inputFileList) {
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
    //    cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //    cout<<CrossSection<<" nEvt:"<<NumEvents<<" Wt:"<<Weight<<endl; 
    // if(s_data=="CS_v2_LL_NLO_PtG0To130") CrossSection = 123.9;//for ZGToLLG Inclv
    // else if(s_data=="CS_v2_NuNu_NLO_PtG0To130")  CrossSection = 27.99;//for ZGToNuNuG Inclv
    // else if(s_data=="CS_v2_LL_NLO_PtG130")  CrossSection = 0.1585;//for ZGTo2LG PtG-130
    // else if(s_data=="CS_v2_NuNu_NLO_PtG130") CrossSection = 0.2762;//for ZGTo2NuNuG PtG-130
    Weight = CrossSection/NumEvents;
    h_selectBaselineYields_->Fill(0);
    storeBTagEff();
    //---------------------------------------------------------------------------------
    double genPho1Pt=0.,genPho1Eta=-10.;
    int genPho1Mom = 0.;
    for(int i=0;i<GenParticles->size();i++){
      if( (abs((*GenParticles_PdgId)[i])==22)
	  && ((abs((*GenParticles_ParentId)[i]) <= 100) ) 
	  && ( ((*GenParticles_Status)[i]==1) || (((*GenParticles_Status)[i]/10)==2) ) ){
	if(genPho1Pt < (*GenParticles)[i].Pt()) {
	  genPho1Pt = (*GenParticles)[i].Pt();
	  genPho1Eta= (*GenParticles)[i].Eta();
	  genPho1Mom = (*GenParticles_ParentId)[i];
	}
      }
    }
    h_LeadGenPhoPt ->Fill(genPho1Pt,Weight*10000.0);
    h_LeadGenPhoEta->Fill(genPho1Eta,Weight*10000.0);
    h_LeadGenPhoMom->Fill(genPho1Mom,Weight*10000.0);
    h_genMET->Fill(GenMET,Weight*10000.0);
    h_nGenJets->Fill(GenJets->size(),Weight*10000.0);
    if(genPho1Pt > 130.0 && (s_data=="CS_v2_LL_NLO_PtG0To130" || s_data=="CS_v2_NuNu_NLO_PtG0To130") ){ //for inclusive sample only
      if(jentry<10) cout<<"Considering only events with photon Pt < 130"<<endl;
      continue;
    }
    h_LeadGenPhoPt_GenFilt->Fill(genPho1Pt,Weight*10000.0);

    //about photons
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt()<100) continue;
    h_selectBaselineYields_->Fill(1);
    h_LeadGenPhoMom_FdBstPho->Fill(genPho1Mom,Weight*10000.0);
    h_BestPhotonPt->Fill(bestPhoton.Pt(),Weight*10000.0);

    // if(Electrons->size()!=0 || Muons->size()!=0) continue;//for ZGToNuNuG
    // else h_selectBaselineYields_->Fill(2);//for ZGToNuNuG
    if( ((Muons->size() == 2) && (Electrons->size()==0)) ||
     	((Electrons->size() == 2) && (Muons->size()==0)) )   h_selectBaselineYields_->Fill(2);//for ZGToLLG
    else continue;//for ZGToLLG
    //calculate some variables and fill some histograms
    TLorentzVector zvec;
    double zmass= -10;
    if(Muons->size()==2){
      zvec =  ((*Muons)[0] + (*Muons)[1]);
      zmass = zvec.M();
    }
    if(Electrons->size()==2 ){
      zvec =  ((*Electrons)[0] + (*Electrons)[1]);
      zmass = zvec.M();
    }
    h_ZMass->Fill(zmass,Weight*10000.0);
    h_ZPt->Fill(zvec.Pt(),Weight*10000.0);

    double dphi_PhoLep1=3.8,dphi_PhoLep2=3.8,dRphoClstLep=10000.0;
    if(Muons->size()==2) { 
      dphi_PhoLep1 = abs(bestPhoton.DeltaPhi((*Muons)[0]));
      dphi_PhoLep2 = abs(bestPhoton.DeltaPhi((*Muons)[1]));
      for(int i=0;i<Muons->size();i++){
	if(dRphoClstLep > bestPhoton.DeltaR((*Muons)[i])) dRphoClstLep = bestPhoton.DeltaR((*Muons)[i]);
      }
    }
    else if(Electrons->size()==2){
      dphi_PhoLep1 = abs(bestPhoton.DeltaPhi((*Electrons)[0]));
      dphi_PhoLep2 = abs(bestPhoton.DeltaPhi((*Electrons)[1]));
      for(int i=0;i<Electrons->size();i++){
	if(dRphoClstLep > bestPhoton.DeltaR((*Electrons)[i])) dRphoClstLep = bestPhoton.DeltaR((*Electrons)[i]);
      }
    }
    h_dR_PhoClstLep->Fill(dRphoClstLep,Weight*10000.0);
    h_dphi_PhoLep1->Fill(dphi_PhoLep1,Weight*10000.0);
    h_dphi_PhoLep2->Fill(dphi_PhoLep2,Weight*10000.0);

    if(Muons->size()==2){
      h_dR_PhoClstMu->Fill(dRphoClstLep,Weight*10000.0);
      h_dphi_PhoMu1->Fill(dphi_PhoLep1,Weight*10000.0);
      h_dphi_PhoMu2->Fill(dphi_PhoLep2,Weight*10000.0);
    }
    if(Electrons->size()==2){
      h_dR_PhoClstEle->Fill(dRphoClstLep,Weight*10000.0);
      h_dphi_PhoEle1->Fill(dphi_PhoLep1,Weight*10000.0);
      h_dphi_PhoEle2->Fill(dphi_PhoLep2,Weight*10000.0);
    }
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
      }
    }//loop over jets and find the closest jet to photon, muons and electrons
    if( minDR > 0.3 )  minDRindx   = -100;
    if( minDRl1 > 0.3) minDRindxl1 = -100;
    if( minDRl2 > 0.3) minDRindxl2 = -100;
    //if matching jet is found, modify jet collection.
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
	if( (i!=minDRindx) && (i!=minDRindxl1) && (i!=minDRindxl2) ){
	  hadJets.push_back((*Jets)[i]);
	  if(hadJetID) hadJetID=(*Jets_ID)[i];
	}
      }
    }
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
    if( ST>500. )           h_selectBaselineYields_->Fill(4);
    else continue;
    //end of select skimming parameters
    
    newtree->Fill();
 
  } // loop over entries
  //  newtree->AutoSave();
}

TLorentzVector SkimmingZGamma::getBestPhoton(){
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

void SkimmingZGamma::storeBTagEff(){
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

    /*    if( minDR<0.3 ) phoMatchingJetIndx=minDRindx;
    //now hadJets contains all jets except the one matched to photon and those matched to leptons. check whether there is energy near photon or not. If yes then add it as a jet. For leptons, do not consider the matching jet.
    if( phoMatchingJetIndx>=0 ){
      if( ((*Jets)[phoMatchingJetIndx].Pt()) > 1.1*(bestPhoton.Pt()) ){
	if( (((*Jets)[phoMatchingJetIndx] - bestPhoton).Pt())>30){
	  hadJets.push_back( (*Jets)[phoMatchingJetIndx] - bestPhoton );
	}
      }
    }
    //hadJets contains all jets if minDR>0.3. If minDR<0.3 and jetPt/GPt < 1.1, hadJets does not contain a jet matched to EMobj.
    //                                        If minDR<0.3 and jetPt/GPt > 1.1  it has only hadronic type of jets(photon is not accounted for)
    //                                        If there are 2 muons or 2 electrons, then the corresponding jet is removed.
    */
