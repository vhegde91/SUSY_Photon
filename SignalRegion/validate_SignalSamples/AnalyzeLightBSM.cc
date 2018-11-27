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
#include <TRandom.h>

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

  string s_data=data;
 
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  
  int evtSurvived=0,minbtags=0;
  double GravitinoMass=1.0;
  TString textMass="m(#tilde{G})"+to_string(GravitinoMass)+"GeV";
  if(GravitinoMass==1)  textMass= "m(#tilde{G})=1GeV";
  else if(GravitinoMass==0.001)  textMass= "m(#tilde{G})=1MeV";
  else if(GravitinoMass==0.000001)  textMass= "m(#tilde{G})=1keV";

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
    wt=Weight*1000.0*lumiInfb;
    h2_mGlmNLSP->Fill(SusyMotherMass,SusyLSPMass);
    //    cout<<"Mom:"<<SusyMotherMass<<" LSP:"<<SusyLSPMass<<endl;
    //    continue;
    //===============================================================
    int gl1Idx=-100,gl2Idx=-100,nGl=0,nPhoFmLSP=0,nZFmLSP=0,nZKids=0,nGenJets=0;
    TLorentzVector gl1,gl2,pholsp1,pholsp2,zlsp1,zlsp2,gravitinos;
    double nlsp1M=-100,nlsp2M=-100,genHT=0;
    vector<TLorentzVector> NLSPs, NLSPkids,genPhotons;
    vector<int> NLSPindx,NLSPkidId;
    
    for(int i=0;i<GenParticles->size();i++){
      if(abs((*GenParticles_PdgId)[i]) == 1000021){
	nGl++;
	if(gl1Idx < 0 ) gl1Idx = i;
	if(gl2Idx < 0 && gl1Idx != i ) gl2Idx = i;
      }
      if(abs((*GenParticles_PdgId)[i])==1000022) gravitinos = gravitinos + (*GenParticles)[i];
      if(abs((*GenParticles_PdgId)[i])==22 && abs((*GenParticles_ParentId)[i])==1000023) genPhotons.push_back((*GenParticles)[i]);
      if(abs((*GenParticles_PdgId)[i])==1000023) h_pNLSP->Fill((*GenParticles)[i].P(),wt);
      //---------------------------------
      if(abs((*GenParticles_PdgId)[i])==1000023){
	NLSPs.push_back((*GenParticles)[i]);
	if(NLSPs.size()==1){ dchain[0].idxGen_ = i; dchain[0].NLSPs_ = (*GenParticles)[i];}
	if(NLSPs.size()==2){ dchain[1].idxGen_ = i; dchain[1].NLSPs_ = (*GenParticles)[i];}
      }
      if(abs((*GenParticles_ParentId)[i])==1000023){
	NLSPkids.push_back((*GenParticles)[i]);
	NLSPindx.push_back((*GenParticles_ParentIdx)[i]);
	NLSPkidId.push_back((*GenParticles_PdgId)[i]);
      }
    }
    h_METGravitino->Fill(gravitinos.Pt(),wt);
    for(int i=0;i<genPhotons.size();i++) h_GenphoPt->Fill(genPhotons[i].Pt(),wt);

    if(NLSPs.size()!=2) cout<<"!!!! no. of NLSPs is not 2. Go ahead at your own risk!!!!"<<NLSPs.size()<<endl;
    for(int i=0;i<NLSPkids.size();i++){
      if(dchain[0].idxGen_ == NLSPindx[i]){
	if(NLSPkidId[i]==1000022) dchain[0].grav_ = NLSPkids[i];
	else{
	  dchain[0].bosonId_ = NLSPkidId[i];
	  dchain[0].boson_ = NLSPkids[i];
	}
      }
      else if(dchain[1].idxGen_ == NLSPindx[i]){
	if(NLSPkidId[i]==1000022) dchain[1].grav_ = NLSPkids[i];
	else{
	  dchain[1].bosonId_ = NLSPkidId[i];
	  dchain[1].boson_ = NLSPkids[i];
	}
      }
    }//got a struct for decay chain. Each element of struct is one decay chain.
    //   cout<<"-----------------------"<<endl;
    TString massFrameType;
    calKinWithDiffGravitinoMass(GravitinoMass);
    TLorentzVector vec,newGravMET;
    for(int i=0;i<2;i++){
      h2_pNLSP->Fill(dchain[i].NLSPs_.P(),"Default",wt);
      h2_pxNLSP->Fill(dchain[i].NLSPs_.Px(),"Default",wt);
      h2_pyNLSP->Fill(dchain[i].NLSPs_.Py(),"Default",wt);
      h2_pzNLSP->Fill(dchain[i].NLSPs_.Pz(),"Default",wt);
      h2_ENLSP->Fill(dchain[i].NLSPs_.E(),"Default",wt);
      h2_MNLSP->Fill(dchain[i].NLSPs_.M(),"Default",wt);

      vec = dchain[i].NLSPs_; 
      vec.Boost(-dchain[i].boost_);
      h2_pNLSP->Fill(vec.P(),"NLSP frame",wt);
      h2_pxNLSP->Fill(vec.Px(),"NLSP frame",wt);
      h2_pyNLSP->Fill(vec.Py(),"NLSP frame",wt);
      h2_pzNLSP->Fill(vec.Pz(),"NLSP frame",wt);
      h2_ENLSP->Fill(vec.E(),"NLSP frame",wt);
      h2_MNLSP->Fill(vec.M(),"NLSP frame",wt);
      //.................Photon.....................
      if(abs(dchain[i].bosonId_) == 22){
	h2_pPhoton->Fill(dchain[i].boson_.P(),"Default",wt);
	h2_pxPhoton->Fill(dchain[i].boson_.Px(),"Default",wt);
	h2_pyPhoton->Fill(dchain[i].boson_.Py(),"Default",wt);
	h2_pzPhoton->Fill(dchain[i].boson_.Pz(),"Default",wt);
	h2_EPhoton->Fill(dchain[i].boson_.E(),"Default",wt);
	h2_MPhoton->Fill(dchain[i].boson_.M(),"Default",wt);
	h2_ptPhoton->Fill(dchain[i].boson_.Pt(),"Default",wt);

	vec = dchain[i].boson_; 
	vec.Boost(-dchain[i].boost_);
	h2_pPhoton->Fill(vec.P(),"NLSP frame",wt);
	h2_pxPhoton->Fill(vec.Px(),"NLSP frame",wt);
	h2_pyPhoton->Fill(vec.Py(),"NLSP frame",wt);
	h2_pzPhoton->Fill(vec.Pz(),"NLSP frame",wt);
	h2_EPhoton->Fill(vec.E(),"NLSP frame",wt);
	h2_MPhoton->Fill(vec.M(),"NLSP frame",wt);
	h2_ptPhoton->Fill(vec.Pt(),"NLSP frame",wt);
	
	massFrameType = "NLSP frame, "+textMass;
	h2_pPhoton->Fill(dchain[i].newBoson_.P(),massFrameType,wt);
	h2_pxPhoton->Fill(dchain[i].newBoson_.Px(),massFrameType,wt);
	h2_pyPhoton->Fill(dchain[i].newBoson_.Py(),massFrameType,wt);
	h2_pzPhoton->Fill(dchain[i].newBoson_.Pz(),massFrameType,wt);
	h2_EPhoton->Fill(dchain[i].newBoson_.E(),massFrameType,wt);
	h2_MPhoton->Fill(dchain[i].newBoson_.M(),massFrameType,wt);
	h2_ptPhoton->Fill(dchain[i].newBoson_.Pt(),massFrameType,wt);

	vec = dchain[i].newBoson_; 
	vec.Boost(dchain[i].boost_);
	massFrameType = "Lab frame, "+textMass;
	h2_pPhoton->Fill(vec.P(),massFrameType,wt);
	h2_pxPhoton->Fill(vec.Px(),massFrameType,wt);
	h2_pyPhoton->Fill(vec.Py(),massFrameType,wt);
	h2_pzPhoton->Fill(vec.Pz(),massFrameType,wt);
	h2_EPhoton->Fill(vec.E(),massFrameType,wt);
	h2_MPhoton->Fill(vec.M(),massFrameType,wt);	
	h2_ptPhoton->Fill(vec.Pt(),massFrameType,wt);	
      }
      //................H/Z......................
      if(abs(dchain[i].bosonId_) == 23 || abs(dchain[i].bosonId_) == 25){
	h2_pBoson->Fill(dchain[i].boson_.P(),"Default",wt);
	h2_pxBoson->Fill(dchain[i].boson_.Px(),"Default",wt);
	h2_pyBoson->Fill(dchain[i].boson_.Py(),"Default",wt);
	h2_pzBoson->Fill(dchain[i].boson_.Pz(),"Default",wt);
	h2_EBoson->Fill(dchain[i].boson_.E(),"Default",wt);
	h2_MBoson->Fill(dchain[i].boson_.M(),"Default",wt);
	h2_ptBoson->Fill(dchain[i].boson_.Pt(),"Default",wt);

	vec = dchain[i].boson_; 
	vec.Boost(-dchain[i].boost_);
	h2_pBoson->Fill(vec.P(),"NLSP frame",wt);
	h2_pxBoson->Fill(vec.Px(),"NLSP frame",wt);
	h2_pyBoson->Fill(vec.Py(),"NLSP frame",wt);
	h2_pzBoson->Fill(vec.Pz(),"NLSP frame",wt);
	h2_EBoson->Fill(vec.E(),"NLSP frame",wt);
	h2_MBoson->Fill(vec.M(),"NLSP frame",wt);
	h2_ptBoson->Fill(vec.Pt(),"NLSP frame",wt);
	
	massFrameType = "NLSP frame, "+textMass;
	h2_pBoson->Fill(dchain[i].newBoson_.P(),massFrameType,wt);
	h2_pxBoson->Fill(dchain[i].newBoson_.Px(),massFrameType,wt);
	h2_pyBoson->Fill(dchain[i].newBoson_.Py(),massFrameType,wt);
	h2_pzBoson->Fill(dchain[i].newBoson_.Pz(),massFrameType,wt);
	h2_EBoson->Fill(dchain[i].newBoson_.E(),massFrameType,wt);
	h2_MBoson->Fill(dchain[i].newBoson_.M(),massFrameType,wt);
	h2_ptBoson->Fill(dchain[i].newBoson_.Pt(),massFrameType,wt);

	vec = dchain[i].newBoson_; 
	vec.Boost(dchain[i].boost_);
	massFrameType = "Lab frame, "+textMass;
	h2_pBoson->Fill(vec.P(),massFrameType,wt);
	h2_pxBoson->Fill(vec.Px(),massFrameType,wt);
	h2_pyBoson->Fill(vec.Py(),massFrameType,wt);
	h2_pzBoson->Fill(vec.Pz(),massFrameType,wt);
	h2_EBoson->Fill(vec.E(),massFrameType,wt);
	h2_MBoson->Fill(vec.M(),massFrameType,wt);	
	h2_ptBoson->Fill(vec.Pt(),massFrameType,wt);	
      }
      //................Gravitino......................
      h2_pLSP->Fill(dchain[i].grav_.P(),"Default",wt);
      h2_pxLSP->Fill(dchain[i].grav_.Px(),"Default",wt);
      h2_pyLSP->Fill(dchain[i].grav_.Py(),"Default",wt);
      h2_pzLSP->Fill(dchain[i].grav_.Pz(),"Default",wt);
      h2_ELSP->Fill(dchain[i].grav_.E(),"Default",wt);
      h2_MLSP->Fill(dchain[i].grav_.M(),"Default",wt);
      h2_ptLSP->Fill(dchain[i].grav_.Pt(),"Default",wt);

      vec = dchain[i].grav_; 
      vec.Boost(-dchain[i].boost_);
      h2_pLSP->Fill(vec.P(),"NLSP frame",wt);
      h2_pxLSP->Fill(vec.Px(),"NLSP frame",wt);
      h2_pyLSP->Fill(vec.Py(),"NLSP frame",wt);
      h2_pzLSP->Fill(vec.Pz(),"NLSP frame",wt);
      h2_ELSP->Fill(vec.E(),"NLSP frame",wt);
      h2_MLSP->Fill(vec.M(),"NLSP frame",wt);
      h2_ptLSP->Fill(vec.Pt(),"NLSP frame",wt);
	
      massFrameType = "NLSP frame, "+textMass;
      h2_pLSP->Fill(dchain[i].newGrav_.P(),massFrameType,wt);
      h2_pxLSP->Fill(dchain[i].newGrav_.Px(),massFrameType,wt);
      h2_pyLSP->Fill(dchain[i].newGrav_.Py(),massFrameType,wt);
      h2_pzLSP->Fill(dchain[i].newGrav_.Pz(),massFrameType,wt);
      h2_ELSP->Fill(dchain[i].newGrav_.E(),massFrameType,wt);
      h2_MLSP->Fill(dchain[i].newGrav_.M(),massFrameType,wt);
      h2_ptLSP->Fill(dchain[i].newGrav_.Pt(),massFrameType,wt);

      vec = dchain[i].newGrav_; 
      vec.Boost(dchain[i].boost_);
      massFrameType = "Lab frame, "+textMass;
      h2_pLSP->Fill(vec.P(),massFrameType,wt);
      h2_pxLSP->Fill(vec.Px(),massFrameType,wt);
      h2_pyLSP->Fill(vec.Py(),massFrameType,wt);
      h2_pzLSP->Fill(vec.Pz(),massFrameType,wt);
      h2_ELSP->Fill(vec.E(),massFrameType,wt);
      h2_MLSP->Fill(vec.M(),massFrameType,wt);	
      h2_ptLSP->Fill(vec.Pt(),massFrameType,wt);
      // cout<<"vec: px:"<<vec.Px()<<" py:"<<vec.Py()<<" pz:"<<vec.Pz()<<" M:"<<vec.M()<<endl;
      // cout<<"newGravMET: px:"<<newGravMET.Px()<<" py:"<<newGravMET.Py()<<" pz:"<<newGravMET.Pz()<<" M:"<<newGravMET.M()<<endl;
      newGravMET = newGravMET + vec;
    }
    h2_METLSP->Fill( (dchain[0].grav_+dchain[1].grav_).Pt(),"Default",wt);
    h2_METLSP->Fill(0.," ",0.);     h2_METLSP->Fill(0.,"  ",0.);
    h2_METLSP->Fill(newGravMET.Pt(),"Lab frame, "+textMass,wt);
    // cout<<"GravMET vec, px:"<<newGravMET.Px()<<" py:"<<newGravMET.Py()<<" pz:"<<newGravMET.Pz()<<endl;
    
    //-------------------------------------------
    if(nGl!=2) cout<<"!!!!!!!!!! nGl !!!!!!!!!!!"<<nGl<<endl;
    else{
      h_mGl->Fill((*GenParticles)[gl1Idx].M());
      h_mGl->Fill((*GenParticles)[gl2Idx].M());
      h_GlPt->Fill((*GenParticles)[gl1Idx].Pt());
      h_GlPt->Fill((*GenParticles)[gl2Idx].Pt());

      for(int i=0;i<GenParticles->size();i++){
	if((*GenParticles_ParentIdx)[i] == gl1Idx){//if gluino1 daughters
	  gl1 = gl1 + (*GenParticles)[i];
	  if(abs((*GenParticles_PdgId)[i]) == 1000023){
	    nlsp1M = (*GenParticles)[i].M();
	    h_mNLSP->Fill((*GenParticles)[i].M());
	    h_NLSP_Pt->Fill((*GenParticles)[i].Pt());
	  }
	}
	if( (abs((*GenParticles_ParentId)[i]) == 1000023) && ((*GenParticles_ParentIdx)[(*GenParticles_ParentIdx)[i]] == gl1Idx) ){//if NLSP daughters and gluino1 grad-daughters
	  if(abs((*GenParticles_PdgId)[i]) == 1000022){
	    h_mLSP->Fill((*GenParticles)[i].M());
	    pholsp1 = pholsp1 + (*GenParticles)[i];
	    zlsp1   = zlsp1   + (*GenParticles)[i];
	  }
	  if(abs((*GenParticles_PdgId)[i]) == 22 ){ nPhoFmLSP++;pholsp1 = pholsp1 + (*GenParticles)[i];}
	  if(abs((*GenParticles_PdgId)[i]) == 23 ){ nZFmLSP++;  zlsp1   = zlsp1   + (*GenParticles)[i];}
	}//gluino 1
	if((*GenParticles_ParentIdx)[i] == gl2Idx){//if gluino2 daughters
	  gl2 = gl2 + (*GenParticles)[i];
	  if(abs((*GenParticles_PdgId)[i]) == 1000023){
	    nlsp2M = (*GenParticles)[i].M();
	    h_mNLSP->Fill((*GenParticles)[i].M());
	    h_NLSP_Pt->Fill((*GenParticles)[i].Pt());
	  }
	}
	if( (abs((*GenParticles_ParentId)[i]) == 1000023) && ((*GenParticles_ParentIdx)[(*GenParticles_ParentIdx)[i]] == gl2Idx) ){//if NLSP daughters and gluino2 grad-daughters
	  if(abs((*GenParticles_PdgId)[i]) == 1000022){
	    h_mLSP->Fill((*GenParticles)[i].M());
	    pholsp2 = pholsp2 + (*GenParticles)[i];
	    zlsp2   = zlsp2   + (*GenParticles)[i];
	  }
	  if(abs((*GenParticles_PdgId)[i]) == 22 ){ nPhoFmLSP++;pholsp2 = pholsp2 + (*GenParticles)[i];}
	  if(abs((*GenParticles_PdgId)[i]) == 23 ){ nZFmLSP++;  zlsp2   = zlsp2   + (*GenParticles)[i];}
	}//gluino 2

      }//for GenParicles
    }//else
    h_nPhoFmNLSP->Fill(nPhoFmLSP);
    h_nZFmNLSP->Fill(nZFmLSP);
    h_invMassGl->Fill(gl1.M());
    h_invMassGl->Fill(gl2.M());
    h_invMassPhoLSP->Fill(pholsp1.M());
    h_invMassPhoLSP->Fill(pholsp2.M());
    h_invMassZLSP->Fill(zlsp1.M());
    h_invMassZLSP->Fill(zlsp2.M());
    // h2_mGlmNLSP->Fill((*GenParticles)[gl1Idx].M(),nlsp1M);
    // h2_mGlmNLSP->Fill((*GenParticles)[gl2Idx].M(),nlsp2M);
    
    if((dchain[0].bosonId_==22) && (dchain[1].bosonId_==22)){
      h2_dPhiPhosGenMET->Fill(abs((dchain[0].boson_).DeltaPhi((dchain[1].boson_))),GenMET,wt);
      if((dchain[0].boson_).Pt() > (dchain[1].boson_).Pt()){
	h2_dPhiPho1GenMET->Fill(abs(DeltaPhi((dchain[0].boson_).Phi(),GenMETPhi)),GenMET,wt);
	h2_dPhiPho2GenMET->Fill(abs(DeltaPhi((dchain[1].boson_).Phi(),GenMETPhi)),GenMET,wt);
	h2_Pho1PtGenMET->Fill((dchain[0].boson_).Pt(),GenMET,wt);
	h2_Pho2PtGenMET->Fill((dchain[1].boson_).Pt(),GenMET,wt);
	h2_PhoPtRatioGenMET->Fill( ((dchain[1].boson_).Pt())/((dchain[0].boson_).Pt()), GenMET,wt);
      }
      else{
	h2_dPhiPho1GenMET->Fill(abs(DeltaPhi((dchain[1].boson_).Phi(),GenMETPhi)),GenMET,wt);
	h2_dPhiPho2GenMET->Fill(abs(DeltaPhi((dchain[0].boson_).Phi(),GenMETPhi)),GenMET,wt);
	h2_Pho1PtGenMET->Fill((dchain[1].boson_).Pt(),GenMET,wt);
	h2_Pho2PtGenMET->Fill((dchain[0].boson_).Pt(),GenMET,wt);
	h2_PhoPtRatioGenMET->Fill( ((dchain[0].boson_).Pt())/((dchain[1].boson_).Pt()), GenMET,wt);
      }
    }
    h_xsec->Fill(1.,CrossSection);
    h_NumEvt->Fill(1.,NumEvents);
    h_Wt->Fill(1.,Weight);
    h_GenMET->Fill(GenMET,wt);
    // if(genPhotons.size()>=2 && genPhotons[0].Pt() > genPhotons[1].Pt() ) h_LeadgenPhoPt->Fill(genPhotons[0].Pt(),wt);
    // else if(genPhotons.size()>=2 && genPhotons[1].Pt() > genPhotons[0].Pt() ) h_LeadgenPhoPt->Fill(genPhotons[1].Pt(),wt);
    // else if(genPhotons.size()==1) h_LeadgenPhoPt->Fill(genPhotons[0].Pt(),wt);
    sortTLorVec(&genPhotons);
    if(genPhotons.size()!=0)
      h_LeadgenPhoPt->Fill(genPhotons[0].Pt(),wt);

    for(int i=0;i<GenParticles->size();i++){
      if(abs((*GenParticles_ParentId)[i])==23) h_ZKids->Fill(abs((*GenParticles_PdgId)[i]));
    }
    for(int i=0;i<GenJets->size();i++){
      if((*GenJets)[i].Pt()>30 && abs((*GenJets)[i].Eta()) < 2.4){
	nGenJets++;
	genHT = genHT+(*GenJets)[i].Pt();
      }
    }
    h_nGenJets->Fill(nGenJets,wt);
    h_genHT->Fill(genHT,wt);

    //===============================================================

    if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;//apply iso track veto
    if(!(HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0) ) continue;

    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhotonIndxAmongPhotons<0) continue;

    bool bestPhoHasPxlSeed=true;
    if((*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons]<0.001) bestPhoHasPxlSeed=false;
  
    bool eMatchedG=check_eMatchedtoGamma();
    bool process=true;
    if(allBestPhotons.size()<2) continue;
    if(s_data=="genprompt" || s_data=="GJets" || s_data=="TTG" || s_data=="WG" || s_data=="ZG"){
      if(jentry==0){cout<<"**********processing "<<s_data<<" with prompt Gen photon"<<endl;}
      if(hasGenPromptPhoton)
	process=true;
      else continue;
    }//Gen prompt
    else if(s_data=="gennonprompt" || s_data=="QCD" || s_data=="TTJets" || s_data=="WJets" || s_data=="ZJets"){
      if(jentry==0){cout<<"**********processing "<<s_data<<" w/o prompt gen photon"<<endl;}
      if(!hasGenPromptPhoton)
	process=true;
      else continue;
    }//Gen non-prompt
    else if(s_data=="signalH"){
      // wt=wt/4.0;//divived by 4 because, Frank Jensen(one who produced sample) has mulyiplied the weight by 4 for his convenience.
      TLorentzVector v1;
      int nHig=0,nHiggsKids=0;
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      //  process=false;
      for(int i=0;i<GenParticles->size();i++){
	if( (abs((*GenParticles_PdgId)[i])==25) && (abs((*GenParticles_ParentId)[i])==1000023) ) {nHig++;}
      }/*
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
      }    */
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
    process = process && !eMatchedG && !bestPhoHasPxlSeed && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && MET > 100 && dphi1 > 0.3 && dphi2 > 0.3 && BTags>=minbtags;
    
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
	h_madHT->Fill(madHT,wt);
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
	
	h_dPhi_METBestPhoton->Fill(dphiG_MET,wt);

	if(hadJets.size() > 0 ) h_dPhi_METjet1->Fill( dphi1,wt );
	if(hadJets.size() > 1 ) h_dPhi_METjet2->Fill( dphi2,wt );
	if(hadJets.size() > 2 ) h_dPhi_METjet3->Fill( dphi3,wt );
	if(hadJets.size() > 3 ) h_dPhi_METjet4->Fill( dphi4,wt );
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

	h_mTPhoMET->Fill( sqrt(2*(bestPhoton.Pt())*MET*(1-cos(DeltaPhi(METPhi,bestPhoton.Phi())))),wt );
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
    if( (*Photons_fullID)[iPho] && (*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons]<0.001) {
      goodPho.push_back( (*Photons)[iPho] );
      goodPhoIndx.push_back(iPho);
      allBestPhotons.push_back((*Photons)[iPho]);
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

void AnalyzeLightBSM::calKinWithDiffGravitinoMass(double newMass){
  TRandom *r1 = new TRandom(EvtNum);
  for(int i=0;i<2;i++){
    dchain[i].boost_ = (dchain[i].NLSPs_).BoostVector();
    dchain[i].newGrav_.SetE( ((pow((dchain[i].NLSPs_).M(),2)) - (pow((dchain[i].boson_).M(),2)) + (pow(newMass,2)) )/(2.0*(dchain[i].NLSPs_).M()) );
    dchain[i].newBoson_.SetE(sqrt( (pow((dchain[i].newGrav_).E(),2)) - (newMass*newMass) + (pow((dchain[i].boson_).M(),2)) ));
    dchain[i].p_ = sqrt( pow((dchain[i].newGrav_).E(),2) - (newMass*newMass));
    r1->Sphere(dchain[i].px_,dchain[i].py_,dchain[i].pz_,dchain[i].p_);

    dchain[i].newGrav_.SetPx(dchain[i].px_); dchain[i].newGrav_.SetPy(dchain[i].py_); dchain[i].newGrav_.SetPz(dchain[i].pz_);
    dchain[i].newBoson_.SetPx(-dchain[i].px_); dchain[i].newBoson_.SetPy(-dchain[i].py_); dchain[i].newBoson_.SetPz(-dchain[i].pz_);
  }
  for(int i=0;i<2&&0;i++){
    cout<<"NLSPs_:"<<" Px:"<<dchain[i].NLSPs_.Px()<<" Py:"<<dchain[i].NLSPs_.Py()<<" Pz:"<<dchain[i].NLSPs_.Pz()<<" P:"<<dchain[i].NLSPs_.P()<<" E:"<<dchain[i].NLSPs_.E()<<" M:"<<dchain[i].NLSPs_.M()<<endl;
    cout<<"boson_:"<<dchain[i].bosonId_<<" Px:"<<dchain[i].boson_.Px()<<" Py:"<<dchain[i].boson_.Py()<<" Pz:"<<dchain[i].boson_.Pz()<<" P:"<<dchain[i].boson_.P()<<" E:"<<dchain[i].boson_.E()<<" M:"<<dchain[i].boson_.M()<<endl;
    cout<<"grav_:"<<" Px:"<<dchain[i].grav_.Px()<<" Py:"<<dchain[i].grav_.Py()<<" Pz:"<<dchain[i].grav_.Pz()<<" P:"<<dchain[i].grav_.P()<<" E:"<<dchain[i].grav_.E()<<" M:"<<dchain[i].grav_.M()<<endl;
    cout<<"Radius:"<<dchain[i].p_<<" "<<sqrt(dchain[i].px_*dchain[i].px_+dchain[i].py_*dchain[i].py_+dchain[i].pz_*dchain[i].pz_)<<" "<<dchain[i].px_<<" "<<dchain[i].py_<<" "<<dchain[i].pz_<<endl;
    cout<<"newBoson_:"<<" Px:"<<dchain[i].newBoson_.Px()<<" Py:"<<dchain[i].newBoson_.Py()<<" Pz:"<<dchain[i].newBoson_.Pz()<<" P:"<<dchain[i].newBoson_.P()<<" E:"<<dchain[i].newBoson_.E()<<" M:"<<dchain[i].newBoson_.M()<<endl;
    cout<<"newGrav_:"<<" Px:"<<dchain[i].newGrav_.Px()<<" Py:"<<dchain[i].newGrav_.Py()<<" Pz:"<<dchain[i].newGrav_.Pz()<<" P:"<<dchain[i].newGrav_.P()<<" E:"<<dchain[i].newGrav_.E()<<" M:"<<dchain[i].newGrav_.M()<<endl;

    // TLorentzVector boson_,newBoson_;
    // int bosonId_,idxGen_;
    // TLorentzVector grav_, newGrav_;
    // TVector3 vec3_;
    // double px_,py_,pz_,p_;

  }

      // newGravitino.SetE( (((NLSPs[0].M())*(NLSPs[0].M()))-((NLSPkids[i].M())*(NLSPkids[i].M()))+(GravitinoMass*GravitinoMass))/(2.0*(NLSPs[0].M())));
      // momentum = sqrt( (newGravitino.E()*newGravitino.E()) - (GravitinoMass*GravitinoMass));
      // randNum.Sphere(px,py,pz,momentum);
      // newGravitino.SetPx(px); newGravitino.SetPy(py); newGravitino.SetPz(pz);
      
      // newPhotonBoson.SetE(sqrt((newGravitino.E()*newGravitino.E()) - (GravitinoMass*GravitinoMass) + (NLSPkids[i].M()*NLSPkids[i].M())));
      // newPhotonBoson.SetPx(-px); newPhotonBoson.SetPy(-py); newPhotonBoson.SetPz(-pz);
      
      // cout<<"radius: "<<sqrt((px*px)+(py*py)+(pz*pz))<<endl;
      // cout<<"After mass change, Gra Px:"<<newGravitino.Px()<<" Py:"<<newGravitino.Py()<<" Pz:"<<newGravitino.Pz()<<" E:"<<newGravitino.E()<<" M:"<<newGravitino.M()<<endl;
      // cout<<"After mass change,"<<NLSPkidId[i]<<" Px:"<<newPhotonBoson.Px()<<" Py:"<<newPhotonBoson.Py()<<" Pz:"<<newPhotonBoson.Pz()<<" E:"<<newPhotonBoson.E()<<" M:"<<newPhotonBoson.M()<<endl;    
}

/*
if(s_data=="TTJets_Tune" || s_data=="TTJets_HT" || s_data=="WJetsToLNu"){
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      if( (*Photons_nonPrompt)[bestPhotonIndxAmongPhotons] )
	process=true;
      else{
	process=false;
	for(int i=0;i<GenParticles->size();i++){
	  if( ((*GenParticles)[i].Pt()!=0) &&
	      (bestPhoton.DeltaR((*GenParticles)[i])<0.2) &&
	      //    (abs((*GenParticles_PdgId)[i])==11) && ( (abs((*GenParticles_ParentId)[i])==24) || (abs((*GenParticles_ParentId)[i])==15) ) ){
	      (abs((*GenParticles_PdgId)[i])==11) && ( (abs((*GenParticles_ParentId)[i])<=24) ) ){
	    process=true;break;
	  }
	}
	for(int i=0;i<GenParticles->size();i++){
	  if( ((*GenParticles)[i].Pt()!=0) && 
	      bestPhoton.DeltaR((*GenParticles)[i]) < 0.2 && (abs((*GenParticles_PdgId)[i])==22) && 
	      ( ((*GenParticles)[i].Pt()/bestPhoton.Pt()) > 0.9) && ((*GenParticles)[i].Pt()/bestPhoton.Pt()) < 1.1 ){
	    process=false;break;
	  }
	}
      }
    }//TTbar: non-prompt photons and fake photons
    else if(s_data=="TTGJets" || s_data=="WG"){
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      if( !((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]) ){
	process=true;
	for(int i=0;i<GenParticles->size();i++){//check if the photon is fake
	  if( ((*GenParticles)[i].Pt()!=0) &&
	      (bestPhoton.DeltaR((*GenParticles)[i])<0.2) &&
	      // (abs((*GenParticles_PdgId)[i])==11) && ( (abs((*GenParticles_ParentId)[i])==24) || (abs((*GenParticles_ParentId)[i])==15) ) ){
	      (abs((*GenParticles_PdgId)[i])==11) && ( (abs((*GenParticles_ParentId)[i])<=24) ) ){
	    process=false;break;
	  }
	}
      }
      else process=false;
    }//TTGamma choose only prompt photons. If photon is fake(e faking photon) do not consider the event.
    else if(s_data=="nonprompt"){
      process=false;
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      if( (*Photons_nonPrompt)[bestPhotonIndxAmongPhotons] )
	process=true;
      else continue;
    }//QCD
    else if(s_data=="prompt"){
      process=false;
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      if( !((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]) )
	process=true;
      else continue;
    }//GJets
    */
