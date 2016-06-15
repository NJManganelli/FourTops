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
names={"di-lepton","single-lepton","combined"}

####
#### THE ARRAYS BELOW IS WHERE YOU ENTER THE ONE- AND TWO- SIGMA LIMITS
####
####
twosigdown=[4.4318,5.3275,9.7247]
onesigdown=[6.1509,7.3513,13.8889]
limit=[8.9688,10.7812,21.1875]
onesigup=[13.3660,16.0671,33.2645]
twosigup=[19.2872,23.3155,50.5505]
observed=[8.9688+0.5,10.7812+0.5,21.1875+0.5]



# dedicated histogram to contain the background
backgrounddummyhist = rt.TH2F("backgrounddummyhist","",2,0,70,4,-0.1,4)
backgrounddummyhist.SetXTitle("95 % CL limit on #mu = #sigma_{obs} / #sigma_{SM}")
backgrounddummyhist.GetYaxis().SetBinLabel(1,"")
backgrounddummyhist.GetYaxis().SetBinLabel(2,"")
backgrounddummyhist.GetYaxis().SetBinLabel(3,"")
backgrounddummyhist.GetYaxis().SetBinLabel(4,"")
backgrounddummyhist.GetYaxis().SetNdivisions(2)

SMband = rt.TGraphAsymmErrors(2)
SMband.SetLineColor(rt.kRed)
SMband.SetLineWidth(0)
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


oneSigma = rt.TGraphAsymmErrors(3)
oneSigma.SetFillColor(rt.kGreen)
oneSigma.SetLineColor(rt.kGreen)
oneSigma.SetFillStyle(1001)
twoSigma = rt.TGraphAsymmErrors(3)
twoSigma.SetFillColor(rt.kYellow)
twoSigma.SetLineColor(rt.kYellow)
twoSigma.SetFillStyle(1001)

for ii in range(len(limit)) :
#    print ii," ",limit[ii]
    centralvals.SetPoint(ii,observed[ii],values[ii])
    centralvals.SetPointError(ii,0.0,0.0,0.5,0.5)
    expectedcentralvals.SetPoint(ii,limit[ii],values[ii])
    expectedcentralvals.SetPointError(ii,0.0,0.0,0.5,0.5)
    oneSigma.SetPoint(ii,limit[ii],values[ii])
    oneSigma.SetPointError(ii,limit[ii]-onesigdown[ii],onesigup[ii]-limit[ii],0.5,0.5)
    twoSigma.SetPoint(ii,limit[ii],values[ii])
    twoSigma.SetPointError(ii,limit[ii]-twosigdown[ii],twosigup[ii]-limit[ii],0.5,0.5)


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
centralvals.Draw("p")
expectedcentralvals.Draw("p")
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

