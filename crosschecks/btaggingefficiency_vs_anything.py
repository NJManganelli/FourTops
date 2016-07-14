import ROOT as rt
import CMS_lumi, tdrstyle
import array
import sys
import csv
import math

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


deltaR_b=rt.TH1D("deltaR_b","",60,0,6)
deltaR_frame=deltaR_b.Clone("deltaR_frame")
deltaR_c=deltaR_b.Clone("deltaR_c")
deltaR_l=deltaR_b.Clone("deltaR_l")
deltaR_b_tag=deltaR_b.Clone("deltaR_b_tag")
deltaR_c_tag=deltaR_b.Clone("deltaR_c_tag")
deltaR_l_tag=deltaR_b.Clone("deltaR_l_tag")
deltaR_l.SetLineColor(rt.kRed)
deltaR_l_tag.SetLineColor(rt.kRed)
deltaR_c.SetLineColor(rt.kBlue)
deltaR_c_tag.SetLineColor(rt.kBlue)

njets_b = rt.TH1D("njets_b","",6,6,12)
njets_frame = njets_b.Clone("njets_frame")
njets_c = njets_b.Clone("njets_c")
njets_l = njets_b.Clone("njets_l")
njets_b_tag = njets_b.Clone("njets_b_tag")
njets_c_tag = njets_b.Clone("njets_c_tag")
njets_l_tag = njets_b.Clone("njets_l_tag")
njets_l.SetLineColor(rt.kRed)
njets_l_tag.SetLineColor(rt.kRed)
njets_c.SetLineColor(rt.kBlue)
njets_c_tag.SetLineColor(rt.kBlue)





filenames=["/user/lbeck/CMSSW_7_6_3/src/V3/TopBrussels/FourTops/Craneens_Mu/Craneens14_7_2016/CraneenJets_TTJets_powheg_norm_*.root"]
           
           #"/user/lbeck/CMSSW_7_6_3/src/V3/TopBrussels/FourTops/Craneens_Mu/Craneens14_7_2016/CraneenJets_ttttNLO_Run2_TopTree_Study_*.root"]

lv2 = rt.TLorentzVector(0,0,0,0)
mediumcut=0.79
lvarray=[]
idarray=[]
tagarray=[]
for filename in filenames :
    ch = rt.TChain("Craneen__Mu")
    ch.Add(filename)
    #ch.Print()
    #event loop
    newevent=0
    previouspuweight=0.0
    previousleptonsf=0.0
    for ev in ch:
        if (previouspuweight!=ev.eventID) :
            previouspuweight=ev.eventID
            newevent=1
            #            print tagarray
            # worker part of loop:
            for ii in range(0,len(lvarray)) :
                if lvarray[ii].Pt() < 50 :
                    continue
                if lvarray[ii].Pt() > 60 :
                    continue
                if abs(lvarray[ii].Eta()) > 0.5 :
                    continue
                
                njets=len(lvarray)
                if idarray[ii]==5 :
                    njets_b.Fill(njets)
                    if tagarray[ii] == 1 :
                        njets_b_tag.Fill(njets)
                elif idarray[ii]==4 :
                    njets_c.Fill(njets)
                    if tagarray[ii] == 1 :
                        njets_c_tag.Fill(njets)
                else :
                    njets_l.Fill(njets)
                    if tagarray[ii] == 1 :
                        njets_l_tag.Fill(njets)

                for jj in range(0,len(lvarray)) :
                    if ii== jj :
                        continue
                    
                    #                    print ii,jj,lvarray[ii].DeltaR(lvarray[jj]),lvarray[ii].Phi(),lvarray[jj].Phi(),lvarray[ii].Eta(),lvarray[jj].Eta(),tagarray[ii],tagarray[jj],idarray[ii],idarray[jj]
                    workval=lvarray[ii].DeltaR(lvarray[jj])
                    if idarray[ii]==5 :
                        deltaR_b.Fill(workval)
                        if tagarray[ii] == 1 :
                            deltaR_b_tag.Fill(workval)
                    elif idarray[ii]==4 :
                        deltaR_c.Fill(workval)
                        if tagarray[ii] == 1 :
                            deltaR_c_tag.Fill(workval)
                    else :
                        deltaR_l.Fill(workval)
                        if tagarray[ii] == 1 :
                            deltaR_l_tag.Fill(workval)
            # delete objects
            del lvarray[:]
            del tagarray[:]
            del idarray[:]
    
        lv1 = rt.TLorentzVector()
        lv1.SetPtEtaPhiE(ev.jetpT,ev.jeteta,ev.jetphi,ev.jetE)
        lvarray.append(lv1)
        tag=0
        if ev.csvDisc > mediumcut :
            tag=1
        tagarray.append(tag)
        idarray.append(abs(ev.jetFlav))



