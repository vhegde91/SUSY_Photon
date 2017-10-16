void printMassPoints(TString name){
  TFile *f2=new TFile(name);
  TH2D *h2_mass = (TH2D*)f2->FindObjectAny("MGlMNLSP");
  for(int i=1;i<=h2_mass->GetNbinsX();i++){
    for(int j=1;j<=h2_mass->GetNbinsY();j++){
      if(h2_mass->GetBinContent(i,j)>0.1){
	cout<<h2_mass->GetXaxis()->GetBinCenter(i)<<" "<<h2_mass->GetYaxis()->GetBinCenter(j)<<endl;
      }
    }
  }
    
}
