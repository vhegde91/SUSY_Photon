#!/usr/bin/env python
from ROOT import *
from array import *
gROOT.SetBatch(True)
import sys, os
from os.path import isfile, join
import math

# ------------------------------
def SetHistoStyle( histo ) :
    histo.SetLineColor(2)
    histo.SetMarkerColor(1)
    histo.GetXaxis().SetTitleFont(42);
    histo.GetYaxis().SetTitleFont(42);
    histo.GetXaxis().SetTitleSize(0.05);
    histo.GetYaxis().SetTitleSize(0.05);
    histo.GetXaxis().SetTitleFont(42)
    histo.GetYaxis().SetTitleFont(42)
    histo.GetXaxis().SetLabelFont(42)
    histo.GetYaxis().SetLabelFont(42)
    histo.GetXaxis().SetTitleSize(0.055)
    histo.GetYaxis().SetTitleSize(0.055)
    histo.GetXaxis().SetTitleOffset(1.1)
    histo.GetYaxis().SetTitleOffset(1.3)
    histo.GetXaxis().SetLabelOffset(0.011)
    histo.GetYaxis().SetLabelOffset(0.011)
    histo.GetXaxis().SetLabelSize(0.045)
    histo.GetYaxis().SetLabelSize(0.045)
    histo.SetMarkerStyle(9)
    histo.SetMarkerColor(1)
    histo.SetFillColor(2)
    histo.SetFillStyle(1001)
    histo.SetTitle("")
    histo.GetYaxis().SetTitle("Count")
    histo.GetYaxis().CenterTitle()
    histo.GetXaxis().CenterTitle()
# ------------------------------

#print "\npython limitPlotter.py <path-to-dir-with-Combine-root-files>  <limitname>\n"

# Extract input arguments
# ------------------------------
inputfoldername=sys.argv[1]
limitname=sys.argv[2]


# Extract file names in target dir
# ------------------------------
filelist = [f for f in os.listdir(inputfoldername) if isfile(join(inputfoldername, f)) if ".root" in f]
#print filelist


# Signal cross sections
# ------------------------------
signalmasscrosssection = [ 
    (100. , 12.42138),
    (200. , 1.063032),
    (300. , 0.2377699),
    (400. , 0.0768125),
    (500. , 0.03020929),
    (600. , 0.01341772),
    (700. , 0.00646713)
    ]

theorycrosssection = [ 
    (100. , 12.42138),
    (200. , 1.063032),
    (300. , 0.2377699),
    (400. , 0.0768125),
    (500. , 0.03020929),
    (600. , 0.01341772),
    (700. , 0.00646713)
    ]
theorymass = [ 
    (100. , 100.),
    (200. , 200.),
    (300. , 300.),
    (400. , 400.),
    (500. , 500.),
    (600. , 600.),
    (700. , 700.)
    ]

# Extract r-values from Combine output root files
# ------------------------------
m2sigmaexparray=[]
m1sigmaexparray=[]
meanexparray=[]
p1sigmaexparray=[]
p2sigmaexparray=[]
obsarray=[]
massarray=[]
masserrorarray=[]
#print
for filename in filelist:
    #print  filename
    infile=TFile(inputfoldername+"/"+filename,"READ")
    infile.ls()
    #print
    tr=infile.Get("limit")
    tr.Show(0)
    for ie,event in enumerate(tr):
        #print event.quantileExpected
        #print round(event.quantileExpected,2)
        if ie==0 and round(event.quantileExpected,2) == 0.03 : m2sigmaexparray.append((event.mh,event.limit))
        if ie==1 and round(event.quantileExpected,2) == 0.16 : m1sigmaexparray.append((event.mh,event.limit))
        if ie==2 and round(event.quantileExpected,2) == 0.5  : meanexparray.append((event.mh,event.limit))
        if ie==3 and round(event.quantileExpected,2) == 0.84 : p1sigmaexparray.append((event.mh,event.limit))
        if ie==4 and round(event.quantileExpected,2) == 0.98 : p2sigmaexparray.append((event.mh,event.limit))
        if ie==5 and round(event.quantileExpected,2) == -1.0 : obsarray.append((event.mh,event.limit))
        if ie==5 and round(event.quantileExpected,2) == -1.0 : masserrorarray.append(0.0)
#print
#print


