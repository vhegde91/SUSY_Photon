#define LostMuon_cxx
#include "LostMuon.h"
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

  LostMuon ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void LostMuon::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  string s_data=data;
  
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  
  int evtSurvived=0;
  //get 2d histogram========================================
  // TFile *f_LP=new TFile("LstMu_CS_TTW_LostMu.root");
  //TFile *f_LP=new TFile("LstMu_CS_TTW_LostMuHadTau.root");
  TFile *f_LP=new TFile("LstMu_CS_TTW_HadTauOnly.root");
  //TFile *f_LP=new TFile("LstMu_b.root");
  TH2D *h2_LP;TH1D *h_LP;
  
  // TFile *f_LP=new TFile("LstMu_CS_WGWJets_LostMu.root");
  //TFile *f_LP=new TFile("LstMu_CS_WGWJets_LostMu_v2.root");
  /*  TFile *f_LP=new TFile("LstMu_CS_TTW_LostMu_v2.root");
      TFile *f_LP=new TFile("LstMu_CS_TTW_LostMu.root");
  TH2D *h2_LP=(TH2D*)f_LP->FindObjectAny("LostProb");
  int nX=h2_LP->GetXaxis()->GetNbins(), nY=h2_LP->GetYaxis()->GetNbins();
  double lostProb[nX][nY],histXLowEdge[nX+1],histYLowEdge[nY+1];
  for(int i=0;i<nX;i++){
    histXLowEdge[i]=h2_LP->GetXaxis()->GetBinLowEdge(i+1);
    for(int j=0;j<nY;j++){
      lostProb[i][j]=h2_LP->GetBinContent(i+1,j+1);
      if(i==0) histYLowEdge[j]=h2_LP->GetYaxis()->GetBinLowEdge(j+1);
    }
  }
  histXLowEdge[nX]=h2_LP->GetXaxis()->GetBinLowEdge(nX+1);
  histYLowEdge[nY]=h2_LP->GetYaxis()->GetBinLowEdge(nY+1);
  delete h2_LP,f_LP;*/
  //get 3d histogram========================================
  //TFile *f_LP=new TFile("LstMu_CS_WGWJets_LostMu.root");
  //TFile *f_LP=new TFile("LstMu_CS_TTW_LostMu_v2.root");
  /*  TFile *f_LP=new TFile("LstMu_CS_TTW_LostMu_v2.root");
  TH3D *h3_LP=(TH3D*)f_LP->FindObjectAny("LostProb");
  int nX=h3_LP->GetXaxis()->GetNbins(), nY=h3_LP->GetYaxis()->GetNbins(), nZ=h3_LP->GetZaxis()->GetNbins();
  double lostProb[nX][nY][nZ],histXLowEdge[nX+1],histYLowEdge[nY+1],histZLowEdge[nZ+1];
  for(int i=0;i<nX;i++){
    histXLowEdge[i]=h3_LP->GetXaxis()->GetBinLowEdge(i+1);
    for(int j=0;j<nY;j++){
      if(i==0) histYLowEdge[j]=h3_LP->GetYaxis()->GetBinLowEdge(j+1);
      for(int k=0;k<nZ;k++){
	lostProb[i][j][k]=h3_LP->GetBinContent(i+1,j+1,k+1);
	if(i==0 && j==0) histZLowEdge[k]=h3_LP->GetZaxis()->GetBinLowEdge(k+1);
      }
    }
  }
  histXLowEdge[nX]=h3_LP->GetXaxis()->GetBinLowEdge(nX+1);
  histYLowEdge[nY]=h3_LP->GetYaxis()->GetBinLowEdge(nY+1);
  histZLowEdge[nZ]=h3_LP->GetZaxis()->GetBinLowEdge(nZ+1);
  delete h3_LP,f_LP;*/
  
  // for(int i=0;i<=nX;i++)
  //   cout<<histYLowEdge[i]<<" ";
  //=========================================================
  int promptFiles=1;
  if(s_data=="CS_TTW_Lost") promptFiles=2;
  cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  cout<<"Only events with "<<endl
      <<"prompt best photon from first "<<promptFiles<<" line(s) in input runlist"<<endl
      <<"non-prompt best photon from remaining files"<<endl
      <<"will be read."<<endl;
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
    if(!(HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0)) continue;
    bool process=true;
    if(s_data=="TTJets_Tune"){
      if(jentry==0){cout<<"**********processing "<<s_data<<endl;}
      if( madHT>600 ){process=false;}
    }
    
    if(Electrons->size()>0) continue;//veto electrons for lost muon estimation
    if(Muons->size()>1) continue;//exactly one RECO muon or no muon(lost)
    double mt_mu=0;
    if(Muons->size()==1){
      mt_mu=sqrt(2*(*Muons)[0].Pt()*MET*(1-cos(DeltaPhi(METPhi,(*Muons)[0].Phi()))));
      if(mt_mu>100) continue;
    }
    int nGenMu=0,nGenEle=0,nGenTau=0,nGenMuFmTau=0,nGenEleFmTau=0;
    vector<TLorentzVector> genMu;
   
    for(int i=0;i<GenParticles->size();i++){
      if((*GenParticles)[i].Pt()!=0){
	if( abs((*GenParticles_PdgId)[i])==13 && (abs((*GenParticles_ParentId)[i])<=24) && ((*GenParticles_Status)[i]==1) ) {
	  nGenMu++; genMu.push_back((*GenParticles)[i]);
	}
	/*else if( abs((*GenParticles_PdgId)[i])==13 && abs((*GenParticles_ParentId)[i])==15 ){
	  if( abs((*GenParticles_ParentId)[(*GenParticles_ParentIdx)[i]]) == 24 ) {
	    nGenMu++; genMu.push_back((*GenParticles)[i]); nGenMuFmTau++;
	  }
	}*/ //muons
	else if( abs((*GenParticles_PdgId)[i])==11 && (abs((*GenParticles_ParentId)[i])<=24) && ((*GenParticles_Status)[i]==1) ) {nGenEle++;}
	/*else if( abs((*GenParticles_PdgId)[i])==11 && abs((*GenParticles_ParentId)[i])==15 ){
	  if( abs((*GenParticles_ParentId)[(*GenParticles_ParentIdx)[i]]) == 24 ) {nGenEle++; nGenEleFmTau++;}
	  }*/  //electrons
	else if( abs((*GenParticles_PdgId)[i])==15 && (abs((*GenParticles_ParentId)[i])<=24) ) {nGenTau++;}//taus
      }
    }
   
    //    if(Muons->size()==0){ 
    /*bool tau2hads=false;
      if(nGenTau>0 && nGenEleFmTau==0 && nGenMuFmTau==0) tau2hads=true;
      else tau2hads = false;*/
    if(nGenMu==0 && nGenEle==0 && nGenTau==0) continue;//to reject W->qq type of events    
    if(Muons->size()==0){
      //if(nGenMu==0) continue;//uncomment this line to do only lost muon, w/o including tau had decays
      bool temp1=true;
      if(nGenMu>0) temp1=false; //set  temp1=true for lost muon + had tau. set temp1=false for only hadtau
      else if(nGenEle>0) temp1=false;
      else temp1=true;
      // cout<<tau2hads;if(!tau2hads) {cout<<"gentaus_had "<<GenTaus_had->size()<<endl;print(jentry);}}
      //    cout<<GenTaus_had->size()<<tau2hads<<" ";
      if(temp1==false) continue;
    }
    if(nGenMu==0) {TLorentzVector v1;genMu.push_back(v1);}
    sortTLorVec(&genMu);
    
    //    if(!(genMu[0].Pt()<10 || abs(genMu[0].Eta())>2.4)) continue;
    //    if(genMu[0].Pt()<10) continue;

    //About photons
    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhoton.Pt() <= 100) continue;
    //bool eMatchedG=check_eMatchedtoGamma();
    bool bestPhoHasPxlSeed=true;
    if(bestPhotonIndxAmongPhotons>=0){
      if((*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons]<0.001) bestPhoHasPxlSeed=false;
    }
    if( bestPhoHasPxlSeed ) continue;
    //if((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]) continue; //select only prompt
    //if(!(*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]) continue; //select only non-promt
    
    if(fCurrent+1<=promptFiles){
      if((*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]) continue; //select only prompt
    }
    else{
      if(!(*Photons_nonPrompt)[bestPhotonIndxAmongPhotons]) continue; //select only non-promt
    }
    
   
    bool realPho=true;
    int matche=0,matchp=0;
    double minDR_Pho_GenObj=1000;
    if(Muons->size()==0){
      for(int i=0;i<GenParticles->size();i++){
	if((*GenParticles)[i].Pt()!=0){
	  double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
	  if(dr1 < 0.1 && (abs((*GenParticles_PdgId)[i])==11) && 
	     (abs((*GenParticles_ParentId)[i])==24 || abs((*GenParticles_ParentId)[i])==15) ) {matche=1;realPho=false;}
	  if(minDR_Pho_GenObj > dr1) minDR_Pho_GenObj=dr1;
	}
      }
      h_minDR_Pho_GenObj->Fill(minDR_Pho_GenObj,wt);
    
      for(int i=0;i<GenParticles->size();i++){
	if((*GenParticles)[i].Pt()!=0){
	  double dr1=bestPhoton.DeltaR((*GenParticles)[i]);
	  if( dr1<0.1 && (abs((*GenParticles_PdgId)[i])==22) && 
	      ( ((*GenParticles)[i].Pt()/bestPhoton.Pt()) > 0.9) && ((*GenParticles)[i].Pt()/bestPhoton.Pt()) < 1.1 ){ matchp=1;realPho=true;}
	}
      }
    }
    /*	  if(abs(photype)!=100){
	  cout<<"pho of unknown type"<<endl;
	  cout<<"Best Photon Pt: "<<bestPhoton.Pt()<<" Eta:"<<bestPhoton.Eta()<<" Phi:"<<bestPhoton.Phi()<<endl;
	  print(jentry);
	  }
    */
    if(matche==1 && matchp==0) continue;
    //if(!realPho) continue;
    
    //if(matche==1) continue;
    //photon has been identified. It is a real photon and it is matched to gen photon with dR(genPho,RecoPho) < 0.1 and Pts are within 10%.   

    //calulate ST and HadJets by cleaning the matching jet.
    bool hadJetID=true;
    int minDRindx=-100,phoMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0,remJetPt=0;
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
    if( minDR<0.3 ) phoMatchingJetIndx=minDRindx;
    //now hadJets contains all jets except the one matched to photon. check whether there is energy near photon or not. If yes then add it as a jet.
    if( phoMatchingJetIndx>=0 ){
      if( ((*Jets)[phoMatchingJetIndx].Pt()) > 1.1*(bestPhoton.Pt()) ){
	if( (((*Jets)[phoMatchingJetIndx] - bestPhoton).Pt())>30){
	  hadJets.push_back( (*Jets)[phoMatchingJetIndx] - bestPhoton );
	  remJetPt=((*Jets)[phoMatchingJetIndx] - bestPhoton).Pt();
	}
      }
    }
    //hadJets contains all jets if minDR>0.3. If minDR<0.3 and jetPt/GPt < 1.1, hadJets does not contain a jet matched to EMobj.
    //                                        If minDR<0.3 and jetPt/GPt > 1.1  it has only hadronic type of jets(photon is not accounted for)
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
    }
    sortTLorVec(&hadJets);
    
    //ST and HadJets have been determined. Now calulate dPhi b/w MET and leading HadJets.
    double dphi1=3.8,dphi2=3.8,dphi3=3.8,dphi4=3.8,dphiPho_MET=3.8;
    if(bestPhoton.Pt()>0.1) dphiPho_MET=abs(DeltaPhi(METPhi,bestPhoton.Phi()));
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));
    if(hadJets.size() > 2 ) dphi3 = abs(DeltaPhi(METPhi,(hadJets)[2].Phi()));
    if(hadJets.size() > 3 ) dphi4 = abs(DeltaPhi(METPhi,(hadJets)[3].Phi()));
  
    //
    //apply baseline selections
    process = process && ST>500 && MET > 100 && nHadJets >=2 && dphi1 > 0.3 && dphi2 > 0.3 && bestPhoton.Pt() > 100;
    //    process = process && ST>500 && MET > 100 && nHadJets >=7 && dphi1 > 0.3 && dphi2 > 0.3 && bestPhoton.Pt() > 100;
    //    process = process && ST>500 && MET > 100 && nHadJets >=2 && bestPhoton.Pt() > 100;
    if(process && hadJetID){
      evtSurvived++;
      h_RunNum->Fill(RunNum);
      // print(jentry);
      h_ST->Fill(ST,wt);
      h_MET->Fill(MET,wt);
      h_nHadJets->Fill(nHadJets,wt);
      h_BTags->Fill(BTags,wt);
      h_HT->Fill(HT,wt);
      h_MHT->Fill(MHT,wt);
      h_nJets->Fill(NJets,wt);
      h_METvBin->Fill(MET,wt);
      h_madHT->Fill(madHT,wt);
      h_nVtx->Fill(NVtx,wt);
      findObjMatchedtoG(bestPhoton);
      int nMuMultJ=0;
      for(int i=0;i<Jets->size();i++){
	if((*Jets)[i].Pt()>30.0){ nMuMultJ = nMuMultJ + (*Jets_muonMultiplicity)[i]; }
      }
      int searchRegion=0;
      if     (nHadJets >= 2 && nHadJets <= 4 ) searchRegion=1;
      else if(nHadJets == 5 || nHadJets == 6 ) searchRegion=2;
      else if(nHadJets >= 7                  ) searchRegion=3;
      
      if(Muons->size()==0){
	h_ST_Mu0->Fill(ST,wt);
	h_MET_Mu0->Fill(MET,wt);
	h_nHadJets_Mu0->Fill(nHadJets,wt);
	h_BTags_Mu0->Fill(BTags,wt);
	h_METvBin_Mu0->Fill(MET,wt);
	h_BestPhotonPt_Mu0->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_Mu0->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_Mu0->Fill(bestPhoton.Phi(),wt);

	h_isoMuonTracks_Mu0->Fill(isoMuonTracks,wt);
	h_MuMultInJets_Mu0->Fill(nMuMultJ,wt);

	h_GenMuons_MT2Activity_Mu0->Fill((*GenMuons_MT2Activity)[0],wt);
	h_nGenEle_Mu0->Fill(nGenEle,wt);
	h_nGenMu_Mu0->Fill(nGenMu,wt);
	h_nGenTau_Mu0->Fill(nGenTau,wt);

	h_dPhi_PhoMET_Mu0->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_Mu0->Fill(dphi1,wt);
	h_dphi_METjet2_Mu0->Fill(dphi2,wt);

	h_Jet1Pt_Mu0->Fill(hadJets[0].Pt(),wt);
	h_Jet2Pt_Mu0->Fill(hadJets[1].Pt(),wt);	

	h_GenMuEta_Mu0->Fill(genMu[0].Eta(),wt);
	h_GenMuPt_Mu0->Fill(genMu[0].Pt(),wt);
	h_nGenEleFmTau_Mu0->Fill(nGenEleFmTau,wt);
	h_nGenMuFmTau_Mu0->Fill(nGenMuFmTau,wt);
	for(int i=0;i<GenParticles->size();i++){
	  if(abs((*GenParticles_PdgId)[i]) == 13){
	    h_genMuParent_Mu0->Fill(abs((*GenParticles_ParentId)[i]),wt);
	  }
	}
	//print(jentry);
	h2_METnHadJ_Mu0->Fill(MET,nHadJets,wt);
	h2_dPhi1nHadJ_Mu0->Fill(dphi1,nHadJets,wt);
	h2_dPhi2nHadJ_Mu0->Fill(dphi2,nHadJets,wt);
	h2_dPhi1MET_Mu0->Fill(dphi1,MET,wt);
	h2_STMET_Mu0->Fill(ST,MET,wt);
	h2_dPhi1dPhi2_Mu0->Fill(dphi1,dphi2,wt);
	h2_GenMuPtGenAct_Mu0->Fill(genMu[0].Pt(),(*GenMuons_MT2Activity)[0],wt);
	h2_STHadJ_Mu0->Fill(ST,nHadJets,wt);
	h2_METJet1Pt_Mu0->Fill(MET,hadJets[0].Pt(),wt);

	if(nHadJets<=3){
	  h2_STMET_2to3HadJ_Mu0->Fill(ST,MET,wt);
	  h2_STMET_2to3HadJ_Mu0_fb->Fill(ST,MET,wt);
	  for(int ri=0;ri<6;ri++){
	    h2_STMET_2to3HadJ_Mu0_R[ri]->Fill(ST,MET,wt);
	    h2_STMET_2to3HadJ_Mu0_R[ri]->ClearUnderflowAndOverflow();
	  }
	}
	if(nHadJets>=4){
	  h2_STMET_min4HadJ_Mu0->Fill(ST,MET,wt);
	}
	h3_STMETnHadJ_Mu0->Fill(ST,MET,nHadJets,wt);
	h2_hadJbTag_Mu0->Fill(nHadJets,BTags,wt);

	if(nHadJets==2 || nHadJets==3)      h2_STMET_NJ2or3_Mu0  ->Fill(ST,bestPhoton.Pt(),wt);
	else if(nHadJets==4)                h2_STMET_NJ4_Mu0     ->Fill(ST,bestPhoton.Pt(),wt);
	else if(nHadJets==5 || nHadJets==6) h2_STMET_NJ5or6_Mu0  ->Fill(ST,MET,wt);
	else if(nHadJets>=7)                h2_STMET_NJ7toInf_Mu0->Fill(ST,MET,wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_Mu0_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! ";
      }//0 muon + photon events
      else{
	double wt_org=wt;
	//	double parX=MET,parY=nHadJets;
	//double parX=nHadJets;
	//	double parX=dphi1,parY=dphi2;
	//	double parX=ST,parY=MET;
	double parX=ST,parY=bestPhoton.Pt();
	//double parX=MET,parY=hadJets[0].Pt();
	//	double parX=ST,parY=nHadJets;
	double tf=0;
	// h2_LP=(TH2D*)f_LP->FindObjectAny("LostProb_1");
	// tf=h2_LP->GetBinContent(h2_LP->FindBin(parX,parY));
	// if(tf<0.000001) cout<<"tf:"<<tf;

	// h_LP=(TH1D*)f_LP->FindObjectAny("LostProb");
	// tf=h_LP->GetBinContent(h_LP->FindBin(parX));
	TString name;
	if(nHadJets==2 || nHadJets==3)      name="LostProb_1";
	else if(nHadJets==4)                name="LostProb_2";
	else if(nHadJets==5 || nHadJets==6) name="LostProb_3";
	else if(nHadJets>=7)                name="LostProb_4";
	h2_LP=(TH2D*)f_LP->FindObjectAny(name);
	if(h2_LP) tf=h2_LP->GetBinContent(h2_LP->FindBin(parX,parY));
	else cout<<"hist not found"<<endl;
	wt=tf*wt;
	
	/*	int xi=-100,yi=-100;
	for(int i=0;i<nX;i++){
	  if(parX>=histXLowEdge[i] && parX < histXLowEdge[i+1]){xi=i;break;}
	}
	for(int i=0;i<nY;i++){
	  if(parY>=histYLowEdge[i] && parY < histYLowEdge[i+1]){yi=i;break;}
	}
	// if(xi<0 || yi<0) {cerr<<"something wrong. check axes of the histogram with lostProb. xi="<<xi<<" parX="<<parX<<" parY="<<parY<<" yi="<<yi<<endl;return;}
	if(xi<0 || yi<0) {cout<<"o";}
	else  wt=wt*lostProb[xi][yi];
	*/
	//-------------------------------------------------------------------------------
	/*	double parX=ST,parY=MET,parZ=nHadJets;
	int xi=-100,yi=-100,zi=-100;
	for(int i=0;i<nX;i++){
	  if(parX>=histXLowEdge[i] && parX < histXLowEdge[i+1]){xi=i;break;}
	}
	for(int i=0;i<nY;i++){
	  if(parY>=histYLowEdge[i] && parY < histYLowEdge[i+1]){yi=i;break;}
	}
	for(int i=0;i<nZ;i++){
	  if(parZ>=histZLowEdge[i] && parZ < histZLowEdge[i+1]){zi=i;break;}
	}
	//	if(xi<0 || yi<0 || zi<0) {cerr<<"something wrong. check axes of the histogram with fakerate. xi="<<xi<<" parX="<<parX<<" parY="<<parY<<" yi="<<yi<<" parZ:"<<parZ<<" zi:"<<zi<<endl;return;}
	if(xi<0 || yi<0 || zi<0) {cout<<"o";}
	else  wt=wt*lostProb[xi][yi][zi];*/
	//	if(wt<0) cout<<xi<<" parX="<<parX<<" parY="<<parY<<" yi="<<yi<<" parZ:"<<parZ<<" zi:"<<zi<<endl;
	//	if(nGenMu==0) print(jentry);

	h_ST_Mu1->Fill(ST,wt);
	h_MET_Mu1->Fill(MET,wt);
	h_nHadJets_Mu1->Fill(nHadJets,wt);
	h_BTags_Mu1->Fill(BTags,wt);
	h_METvBin_Mu1->Fill(MET,wt);
	h_BestPhotonPt_Mu1->Fill(bestPhoton.Pt(),wt);
	h_BestPhotonEta_Mu1->Fill(bestPhoton.Eta(),wt);
	h_BestPhotonPhi_Mu1->Fill(bestPhoton.Phi(),wt);

	h_isoMuonTracks_Mu1->Fill(isoMuonTracks,wt);
	h_MuMultInJets_Mu1->Fill(nMuMultJ,wt);

	h_GenMuons_MT2Activity_Mu1->Fill((*GenMuons_MT2Activity)[0],wt);
	h_nGenEle_Mu1->Fill(nGenEle,wt);
	h_nGenMu_Mu1->Fill(nGenMu,wt);
	h_nGenTau_Mu1->Fill(nGenTau,wt);

	h_MuPt->Fill((*Muons)[0].Pt(),wt);
	h_MuEta->Fill((*Muons)[0].Eta(),wt);
	h_MuPhi->Fill((*Muons)[0].Phi(),wt);
	h_myMT->Fill(mt_mu,wt);

	h_dPhi_PhoMET_Mu1->Fill(dphiPho_MET,wt);
	h_dphi_METjet1_Mu1->Fill(dphi1,wt);
	h_dphi_METjet2_Mu1->Fill(dphi2,wt);
	h_dPhi_Muon_Photon->Fill(abs(bestPhoton.DeltaPhi((*Muons)[0])),wt);

	h_Jet1Pt_Mu1->Fill(hadJets[0].Pt(),wt);	
	h_Jet2Pt_Mu1->Fill(hadJets[1].Pt(),wt);	

	h_GenMuPt_Mu1->Fill(genMu[0].Pt(),wt);
	h_nGenEleFmTau_Mu1->Fill(nGenEleFmTau,wt);
	h_nGenMuFmTau_Mu1->Fill(nGenMuFmTau,wt);

	h2_STvsMuPt_Mu1->Fill((*Muons)[0].Pt(),ST,wt);
	h2_METvsMuPt_Mu1->Fill((*Muons)[0].Pt(),MET,wt);

	//	if(GenMuons_MT2Activity->size()!=1 || Muons_MT2Activity->size()!=1) cout<<"!! GenMuons_MT2Activity->size="<<GenMuons_MT2Activity->size()<<" Muons_MT2Activity->size="<<Muons_MT2Activity->size()<<endl;
	h2_GenActVsRECOAct->Fill((*GenMuons_MT2Activity)[0],(*Muons_MT2Activity)[0],wt);
	h2_GenMuPtVsRECOMuPt->Fill(genMu[0].Pt(),(*Muons)[0].Pt(),wt);

	h2_METnHadJ_Mu1->Fill(MET,nHadJets,wt);
	h2_dPhi1nHadJ_Mu1->Fill(dphi1,nHadJets,wt);
	h2_dPhi2nHadJ_Mu1->Fill(dphi2,nHadJets,wt);
	h2_dPhi1MET_Mu1->Fill(dphi1,MET,wt);
	h2_STMET_Mu1->Fill(ST,MET,wt);
	h2_dPhi1dPhi2_Mu1->Fill(dphi1,dphi2,wt);
	h2_STHadJ_Mu1->Fill(ST,nHadJets,wt);
	h2_METJet1Pt_Mu1->Fill(MET,hadJets[0].Pt(),wt);
	h2_RecoMuPtRecoAct_Mu1->Fill((*Muons)[0].Pt(),(*Muons_MT2Activity)[0],wt);

	if(nHadJets<=3){
	  h2_STMET_2to3HadJ_Mu1->Fill(ST,MET,wt);
	  h2_STMET_2to3HadJ_Mu1_fb->Fill(ST,MET,wt);
	  for(int ri=0;ri<6;ri++){
	    h2_STMET_2to3HadJ_Mu1_R[ri]->Fill(ST,MET,wt);
	    h2_STMET_2to3HadJ_Mu1_R[ri]->ClearUnderflowAndOverflow();
	  }
	}
	if(nHadJets>=4){
	  h2_STMET_min4HadJ_Mu1->Fill(ST,MET,wt);
	}
	h3_STMETnHadJ_Mu1->Fill(ST,MET,nHadJets,wt);
	
	h2_hadJbTag_Mu1->Fill(nHadJets,BTags,wt);
	if(nHadJets==2 || nHadJets==3)      h2_STMET_NJ2or3_Mu1  ->Fill(ST,bestPhoton.Pt(),wt);
	else if(nHadJets==4)                h2_STMET_NJ4_Mu1     ->Fill(ST,bestPhoton.Pt(),wt);
	else if(nHadJets==5 || nHadJets==6) h2_STMET_NJ5or6_Mu1  ->Fill(ST,MET,wt);
	else if(nHadJets>=7)                h2_STMET_NJ7toInf_Mu1->Fill(ST,MET,wt);
	//---------------- search bins -----------------------
	if( searchRegion > 0 && searchRegion < 4){
	  h_MET_Mu1_R[searchRegion-1]->Fill(MET,wt);
	}
	else cout<<"Event outside search region! "<<searchRegion;
	wt=wt_org;
	//	if(!nMuMultJ) print(jentry);
      }//muon + photon events
    }
  } // loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
}


