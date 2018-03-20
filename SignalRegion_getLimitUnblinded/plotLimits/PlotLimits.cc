#define PlotLimits_cxx
#include "PlotLimits.h"
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

  PlotLimits ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void PlotLimits::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  TFile *fxsec=TFile::Open("mGl_Xsecpb_absUnc.root");
  TH1D *h1_xsec=(TH1D*)fxsec->FindObjectAny("mGlXsec");

  string s_data=data;
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  int evtSurvived=0;

  vector<TString> fNames;
  ifstream filein(inputFileList);
  if(filein.is_open()){
    string line1;
    while(getline(filein,line1))
      fNames.push_back(line1);    
  }
  else cout<<"Could not open file: "<<inputFileList<<endl;
  filein.close();
  cout<<"Using Asymptotic "<<endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
   
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    // if (k > decade)
    //   cout << 10 * k << " %" <<endl;
    // decade = k;
    // cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //    cout<<fCurrent<<" "<<fNames[fCurrent]<<endl;

    //    if(quantileExpected > 0) continue;
    int m1=mh, m2=(mh-m1)*10000;
    double mGl=m1,mNLSP=round(m2),xsec=0,xsecUnc=0;
    xsec = h1_xsec->GetBinContent(h1_xsec->FindBin(mGl));
    xsecUnc = h1_xsec->GetBinError(h1_xsec->FindBin(mGl));
    if(mGl < 649) continue;
    if(limit > 100. || limit < 0.0000001) cout<<"Limit:"<<limit<<" (mGl,mNLSP):"<<mGl<<" "<<mNLSP<<endl;

    if(quantileExpected < 0){
      h2_mGlmNLSP_r->Fill(mGl,mNLSP,limit);
      h2_mGlmNLSP_XsecUL->Fill(mGl,mNLSP,xsec/limit);
      h2_mGlmNLSP_XsecUL_v1->Fill(mGl,mNLSP,xsec/limit);
      h2_mGlmNLSP_r_fb->Fill(mGl,mNLSP,limit);
      h2_mGlmNLSP_XsecUL_fb->Fill(mGl,mNLSP,xsec/limit);

      h2_mGlmNLSP_rUnc->Fill(mGl,mNLSP,limitErr);
      h2_mGlmNLSP_r_XsecUp->Fill(mGl,mNLSP,( limit * (xsec/(xsec+xsecUnc)) ));
      h2_mGlmNLSP_r_XsecDn->Fill(mGl,mNLSP,( limit * (xsec/(xsec-xsecUnc)) ));
    }
    if(abs(quantileExpected - 0.1599999) <= 0.0001){
      h2_mGlmNLSP_16pc->Fill(mGl,mNLSP,limit);
    }
    if(abs(quantileExpected - 0.8399999) <= 0.0001){
      h2_mGlmNLSP_84pc->Fill(mGl,mNLSP,limit);
    }
    if(abs(quantileExpected - 0.5) <= 0.0001){
      h2_mGlmNLSP_median->Fill(mGl,mNLSP,limit);
    }

    //    cout<<GluinoMass<<" "<<NLSPMass<<endl;
  }
}
