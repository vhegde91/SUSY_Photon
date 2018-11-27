TString getLegName(TString);
void plotCutFlow(TString name){
  gStyle->SetOptStat(0);
  gStyle->SetPadGridY(0);
  TLatex textOnTop,intLumiE;
  double intLumi=35.9;
  TCanvas *c=new TCanvas("name","",1200,800);
  TLegend *leg = new TLegend(0.2,0.8,0.82,0.87); leg->SetBorderSize(0); leg->SetMargin(0.);
  TFile *f = new TFile(name);
  TH1D *h1 = (TH1D*)f->Get("cutFlow");

  TString name3 = name; name3.ReplaceAll("withCutFlow","genMET");
  TFile *fg = new TFile(name3);
  TH1D *h2 = (TH1D*)fg->Get("cutFlow");
  h1->Add(h2); h1->Scale(0.5);
  double N0 = h1->GetBinContent(1);
  double N0Err = h1->GetBinError(1);
  h1->Scale(0.98);
  h1->SetBinContent(1,N0);
  h1->SetBinError(1,N0Err);

  TString modelName;
  if(name.Contains("T5bbbbZg_1800_150")) modelName = "CutFlow_T5bbbbZg_1800_150";
  else if(name.Contains("T5bbbbZg_1800_1750")) modelName = "CutFlow_T5bbbbZg_1800_1750";
  else if(name.Contains("T5ttttZg_1800_150")) modelName = "CutFlow_T5ttttZg_1800_150";
  else if(name.Contains("T5ttttZg_1800_1550")) modelName = "CutFlow_T5ttttZg_1800_1550";
  else if(name.Contains("T5qqqqHg_1800_150")) modelName = "CutFlow_T5qqqqHg_1800_150";
  else if(name.Contains("T5qqqqHg_1800_1750")) modelName = "CutFlow_T5qqqqHg_1800_1750";
  else if(name.Contains("T6ttZg_1000_100")) modelName = "CutFlow_T6ttZg_1000_100";
  else if(name.Contains("T6ttZg_1000_900")) modelName = "CutFlow_T6ttZg_1000_900";

  h1->SetTitle(";;Events"); 
  h1->GetXaxis()->SetRangeUser(0,10); 
  h1->SetMaximum(150);
  TH1D *h1cp=(TH1D*)h1->Clone(modelName);
  leg->AddEntry(h1cp,getLegName(modelName),"lp");
  h1cp->Draw("text");
  h1cp->SetMarkerSize(2.0);
  leg->Draw();

  char name2[100];
  textOnTop.SetTextSize(0.05);
  intLumiE.SetTextSize(0.05);
  textOnTop.DrawLatexNDC(0.12,0.91,"CMS #it{#bf{Simulation Supplementary}}");
  sprintf(name2,"#bf{%0.1f fb^{-1} (13TeV)}",intLumi);
  intLumiE.DrawLatexNDC(0.65,0.91,name2);
  h1cp->SaveAs(modelName+".root");
  c->SaveAs(modelName+".pdf");  
}


TString getLegName(TString fname){
  if(fname.Contains("T5bbbbZg_1800_150")){return "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 150 GeV)";}
  else if(fname.Contains("T5bbbbZg_1800_1750")){return "#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 1750 GeV)";}
  else if(fname.Contains("T5qqqqHg_1800_150")){return "#tilde{g} #rightarrow q #bar{q} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/H #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 150 GeV)";}
  else if(fname.Contains("T5qqqqHg_1800_1750")){return "#tilde{g} #rightarrow q #bar{q} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/H #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 1750 GeV)";}
  else if(fname.Contains("T5ttttZg_1800_150")){return "#tilde{g} #rightarrow t #bar{t} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 150 GeV)";}
  else if(fname.Contains("T5ttttZg_1800_1550")){return "#tilde{g} #rightarrow t #bar{t} #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1800 GeV, m_{#tilde{#chi}_{1}^{0}} = 1550 GeV)";}
  else if(fname.Contains("T6ttZg_1000_100")){return "#tilde{t} #rightarrow t #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1000 GeV, m_{#tilde{#chi}_{1}^{0}} = 100 GeV)";}
  else if(fname.Contains("T6ttZg_1000_900")){return "#tilde{t} #rightarrow t #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow #gamma/Z #tilde{G} (m_{#tilde{g}} = 1000 GeV, m_{#tilde{#chi}_{1}^{0}} = 900 GeV)";}
  else return fname;
}
