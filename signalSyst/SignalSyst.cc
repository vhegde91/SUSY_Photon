#define SignalSyst_cxx
#include "SignalSyst.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <fstream>
//#include "btag/BTagCorrector.h"

using namespace std;

int main(int argc, char* argv[])
{

  if (argc < 4) {
    cerr << "Please give 5 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << " mass of gluino and mass of (N)LSP"<< endl;
    return -1;
  }

  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *s_mGl         = argv[4];
  const char *s_mNLSP       = argv[5];

  SignalSyst ana(inputFileList, outFileName, data, s_mGl, s_mNLSP);
  cout << "dataset " << data << " mGl: "<<s_mGl<<" mNLSP: "<<s_mNLSP << endl;

  double mGl=atof(s_mGl),mNLSP=atof(s_mNLSP);
  ana.EventLoop(data,inputFileList,mGl,mNLSP);
  
  return 0;
}

void SignalSyst::EventLoop(const char *data,const char *inputFileList,const double mGl,const double mNLSP) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  bool do_JECup = 1;
  bool do_JECdn = 1;
  bool do_JERup = 1;
  bool do_JERdn = 1;
  TString s_data=data;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  int evtSurvived=0;
  bool process = 1;
  double momMass = mGl, nlspMass = mNLSP, nevents=0, xsec1=1.0;

  TFile* pufile = TFile::Open("PileupHistograms_0121_69p2mb_pm4p6.root","READ");
  //choose central, up, or down
  TH1* puhist = (TH1*)pufile->Get("pu_weights_down");
  //----------- btags SFs-----------------
  bool applybTagSFs=0;
  int fListIndxOld=-1;
  double prob0=-100,prob1=-100;
  vector<TString> inFileName;
  TString sampleName;
  string str1;
  ifstream runListFile(inputFileList);
  TFile *currFile;
  while (std::getline(runListFile, str1)) {
    inFileName.push_back(str1);
  }runListFile.close();
  cout<<"applying b-tag SFs? "<<applybTagSFs<<endl;
  // BTagCorrector btagcorr;
  // //if fastsim
  // if(s_data.Contains("FastSim"))
  //   btagcorr.SetFastSim(true);
  //--------------------------------------
  //-----------ISR rewighting for signal ------------------
  bool isrReweight = 0;
  TFile *fSampleInfo;  TH2D *h2_isrWtCorr;
  vector<double> isrwt_arr={1., 0.920, 0.821, 0.715, 0.662, 0.561, 0.511};
  vector<double> isrwtUnc_arr={1., 0.04, 0.09, 0.143, 0.169, 0.219, 0.244};//if applying unc
  if(s_data.Contains("FastSim")){
    isrReweight = 1;//set here for doing ISR weighting. Reset for not applying.
    cout<<"Applying ISR weights to these signal samples? "<<isrReweight<<endl;
    //    cout<<"applying b-tag SFs for signal?(ignore last message on bTag SFs) "<<applybTagSFs<<endl;
    if(s_data.Contains("T5bbbb")) fSampleInfo = new TFile("T5bbbbZg_MassScan.root");
    else if(s_data.Contains("T5tttt")) fSampleInfo = new TFile("T5ttttZg_MassScan.root");
    else if(s_data.Contains("T5qqqq")) fSampleInfo = new TFile("T5qqqqHg_MassScan.root");
    else if(s_data.Contains("T6tt")) fSampleInfo = new TFile("T6ttZg_MassScan.root");
    h2_isrWtCorr = (TH2D*)fSampleInfo->Get("Nevts_NoISRWt");
    h2_isrWtCorr->Divide((TH2D*)fSampleInfo->Get("Nevts_ISRWt"));
  }
  //----------------Get Xsec and Num events for the samples ----------------------------------
  TH1D *h1_xsechist = (TH1D*)fSampleInfo->FindObjectAny("mGlXsec");
  TH2D *h2_mass = (TH2D*)fSampleInfo->FindObjectAny("MGlMNLSP");
  if(!h1_xsechist){ cout<<"AHHHHH: could not find xsec hist"<<endl; return;}
  if(!h2_mass){ cout<<"AHHHHH: could not find no. of events hist"<<endl; return;}
  xsec1 = h1_xsechist->GetBinContent(h1_xsechist->FindBin(momMass));

  int nxbins = h2_mass->GetNbinsX();
  int nybins = h2_mass->GetNbinsY();
  //    cout<<"Nx bins:"<<nxbins<<" Ny bins:"<<nybins<<endl;
  for(int nx=1;nx<=nxbins;nx++){
    if(nevents > 9.99) break;
    if(abs(momMass - (h2_mass->GetXaxis()->GetBinCenter(nx))) > 0.1) continue;
    for(int ny=1;ny<=nybins;ny++){
      if(abs(nlspMass - (h2_mass->GetYaxis()->GetBinCenter(ny))) > 0.1) continue;
      else{ nevents = h2_mass->GetBinContent(nx,ny); break;}
    }
  }
  cout<<"********************"<<endl;
  cout<<"Gluino Mass: "<<momMass<<endl;
  cout<<"NLSP Mass: "<<nlspMass<<endl;
  cout<<"Produced no. of events in whole dataset: "<<nevents<<endl;
  cout<<"Xsec in pb: "<<xsec1<<endl;
  if(nevents<1) { cout<<"AHHHHH: No. of events for this mass point:"<<nevents<<endl; return;}
  //--------------------------------------------

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

    NumEvents = nevents;
    CrossSection = xsec1;
    Weight = CrossSection/NumEvents;
    if(abs(SusyMotherMass-momMass) > 0.001 || abs(SusyLSPMass-nlspMass) > 0.001) continue;

    process = true;
    wt=Weight*1000.0*lumiInfb;
    h_cutFlow->Fill(0);
        
    //-----------ISR reweighting for signal ------------------
    double isrWt = 0,isrWtCorr = 0.;
    if(s_data.Contains("FastSim") && isrReweight){
      h_ISRCorrFactors->Fill(0.0,wt);
      isrWtCorr = h2_isrWtCorr->GetBinContent(h2_isrWtCorr->GetXaxis()->FindBin(SusyMotherMass),h2_isrWtCorr->GetYaxis()->FindBin(SusyLSPMass));

      if(NJetsISR>=6) isrWt = isrwt_arr[6]+isrwtUnc_arr[6];//if applying unc, +isrwtUnc_arr[6];
      else isrWt = isrwt_arr[NJetsISR]+isrwtUnc_arr[6];//if applying unc, +isrwtUnc_arr[NJetsISR];
      h_ISRCorrFactors->Fill(2,wt*isrWt);

      if(NJetsISR>=6) isrWt = isrwt_arr[6]-isrwtUnc_arr[6];//if applying unc, -isrwtUnc_arr[6];
      else isrWt = isrwt_arr[NJetsISR]-isrwtUnc_arr[6];//if applying unc, -isrwtUnc_arr[NJetsISR];
      h_ISRCorrFactors->Fill(3,wt*isrWt);

      if(NJetsISR>=6) isrWt = isrwt_arr[6];
      else isrWt = isrwt_arr[NJetsISR];
      h_ISRCorrFactors->Fill(1,wt*isrWt);
      wt = wt*isrWt*isrWtCorr;
    }
    //--------------------------------------------------------
    wt=wt*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));    
    TLorentzVector bestPhoton=getBestPhoton();
    bool eMatchedG=check_eMatchedtoGamma();//this may not be necessary since e veto is there.
    if(bestPhoton.Pt()<99.9) continue;
    h_cutFlow->Fill(1);

    if(fListIndxOld!=fCurrent){
      fListIndxOld = fCurrent;
      sampleName = inFileName[fCurrent];
      //----------- btags SFs-----------------                                                     
      // if(applybTagSFs){
      //   currFile = TFile::Open(sampleName);
      //   btagcorr.SetEffs(currFile);
      //   btagcorr.SetCalib("btag/CSVv2_Moriond17_B_H_mod.csv");
      // 	if(s_data.Contains("FastSim"))btagcorr.SetCalibFastSim("btag/fastsim_csvv2_ttbar_26_1_2017.csv");
      // }
    }
    // vector<double> prob;
    // if(applybTagSFs){
    //   // btagcorr.SetBtagSFunc(1); 
    //   // btagcorr.SetMistagSFunc(1); 
    //   // btagcorr.SetBtagCFunc(1); 
    //   // btagcorr.SetMistagCFunc(1);      
    //   prob = btagcorr.GetCorrections(Jets,Jets_hadronFlavor,Jets_HTMask);
    //   prob0 = prob[0]; prob1 = prob[1]+prob[2]+prob[3];
    //   //      double corr = btagcorr.GetSimpleCorrection(Jets,Jets_hadronFlavor,Jets_HTMask,Jets_bDiscriminatorCSV);                                               
    // }
    
    if( (Electrons->size() !=0) || (Muons->size() !=0) ) continue;
    h_cutFlow->Fill(2);
    if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;
    h_cutFlow->Fill(3);
      
    bool noFakeJet = true;
    if(s_data.Contains("FastSim")){
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
    }

    bool hadJetID=true;
    int minDRindx=-100,photonMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0.;
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
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
    }
    sortTLorVec(&hadJets);

    double dphi1=3.8,dphi2=3.8,dphi1_genMET=3.8,dphi2_genMET=3.8;
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));

    if(hadJets.size() > 0 ) dphi1_genMET = abs(DeltaPhi(GenMETPhi,(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2_genMET = abs(DeltaPhi(GenMETPhi,(hadJets)[1].Phi()));

    if(photonMatchingJetIndx>=0 && ((*Jets)[photonMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    if(photonMatchingJetIndx<0) continue;
    h_cutFlow->Fill(4);

    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    h_cutFlow->Fill(5);

    process = process && !eMatchedG && bestPhoton.Pt()>=100 && (dphi1 > 0.3 && dphi2 > 0.3) && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && MET > 100;    
    if(!eMatchedG && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && GenMET > 100 && dphi1_genMET > 0.3 && dphi2_genMET > 0.3){
      int sBin7 = getBinNoV7(nHadJets,GenMET);
      h_SBins_v7_genMET_CD->Fill(sBin7,wt);
    }
    if(s_data.Contains("FastSim")){
      hadJetID = true;
      if(!noFakeJet) continue;
    }

    if(process){
      evtSurvived++;      
      h_cutFlow->Fill(6);
      h_RunNum->Fill(RunNum);
      h_intLumi->Fill(lumiInfb,wt);
      //------------------------ Sbins----------------------------
      int sBin7 = getBinNoV7(nHadJets,MET);
      //------------------------ Sbins----------------------------
      h_ST_CD->Fill(ST,wt);
      h_MET_CD->Fill(MET,wt);
      h_nHadJets_CD->Fill(nHadJets,wt);
      h_BTags_CD->Fill(BTags,wt);
      
      h_nVtx_CD->Fill(NVtx,wt);
      h_GenMET_CD->Fill(GenMET,wt);
      h_METPhi_CD->Fill(METPhi,wt);
      h_SBins_v7_CD->Fill(sBin7,wt);
      h_SBins_v7_ISRup_CD->Fill(sBin7, (wt/(isrWt*isrWtCorr)) * (isrWt + (abs(1-isrWt)/2) ));//isr up
      h_SBins_v7_ISRdn_CD->Fill(sBin7, (wt/(isrWt*isrWtCorr)) * (isrWt - (abs(1-isrWt)/2) ));//isr down

      h_BestPhotonPt_CD->Fill( bestPhoton.Pt(),wt );
      h_METvBin_CD->Fill(MET,wt);
      h_mindPhi1dPhi2_CD->Fill(min(dphi1,dphi2),wt);

      for(int j=0;j<ScaleWeights->size();j++)
	h2_SBins_v7_CD_vs_ScaleIdx->Fill(sBin7, j, wt*(*ScaleWeights)[j]);
      h_madHT->Fill(madHT,wt);
    }//process
  }// loop over entries
  cout<<"Events Survied:"<<evtSurvived<<endl;
 

  /////////////////////////////////////////////////////////////////////////////////
  ////////////////////// JEC up ///////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  cout<<"Doing JEC up"<<endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if(!do_JECup) break;

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    NumEvents = nevents;
    CrossSection = xsec1;
    Weight = CrossSection/NumEvents;
    if(abs(SusyMotherMass-momMass) > 0.001 || abs(SusyLSPMass-nlspMass) > 0.001) continue;

    process = true;
    wt=Weight*1000.0*lumiInfb*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));
    //-----------ISR reweighting for signal ------------------
    if(s_data.Contains("FastSim") && isrReweight){
      double isrWt = 0,isrWtCorr = h2_isrWtCorr->GetBinContent(h2_isrWtCorr->GetXaxis()->FindBin(SusyMotherMass),h2_isrWtCorr->GetYaxis()->FindBin(SusyLSPMass));

      if(NJetsISRJECup>=6) isrWt = isrwt_arr[6];
      else isrWt = isrwt_arr[NJetsISRJECup];
      wt = wt*isrWt*isrWtCorr;
    }
    //--------------------------------------------------------
    TLorentzVector bestPhoton=getBestPhoton();
    bool eMatchedG=check_eMatchedtoGamma();//this may not be necessary since e veto is there.
    if(bestPhoton.Pt()<99.9) continue;

    if( (Electrons->size() !=0) || (Muons->size() !=0) ) continue;
    if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;
    
    bool noFakeJet = true;
    if(s_data.Contains("FastSim")){
      //      //reject events with any jet pt>20, |eta|<2.5 NOT matched to a GenJet (w/in DeltaR<0.3) and chfrac < 0.1
      for(unsigned j = 0; j < JetsJECup->size(); ++j){
      	if(JetsJECup->at(j).Pt() <= 20 || fabs(JetsJECup->at(j).Eta())>=2.5) continue;
      	bool genMatched = false;
      	for(unsigned g = 0; g < GenJets->size(); ++g){
      	  if(GenJets->at(g).DeltaR(JetsJECup->at(j)) < 0.3) {
      	    genMatched = true;
      	    break;
      	  }
      	}
      	if(!genMatched && JetsJECup_chargedHadronEnergyFraction->at(j) < 0.1){
      	  noFakeJet = false;
      	  break;
      	}
      }
    }

    bool hadJetID=true;
    int minDRindx=-100,photonMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<JetsJECup->size();i++){
      if( ((*JetsJECup)[i].Pt() > MHT_PtCut) && (abs((*JetsJECup)[i].Eta()) <= HT_EtaCut) ){
	double dR=bestPhoton.DeltaR((*JetsJECup)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    for(int i=0;i<JetsJECup->size();i++){
      if( ((*JetsJECup)[i].Pt() > MHT_PtCut) && (abs((*JetsJECup)[i].Eta()) <= HT_EtaCut) ){
	if( !(minDR < 0.3 && i==minDRindx) ){
	  hadJets.push_back((*JetsJECup)[i]);
	  if(hadJetID) hadJetID=(*JetsJECup_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) photonMatchingJetIndx=minDRindx;
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
    }
    sortTLorVec(&hadJets);
    
    double dphi1=3.8,dphi2=3.8;
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi((*METPhiUp)[1],(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi((*METPhiUp)[1],(hadJets)[1].Phi()));
     
    if(photonMatchingJetIndx>=0 && ((*JetsJECup)[photonMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    if(photonMatchingJetIndx<0) continue;

    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    process = process && !eMatchedG && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && (*METUp)[1] > 100 && dphi1 > 0.3 && dphi2 > 0.3;

    if(s_data.Contains("FastSim")){
      hadJetID = true;
      if(!noFakeJet) continue;
    }
    if(process){
      evtSurvived++;
      //------------------------ Sbins----------------------------
      int sBin7 = getBinNoV7(nHadJets,(*METUp)[1]);
      //------------------------ Sbins----------------------------
      h_SBins_v7_JECup_CD->Fill(sBin7,wt);
    }//process
  }//loop entries, jec up
  /////////////////////////////////////////////////////////////////////////////////
  ////////////////////// JEC Down ///////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  cout<<"Doing JEC Down"<<endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if(!do_JECdn) break;

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    NumEvents = nevents;
    CrossSection = xsec1;
    Weight = CrossSection/NumEvents;
    if(abs(SusyMotherMass-momMass) > 0.001 || abs(SusyLSPMass-nlspMass) > 0.001) continue;

    process = true;
    wt=Weight*1000.0*lumiInfb*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));
    //-----------ISR reweighting for signal ------------------
    if(s_data.Contains("FastSim") && isrReweight){
      double isrWt = 0,isrWtCorr = h2_isrWtCorr->GetBinContent(h2_isrWtCorr->GetXaxis()->FindBin(SusyMotherMass),h2_isrWtCorr->GetYaxis()->FindBin(SusyLSPMass));

      if(NJetsISRJECdown>=6) isrWt = isrwt_arr[6];
      else isrWt = isrwt_arr[NJetsISRJECdown];
      wt = wt*isrWt*isrWtCorr;
    }
    //--------------------------------------------------------
    TLorentzVector bestPhoton=getBestPhoton();
    bool eMatchedG=check_eMatchedtoGamma();//this may not be necessary since e veto is there.
    if(bestPhoton.Pt()<99.9) continue;

    if( (Electrons->size() !=0) || (Muons->size() !=0) ) continue;
    if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;
    
    bool noFakeJet = true;
    if(s_data.Contains("FastSim")){
      //      //reject events with any jet pt>20, |eta|<2.5 NOT matched to a GenJet (w/in DeltaR<0.3) and chfrac < 0.1
      for(unsigned j = 0; j < JetsJECdown->size(); ++j){
      	if(JetsJECdown->at(j).Pt() <= 20 || fabs(JetsJECdown->at(j).Eta())>=2.5) continue;
      	bool genMatched = false;
      	for(unsigned g = 0; g < GenJets->size(); ++g){
      	  if(GenJets->at(g).DeltaR(JetsJECdown->at(j)) < 0.3) {
      	    genMatched = true;
      	    break;
      	  }
      	}
      	if(!genMatched && JetsJECdown_chargedHadronEnergyFraction->at(j) < 0.1){
      	  noFakeJet = false;
      	  break;
      	}
      }
    }

    bool hadJetID=true;
    int minDRindx=-100,photonMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<JetsJECdown->size();i++){
      if( ((*JetsJECdown)[i].Pt() > MHT_PtCut) && (abs((*JetsJECdown)[i].Eta()) <= HT_EtaCut) ){
	double dR=bestPhoton.DeltaR((*JetsJECdown)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    for(int i=0;i<JetsJECdown->size();i++){
      if( ((*JetsJECdown)[i].Pt() > MHT_PtCut) && (abs((*JetsJECdown)[i].Eta()) <= HT_EtaCut) ){
	if( !(minDR < 0.3 && i==minDRindx) ){
	  hadJets.push_back((*JetsJECdown)[i]);
	  if(hadJetID) hadJetID=(*JetsJECdown_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) photonMatchingJetIndx=minDRindx;
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
    }
    sortTLorVec(&hadJets);
    
    double dphi1=3.8,dphi2=3.8;
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi((*METPhiDown)[1],(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi((*METPhiDown)[1],(hadJets)[1].Phi()));
    
    if(photonMatchingJetIndx>=0 && ((*JetsJECdown)[photonMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    if(photonMatchingJetIndx<0) continue;

    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    process = process && !eMatchedG && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && (*METDown)[1] > 100 && dphi1 > 0.3 && dphi2 > 0.3;

    if(s_data.Contains("FastSim")){
      hadJetID = true;
      if(!noFakeJet) continue;
    }
    if(process){
      evtSurvived++;
      //------------------------ Sbins----------------------------
      int sBin7 = getBinNoV7(nHadJets,(*METDown)[1]);
      //------------------------ Sbins----------------------------
      h_SBins_v7_JECdn_CD->Fill(sBin7,wt);
    }//process
  }//loop over entries, JEC down
  /////////////////////////////////////////////////////////////////////////////////
  ////////////////////// JER up ///////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  cout<<"Doing JER up"<<endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if(!do_JERup) break;

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
  
    NumEvents = nevents;
    CrossSection = xsec1;
    Weight = CrossSection/NumEvents;
    if(abs(SusyMotherMass-momMass) > 0.001 || abs(SusyLSPMass-nlspMass) > 0.001) continue;

    process = true;
    wt=Weight*1000.0*lumiInfb*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));
    //-----------ISR reweighting for signal ------------------
    if(s_data.Contains("FastSim") && isrReweight){
      double isrWt = 0,isrWtCorr = h2_isrWtCorr->GetBinContent(h2_isrWtCorr->GetXaxis()->FindBin(SusyMotherMass),h2_isrWtCorr->GetYaxis()->FindBin(SusyLSPMass));

      if(NJetsISRJERup>=6) isrWt = isrwt_arr[6];
      else isrWt = isrwt_arr[NJetsISRJERup];
      wt = wt*isrWt*isrWtCorr;
    }
    //--------------------------------------------------------
    TLorentzVector bestPhoton=getBestPhoton();
    bool eMatchedG=check_eMatchedtoGamma();//this may not be necessary since e veto is there.
    if(bestPhoton.Pt()<99.9) continue;

    if( (Electrons->size() !=0) || (Muons->size() !=0) ) continue;
    if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;
    
    bool noFakeJet = true;
    if(s_data.Contains("FastSim")){
      //      //reject events with any jet pt>20, |eta|<2.5 NOT matched to a GenJet (w/in DeltaR<0.3) and chfrac < 0.1
      for(unsigned j = 0; j < JetsJERup->size(); ++j){
      	if(JetsJERup->at(j).Pt() <= 20 || fabs(JetsJERup->at(j).Eta())>=2.5) continue;
      	bool genMatched = false;
      	for(unsigned g = 0; g < GenJets->size(); ++g){
      	  if(GenJets->at(g).DeltaR(JetsJERup->at(j)) < 0.3) {
      	    genMatched = true;
      	    break;
      	  }
      	}
      	if(!genMatched && JetsJERup_chargedHadronEnergyFraction->at(j) < 0.1){
      	  noFakeJet = false;
      	  break;
      	}
      }
    }

    bool hadJetID=true;
    int minDRindx=-100,photonMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<JetsJERup->size();i++){
      if( ((*JetsJERup)[i].Pt() > MHT_PtCut) && (abs((*JetsJERup)[i].Eta()) <= HT_EtaCut) ){
	double dR=bestPhoton.DeltaR((*JetsJERup)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    for(int i=0;i<JetsJERup->size();i++){
      if( ((*JetsJERup)[i].Pt() > MHT_PtCut) && (abs((*JetsJERup)[i].Eta()) <= HT_EtaCut) ){
	if( !(minDR < 0.3 && i==minDRindx) ){
	  hadJets.push_back((*JetsJERup)[i]);
	  if(hadJetID) hadJetID=(*JetsJERup_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) photonMatchingJetIndx=minDRindx;
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
    }
    sortTLorVec(&hadJets);
    
    double dphi1=3.8,dphi2=3.8;
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi((*METPhiUp)[0],(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi((*METPhiUp)[0],(hadJets)[1].Phi()));
     
    if(photonMatchingJetIndx>=0 && ((*JetsJERup)[photonMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    if(photonMatchingJetIndx<0) continue;

    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    process = process && !eMatchedG && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && (*METUp)[0] > 100 && dphi1 > 0.3 && dphi2 > 0.3;

    if(s_data.Contains("FastSim")){
      hadJetID = true;
      if(!noFakeJet) continue;
    }
    if(process){
      evtSurvived++;
      //------------------------ Sbins----------------------------
      int sBin7 = getBinNoV7(nHadJets,(*METUp)[0]);
      //------------------------ Sbins----------------------------
      h_SBins_v7_JERup_CD->Fill(sBin7,wt);
    }//process
  }//loop entries, jec up
  /////////////////////////////////////////////////////////////////////////////////
  ////////////////////// JER Down ///////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  cout<<"Doing JER down"<<endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if(!do_JERdn) break;

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    NumEvents = nevents;
    CrossSection = xsec1;
    Weight = CrossSection/NumEvents;
    if(abs(SusyMotherMass-momMass) > 0.001 || abs(SusyLSPMass-nlspMass) > 0.001) continue;

    process = true;
    wt=Weight*1000.0*lumiInfb*(puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))));
    //-----------ISR reweighting for signal ------------------
    if(s_data.Contains("FastSim") && isrReweight){
      double isrWt = 0,isrWtCorr = h2_isrWtCorr->GetBinContent(h2_isrWtCorr->GetXaxis()->FindBin(SusyMotherMass),h2_isrWtCorr->GetYaxis()->FindBin(SusyLSPMass));

      if(NJetsISRJERdown>=6) isrWt = isrwt_arr[6];
      else isrWt = isrwt_arr[NJetsISRJERdown];
      wt = wt*isrWt*isrWtCorr;
    }
    //--------------------------------------------------------
    TLorentzVector bestPhoton=getBestPhoton();
    bool eMatchedG=check_eMatchedtoGamma();//this may not be necessary since e veto is there.
    if(bestPhoton.Pt()<99.9) continue;

    if( (Electrons->size() !=0) || (Muons->size() !=0) ) continue;
    if(isoElectronTracks!=0 || isoMuonTracks!=0 || isoPionTracks!=0) continue;
    
    bool noFakeJet = true;
    if(s_data.Contains("FastSim")){
      //      //reject events with any jet pt>20, |eta|<2.5 NOT matched to a GenJet (w/in DeltaR<0.3) and chfrac < 0.1
      for(unsigned j = 0; j < JetsJERdown->size(); ++j){
      	if(JetsJERdown->at(j).Pt() <= 20 || fabs(JetsJERdown->at(j).Eta())>=2.5) continue;
      	bool genMatched = false;
      	for(unsigned g = 0; g < GenJets->size(); ++g){
      	  if(GenJets->at(g).DeltaR(JetsJERdown->at(j)) < 0.3) {
      	    genMatched = true;
      	    break;
      	  }
      	}
      	if(!genMatched && JetsJERdown_chargedHadronEnergyFraction->at(j) < 0.1){
      	  noFakeJet = false;
      	  break;
      	}
      }
    }

    bool hadJetID=true;
    int minDRindx=-100,photonMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0;
    vector<TLorentzVector> hadJets;

    for(int i=0;i<JetsJERdown->size();i++){
      if( ((*JetsJERdown)[i].Pt() > MHT_PtCut) && (abs((*JetsJERdown)[i].Eta()) <= HT_EtaCut) ){
	double dR=bestPhoton.DeltaR((*JetsJERdown)[i]);
	if(dR<minDR){minDR=dR;minDRindx=i;}
      }
    }
    for(int i=0;i<JetsJERdown->size();i++){
      if( ((*JetsJERdown)[i].Pt() > MHT_PtCut) && (abs((*JetsJERdown)[i].Eta()) <= HT_EtaCut) ){
	if( !(minDR < 0.3 && i==minDRindx) ){
	  hadJets.push_back((*JetsJERdown)[i]);
	  if(hadJetID) hadJetID=(*JetsJERdown_ID)[i];
	}
      }
    }
    if( minDR<0.3 ) photonMatchingJetIndx=minDRindx;
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < HT_EtaCut) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < Njets_EtaCut) ){nHadJets++;}
    }
    if( minDR<0.3 ){
      ST=ST+bestPhoton.Pt();
    }
    sortTLorVec(&hadJets);
    
    double dphi1=3.8,dphi2=3.8;
    if(hadJets.size() > 0 ) dphi1 = abs(DeltaPhi((*METPhiDown)[0],(hadJets)[0].Phi()));
    if(hadJets.size() > 1 ) dphi2 = abs(DeltaPhi((*METPhiDown)[0],(hadJets)[1].Phi()));
    
    if(photonMatchingJetIndx>=0 && ((*JetsJERdown)[photonMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    if(photonMatchingJetIndx<0) continue;

    if( !((ST>800 && bestPhoton.Pt()>100) || (bestPhoton.Pt()>190)) ) continue;
    process = process && !eMatchedG && bestPhoton.Pt()>=100 && (Electrons->size()==0) && (Muons->size()==0) && ST>500 && nHadJets>=2 && (*METDown)[0] > 100 && dphi1 > 0.3 && dphi2 > 0.3;

    if(s_data.Contains("FastSim")){
      hadJetID = true;
      if(!noFakeJet) continue;
    }
    if(process){
      evtSurvived++;
      //------------------------ Sbins----------------------------
      int sBin7 = getBinNoV7(nHadJets,(*METDown)[0]);
      //------------------------ Sbins----------------------------
      h_SBins_v7_JERdn_CD->Fill(sBin7,wt);
    }//process
  }//loop over entries, JER down

}


TLorentzVector SignalSyst::getBestPhoton(){
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  allBestPhotons.resize(0);
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( (*Photons_fullID)[iPho] && ((*Photons_hasPixelSeed)[iPho]<0.001) ) {
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

int SignalSyst::getBinNoV7(int nHadJets, double met){
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
      if(met >= METBinLowEdgeV7_njLow[i] && MET < METBinLowEdgeV7_njLow[i+1]){ sBin = sBin+m_i;break; }
      else if(met >= METBinLowEdgeV7_njLow[METBinLowEdgeV7_njLow.size()-1])  { sBin = 6         ;break; }
    }
  }
  else{
    for(int i=0;i<METBinLowEdgeV7.size()-1;i++){
      if(METBinLowEdgeV7[i]<99.99) continue;
      m_i++;
      if(met >= METBinLowEdgeV7[i] && met < METBinLowEdgeV7[i+1]){ sBin = sBin+m_i;break; }
      else if(met >= METBinLowEdgeV7[METBinLowEdgeV7.size()-1])  { sBin = sBin+5   ;break; }
    }
  }
  return sBin;
}

bool SignalSyst::check_eMatchedtoGamma(){
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

void SignalSyst::print(Long64_t jentry){
  //cout<<endl;
  TLorentzVector v1,photo;
  for(int i=0;i<GenParticles->size();i++){
    //    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<</*"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<*/"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
  }

  for(int i=0;i<GenJets->size();i++){
    if(i==0) cout<<"-------------------------------- GenJets -------------------------------------------"<<endl;
    cout<<"GenJetsPt:"<<(*GenJets)[i].Pt()<<" eta:"<<(*GenJets)[i].Eta()<<" phi:"<<(*GenJets)[i].Phi()<<endl;
  }
  
  for(int i=0;i<Photons->size();i++){
    if(i==0) cout<<"-------------------------------- Photons -------------------------------------------"<<endl;
    double dR=0;//DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*Photons)[i].Eta(),(*Photons)[i].Phi() );
    //    cout<<jentry<<" i:"<<i<<" phoSize:"<<Photons->size()<<" Pt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" otherP:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" dR:"<<dR<<endl;
    cout<<"Photon Pt:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" E:"<<(*Photons)[i].Energy()<<" FullID:"<<(*Photons_fullID)[i]<<" Pixel Seed:"<<(*Photons_hasPixelSeed)[i]<<endl;
  }
  
  // for(int i=0;i<TAPElectronTracks->size();i++){
  //   cout<<"Ele Track Pt:"<<(*TAPElectronTracks)[i].Pt()<<" Eta:"<<(*TAPElectronTracks)[i].Eta()<<" Phi:"<<(*TAPElectronTracks)[i].Phi()<<endl;
  // }
  // for(int i=0;i<TAPMuonTracks->size();i++){
  //   cout<<"Mu Track Pt:"<<(*TAPMuonTracks)[i].Pt()<<" Eta:"<<(*TAPMuonTracks)[i].Eta()<<" Phi:"<<(*TAPMuonTracks)[i].Phi()<<endl;
  // }
  // for(int i=0;i<TAPPionTracks->size();i++){
  //   cout<<"Pi Track Pt:"<<(*TAPPionTracks)[i].Pt()<<" Eta:"<<(*TAPPionTracks)[i].Eta()<<" Phi:"<<(*TAPPionTracks)[i].Phi()<<endl;
  // }

  for(int i=0;i<Jets->size();i++){
    //  if( ((*Jets)[i].Pt() > MHT_PtCut) && (abs((*Jets)[i].Eta()) <= MHT_EtaCut) ){
    cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<" energy:"<<(*Jets)[i].Energy()<<" QMult:"<<(*Jets_chargedMultiplicity)[i]<<" CSV:"<<(*Jets_bDiscriminatorCSV)[i]<<" dPhi with photon:"<<(*Jets)[i].DeltaPhi((*Photons)[bestPhotonIndxAmongPhotons])<<endl;
  }
  
  for(int i=0;i<Muons->size();i++){
    if(i==0) cout<<"-------------------------------- Muons -------------------------------------------"<<endl;
    cout<<"MuonPt: "<<(*Muons)[i].Pt()<<" Eta: "<<(*Muons)[i].Eta()<<" Phi: "<<(*Muons)[i].Phi()<<" M: "<<(*Muons)[i].M()<<" E:"<<(*Muons)[i].Energy()<<endl;
  }
  for(int i=0;i<Electrons->size();i++){
    if(i==0) cout<<"-------------------------------- Electrons -------------------------------------------"<<endl;
    cout<<"ElectronPt: "<<(*Electrons)[i].Pt()<<" Eta: "<<(*Electrons)[i].Eta()<<" Phi: "<<(*Electrons)[i].Phi()<<" M: "<<(*Electrons)[i].M()<<" E:"<<(*Electrons)[i].Energy()<<endl;
  }
  cout<<"MET:"<<MET<<" METPhi:"<<METPhi<<endl;
  cout<<"^^^^^^^^^^^^^^^^^^ Event ends ^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl<<endl;
}
