void plotPhotonEff(TString name){
  TFile *f = TFile::Open(name);
  TH2D *h2r = (TH2D*)f->Get("RecoPhoPtEta");
  TH2D *h2g = (TH2D*)f->Get("GenPhoPtEta");
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.3f");
  h2r->GetZaxis()->SetRangeUser(0,1.0);
  for(int i=1;i<=h2r->GetNbinsX();i++){
    for(int j=1;j<=h2r->GetNbinsY();j++){
      if(h2r->GetYaxis()->GetBinCenter(j) < 110){
	h2r->SetBinContent(i,j,0);
	h2r->SetBinError(i,j,0);
	h2g->SetBinContent(i,j,0);
	h2g->SetBinError(i,j,0);
      }
      if(h2r->GetXaxis()->GetBinCenter(i) > 1.44 && h2r->GetXaxis()->GetBinCenter(i) < 1.52){
	h2r->SetBinContent(i,j,0);
	h2r->SetBinError(i,j,0);
	h2g->SetBinContent(i,j,0);
	h2g->SetBinError(i,j,0);
      }
      // h2r->SetBinContent(h2r->GetXaxis()->FindBin(1.49),h2r->GetYaxis()->FindBin(150),0);
    }
  }
  // h2r->RebinY(h2r->GetNbinsY());
  // h2g->RebinY(h2g->GetNbinsY());
  h2r->Divide(h2g);
  h2r->Draw("colz texte");
  h2r->SetTitle(";Gen |#eta^{#gamma}|;Gen p_{T}^{#gamma} (GeV)");

  return;

  vector<double> eta={0.4, 1.12221, 1.783, 2.25};
  vector<double> etaErr={0.4, 0.3221, 0.217, 0.25};

  ////////////// Pixel veto /////////////////////////////  
  vector<double> offEff={0.848, 0.809, 0.723, 0.612};
  vector<double> offEffErr={0.019, 0.013, 0.024, 0.021};
  vector<double> myEff={0.844, 0.790, 0.710, 0.579};
  vector<double> myEffErr={0.002, 0.002, 0.002, 0.002};
  //////////////////////////////////////////////////////
  ////////////// CSEV /////////////////////////////////
  // vector<double> offEff={0.875, 0.889, 0.850, 0.856};
  // vector<double> offEffErr={0.019, 0.013, 0.020, 0.021};
  // vector<double> myEff={0.870, 0.869, 0.837, 0.824};
  // vector<double> myEffErr={0.002, 0.002, 0.002, 0.002};
  //////////////////////////////////////////////////////

  TGraphErrors *grOff = new TGraphErrors(eta.size(), &(eta[0]), &(offEff[0]), &(etaErr[0]), &(offEffErr[0]));
  TGraphErrors *grMy = new TGraphErrors(eta.size(), &(eta[0]), &(myEff[0]), &(etaErr[0]), &(myEffErr[0]));
  TLegend *leg = new TLegend(0.4,0.5,0.6,0.6);
  TCanvas *c = new TCanvas("name","",800,800);
  leg->AddEntry(grOff,"Pixel veto (official)","lp");
  leg->AddEntry(grMy,"Pixel veto + CSEV (private, MC)","lp");

  // leg->AddEntry(grOff,"CSEV (offical)","lp");
  // leg->AddEntry(grMy,"CSEV (Private, MC)","lp");

  grOff->SetTitle(";|#eta|;Efficiency");
  grOff->SetLineColor(kRed);
  grOff->SetLineWidth(2);
  grOff->SetMaximum(1.0);
  grOff->SetMinimum(0.5);
  grMy->SetLineWidth(2);
  grOff->Draw("AP");
  grMy->Draw("sames");
  leg->Draw();
}
