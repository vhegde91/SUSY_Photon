//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct  3 10:14:47 2017 by ROOT version 6.10/02
// from TTree tnpTree/tree for e faking photon measurement
// found on file: /home/vinay/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/MS_EleFakes_v2/tnpTrees/MS_FR_DYJetsToLL_HT-100toInf_tnpTree.root
//////////////////////////////////////////////////////////

#ifndef a_h
#define a_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class a {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxrunnum = 1;
   static constexpr Int_t kMaxevtnum = 1;
   static constexpr Int_t kMaxst = 1;
   static constexpr Int_t kMaxpho = 1;
   static constexpr Int_t kMaxisbg = 1;
   static constexpr Int_t kMaxmet = 1;
   static constexpr Int_t kMaxnhadjets = 1;
   static constexpr Int_t kMaxprobept = 1;
   static constexpr Int_t kMaxtagpt = 1;
   static constexpr Int_t kMaxprobeeta = 1;
   static constexpr Int_t kMaxtageta = 1;
   static constexpr Int_t kMaxprobephi = 1;
   static constexpr Int_t kMaxtagphi = 1;
   static constexpr Int_t kMaxqmultprobe = 1;
   static constexpr Int_t kMaxqmulttag = 1;
   static constexpr Int_t kMaxzmass = 1;
   static constexpr Int_t kMaxzmasspho = 1;
   static constexpr Int_t kMaxzmassele = 1;
   static constexpr Int_t kMaxwt = 1;

   // Declaration of leaf types
   Int_t           RunNum;
   Int_t           EvtNum;
   Double_t        ST;
   Char_t          ProbeIsPhoton;
   Int_t           IsBGEvent;
   Double_t        MET;
   Int_t           HadJets;
   Double_t        ProbePt;
   Double_t        TagPt;
   Double_t        ProbeEta;
   Double_t        TagEta;
   Double_t        ProbePhi;
   Double_t        TagPhi;
   Int_t           QMultProbeJet;
   Int_t           QMultTagJet;
   Double_t        ZMass;
   Double_t        ZMass_Pho;
   Double_t        ZMass_Ele;
   Double_t        EvtWt;

   // List of branches
   TBranch        *b_runnum_;   //!
   TBranch        *b_evtnum_;   //!
   TBranch        *b_st_;   //!
   TBranch        *b_pho_;   //!
   TBranch        *b_isbg_;   //!
   TBranch        *b_met_;   //!
   TBranch        *b_nhadjets_;   //!
   TBranch        *b_probept_;   //!
   TBranch        *b_tagpt_;   //!
   TBranch        *b_probeeta_;   //!
   TBranch        *b_tageta_;   //!
   TBranch        *b_probephi_;   //!
   TBranch        *b_tagphi_;   //!
   TBranch        *b_qmultprobe_;   //!
   TBranch        *b_qmulttag_;   //!
   TBranch        *b_zmass_;   //!
   TBranch        *b_zmasspho_;   //!
   TBranch        *b_zmassele_;   //!
   TBranch        *b_wt_;   //!

   a(TTree *tree=0);
   virtual ~a();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef a_cxx
a::a(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/vinay/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/MS_EleFakes_v2/tnpTrees/MS_FR_DYJetsToLL_HT-100toInf_tnpTree.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/vinay/Phy_Work/ROOT_Files/Physics/GMSB_susy/GMSB_skims_ST_RA2b_TreesV12/MS_EleFakes_v2/tnpTrees/MS_FR_DYJetsToLL_HT-100toInf_tnpTree.root");
      }
      f->GetObject("tnpTree",tree);

   }
   Init(tree);
}

a::~a()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t a::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t a::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void a::Init(TTree *tree)
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

   fChain->SetBranchAddress("RunNum", &RunNum, &b_runnum_);
   fChain->SetBranchAddress("EvtNum", &EvtNum, &b_evtnum_);
   fChain->SetBranchAddress("ST", &ST, &b_st_);
   fChain->SetBranchAddress("ProbeIsPhoton", &ProbeIsPhoton, &b_pho_);
   fChain->SetBranchAddress("IsBGEvent", &IsBGEvent, &b_isbg_);
   fChain->SetBranchAddress("MET", &MET, &b_met_);
   fChain->SetBranchAddress("HadJets", &HadJets, &b_nhadjets_);
   fChain->SetBranchAddress("ProbePt", &ProbePt, &b_probept_);
   fChain->SetBranchAddress("TagPt", &TagPt, &b_tagpt_);
   fChain->SetBranchAddress("ProbeEta", &ProbeEta, &b_probeeta_);
   fChain->SetBranchAddress("TagEta", &TagEta, &b_tageta_);
   fChain->SetBranchAddress("ProbePhi", &ProbePhi, &b_probephi_);
   fChain->SetBranchAddress("TagPhi", &TagPhi, &b_tagphi_);
   fChain->SetBranchAddress("QMultProbeJet", &QMultProbeJet, &b_qmultprobe_);
   fChain->SetBranchAddress("QMultTagJet", &QMultTagJet, &b_qmulttag_);
   fChain->SetBranchAddress("ZMass", &ZMass, &b_zmass_);
   fChain->SetBranchAddress("ZMass_Pho", &ZMass_Pho, &b_zmasspho_);
   fChain->SetBranchAddress("ZMass_Ele", &ZMass_Ele, &b_zmassele_);
   fChain->SetBranchAddress("EvtWt", &EvtWt, &b_wt_);
   Notify();
}

Bool_t a::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void a::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t a::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef a_cxx
