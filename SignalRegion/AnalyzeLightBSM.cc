#define AnalyzeLightBSM_cxx
#include "AnalyzeLightBSM.h"
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

  AnalyzeLightBSM ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void AnalyzeLightBSM::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  TString s_data=data;
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  bool applISRWtsTottbar = 1;  
  int evtSurvived=0,minbtags=0;
  TFile* pufile = TFile::Open("PileupHistograms_0121_69p2mb_pm4p6.root","READ");
  //choose central, up, or down 
  TH1* puhist = (TH1*)pufile->Get("pu_weights_down");
  cout<<"applying PU weights."<<endl
      <<"applying ISR weights to ttbar? "<<applISRWtsTottbar<<endl;
  //set file for getting ISR wts for signal
  TFile *fISR;  TH2D *h2_isrWtCorr;
  if(s_data.Contains("FastSim")){
    if(s_data.Contains("T5bbbb")) fISR = new TFile("T5bbbbZg_MassScan.root");
    else if(s_data.Contains("T5tttt")) fISR = new TFile("T5ttttZg_MassScan.root");
    else if(s_data.Contains("T5qqqq")) fISR = new TFile("T5qqqqHg_MassScan.root");
    else if(s_data.Contains("T6tt")) fISR = new TFile("T6ttZg_MassScan.root");
    h2_isrWtCorr = (TH2D*)fISR->Get("Nevts_NoISRWt");
    h2_isrWtCorr->Divide((TH2D*)fISR->Get("Nevts_ISRWt"));
  }
  //////////////////////////////////////
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
    //    print(jentry);    
    //    wt=Weight*1000.0*lumiInfb;
    wt=Weight*1000.0*lumiInfb*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));
    //ISR weighting. Makes sense only if you have all the events in ntuples.
    h_nEvts->Fill(1,wt);
    double isrWt=0.;
    vector<double> isrwt_arr={1., 0.920, 0.821, 0.715, 0.662, 0.561, 0.511};
    if(NJetsISR>=6) isrWt = isrwt_arr[6];
    else isrWt = isrwt_arr[NJetsISR];
    h_nEvts->Fill(2,wt*isrWt);
    //ISR weighting, end.

    if(!s_data.Contains("FastSim")){
      if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0) ) continue;
    }
    if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;//apply iso track veto
    
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhotonIndxAmongPhotons<0) continue;
    bool bestPhoHasPxlSeed=true;
    if((*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons]<0.001) bestPhoHasPxlSeed=false;
    if( bestPhoHasPxlSeed ) continue;
    bool eMatchedG=check_eMatchedtoGamma();
    bool process=true;
    if(eMatchedG) continue;
    //    if(allBestPhotons.size()<2) continue;
    bool noFakeJet = true;
    double gendRLepPho = getGendRLepPho();

    if(s_data=="genprompt" || s_data=="TTG" || s_data=="WG" || s_data=="ZG"){
      if(jentry==0){cout<<"**********processing "<<s_data<<" with prompt Gen photon"<<endl;}
      // if(hasGenPromptPhoton)
      // 	process=true;
      // else continue;
    }//Gen prompt
    else if( s_data=="WJets" ){
      if(hasGenPromptPhoton && gendRLepPho > 0.5 && madMinPhotonDeltaR > 0.5) continue;
      if(jentry<3) cout<<"Non-Prompt, dR(pho,q/g/lep) < 0.5 ";
    }
    else if((s_data=="gennonprompt") || (s_data=="TTJets") || (s_data=="SingleTop") || (s_data=="ZJets") ){
      if(hasGenPromptPhoton && gendRLepPho > 0.3 && madMinPhotonDeltaR > 0.3) continue;
      if(jentry<3) cout<<"Non-Prompt, dR(pho,q/g/lep) < 0.3 ";
    }//Gen non-prompt
    else if(s_data=="QCD"){
      if(jentry==0){cout<<"**********processing "<<s_data<<" w/o prompt gen photon"<<endl;}
      //      if(hasGenPromptPhoton && madMinDeltaRStatus==1 && madMinPhotonDeltaR > 0.4) continue;
      if(!((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]) && madMinDeltaRStatus==1 && madMinPhotonDeltaR > 0.4) continue;
    }
    else if(s_data=="GJets"){
      if(jentry==0){cout<<"**********processing "<<s_data<<" with prompt Gen photon"<<endl;}
      //      if(!hasGenPromptPhoton) continue;
      if((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]) continue;
    }
    else if(s_data=="SignalZ"){
      TLorentzVector v1;
      int nZbsn=0,nZbsngsKids=0;
      //      if(jentry==0){cout<<"**********processing "<<s_data<<" You may not want to do this way!!!!!!"<<endl;}
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      //  process=false;
      for(int i=0;i<GenParticles->size();i++){
	if( (abs((*GenParticles_PdgId)[i])==23) && (abs((*GenParticles_ParentId)[i])==1000023) ) {nZbsn++;}
      }
      //      cout<<nZbsn;
      //      if(nZbsn!=1){continue;}    
    }
    else if(s_data=="signalH"){
      wt=wt/4.0;//divived by 4 because, Frank Jensen(one who produced sample) has mulyiplied the weight by 4 for his convenience.
      TLorentzVector v1;
      int nHig=0,nHiggsKids=0;
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      process=false;
      for(int i=0;i<GenParticles->size();i++){
	if( (abs((*GenParticles_PdgId)[i])==25) && (abs((*GenParticles_ParentId)[i])==1000023) ) {nHig++;}
      }
      if(nHig!=1){continue;}
      else{
	process=true;
	int nbs=0;
	for(int i=0;i<GenParticles->size();i++){
	  if( ((*GenParticles_ParentId)[i]==25)){nHiggsKids++;v1=v1+(*GenParticles)[i];}
	  if( (abs((*GenParticles_PdgId)[i])==5) && ((*GenParticles_ParentId)[i]==25) ){
	    nbs++;
	    // v1=v1+(*GenParticles)[i]; 
	  }
	}
	h2_mH_HKids->Fill(v1.M(),nHiggsKids);
      }    
    }
    else if(s_data.Contains("FastSim")){
      //      //reject events with any jet pt>20, |eta|<2.5 NOT matched to a GenJet (w/in DeltaR<0.3) and chfrac < 0.1
      for(unsigned j = 0; j < Jets->size(); ++j){
      	if(Jets->at(j).Pt() <= 20 || fabs(Jets->at(j).Eta())>=2.5) continue;
      	bool genMatched = false;
      	for(unsigned g = 0; g < GenJets->size(); ++g){
      	  if(GenJets->at(g).DeltaR(Jets->at(j)) < 0.3) {
      	    genMatched = true;
      	    break;
      	  }
      	}
      	if(!genMatched && Jets_chargedHadronEnergyFraction->at(j) < 0.1){
      	  noFakeJet = false;
      	  break;
      	}
      }
      double isrWt = 0,isrWtCorr = h2_isrWtCorr->GetBinContent(h2_isrWtCorr->GetXaxis()->FindBin(SusyMotherMass),h2_isrWtCorr->GetYaxis()->FindBin(SusyLSPMass));
      vector<double> isrwt_arr={1., 0.920, 0.821, 0.715, 0.662, 0.561, 0.511};
      if(NJetsISR>=6) isrWt = isrwt_arr[6];
      else isrWt = isrwt_arr[NJetsISR];
      wt = wt*isrWt*isrWtCorr;
    }

    if(applISRWtsTottbar && s_data=="TTJets"){
      double isrWt = 0;
      vector<double> D_values={1.0697, 1.0150, 0.9917, 0.9435, 0.95};
      //      vector<double> D_values={1.071, 0.7838, 0.7600, 0.7365, 0.7254};
      vector<double> isrwt_arr={1., 0.920, 0.821, 0.715, 0.662, 0.561, 0.511};
      if(NJetsISR>=6) isrWt = isrwt_arr[6];
      else isrWt = isrwt_arr[NJetsISR];
      
      if(madHT<600) isrWt = isrWt*D_values[0];
      else if(madHT < 800) isrWt = isrWt*D_values[1];
      else if(madHT < 1200) isrWt = isrWt*D_values[2];
      else if(madHT < 2500) isrWt = isrWt*D_values[3];
      else isrWt = isrWt*D_values[4];
      wt = wt*isrWt;
    }

    if( eMatchedG || bestPhoHasPxlSeed || !process || (bestPhoton.Pt()<100) || (Electrons->size() > 0) || (Muons->size() > 0) )
      continue;

    bool hadJetID=true;
    int minDRindx=-100,photonMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0,mhtstar=0;
    TLorentzVector mhtstar_vec;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= HT_EtaCut) ){
	double dR=bestPhoton.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= HT_EtaCut) ){
	if( !(minDR < 0.3 && i==minDRindx) ){
	  hadJets.push_back((*Jets)[i]);
	  if(hadJetID) hadJetID=(*Jets_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) photonMatchingJetIndx=minDRindx;
    //now hadJets contains all jets except the one matched to photon. check whether there is energy near photon or not. If yes then add it as a jet.
    /*if( photonMatchingJetIndx>=0 ){
      if( ((*Jets)[photonMatchingJetIndx].Pt()) > 1.1*(bestPhoton.Pt()) && ((*Jets)[photonMatchingJetIndx] - bestPhoton).Pt()>30.0 ){
	hadJets.push_back( (*Jets)[photonMatchingJetIndx] - bestPhoton );
	if(hadJetID) h_jetPtNearPhoton->Fill( ((*Jets)[photonMatchingJetIndx] - bestPhoton).Pt() );
      }
    }*/
    //hadJets contains all jets if jetPt/GPt > 1.1, else it has only hadronic type of jets(photon is not accounted for)
    for(int i=0;i<hadJets.size();i++){
      mhtstar_vec=mhtstar_vec-hadJets[i];
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
      mhtstar_vec=mhtstar_vec-bestPhoton;
    }
    sortTLorVec(&hadJets);
    mhtstar=mhtstar_vec.Pt();
    h_minDR_phoJet->Fill(minDR);
    
    double dphi1=3.8,dphi2=3.8,dphi3=3.8,dphi4=3.8,dphiG_MET=3.8;
    if(bestPhoton.Pt()>0.1) dphiG_MET=abs(DeltaPhi(METPhi,bestPhoton.Phi()));
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));
    if(hadJets.size() > 2 ) dphi3 = abs(DeltaPhi(METPhi,(hadJets)[2].Phi()));
    if(hadJets.size() > 3 ) dphi4 = abs(DeltaPhi(METPhi,(hadJets)[3].Phi()));

    double mTPhoMET=sqrt(2*(bestPhoton.Pt())*MET*(1-cos(DeltaPhi(METPhi,bestPhoton.Phi()))));
    //    if(mTPhoMET<100) continue;
    // if(BTags!=0) continue;
    // if(nHadJets>4) continue;
    //    if(MET<=200) continue;
    if(photonMatchingJetIndx>=0 && ((*Jets)[photonMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    if(photonMatchingJetIndx<0) continue;

    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    process = process && !eMatchedG && !bestPhoHasPxlSeed && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && MET > 100 && dphi1 > 0.3 && dphi2 > 0.3;
    //  process = process && ST>500 && nHadJets>=2 && MET>100 && dphi1 > 0.3 && dphi2 > 0.3;
    //    process = process && NJets>=3 && MET>100;// && dphi1 > 0.3;
    //if(process){process=HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0 && minDR<0.3;}
    if(s_data.Contains("FastSim")){
      hadJetID = true;
      if(!noFakeJet) continue;
    }
    if(process){
      evtSurvived++;
      h_RunNum->Fill(RunNum);
      h2_mMomvsmKid->Fill(SusyMotherMass,SusyLSPMass,wt);

      if(hadJetID){
	h_intLumi->Fill(lumiInfb,wt);
	h_ST->Fill(ST,wt);
	h_MET->Fill(MET,wt);
	h_nHadJets->Fill(nHadJets,wt);
	h_BTags->Fill(BTags,wt);
	h_BestPhotonPt->Fill( bestPhoton.Pt(),wt );

	h_HT->Fill(HT,wt);
	h_MHT->Fill(MHT,wt);
	h_nJets->Fill(NJets,wt);
	h_MHTstar->Fill(mhtstar,wt);

	h2_PtPhotonvsMET->Fill( bestPhoton.Pt(),MET,wt);
	h2_METvsnHadJets->Fill(MET,nHadJets,wt);
	h2_METvBinvsnHadJets->Fill(MET,nHadJets,wt);
	h2_METvsdPhi1->Fill(MET,dphi1,wt);
	h2_METvsdPhi2->Fill(MET,dphi2,wt);
	h2_METvsMindPhi1dPhi2->Fill(MET,min(dphi1,dphi2),wt);
	h2_dPhi1dPhi2->Fill(dphi1,dphi2,wt);

	h_madHT->Fill(madHT,wt);
	findObjMatchedtoG(bestPhoton);
	
	//------------------------ Sbins-----------------------------
	int searchRegion=0,sBin1=-100,m_i1=0;
	if     (nHadJets >= 2 && nHadJets <= 4 ){ searchRegion=1; sBin1 = 0;}
	else if(nHadJets == 5 || nHadJets == 6 ){ searchRegion=2; sBin1 = 7;}
	else if(nHadJets >= 7                  ){ searchRegion=3; sBin1 = 14;}
	int sBin2=-100,m_i=0;
	if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      sBin2 = 0;
	else if(nHadJets >= 5 && BTags==0)                  sBin2 = 7;
	else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) sBin2 = 14;
	else if(nHadJets >= 5 && BTags==1)                  sBin2 = 21;
	else if(BTags>=2)                                   sBin2 = 28;
	for(int i=0;i<METBinLowEdge.size()-1;i++){
	  if(METBinLowEdge[i]<99.99) continue;
	  m_i++;
	  if(MET>=METBinLowEdge[i] && MET<METBinLowEdge[i+1]){ sBin2 = sBin2+m_i;break; }
	  else if(MET>=METBinLowEdge[METBinLowEdge.size()-1]){ sBin2 = sBin2+7  ;break; }
	}
	if(BTags>=2 && sBin2==35) sBin2=34;
	int sBin4 = getBinNoV4(nHadJets),  sBin7 = getBinNoV7(nHadJets);
	
	h_STvBin->Fill(ST,wt);
	h_METvBin->Fill(MET,wt);
	h_BestPhotonPtvBin->Fill(bestPhoton.Pt(),wt);
	h_MHTvBin->Fill(MHT,wt);
	h_MET_Rall->Fill(MET,wt);

	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_R[searchRegion-1]->Fill(MET,wt);
	  for(int i=0;i<METBinLowEdge2.size()-1;i++){
	    if(METBinLowEdge2[i]<99.99) continue;
	    m_i1++;
	    if(MET>=METBinLowEdge2[i] && MET<METBinLowEdge2[i+1]){ sBin1 = sBin1+m_i1;break; }
	    else if(MET>=METBinLowEdge2[METBinLowEdge2.size()-1]){ sBin1 = sBin1+7   ;break; }
	  }
	  h_SBins_v1->Fill(sBin1,wt);
	}
        else cout<<"Event outside search region! ";
	if(nHadJets >= 2 && nHadJets <= 4 && BTags==0)      h_MET_R_v2[0]->Fill(MET,wt);
        else if(nHadJets >= 5 && BTags==0)                  h_MET_R_v2[1]->Fill(MET,wt);
        else if(nHadJets >= 2 && nHadJets <= 4 && BTags==1) h_MET_R_v2[2]->Fill(MET,wt);
        else if(nHadJets >= 5 && BTags==1)                  h_MET_R_v2[3]->Fill(MET,wt);
        else if(BTags>=2)                                   h_MET_R_v2[4]->Fill(MET,wt);
        h_SBins->Fill(sBin2,wt);

	int sBin3=-100,m_i3=0;
	if(BTags==0){
	  if(nHadJets>=2 && nHadJets<=4) sBin3=0;
	  else if(nHadJets==5 || nHadJets==6) sBin3=7;
	  else sBin3=14;
	}
	else if(BTags==1){
	  if(nHadJets>=2 && nHadJets<=4) sBin3=21;
	  else if(nHadJets==5 || nHadJets==6) sBin3=28;
	  else sBin3=35;
	}
	else{
	  if(nHadJets>=2 && nHadJets<=4) sBin3=42;
	  else if(nHadJets==5 || nHadJets==6) sBin3=49;
	  else sBin3=56;
	}
	for(int i=0;i<METBinLowEdge2.size()-1;i++){
	  if(METBinLowEdge2[i]<99.99) continue;
	  m_i3++;
	  if(MET>=METBinLowEdge2[i] && MET<METBinLowEdge2[i+1]){ sBin3 = sBin3+m_i3;break; }
	  else if(MET>=METBinLowEdge2[METBinLowEdge2.size()-1]){ sBin3 = sBin3+7   ;break; }
	}
	h_SBins_v3->Fill(sBin3,wt);
	h_SBins_v4_CD->Fill(sBin4,wt);
	h_SBins_v7_CD->Fill(sBin7,wt);
	//------------------------ Sbins-----------------------------
	h_BestG_Eta->Fill(bestPhoton.Eta(),wt);
	h_BestG_Phi->Fill(bestPhoton.Phi(),wt);

	h2_ptRatiovsDR->Fill(minDR,((*Jets)[minDRindx].Pt())/(bestPhoton.Pt()));
	h2_MET_MHTstar->Fill(MET,mhtstar);
	h2_STvsHT->Fill(ST,HT,wt);

	int nGenLep=0;
	for(int i=0;i<GenParticles->size();i++){
	  if((*GenParticles)[i].Pt()!=0){
	    if( (abs((*GenParticles_PdgId)[i])==11 || abs((*GenParticles_PdgId)[i])==13) && (abs((*GenParticles_ParentId)[i])==24) ) nGenLep++;
	  }
	}
	h_nGenLep->Fill(nGenLep,wt);
	//if(photonMatchingJetIndx>=0 && ( ((*Jets)[photonMatchingJetIndx].Pt())/(bestPhoton.Pt()) > 1.1) ) h_jetPtNearPhoton->Fill( ((*Jets)[photonMatchingJetIndx] - bestPhoton).Pt(),wt );
	if(hadJets.size() > 0 )	h2_METvsdPhiJ1MET->Fill(dphi1,MET,wt);
	if( (Photons->size())==1 ){h2_1G_PtPhotonvsMET->Fill(bestPhoton.Pt(),MET,wt);}
	else{h2_More2G_PtPhotonvsMET->Fill(bestPhoton.Pt(),MET,wt);}
	
	h_dPhi_METBestPhoton->Fill(dphiG_MET,wt);

	if(hadJets.size() > 0 ) h_dPhi_METjet1->Fill( dphi1,wt );
	if(hadJets.size() > 1 ) h_dPhi_METjet2->Fill( dphi2,wt );
	if(hadJets.size() > 2 ) h_dPhi_METjet3->Fill( dphi3,wt );
	if(hadJets.size() > 3 ) h_dPhi_METjet4->Fill( dphi4,wt );
	h_mindPhi1dPhi2->Fill(min(dphi1,dphi2),wt);
	//	if(hadJets.size() > 3 ) h_dPhi_METjet4->Fill( abs(DeltaPhi(METPhi,( ((hadJets)[0]+(hadJets)[1]+(hadJets)[2]+(hadJets)[3]).Phi()))) );
	for(int i=0;i<hadJets.size() && i<4;i++){
	  h_jetPt[i] ->Fill(hadJets[i].Pt());
	  h_jetEta[i]->Fill(hadJets[i].Eta());
	  h_jetPhi[i]->Fill(hadJets[i].Phi());
	}
	
	for(int i=0;i<hadJets.size();i++){
	  h_jetsPt ->Fill(hadJets[i].Pt(),wt);
	  h_jetsEta->Fill(hadJets[i].Eta(),wt);
	  h_jetsPhi->Fill(hadJets[i].Phi(),wt);
	}
	h_dPhi_MET_jets->Fill( abs(DeltaPhi(METPhi,(mhtstar_vec-bestPhoton).Phi())) );
	h_isoEleTrack->Fill(isoElectronTracks,wt);
	h_isoMuTrack->Fill(isoMuonTracks,wt);
	h_isoPiTrack->Fill(isoPionTracks,wt);

	h_hasGenPrompt->Fill(hasGenPromptPhoton,wt);
	h_photonPrompt->Fill(!(*Photons_nonPrompt)[bestPhotonIndxAmongPhotons],wt);
	h_fakePhoton->Fill((*Photons_electronFakes)[bestPhotonIndxAmongPhotons],wt);

	h_mTPhoMET->Fill( mTPhoMET,wt );
	h_nBestPho->Fill(allBestPhotons.size(),wt);
	//h_nBestPho->Fill(Photons->size(),wt);
	if(allBestPhotons.size()>1){
	  sortTLorVec(&allBestPhotons);
	  h_BestPho2Pt->Fill(allBestPhotons[1].Pt(),wt);
	  h_dPhi2Photons->Fill(abs(allBestPhotons[0].DeltaPhi(allBestPhotons[1])),wt);
	  double dR=100;
	  int match=-100;
	  for(int i=0;i<GenParticles->size();i++){
	    if((*GenParticles)[i].Pt()!=0){
	      if(dR > (allBestPhotons[1].DeltaR((*GenParticles)[i])) && ( (allBestPhotons[1].Pt()/(*GenParticles)[i].Pt()> 0.5) && (allBestPhotons[1].Pt()/(*GenParticles)[i].Pt()< 1.5) ) ){
		dR=allBestPhotons[1].DeltaR((*GenParticles)[i]);
		match=i;
	      }
	    }
	  }
	  if(dR<0.2 && match>0)  h_2ndPhoGenMatch->Fill(abs((*GenParticles_PdgId)[match]),wt);
	  else h_2ndPhoGenMatch->Fill(0.0,wt);
	  /*	  cout<<"----------------------------------"<<endl;
	  for(int ii=0;ii<allBestPhotons.size();ii++){
	    cout<<ii<<" Pt:"<<allBestPhotons[ii].Pt()<<" eta:"<<allBestPhotons[ii].Eta()<<" phi:"<<allBestPhotons[ii].Phi()<<" E:"<<allBestPhotons[ii].Energy()<<endl;
	  }
	  print(jentry);*/
	}
	h_my_ST->Fill(ST,wt);
	h_my_MET->Fill(MET,wt);
      }

      if(!hadJetID){
	h_ST_FalseJID->Fill(ST,wt);
	h_MET_FalseJID->Fill(MET,wt);
	h_nHadJets_FalseJID->Fill(nHadJets,wt);
	h_BTags_FalseJID->Fill(BTags,wt);
	h_BestPhotonPt_FalseJID->Fill( bestPhoton.Pt(),wt );
	h2_PtPhotonvsMET_FalseJID->Fill( bestPhoton.Pt(),MET,wt);
      }
      h_ST_NoJID->Fill(ST,wt);
      h_MET_NoJID->Fill(MET,wt);
      h_nHadJets_NoJID->Fill(nHadJets,wt);
      h_BTags_NoJID->Fill(BTags,wt);
      h_BestPhotonPt_NoJID->Fill( bestPhoton.Pt(),wt );
      h2_PtPhotonvsMET_NoJID->Fill( bestPhoton.Pt(),MET,wt);
      
      for(int i=0;i<Jets->size();i++){
	if((*Jets)[i].Pt()>30 && (*Jets_ID)[i] ){
	  h_NM->Fill((*Jets_neutralMultiplicity)[i],wt);
	  if( abs((*Jets)[i].Eta())<3){h_NMEta3->Fill((*Jets_neutralMultiplicity)[i],wt);}
	  if( abs((*Jets)[i].Eta())>=3){h_NMHF->Fill((*Jets_neutralMultiplicity)[i],wt);}
	  h_CM->Fill((*Jets_chargedMultiplicity)[i],wt);
	  h_NHF->Fill((*Jets_neutralHadronEnergyFraction)[i],wt);
	  h_NEMF->Fill((*Jets_neutralEmEnergyFraction)[i],wt);
	  h_CHF->Fill((*Jets_chargedHadronEnergyFraction)[i],wt);
	  h_CEMF->Fill((*Jets_chargedEmEnergyFraction)[i],wt);

	  h_PhoMult->Fill((*Jets_photonMultiplicity)[i],wt);
	  h_PhoFrac->Fill((*Jets_photonEnergyFraction)[i],wt);
	}			
      }	
      
      h_GenMET->Fill(GenMET,wt);
      
    }//process
    //}
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
}


