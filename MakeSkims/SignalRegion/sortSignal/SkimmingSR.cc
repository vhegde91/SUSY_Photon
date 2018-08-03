#define SkimmingSR_cxx
#include "SkimmingSR.h"
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

  if (argc < 3) {
    cerr << "Please give 3 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];

  SkimmingSR ana(inputFileList, outFileName, data);
  cout << "dataset " << data << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void SkimmingSR::EventLoop(const char *data,const char *inputFileList){
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries in the file " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;
  
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  TTree *newtree = fChain->CloneTree(0);
  double xsec1=0.0, numevents=0., nlspMass=0., momMass=0.;
  double CSVv2WP = 0.8484;
  string s_data = data;
  //-------------------- for signal only ---------------
  //------------------------ for strong production models ------------------------
  // TFile *f1 = new TFile("T6ttZg_MassScan.root");
  // TH1D *h1_xsechist = (TH1D*)f1->FindObjectAny("mGlXsec");
  // TH2D *h2_mass = (TH2D*)f1->FindObjectAny("MGlMNLSP");
  // if(!h1_xsechist){ cout<<"AHHHHH: could not find xsec hist"<<endl; return;}
  // if(!h2_mass){ cout<<"AHHHHH: could not find no. of events hist"<<endl; return;}
  // xsec1 = h1_xsechist->GetBinContent(h1_xsechist->FindBin(momMass));
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
  //------------------------ for GGM models ----------------------------------
  TFile *f1 = new TFile("GGM_M1M3_MassScan.root");
  TH2D *h2_xsechist = (TH2D*)f1->FindObjectAny("GGM_M1M3_xsec");
  TH2D *h2_mass = (TH2D*)f1->FindObjectAny("MGlMNLSP");
  if(!h2_xsechist){ cout<<"AHHHHH: could not find xsec hist"<<endl; return;}
  if(!h2_mass){ cout<<"AHHHHH: could not find no. of events hist"<<endl; return;}
  xsec1 = h2_xsechist->GetBinContent(h2_xsechist->GetXaxis()->FindBin(momMass),h2_xsechist->GetYaxis()->FindBin(nlspMass));
  int nP=0;
  for(int i=1;i<=h2_mass->GetNbinsX();i++){
    for(int j=1;j<=h2_mass->GetNbinsY();j++){
      if(h2_mass->GetBinContent(i,j) < 0.1) continue;
      newTree2[nP] = fChain->CloneTree(0);
      nP++;
    }
  }
  
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
    if (ientry > 10) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    NumEvents = numevents;
    CrossSection = xsec1;
    Weight = CrossSection/NumEvents;

    cout<<jentry<<" SusyMotherMass:"<<SusyMotherMass<<" SusyLSPMass:"<<SusyLSPMass<<endl;
    int nP=0;
    for(int i=1;i<=h2_mass->GetNbinsX();i++){
      for(int j=1;j<=h2_mass->GetNbinsY();j++){
	if(h2_mass->GetBinContent(i,j) < 0.1) continue;
	if( (abs(h2_mass->GetXaxis()->GetBinCenter(i)-SusyMotherMass) < 0.1) &&
	    (abs(h2_mass->GetYaxis()->GetBinCenter(j)-SusyLSPMass)    < 0.1)){
	  cout<<nP<<" SusyMotherMass:"<<SusyMotherMass<<" SusyLSPMass:"<<SusyLSPMass<<endl;
	  i=1000000;
	  break;
	}
	else nP++;
      }
    }
    cout<<"nP:"<<nP<<endl;
    newTree2[nP]->Fill();	
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
	d_eff_b[nP]->Fill(pt,eta);
	if(csv > CSVv2WP) n_eff_b[nP]->Fill(pt,eta);
      }
      else if(flav==4){
	d_eff_c[nP]->Fill(pt,eta);
	if(csv > CSVv2WP) n_eff_c[nP]->Fill(pt,eta);
      }
      else if(flav<4 || flav==21){
	d_eff_udsg[nP]->Fill(pt,eta);
	if(csv > CSVv2WP) n_eff_udsg[nP]->Fill(pt,eta);
      }
    }
    
    //---------------------------------------------------------
    //      cout<<"SusyMotherMass:"<<SusyMotherMass<<" SusyLSPMass:"<<SusyLSPMass<<endl;
    //      cout<<"SusyMotherMass:"<<SusyMotherMass<<" SusyLSPMass:"<<SusyLSPMass<<endl;
    //    }

    //    storeBTagEff();
    
    //    newtree->Fill();
 
  } // loop over entries
  //  newtree->AutoSave();
}

void SkimmingSR::storeBTagEff(){
}
