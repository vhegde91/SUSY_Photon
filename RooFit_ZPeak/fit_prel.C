using namespace RooFit;
//TFile * hFile = new TFile("DMS_Run2016_FR_ST200_NoZWindow.root");
TFile * hFile = new TFile("MS_FR_SigBG_ST200NoZWindow.root");
TH1F * DataHist = (TH1F*) hFile->Get("ZMass_Ele");

void fit_prel(){
  TCanvas *c1=new TCanvas("c1","",1000,1000);
  RooRealVar x("x", "Mass (GeV/c^{2})", 60, 120);
  RooDataHist data("data","data",x,DataHist);
  
  RooRealVar bwmean("bwmean", "bwmean" , 90.0, 20, 180);
  RooRealVar bwsigma("bwsigma", "bwsigma" , 4.12, 0.1, 100.0);
  RooRealVar bwsig("bwsig", "signal", 10, 0, 1000000);
  RooBreitWigner bwgauss("bwgauss","bwgauss", x, bwmean, bwsigma);

  //  RooRealVar cbmean("cbmean", "cbmean" , 90.0, 20, 180.0) ;
  RooRealVar cbmean("cbmean", "cbmean" , 0.0, -0.001, 0.001) ;
  RooRealVar cbsigma("cbsigma", "cbsigma" , 2.0, 1.0, 40.0) ;
  RooRealVar cbsig("cbsig", "cbsignal", 10, 0, 1000000);
  RooRealVar n("n","", 5.1);
  RooRealVar alpha("alpha","", 1.3);
  RooCBShape cball("cball", "crystal ball", x, cbmean, cbsigma, alpha, n);

  RooRealVar a("a", "a", -2.0, 2.0);
  // RooRealVar a1("a1","a1",0,0.2);
  // RooRealVar a2("a2","a2",-0.002,0.002);
  // RooRealVar a3("a3","a3",0.0,0.000006);
  //TF1 *for1 = new TF1("for1","exp(([0]*x)+([1]*x*x))",60,120);
  //  RooAbsReal *expo = bindFunction(for1,a1);
  //  RooAbsReal *expo = bindFunction(for1,x,RooArgList(a1,a2,a3));

  RooExponential expo("expo", "exponential", x, a);
  // RooPolynomial expo("expo", "exponential", x, a1,3);

  // RooRealVar mean("mean", "mean", 90.0, 20.0, 160.0);
  // RooRealVar sigma("sigma", "sigma", 3.0, 0.1, 20.0);
  // RooGaussian gauss("gauss", "gauss", x, mean, sigma);

  RooRealVar b("background", "background yield", 10, 0, 1000000);

  RooRealVar bwcbsig("bwcbsig", "bwcbsignal", 10, 0, 1000000);
  RooFFTConvPdf bwcbconv("bwcbconv","bwcbconv",x,bwgauss,cball);//vvv
  RooAddPdf sum("sum", "", RooArgList(bwcbconv, expo), RooArgList(bwcbsig, b));
  //  RooAddPdf sum("sum", "", RooArgList(bwgauss, cball, expo), RooArgList(bwsig, cbsig, b));


  // RooAbsReal intgr=sum.createIntegral(x,"range1");
  //  bwgauss.fitTo(data);
  //  gauss.fitTo(data);
  // expo.fitTo(data);
  //cball.fitTo(data);
  sum.fitTo(data);

  RooPlot *xframe=x.frame();
  data.plotOn(xframe);
  // bwgauss.plotOn(xframe,LineColor(kRed));
  // gauss.plotOn(xframe,LineColor(kGreen));
  // cball.plotOn(xframe,LineColor(kMagenta));
  // expo.plotOn(xframe,LineStyle(kDashed),LineColor(kBlue));
  sum.plotOn(xframe, LineColor(kMagenta));
  sum.plotOn(xframe, RooFit::Components(expo), RooFit::LineStyle(kDashed));
  sum.plotOn(xframe, Components(RooArgSet(expo)),DrawOption("F"), FillColor(kGreen));
  data.plotOn(xframe);
  //  b.setRange("r1",80,100);
  sum.paramOn(xframe, Layout(0.6), Format("NEU", AutoPrecision(1)),Parameters(RooArgList(bwmean, bwsigma, cbmean, cbsigma, bwsig,cbsig, b, bwcbsig)));
  c1->SetLogy();
  xframe->Draw();
  cout<<"---------------------------------------------"<<sum.expectedEvents(RooArgList(bwsig,cbsig))<<endl;
   
  b.Print();
  a.Print();
  bwcbsig.Print();
  n.Print();
  alpha.Print();
  cout<<"============================================="<<endl;

 

}

