#define PickMassPoint_cxx
#include "PickMassPoint.h"
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

  if (argc < 4) {
    cerr << "Please give 5 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << " mass of gluino and mass of (N)LSP"<< endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *s_mGl         = argv[4];
  const char *s_mNLSP       = argv[5];

  PickMassPoint ana(inputFileList, outFileName, data, s_mGl, s_mNLSP);
  cout << "dataset " << data << " mGl: "<<s_mGl<<" mNLSP: "<<s_mNLSP << endl;

  double mGl=atof(s_mGl),mNLSP=atof(s_mNLSP);
  ana.EventLoop(data,inputFileList,mGl,mNLSP);

  return 0;
}

void PickMassPoint::EventLoop(const char *data,const char *inputFileList,const double mGl,const double mNLSP) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  //  cout << "nentries in the file " << nentries << endl;
  //  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  TTree *newtree = fChain->CloneTree(0);

  TString s_data = data;
  //-------------------- for signal only ---------------
  double momMass = mGl, nlspMass = mNLSP, nevents=0, xsec1=1.0, evtLeftAfterSkim=0;
  //------------------------ for strong production models ------------------------
  TFile *f1;
  if(s_data.Contains("T5bbbbZg")) f1 = new TFile("T5bbbbZg_MassScan.root");
  else if(s_data.Contains("T5ttttZg")) f1 = new TFile("T5ttttZg_MassScan.root");
  else if(s_data.Contains("T5qqqqHg")) f1 = new TFile("T5qqqqHg_MassScan.root");
  else {cout<<"AHHHH:Hist for Nevents and Xsec not found."<<endl; return;}

  TH1D *h1_xsechist = (TH1D*)f1->FindObjectAny("mGlXsec");
  TH2D *h2_mass = (TH2D*)f1->FindObjectAny("MGlMNLSP");
  if(!h1_xsechist){ cout<<"AHHHHH: could not find xsec hist"<<endl; return;}
  if(!h2_mass){ cout<<"AHHHHH: could not find no. of events hist"<<endl; return;}
  xsec1 = h1_xsechist->GetBinContent(h1_xsechist->FindBin(momMass));
  //------------------------ for EW production models ------------------------
  //TFile *f1 = new TFile("TChiNG_MassScan.root");
  //  TH1D *h1_xsechist = (TH1D*)f1->FindObjectAny("mEWeakino_N2C1Xsec");//N2C1 for TChiWG
  // TH1D *h1_xsechist = (TH1D*)f1->FindObjectAny("mEWeakino_SumN2C1_C1C1Xsec");//N2C1 for TChiNG
  // TH2D *h2_mass = (TH2D*)f1->FindObjectAny("MGlMNLSP");
  // if(!h1_xsechist){ cout<<"AHHHHH: could not find xsec hist"<<endl; return;}
  // if(!h2_mass){ cout<<"AHHHHH: could not find no. of events hist"<<endl; return;}
  // xsec1 = h1_xsechist->GetBinContent(h1_xsechist->FindBin(nlspMass));
  //----------------------- for Stop models ----------------------------------
  // TFile *f1 = new TFile("T6ttZg_MassScan.root");
  // TH1D *h1_xsechist = (TH1D*)f1->FindObjectAny("mStopXsec");
  // TH2D *h2_mass = (TH2D*)f1->FindObjectAny("MGlMNLSP");
  // if(!h1_xsechist){ cout<<"AHHHHH: could not find xsec hist"<<endl; return;}
  // if(!h2_mass){ cout<<"AHHHHH: could not find no. of events hist"<<endl; return;}
  // xsec1 = h1_xsechist->GetBinContent(h1_xsechist->FindBin(momMass));

  //------ get number of events ---------------
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
  //--------------------------------------------
    
  cout<<"********************"<<endl;
  cout<<"Gluino Mass: "<<momMass<<endl;
  cout<<"NLSP Mass: "<<nlspMass<<endl;
  cout<<"Produced no. of events in whole dataset: "<<nevents<<endl;
  cout<<"Xsec in pb: "<<xsec1<<endl;
  if(nevents<1) { cout<<"AHHHHH: No. of events for this mass point:"<<nevents<<endl; return;}  
  //  cout<<"No. of entries in whole dataset for mGluino = "<<momMass<<" mNLSP = "<<nlspMass<<" : "<<nevents<<endl;
  //-------------------- for signal only ends ------------------

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //    if(jentry>10) break;
    // ==============print number of events done == == == == == == == =
    // double progress = 10.0 * jentry / (1.0 * nentries);
    // int k = int (progress);
    // if (k > decade)
    //   cout << 10 * k << " %" <<endl;
    // decade = k;
    //cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
      NumEvents = nevents;
      CrossSection = xsec1;
      Weight = CrossSection/NumEvents;
      //---------------------------------------------------------
      //      cout<<"SusyMotherMass:"<<SusyMotherMass<<" SusyLSPMass:"<<SusyLSPMass<<endl;
      if(abs(SusyMotherMass-momMass) > 0.001 || abs(SusyLSPMass-nlspMass) > 0.001) continue;
      //      cout<<"SusyMotherMass:"<<SusyMotherMass<<" SusyLSPMass:"<<SusyLSPMass<<endl;
      //    }

    h_selectBaselineYields_->Fill(0);
    storeBTagEff();

    h_met0->Fill(MET);
    h_mht0->Fill(MHT);  
    h_ht0->Fill(HT);
    h_nj0->Fill(NJets);
    
    for(int ipho=0;ipho<Photons->size();ipho++){
      h_phopt0->Fill((*Photons)[ipho].Pt());
    }

    int nZbsn=0,nHbsn=0,nGamma=0;
    for(int i=0;i<GenParticles->size();i++){
      if( (abs((*GenParticles_PdgId)[i])==23) && (abs((*GenParticles_ParentId)[i])==1000023) ) {nZbsn++;}
      if( (abs((*GenParticles_PdgId)[i])==25) && (abs((*GenParticles_ParentId)[i])==1000023) ) {nHbsn++;}
      if( (abs((*GenParticles_PdgId)[i])==22) && (abs((*GenParticles_ParentId)[i])==1000023) ) {nGamma++;}
    }
    h_nZFmNLSP->Fill(nZbsn);
    h_nHFmNLSP->Fill(nHbsn);
    h_nGFmNLSP->Fill(nGamma);

    //end of select skimming parameters
    evtLeftAfterSkim+=1;
    newtree->Fill();
 
  } // loop over entries
  cout<<"no. of events left after skimming yyyyyyy: "<<evtLeftAfterSkim<<endl;
  cout<<"********************"<<endl;
  //  newtree->AutoSave();
}

void PickMassPoint::storeBTagEff(){
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
