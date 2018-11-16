#include<iostream>
#include<string>
#include<vector>
#include"TString.h"
#include"TH1.h"

void histToYaml(){
  TString fName = "SBinHists.root";
  TFile *f = new TFile(fName);
  TH1D *h;
  int nDecPl = 3;
  vector<TString> histName = {"AllSBins_v7_LElePred","AllSBins_v7_LMuPred","AllSBins_v7_FRPred","AllSBins_v7_ZGPred","AllSBins_v7_MultiJPred","AllSBins_v7_TotalBG","AllSBins_v7_Obs"};
  vector<TString> bgName = {"Lost e","'Lost $\\mu+\\tau_h$'","Misid. $\\gamma$","'$Z(\\nu\\bar{\\nu})+\\gamma$'","$\\gamma+$jets","Total Pred.","Data"};
  vector<TString> binName = {"'$N_{jets}=2-4,\\ N_{b-jets}=0,\\\\ p_T^{miss}=200-270$'",
			     "'$N_{jets}=2-4,\\ N_{b-jets}=0,\\\\ p_T^{miss}=270-350$'",
			     "'$N_{jets}=2-4,\\ N_{b-jets}=0,\\\\ p_T^{miss}=350-450$'",
			     "'$N_{jets}=2-4,\\ N_{b-jets}=0,\\\\ p_T^{miss}=450-750$'",
			     "'$N_{jets}=2-4,\\ N_{b-jets}=0,\\\\ p_T^{miss}\\geq750$'",
			     "'$N_{jets}=5-6,\\ N_{b-jets}=0,\\\\ p_T^{miss}=200-270$'",
			     "'$N_{jets}=5-6,\\ N_{b-jets}=0,\\\\ p_T^{miss}=270-350$'",
			     "'$N_{jets}=5-6,\\ N_{b-jets}=0,\\\\ p_T^{miss}=350-450$'",
			     "'$N_{jets}=5-6,\\ N_{b-jets}=0,\\\\ p_T^{miss}\\geq450$'",
			     "'$N_{jets}\\geq7,\\ N_{b-jets}=0,\\\\ p_T^{miss}=200-270$'",
			     "'$N_{jets}\\geq7,\\ N_{b-jets}=0,\\\\ p_T^{miss}=270-350$'",
			     "'$N_{jets}\\geq7,\\ N_{b-jets}=0,\\\\ p_T^{miss}=350-450$'",
			     "'$N_{jets}\\geq7,\\ N_{b-jets}=0,\\\\ p_T^{miss}\\geq450$'",
			     "'$N_{jets}=2-4,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=200-270$'",
			     "'$N_{jets}=2-4,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=270-350$'",
			     "'$N_{jets}=2-4,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=350-450$'",
			     "'$N_{jets}=2-4,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}\\geq450$'",
			     "'$N_{jets}=5-6,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=200-270$'",
			     "'$N_{jets}=5-6,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=270-350$'",
			     "'$N_{jets}=5-6,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=350-450$'",
			     "'$N_{jets}=5-6,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}\\geq450$'",
			     "'$N_{jets}\\geq7,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=200-270$'",
			     "'$N_{jets}\\geq7,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=270-350$'",
			     "'$N_{jets}\\geq7,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}=350-450$'",
			     "'$N_{jets}\\geq7,\\ N_{b-jets}\\geq1,\\\\ p_T^{miss}\\geq450$'"};
  // cout<<fName<<" No. of hists:"<<histName.size()<<" no. of BGs"<<bgName.size()<<" no. of bins:"<<binName.size()<<endl;
  // for(int i =0;i<bgName.size();i++) cout<<bgName[i]<<endl;
  // for(int i =0;i<binName.size();i++) cout<<binName[i]<<endl;
  ofstream ofile("allBGpredObs.yaml");
  ofile << "dependent_variables:\n";
  ofile << "- header: {name: Category}\n";
  ofile << "  values:\n";
  for(int i=0;i<binName.size();i++)
    ofile << "  - {value: "<<binName[i]<<"}\n";
  for(int i=0;i<bgName.size();i++){
    ofile << "- header: {name: "<<bgName[i]<<"}\n";
    h = (TH1D*)f->Get(histName[i]);
    ofile << "  values:\n";
    for(int j=1;j<=h->GetNbinsX();j++){
      if( j==1 || j==7 || j==12 || j==17 || j==22 || j==27 ) continue;
      if(bgName[i]!="Data"){
	ofile << "  - errors:\n";
	if( (h->GetBinContent(j) - h->GetBinError(j)) >= 0.0){
	  ofile << "    - {label: total, symerror: "<<std::fixed << std::setprecision(nDecPl)<<h->GetBinError(j)<<"}\n";
	}
	else{
	  ofile << "    - asymerror: {minus: "<<abs(h->GetBinContent(j))<<", plus: "<<h->GetBinError(j)<<"}\n";
	  ofile << "      label: total\n";
	}
	ofile << "    value: "<<std::fixed << std::setprecision(nDecPl)<<h->GetBinContent(j)<<endl;
      }
      else  ofile << "  - {value: "<<std::fixed << std::setprecision(0)<<h->GetBinContent(j)<<"}\n";
    }
  }
  ofile << "independent_variables:\n";
  ofile << "- header: {name: Bin no.}\n";
  ofile << "  values:\n";
  for(int i=0;i<binName.size();i++)
    ofile << "  - {value: "<<i+1<<"}\n";
  //  ----------------
}
