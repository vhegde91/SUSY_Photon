//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Nov  4 01:48:45 2016 by ROOT version 6.06/01
// from TTree PreSelection/PreSelection
// found on file: root://cmseos.fnal.gov//store/user/vhegde/myProduction_V11/Spring16.WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_96_RA2AnalysisTree.root
//////////////////////////////////////////////////////////
#ifndef NtupleVariables_h
#define NtupleVariables_h

#include <TROOT.h>
#include <TChain.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

using namespace std;
class NtupleVariables : public TSelector {
 public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          RunNum;
   UInt_t          LumiBlockNum;
   ULong64_t       EvtNum;
   Bool_t          BadChargedCandidateFilter;
   Bool_t          BadGlobalMuon;
   Double_t        BadGlobalMuonLeadPt;
   Int_t           badMuonsFilter;
   Bool_t          BadPFMuonFilter;
   Bool_t          BadTrkGlobalMuon;
   Double_t        BadTrkGlobalMuonLeadPt;
   Int_t           BTags;
   Int_t           BTagsclean;
   Int_t           BTagsMVA;
   Int_t           BTagsMVAclean;
   Double_t        CaloMET;
   Double_t        CaloMETPhi;
   Int_t           CSCTightHaloFilter;
   Double_t        DeltaPhi1;
   Double_t        DeltaPhi1clean;
   Double_t        DeltaPhi1Orig;
   Double_t        DeltaPhi1Origclean;
   Double_t        DeltaPhi2;
   Double_t        DeltaPhi2clean;
   Double_t        DeltaPhi2Orig;
   Double_t        DeltaPhi2Origclean;
   Double_t        DeltaPhi3;
   Double_t        DeltaPhi3clean;
   Double_t        DeltaPhi3Orig;
   Double_t        DeltaPhi3Origclean;
   Double_t        DeltaPhi4;
   Double_t        DeltaPhi4clean;
   Double_t        DeltaPhi4Orig;
   Double_t        DeltaPhi4Origclean;
   Bool_t          DupGlobalMuon;
   Double_t        DupGlobalMuonLeadPt;
   Int_t           duplicateMuonsFilter;
   Int_t           EcalDeadCellTriggerPrimitiveFilter;
   Int_t           eeBadScFilter;
   vector<TLorentzVector> *Electrons;
   vector<int>     *Electrons_charge;
   vector<bool>    *Electrons_mediumID;
   vector<double>  *Electrons_MT2Activity;
   vector<double>  *Electrons_MTW;
   vector<double>  *Electrons_PTW;
   vector<bool>    *Electrons_tightID;
   vector<TLorentzVector> *ElectronsNoIso;
   vector<bool>    *ElectronsNoIso_mediumID;
   vector<double>  *ElectronsNoIso_MiniIso;
   vector<double>  *ElectronsNoIso_MT2Activity;
   vector<double>  *ElectronsNoIso_MTW;
   vector<bool>    *ElectronsNoIso_tightID;
   Int_t           globalTightHalo2016Filter;
   Bool_t          hasGenPromptPhoton;
   Int_t           HBHEIsoNoiseFilter;
   Int_t           HBHENoiseFilter;
   Double_t        HT;
   Double_t        HTclean;
   Double_t        HTOnline;
   Int_t           isoElectronTracks;
   Int_t           isoElectronTracksclean;
   Int_t           isoMuonTracks;
   Int_t           isoMuonTracksclean;
   Int_t           isoPionTracks;
   Int_t           isoPionTracksclean;
   Bool_t          JetID;
   Bool_t          JetIDAK8;
   Bool_t          JetIDAK8Clean;
   Bool_t          JetIDclean;
   vector<TLorentzVector> *Jets;
   vector<double>  *Jets_bDiscriminatorCSV;
   vector<double>  *Jets_bDiscriminatorMVA;
   vector<double>  *Jets_chargedEmEnergyFraction;
   vector<double>  *Jets_chargedHadronEnergyFraction;
   vector<int>     *Jets_chargedHadronMultiplicity;
   vector<int>     *Jets_chargedMultiplicity;
   vector<int>     *Jets_electronMultiplicity;
   vector<int>     *Jets_hadronFlavor;
   vector<bool>    *Jets_HTMask;
   vector<bool>    *Jets_ID;
   vector<double>  *Jets_jecFactor;
   vector<double>  *Jets_jecUnc;
   vector<bool>    *Jets_LeptonMask;
   vector<bool>    *Jets_MHTMask;
   vector<bool>    *Jets_MHTOrigMask;
   vector<double>  *Jets_muonEnergyFraction;
   vector<int>     *Jets_muonMultiplicity;
   vector<double>  *Jets_neutralEmEnergyFraction;
   vector<double>  *Jets_neutralHadronEnergyFraction;
   vector<int>     *Jets_neutralHadronMultiplicity;
   vector<int>     *Jets_neutralMultiplicity;
   vector<int>     *Jets_partonFlavor;
   vector<double>  *Jets_photonEnergyFraction;
   vector<int>     *Jets_photonMultiplicity;
   vector<double>  *Jets_qgAxis2;
   vector<double>  *Jets_qgLikelihood;
   vector<int>     *Jets_qgMult;
   vector<double>  *Jets_qgPtD;
   vector<TLorentzVector> *JetsAK8;
   vector<double>  *JetsAK8_bDiscriminatorSubjet1CSV;
   vector<double>  *JetsAK8_bDiscriminatorSubjet2CSV;
   vector<double>  *JetsAK8_doubleBDiscriminator;
   vector<bool>    *JetsAK8_ID;
   vector<double>  *JetsAK8_NsubjettinessTau1;
   vector<double>  *JetsAK8_NsubjettinessTau2;
   vector<double>  *JetsAK8_NsubjettinessTau3;
   vector<int>     *JetsAK8_NumBhadrons;
   vector<int>     *JetsAK8_NumChadrons;
   vector<double>  *JetsAK8_prunedMass;
   vector<TLorentzVector> *JetsAK8Clean;
   vector<double>  *JetsAK8Clean_doubleBDiscriminator;
   vector<bool>    *JetsAK8Clean_ID;
   vector<double>  *JetsAK8Clean_NsubjettinessTau1;
   vector<double>  *JetsAK8Clean_NsubjettinessTau2;
   vector<double>  *JetsAK8Clean_NsubjettinessTau3;
   vector<int>     *JetsAK8Clean_NumBhadrons;
   vector<int>     *JetsAK8Clean_NumChadrons;
   vector<double>  *JetsAK8Clean_prunedMass;
   vector<TLorentzVector> *Jetsclean;
   vector<double>  *Jetsclean_bDiscriminatorCSV;
   vector<double>  *Jetsclean_bDiscriminatorMVA;
   vector<double>  *Jetsclean_chargedHadronEnergyFraction;
   vector<int>     *Jetsclean_hadronFlavor;
   vector<bool>    *Jetsclean_HTMask;
   vector<bool>    *Jetsclean_ID;
   vector<bool>    *Jetsclean_MHTMask;
   vector<bool>    *Jetsclean_MHTOrigMask;
   vector<double>  *Jetsclean_muonEnergyFraction;
   vector<int>     *Jetsclean_partonFlavor;
   Double_t        MET;
   Double_t        METclean;
   Double_t        METPhi;
   Double_t        METPhiclean;
   Double_t        MHT;
   Double_t        MHTclean;
   Double_t        MHTOnline;
   Double_t        MHTOrig;
   Double_t        MHTOrigclean;
   Double_t        MHTPhi;
   Double_t        MHTPhiclean;
   Double_t        MHTPhiOrig;
   Double_t        MHTPhiOrigclean;
   Double_t        MT2;
   vector<TLorentzVector> *Muons;
   vector<int>     *Muons_charge;
   vector<double>  *Muons_MT2Activity;
   vector<double>  *Muons_MTW;
   vector<double>  *Muons_PTW;
   vector<bool>    *Muons_tightID;
   vector<TLorentzVector> *MuonsNoIso;
   vector<double>  *MuonsNoIso_MiniIso;
   vector<double>  *MuonsNoIso_MT2Activity;
   vector<double>  *MuonsNoIso_MTW;
   vector<bool>    *MuonsNoIso_tightID;
   Int_t           nAllVertices;
   Int_t           NJets;
   Int_t           NJetsclean;
   Int_t           noBadMuonsFilter;
   Int_t           NVtx;
   Double_t        PFCaloMETRatio;
   vector<TLorentzVector> *Photons;
   vector<bool>    *Photons_electronFakes;
   vector<bool>    *Photons_fullID;
   vector<double>  *Photons_genMatched;
   vector<double>  *Photons_hadTowOverEM;
   vector<double>  *Photons_hasPixelSeed;
   vector<double>  *Photons_isEB;
   vector<bool>    *Photons_nonPrompt;
   vector<double>  *Photons_passElectronVeto;
   vector<double>  *Photons_pfChargedIso;
   vector<double>  *Photons_pfChargedIsoRhoCorr;
   vector<double>  *Photons_pfGammaIso;
   vector<double>  *Photons_pfGammaIsoRhoCorr;
   vector<double>  *Photons_pfNeutralIso;
   vector<double>  *Photons_pfNeutralIsoRhoCorr;
   vector<double>  *Photons_sigmaIetaIeta;
   Double_t        PrescaleWeightHT;
   vector<TLorentzVector> *SoftJets;
   vector<double>  *SoftJets_bDiscriminatorCSV;
   vector<bool>    *SoftJets_ID;
   vector<double>  *SoftJets_jecFactor;
   vector<double>  *SoftJets_jecUnc;
   vector<TLorentzVector> *TAPElectronTracks;
   vector<double>  *TAPElectronTracks_activity;
   vector<int>     *TAPElectronTracks_charge;
   vector<double>  *TAPElectronTracks_mT;
   vector<double>  *TAPElectronTracks_trkiso;
   vector<TLorentzVector> *TAPMuonTracks;
   vector<double>  *TAPMuonTracks_activity;
   vector<int>     *TAPMuonTracks_charge;
   vector<double>  *TAPMuonTracks_mT;
   vector<double>  *TAPMuonTracks_trkiso;
   vector<TLorentzVector> *TAPPionTracks;
   vector<double>  *TAPPionTracks_activity;
   vector<int>     *TAPPionTracks_charge;
   vector<double>  *TAPPionTracks_mT;
   vector<double>  *TAPPionTracks_trkiso;
   vector<string>  *TriggerNames;
   vector<int>     *TriggerPass;
   vector<int>     *TriggerPrescales;
   vector<TLorentzVector> *ZCandidates;

