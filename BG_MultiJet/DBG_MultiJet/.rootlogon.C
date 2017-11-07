{
  gStyle->SetOptStat("nemri");

  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);

  gStyle->SetHistLineWidth(2);

  gStyle->SetTitleSize(0.05,"X");
  gStyle->SetLabelSize(0.05,"X");
  gStyle->SetTitleSize(0.05,"Y");
  gStyle->SetLabelSize(0.05,"Y");
  gStyle->SetTitleSize(0.05,"Z");
  gStyle->SetLabelSize(0.05,"Z");

  //  gStyle->SetGridColor(kGray+1);
  gROOT->ForceStyle();
}
