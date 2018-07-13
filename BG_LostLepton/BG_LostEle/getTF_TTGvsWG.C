void getTF_TTGvsWG(){
  TFile *wg=new TFile("CS_wGwJ_LostEle_v2.root");
  TFile *ttg=new TFile("CS_ttGttJ_LostEle_v2.root");
  // TFile *wg=new TFile("CS_WG_LostEle_v2.root");
  // TFile *ttg=new TFile("CS_TTG_LostEle_v2.root");
  gStyle->SetOptStat(0);

  //------------- print WG TFs -----------------
  TH1D *hL=(TH1D*)wg->Get("nHadJets_Ele0");
  TH1D *hF=(TH1D*)wg->Get("nHadJets_Ele1");
  double errN=0,errD=0;
  double intrN=hL->IntegralAndError(0,hL->GetNbinsX(),errN);
  double intrD=hF->IntegralAndError(0,hF->GetNbinsX(),errD);
  cout<<"for WG: lost/found---> "<<intrN<<" +/- "<<errN<<" / "<<intrD<<" +/- "<<errD<<endl;
  cout<<"for WG TF: "<<intrN/intrD<<" +/- "<<(intrN/intrD)*sqrt(pow(errN/intrN,2)+pow(errD/intrD,2))<<endl;
  //------------- print TTG TFs -----------------
  cout<<endl;
  hL=(TH1D*)ttg->Get("nHadJets_Ele0");
  hF=(TH1D*)ttg->Get("nHadJets_Ele1");
  errN=0,errD=0;
  intrN=hL->IntegralAndError(0,hL->GetNbinsX(),errN);
  intrD=hF->IntegralAndError(0,hF->GetNbinsX(),errD);
  cout<<"for TTG: lost/found---> "<<intrN<<" +/- "<<errN<<" / "<<intrD<<" +/- "<<errD<<endl;
  cout<<"for TTG TF: "<<intrN/intrD<<" +/- "<<(intrN/intrD)*sqrt(pow(errN/intrN,2)+pow(errD/intrD,2))<<endl;

  TCanvas *cA=new TCanvas("cA","cA",1500,600);
  cA->Divide(2,1);

  int nb=0;
  TH2D *h2_TFwg=(TH2D*)wg->Get("hadJbTag_Ele0");
  h2_TFwg->Divide((TH2D*)wg->Get("hadJbTag_Ele1"));

  TH2D *h2_TFttg=(TH2D*)ttg->Get("hadJbTag_Ele0");
  h2_TFttg->Divide((TH2D*)ttg->Get("hadJbTag_Ele1"));
  
  TLegend *leg=new TLegend(0.8,0.8,0.88,0.88);
  
  TH1D *h1_TFwg0,*h1_TFttg0;
  TH1D *h1_TFwg1,*h1_TFttg1;

  h1_TFwg0=(TH1D*)h2_TFwg->ProjectionX("WG0_TF",1,1);
  h1_TFttg0=(TH1D*)h2_TFttg->ProjectionX("TTG0_TF",1,1);
  h1_TFwg0->SetLineColor(kRed);
  h1_TFttg0->SetLineColor(kBlue);
  leg->AddEntry(h1_TFwg0,"W#gamma","lpe");
  leg->AddEntry(h1_TFttg0,"tt#gamma","lpe");
  h1_TFwg0->SetTitle("0b region;N-jets;Lost e TF");
  cA->cd(1);
  h1_TFwg0->SetMinimum(0.12);
  h1_TFwg0->SetMaximum(1.19);
  h1_TFwg0->Draw();
  h1_TFttg0->Draw("sames");
  leg->Draw();

  h1_TFwg1=(TH1D*)h2_TFwg->ProjectionX("WG1_TF",2,2);
  h1_TFttg1=(TH1D*)h2_TFttg->ProjectionX("TTG1_TF",2,2);
  h1_TFwg1->SetLineColor(kRed);
  h1_TFttg1->SetLineColor(kBlue);
  h1_TFwg1->SetTitle("#geq1b region;N-jets;Lost e TF");
  cA->cd(2);
  h1_TFwg1->SetMinimum(0.12);
  h1_TFwg1->SetMaximum(1.19);
  h1_TFwg1->Draw();
  h1_TFttg1->Draw("sames");
  leg->Draw();
  
}