#        print ev.jetpT


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

#deltaR_b.Draw()
#deltaR_b_tag.Draw("same")
deltaR_b_eff = rt.TEfficiency(deltaR_b_tag,deltaR_b)
deltaR_b_eff.SetLineColor(deltaR_b.GetLineColor())
deltaR_b_eff.SetMarkerColor(deltaR_b.GetLineColor())
deltaR_frame.SetMinimum(0.0)
deltaR_frame.SetMaximum(1.05)
deltaR_frame.SetXTitle("DeltaR(tag jet,other jet)")
deltaR_frame.SetYTitle("efficiency")
deltaR_frame.Draw("axis")
deltaR_b_eff.Draw("same")

deltaR_c_eff = rt.TEfficiency(deltaR_c_tag,deltaR_c)
deltaR_c_eff.SetLineColor(deltaR_c.GetLineColor())
deltaR_c_eff.SetMarkerColor(deltaR_c.GetLineColor())
deltaR_c_eff.Draw("same")
deltaR_l_eff = rt.TEfficiency(deltaR_l_tag,deltaR_l)
deltaR_l_eff.SetLineColor(deltaR_l.GetLineColor())
deltaR_l_eff.SetMarkerColor(deltaR_l.GetLineColor())
deltaR_l_eff.Draw("same")

leg = rt.TLegend(0.2,0.8,0.9,0.95)
leg.SetBorderSize(0)
leg.SetHeader("#mu + 6 jets")
leg.AddEntry(deltaR_b_eff,"b quark jets","lp")
leg.AddEntry(deltaR_c_eff,"c quark jets","lp")
leg.AddEntry(deltaR_l_eff,"l quark jets","lp")
leg.Draw("same")

canvas.Modified()

canvas2 = rt.TCanvas("c2","c2",50,50,W,H)
canvas2.SetFillColor(0)
canvas2.SetBorderMode(0)
canvas2.SetFrameFillStyle(0)
canvas2.SetFrameBorderMode(0)
canvas2.SetLeftMargin( L/W )
canvas2.SetRightMargin( R/W )
canvas2.SetTopMargin( T/H )
canvas2.SetBottomMargin( B/H )
canvas2.SetTickx(0)
canvas2.SetTicky(0)

njets_b_eff = rt.TEfficiency(njets_b_tag,njets_b)
njets_b_eff.SetLineColor(njets_b.GetLineColor())
njets_b_eff.SetMarkerColor(njets_b.GetLineColor())
njets_frame.SetMinimum(0.0)
njets_frame.SetMaximum(1.05)
njets_frame.SetXTitle("jet multiplicity")
njets_frame.SetYTitle("efficiency")
njets_frame.Draw("axis")
njets_b_eff.Draw("same")

njets_c_eff = rt.TEfficiency(njets_c_tag,njets_c)
njets_c_eff.SetLineColor(njets_c.GetLineColor())
njets_c_eff.SetMarkerColor(njets_c.GetLineColor())
njets_c_eff.Draw("same")
njets_l_eff = rt.TEfficiency(njets_l_tag,njets_l)
njets_l_eff.SetLineColor(njets_l.GetLineColor())
njets_l_eff.SetMarkerColor(njets_l.GetLineColor())
njets_l_eff.Draw("same")

leg2 = rt.TLegend(0.2,0.8,0.9,0.95)
leg2.SetBorderSize(0)
leg2.SetHeader("#mu + 6 jets")
leg2.AddEntry(deltaR_b_eff,"b quark jets","lp")
leg2.AddEntry(deltaR_c_eff,"c quark jets","lp")
leg2.AddEntry(deltaR_l_eff,"l quark jets","lp")
leg2.Draw("same")

canvas2.Modified()


raw_input("Press Enter to Continue: ")