TLorentzVector LostMuon::getBestPhoton(){
  // bool passIDVeryLoose=false,passIDLoose=false,passIsoVeryLoose=false,passIsoLoose=false;
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;

  for(int iPho=0;iPho<Photons->size();iPho++){
    if( (*Photons_fullID)[iPho] ) {
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
    return goodPho[highPtIndx];
  }
  else{
    bestPhotonIndxAmongPhotons = -100;
    TLorentzVector v0;return v0;
  }
}


bool LostMuon::check_eMatchedtoGamma(){
  if(bestPhotonIndxAmongPhotons>=0){
    for(int i=0;i<Electrons->size();i++){
      if( (*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i] ) < 0.1){
	//	cout<<(*Electrons)[i].Pt()<<" "<<(*Electrons)[i].Eta()<<" "<<(*Electrons)[i].Phi()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Pt()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Eta()<<" "<<(*Photons)[bestPhotonIndxAmongPhotons].Phi()<<" dR:"<<(*Photons)[bestPhotonIndxAmongPhotons].DeltaR( (*Electrons)[i])<<endl;
	return true;
      }
    }
  }
  else
    return false;
}

void  LostMuon::findObjMatchedtoG(TLorentzVector bestPhoton){
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
  }

}



void LostMuon::print(Long64_t jentry){
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
    //  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
    if(1){
      cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<endl;
    }
  }

  for(int i=0;i<Muons->size();i++){
    if(i==0) cout<<"-------------------------------- Muons -------------------------------------------"<<endl;
    cout<<"MuonPt: "<<(*Muons)[i].Pt()<<" Eta: "<<(*Muons)[i].Eta()<<" Phi: "<<(*Muons)[i].Phi()<<" M: "<<(*Muons)[i].M()<<endl;
  }
  cout<<"^^^^^^^^^^^^^^^^^^ Event ends ^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;
}


