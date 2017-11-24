void fitter (char * filename = "DMS_Run2016_FR_NoZWindow.root", float startmass = 91) {
  //void fitter (TString filename = "plots_loose.root", float startmass = 91) {
  using namespace RooFit;

  //  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gROOT->SetStyle("Plain");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetTitleOffset(1.8,"y");
  gStyle->SetTitleW(.54);
  TCanvas * c1 = new TCanvas("c1", "c1",430, 10, 800, 800);
  c1->SetBorderMode(0);
  
  //Parameter used for mass in both CrystalBall or BreitWigner
  //  RooRealVar mass("mass","Mass_{2#gamma}", 70, 130,"GeV/c^{2}");
  RooRealVar mass("mass","Mass_{2#gamma}", 80, 130,"GeV/c^{2}");//vvvv
  //mass.setBins(10000,"cache");

  //Parameters for Crystal Ball Lineshape 
  //  RooRealVar m0("M_{Z}", "Bias", startmass, 70, 130);//,"GeV/c^{2}");
  RooRealVar m0("M_{Z}", "Bias", startmass, 70, 130);//,"GeV/c^{2}"); //vvv
  RooRealVar sigma("#sigma_{CB}","Width", 1.2,0.1,10);//,"GeV/c^{2}"); 
  RooRealVar cut("#alpha","Cut", 1., 0., 10.); 
  RooRealVar power("#gamma","Power", 1., 0., 10.); 
  RooCBShape CrystalBall("CrystalBall", "A  Crystal Ball Lineshape", mass, m0,sigma, cut, power);
  //RooCBShape ResolutionModel("ResolutionModel", "Resolution Model", mass, m0,sigma, cut, power);
            
  //Parameters for Breit-Wigner Distribution
  RooRealVar mRes("#DeltaM_{Z}", "Z Resonance  Mass", 0, -0.0021, 0.0021);//,"GeV/c^{2}"); 
  RooRealVar Gamma("#Gamma_{Z}", "#Gamma", 2.4952, 2.4929, 2.4975);//,"GeV/c^{2}"); 
  RooBreitWigner BreitWigner("BreitWigner","A Breit-Wigner Distribution",mass,mRes,Gamma);

  //Convoluve the BreitWigner and Crystal Ball
  RooFFTConvPdf ResolutionModel("Convolution","Convolution", mass, CrystalBall, BreitWigner);

  //Introduce a resolution model
  //Gaussian Sigma
  //RooRealVar GaussianSigma("#sigma_{G}","Core Width", 0.5,0,1);
  //RooRealVar GaussianMean("GaussianMean","Gaussian Mean",0.0,-1.0,1.0);
      
  //RooBifurGauss  res("res", "A Bifurcated Gaussian Distribution", deltam, mu,sigL,sigR);
  //RooGaussian GaussianResolution("GaussianResolution", "A  Gaussian Lineshape", mass, GaussianMean,GaussianSigma);
  //RooGaussian ResolutionModel("GaussianResolution", "A  Gaussian Lineshape", mass, m0,GaussianSigma);
  //RooRealVar fracG("f_{G}", "Gaussian Fraction", 0.0,0.0,1.0);
  //RooAddPdf ResolutionModel("ResolutionModel", "Resolution Model", GaussianResolution, CrystalBall, fracG); 
  //fracG.setConstant();
  //RooFFTConvPdf ResolutionModel("ResolutionModel","Resolution Model", mass, CrystalBall, GaussianResolution);

  TFile * hFile = new TFile(filename);
  //  TH1F * DataHist = (TH1F*) hFile->Get("h_elec_zmass");
  TH1F * DataHist = (TH1F*) hFile->Get("ZMass_Pho");
  DataHist->UseCurrentStyle();
  TString DataHistName = DataHist->GetName();
  DataHistName += "fit";
  RooDataHist data(DataHistName,DataHistName,mass,DataHist);
  ResolutionModel.fitTo(data,Range(startmass-20,startmass+10));
  RooPlot * plot = mass.frame();
  data.plotOn(plot,DataError(RooAbsData::None));
  plot->SetMaximum(1.2*plot->GetMaximum());
  //CrystalBall.plotOn(plot);
  //CrystalBall.paramOn(plot,Format("NEL",FixedPrecision(4)), Parameters(RooArgSet(m0, sigma, cut, power)), Layout(.55, 0.99, 0.99), ShowConstants(kFALSE));
  //CrystalBall.plotOn(plot);
  //BreitWigner.plotOn(plot);
  ResolutionModel.plotOn(plot);
  ResolutionModel.paramOn(plot,Format("NEL",AutoPrecision(1)), Parameters(RooArgSet(m0, sigma, cut, power, mRes, Gamma)), Layout(.15, 0.5, 0.9), ShowConstants(kFALSE));
  TString PlotTitle(DataHist->GetTitle());
  PlotTitle += ";Mass (GeV/c^{2});Events";
  plot->SetTitle(PlotTitle);
  plot->GetXaxis()->SetRangeUser(0,startmass+20);
  TPaveText *box = (TPaveText*) plot->findObject("Convolution_paramBox");
  box->SetTextSize(0.03);
  plot->Draw();
  TString OutPutName = DataHistName + "fit.png";
  c1->SaveAs(OutPutName);
  
}
