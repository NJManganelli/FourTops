# limit overview plot as asked by the conveners.
#
# in py root, so execute by 
# >> python limit_overview_plot.py
#
# note: the observed and expected limit values are manually added to the tables annotated in the first half of the code
#
# the macro requires the official pyRoot root style macros provided by CMS and documented here:
# https://ghm.web.cern.ch/ghm/plots/

import ROOT as rt
import CMS_lumi, tdrstyle
import array
import sys
import csv

#set the tdr style
tdrstyle.setTDRStyle()

#change the CMS_lumi variables (see CMS_lumi.py)
CMS_lumi.lumi_7TeV = "2.6-2600 fb^{-1}"
CMS_lumi.lumi_8TeV = "2.6-2600 fb^{-1}"
CMS_lumi.lumi_13TeV = "2.6 fb^{-1}"
CMS_lumi.writeExtraText = 1
CMS_lumi.extraText = "Preliminary"
CMS_lumi.lumi_sqrtS = "13 TeV" # used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)


iPeriod = 4

iPos = 11
if( iPos==0 ): CMS_lumi.relPosX = 0.12

H_ref = 600;
W_ref = 800;
W = W_ref
H  = H_ref

# references for T, B, L, R
T = 0.08*H_ref
B = 0.12*H_ref
L = 0.12*W_ref
R = 0.04*W_ref

canvas = rt.TCanvas("c2","c2",50,50,W,H)
canvas.SetFillColor(0)
canvas.SetBorderMode(0)
canvas.SetFrameFillStyle(0)
canvas.SetFrameBorderMode(0)
canvas.SetLeftMargin( L/W )
canvas.SetRightMargin( R/W )
canvas.SetTopMargin( T/H )
canvas.SetBottomMargin( B/H )
canvas.SetTickx(0)
canvas.SetTicky(0)

values=[0.5,1.5,2.5]
names={"dilepton","singlelepton","combined"}

####
#### THE ARRAYS BELOW IS WHERE YOU ENTER THE ONE- AND TWO- SIGMA LIMITS
####
####
twosigdown=[4.9983,5.8729,9.4567]
onesigdown=[7.0762,8.2953,13.9261]
limit=[10.8438,12.6875,22.3125]
onesigup=[17.5002,20.4757,38.1436]
twosigup=[28.2595,32.7815,63.9038]
observed=[10.2256,16.0633,14.8942]



# dedicated histogram to contain the background
backgrounddummyhist = rt.TH2F("backgrounddummyhist","",2,0,90,4,-0.1,4.5)
backgrounddummyhist.SetXTitle("95 % CL limit on #mu = #sigma_{obs} / #sigma_{SM}")
backgrounddummyhist.GetXaxis().SetTitleSize(0.9*backgrounddummyhist.GetXaxis().GetTitleSize())
backgrounddummyhist.GetXaxis().SetTitleOffset(backgrounddummyhist.GetXaxis().GetTitleOffset()/0.9)
backgrounddummyhist.GetYaxis().SetBinLabel(1,"")
backgrounddummyhist.GetYaxis().SetBinLabel(2,"")
backgrounddummyhist.GetYaxis().SetBinLabel(3,"")
backgrounddummyhist.GetYaxis().SetBinLabel(4,"")
backgrounddummyhist.GetYaxis().SetNdivisions(1)
backgrounddummyhist.GetYaxis().SetTickLength(0.0)

SMband = rt.TGraphAsymmErrors(2)
SMband.SetLineColor(rt.kRed)
SMband.SetLineWidth(2)
SMband.SetFillColor(rt.kRed)
SMband.SetFillStyle(500)
SMband.SetPoint(0,1.,values[0]-0.5)
SMband.SetPoint(1,1.,values[2]+0.5)
SMband.SetPointError(0,0.028/9.201,0.028/9.201,0.0,0.0)
SMband.SetPointError(1,0.028/9.201,0.028/9.201,0.0,0.0)
centralvals = rt.TGraphAsymmErrors(3)

expectedcentralvals = rt.TGraphAsymmErrors(3)
expectedcentralvals.SetLineStyle(2)
expectedcentralvals.SetLineColor(rt.kGray+1)
expectedcentralvals.SetMarkerSize(0)
expectedcentralvals.SetMarkerStyle(0)

sigmatttt=1.0 # set to 1 for signal strength plot

oneSigma = rt.TGraphAsymmErrors(3)
oneSigma.SetFillColor(rt.kGray+1)
oneSigma.SetLineColor(rt.kGray+1)
oneSigma.SetFillStyle(1001)
twoSigma = rt.TGraphAsymmErrors(3)
twoSigma.SetFillColor(rt.kGray)
twoSigma.SetLineColor(rt.kGray)
twoSigma.SetFillStyle(1001)

for ii in range(len(limit)) :
#    print ii," ",limit[ii]
    centralvals.SetPoint(ii,sigmatttt*observed[ii],values[ii])
    centralvals.SetPointError(ii,0.0,0.0,0.5,0.5)
    expectedcentralvals.SetPoint(ii,sigmatttt*limit[ii],values[ii])
    expectedcentralvals.SetPointError(ii,0.0,0.0,0.5,0.5)
    oneSigma.SetPoint(ii,sigmatttt*limit[ii],values[ii])
    oneSigma.SetPointError(ii,sigmatttt*(limit[ii]-onesigdown[ii]),sigmatttt*(onesigup[ii]-limit[ii]),0.5,0.5)
    twoSigma.SetPoint(ii,sigmatttt*limit[ii],values[ii])
    twoSigma.SetPointError(ii,sigmatttt*(limit[ii]-twosigdown[ii]),sigmatttt*(twosigup[ii]-limit[ii]),0.5,0.5)


x1_l = 0.92
y1_l = 0.90

dx_l = 0.30
dy_l = 0.18
x0_l = x1_l-dx_l
y0_l = y1_l-dy_l

backgrounddummyhist.Draw()
twoSigma.Draw("2same")
oneSigma.Draw("2same")
SMband.Draw("lsame")
centralvals.SetLineWidth(2*centralvals.GetLineWidth())
centralvals.SetMarkerSize(1.2*centralvals.GetMarkerSize())
centralvals.Draw("p")
expectedcentralvals.Draw("p")

leg = rt.TLegend(0.6,0.7,0.9,0.9,"limits on #sigma_{tttt}")
leg.SetFillStyle(0)
leg.SetBorderSize(0)
leg.AddEntry(centralvals,"observed","lp")
leg.AddEntry(oneSigma,"expected #pm 1 #sigma","f")
leg.AddEntry(twoSigma,"expected #pm 2 #sigma","f")

leg.Draw("same")
latex = rt.TLatex()
latex.SetTextSize(0.9*latex.GetTextSize())
latex.SetTextAlign(12)
latex.DrawLatex(1.05*twosigup[2],values[0],"combined")
latex.DrawLatex(1.05*twosigup[2],values[1],"single lepton")
latex.DrawLatex(1.05*twosigup[2],values[2],"dilepton")
latex.SetTextColor(rt.kRed)
latex.SetTextAlign(21)
latex.SetTextSize(0.5*latex.GetTextSize())
latex.DrawLatex(1.02,3.05,"SM")
canvas.Update()

CMS_lumi.CMS_lumi(canvas, iPeriod, iPos)

canvas.cd()
canvas.Update()
canvas.RedrawAxis()
frame = canvas.GetFrame()
frame.Draw()

canvas.Print("limit_overview_plot.pdf")
canvas.Print("limit_overview_plot.root")


raw_input("Press Enter to Continue: ")

