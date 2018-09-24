void GausPoissonToyRatio(){
  double g1=100,p1=100,r1=1000;
  TH1D *g=new TH1D("hg","gaus",2000,0,200);
  TH1D *p=new TH1D("hp","pois",20000,0,200);
  TH1D *ratio1=new TH1D("ratio","ratio",2000,0,5);
  TGraph *gr;

  TRandom r;
  Long64_t nToys = 30000000,poisMean=-1;
  vector<double> x,y;
  for(int ip=85;ip<=95;ip++){// for many poisson
    //for(int ip=91;ip==91;ip++){//for poisson mean 91
    poisMean=ip;
    x.push_back(poisMean);
    for(int i=0;i<nToys;i++){
      g1=r.Gaus(91,10);//gauss with mean 91 and rms 10
      p1=r.PoissonD(poisMean);//poisson with mean 51
      g->Fill(g1);
      p->Fill(p1);
      ratio1->Fill(p1/g1);//take ratio of poisson to gauss
    }
    y.push_back(ratio1->Integral(1,ratio1->FindBin(0.56))/nToys);
    ratio1->Reset();//comment if only one poisson
  }
  g->SetLineColor(kRed);
  // p->SetLineColor(kBlack);
  
  //g->Draw();
  //  p->Draw();
  ratio1->Draw();
  gr=new TGraph(x.size(),&(x[0]),&(y[0]));
  gr->SetMarkerStyle(20);
  gr->Draw("ap");//draw p-value vs poisson mean
  gr->Print("all");
  
  // cout<<"---------------------------------"<<endl;
  // cout<<"Toys|"<<"Poisson Mean|"<<"r < 0.56|"<<"p-value"<<endl;
  // cout<<nToys<<" "<<poisMean<<" "<<ratio1->Integral(1,ratio1->FindBin(0.56))<<" "<<ratio1->Integral(1,ratio1->FindBin(0.56))/nToys<<endl;
  
}
