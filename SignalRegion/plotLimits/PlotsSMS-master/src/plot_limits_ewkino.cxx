//// plot_limits_summary: Plots various limit curves on same canvas

// System includes
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>  // setw

// ROOT includes
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TList.h"
#include "TLine.h"
#include "TROOT.h"
#include "TPad.h"
#include "TError.h" // Controls error level reporting

// User includes
#include "utilities.hpp"
#include "model_limits.hpp"

using namespace std;
namespace{
  bool do_prelim = false;
  bool no_shade = true;
  bool debug = false;

  int LineWidth = 4;
  float cmsH = 0.01;
  float legLineH = 0.039;
  float legTextSize = 0.035;
  float TopMargin = 0.08;
  float legY = 1-TopMargin-cmsH-0.025;
  float fillTransparency = 0.06;

  TString lsp = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString chipm = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{#pm}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString chi2 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1]{#scale[0.85]{_{2}}}";
  TString ifb("fb#lower[.2]{^{-1}}");
  TString xl("x#lower[-.2]{_{l}}");

  TString flavDem = chipm+chi2+"#rightarrowl#tilde{#nu}l#tilde{l}, BF(ll)=0.5";
  TString tauRich = chipm+chi2+"#rightarrow#tilde{#tau}#nul#tilde{l}";
  TString tauDom  = chipm+chi2+"#rightarrow#tilde{#tau}#nu#tau#tilde{#tau}";

  TString WZdecay = "BF("+chi2+"#rightarrowZ#kern[0.3]{"+lsp+"})";
  TString WHdecay = "BF("+chi2+"#rightarrowH#kern[0.3]{"+lsp+"})";

  int c5050 = kBlue, c5005 = kAzure+7, c5095 = kSpring-6;
  int c10050 = kOrange-2, c10005 = kGray+1, c10095 = kOrange+2;
  int ctau50 = kPink+1;
  int cwh = kMagenta+1, cwh43 = kGreen+1, cwz39 = kRed, cwz48 = 1, cwz34 = kCyan+2;
}

TString getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp);
void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title, TString lumi);