   // List of branches
   TBranch        *b_RunNum;   //!
   TBranch        *b_LumiBlockNum;   //!
   TBranch        *b_EvtNum;   //!
   TBranch        *b_BadChargedCandidateFilter;   //!
   TBranch        *b_BadGlobalMuon;   //!
   TBranch        *b_BadGlobalMuonLeadPt;   //!
   TBranch        *b_badMuonsFilter;   //!
   TBranch        *b_BadPFMuonFilter;   //!
   TBranch        *b_BadTrkGlobalMuon;   //!
   TBranch        *b_BadTrkGlobalMuonLeadPt;   //!
   TBranch        *b_BTags;   //!
   TBranch        *b_BTagsclean;   //!
   TBranch        *b_BTagsMVA;   //!
   TBranch        *b_BTagsMVAclean;   //!
   TBranch        *b_CaloMET;   //!
   TBranch        *b_CaloMETPhi;   //!
   TBranch        *b_CSCTightHaloFilter;   //!
   TBranch        *b_DeltaPhi1;   //!
   TBranch        *b_DeltaPhi1clean;   //!
   TBranch        *b_DeltaPhi1Orig;   //!
   TBranch        *b_DeltaPhi1Origclean;   //!
   TBranch        *b_DeltaPhi2;   //!
   TBranch        *b_DeltaPhi2clean;   //!
   TBranch        *b_DeltaPhi2Orig;   //!
   TBranch        *b_DeltaPhi2Origclean;   //!
   TBranch        *b_DeltaPhi3;   //!
   TBranch        *b_DeltaPhi3clean;   //!
   TBranch        *b_DeltaPhi3Orig;   //!
   TBranch        *b_DeltaPhi3Origclean;   //!
   TBranch        *b_DeltaPhi4;   //!
   TBranch        *b_DeltaPhi4clean;   //!
   TBranch        *b_DeltaPhi4Orig;   //!
   TBranch        *b_DeltaPhi4Origclean;   //!
   TBranch        *b_DupGlobalMuon;   //!
   TBranch        *b_DupGlobalMuonLeadPt;   //!
   TBranch        *b_duplicateMuonsFilter;   //!
   TBranch        *b_EcalDeadCellTriggerPrimitiveFilter;   //!
   TBranch        *b_eeBadScFilter;   //!
   TBranch        *b_Electrons;   //!
   TBranch        *b_Electrons_charge;   //!
   TBranch        *b_Electrons_mediumID;   //!
   TBranch        *b_Electrons_MT2Activity;   //!
   TBranch        *b_Electrons_MTW;   //!
   TBranch        *b_Electrons_PTW;   //!
   TBranch        *b_Electrons_tightID;   //!
   TBranch        *b_ElectronsNoIso;   //!
   TBranch        *b_ElectronsNoIso_mediumID;   //!
   TBranch        *b_ElectronsNoIso_MiniIso;   //!
   TBranch        *b_ElectronsNoIso_MT2Activity;   //!
   TBranch        *b_ElectronsNoIso_MTW;   //!
   TBranch        *b_ElectronsNoIso_tightID;   //!
   TBranch        *b_globalTightHalo2016Filter;   //!
   TBranch        *b_hasGenPromptPhoton;   //!
   TBranch        *b_HBHEIsoNoiseFilter;   //!
   TBranch        *b_HBHENoiseFilter;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_HTclean;   //!
   TBranch        *b_HTOnline;   //!
   TBranch        *b_isoElectronTracks;   //!
   TBranch        *b_isoElectronTracksclean;   //!
   TBranch        *b_isoMuonTracks;   //!
   TBranch        *b_isoMuonTracksclean;   //!
   TBranch        *b_isoPionTracks;   //!
   TBranch        *b_isoPionTracksclean;   //!
   TBranch        *b_JetID;   //!
   TBranch        *b_JetIDAK8;   //!
   TBranch        *b_JetIDAK8Clean;   //!
   TBranch        *b_JetIDclean;   //!
   TBranch        *b_Jets;   //!
   TBranch        *b_Jets_bDiscriminatorCSV;   //!
   TBranch        *b_Jets_bDiscriminatorMVA;   //!
   TBranch        *b_Jets_chargedEmEnergyFraction;   //!
   TBranch        *b_Jets_chargedHadronEnergyFraction;   //!
   TBranch        *b_Jets_chargedHadronMultiplicity;   //!
   TBranch        *b_Jets_chargedMultiplicity;   //!
   TBranch        *b_Jets_electronMultiplicity;   //!
   TBranch        *b_Jets_hadronFlavor;   //!
   TBranch        *b_Jets_HTMask;   //!
   TBranch        *b_Jets_ID;   //!
   TBranch        *b_Jets_jecFactor;   //!
   TBranch        *b_Jets_jecUnc;   //!
   TBranch        *b_Jets_LeptonMask;   //!
   TBranch        *b_Jets_MHTMask;   //!
   TBranch        *b_Jets_MHTOrigMask;   //!
   TBranch        *b_Jets_muonEnergyFraction;   //!
   TBranch        *b_Jets_muonMultiplicity;   //!
   TBranch        *b_Jets_neutralEmEnergyFraction;   //!
   TBranch        *b_Jets_neutralHadronEnergyFraction;   //!
   TBranch        *b_Jets_neutralHadronMultiplicity;   //!
   TBranch        *b_Jets_neutralMultiplicity;   //!
   TBranch        *b_Jets_partonFlavor;   //!
   TBranch        *b_Jets_photonEnergyFraction;   //!
   TBranch        *b_Jets_photonMultiplicity;   //!
   TBranch        *b_Jets_qgAxis2;   //!
   TBranch        *b_Jets_qgLikelihood;   //!
   TBranch        *b_Jets_qgMult;   //!
   TBranch        *b_Jets_qgPtD;   //!
   TBranch        *b_JetsAK8;   //!
   TBranch        *b_JetsAK8_bDiscriminatorSubjet1CSV;   //!
   TBranch        *b_JetsAK8_bDiscriminatorSubjet2CSV;   //!
   TBranch        *b_JetsAK8_doubleBDiscriminator;   //!
   TBranch        *b_JetsAK8_ID;   //!
   TBranch        *b_JetsAK8_NsubjettinessTau1;   //!
   TBranch        *b_JetsAK8_NsubjettinessTau2;   //!
   TBranch        *b_JetsAK8_NsubjettinessTau3;   //!
   TBranch        *b_JetsAK8_NumBhadrons;   //!
   TBranch        *b_JetsAK8_NumChadrons;   //!
   TBranch        *b_JetsAK8_prunedMass;   //!
   TBranch        *b_JetsAK8Clean;   //!
   TBranch        *b_JetsAK8Clean_doubleBDiscriminator;   //!
   TBranch        *b_JetsAK8Clean_ID;   //!
   TBranch        *b_JetsAK8Clean_NsubjettinessTau1;   //!
   TBranch        *b_JetsAK8Clean_NsubjettinessTau2;   //!
   TBranch        *b_JetsAK8Clean_NsubjettinessTau3;   //!
   TBranch        *b_JetsAK8Clean_NumBhadrons;   //!
   TBranch        *b_JetsAK8Clean_NumChadrons;   //!
   TBranch        *b_JetsAK8Clean_prunedMass;   //!
   TBranch        *b_Jetsclean;   //!
   TBranch        *b_Jetsclean_bDiscriminatorCSV;   //!
   TBranch        *b_Jetsclean_bDiscriminatorMVA;   //!
   TBranch        *b_Jetsclean_chargedHadronEnergyFraction;   //!
   TBranch        *b_Jetsclean_hadronFlavor;   //!
   TBranch        *b_Jetsclean_HTMask;   //!
   TBranch        *b_Jetsclean_ID;   //!
   TBranch        *b_Jetsclean_MHTMask;   //!
   TBranch        *b_Jetsclean_MHTOrigMask;   //!
   TBranch        *b_Jetsclean_muonEnergyFraction;   //!
   TBranch        *b_Jetsclean_partonFlavor;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_METclean;   //!
   TBranch        *b_METPhi;   //!
   TBranch        *b_METPhiclean;   //!
   TBranch        *b_MHT;   //!
   TBranch        *b_MHTclean;   //!
   TBranch        *b_MHTOnline;   //!
   TBranch        *b_MHTOrig;   //!
   TBranch        *b_MHTOrigclean;   //!
   TBranch        *b_MHTPhi;   //!
   TBranch        *b_MHTPhiclean;   //!
   TBranch        *b_MHTPhiOrig;   //!
   TBranch        *b_MHTPhiOrigclean;   //!
   TBranch        *b_MT2;   //!
   TBranch        *b_Muons;   //!
   TBranch        *b_Muons_charge;   //!
   TBranch        *b_Muons_MT2Activity;   //!
   TBranch        *b_Muons_MTW;   //!
   TBranch        *b_Muons_PTW;   //!
   TBranch        *b_Muons_tightID;   //!
   TBranch        *b_MuonsNoIso;   //!
   TBranch        *b_MuonsNoIso_MiniIso;   //!
   TBranch        *b_MuonsNoIso_MT2Activity;   //!
   TBranch        *b_MuonsNoIso_MTW;   //!
   TBranch        *b_MuonsNoIso_tightID;   //!
   TBranch        *b_nAllVertices;   //!
   TBranch        *b_NJets;   //!
   TBranch        *b_NJetsclean;   //!
   TBranch        *b_noBadMuonsFilter;   //!
   TBranch        *b_NVtx;   //!
   TBranch        *b_PFCaloMETRatio;   //!
   TBranch        *b_Photons;   //!
   TBranch        *b_Photons_electronFakes;   //!
   TBranch        *b_Photons_fullID;   //!
   TBranch        *b_Photons_genMatched;   //!
   TBranch        *b_Photons_hadTowOverEM;   //!
   TBranch        *b_Photons_hasPixelSeed;   //!
   TBranch        *b_Photons_isEB;   //!
   TBranch        *b_Photons_nonPrompt;   //!
   TBranch        *b_Photons_passElectronVeto;   //!
   TBranch        *b_Photons_pfChargedIso;   //!
   TBranch        *b_Photons_pfChargedIsoRhoCorr;   //!
   TBranch        *b_Photons_pfGammaIso;   //!
   TBranch        *b_Photons_pfGammaIsoRhoCorr;   //!
   TBranch        *b_Photons_pfNeutralIso;   //!
   TBranch        *b_Photons_pfNeutralIsoRhoCorr;   //!
   TBranch        *b_Photons_sigmaIetaIeta;   //!
   TBranch        *b_PrescaleWeightHT;   //!
   TBranch        *b_SoftJets;   //!
   TBranch        *b_SoftJets_bDiscriminatorCSV;   //!
   TBranch        *b_SoftJets_ID;   //!
   TBranch        *b_SoftJets_jecFactor;   //!
   TBranch        *b_SoftJets_jecUnc;   //!
   TBranch        *b_TAPElectronTracks;   //!
   TBranch        *b_TAPElectronTracks_activity;   //!
   TBranch        *b_TAPElectronTracks_charge;   //!
   TBranch        *b_TAPElectronTracks_mT;   //!
   TBranch        *b_TAPElectronTracks_trkiso;   //!
   TBranch        *b_TAPMuonTracks;   //!
   TBranch        *b_TAPMuonTracks_activity;   //!
   TBranch        *b_TAPMuonTracks_charge;   //!
   TBranch        *b_TAPMuonTracks_mT;   //!
   TBranch        *b_TAPMuonTracks_trkiso;   //!
   TBranch        *b_TAPPionTracks;   //!
   TBranch        *b_TAPPionTracks_activity;   //!
   TBranch        *b_TAPPionTracks_charge;   //!
   TBranch        *b_TAPPionTracks_mT;   //!
   TBranch        *b_TAPPionTracks_trkiso;   //!
   TBranch        *b_TriggerNames;   //!
   TBranch        *b_TriggerPass;   //!
   TBranch        *b_TriggerPrescales;   //!
   TBranch        *b_ZCandidates;   //!
   