/*
  double parX=nHadJets;
  // double lowEdge[]={0,250,500,750,1000,1250,1500,1750,2000,2250,2500,2750,3000,3250,3500,3750,5000};//ST
  // double prob[]={0,0,0.375449,0.240811,0.167688,0.248128,0.25,0,0.333333,0,0,0,0,0,0,0};//ST
  // double prob[]={0,0,0,0,0,0.20211,0.283436,0.425507,0.251927,0.635737,0.573739,0,0,0,0};//MET var bin
  // double lowEdge[]={0,20,40,60,80,100,120,160,200,270,350,450,600,750,900,1500};//MET var bin
  double prob[]={0,0,0.443187,0.384385,0.214079,0.117395,0.419692,1.84794,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//nHadJets
  double lowEdge[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};//nHadJets
  for(int i=0;i<sizeof(lowEdge)/sizeof(double);i++){
  if(parX>=lowEdge[i] && parX<lowEdge[i+1]) {
  //	    cout<<ST<<" "<<lowEdge[i]<<" "<<lowEdge[i+1]<<" parx:"<<parX<<" prob:"<<prob[i]<<endl;
  wt=wt*prob[i];
  break;
  }
  }
*/
/*

 vector<TLorentzVector> visObj;//cout<<"phoPt:"<<bestPhoton.Pt()<<endl;
    visObj.push_back(bestPhoton);
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){visObj.push_back(hadJets[i]);}
      //      cout<<hadJets[i].Pt()<<" ";
    }
    sortTLorVec(&visObj);
    //    cout<<endl<<"sorted:"<<endl;
    //    for(int i=0;i<visObj.size();i++){cout<<visObj[i].Pt()<<" ";}
    //    cout<<"-----------------------------"<<endl;
  if(visObj.size() > 0 ) dphi1 = abs(DeltaPhi(METPhi,(visObj)[0].Phi()));
    if(visObj.size() > 1 ) dphi2 = abs(DeltaPhi(METPhi,(visObj)[1].Phi()));
    if(visObj.size() > 2 ) dphi3 = abs(DeltaPhi(METPhi,(visObj)[2].Phi()));
    if(visObj.size() > 3 ) dphi4 = abs(DeltaPhi(METPhi,(visObj)[3].Phi()));
*/