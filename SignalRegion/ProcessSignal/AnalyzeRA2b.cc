#define AnalyzeRA2b_cxx
#include "AnalyzeRA2b.h"
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

  AnalyzeRA2b ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void AnalyzeRA2b::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  string s_data=data;
 
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  
  int evtSurvived=0;
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
    
    wt=Weight*1000.0*lumiInfb;
    if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;//apply iso track veto
    //    if(!(HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0) ) continue;
   
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhotonIndxAmongPhotons<0) continue;

    bool bestPhoHasPxlSeed=true;
    if((*photon_hasPixelSeed)[bestPhotonIndxAmongPhotons]<0.001) bestPhoHasPxlSeed=false;
  
    bool eMatchedG=check_eMatchedtoGamma();
    bool process=true;

    int nHig=0,nHiggsKids=0;
    vector<TLorentzVector> NLSP_Pho;

    if(s_data=="TTJets_Tune"){
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      if( madHT>600 ){process=false;}
    }
    else if(s_data=="QCD"){
      process=false;
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      if( (*photon_nonPrompt)[bestPhotonIndxAmongPhotons] )
	process=true;
      else continue;
    }//QCD
    else if(s_data=="GJets"){
      process=false;
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      if( !((*photon_nonPrompt)[bestPhotonIndxAmongPhotons]) )
	process=true;
      else continue;
    }//GJets
    else if(s_data=="signalH"){
      wt=wt/4.0;//divived by 4 because, Frank Jensen(one who produced sample) has mulyiplied the weight by 4 for his convenience.
      TLorentzVector v1;
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      MET=METPt;   
      process=false;
      for(int i=0;i<GenParticles->size();i++){
	if( (abs((*GenParticles_PdgId)[i])==25) && (abs((*GenParticles_ParentId)[i])==1000023) ) {nHig++;}
      }
      for(int i=0;i<GenParticles->size();i++){
	if( (abs((*GenParticles_PdgId)[i])==22) && (abs((*GenParticles_ParentId)[i])==1000023) ) {
	  NLSP_Pho.push_back((*GenParticles)[i]);
	}
      }
      //      cout<<nHig;
      //      if(nHig!=1){continue;}
      if(nHig!=1){process=true;}
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
    
    if( eMatchedG || bestPhoHasPxlSeed || !process || (bestPhoton.Pt()<100) || (Electrons->size() > 0) || (Muons->size() > 0) )
      continue;

    bool hadJetID=true;
    int minDRindx=-100,photonMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0,mhtstar=0;
    TLorentzVector mhtstar_vec;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
	double dR=bestPhoton.DeltaR((*Jets)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    
    for(int i=0;i<Jets->size();i++){
      if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
	if( !(minDR < 0.3 && i==minDRindx) ){
	  hadJets.push_back((*Jets)[i]);
	  if(hadJetID) hadJetID=(*Jets_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) photonMatchingJetIndx=minDRindx;
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
    
    if( !((ST>700 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>180)) ) continue;
    process = process && !eMatchedG && !bestPhoHasPxlSeed && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && MET > 100 && dphi1 > 0.3 && dphi2 > 0.3;
    //    process = process && ST>500 && nHadJets>=2 && MET>100 && dphi1 > 0.3 && dphi2 > 0.3;
    //    process = process && NJets>=3 && MET>100;// && dphi1 > 0.3;
    //if(process){process=HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0 && minDR<0.3;}
    if(process){
      evtSurvived++;
      h_RunNum->Fill(RunNum);
      h2_mMomvsmKid->Fill(SusyMotherMass,SusyLSPMass,wt);
      // if(JetID && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && NVtx > 0){
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
	findObjMatchedtoG(bestPhoton);

	int searchRegion=0;
	if     (nHadJets >= 2 && nHadJets <= 4 ) searchRegion=1;
	else if(nHadJets == 5 || nHadJets == 6 ) searchRegion=2;
	else if(nHadJets >= 7                  ) searchRegion=3;

	h_STvBin->Fill(ST,wt);
	h_METvBin->Fill(MET,wt);
	h_BestPhotonPtvBin->Fill(bestPhoton.Pt(),wt);
	h_MHTvBin->Fill(MHT,wt);
	h_MET_Rall->Fill(MET,wt);
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_R[searchRegion-1]->Fill(MET,wt);
        }
        else cout<<"Event outside search region! ";

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
	
	h_dPhi_METBestPhoton->Fill(dphiG_MET);

	if(hadJets.size() > 0 ) h_dPhi_METjet1->Fill( dphi1 );
	if(hadJets.size() > 1 ) h_dPhi_METjet2->Fill( dphi2 );
	if(hadJets.size() > 2 ) h_dPhi_METjet3->Fill( dphi3 );
	if(hadJets.size() > 3 ) h_dPhi_METjet4->Fill( dphi4 );
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

	h_nHigs->Fill(nHig,wt);
	h_nPhoFmNLSP->Fill(NLSP_Pho.size(),wt);
	double mTPho = sqrt(2*(bestPhoton.Pt())*MET*(1-cos(DeltaPhi(METPhi,bestPhoton.Phi()))));
	h_mTPhoton->Fill(mTPho,wt);
	if(allBestPhotons.size()>1 && allBestPhotons[1].Pt()!=0)
	  h_RatioPhoPts->Fill(allBestPhotons[0].Pt()/allBestPhotons[1].Pt(),wt);
	if(allBestPhotons.size()>1){
	  h_dPhiPhotons->Fill(abs(bestPhoton.DeltaPhi(allBestPhotons[1])),wt);
	  h_dPhiPho2MET->Fill(abs(DeltaPhi(METPhi,allBestPhotons[1].Phi()) ),wt);
	  mTPho=sqrt( 2*(bestPhoton.Pt())*allBestPhotons[1].Pt()*(1-cos(bestPhoton.DeltaPhi(allBestPhotons[1]))) );
	  h_mT2Photons->Fill(mTPho,wt);
	  h2_Pho1PtVsPho2Pt->Fill(bestPhoton.Pt(),allBestPhotons[1].Pt(),wt);
	  TLorentzVector METz;
	  METz.SetPtEtaPhiE(MET,0.0,METPhi,0);
	  METz = METz - bestPhoton;
	  mTPho = sqrt(2*(allBestPhotons[1].Pt())*METz.Pt()*(1-cos(allBestPhotons[1].DeltaPhi(METz))));
	  h_mTMETPho2->Fill(mTPho,wt);
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
      //	cout<<bestPhoton.Phi()<<" KHT:"<<METPhi<<" dphi:"<<dphi<<endl;
      //cout<<Weight<<" ";
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


TLorentzVector AnalyzeRA2b::getBestPhoton(){
  bool passIDVeryLoose=false,passIDLoose=false,passIsoVeryLoose=false,passIsoLoose=false;
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  allBestPhotons.resize(0);
  for(int iPh=0;iPh<Photons->size();iPh++){
    // ---------------------------------- apply id cuts                         
    if(photon_isEB->at(iPh)){
      if(photon_hadTowOverEM->at(iPh) < 0.028 ){
	passIDVeryLoose=true;
      }//id criterea barrel (very loose)
      if(photon_hadTowOverEM->at(iPh) < 0.028 && photon_sigmaIetaIeta->at(iPh) < 0.0107){
	passIDLoose=true;
      }//id criterea barrel (loose)                    
    }
    else if(!photon_isEB->at(iPh)){
      if(photon_hadTowOverEM->at(iPh) < 0.093 ){
	passIDVeryLoose=true;
      }//id criterea endcap (very loose)                                                                                                                
      if(photon_hadTowOverEM->at(iPh) < 0.093 && photon_sigmaIetaIeta->at(iPh) < 0.0272){
	passIDLoose=true;
      }//id criterea endcap (loose)
    }// ---------------------------------- end id cuts                           

    // ----------------------------------- apply isolation cuts                      
    if(photon_isEB->at(iPh)){
      if(photon_pfNeutralIsoRhoCorr->at(iPh) <  (7.23 + TMath::Exp(0.0028*(Photons->at(iPh).Pt()+0.5408)))  && photon_pfGammaIsoRhoCorr->at(iPh) < ( 2.11 + 0.0014*(Photons->at(iPh).Pt())) ){
	passIsoVeryLoose=true;
      }//very loose, no charged iso cut
      if(photon_pfChargedIsoRhoCorr->at(iPh) <2.67 && photon_pfNeutralIsoRhoCorr->at(iPh) <  (7.23 + TMath::Exp(0.0028*(Photons->at(iPh).Pt()+0.5408)))  && photon_pfGammaIsoRhoCorr->at(iPh) < ( 2.11 + 0.0014*(Photons->at(iPh).Pt() )) ){
	passIsoLoose=true;
      }//loose isolation  
    }
    else if(!photon_isEB->at(iPh)){
      if(photon_pfNeutralIsoRhoCorr->at(iPh) <  (8.89 + 0.01725*(Photons->at(iPh).Pt()))  && photon_pfGammaIsoRhoCorr->at(iPh) < ( 3.09 + 0.0091*(Photons->at(iPh).Pt())) ){
	passIsoVeryLoose=true;
      }//very loose, no charged iso cut
      if(photon_pfChargedIsoRhoCorr->at(iPh) <1.79 && photon_pfNeutralIsoRhoCorr->at(iPh) <  (8.89 + 0.01725*(Photons->at(iPh).Pt()))  && photon_pfGammaIsoRhoCorr->at(iPh) < ( 3.09 + 0.0091*(Photons->at(iPh).Pt())) ){
	passIsoLoose=true;
      }//loose isolation 
    }// ---------------------------------- end iso cuts   
    
    if(passIDLoose && passIsoLoose){
      goodPho.push_back( (*Photons)[iPh] );
      goodPhoIndx.push_back( iPh );
      allBestPhotons.push_back( (*Photons)[iPh] );
    }
    
  }
  
  if(allBestPhotons.size()>1)
    sortTLorVec(&allBestPhotons);

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


bool AnalyzeRA2b::check_eMatchedtoGamma(){
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

void  AnalyzeRA2b::findObjMatchedtoG(TLorentzVector bestPhoton){
  double dR=100;
  int match=-100;
  for(int i=0;i<GenParticles->size();i++){
    if((*GenParticles)[i].Pt()!=0){
      if(dR > (DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi())) && ( (bestPhoton.Pt()/(*GenParticles)[match].Pt() < 0.5) || ((*GenParticles)[match].Pt()/bestPhoton.Pt())<0.5) ){
	dR=(DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi()));
	match=i;
      }
    }
  }
  //cout<<"Pt: "<<bestPhoton.Pt()<<" Eta:"<<bestPhoton.Eta()<<" Phi:"<<bestPhoton.Phi()<<" PdgID:"<<(*GenParticles_PdgId)[match]<<" Pt:"<<(*GenParticles)[match].Pt()<<" Eta:"<<(*GenParticles)[match].Eta()<<" Phi:"<<(*GenParticles)[match].Phi()<<" parentId:"<<(*GenParticles_ParentId)[match]<<endl;
  if(dR<0.1){
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

  // for(int i=0;i<GenParticles->size();i++){
  //   if((*GenParticles)[i].Pt()!=0){
  //     if(abs((*GenParticles_PdgId)[i])==22 && abs((*GenParticles_ParentId)[i])==24) print(0);
  //   }
  // }
}

void AnalyzeRA2b::print(Long64_t jentry){
  //cout<<endl;
  TLorentzVector v1,photo;
  for(int i=0;i<GenParticles->size();i++){
   
    // cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;

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


double AnalyzeRA2b::dR_PtEtaPhi(double pt1,double eta1,double phi1,double pt2,double eta2,double phi2){
  return sqrt( (pt1-pt2)*(pt1-pt2) + (DeltaR(eta1,phi1,eta2,phi2))*(DeltaR(eta1,phi1,eta2,phi2)) );
}


/*
  if(myjetid && minDR>=0.3 && process && !eMatchedG && !bestPhoHasPxlSeed && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && HT>500 && NJets>=2){
  //      cout<<"JetID:"<<JetID<<" METFilter:"<<METFilters<<endl;
  cout<<"-------------------------GenJets-------------------------------"<<endl;
  for(int i=0;i<GenJets->size();i++){
  if( ((*GenJets)[i].Pt() > MET_PtCut) && (abs((*GenJets)[i].Eta()) <= MET_EtaCut) ){
  cout<<i<<" Px:"<<(*GenJets)[i].Px()<<" Py:"<<(*GenJets)[i].Py()<<" Pz:"<<(*GenJets)[i].Pz()<<" E:"<<(*GenJets)[i].Energy()
  <<" Pt:"<<(*GenJets)[i].Pt()<<" Eta:"<<(*GenJets)[i].Eta()<<" Phi:"<<(*GenJets)[i].Phi()<<" dR:"<<bestPhoton.DeltaR((*GenJets)[i])<<endl;
  }
  }
  cout<<"-------------------------HadJets-------------------------------"<<endl;
  for(int i=0;i<hadJets.size();i++){
  cout<<i<<" Px:"<<hadJets[i].Px()<<" Py:"<<hadJets[i].Py()<<" Pz:"<<hadJets[i].Pz()<<" E:"<<hadJets[i].Energy()
  <<" Pt:"<<hadJets[i].Pt()<<" Eta:"<<hadJets[i].Eta()<<" Phi:"<<hadJets[i].Phi()<<endl;
  }
  cout<<"mht:"<<mht<<" "<<mhtstar_vec.Eta()<<" "<<mhtstar_vec.Phi()<<endl;
  cout<<"-------------------------Jets-------------------------------"<<endl;
  TLorentzVector v2_mht;
  for(int i=0;i<Jets->size();i++)
  if( ((*Jets)[i].Pt() > MET_PtCut) && (abs((*Jets)[i].Eta()) <= MET_EtaCut) ){
  v2_mht=v2_mht-(*Jets)[i];
  cout<<i<<" Px:"<<(*Jets)[i].Px()<<" Py:"<<(*Jets)[i].Py()<<" Pz:"<<(*Jets)[i].Pz()<<" E:"<<(*Jets)[i].Energy()
  <<" Pt:"<<(*Jets)[i].Pt()<<" Eta:"<<(*Jets)[i].Eta()<<" Phi:"<<(*Jets)[i].Phi()<<" dR:"<<bestPhoton.DeltaR((*Jets)[i])<<" id:"<<(*Jets_ID)[i]<<endl;
  }
  cout<<"MET_Jets:"<<v2_mht.Pt()<<" "<<v2_mht.Eta()<<" "<<v2_mht.Phi()<<endl<<"GenMET:"<<GenMET<<" MET:"<<MET<<" GenMET:"<<GenMET<<endl;
  cout<<"-------------------------BestPhoton-------------------------------"<<endl;
  cout<<"Photon:"<<bestPhoton.Px()<<" "<<bestPhoton.Py()<<" "<<bestPhoton.Pz()<<" "<<bestPhoton.Energy()<<endl;
  cout<<"Phootn PtEtaPhi:"<<bestPhoton.Pt()<<" "<<bestPhoton.Eta()<<" "<<bestPhoton.Phi()<<endl;
  cout<<"BadChargedCandidateFilter:"<<BadChargedCandidateFilter<<" BadPFMuonFilter:"<<BadPFMuonFilter<<" CSCTightHaloFilter:"<<CSCTightHaloFilter<<" EcalDeadCellTriggerPrimitiveFilter:"<<EcalDeadCellTriggerPrimitiveFilter<<" eeBadScFilter:"<<eeBadScFilter<<" globalTightHalo2016Filter:"<<globalTightHalo2016Filter<<" HBHEIsoNoiseFilter:"<<HBHEIsoNoiseFilter<<" HBHENoiseFilter:"<<HBHENoiseFilter<<endl;
  print(jentry);
  }
*/


/*
  if((abs((*GenJets)[0].Eta()) <= MHT_EtaCut) && (*GenJets)[0].Pt()>30 ){
  double dR=100000;int match=-100;
  for(int i=0;i<GenParticles->size();i++){
  if((*GenParticles)[i].Pt()>5){
  //if(dR>dR_PtEtaPhi((*GenJets)[0].Pt(),(*GenJets)[0].Eta(),(*GenJets)[0].Phi(),(*GenParticles)[i].Pt(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi() )){
  // 	dR=dR_PtEtaPhi((*GenJets)[0].Pt(),(*GenJets)[0].Eta(),(*GenJets)[0].Phi(),(*GenParticles)[i].Pt(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi());
  // 	match=i;
  // }
  //if((*GenParticles_PdgId)[i]==22 && (*GenParticles_ParentIdx)[i]==0) h2_PtGenReco->Fill( (*GenParticles)[i].Pt(),bestPhoton.Pt());
  int indx1=0;
  if(DeltaR((*GenJets)[0].Eta(),(*GenJets)[0].Phi(),bestPhoton.Eta(),bestPhoton.Phi())<DeltaR((*GenJets)[1].Eta(),(*GenJets)[1].Phi(),bestPhoton.Eta(),bestPhoton.Phi())){indx1=1;}
  h_jetPt[0]->Fill((*GenJets)[indx1].Pt());
  if(dR>DeltaR((*GenJets)[indx1].Eta(),(*GenJets)[indx1].Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi() )){
  dR=DeltaR((*GenJets)[indx1].Eta(),(*GenJets)[indx1].Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi() );
  match=i;
  }
  }
  }
  for(int i=0;i<Jets->size();i++){
  if( ((*Jets)[i].Pt() > MET_PtCut) && (abs((*Jets)[i].Eta()) <= MET_EtaCut) ){
  for(int j=0;j<GenJets->size();j++){
  if((DeltaR( (*Jets)[i].Eta(),(*Jets)[i].Phi(),(*GenJets)[j].Eta(),(*GenJets)[j].Phi()))< 0.3){}
  //		if((DeltaR( (*Jets)[i].Eta(),(*Jets)[i].Phi(),(*GenJets)[j].Eta(),(*GenJets)[j].Phi()))< 0.8 && bestPhoton.DeltaR((*GenJets)[i])<0.3)
  //		  h2_PtGenReco->Fill( (*GenJets)[j].Pt(),(*Jets)[i].Pt());
  }
  }
  }
  //cout<<(*GenJets)[0].Pt()<<" "<<(*GenJets)[0].Eta()<<" "<<(*GenJets)[0].Phi()<<" "<<match<<endl;
  //	  if(dR<0.4) {h_temp->Fill(abs((*GenParticles_PdgId)[match]));}
  //cout<<"-----------------------------------------------------------"<<endl;
  // cout<<hadJets[0].Pt()<<" "<<hadJets[0].Eta()<<" "<<hadJets[0].Phi()<<endl;
  // print(jentry);
  }
*/


/*    if( minDR<0.3 && ((*Jets)[photonMatchingJetIndx].Pt()) > 1.2*(bestPhoton.Pt()) ){ 
      cout<<"JetPt: "<<(*Jets)[photonMatchingJetIndx].Pt()<<" Eta:"<<(*Jets)[photonMatchingJetIndx].Eta()<<" Phi:"<<(*Jets)[photonMatchingJetIndx].Phi()<<" E:"<<(*Jets)[photonMatchingJetIndx].Energy()<<endl
	  <<"JetPx: "<<(*Jets)[photonMatchingJetIndx].Px()<<" Py:"<<(*Jets)[photonMatchingJetIndx].Py()<<" Pz:"<<(*Jets)[photonMatchingJetIndx].Pz()<<endl;
      cout<<"PhotonPt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" E:"<<bestPhoton.Energy()<<endl
	  <<"PhotonPx:"<<bestPhoton.Px()<<" Py:"<<bestPhoton.Py()<<" Pz:"<<bestPhoton.Pz()<<" E:"<<bestPhoton.Energy()<<endl;
    }*/
/*
//	cout<<"JetStarPt: "<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Pt()<<" Eta:"<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Eta()<<" Phi:"<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Phi()<<" E:"<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Energy()<<endl
//<<"JetStarPx: "<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Px()<<" Py:"<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Py()<<" Pz:"<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Pz()<<endl;
if(((*Jets)[photonMatchingJetIndx] - bestPhoton).Pt()<20.0) cout<<cout<<"JetStarPt: "<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Pt()<<" Eta:"<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Eta()<<" Phi:"<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Phi()<<" E:"<<((*Jets)[photonMatchingJetIndx] - bestPhoton).Energy()<<endl;*/
