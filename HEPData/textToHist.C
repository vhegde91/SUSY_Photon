void textToHist(string f){
  // TString fName = f, histName = "AllSBins_v7_CD";
  // fName.ReplaceAll(".txt",".root");
  
  TH1D *h = new TH1D("h", "example histogram",25,0.5,25.5);
  ifstream inp; double x, y, errY;
  inp.open(f);
  for (int i=1; i<=25; i++) {
    inp >> x;
    inp >> y;
    inp >> errY;
    h->SetBinContent(i,y);
    h->SetBinError(i,errY);
  }
  h->Draw();
}
