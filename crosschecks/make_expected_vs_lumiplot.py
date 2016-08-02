import ROOT as rt
import CMS_lumi, tdrstyle
import array
import sys
import numpy as n

#set the tdr style
tdrstyle.setTDRStyle()

#change the CMS_lumi variables (see CMS_lumi.py)
CMS_lumi.lumi_7TeV = "2.6-2600 fb^{-1}"
CMS_lumi.lumi_8TeV = "2.6-2600 fb^{-1}"
CMS_lumi.lumi_13TeV = "1-1000 fb^{-1}"
CMS_lumi.writeExtraText = 1
CMS_lumi.extraText = "Simulation Preliminary"
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
canvas.SetLogx(1)
canvas.SetGridy()

#graph_a = rt.TGraphErrors('graph_data.dat')
#graph_a = rt.TGraphErrors('graph_data_dileptons.dat')
graph_a = rt.TGraphErrors('graph_data_combo.dat')
#graph_a.SetFillColor(rt.kYellow)
#graph_a.SetFillColor(rt.kGreen)
graph_a.SetFillColor(rt.kGray)
graph_a.SetFillStyle(1001)
graph_a.SetLineColor(rt.kGray)
graph_b=graph_a.Clone()
graph_b.SetLineColor(rt.kBlack)
graph_b.SetLineWidth(2)

graph_a.Draw('A3')
graph_b.Draw('cx')

theline2 = rt.TLine(2.6,0,2.6,0.8*graph_a.GetHistogram().GetMaximum())
theline2.SetLineStyle(2)
theline2.Draw("same")
theline3 = rt.TLine(30,0,30,0.8*graph_a.GetHistogram().GetMaximum())
theline3.SetLineStyle(3)
#theline3.Draw("same")

graph_a.Draw('c')
theline = rt.TF1("theline","1",0,1000)
theline.Draw("same")
theline2.Draw("same")
#theline3.Draw("same")
graph_a.GetXaxis().SetTitle("(expected) integrated luminosity (fb^{-1})")
graph_a.GetXaxis().SetTitleSize(graph_a.GetXaxis().GetTitleSize()*0.85)
graph_a.GetYaxis().SetTitle("95 % CL limit on #sigma_{t#bar{t}t#bar{t}} (#sigma_{obs} / #sigma_{SM})")
graph_a.GetYaxis().SetTitleSize(graph_a.GetYaxis().GetTitleSize()*0.95)
graph_a.GetYaxis().SetTitleOffset(0.9)
graph_a.GetXaxis().SetTitleOffset(1.1)



x1_l = 0.92
y1_l = 0.90

dx_l = 0.40
dy_l = 0.25
x0_l = x1_l-dx_l
y0_l = y1_l-dy_l


legend =  rt.TLegend(x0_l,y0_l,x1_l, y1_l )
legend.SetFillColor(rt.kWhite)
legend.SetBorderSize(0)
#legend.AddEntry(graph_b,"2015 data","p")
#legend.AddEntry(graph_a,"lepton+jets","lf")
#legend.AddEntry(graph_a,"dilepton","lf")
legend.AddEntry(graph_a,"combined limit (expected)","lf")
legend.AddEntry(theline,"SM t#bar{t}t#bar{t}","l")
legend.AddEntry(theline2,"2015 integrated lumi","l")
#legend.AddEntry(theline3,"2016 integrated lumi (expected)","l")

legend.Draw()


canvas.Update()

CMS_lumi.CMS_lumi(canvas, iPeriod, iPos)

canvas.cd()
canvas.Update()
canvas.RedrawAxis()
frame = canvas.GetFrame()
frame.Draw()

#canvas.Print("dilepton_limitvslumi.pdf")
#canvas.Print("leptonJets_limitvslumi.pdf")
canvas.Print("combined_limitvslumi.pdf")



raw_input("Press Enter to Continue: ")

