#define ZGamma_cxx
#include "ZGamma.h"
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

  ZGamma ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void ZGamma::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  string s_data=data;
  bool mcSample=true;
  if(s_data=="Data") mcSample=false;    
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  
  int evtSurvived=0;
  // TFile *f_TF = new TFile("TF_CS_ZDYToLLG.root");
  // TH1D *h_TF=(TH1D*)f_TF->FindObjectAny("Ratio_NuNuToLL");
  bool do_prediction=0;
  //  cout<<"Doing prediction from ZToLL sample from file |"<<f_TF->GetName()<<"|?"<<do_prediction<<endl;
  
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
    wt=Weight*lumiInfb*1000.0;
    //cout<<CrossSection<<" NEvt:"<<NumEvents<<" Wt:"<<Weight<<endl;
    bool process=true;
    TLorentzVector genPho1,neutr,lep1,lep2,nu1,nu2;
    vector<TLorentzVector> genEle,genMu,genTau;
    bool genPromt1=false,phoFmLep=false;
    int leadGenPhoIdx=-100,nGenbs=0;
    vector<int> bParent;
    double genLeadPhoPt=0,genLeadPhoEta = 100.,genLeadPhoPhi=4.;
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if( abs((*GenParticles_PdgId)[i])==5 ){ 
	  nGenbs++;
	  bParent.push_back(abs((*GenParticles_ParentId)[i]));
	  h_bStatus->Fill((*GenParticles_Status)[i],wt);
	}
	if((abs((*GenParticles_PdgId)[i])==22) &&  (*GenParticles_Status)[i]==1 &&
	   (abs((*GenParticles_ParentId)[i])<=25 || ((*GenParticles_ParentId)[i]==2212)) &&
	   (genLeadPhoPt < (*GenParticles)[i].Pt())){
	  genLeadPhoPt = (*GenParticles)[i].Pt();
	  genLeadPhoEta = (*GenParticles)[i].Eta();
	  genLeadPhoPhi = (*GenParticles)[i].Phi();
	  leadGenPhoIdx = i;
	  if( (abs((*GenParticles_ParentId)[i])==11) || (abs((*GenParticles_ParentId)[i])==13) || (abs((*GenParticles_ParentId)[i])==15))
	    phoFmLep = true;
	  else phoFmLep = false;
	}
	if((abs((*GenParticles_PdgId)[i])==11) && (*GenParticles_Status)[i]==1 &&
	   (abs((*GenParticles_ParentId)[i])<=25 && 
	    abs((*GenParticles_ParentId)[i])!=13 && 
	    abs((*GenParticles_ParentId)[i])!=15 && 
	    abs((*GenParticles_ParentId)[i])!=22) &&
	   hasNeutrinoBrother(11,i))  genEle.push_back((*GenParticles)[i]);
	if((abs((*GenParticles_PdgId)[i])==13) && (*GenParticles_Status)[i]==1 &&
	   (abs((*GenParticles_ParentId)[i])<=25 && 
	    abs((*GenParticles_ParentId)[i])!=15 && 
	    abs((*GenParticles_ParentId)[i])!=22) &&
	   hasNeutrinoBrother(13,i))  genMu.push_back((*GenParticles)[i]);
	if((abs((*GenParticles_PdgId)[i])==15) &&
	   (abs((*GenParticles_ParentId)[i])<=25 && 
	    abs((*GenParticles_ParentId)[i])!=22))  genTau.push_back((*GenParticles)[i]);
      }
    }
    if(leadGenPhoIdx>=0) genPromt1=true;
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if( ( (abs((*GenParticles_PdgId)[i])==11) || (abs((*GenParticles_PdgId)[i])==13) || (abs((*GenParticles_PdgId)[i])==15) ) && (abs((*GenParticles_ParentId)[i])==23) ){
	  if(lep1.Pt() < 0.000001) lep1 = (*GenParticles)[i];
	  else lep2 = (*GenParticles)[i];
	}
	if( ( (abs((*GenParticles_PdgId)[i])==12) || (abs((*GenParticles_PdgId)[i])==14) || (abs((*GenParticles_PdgId)[i])==16) ) && (abs((*GenParticles_ParentId)[i])==23) ){
	  if(nu1.Pt() < 0.000001) nu1 = (*GenParticles)[i];
	  else nu2 = (*GenParticles)[i];
	}	
      }
    }
    double invMass = 0;
    if(lep1.Pt() > 0.0001 && lep2.Pt() > 0.0001) invMass = (lep1+lep2).M();
    if(nu1.Pt() > 0.0001 && nu2.Pt() > 0.0001) invMass = (nu1+nu2).M();
    //    if(invMass < 80 || invMass > 100) continue;
    if(genMu.size()!=1) continue;
    h_invMass->Fill(invMass,wt);
    evtSurvived++;
    // if(nGenbs==3){
    //   cout<<"-----------------------"<<endl;
    //   cout<<"Electrons"<<endl;
    //   for(int i=0;i<genEle.size();i++){
    // 	cout<<"Pt: "<<genEle[i].Pt()<<" eta:"<<genEle[i].Eta()<<" phi:"<<genEle[i].Phi()<<" M:"<<genEle[i].M()<<endl;
    //   }
    //   cout<<"Muons"<<endl;
    //   for(int i=0;i<genMu.size();i++){
    // 	cout<<"Pt: "<<genMu[i].Pt()<<" eta:"<<genMu[i].Eta()<<" phi:"<<genMu[i].Phi()<<" M:"<<genMu[i].M()<<endl;
    //   }
    //   cout<<"Taus"<<endl;
    //   for(int i=0;i<genTau.size();i++){
    // 	cout<<"Pt: "<<genTau[i].Pt()<<" eta:"<<genTau[i].Eta()<<" phi:"<<genTau[i].Phi()<<" M:"<<genTau[i].M()<<endl;
    //   }
    //   print(jentry);
    // }
    gendRLepPho=1000;
    gendRLepPho = getGendRLepPho();

    if(abs(genLeadPhoPhi) < 3.2){
      h_genLeadPhoPt->Fill(genLeadPhoPt,wt);
      if(genLeadPhoEta>=0) h_genLeadPhoEtaP->Fill(genLeadPhoEta,wt);
      else h_genLeadPhoEtaM->Fill(abs(genLeadPhoEta),wt);
      h_genLeadPhoPhi->Fill(genLeadPhoPhi,wt);
      h2_genLeadPhoPtEta->Fill(genLeadPhoPt,genLeadPhoEta,wt);
    }

    h_nGenEle->Fill(genEle.size(),wt);
    h_nGenMu->Fill(genMu.size(),wt);
    h_nGenTau->Fill(genTau.size(),wt);
    h_nLep->Fill(genEle.size()+genMu.size()+genTau.size(),wt);

    if(genEle.size()==1){
      h_genEle1Pt->Fill(genEle[0].Pt(),wt);
      if(genEle[0].Eta()>=0) h_genEle1EtaP->Fill(genEle[0].Eta(),wt);
      else h_genEle1EtaM->Fill(abs(genEle[0].Eta()),wt);
      h_genEle1Phi->Fill(genEle[0].Phi(),wt);
    }
    if(genMu.size()==1){
      h_genMu1Pt->Fill(genMu[0].Pt(),wt);
      if(genMu[0].Eta()>=0) h_genMu1EtaP->Fill(genMu[0].Eta(),wt);
      else h_genMu1EtaM->Fill(abs(genMu[0].Eta()),wt);
      h_genMu1Phi->Fill(genMu[0].Phi(),wt);
    }
    if(genTau.size()==1){
      h_genTau1Pt->Fill(genTau[0].Pt(),wt);
      if(genTau[0].Eta()>=0) h_genTau1EtaP->Fill(genTau[0].Eta(),wt);
      else h_genTau1EtaM->Fill(abs(genTau[0].Eta()),wt);
      h_genTau1Phi->Fill(genTau[0].Phi(),wt);
    }

    if(genLeadPhoPt > 100.){
      h_genLeadPhoPt_PtG100->Fill(genLeadPhoPt,wt);
      if(genLeadPhoEta>=0) h_genLeadPhoEtaP_PtG100->Fill(genLeadPhoEta,wt);
      else h_genLeadPhoEtaM_PtG100->Fill(abs(genLeadPhoEta),wt);
      h_genLeadPhoPhi_PtG100->Fill(genLeadPhoPhi,wt);
      h2_genLeadPhoPtEta_PtG100->Fill(genLeadPhoPt,genLeadPhoEta,wt);

      h_nGenEle_PtG100->Fill(genEle.size(),wt);
      h_nGenMu_PtG100->Fill(genMu.size(),wt);
      h_nGenTau_PtG100->Fill(genTau.size(),wt);

      h_nLep_PtG100->Fill(genEle.size()+genMu.size()+genTau.size(),wt);
      
      if(genEle.size()==1){
	h_genEle1Pt_PtG100->Fill(genEle[0].Pt(),wt);
	if(genEle[0].Eta()>=0) h_genEle1EtaP_PtG100->Fill(genEle[0].Eta(),wt);
	else h_genEle1EtaM_PtG100->Fill(abs(genEle[0].Eta()),wt);
	h_genEle1Phi_PtG100->Fill(genEle[0].Phi(),wt);
      }
      if(genMu.size()==1){
	h_genMu1Pt_PtG100->Fill(genMu[0].Pt(),wt);
	if(genMu[0].Eta()>=0) h_genMu1EtaP_PtG100->Fill(genMu[0].Eta(),wt);
	else h_genMu1EtaM_PtG100->Fill(abs(genMu[0].Eta()),wt);
	h_genMu1Phi_PtG100->Fill(genMu[0].Phi(),wt);
      }
      if(genTau.size()==1){
	h_genTau1Pt_PtG100->Fill(genTau[0].Pt(),wt);
	if(genTau[0].Eta()>=0) h_genTau1EtaP_PtG100->Fill(genTau[0].Eta(),wt);
	else h_genTau1EtaM_PtG100->Fill(abs(genTau[0].Eta()),wt);
	h_genTau1Phi_PtG100->Fill(genTau[0].Phi(),wt);
      }
    }

    if(leadGenPhoIdx>=0) h_leadGenPhoParent->Fill(abs((*GenParticles_ParentId)[leadGenPhoIdx]),wt);
    h_nGenbs->Fill(nGenbs,wt);
    for(int i=0;i<bParent.size();i++){
      if(bParent[i]==2212) bParent[i]=26;
      h_bParent_PdgId->Fill(bParent[i],wt);
    }
    h_intLumi->Fill(lumiInfb);
    h_MET->Fill(MET,wt);
    h_BTags->Fill(BTags,wt);
    h_HT->Fill(HT,wt);
    h_MHT->Fill(MHT,wt);
    h_nJets->Fill(NJets,wt);
    h_METvBin->Fill(MET,wt);
        
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
}