# Create r-value limit arrays
# ------------------------------
signalcrosssectionsorted = [ x[1] for x in sorted(signalmasscrosssection, key=lambda tup: tup[0]) ]
theorycrosssectionsorted = [ x[1] for x in sorted(theorycrosssection, key=lambda tup: tup[0]) ]
theorymasssorted      = [ x[1] for x in sorted(theorymass,      key=lambda tup: tup[0]) ]
obsarraysorted        = [ x[1] for x in sorted(obsarray,        key=lambda tup: tup[0]) ]
m2sigmaexparraysorted = [ x[1] for x in sorted(m2sigmaexparray, key=lambda tup: tup[0]) ]
m1sigmaexparraysorted = [ x[1] for x in sorted(m1sigmaexparray, key=lambda tup: tup[0]) ]
meanexparraysorted    = [ x[1] for x in sorted(meanexparray,    key=lambda tup: tup[0]) ]
massarraysorted       = [ x[0] for x in sorted(meanexparray,    key=lambda tup: tup[0]) ]
massarraysortedV2     = [ x[0] for x in sorted(signalmasscrosssection,    key=lambda tup: tup[0]) ]
p1sigmaexparraysorted = [ x[1] for x in sorted(p1sigmaexparray, key=lambda tup: tup[0]) ]
p2sigmaexparraysorted = [ x[1] for x in sorted(p2sigmaexparray, key=lambda tup: tup[0]) ]
#print "**Before signal cross-section normalization**"
#print "signalcrosssectionsorted : ",signalcrosssectionsorted
#print "          obsarraysorted : ",obsarraysorted
#print "   m2sigmaexparraysorted : ",m2sigmaexparraysorted
#print "   m1sigmaexparraysorted : ",m1sigmaexparraysorted
#print "      meanexparraysorted : ",meanexparraysorted
#print "   p1sigmaexparraysorted : ",p1sigmaexparraysorted
#print "   p2sigmaexparraysorted : ",p2sigmaexparraysorted
#print "         massarraysorted : ",massarraysorted
#print "Th. XSec massarraysorted : ",massarraysortedV2
#print


for imass in massarraysorted:
    if imass not in massarraysortedV2 : 
        #print "\nERROR: input limit mass "+str(imass)+" is not included in the signal cross-section array!"
        #print "available signal masses: ",massarraysortedV2
        #print "..exiting!\n"
        exit(1)



# Create cross-section normalized limit arrays
# ------------------------------
obsarraysorted        = [a*b for a,b in zip(signalcrosssectionsorted,obsarraysorted)]
m2sigmaexparraysorted = [a*b for a,b in zip(signalcrosssectionsorted,m2sigmaexparraysorted)]
m1sigmaexparraysorted = [a*b for a,b in zip(signalcrosssectionsorted,m1sigmaexparraysorted)]
meanexparraysorted    = [a*b for a,b in zip(signalcrosssectionsorted,meanexparraysorted)]
p1sigmaexparraysorted = [a*b for a,b in zip(signalcrosssectionsorted,p1sigmaexparraysorted)]
p2sigmaexparraysorted = [a*b for a,b in zip(signalcrosssectionsorted,p2sigmaexparraysorted)]
#print "**After signal cross-section normalization**"
#print "signalcrosssectionsorted : ",signalcrosssectionsorted
#print "          obsarraysorted : ",obsarraysorted
#print "   m2sigmaexparraysorted : ",m2sigmaexparraysorted
#print "   m1sigmaexparraysorted : ",m1sigmaexparraysorted
#print "      meanexparraysorted : ",meanexparraysorted
#print "   p1sigmaexparraysorted : ",p1sigmaexparraysorted
#print "   p2sigmaexparraysorted : ",p2sigmaexparraysorted
#print "         massarraysorted : ",massarraysorted
#print

