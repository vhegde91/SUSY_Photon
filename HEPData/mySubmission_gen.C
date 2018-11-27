#include<iostream>
#include<string>
#include<vector>
#include"TString.h"

void mySubmission_gen(string fName){
  ifstream table_files(fName);
  ofstream submission("submission.yaml");
    
  // submission << "comment: | #Information that applies to all data tables." << "\n";
  // submission << "\n\n\n\n\n";
  submission << "additional_resources:"<<endl;
  submission << "- {description: web page with auxiliary material, location: 'http://cms-results.web.cern.ch/cms-results/public-results/preliminary-results/SUS-18-002/index.html'}"<<endl;
  submission << "comment: |"<<endl;
  submission << "  CERN-LHC. CMS. A search for supersymmetry is presented based on events with at least one photon, multiple jets, and large missing transverse momentum produced in proton-proton collisions at a center-of-mass energy of sqrt(s) = 13 TeV. The data correspond to an integrated luminosity of 35.9 1/fb and were recorded by the CMS detector in 2016 at the LHC. The analysis characterizes signal-like events by categorizing the data into various signal regions based on the number of jets, the number of b-tagged jets, and missing transverse momentum. No significant excess of events is observed with respect to expectations from standard model processes. Limits are placed on gluino, top squark, and neutralino masses using several simplified models of pair production of supersymmetric particles with gauge-mediated supersymmetry breaking. Depending on the model and the mass of the next-to-lightest supersymmetric particle, gluino masses as large as 2120 GeV and top squark masses as large as 1230 GeV are excluded."<<endl;
  // submission << "hepdata_doi: XYZ"<<endl;

  int i=1;
  string filename;

  bool isLimitPlot;
  int figInPaper = -1;
  string subFigInPaper = "";
  string locStr, desStr;
  string table_doi = "table_doi: http://cds.cern.ch/record/2638619?ln=en";

  while(1){
    table_files >> filename;
        
    if(table_files.eof()) break;

    //--- check if it is limit plot or exp vs obs comparison plot
    if( (filename.find("_XsecLimit.yaml")!=string::npos) || 
	(filename.find("_obs.yaml")!=string::npos) || (filename.find("_exp.yaml")!=string::npos) ||
	(filename.find("exp1dn.yaml")!=string::npos) || (filename.find("exp1up.yaml")!=string::npos) || 
	(filename.find("obs_XsecUp.yaml")!=string::npos) || (filename.find("obs_XsecDn.yaml")!=string::npos)) isLimitPlot = true;
    else isLimitPlot = false;
    //--- descrition for limit plots
    if( (filename.find("obs_XsecLimit.yaml")!=string::npos)) desStr = "Observed $95\\%$ CL upper limit on the production cross section of ";
    else if( (filename.find("exp_XsecLimit.yaml")!=string::npos)) desStr = "Expected $95\\%$ CL upper limit on the production cross section of ";
    else if( (filename.find("_obs.yaml")!=string::npos)) desStr = "observed exclusion contour";
    else if( (filename.find("_exp.yaml")!=string::npos)) desStr = "expected exclusion contour";
	
    //--- assign location, description in paper
    if(!isLimitPlot){
      if( (!(filename.find("Matrix")!=string::npos)) && (!(filename.find("Cutflow")!=string::npos)) ){
	figInPaper = 4; subFigInPaper = "";
	locStr = "Values from figure " + std::to_string(figInPaper) + " of the paper.";
	desStr = "Values of the predicted SM background events from various sources and observed events in each of the 25 signal regions.";
      }
      else{
	figInPaper = 0; subFigInPaper = "";
	locStr = "Not in paper, additional material.";
	if((filename.find("Corr")!=string::npos)) desStr = " Correlation among different search bins.";
	else if((filename.find("Cov")!=string::npos)) desStr = " Covariance among different search bins.";
	else if((filename.find("Cutflow")!=string::npos)) desStr = " Cutflow table for a few signal models. Yields are normalized to integrated luminosity.";
      }
    }
    else {
      figInPaper = 5;
      if(filename.find("T5qqqqH")!=string::npos){
	subFigInPaper = "(top left)";
	if((filename.find("_XsecLimit.yaml")!=string::npos)) desStr = desStr + "gluinos in T5qqqqHG model.";
	else desStr = "Gluino - neutralino mass points lying on the "+desStr+" of T5qqqqHG model.";
	locStr = "Data in figure "+to_string(figInPaper)+" (top left) of the paper.";
      }
      else if(filename.find("T5bbbbZ")!=string::npos){
	subFigInPaper = "(top right)";
	if((filename.find("_XsecLimit.yaml")!=string::npos)) desStr = desStr + "gluinos in T5bbbbZG model.";
	else desStr = "Gluino - neutralino mass points lying on the "+desStr+" of T5qqqqHG model.";
	locStr = "Data in figure "+to_string(figInPaper)+" (top right) of the paper.";
      }
      else if(filename.find("T5ttttZ")!=string::npos){
	subFigInPaper = "(bottom left)";
	if((filename.find("_XsecLimit.yaml")!=string::npos)) desStr = desStr + "gluinos in T5ttttZG model.";
	else desStr = "Gluino - neutralino mass points lying on the "+desStr+" of T5qqqqHG model.";
	locStr = "Data in figure "+to_string(figInPaper)+" (bottom left) of the paper.";
      }
      else if(filename.find("T6ttZ")!=string::npos){
	subFigInPaper = "(bottom right)";
	if((filename.find("_XsecLimit.yaml")!=string::npos)) desStr = desStr + "gluinos in T6ttZG model.";
	else desStr = "Stop - neutralino mass points lying on the "+desStr+" of T5qqqqHG model.";
	locStr = "Data in figure "+to_string(figInPaper)+" (bottom right) of the paper.";
      }
    }
    // cout<<"--------------------------"<<filename<<endl;
    // cout<<" Loc "<<figInPaper<<" "<<subFigInPaper<<endl;
    // cout<<" Loc "<<locStr<<endl;
    // cout<<" Des "<<desStr<<endl;

    submission << "---\n\n";
    if(figInPaper > 0) submission << "name: '"<<i<<". Figure " << figInPaper << " " << subFigInPaper << "'" << "\n";
    else submission << "name: '"<<i<<". Additional material.'\n";
    submission << "location: "<< locStr << "\n";
    submission << "description: " << desStr << "\n";
    submission << "keywords: " << "\n";
    submission << " - { name: reactions, values: [pp]}" << "\n";
    if(isLimitPlot) submission << " - { name: observables, values: [Signal]}" << "\n";
    else submission << " - { name: observables, values: []}" << "\n";
    submission << " - { name: cmenergies, values: [13000]}" << "\n";
    submission << " - { name: phrases, values: [SUSY, Supersymmetry, GMSB, Proton-Proton Scattering, Photon, b-jet, Gravitino]}" << "\n";
    //  submission << table_doi << "\n";
    submission << "data_file: " << filename << "\n\n\n";
        
    figInPaper = -1;
    subFigInPaper = "";
    desStr = "";
    locStr = "";

    i=i+1;
  }

  return 0;
}


//comment: | #Information that applies to all data tables.