TLorentzVector ZGamma::getBestPhoton(){
  // bool passIDVeryLoose=false,passIDLoose=false,passIsoVeryLoose=false,passIsoLoose=false;
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  allBestPhotons.resize(0);
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( ((*Photons_fullID)[iPho]) && ((*Photons_hasPixelSeed)[iPho]<0.001) ) {
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

bool ZGamma::hasNeutrinoBrother(int lepPdg, int genIdx){
  lepPdg = abs(lepPdg);
  for(int i=0;i<GenParticles->size();i++){
    if((*GenParticles)[i].Pt()!=0){
      if(abs((*GenParticles_PdgId)[i])==(lepPdg+1) && 
	 abs((*GenParticles_ParentId)[i])<=25 && 
	 (*GenParticles_ParentIdx)[i]==(*GenParticles_ParentIdx)[genIdx]) return true;
    }
  }
  return false;
}

bool ZGamma::check_eMatchedtoGamma(){
  if(bestPhotonIndxAmongPhotons>=0){
    for(int i=0;i<Electrons->size();i++){
      if( (*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i] ) < 0.2){
	//	cout<<(*Electrons)[i].Pt()<<" "<<(*Electrons)[i].Eta()<<" "<<(*Electrons)[i].Phi()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Pt()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Eta()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Phi()<<" dR:"<<(*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i])<<endl;
	return true;
      }
    }
  }
  else
    return false;
}

bool ZGamma::check_muMatchedtoGamma(){
  if(bestPhotonIndxAmongPhotons>=0){
    for(int i=0;i<Muons->size();i++){
      if( (*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Muons)[i] ) < 0.2){
	return true;
      }
    }
  }
  else
    return false;
}

