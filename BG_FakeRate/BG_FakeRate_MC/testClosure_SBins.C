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

const int nfiles=1,nBG=1;    //Specify no. of files
TFile *f[nfiles];
//int col[10]={kOrange,kBlue,kTeal+9,kGray+1,kCyan,kOrange-9,kYellow+2,kRed,kMagenta+2,kMagenta};  //Specify Colors
int col[10]={kOrange,kTeal+9,kBlue,kGray+1,kCyan,kMagenta+2,kYellow+2,kRed,kMagenta,kOrange-9};  //Specify Colors
TString name;
bool saveCanvas=0;
void setLastBinAsOverFlow(TH1D*);
TString getXaxisName(TString);
TLatex textOnTop,intLumiE;
double intLumi=35.9;

double sr_Integral=0,cr_Integral=0;

void testClosure_SBins(){
  TString iFname = "CS_TTW_FR_ISRWt_clsr_v2.root";
  TH1::SetDefaultSumw2(1);
  gStyle->SetOptStat(0);
 
  f[0] = new TFile(iFname);
  //  gStyle->SetOptStat("nemri");

  TH1D *h_FRUnc=(TH1D*)((TH1D*)f[0]->Get("AllSBins_v7_Pho"))->Clone("FRsystunc");
  for(int i=1;i<=h_FRUnc->GetNbinsX();i++){
    h_FRUnc->SetBinError(i,0.154);
    h_FRUnc->SetBinContent(i,1);
  }


  vector<string> name1,name2;
  vector<int> rebin;
  name1.push_back("AllSBins_v7_Pho");name2.push_back("AllSBins_v7_Ele");rebin.push_back(1);
  //  name1.push_back("AllSBins_v4_Pho");          name2.push_back("AllSBins_v4_Ele");      rebin.push_back(1);
  
  TLegend *legend[name1.size()];//=new TLegend(0.6, 0.90,  0.98, 0.45);
  TCanvas *c_cA[name1.size()];
  TLine *line1=new TLine( 8.5,0.01,  8.5,90);
  TLine *line2=new TLine(15.5,0.01, 15.5,90);
  TLine *line3=new TLine(22.5,0.01, 22.5,90);
  TLine *line4=new TLine(29.5,0.01, 29.5,90);
  TLine *line5=new TLine(36.5,0.01, 36.5,9 );
  TLine *line1V7=new TLine( 6.5,0.01,  6.5,200);
  TLine *line2V7=new TLine(11.5,0.01, 11.5,200);
  TLine *line3V7=new TLine(16.5,0.01, 16.5,200);
  TLine *line4V7=new TLine(21.5,0.01, 21.5,200);
  TLine *line5V7=new TLine(26.5,0.01, 26.5,200);

  line1V7->SetLineStyle(2);
  line2V7->SetLineStyle(2);
  line3V7->SetLineStyle(2);
  line4V7->SetLineStyle(2);
  line5V7->SetLineStyle(2);
  //  TCanvas *c_cB=new TCanvas("closure_test","closure test",1500,800); c_cB->Divide(4,2);
  TPad *p_top[name1.size()];
  TPad *p_bot[name1.size()];

  for(int i=0;i<name1.size();i++){
    name=name1[i]+name2[i];
    c_cA[i]=new TCanvas(name,name,1500,800);//c_cA[i]->Divide(4,2);
    p_top[i]=new TPad(name+"_top",name+"_top",0,0.4,1,1);
    p_bot[i]=new TPad(name+"_bot",name+"_bot",0,0.0,1,0.4);
    p_top[i]->Draw();p_top[i]->SetGridx();p_top[i]->SetGridy();p_top[i]->SetLogy();
    p_top[i]->SetBottomMargin(0);
    p_bot[i]->SetTopMargin(0);
    p_bot[i]->SetBottomMargin(0.3);
    p_bot[i]->Draw();p_bot[i]->SetGridx();p_bot[i]->SetGridy();    

    
    TH1D *h_histG,*h_histE,*h_histGcopy;
    
    for(int j=0;j<nfiles;j++){
      c_cA[i]->cd();
      //c_cB->cd(i+1);
      name=name1[i];
      h_histG=(TH1D*)f[j]->FindObjectAny(name);//h_histG->Rebin(2);
      h_histG->Rebin(rebin[i]);//h_histG->Scale(1.0/h_histG->Integral());
          
      name=name2[i];
      h_histE=(TH1D*)f[j]->FindObjectAny(name);//h_histE->Scale(150.9/481.6);
      h_histE->Rebin(rebin[i]);//h_histE->Scale(1.0/h_histE->Integral());
     
      if(h_histG && h_histE){
	if(name1[i]=="nHadJets_Pho"){
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
	h_histG->GetYaxis()->SetLabelSize(0.10);
	h_histG->GetYaxis()->SetTitleSize(0.10);
	h_histG->GetYaxis()->SetTitleOffset(0.45);
	h_histG->GetYaxis()->SetNdivisions(10);
	
	h_histE->SetLineColor(kBlue);
	h_histE->SetLineWidth(2);
	h_histE->SetMarkerStyle(20);
	h_histE->SetMarkerColor(h_histE->GetLineColor());

	c_cA[i]->cd();p_top[i]->cd();
	//c_cB->cd(i+1);p_top[i]->cd();
	h_histG->GetYaxis()->SetRangeUser(0.04,3000);
	h_histG->Draw("e1");
	h_histG->Draw("same e0");
	// h_histE->SetFillStyle(3004);
	// h_histE->SetFillColor(h_histE->GetLineColor());
 	h_histE->Draw("same e1");
 	h_histE->Draw("same e0");
	//	h_histE->Draw("L same");
	h_histG->GetYaxis()->SetTickLength(0.0328/1.5);
	//	legend[i]=new TLegend(0.65, 0.88,  0.87, 0.7);
	legend[i]=new TLegend(0.5246996,0.7427583,0.8551402,0.8421963,NULL,"brNDC");
	legend[i]->SetNColumns(2);
	legend[i]->SetBorderSize(1);
	name=name1[i];
	legend[i]->AddEntry(h_histG,"Expected","lp");
	name=name2[i];
	legend[i]->AddEntry(h_histE,"Predicted","lp");
	legend[i]->Draw();
	
	TString name = h_histG->GetName();
	if(name.Contains("SBins_v4")){ 
	  line1->Draw();	line2->Draw();	line3->Draw();	line4->Draw();	line5->Draw();
	  p_top[i]->SetGridx(0);
	}

	else if(name.Contains("SBins_v7")){ 
	  line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
	  p_top[i]->SetGridx(0); 	  p_top[i]->SetGridy(0);
	}	
      }
    }
    
    TH1D *h_numr=(TH1D*)h_histG->Clone();
    
    h_numr->Divide(h_histE);
    TString xaxisName = getXaxisName(h_histG->GetName());

    h_numr->SetLineColor(kBlack);
    h_numr->SetMarkerColor(kBlack);
    h_numr->SetTitle(";;Exp. / Pred.");
    h_numr->GetXaxis()->SetLabelSize(0.13);
    h_numr->GetXaxis()->SetTitle(xaxisName);
    h_numr->GetXaxis()->SetTitleSize(0.13);
    h_numr->GetXaxis()->SetTitleOffset(0.95);

    h_numr->GetYaxis()->SetTitleOffset(0.35);
    h_numr->GetYaxis()->SetTitleSize(0.13);
    h_numr->GetYaxis()->SetLabelSize(0.13);
    h_numr->GetYaxis()->SetNdivisions(505);
    h_numr->SetMaximum(1.90);
    h_numr->SetMinimum(0.01);
    c_cA[i]->cd();    p_bot[i]->cd();
    //    c_cB->cd(i+1);    p_bot[i]->cd();
    h_numr->Draw("e0");
    h_numr->Draw("e1 same");
    h_numr->GetYaxis()->SetTickLength(0.042/1.5);
    h_FRUnc->SetFillStyle(3013);
    h_FRUnc->SetFillColor(1);
    h_FRUnc->Draw("E2same");

    TString name = h_histG->GetName();
    if(name.Contains("SBins_v4")){ 
      line1->Draw();	line2->Draw();	line3->Draw();	line4->Draw();	line5->Draw();
    }
    else if(name.Contains("SBins_v7")){
      line1V7->Draw();	line2V7->Draw();	line3V7->Draw();	line4V7->Draw();	line5V7->Draw();
      p_bot[i]->SetGridx(0);
      TH1D *h_pullHist = new TH1D("pull_Fakerate","1D pull for fakerate",50,-4.5,10.5);
      for(int p=1;p<=h_numr->GetNbinsX();p++){
        h_pullHist->Fill( (1.0-h_numr->GetBinContent(p))/h_numr->GetBinError(p));
	//	cout<<p<<" "<<(1.0-h_numr->GetBinContent(p))/h_numr->GetBinError(p)<<endl;
      }
    }
    
    c_cA[i]->cd();    p_top[i]->cd();
    char name2[100];
    textOnTop.SetTextSize(0.07);
    intLumiE.SetTextSize(0.07);
    textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation Supplementary}}");
    sprintf(name2,"#bf{%0.1f fb^{-1} (13 TeV)}",intLumi);
    intLumiE.DrawLatexNDC(0.72,0.91,name2);
   
    c_cA[i]->cd();    p_top[i]->cd();
    TArrow *arrow1 = new TArrow(0.5,60,6.5,60,0.01,"<|>");
    TArrow *arrow2 = new TArrow(6.5,60,11.5,60,0.01,"<|>");
    TArrow *arrow3 = new TArrow(11.5,60,16.5,60,0.01,"<|>");
    TArrow *arrow4 = new TArrow(16.5,60,21.5,60,0.01,"<|>");
    TArrow *arrow5 = new TArrow(21.5,60,26.5,60,0.01,"<|>");
    TArrow *arrow6 = new TArrow(26.5,60,31.5,60,0.01,"<|>");
    arrow1->Draw(); arrow2->Draw(); arrow3->Draw();
    arrow4->Draw(); arrow5->Draw(); arrow6->Draw();
    TLatex Tl;
    Tl.SetTextSize(0.06);
    Tl.DrawLatex(3.,90,"N^{ 0}_{ 2-4}");
    Tl.DrawLatex(8.5,90,"N^{ 0}_{ 5-6}");
    Tl.DrawLatex(13.5,90,"N^{ 0}_{ #geq7}");
    Tl.DrawLatex(18.5,90,"N^{ #geq1}_{ 2-4}");
    Tl.DrawLatex(23.5,90,"N^{ #geq1}_{ 5-6}");
    Tl.DrawLatex(28.5,90,"N^{ #geq1}_{ #geq7}");

    Tl.SetTextSize(0.06);
    Tl.DrawLatexNDC(0.48,0.91,"#bf{arXiv:xxxx.xxxxx}");
    
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
  else if(axname.Contains("BTags")) return "b tags";
  else if(axname.Contains("ElePt")) return "e pT(GeV)";
  else if(axname.Contains("EleEta")) return "e #eta";
  else if(axname.Contains("ElePhi")) return "e #Phi";
  else if(axname.Contains("MuPt")) return "#mu pT(GeV)";
  else if(axname.Contains("MuEta")) return "#mu #eta";
  else if(axname.Contains("PhotonPt")) return "#gamma pT(GeV)";
  else if(axname.Contains("PhotonEta")) return "#gamma #eta";
  else if(axname.Contains("PhotonPhi")) return "#gamma #Phi";
  else if(axname.Contains("mTPhoEleMET")) return "mT(#gamma+e,p_{T}^{miss})(GeV)";
  else if(axname.Contains("mTPho")) return "mT(#gamma,p_{T}^{miss})(GeV)";
  else if(axname.Contains("dR_ElePho")) return "#DeltaR(e,#gamma)";
  else if(axname.Contains("AllSBin")) return "Bin Number";
  else if(axname.Contains("nVtx")) return "No. of primary vertices";
  else if(axname.Contains("dPhi_METjet1") || axname.Contains("dphi1_METjet1")) return "#Delta#Phi_{1}";
  else if(axname.Contains("dPhi_METjet2") || axname.Contains("dphi2_METjet2")) return "#Delta#Phi_{2}";
  else if(axname.Contains("dPhi_METBestPhoton") ) return "#Delta#Phi(MET,#gamma)";
  else if(axname.Contains("dPhi_Muon_Photon") ) return "#Delta#Phi(#mu,#gamma)";
  else if(axname.Contains("QMut") || axname.Contains("Qmult")) return "QMult";
  else if(axname.Contains("MT_Ele")) return "mT(e,p_{T}^{miss})(GeV)";
  else if(axname.Contains("MET")) return "p_{T}^{miss}(GeV)";
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