# Create bands for +/- 1 and 2 sigma & theory xsec uncertainty
# ------------------------------
m2bandmeanexpsorted=[]
m1bandmeanexpsorted=[]
p1bandmeanexpsorted=[]
p2bandmeanexpsorted=[]
expLimit = 0
obsLimit = 0
for im,mass in enumerate(massarraysorted) :
    m2bandmeanexpsorted.append( meanexparraysorted[im]-m2sigmaexparraysorted[im])
    m1bandmeanexpsorted.append( meanexparraysorted[im]-m1sigmaexparraysorted[im])
    p1bandmeanexpsorted.append(-meanexparraysorted[im]+p1sigmaexparraysorted[im])
    p2bandmeanexpsorted.append(-meanexparraysorted[im]+p2sigmaexparraysorted[im])
    
    # Calculate limit (obs. & exp.) automatically
    # ------------------------------
    if im+1 < len(massarraysorted) :
        # log y = a*x+b --> because of using log scale !!!
        aExp = (TMath.Log(meanexparraysorted[im])-TMath.Log(meanexparraysorted[im+1]))/(massarraysorted[im]-massarraysorted[im+1])
        aObs = (TMath.Log(obsarraysorted[im])-TMath.Log(obsarraysorted[im+1]))/(massarraysorted[im]-massarraysorted[im+1])
        aTheo = (TMath.Log(signalcrosssectionsorted[im])-TMath.Log(signalcrosssectionsorted[im+1]))/(massarraysorted[im]-massarraysorted[im+1])
        bExp = TMath.Log(meanexparraysorted[im])-aExp*massarraysorted[im]
        bObs = TMath.Log(obsarraysorted[im])-aObs*massarraysorted[im]
        bTheo = TMath.Log(signalcrosssectionsorted[im])-aTheo*massarraysorted[im]
        
        canTF=TCanvas("canTF","canTF",0,0,700,700); canTF.cd()
        lineExp = TF1("lineExp","TMath::Exp([0]*x+[1])-TMath::Exp([2]*x+[3])",massarraysorted[im],massarraysorted[im+1])
        lineExp.SetParameter(0,aExp)
        lineExp.SetParameter(1,bExp)
        lineExp.SetParameter(2,aTheo)
        lineExp.SetParameter(3,bTheo)
        lineExp.Draw()
        lineExp.SetNpx(int(massarraysorted[len(massarraysorted)-1]-massarraysorted[0]))
        if lineExp.GetX(0,massarraysorted[0],massarraysorted[len(massarraysorted)-1]) <= massarraysorted[im+1] and lineExp.GetX(0,massarraysorted[0],massarraysorted[len(massarraysorted)-1]) >= massarraysorted[im] : expLimit = lineExp.GetX(0,massarraysorted[0],massarraysorted[len(massarraysorted)-1])
        
        lineObs = TF1("lineObs","TMath::Exp([0]*x+[1])-TMath::Exp([2]*x+[3])",massarraysorted[im],massarraysorted[im+1])
        lineObs.SetParameter(0,aObs)
        lineObs.SetParameter(1,bObs)
        lineObs.SetParameter(2,aTheo)
        lineObs.SetParameter(3,bTheo)
        lineObs.Draw()
        lineObs.SetNpx(int(massarraysorted[len(massarraysorted)-1]-massarraysorted[0]))
        if lineObs.GetX(0,massarraysorted[0],massarraysorted[len(massarraysorted)-1]) <= massarraysorted[im+1] and lineObs.GetX(0,massarraysorted[0],massarraysorted[len(massarraysorted)-1]) >= massarraysorted[im] : obsLimit = lineObs.GetX(0,massarraysorted[0],massarraysorted[len(massarraysorted)-1])
        canTF.Close()
        
        
    

# Create output canvas
# ------------------------------
can=TCanvas("can1","can1",0,0,700,700); can.cd()
gStyle.SetOptStat(0)
can.SetLeftMargin(0.15); can.SetRightMargin(0.10); can.SetTopMargin(0.07); can.SetBottomMargin(0.13)
can.SetTickx(1); can.SetTicky(1); can.SetLogy(0)


# Create limit curves
# ------------------------------
theorycurve             = TGraphErrors ( len(theorycrosssectionsorted), array('d',theorymasssorted), array('d',theorycrosssectionsorted) )
for im,mass in enumerate(theorymasssorted) :
    theorycurve.SetPointError(im,0,(0.01*(math.sqrt(0.169*theorymasssorted[im])+0.029))*theorycrosssectionsorted[im])
    
observedlimit           = TGraph( len(massarraysorted), array('d',massarraysorted), array('d',obsarraysorted) )
m2sigmaexpectedlimit    = TGraph( len(massarraysorted), array('d',massarraysorted), array('d',m2sigmaexparraysorted) )
m1sigmaexpectedlimit    = TGraph( len(massarraysorted), array('d',massarraysorted), array('d',m1sigmaexparraysorted) )
expectedlimit           = TGraph( len(massarraysorted), array('d',massarraysorted), array('d',meanexparraysorted)    )
expected1sigmalimitarea = TGraphAsymmErrors( len(massarraysorted), array('d',massarraysorted), array('d',meanexparraysorted), array('d',masserrorarray), array('d',masserrorarray), array('d',m1bandmeanexpsorted), array('d',p1bandmeanexpsorted)   )
expected2sigmalimitarea = TGraphAsymmErrors( len(massarraysorted), array('d',massarraysorted), array('d',meanexparraysorted), array('d',masserrorarray), array('d',masserrorarray), array('d',m2bandmeanexpsorted), array('d',p2bandmeanexpsorted)   )
p1sigmaexpectedlimit    = TGraph( len(massarraysorted), array('d',massarraysorted), array('d',p1sigmaexparraysorted) )
p2sigmaexpectedlimit    = TGraph( len(massarraysorted), array('d',massarraysorted), array('d',p2sigmaexparraysorted) )

