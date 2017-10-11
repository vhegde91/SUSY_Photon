//double fitfunc(double*,double*);
double fitfunc2(double*,double*);
void fitMET(){
  TCanvas *c1=new TCanvas("c1","c1",900,800);
  c1->SetLogy();
  TFile *f1=new TFile("HLR_gjets_qcd.root");
  //  TH1D *h1=(TH1D*)f1->Get("MET_CD");
  TH1D *h1=(TH1D*)f1->Get("HLratio_1D");
  TH1D *h2=(TH1D*)f1->Get("MET_AB");
  h1->Rebin(1);
  //  h2->Rebin(2);

  gStyle->SetOptFit(101);
  h1->Draw();

  //TF1 *fit1=new TF1("fit1","([1]*exp([2]*x)/(x*x))+([3]*exp([4]*x*x)/(x))",100,2000);
  // TF1 *fit1=new TF1("fit1",fitfunc,100,2000,4);
  // fit1->SetParameters(1.23698e+06,-5.53911e-02,3.11799e+03,-2.44042e-02);
  TF1 *fit2=new TF1("fit2",fitfunc2,0.5,6.5,4);
  fit2->SetParameters(1.79062e+01,-1.99314e-02,0.0001,0.3);
  h1->Fit("fit2","R");
  //  h2->SetLineColor(kGreen);
  // h2->Draw("sames");
  // h2->Fit("fit1","R");
}

// double fitfunc(double *x, double *p){
//   //  return p[0]*x[0]+p[2]*exp(p[3]*x[0]*x[0]);
//   return p[0]*exp(p[1]*x[0])+p[2]*exp(p[3]*x[0]);
// }

double fitfunc2(double *x, double *p){
  //  return p[0]*x[0]+p[2]*exp(p[3]*x[0]*x[0]);
  return p[0]*exp(p[1]*x[0])+p[2]*x[0]+p[3];
}
