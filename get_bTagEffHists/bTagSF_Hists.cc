#define bTagSF_Hists_cxx
#include "bTagSF_Hists.h"
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

  bTagSF_Hists ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void bTagSF_Hists::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  //  TTree *newtree = fChain->CloneTree(0);

  TString s_data=data;

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

    if(s_data.Contains("madHT0to600") && madHT > 600) continue;

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
  } // loop over entries
  //  newtree->AutoSave();
}