 NtupleVariables(TTree * /*tree*/ =0) : fChain(0) { }
   ~NtupleVariables() { }
   void    Init(TTree *tree, string);
   Bool_t  Notify();
   Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   double  DeltaPhi(double, double);
   double  DeltaR(double eta1, double phi1, double eta2, double phi2);
   void    sortTLorVec(vector<TLorentzVector> *);   
};

#endif
#ifdef NtupleVariables_cxx
void NtupleVariables::Init(TTree *tree, string nameData)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

   // Set object pointer
   Electrons = 0;
   Electrons_charge = 0;
   Electrons_mediumID = 0;
   Electrons_MT2Activity = 0;
   Electrons_MTW = 0;
   Electrons_PTW = 0;
   Electrons_tightID = 0;
   ElectronsNoIso = 0;
   ElectronsNoIso_mediumID = 0;
   ElectronsNoIso_MiniIso = 0;
   ElectronsNoIso_MT2Activity = 0;
   ElectronsNoIso_MTW = 0;
   ElectronsNoIso_tightID = 0;
   Jets = 0;
   Jets_bDiscriminatorCSV = 0;
   Jets_bDiscriminatorMVA = 0;
   Jets_chargedEmEnergyFraction = 0;
   Jets_chargedHadronEnergyFraction = 0;
   Jets_chargedHadronMultiplicity = 0;
   Jets_chargedMultiplicity = 0;
   Jets_electronMultiplicity = 0;
   Jets_hadronFlavor = 0;
   Jets_HTMask = 0;
   Jets_ID = 0;
   Jets_jecFactor = 0;
   Jets_jecUnc = 0;
   Jets_LeptonMask = 0;
   Jets_MHTMask = 0;
   Jets_MHTOrigMask = 0;
   Jets_muonEnergyFraction = 0;
   Jets_muonMultiplicity = 0;
   Jets_neutralEmEnergyFraction = 0;
   Jets_neutralHadronEnergyFraction = 0;
   Jets_neutralHadronMultiplicity = 0;
   Jets_neutralMultiplicity = 0;
   Jets_partonFlavor = 0;
   Jets_photonEnergyFraction = 0;
   Jets_photonMultiplicity = 0;
   Jets_qgAxis2 = 0;
   Jets_qgLikelihood = 0;
   Jets_qgMult = 0;
   Jets_qgPtD = 0;
   JetsAK8 = 0;
   JetsAK8_bDiscriminatorSubjet1CSV = 0;
   JetsAK8_bDiscriminatorSubjet2CSV = 0;
   JetsAK8_doubleBDiscriminator = 0;
   JetsAK8_ID = 0;
   JetsAK8_NsubjettinessTau1 = 0;
   JetsAK8_NsubjettinessTau2 = 0;
   JetsAK8_NsubjettinessTau3 = 0;
   JetsAK8_NumBhadrons = 0;
   JetsAK8_NumChadrons = 0;
   JetsAK8_prunedMass = 0;
   JetsAK8Clean = 0;
   JetsAK8Clean_doubleBDiscriminator = 0;
   JetsAK8Clean_ID = 0;
   JetsAK8Clean_NsubjettinessTau1 = 0;
   JetsAK8Clean_NsubjettinessTau2 = 0;
   JetsAK8Clean_NsubjettinessTau3 = 0;
   JetsAK8Clean_NumBhadrons = 0;
   JetsAK8Clean_NumChadrons = 0;
   JetsAK8Clean_prunedMass = 0;
   Jetsclean = 0;
   Jetsclean_bDiscriminatorCSV = 0;
   Jetsclean_bDiscriminatorMVA = 0;
   Jetsclean_chargedHadronEnergyFraction = 0;
   Jetsclean_hadronFlavor = 0;
   Jetsclean_HTMask = 0;
   Jetsclean_ID = 0;
   Jetsclean_MHTMask = 0;
   Jetsclean_MHTOrigMask = 0;
   Jetsclean_muonEnergyFraction = 0;
   Jetsclean_partonFlavor = 0;
   Muons = 0;
   Muons_charge = 0;
   Muons_MT2Activity = 0;
   Muons_MTW = 0;
   Muons_PTW = 0;
   Muons_tightID = 0;
   MuonsNoIso = 0;
   MuonsNoIso_MiniIso = 0;
   MuonsNoIso_MT2Activity = 0;
   MuonsNoIso_MTW = 0;
   MuonsNoIso_tightID = 0;
   Photons = 0;
   Photons_electronFakes = 0;
   Photons_fullID = 0;
   Photons_genMatched = 0;
   Photons_hadTowOverEM = 0;
   Photons_hasPixelSeed = 0;
   Photons_isEB = 0;
   Photons_nonPrompt = 0;
   Photons_passElectronVeto = 0;
   Photons_pfChargedIso = 0;
   Photons_pfChargedIsoRhoCorr = 0;
   Photons_pfGammaIso = 0;
   Photons_pfGammaIsoRhoCorr = 0;
   Photons_pfNeutralIso = 0;
   Photons_pfNeutralIsoRhoCorr = 0;
   Photons_sigmaIetaIeta = 0;
   SoftJets = 0;
   SoftJets_bDiscriminatorCSV = 0;
   SoftJets_ID = 0;
   SoftJets_jecFactor = 0;
   SoftJets_jecUnc = 0;
   TAPElectronTracks = 0;
   TAPElectronTracks_activity = 0;
   TAPElectronTracks_charge = 0;
   TAPElectronTracks_mT = 0;
   TAPElectronTracks_trkiso = 0;
   TAPMuonTracks = 0;
   TAPMuonTracks_activity = 0;
   TAPMuonTracks_charge = 0;
   TAPMuonTracks_mT = 0;
   TAPMuonTracks_trkiso = 0;
   TAPPionTracks = 0;
   TAPPionTracks_activity = 0;
   TAPPionTracks_charge = 0;
   TAPPionTracks_mT = 0;
   TAPPionTracks_trkiso = 0;
   TriggerNames = 0;
   TriggerPass = 0;
   TriggerPrescales = 0;
   ZCandidates = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("RunNum", &RunNum, &b_RunNum);
   fChain->SetBranchAddress("LumiBlockNum", &LumiBlockNum, &b_LumiBlockNum);
   fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
   fChain->SetBranchAddress("BadChargedCandidateFilter", &BadChargedCandidateFilter, &b_BadChargedCandidateFilter);
   fChain->SetBranchAddress("BadGlobalMuon", &BadGlobalMuon, &b_BadGlobalMuon);
   fChain->SetBranchAddress("BadGlobalMuonLeadPt", &BadGlobalMuonLeadPt, &b_BadGlobalMuonLeadPt);
   fChain->SetBranchAddress("badMuonsFilter", &badMuonsFilter, &b_badMuonsFilter);
   fChain->SetBranchAddress("BadPFMuonFilter", &BadPFMuonFilter, &b_BadPFMuonFilter);
   fChain->SetBranchAddress("BadTrkGlobalMuon", &BadTrkGlobalMuon, &b_BadTrkGlobalMuon);
   fChain->SetBranchAddress("BadTrkGlobalMuonLeadPt", &BadTrkGlobalMuonLeadPt, &b_BadTrkGlobalMuonLeadPt);
   fChain->SetBranchAddress("BTags", &BTags, &b_BTags);
   fChain->SetBranchAddress("BTagsclean", &BTagsclean, &b_BTagsclean);
   fChain->SetBranchAddress("BTagsMVA", &BTagsMVA, &b_BTagsMVA);
   fChain->SetBranchAddress("BTagsMVAclean", &BTagsMVAclean, &b_BTagsMVAclean);
   fChain->SetBranchAddress("CaloMET", &CaloMET, &b_CaloMET);
   fChain->SetBranchAddress("CaloMETPhi", &CaloMETPhi, &b_CaloMETPhi);
   fChain->SetBranchAddress("CSCTightHaloFilter", &CSCTightHaloFilter, &b_CSCTightHaloFilter);
   fChain->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
   fChain->SetBranchAddress("DeltaPhi1clean", &DeltaPhi1clean, &b_DeltaPhi1clean);
   fChain->SetBranchAddress("DeltaPhi1Orig", &DeltaPhi1Orig, &b_DeltaPhi1Orig);
   fChain->SetBranchAddress("DeltaPhi1Origclean", &DeltaPhi1Origclean, &b_DeltaPhi1Origclean);
   fChain->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
   fChain->SetBranchAddress("DeltaPhi2clean", &DeltaPhi2clean, &b_DeltaPhi2clean);
   fChain->SetBranchAddress("DeltaPhi2Orig", &DeltaPhi2Orig, &b_DeltaPhi2Orig);
   fChain->SetBranchAddress("DeltaPhi2Origclean", &DeltaPhi2Origclean, &b_DeltaPhi2Origclean);
   fChain->SetBranchAddress("DeltaPhi3", &DeltaPhi3, &b_DeltaPhi3);
   fChain->SetBranchAddress("DeltaPhi3clean", &DeltaPhi3clean, &b_DeltaPhi3clean);
   fChain->SetBranchAddress("DeltaPhi3Orig", &DeltaPhi3Orig, &b_DeltaPhi3Orig);
   fChain->SetBranchAddress("DeltaPhi3Origclean", &DeltaPhi3Origclean, &b_DeltaPhi3Origclean);
   fChain->SetBranchAddress("DeltaPhi4", &DeltaPhi4, &b_DeltaPhi4);
   fChain->SetBranchAddress("DeltaPhi4clean", &DeltaPhi4clean, &b_DeltaPhi4clean);
   fChain->SetBranchAddress("DeltaPhi4Orig", &DeltaPhi4Orig, &b_DeltaPhi4Orig);
   fChain->SetBranchAddress("DeltaPhi4Origclean", &DeltaPhi4Origclean, &b_DeltaPhi4Origclean);
   fChain->SetBranchAddress("DupGlobalMuon", &DupGlobalMuon, &b_DupGlobalMuon);
   fChain->SetBranchAddress("DupGlobalMuonLeadPt", &DupGlobalMuonLeadPt, &b_DupGlobalMuonLeadPt);
   fChain->SetBranchAddress("duplicateMuonsFilter", &duplicateMuonsFilter, &b_duplicateMuonsFilter);
   fChain->SetBranchAddress("EcalDeadCellTriggerPrimitiveFilter", &EcalDeadCellTriggerPrimitiveFilter, &b_EcalDeadCellTriggerPrimitiveFilter);
   fChain->SetBranchAddress("eeBadScFilter", &eeBadScFilter, &b_eeBadScFilter);
   fChain->SetBranchAddress("Electrons", &Electrons, &b_Electrons);
   fChain->SetBranchAddress("Electrons_charge", &Electrons_charge, &b_Electrons_charge);
   fChain->SetBranchAddress("Electrons_mediumID", &Electrons_mediumID, &b_Electrons_mediumID);
   fChain->SetBranchAddress("Electrons_MT2Activity", &Electrons_MT2Activity, &b_Electrons_MT2Activity);
   fChain->SetBranchAddress("Electrons_MTW", &Electrons_MTW, &b_Electrons_MTW);
   fChain->SetBranchAddress("Electrons_PTW", &Electrons_PTW, &b_Electrons_PTW);
   fChain->SetBranchAddress("Electrons_tightID", &Electrons_tightID, &b_Electrons_tightID);
   fChain->SetBranchAddress("ElectronsNoIso", &ElectronsNoIso, &b_ElectronsNoIso);
   fChain->SetBranchAddress("ElectronsNoIso_mediumID", &ElectronsNoIso_mediumID, &b_ElectronsNoIso_mediumID);
   fChain->SetBranchAddress("ElectronsNoIso_MiniIso", &ElectronsNoIso_MiniIso, &b_ElectronsNoIso_MiniIso);
   fChain->SetBranchAddress("ElectronsNoIso_MT2Activity", &ElectronsNoIso_MT2Activity, &b_ElectronsNoIso_MT2Activity);
   fChain->SetBranchAddress("ElectronsNoIso_MTW", &ElectronsNoIso_MTW, &b_ElectronsNoIso_MTW);
   fChain->SetBranchAddress("ElectronsNoIso_tightID", &ElectronsNoIso_tightID, &b_ElectronsNoIso_tightID);
   fChain->SetBranchAddress("globalTightHalo2016Filter", &globalTightHalo2016Filter, &b_globalTightHalo2016Filter);
   fChain->SetBranchAddress("hasGenPromptPhoton", &hasGenPromptPhoton, &b_hasGenPromptPhoton);
   fChain->SetBranchAddress("HBHEIsoNoiseFilter", &HBHEIsoNoiseFilter, &b_HBHEIsoNoiseFilter);
   fChain->SetBranchAddress("HBHENoiseFilter", &HBHENoiseFilter, &b_HBHENoiseFilter);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("HTclean", &HTclean, &b_HTclean);
   fChain->SetBranchAddress("HTOnline", &HTOnline, &b_HTOnline);
   fChain->SetBranchAddress("isoElectronTracks", &isoElectronTracks, &b_isoElectronTracks);
   fChain->SetBranchAddress("isoElectronTracksclean", &isoElectronTracksclean, &b_isoElectronTracksclean);
   fChain->SetBranchAddress("isoMuonTracks", &isoMuonTracks, &b_isoMuonTracks);
   fChain->SetBranchAddress("isoMuonTracksclean", &isoMuonTracksclean, &b_isoMuonTracksclean);
   fChain->SetBranchAddress("isoPionTracks", &isoPionTracks, &b_isoPionTracks);
   fChain->SetBranchAddress("isoPionTracksclean", &isoPionTracksclean, &b_isoPionTracksclean);
   fChain->SetBranchAddress("JetID", &JetID, &b_JetID);
   fChain->SetBranchAddress("JetIDAK8", &JetIDAK8, &b_JetIDAK8);
   fChain->SetBranchAddress("JetIDAK8Clean", &JetIDAK8Clean, &b_JetIDAK8Clean);
   fChain->SetBranchAddress("JetIDclean", &JetIDclean, &b_JetIDclean);
   fChain->SetBranchAddress("Jets", &Jets, &b_Jets);
   fChain->SetBranchAddress("Jets_bDiscriminatorCSV", &Jets_bDiscriminatorCSV, &b_Jets_bDiscriminatorCSV);
   fChain->SetBranchAddress("Jets_bDiscriminatorMVA", &Jets_bDiscriminatorMVA, &b_Jets_bDiscriminatorMVA);
   fChain->SetBranchAddress("Jets_chargedEmEnergyFraction", &Jets_chargedEmEnergyFraction, &b_Jets_chargedEmEnergyFraction);
   fChain->SetBranchAddress("Jets_chargedHadronEnergyFraction", &Jets_chargedHadronEnergyFraction, &b_Jets_chargedHadronEnergyFraction);
   fChain->SetBranchAddress("Jets_chargedHadronMultiplicity", &Jets_chargedHadronMultiplicity, &b_Jets_chargedHadronMultiplicity);
   fChain->SetBranchAddress("Jets_chargedMultiplicity", &Jets_chargedMultiplicity, &b_Jets_chargedMultiplicity);
   fChain->SetBranchAddress("Jets_electronMultiplicity", &Jets_electronMultiplicity, &b_Jets_electronMultiplicity);
   fChain->SetBranchAddress("Jets_hadronFlavor", &Jets_hadronFlavor, &b_Jets_hadronFlavor);
   fChain->SetBranchAddress("Jets_HTMask", &Jets_HTMask, &b_Jets_HTMask);
   fChain->SetBranchAddress("Jets_ID", &Jets_ID, &b_Jets_ID);
   fChain->SetBranchAddress("Jets_jecFactor", &Jets_jecFactor, &b_Jets_jecFactor);
   fChain->SetBranchAddress("Jets_jecUnc", &Jets_jecUnc, &b_Jets_jecUnc);
   fChain->SetBranchAddress("Jets_LeptonMask", &Jets_LeptonMask, &b_Jets_LeptonMask);
   fChain->SetBranchAddress("Jets_MHTMask", &Jets_MHTMask, &b_Jets_MHTMask);
   fChain->SetBranchAddress("Jets_MHTOrigMask", &Jets_MHTOrigMask, &b_Jets_MHTOrigMask);
   fChain->SetBranchAddress("Jets_muonEnergyFraction", &Jets_muonEnergyFraction, &b_Jets_muonEnergyFraction);
   fChain->SetBranchAddress("Jets_muonMultiplicity", &Jets_muonMultiplicity, &b_Jets_muonMultiplicity);
   fChain->SetBranchAddress("Jets_neutralEmEnergyFraction", &Jets_neutralEmEnergyFraction, &b_Jets_neutralEmEnergyFraction);
   fChain->SetBranchAddress("Jets_neutralHadronEnergyFraction", &Jets_neutralHadronEnergyFraction, &b_Jets_neutralHadronEnergyFraction);
   fChain->SetBranchAddress("Jets_neutralHadronMultiplicity", &Jets_neutralHadronMultiplicity, &b_Jets_neutralHadronMultiplicity);
   fChain->SetBranchAddress("Jets_neutralMultiplicity", &Jets_neutralMultiplicity, &b_Jets_neutralMultiplicity);
   fChain->SetBranchAddress("Jets_partonFlavor", &Jets_partonFlavor, &b_Jets_partonFlavor);
   fChain->SetBranchAddress("Jets_photonEnergyFraction", &Jets_photonEnergyFraction, &b_Jets_photonEnergyFraction);
   fChain->SetBranchAddress("Jets_photonMultiplicity", &Jets_photonMultiplicity, &b_Jets_photonMultiplicity);
   fChain->SetBranchAddress("Jets_qgAxis2", &Jets_qgAxis2, &b_Jets_qgAxis2);
   fChain->SetBranchAddress("Jets_qgLikelihood", &Jets_qgLikelihood, &b_Jets_qgLikelihood);
   fChain->SetBranchAddress("Jets_qgMult", &Jets_qgMult, &b_Jets_qgMult);
   fChain->SetBranchAddress("Jets_qgPtD", &Jets_qgPtD, &b_Jets_qgPtD);
   fChain->SetBranchAddress("JetsAK8", &JetsAK8, &b_JetsAK8);
   fChain->SetBranchAddress("JetsAK8_bDiscriminatorSubjet1CSV", &JetsAK8_bDiscriminatorSubjet1CSV, &b_JetsAK8_bDiscriminatorSubjet1CSV);
   fChain->SetBranchAddress("JetsAK8_bDiscriminatorSubjet2CSV", &JetsAK8_bDiscriminatorSubjet2CSV, &b_JetsAK8_bDiscriminatorSubjet2CSV);
   fChain->SetBranchAddress("JetsAK8_doubleBDiscriminator", &JetsAK8_doubleBDiscriminator, &b_JetsAK8_doubleBDiscriminator);
   fChain->SetBranchAddress("JetsAK8_ID", &JetsAK8_ID, &b_JetsAK8_ID);
   fChain->SetBranchAddress("JetsAK8_NsubjettinessTau1", &JetsAK8_NsubjettinessTau1, &b_JetsAK8_NsubjettinessTau1);
   fChain->SetBranchAddress("JetsAK8_NsubjettinessTau2", &JetsAK8_NsubjettinessTau2, &b_JetsAK8_NsubjettinessTau2);
   fChain->SetBranchAddress("JetsAK8_NsubjettinessTau3", &JetsAK8_NsubjettinessTau3, &b_JetsAK8_NsubjettinessTau3);
   fChain->SetBranchAddress("JetsAK8_NumBhadrons", &JetsAK8_NumBhadrons, &b_JetsAK8_NumBhadrons);
   fChain->SetBranchAddress("JetsAK8_NumChadrons", &JetsAK8_NumChadrons, &b_JetsAK8_NumChadrons);
   fChain->SetBranchAddress("JetsAK8_prunedMass", &JetsAK8_prunedMass, &b_JetsAK8_prunedMass);
   fChain->SetBranchAddress("JetsAK8Clean", &JetsAK8Clean, &b_JetsAK8Clean);
   fChain->SetBranchAddress("JetsAK8Clean_doubleBDiscriminator", &JetsAK8Clean_doubleBDiscriminator, &b_JetsAK8Clean_doubleBDiscriminator);
   fChain->SetBranchAddress("JetsAK8Clean_ID", &JetsAK8Clean_ID, &b_JetsAK8Clean_ID);
   fChain->SetBranchAddress("JetsAK8Clean_NsubjettinessTau1", &JetsAK8Clean_NsubjettinessTau1, &b_JetsAK8Clean_NsubjettinessTau1);
   fChain->SetBranchAddress("JetsAK8Clean_NsubjettinessTau2", &JetsAK8Clean_NsubjettinessTau2, &b_JetsAK8Clean_NsubjettinessTau2);
   fChain->SetBranchAddress("JetsAK8Clean_NsubjettinessTau3", &JetsAK8Clean_NsubjettinessTau3, &b_JetsAK8Clean_NsubjettinessTau3);
   fChain->SetBranchAddress("JetsAK8Clean_NumBhadrons", &JetsAK8Clean_NumBhadrons, &b_JetsAK8Clean_NumBhadrons);
   fChain->SetBranchAddress("JetsAK8Clean_NumChadrons", &JetsAK8Clean_NumChadrons, &b_JetsAK8Clean_NumChadrons);
   fChain->SetBranchAddress("JetsAK8Clean_prunedMass", &JetsAK8Clean_prunedMass, &b_JetsAK8Clean_prunedMass);
   fChain->SetBranchAddress("Jetsclean", &Jetsclean, &b_Jetsclean);
   fChain->SetBranchAddress("Jetsclean_bDiscriminatorCSV", &Jetsclean_bDiscriminatorCSV, &b_Jetsclean_bDiscriminatorCSV);
   fChain->SetBranchAddress("Jetsclean_bDiscriminatorMVA", &Jetsclean_bDiscriminatorMVA, &b_Jetsclean_bDiscriminatorMVA);
   fChain->SetBranchAddress("Jetsclean_chargedHadronEnergyFraction", &Jetsclean_chargedHadronEnergyFraction, &b_Jetsclean_chargedHadronEnergyFraction);
   fChain->SetBranchAddress("Jetsclean_hadronFlavor", &Jetsclean_hadronFlavor, &b_Jetsclean_hadronFlavor);
   fChain->SetBranchAddress("Jetsclean_HTMask", &Jetsclean_HTMask, &b_Jetsclean_HTMask);
   fChain->SetBranchAddress("Jetsclean_ID", &Jetsclean_ID, &b_Jetsclean_ID);
   fChain->SetBranchAddress("Jetsclean_MHTMask", &Jetsclean_MHTMask, &b_Jetsclean_MHTMask);
   fChain->SetBranchAddress("Jetsclean_MHTOrigMask", &Jetsclean_MHTOrigMask, &b_Jetsclean_MHTOrigMask);
   fChain->SetBranchAddress("Jetsclean_muonEnergyFraction", &Jetsclean_muonEnergyFraction, &b_Jetsclean_muonEnergyFraction);
   fChain->SetBranchAddress("Jetsclean_partonFlavor", &Jetsclean_partonFlavor, &b_Jetsclean_partonFlavor);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("METclean", &METclean, &b_METclean);
   fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
   fChain->SetBranchAddress("METPhiclean", &METPhiclean, &b_METPhiclean);
   fChain->SetBranchAddress("MHT", &MHT, &b_MHT);
   fChain->SetBranchAddress("MHTclean", &MHTclean, &b_MHTclean);
   fChain->SetBranchAddress("MHTOnline", &MHTOnline, &b_MHTOnline);
   fChain->SetBranchAddress("MHTOrig", &MHTOrig, &b_MHTOrig);
   fChain->SetBranchAddress("MHTOrigclean", &MHTOrigclean, &b_MHTOrigclean);
   fChain->SetBranchAddress("MHTPhi", &MHTPhi, &b_MHTPhi);
   fChain->SetBranchAddress("MHTPhiclean", &MHTPhiclean, &b_MHTPhiclean);
   fChain->SetBranchAddress("MHTPhiOrig", &MHTPhiOrig, &b_MHTPhiOrig);
   fChain->SetBranchAddress("MHTPhiOrigclean", &MHTPhiOrigclean, &b_MHTPhiOrigclean);
   fChain->SetBranchAddress("MT2", &MT2, &b_MT2);
   fChain->SetBranchAddress("Muons", &Muons, &b_Muons);
   fChain->SetBranchAddress("Muons_charge", &Muons_charge, &b_Muons_charge);
   fChain->SetBranchAddress("Muons_MT2Activity", &Muons_MT2Activity, &b_Muons_MT2Activity);
   fChain->SetBranchAddress("Muons_MTW", &Muons_MTW, &b_Muons_MTW);
   fChain->SetBranchAddress("Muons_PTW", &Muons_PTW, &b_Muons_PTW);
   fChain->SetBranchAddress("Muons_tightID", &Muons_tightID, &b_Muons_tightID);
   fChain->SetBranchAddress("MuonsNoIso", &MuonsNoIso, &b_MuonsNoIso);
   fChain->SetBranchAddress("MuonsNoIso_MiniIso", &MuonsNoIso_MiniIso, &b_MuonsNoIso_MiniIso);
   fChain->SetBranchAddress("MuonsNoIso_MT2Activity", &MuonsNoIso_MT2Activity, &b_MuonsNoIso_MT2Activity);
   fChain->SetBranchAddress("MuonsNoIso_MTW", &MuonsNoIso_MTW, &b_MuonsNoIso_MTW);
   fChain->SetBranchAddress("MuonsNoIso_tightID", &MuonsNoIso_tightID, &b_MuonsNoIso_tightID);
   fChain->SetBranchAddress("nAllVertices", &nAllVertices, &b_nAllVertices);
   fChain->SetBranchAddress("NJets", &NJets, &b_NJets);
   fChain->SetBranchAddress("NJetsclean", &NJetsclean, &b_NJetsclean);
   fChain->SetBranchAddress("noBadMuonsFilter", &noBadMuonsFilter, &b_noBadMuonsFilter);
   fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
   fChain->SetBranchAddress("PFCaloMETRatio", &PFCaloMETRatio, &b_PFCaloMETRatio);
   fChain->SetBranchAddress("Photons", &Photons, &b_Photons);
   fChain->SetBranchAddress("Photons_electronFakes", &Photons_electronFakes, &b_Photons_electronFakes);
   fChain->SetBranchAddress("Photons_fullID", &Photons_fullID, &b_Photons_fullID);
   fChain->SetBranchAddress("Photons_genMatched", &Photons_genMatched, &b_Photons_genMatched);
   fChain->SetBranchAddress("Photons_hadTowOverEM", &Photons_hadTowOverEM, &b_Photons_hadTowOverEM);
   fChain->SetBranchAddress("Photons_hasPixelSeed", &Photons_hasPixelSeed, &b_Photons_hasPixelSeed);
   fChain->SetBranchAddress("Photons_isEB", &Photons_isEB, &b_Photons_isEB);
   fChain->SetBranchAddress("Photons_nonPrompt", &Photons_nonPrompt, &b_Photons_nonPrompt);
   fChain->SetBranchAddress("Photons_passElectronVeto", &Photons_passElectronVeto, &b_Photons_passElectronVeto);
   fChain->SetBranchAddress("Photons_pfChargedIso", &Photons_pfChargedIso, &b_Photons_pfChargedIso);
   fChain->SetBranchAddress("Photons_pfChargedIsoRhoCorr", &Photons_pfChargedIsoRhoCorr, &b_Photons_pfChargedIsoRhoCorr);
   fChain->SetBranchAddress("Photons_pfGammaIso", &Photons_pfGammaIso, &b_Photons_pfGammaIso);
   fChain->SetBranchAddress("Photons_pfGammaIsoRhoCorr", &Photons_pfGammaIsoRhoCorr, &b_Photons_pfGammaIsoRhoCorr);
   fChain->SetBranchAddress("Photons_pfNeutralIso", &Photons_pfNeutralIso, &b_Photons_pfNeutralIso);
   fChain->SetBranchAddress("Photons_pfNeutralIsoRhoCorr", &Photons_pfNeutralIsoRhoCorr, &b_Photons_pfNeutralIsoRhoCorr);
   fChain->SetBranchAddress("Photons_sigmaIetaIeta", &Photons_sigmaIetaIeta, &b_Photons_sigmaIetaIeta);
   fChain->SetBranchAddress("PrescaleWeightHT", &PrescaleWeightHT, &b_PrescaleWeightHT);
   fChain->SetBranchAddress("SoftJets", &SoftJets, &b_SoftJets);
   fChain->SetBranchAddress("SoftJets_bDiscriminatorCSV", &SoftJets_bDiscriminatorCSV, &b_SoftJets_bDiscriminatorCSV);
   fChain->SetBranchAddress("SoftJets_ID", &SoftJets_ID, &b_SoftJets_ID);
   fChain->SetBranchAddress("SoftJets_jecFactor", &SoftJets_jecFactor, &b_SoftJets_jecFactor);
   fChain->SetBranchAddress("SoftJets_jecUnc", &SoftJets_jecUnc, &b_SoftJets_jecUnc);
   fChain->SetBranchAddress("TAPElectronTracks", &TAPElectronTracks, &b_TAPElectronTracks);
   fChain->SetBranchAddress("TAPElectronTracks_activity", &TAPElectronTracks_activity, &b_TAPElectronTracks_activity);
   fChain->SetBranchAddress("TAPElectronTracks_charge", &TAPElectronTracks_charge, &b_TAPElectronTracks_charge);
   fChain->SetBranchAddress("TAPElectronTracks_mT", &TAPElectronTracks_mT, &b_TAPElectronTracks_mT);
   fChain->SetBranchAddress("TAPElectronTracks_trkiso", &TAPElectronTracks_trkiso, &b_TAPElectronTracks_trkiso);
   fChain->SetBranchAddress("TAPMuonTracks", &TAPMuonTracks, &b_TAPMuonTracks);
   fChain->SetBranchAddress("TAPMuonTracks_activity", &TAPMuonTracks_activity, &b_TAPMuonTracks_activity);
   fChain->SetBranchAddress("TAPMuonTracks_charge", &TAPMuonTracks_charge, &b_TAPMuonTracks_charge);
   fChain->SetBranchAddress("TAPMuonTracks_mT", &TAPMuonTracks_mT, &b_TAPMuonTracks_mT);
   fChain->SetBranchAddress("TAPMuonTracks_trkiso", &TAPMuonTracks_trkiso, &b_TAPMuonTracks_trkiso);
   fChain->SetBranchAddress("TAPPionTracks", &TAPPionTracks, &b_TAPPionTracks);
   fChain->SetBranchAddress("TAPPionTracks_activity", &TAPPionTracks_activity, &b_TAPPionTracks_activity);
   fChain->SetBranchAddress("TAPPionTracks_charge", &TAPPionTracks_charge, &b_TAPPionTracks_charge);
   fChain->SetBranchAddress("TAPPionTracks_mT", &TAPPionTracks_mT, &b_TAPPionTracks_mT);
   fChain->SetBranchAddress("TAPPionTracks_trkiso", &TAPPionTracks_trkiso, &b_TAPPionTracks_trkiso);
   fChain->SetBranchAddress("TriggerNames", &TriggerNames, &b_TriggerNames);
   fChain->SetBranchAddress("TriggerPass", &TriggerPass, &b_TriggerPass);
   fChain->SetBranchAddress("TriggerPrescales", &TriggerPrescales, &b_TriggerPrescales);
   fChain->SetBranchAddress("ZCandidates", &ZCandidates, &b_ZCandidates);
   Notify();
}

Bool_t NtupleVariables::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef temp_cxx