int main(){
  gErrorIgnoreLevel=kWarning; // Turns off ROOT INFO messages

  // Label definitions
  TString pp_gluglu("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}");
  TString basetitle(pp_gluglu+",  #tilde{g} #rightarrow ");

  // Folder with root files containing the TGraphs
  TString folder("root/limits_moriond2017/");
  vector<model_limits> models;

  ///////////////////////////////    Defining EWKino only plot   //////////////////////////////
  models.push_back(model_limits("EWKino","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}"));
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 250);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 250);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 250);
  models.back().setRanges(100, 650, 0, 440);

  models.back().add("\\text{SUS-16-039, }\\geq3\\ell\\text{ (WH)}", folder+"ewkino_sus16_039_wh_gr.root", 
  		    cwh, "gr_obs", "gr_exp", 124.5);
  models.back().add("\\text{SUS-16-043, }1\\ell\\text{ 2b (WH)}", folder+"ewkino_sus16_043_wh_aux.root", 
  		    cwh43, "gObs", "gExp", 124.5);
  models.back().add("\\text{SUS-16-045, }H\\rightarrow\\gamma\\gamma\\text{ (WH)}", folder+"ewkino_sus16_045_wh.root", 
   		    kOrange, "obs", "exp", 124.5);
  models.back().add("\\text{SUS-16-034, }2\\ell\\text{ on-Z (WZ)}", folder+"ewkino_sus16_034_wz.root", 
  		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 9999);
  models.back().add("\\text{SUS-16-048, }2\\ell\\text{ soft (WZ)}", folder+"ewkino_sus16_048_wz.root", 
   		    cwz48, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);
  models.back().add("\\text{This result, }3\\ell\\text{ (WZ)}", folder+"ewkino_sus16_039_wz_gr.root", 
  		    cwz39, "gr_obs", "gr_exp", 7);

  ///////////////////////////////    Defining EWKino only plot   //////////////////////////////
  models.push_back(model_limits("EWKino-cadi","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}"));
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 250);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 250);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 250);
  models.back().setRanges(100, 650, 0, 440);

  models.back().add("\\text{SUS-16-039, }\\geq3\\ell\\text{ (WH)}", folder+"ewkino_sus16_039_wh_gr.root", 
  		    cwh, "gr_obs", "gr_exp", 124.5);
  models.back().add("\\text{SUS-16-043, }1\\ell\\text{ 2b (WH)}", folder+"ewkino_sus16_043_wh_aux.root", 
  		    cwh43, "gObs", "gExp", 124.5);
  models.back().add("\\text{SUS-16-045, }H\\rightarrow\\gamma\\gamma\\text{ (WH)}", folder+"ewkino_sus16_045_wh.root", 
   		    kOrange, "obs", "exp", 124.5);
  models.back().add("\\text{SUS-16-034, }2\\ell\\text{ on-Z (WZ)}", folder+"ewkino_sus16_034_wz.root", 
  		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 9999);
  models.back().add("\\text{SUS-16-048, }2\\ell\\text{ soft (WZ)}", folder+"ewkino_sus16_048_wz.root", 
   		    cwz48, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);
  models.back().add("\\text{SUS-17-004, }3\\ell\\text{ (WZ)}", folder+"ewkino_sus16_039_wz_gr.root", 
  		    cwz39, "gr_obs", "gr_exp", 7);

  ///////////////////////////////    Defining EWKino only plot   //////////////////////////////
  models.push_back(model_limits("EWKino-nocadi","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}"));
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 250);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 250);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 250);
  models.back().setRanges(100, 650, 0, 370);

  models.back().add("\\geq3\\ell\\text{ (WH)}", folder+"ewkino_sus16_039_wh_gr.root", 
  		    cwh, "gr_obs", "gr_exp", 124.5);
  models.back().add("2\\ell\\text{ on-Z (WZ)}", folder+"ewkino_sus16_034_wz.root", 
  		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 9999);
  models.back().add("1\\ell\\text{ 2b (WH)}", folder+"ewkino_sus16_043_wh_aux.root", 
  		    cwh43, "gObs", "gExp", 124.5);
  models.back().add("2\\ell\\text{ soft (WZ)}", folder+"ewkino_sus16_048_wz.root", 
   		    cwz48, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);
  models.back().add("H\\rightarrow\\gamma\\gamma\\text{ (WH)}", folder+"ewkino_sus16_045_wh.root", 
   		    kOrange, "obs", "exp", 124.5);
  models.back().add("3\\ell\\text{ (WZ)}", folder+"ewkino_sus16_039_wz_gr.root", 
  		    cwz39, "gr_obs", "gr_exp", 7);

  ///////////////////////////////    Defining EWK combination plot   //////////////////////////////
  models.push_back(model_limits("EWKComb","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}", 0.9)); 
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 290);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 290);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 290);
  models.back().setRanges(125, 700, 0, 400);

  models.back().add(WHdecay+" = 1 (WH)", folder+"ewkino_sus17_004_wh.root", 
  		    cwh43, "gr_obs", "gr_exp", 124.5);
  models.back().add(WZdecay+" = 1 (WZ)", folder+"ewkino_sus17_004_wz.root", 
  		    cwz39, "gr_obs", "gr_exp", 7);
  models.back().add(WZdecay+" = "+WHdecay+" = 0.5 (WH+WZ)", folder+"ewkino_sus17_004_wzmix.root", 
  		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 124.5);

  
  
  //////////////////////////////////////////////////////////////////////////////////////// 
  //////////////////////////////////    Making plots    //////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////// 
  
  //// Creating canvas
  gStyle->SetOptStat(0);  
  float lMargin(0.14), tMargin(TopMargin), rMargin(0.04), bMargin(0.14);
  TCanvas can("canvas","", 600, 600);
  setCanvas(can, lMargin, tMargin, rMargin, bMargin);

  //// Creating base legend for observed/expected
  TH1D hobs("hobs","",1,0,1), hexp("hexp","",1,0,1);
  hobs.SetLineColor(1); hobs.SetLineWidth(LineWidth);
  hexp.SetLineColor(1); hexp.SetLineStyle(2); hexp.SetLineWidth(LineWidth);

  double legX(1-rMargin-0.045), baselegY = 1-tMargin-cmsH-0.02;
  double legW = 0.14, legH = 0.07;
  TLegend baseleg(legX-legW, baselegY-legH, legX, baselegY);
  baseleg.SetTextSize(0.031); baseleg.SetFillColor(0); 
  baseleg.SetFillStyle(0); baseleg.SetBorderSize(0);
  baseleg.AddEntry(&hexp, "Expected");
  baselegY = baselegY - 0.038;
  TLegend obsleg(legX-legW, baselegY-legH, legX, baselegY);
  obsleg.SetTextSize(0.031); obsleg.SetFillColor(0); 
  obsleg.SetFillStyle(0); obsleg.SetBorderSize(0);
  obsleg.AddEntry(&hobs, "Observed");

  //// General line and label
  TLatex labMass; labMass.SetNDC();  
  labMass.SetTextAlign(33); labMass.SetTextFont(42); 
  TLine line;
  TLatex label;

  //// Looping over each model
  cout<<endl;
  for(size_t imodel(0); imodel < models.size(); imodel++){
    model_limits mod(models[imodel]);

    //// Creating base histogram and drawing lumi labels
    float Xmin(700), Xmax(1750), Ymin(0), Ymax(1800), glu_lsp;
    TString xtitle = getModelParams(mod.model, Xmin, Xmax, Ymin, Ymax, glu_lsp);
    if(mod.Xmin != -99) Xmin = mod.Xmin; if(mod.Xmax != -99) Xmax = mod.Xmax;
    if(mod.Ymin != -99) Ymin = mod.Ymin; if(mod.Ymax != -99) Ymax = mod.Ymax;

    TH2D hbase = baseHistogram(Xmin, Xmax, Ymin, Ymax, xtitle);
    hbase.Draw();
    addLabelsTitle(lMargin, tMargin, rMargin, mod.title, mod.lumi);

    //// Plot lines at constant dM
    float degrees = 180/3.141593*atan2((1-tMargin-bMargin)/(Ymax-Ymin) , (1-lMargin-rMargin)/(Xmax-Xmin));
    int lcolor(kGray+2);
    line.SetLineStyle(2); line.SetLineWidth(2); line.SetLineColor(lcolor);
    label.SetTextSize(0.027); label.SetTextAngle(degrees); label.SetTextColor(lcolor);
    for(size_t iline=0; iline<mod.lines.size(); iline++){
      float dM = mod.lines[iline].dm;
      float minGlu=dM;
      float maxh = mod.lines[iline].maxHeight;
      if(dM<Xmin) minGlu = Xmin;
      line.DrawLine(minGlu, minGlu-dM, dM+maxh, maxh);
      // offsetX controls where along the line to put the label, offsetY how far from it
      float offsetX=(Xmax-Xmin)/38, offsetY=(Xmax-Xmin)/38;
      if(mod.lines[iline].above){
	label.SetTextAlign(31); 
      } else {
	label.SetTextAlign(33);
	offsetX *= -0.4;
	offsetY *= -0.7;
      }
      label.DrawLatex(dM+maxh-offsetX, maxh-offsetX+offsetY, mod.lines[iline].label);
    }// Loop over lines

    //// Plotting limits
    size_t ncurves(mod.files.size());
    vector<TGraph*> obs(ncurves, 0), exp(ncurves, 0);
    // Getting all graphs first because the ones that come from TCanvas mess up the colors
    for(size_t file(0); file < ncurves; file++){
      cout<<"Loading "<<mod.files[file]<<endl;
      TFile flimit(mod.files[file]);
      exp[file] = getGraph(flimit, mod.expnames[file]);
      obs[file] = getGraph(flimit, mod.obsnames[file]);
    }
    cout<<endl<<"Model "<<mod.model<<endl<<"============================="<<endl;
    for(size_t file(0); file < ncurves; file++){
      if (debug) cout<<"Doing "<<mod.files[file]<<endl;
      float mod_gl = glu_lsp;
      if(mod.glu_lsps[file]>0) mod_gl = mod.glu_lsps[file];

      if(mod.labels[file].Contains("16-048") || mod.labels[file].Contains("soft")){
	changeDmCoordinates(exp[file]);
	changeDmCoordinates(obs[file]);
      }
      setGraphStyle(exp[file], mod.colors[file], 2, LineWidth, mod_gl, debug);
      setGraphStyle(obs[file], mod.colors[file], 1, LineWidth, mod_gl, debug);
      obs[file]->Draw("f same");

      TString obsname("obs"); obsname += imodel; obsname += file;
      obs[file]->SetName(obsname);
    } // Loop over curves in each model
    //// Plotting the lines on top of the fills
    if(mod.reverseOrder)
      for(size_t file(ncurves-1); file < ncurves; file--){
	if(exp[file])  exp[file]->Draw("same");
	obs[file]->Draw("same");
      }// Loop over curves in each model
    else
      for(size_t file(0); file < ncurves; file++){
	if(exp[file])  exp[file]->Draw("same");
	obs[file]->Draw("same");
      }// Loop over curves in each model
    hbase.Draw("axis same");

    // Drawing legends
    baseleg.Draw();
    obsleg.Draw();
    legX = lMargin+0.03; //legY = 1-tMargin-cmsH-0.035;
    legW = 0.13; 
    legH = legLineH * ncurves * mod.legScale;
    if(!mod.labels[0].Contains("SUS") && !mod.labels[0].Contains("BF")){
      legH /= 2.;
      legW = 0.6;
    }
    TLegend limleg(legX, legY-legH, legX+legW, legY);
    limleg.SetTextSize(legTextSize * mod.legScale); limleg.SetFillColor(0); 
    limleg.SetFillStyle(0); limleg.SetBorderSize(0);
    if(!mod.labels[0].Contains("SUS") && !mod.labels[0].Contains("BF")) limleg.SetNColumns(2);
    for(size_t file(0); file < ncurves; file++)
      limleg.AddEntry(obs[file]->GetName(), mod.labels[file], "l");
    limleg.Draw();
    labMass.SetTextSize(legTextSize * mod.legScale * 1.2);
    labMass.DrawLatex(0.93, legY-legH-0.5*legLineH, mod.labMass);

    TString plotname(mod.model+"_limits_summary_cms"+(!no_shade?"_shade":"")+".eps");
    can.SaveAs(plotname);
    TString command = "pstopdf "+plotname+" "+plotname.ReplaceAll(".eps",".pdf");
    execute(command.Data());
    execute("rm *.eps");
    
    cout<<" open "<<plotname<<endl<<endl;
  } // Loop over models
  cout<<endl<<endl;
}


