void smoothGraphFmTH2D(TString fName,TString histName){
  TFile *f=TFile::Open(fName);
  TH2D *h2=(TH2D*)f->FindObjectAny(histName);
  TGraph2D *gr2 = new TGraph2D(h2);
  TCanvas *c3=new TCanvas("c3","c3",1600,1200);
  c3->SetRightMargin(0.2);
  gr2->Draw("colz");
  gr2->SetTitle(";M3(GeV);M1(GeV)");
  gr2->GetZaxis()->SetTitle("Cross section (pb)");
  // TList *listContr =gr2->GetContourList(0.000981077);
  // TGraph *gr1d=(TGraph*)listContr->First();
  // gr1d->Draw("C");
}