double ZGamma::getGendRLepPho(){//MC only
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

void  ZGamma::findObjMatchedtoG(TLorentzVector bestPhoton){
  
  double dR=100;
  int match=-100;
  for(int i=0;i<GenParticles->size();i++){
    if((*GenParticles)[i].Pt()!=0){
      if(i==0){dR=DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi() );}
      else if(dR > (DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi())) ){
	dR=(DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*GenParticles)[i].Eta(),(*GenParticles)[i].Phi()));
	match=i;
      }
    }
  }
  //cout<<"Pt: "<<bestPhoton.Pt()<<" Eta:"<<bestPhoton.Eta()<<" Phi:"<<bestPhoton.Phi()<<" PdgID:"<<(*GenParticles_PdgId)[match]<<" Pt:"<<(*GenParticles)[match].Pt()<<" Eta:"<<(*GenParticles)[match].Eta()<<" Phi:"<<(*GenParticles)[match].Phi()<<" parentId:"<<(*GenParticles_ParentId)[match]<<endl;
  if(dR<0.1){
    h_GmatchedObj->Fill(abs((*GenParticles_PdgId)[match]),wt);
    if(abs((*GenParticles_PdgId)[match])==22)  h_PdgIdPhoParent->Fill(abs((*GenParticles_ParentId)[match]),wt);
  }
  else{
    h_GmatchedObj->Fill(0.0,wt);
    h_PdgIdPhoParent->Fill(0.0,wt);
  }
  //find obj matched to muon
  /*
    dR=100;match=-100;
    if(Muons->size()==1){
    for(int i=0;i<GenParticles->size();i++){
    if((*GenParticles)[i].Pt()!=0){
    if(i==0){dR=(*GenParticles)[i].DeltaR((*Muons)[0]);}
    else if(dR > ((*GenParticles)[i].DeltaR( (*Muons)[0]) ) ){
    dR= (*GenParticles)[i].DeltaR((*Muons)[0]);
    match=i;
    }
    }
    }
    if(dR<0.1){
    h_MuMatchedObj->Fill(abs((*GenParticles_PdgId)[match]),wt);
    if(abs((*GenParticles_PdgId)[match])==13){
    h_PdgIdMuParent->Fill(abs((*GenParticles_ParentId)[match]),wt);
    // if(abs((*GenParticles_ParentId)[match])!=24 && abs((*GenParticles_ParentId)[match])!=15){
    //   print(0);
    //   cout<<(*GenParticles_PdgId)[match]<<" Mu Pt: "<<(*Muons)[0].Pt()<<" eta: "<<(*Muons)[0].Eta()<<" phi: "<<(*Muons)[0].Phi()<<endl<<abs((*GenParticles_ParentId)[match])<<endl;
    //	}
    }
    }
    else{
    h_MuMatchedObj->Fill(0.0,wt);
    h_PdgIdMuParent->Fill(0.0,wt);
    }
    }*/
  
}