TString getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp){
  TString xtitle = "m#kern[0.12]{_{#lower[-0.12]{#tilde{g}}}}";
  glu_lsp = 25; xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  if(model.Contains("Sleptons")){
    Xmin = 100; Xmax = 1250;
    Ymin = 0;   Ymax = 1350;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model.Contains("EWK-slep")){
    Xmin = 100; Xmax = 1250;
    Ymin = 0;   Ymax = 1570;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model.Contains("EWK-slep-0p5")){
    Xmin = 100; Xmax = 1250;
    Ymin = 0;   Ymax = 1260;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model == "EWKino"){
    Xmin = 100; Xmax = 650;
    Ymin = 0;   Ymax = 440;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model == "WZ"){
    Xmin = 100; Xmax = 650;
    Ymin = 0;   Ymax = 390;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model == "WH"){
    Xmin = 120; Xmax = 550;
    Ymin = 0;   Ymax = 190;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model.Contains("T1tttt")){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 2050;
    glu_lsp = 225;
  }
  if(model.Contains("T1tttt_leptonic") || model.Contains("T1tttt_hadronic")){
    Xmin = 700; Xmax = 2000;
    Ymin = 0;   Ymax = 1675;
    glu_lsp = 225;
  }
  if(model == "T1bbbb"){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 1950;
    glu_lsp = 25;
  }    
  if(model == "T1qqqq"){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 1750;
    glu_lsp = 25;
  }    
  if(model.Contains("T5qqqqVV")){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 1850;
    glu_lsp = 125;
  }    
  return xtitle;
}