# Create canvas
# ------------------------------
can.cd(); can.SetLogy(1)
can.SetLeftMargin(0.16)
can.SetRightMargin(0.08)
can.SetTopMargin(0.07)
can.SetBottomMargin(0.13)

# Create legends
# ------------------------------
legend2 = TLegend(0.52,0.84,0.83,0.9)
legend2.SetFillColor(0); legend2.SetLineColor(0); legend2.SetFillColor(0); legend2.SetFillStyle(0)
legend2.SetTextSize(0.035); legend2.SetTextFont(42); legend2.SetTextAlign(12)
legend2.AddEntry(theorycurve,"#sigma_{VLL} with unc.","FL")
legend = TLegend(0.52,0.57,0.83,0.82)
legend.SetFillColor(0); legend.SetLineColor(0); legend.SetFillColor(0); legend.SetFillStyle(0)
legend.SetTextSize(0.035); legend.SetTextFont(42); legend.SetTextAlign(12)
legend.SetHeader("95% CL upper limits")
legend.AddEntry(observedlimit,"Observed","PL")
legend.AddEntry(expectedlimit,"Expected","PL")
legend.AddEntry(expected1sigmalimitarea,"Expected #pm 1 s.d.","F")
legend.AddEntry(expected2sigmalimitarea,"Expected #pm 2 s.d.","F")


# Create labels
# ------------------------------
cmslabel      = TLatex(); cmslabel.SetTextFont(61);      cmslabel.SetTextAlign(11);      cmslabel.SetNDC();      cmslabel.SetTextSize(0.042)
cmsextralabel = TLatex(); cmsextralabel.SetTextFont(52); cmsextralabel.SetTextAlign(11); cmsextralabel.SetNDC(); cmsextralabel.SetTextSize(0.042)
lumilabel     = TLatex(); lumilabel.SetTextFont(42);     lumilabel.SetTextAlign(31);     lumilabel.SetNDC();     lumilabel.SetTextSize(0.042)


# Draw & save output 
# ------------------------------
hFrame = TH1D("","",1,80,720)
hFrame.Draw("HIST")
SetHistoStyle(hFrame)
hFrame.GetYaxis().SetRangeUser(0.002,30.)
hFrame.GetYaxis().SetTitle("#sigma (pb)")
hFrame.GetXaxis().SetTitle("VLL Mass (GeV)")
expectedlimit.SetLineStyle(3)
expectedlimit.SetLineWidth(2)
observedlimit.SetMarkerStyle(9)
observedlimit.SetLineStyle(7)
observedlimit.SetLineWidth(3)
expected1sigmalimitarea.SetFillColor(kGreen+1)
expected1sigmalimitarea.SetLineColor(kGreen+1)
expected1sigmalimitarea.SetFillStyle(1001)
expected2sigmalimitarea.SetFillColor(kOrange)
expected2sigmalimitarea.SetLineColor(kOrange)
expected2sigmalimitarea.SetFillStyle(1001)
theorycurve.SetFillColor(kRed-9)
theorycurve.SetFillStyle(1001)
theorycurve.SetLineColor(kRed+1)
theorycurve.SetLineWidth(2)
expected2sigmalimitarea.Draw("3same")
expected1sigmalimitarea.Draw("3same")
#m1sigmaexpectedlimit.Draw("Lsame")
#p1sigmaexpectedlimit.Draw("Lsame")
#m2sigmaexpectedlimit.Draw("Lsame")
#p2sigmaexpectedlimit.Draw("Lsame")
theorycurve.Draw("3same")
theorycurve.Draw("LXsame")
expectedlimit.Draw("PLsame")
observedlimit.Draw("PLsame")
legend2.Draw()
legend.Draw()
cmslabel.DrawLatex(0.16,0.942,"CMS")
cmsextralabel.DrawLatex(0.26,0.942,"Preliminary")
lumilabel.DrawLatex(0.92,0.942,"41.4 fb^{-1} (13 TeV)")
gPad.RedrawAxis()
can.SaveAs("limitPlotter_"+limitname+".pdf")

#print 
if expLimit <= massarraysorted[0] or expLimit >= massarraysorted[len(massarraysorted)-1] : print "No expected limit calculation possible!"
elif expLimit != 0 : 
    print "Expected Limit: %.0f GeV" % expLimit
if obsLimit <= massarraysorted[0] or obsLimit >= massarraysorted[len(massarraysorted)-1] : print "No observed limit calculation possible!"
elif obsLimit != 0 : 
    print "Observed Limit: %.0f GeV" % obsLimit