void ZGamma::print(Long64_t jentry){
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

  for(int i=0;i<GenJets->size();i++){
    cout<<"GenJetsPt:"<<(*GenJets)[i].Pt()<<" eta:"<<(*GenJets)[i].Eta()<<" phi:"<<(*GenJets)[i].Phi()<<endl;
  }
  for(int i=0;i<Jets->size();i++){
    //  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
    if(Muons->size()==2){
      cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<" energy:"<<(*Jets)[i].Energy()<<" dR with Muon: "<<(*Jets)[i].DeltaR((*Muons)[0])<<endl;
    }
    if(Electrons->size()==2){
      cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<" energy:"<<(*Jets)[i].Energy()<<" dR with electron: "<<(*Jets)[i].DeltaR((*Electrons)[0])<<endl;
    }
  }

  for(int i=0;i<Muons->size();i++){
    if(i==0) cout<<"-------------------------------- Muons -------------------------------------------"<<endl;
    cout<<"MuonPt: "<<(*Muons)[i].Pt()<<" Eta: "<<(*Muons)[i].Eta()<<" Phi: "<<(*Muons)[i].Phi()<<" M: "<<(*Muons)[i].M()<<" E:"<<(*Muons)[i].Energy()<<endl;
  }
  for(int i=0;i<Electrons->size();i++){
    if(i==0) cout<<"-------------------------------- Electrons -------------------------------------------"<<endl;
    cout<<"ElectronPt: "<<(*Electrons)[i].Pt()<<" Eta: "<<(*Electrons)[i].Eta()<<" Phi: "<<(*Electrons)[i].Phi()<<" M: "<<(*Electrons)[i].M()<<" E:"<<(*Electrons)[i].Energy()<<endl;
  }
  cout<<"^^^^^^^^^^^^^^^^^^ Event ends ^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl<<endl;
}