void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title, TString lumi){
  TLatex label; label.SetNDC();  
  // Printing CMS Preliminary
  double offsetx(0.025), ycms(1-tMargin-0.075);
  label.SetTextAlign(11); label.SetTextFont(52); label.SetTextSize(0.038);
  //if(do_prelim) label.DrawLatex(0.27+offsetx, ycms, "Preliminary");
  //label.DrawLatex(0.27+offsetx, ycms, "Preliminary");

  // Printing top title
  label.SetTextAlign(21); label.SetTextFont(42); label.SetTextSize(0.6*tMargin);
  label.DrawLatex((1-rMargin-lMargin)/2.+lMargin-0.05, 1-tMargin+0.02, title);
  // Printing date
  label.SetTextAlign(31); label.SetTextFont(52); label.SetTextSize(0.45*tMargin);
  if(do_prelim) label.DrawLatex(1-rMargin-0.02, 1-tMargin+0.018, "Moriond 2017");
  // Printing energy
  label.SetTextAlign(31); label.SetTextFont(42); label.SetTextSize(0.53*tMargin);
  if(lumi=="") lumi = "13 TeV";
  else lumi += " "+ifb+" (13 TeV)";
  label.DrawLatex(1-rMargin, 1-tMargin+0.015, lumi);
  label.SetTextAlign(11); label.SetTextFont(61); label.SetTextSize(0.75*tMargin);
  label.DrawLatex(lMargin, 1-tMargin+0.012, "CMS");
}