TLorentzVector AnalyzeLightBSM::getBestPhoton(){
  bool passIDVeryLoose=false,passIDLoose=false,passIsoVeryLoose=false,passIsoLoose=false;
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  allBestPhotons.resize(0);
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( ((*Photons_fullID)[iPho]) && ((*Photons_hasPixelSeed)[iPho]<0.001) ) {
      //if( ((*Photons_fullID)[iPho]) ) {
      goodPho.push_back( (*Photons)[iPho] );
      goodPhoIndx.push_back(iPho);
      allBestPhotons.push_back((*Photons)[iPho]);
    }
  }
  if(allBestPhotons.size()>1) sortTLorVec(&allBestPhotons);
  int highPtIndx=-100;
  for(int i=0;i<goodPho.size();i++){
    if(i==0) highPtIndx=0;
    else if( (goodPho[highPtIndx].Pt()) < (goodPho[i].Pt()) ){highPtIndx=i;}
  }

  if(highPtIndx>=0){
    bestPhotonIndxAmongPhotons = goodPhoIndx[highPtIndx];
    return goodPho[highPtIndx];
  }
  else{
    bestPhotonIndxAmongPhotons = -100;
    TLorentzVector v0;return v0;
  }
}

int AnalyzeLightBSM::getBinNoV4(int nHadJets){
  int sBin=-100,m_i=0;
  if(BTags==0){
    if(nHadJets>=2 && nHadJets<=4)     { sBin=0;}
    else if(nHadJets==5 || nHadJets==6){ sBin=8;}
    else if(nHadJets>=7)               { sBin=15;}
  }
  else{
    if(nHadJets>=2 && nHadJets<=4)     { sBin=22;}
    else if(nHadJets==5 || nHadJets==6){ sBin=29;}
    else if(nHadJets>=7)               { sBin=36;}
  }
  if(sBin==0){
    for(int i=0;i<METBinLowEdgeV4_njLow.size()-1;i++){
      if(METBinLowEdgeV4_njLow[i]<99.99) continue;
      m_i++;
      if(MET >= METBinLowEdgeV4_njLow[i] && MET < METBinLowEdgeV4_njLow[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METBinLowEdgeV4_njLow[METBinLowEdgeV4_njLow.size()-1])  { sBin = 8         ;break; }
    }
  }
  else{
    for(int i=0;i<METBinLowEdgeV4.size()-1;i++){
      if(METBinLowEdgeV4[i]<99.99) continue;
      m_i++;
      if(MET >= METBinLowEdgeV4[i] && MET < METBinLowEdgeV4[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METBinLowEdgeV4[METBinLowEdgeV4.size()-1])  { sBin = sBin+7   ;break; }
    }
  }
  return sBin;
}

int AnalyzeLightBSM::getBinNoV7(int nHadJets){
  int sBin=-100,m_i=0;
  if(BTags==0){
    if(nHadJets>=2 && nHadJets<=4)     { sBin=0;}
    else if(nHadJets==5 || nHadJets==6){ sBin=6;}
    else if(nHadJets>=7)               { sBin=11;}
  }
  else{
    if(nHadJets>=2 && nHadJets<=4)     { sBin=16;}
    else if(nHadJets==5 || nHadJets==6){ sBin=21;}
    else if(nHadJets>=7)               { sBin=26;}
  }
  if(sBin==0){
    for(int i=0;i<METBinLowEdgeV7_njLow.size()-1;i++){
      if(METBinLowEdgeV7_njLow[i]<99.99) continue;
      m_i++;
      if(MET >= METBinLowEdgeV7_njLow[i] && MET < METBinLowEdgeV7_njLow[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METBinLowEdgeV7_njLow[METBinLowEdgeV7_njLow.size()-1])  { sBin = 6         ;break; }
    }
  }
  else{
    for(int i=0;i<METBinLowEdgeV7.size()-1;i++){
      if(METBinLowEdgeV7[i]<99.99) continue;
      m_i++;
      if(MET >= METBinLowEdgeV7[i] && MET < METBinLowEdgeV7[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METBinLowEdgeV7[METBinLowEdgeV7.size()-1])  { sBin = sBin+5   ;break; }
    }
  }
  return sBin;
}

bool AnalyzeLightBSM::check_eMatchedtoGamma(){
  for(int i=0;i<Electrons->size();i++){
    for(int j=0;j<Photons->size();j++){
      if( (DeltaR( (*Electrons)[i].Eta(),(*Electrons)[i].Phi(),(*Photons)[j].Eta(),(*Photons)[j].Phi()) ) < 0.01){
	//cout<<(*Electrons)[i].Pt()<<" "<<(*Electrons)[i].Eta()<<" "<<(*Electrons)[i].Phi()<<" "<<(*Photons)[i].Pt()<<" "<<(*Photons)[i].Eta()<<" "<<(*Photons)[i].Phi()<<endl;
	//print(0);
	return true;
      }
    }
  }
  return false;
}

double AnalyzeLightBSM::getGendRLepPho(){//MC only
  TLorentzVector genPho1,genLep1;
  int leadGenPhoIdx=-100;
  for(int i=0;i<GenParticles->size();i++){
    if((*GenParticles)[i].Pt()!=0){
      if((abs((*GenParticles_PdgId)[i])==22) && ((abs((*GenParticles_ParentId)[i])<=25) || ((*GenParticles_ParentId)[i]==2212) ) && (*GenParticles_Status)[i]==1 ){
	if(genPho1.Pt() < (*GenParticles)[i].Pt()){
	  leadGenPhoIdx = i;
	  genPho1 = ((*GenParticles)[i]);
	}
      }
      if( (abs((*GenParticles_PdgId)[i])==11 || abs((*GenParticles_PdgId)[i])==13 || abs((*GenParticles_PdgId)[i])==15 ) && (abs((*GenParticles_ParentId)[i])<=25) && (abs((*GenParticles_ParentId)[i])!=15) ){
	if(genLep1.Pt() < ((*GenParticles)[i]).Pt()) genLep1 = ((*GenParticles)[i]);
      }
    }
  }//for
  if(genPho1.Pt() > 0. && genLep1.Pt() > 0.) return genLep1.DeltaR(genPho1);
  else return 1000.0;
}

void  AnalyzeLightBSM::findObjMatchedtoG(TLorentzVector bestPhoton){
  double dR=100,mindr=1000;
  int match=-100;
  for(int i=0;i<GenParticles->size();i++){
    if((*GenParticles)[i].Pt()!=0){
      if(dR > (bestPhoton.DeltaR((*GenParticles)[i])) && ( (bestPhoton.Pt()/(*GenParticles)[i].Pt() > 0.5) && (bestPhoton.Pt()/(*GenParticles)[i].Pt()<1.5)) ){
	dR=bestPhoton.DeltaR((*GenParticles)[i]);
	match=i;
      }
    }
  }
  
  if(dR<0.2){
    h_GmatchedObj->Fill(abs((*GenParticles_PdgId)[match]),wt);
    if(abs((*GenParticles_PdgId)[match])==22) {
      //if((*GenParticles_Status)[match]!=1) print(0);//cout<<" status:"<<(*GenParticles_Status)[match];
      h_PdgIdPhoParent->Fill(abs((*GenParticles_ParentId)[match]),wt);
    }
  }
  else{
    h_GmatchedObj->Fill(0.0,wt);
    h_PdgIdPhoParent->Fill(0.0,wt);
  }
}

void AnalyzeLightBSM::print(Long64_t jentry){
  //cout<<endl;
  TLorentzVector v1,photo;
  cout<<"------------------------------------------------------------"<<endl;
  //cout<<"bestPhoPt: "<<(*Photons)[bestPhotonIndxAmongPhotons].Pt()<<" Eta: "<<(*Photons)[bestPhotonIndxAmongPhotons].Eta()<<" phi: "<<(*Photons)[bestPhotonIndxAmongPhotons].Phi()<<" E: "<<(*Photons)[bestPhotonIndxAmongPhotons].Energy()<<endl;
  cout<<"MomMass:"<<SusyMotherMass<<" Kid Mass:"<<SusyLSPMass<<endl;
  for(int i=0;i<GenParticles->size();i++){
   
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<" E:"<<(*GenParticles)[i].Energy()<<" M:"<<(*GenParticles)[i].M()<<endl;
    //    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<</*"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<*/"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<" dR bestPho: "<<(*GenParticles)[i].DeltaR((*Photons)[bestPhotonIndxAmongPhotons])<<endl;

  }
  for(int i=0;i<Photons->size();i++){
    double dR=0;//DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*Photons)[i].Eta(),(*Photons)[i].Phi() );
    //cout<<jentry<<" i:"<<i<<" phoSize:"<<Photons->size()<<" Pt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" otherP:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" dR:"<<dR<<endl;
  }
  for(int i=0;i<Jets->size();i++){
    if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
      // cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<endl;
    }
  }
}

