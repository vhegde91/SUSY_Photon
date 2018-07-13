#include<iostream>
#include<iomanip>
#include"TH1.h"
#include"TROOT.h"
#include"TH2.h"
#include"TFile.h"
#include"TDirectory.h"
#include"TTree.h"
#include"TBrowser.h"
#include"TF1.h"
#include<string>
#include<vector>
#include"TGraphErrors.h"
#include"TGraph.h"
#include"TLegend.h"
#include"TLatex.h"
#include"TCanvas.h"
#include"THStack.h"
#include"TStyle.h"

const int nfiles=2,nBG=1;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getXaxisName(TString);
TLatex textOnTop,intLumiE;
double intLumi=35.9;

double sr_Integral=0,cr_Integral=0;

void testClosure(){
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
  
  f[0] = new TFile("CS_TTWZ_LostMuHadTau_clsr_METnJbJ_v2.root");
  f[1] = new TFile("CS_TTWZ_LostEle_clsr_METNJbJ_v2.root");

  // TFile *fTF = new TFile("SBinHists.root");
  //  TH1D *h_tfe = (TH1D*)fTF->Get("LEleTFs_Final");
  // TH1D *h_tfm = (TH1D*)fTF->Get("LMuTFs_Final");
  
  double pcUncLEleTF = sqrt((0.12*0.12) + (0.05*0.05) + (0.02*0.02) + (0.02*0.02));//12% for dR(l,phootn), 5% for PDF, 2% each for JEC & ele SF
  double pcUncLMuTF  = sqrt((0.015*0.015) + (0.03*0.03));//1.5% for PDF, 3% each for mu SF

  //  TFile *fout = new TFile("forPull.root","recreate");
  //  gStyle->SetOptStat("nemri");
  
  //  cout<<pcUncLEleTF<<" muUnc: "<<pcUncLMuTF<<endl;

  TH1D *h0=(TH1D*)f[0]->Get("AllSBins_v7_Mu0");
  TH1D *h1=(TH1D*)f[0]->Get("AllSBins_v7_Mu1");
  TH1D *htemp=(TH1D*)f[1]->Get("AllSBins_v7_Ele0");
  h0->Add(htemp);//add LMu and LEle
  htemp=(TH1D*)f[1]->Get("AllSBins_v7_Ele1");
  h1->Add(htemp);
  
  TH1D *h_LEleTFUnc=(TH1D*)h0->Clone("LEleTFUnc");
  TH1D *h_LMuTFUnc=(TH1D*)h0->Clone("LMuTFUnc");
  for(int i=1;i<=h0->GetNbinsX();i++){
    //    h0->SetBinError(i, sqrt(pow((h0->GetBinContent(i)*pcUncLMuTF),2) + pow(h0->GetBinError(i),2)) );
    h_LEleTFUnc->SetBinError(i,pcUncLEleTF);
    h_LMuTFUnc->SetBinError(i,pcUncLMuTF);
    h_LEleTFUnc->SetBinContent(i,0);
    h_LMuTFUnc->SetBinContent(i,1);
  }
  h_LMuTFUnc->Add(h_LEleTFUnc);
  
  //add LMu TF unc to prediction unc
  // for(int i=1;i<=h0->GetNbinsX();i++){
  //   h0->SetBinError(i, sqrt(pow((h0->GetBinContent(i)*pcUncLMuTF),2) + pow(h0->GetBinError(i),2)) );
  // }
  //add LEle TF unc to prediction unc
  //  TH1D *htemp=(TH1D*)f[1]->Get("AllSBins_v7_Ele0");
  // for(int i=1;i<=htemp->GetNbinsX();i++){
  //   htemp->SetBinError(i, sqrt(pow((htemp->GetBinContent(i)*pcUncLEleTF),2) + pow(htemp->GetBinError(i),2)) );
  // }

  // h1->Draw();
  // h0->Draw("same");
  // return;
  vector<string> name1,name2;
  vector<int> rebin;
 
  name1.push_back("AllSBins_v7_Mu0");    name2.push_back("AllSBins_v7_Mu1");  rebin.push_back(1);
 

  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  TLine *line1=new TLine( 8.5,0.05,  8.5,100);
  TLine *line2=new TLine(15.5,0.05, 15.5,100);
  TLine *line3=new TLine(22.5,0.05, 22.5,100);
  TLine *line4=new TLine(29.5,0.05, 29.5,100);
  TLine *line5=new TLine(36.5,0.05, 36.5,10 );
  TLine *line1V7=new TLine( 6.5,0.05,  6.5,400);
  TLine *line2V7=new TLine(11.5,0.05, 11.5,400);
  TLine *line3V7=new TLine(16.5,0.05, 16.5,400);
  TLine *line4V7=new TLine(21.5,0.05, 21.5,200);
  TLine *line5V7=new TLine(26.5,0.05, 26.5,200 );
  
  //  TCanvas *c_cB=new TCanvas("closure_test","closure test",1500,800); c_cB->Divide(4,2);
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];

  for(int i=0;i<name1.size();i++){
    name=name1[i]+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    p_top[i]=new TPad(name+"_top",name+"_top",0,0.4,1,1);
    p_bot[i]=new TPad(name+"_bot",name+"_bot",0,0.0,1,0.4);
    p_top[i]->Draw();p_top[i]->SetGridx(0);p_top[i]->SetGridy(0);p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.3);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy();    

    
    TH1D *h_histG,*h_histE,*h_histGcopy;
    
    for(int j=0;j<nfiles;j++){
      c_cA[i]->cd();
      //c_cB->cd(i+1);
      name=name1[i];
      // h_histG=(TH1D*)f[j]->FindObjectAny(name);
      // h_histG->Rebin(rebin[i]);
          
      // name=name2[i];
      // h_histE=(TH1D*)f[j]->FindObjectAny(name);//h_histE->Scale(150.9/481.6);
      // h_histE->Rebin(rebin[i]);//h_histE->Scale(1.0/h_histE->Integral());
      h_histG=(TH1D*)h0->Clone();
      h_histE=(TH1D*)h1->Clone();
     
      if(h_histG && h_histE){
	if(name1[i]=="nHadJets_Mu0"){
	  sr_Integral=h_histG->Integral();
	  cr_Integral=h_histE->Integral();
	}
	setLastBinAsOverFlow(h_histG);
	setLastBinAsOverFlow(h_histE);

	h_histG->SetLineColor(kRed);
	h_histG->SetLineWidth(2);
	h_histG->SetMarkerStyle(21);
	h_histG->SetMarkerColor(h_histG->GetLineColor());
	h_histG->SetTitle(";;Events");
	h_histG->GetYaxis()->SetLabelSize(0.09);
	h_histG->GetYaxis()->SetTitleSize(0.09);
	h_histG->GetYaxis()->SetTitleOffset(0.47);
	h_histG->GetYaxis()->SetNdivisions(10);
	
	//	h_histE->SetLineColor(kGreen+2);
	h_histE->SetLineColor(kBlue);
	h_histE->SetLineWidth(2);
	h_histE->SetMarkerStyle(20);
	h_histE->SetMarkerColor(h_histE->GetLineColor());

	c_cA[i]->cd();p_top[i]->cd();
	//c_cB->cd(i+1);p_top[i]->cd();
	h_histG->GetYaxis()->SetTickLength(0.0328/1.5);
	h_histG->SetMaximum(999);
	h_histG->Draw("e1");
	//	cout<<"y "<<h_histG->GetYaxis()->GetTickLength()<<endl;
	//	h_histG->GetYaxis()->SetTickLength(0.05);
	//	h_histG->GetYaxis()->SetTickLength(0.25);
	// h_histE->SetFillStyle(3004);
	// h_histE->SetFillColor(h_histE->GetLineColor());
 	h_histE->Draw("e1 same");
	//	h_histE->Draw("L same");

	//	legend[i]=new TLegend(0.69, 0.66,  0.85, 0.86);
	//	legend[i]=new TLegend(0.62, 0.76,  0.86, 0.86); legend[i]->SetNColumns(2);
	legend[i]=new TLegend(0.6241656,0.7686987,0.864486,0.8702983); legend[i]->SetNColumns(2);
	name=name1[i];
	legend[i]->AddEntry(h_histG,"Expected","lep");
	name=name2[i];
	legend[i]->AddEntry(h_histE,"Prediction","lep");
	legend[i]->Draw();
	
	TString name = h_histG->GetName();
	if(name.Contains("SBins_v4")){ 
	  line1->Draw();	line2->Draw();	line3->Draw();	line4->Draw();	line5->Draw();
	  p_top[i]->SetGridx(0); p_bot[i]->SetGridx(0);
	}
	else if(name.Contains("SBins_v7")){ 
	  line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
	  p_top[i]->SetGridx(0); 	  p_bot[i]->SetGridx(0);
	}
	
      }
    }
    
    TH1D *h_numr=(TH1D*)h_histG->Clone("ratioPlot");
    
    h_numr->Divide(h_histE);
    TString xaxisName = getXaxisName(h_histG->GetName());

    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    //    h_numr->SetTitle(";;#frac{Exp}{Pred }");
    h_numr->SetTitle(";;Exp. / Pred.");
    h_numr->GetXaxis()->SetLabelSize(0.13);
    h_numr->GetXaxis()->SetTitle(xaxisName);
    h_numr->GetXaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetTitleOffset(0.9);

    //    h_numr->GetYaxis()->SetTitleOffset(0.36);
    h_numr->GetYaxis()->CenterTitle();
    h_numr->GetYaxis()->SetTitleOffset(0.30);
    h_numr->GetYaxis()->SetTitleSize(0.14);
    h_numr->GetYaxis()->SetLabelSize(0.14);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->GetYaxis()->SetTickLength(0.042/1.5);
    h_numr->SetMaximum(1.90);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e1");
    h_LMuTFUnc->SetFillStyle(3013);
    h_LMuTFUnc->SetFillColor(1); 
    h_LMuTFUnc->Draw("E2same");
    h_numr->Draw("e0 same");
    //    h_LMuTFUnc->Print("all");
    //    cout<<"Bot:"<<h_numr->GetYaxis()->GetTickLength()<<endl;
    TString name = h_histG->GetName();
    if(name.Contains("SBins_v4")){ 
      line1->Draw();	line2->Draw();	line3->Draw();	line4->Draw();	line5->Draw();
      TH1D *h_pullHist = new TH1D("pull_lostMuHadTau","1D pull for lost Mu+hadTau",50,-2.5,7.5);
      //TH1D *h_pullHist = new TH1D("pull_lostMuHadTau","1D pull for lost Mu+hadTau",43,0.5,43.5);
      for(int p=1;p<=h_numr->GetNbinsX();p++){
	h_pullHist->Fill( (1.0-h_numr->GetBinContent(p))/h_numr->GetBinError(p));
	//h_pullHist->Fill( p,(1.0-h_numr->GetBinContent(p))/h_numr->GetBinError(p));
      }
      h_pullHist->Write();
      //      p_bot[i]->SetGridx(0);
    }
    else if(name.Contains("SBins_v7")){
      line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
      line1V7->SetLineStyle(2);	line2V7->SetLineStyle(2);	line3V7->SetLineStyle(2);	line4V7->SetLineStyle(2);	line5V7->SetLineStyle(2);
    }
    c_cA[i]->cd();    p_top[i]->cd();
    char name2[100];
    textOnTop.SetTextSize(0.07);
    intLumiE.SetTextSize(0.07);
    //    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation}}");
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation Preliminary}}");
    sprintf(name2,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.72,0.91,name2);
 
    c_cA[i]->cd();    p_top[i]->cd();
    TArrow *arrow1 = new TArrow(0.5,110,6.5,110,0.01,"<|>");
    TArrow *arrow2 = new TArrow(6.5,110,11.5,110,0.01,"<|>");
    TArrow *arrow3 = new TArrow(11.5,110,16.5,110,0.01,"<|>");
    TArrow *arrow4 = new TArrow(16.5,110,21.5,110,0.01,"<|>");
    TArrow *arrow5 = new TArrow(21.5,110,26.5,110,0.01,"<|>");
    TArrow *arrow6 = new TArrow(26.5,110,31.5,110,0.01,"<|>");
    arrow1->Draw(); arrow2->Draw(); arrow3->Draw();
    arrow4->Draw(); arrow5->Draw(); arrow6->Draw();
    TLatex Tl;
    Tl.SetTextSize(0.06);
    Tl.DrawLatex(3.,135,"N^{ 0}_{ 2-4}");
    Tl.DrawLatex(8.5,135,"N^{ 0}_{ 5-6}");
    Tl.DrawLatex(13.5,135,"N^{ 0}_{ #geq7}");
    Tl.DrawLatex(18.5,135,"N^{ #geq1}_{ 2-4}");
    Tl.DrawLatex(23.5,135,"N^{ #geq1}_{ 5-6}");
    Tl.DrawLatex(28.5,135,"N^{ #geq1}_{ #geq7}");

    // TLatex Tl2;
    // Tl2.SetTextSize(0.05);
    // Tl2.SetTextAngle(90);
    // Tl2.DrawLatex(1.1,1,"#bf{Sideband}");
    // Tl2.DrawLatex(7.1,1,"#bf{Sideband}");
    // Tl2.DrawLatex(12.1,1,"#bf{Sideband}");
    // Tl2.DrawLatex(17.1,1,"#bf{Sideband}");
    // Tl2.DrawLatex(22.1,1,"#bf{Sideband}");
    // Tl2.DrawLatex(27.1,1,"#bf{Sideband}");

    if(saveCanvas){name="c_"+name1[i]+name2[i]+".png";c_cA[i]->SaveAs(name);}
  }
  gStyle->SetTextSize(2);
  //fout->cd();
  //  h2_histGcopy->Write();
  cout<<"Integral in SR(0 Mu events) "<<sr_Integral<<endl
      <<"Integral in CR(1 Mu events) "<<cr_Integral<<endl;


}
TString getXaxisName(TString axname){
  if(axname.Contains("nHadJets")) return "Jets";
  else if(axname.Contains("ST")) return "ST(GeV)";
  else if(axname.Contains("BTags")) return "b-Tags";
  else if(axname.Contains("ElePt")) return "e pT(GeV)";
  else if(axname.Contains("PhotonPt")) return "#gamma pT(GeV)";
  else if(axname.Contains("mT_")) return "mT(GeV)";
  else if(axname.Contains("AllSBin")) return "Bin Number";
  else if(axname.Contains("dPhi_METjet1") || axname.Contains("dphi1_METjet1")) return "#Delta#Phi_{1}";
  else if(axname.Contains("dPhi_METjet2") || axname.Contains("dphi2_METjet2")) return "#Delta#Phi_{2}";
  else if(axname.Contains("dPhi_METBestPhoton") ) return "#Delta#Phi(MET,#gamma)";
  else if(axname.Contains("QMut") || axname.Contains("Qmut")) return "QMult";
  else if(axname.Contains("MET")) return "MET(GeV)";
  else return axname;

}


void setLastBinAsOverFlow(TH1D* h_hist){
  double lastBinCt =h_hist->GetBinContent(h_hist->GetNbinsX()),overflCt =h_hist->GetBinContent(h_hist->GetNbinsX()+1);
  double lastBinErr=h_hist->GetBinError(h_hist->GetNbinsX()),  overflErr=h_hist->GetBinError(h_hist->GetNbinsX()+1);
  
  if(lastBinCt!=0 && overflCt!=0)
    lastBinErr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );
  
  else if(lastBinCt==0 && overflCt!=0)
    lastBinErr = overflErr;
  else if(lastBinCt!=0 && overflCt==0)
    lastBinErr = lastBinErr;
  else lastBinErr=0;

  lastBinCt = lastBinCt+overflCt;
  h_hist->SetBinContent(h_hist->GetNbinsX(),lastBinCt);
  h_hist->SetBinError(h_hist->GetNbinsX(),lastBinErr);
    
}
