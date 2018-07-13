#include "TH1.h"
{
  TH1D * h1 = new TH1D("h1","h1",50,-4,4);
  h1->Sumw2(kFALSE);
  h1->FillRandom("gaus",100);

  h1->SetBinErrorOption(TH1::kPoisson);

  //example: lower /upper error for bin 20
  int ibin = 20;
  double err_low = h1->GetBinErrorLow(ibin);
  double err_up = h1->GetBinErrorUp(ibin);
    

  // draw histogram with errors 
  // use the drawing option "E0" for drawing the errors
  // for the bin with zero content

  h1->SetMarkerStyle(20);
  h1->Draw("E");
}   
