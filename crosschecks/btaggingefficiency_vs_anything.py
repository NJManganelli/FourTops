import ROOT as rt
import CMS_lumi, tdrstyle
import array
import sys
import csv
from math import *
import parser

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


# CSV file:

with open('CSVv2.csv', 'r') as f:
    reader = csv.reader(f)
    my_list = list(reader)


workerbtagsfarray=[]

for line in my_list :
    if line[1].find('mujets') == 1 and line[0]=='1' :
        # split in central-up-down
        #            print line
        #            print line[2],line[3],line[4],line[5],line[6],line[7],line[10]
            scaleval=0
            if line[2].find('up') ==1 :
                scaleval=1
            elif line[2].find('down') ==1 :
                scaleval=-1
            bidval=0
            if line[3]==' 0' :
                bidval=5
            elif line[3]==' 1' :
                bidval=0
    #            print bidval,scaleval
            processingarray=[]
            processingarray.append(bidval)
            processingarray.append(scaleval)
            processingarray.append(float(line[3]))
            processingarray.append(float(line[4]))
            processingarray.append(float(line[5]))
            processingarray.append(float(line[6]))
            processingarray.append(line[10].replace("\"",""))
            workerbtagsfarray.append(processingarray)

#print workerbtagsfarray


histosfworkb = rt.TH2D("histosfworkb","",5,0,2.5,20,0,400)
histosfworkl = histosfworkb.Clone("histosfworkl")
histosfworklup = histosfworkb.Clone("histosfworklup")
histosfworkldown = histosfworkb.Clone("histosfworkldown")
histosfworkbup = histosfworkb.Clone("histosfworkbup")
histosfworkbdown = histosfworkb.Clone("histosfworkbdown")


for func in workerbtagsfarray :
    
    #    x=(func[4]+func[5])*0.5
    
    #print "y="+func[6]
    
    #    sf=eval(func[6])
    #print sf,x,func[6]
    
    for ibin in range(1,histosfworkb.GetNbinsX()+1) :
        for jbin in range(1,histosfworkb.GetNbinsY()+1) :
            etaval=histosfworkb.GetXaxis().GetBinCenter(ibin)
            ptval=histosfworkb.GetYaxis().GetBinCenter(jbin)
            if ptval<func[4] and ptval>func[5]:
                continue
            if etaval<func[2] and etaval>func[3]:
                continue
            x=ptval
            sfval = eval(func[6])
            print x,sfval,etaval,func[3],func[4],func[5],func[6]
            if func[0]==5 and func[1]==0:
                histosfworkb.SetBinContent(ibin,jbin,sfval)
            if func[0]==5 and func[1]==-1:
                histosfworkbdown.SetBinContent(ibin,jbin,sfval)
            if func[0]==5 and func[1]==1:
                histosfworkbup.SetBinContent(ibin,jbin,sfval)
            if func[0]!=5 and func[1]==0:
                histosfworkl.SetBinContent(ibin,jbin,sfval)
            if func[0]!=5 and func[1]==-1:
                histosfworkldown.SetBinContent(ibin,jbin,sfval)
            if func[0]==5 and func[1]==1:
                histosfworklup.SetBinContent(ibin,jbin,sfval)


file = rt.TFile("allhistograms_bookkeeping.root","recreate")
file.cd()


filenames=[["/user/lbeck/CMSSW_7_6_3/src/V3/TopBrussels/FourTops/Craneens_Mu/Craneens14_7_2016/CraneenJets_TTJets_powheg_norm_*.root","ttbar"],["/user/lbeck/CMSSW_7_6_3/src/V3/TopBrussels/FourTops/Craneens_Mu/Craneens14_7_2016/CraneenJets_ttttNLO_Run2_TopTree_Study_*.root","tttt"]]

lv2 = rt.TLorentzVector(0,0,0,0)
mediumcut=0.800
lvarray=[]
idarray=[]
tagarray=[]
faketagarray=[]
faketagarrayup=[]
faketagarraydown=[]

ran = rt.TRandom3()

for filename in filenames :
    
    
    deltaR_b=rt.TH1D("deltaR_b_"+filename[1],"",60,0,6)
    deltaR_frame=deltaR_b.Clone("deltaR_frame"+filename[1])
    deltaR_c=deltaR_b.Clone("deltaR_c_"+filename[1])
    deltaR_l=deltaR_b.Clone("deltaR_l_"+filename[1])
    deltaR_b_tag=deltaR_b.Clone("deltaR_b_tag_"+filename[1])
    deltaR_c_tag=deltaR_b.Clone("deltaR_c_tag_"+filename[1])
    deltaR_l_tag=deltaR_b.Clone("deltaR_l_tag_"+filename[1])
    deltaR_b_faketag=deltaR_b.Clone("deltaR_b_faketag_"+filename[1])
    deltaR_c_faketag=deltaR_b.Clone("deltaR_c_faketag_"+filename[1])
    deltaR_l_faketag=deltaR_b.Clone("deltaR_l_faketag_"+filename[1])
    deltaR_b_faketagup=deltaR_b.Clone("deltaR_b_faketagup_"+filename[1])
    deltaR_c_faketagup=deltaR_b.Clone("deltaR_c_faketagup_"+filename[1])
    deltaR_l_faketagup=deltaR_b.Clone("deltaR_l_faketagup_"+filename[1])
    deltaR_b_faketagdown=deltaR_b.Clone("deltaR_b_faketagdown_"+filename[1])
    deltaR_c_faketagdown=deltaR_b.Clone("deltaR_c_faketagdown_"+filename[1])
    deltaR_l_faketagdown=deltaR_b.Clone("deltaR_l_faketagdown_"+filename[1])
    deltaR_l.SetLineColor(rt.kRed)
    deltaR_l_tag.SetLineColor(rt.kRed)
    deltaR_l_faketag.SetLineColor(rt.kRed)
    deltaR_l_faketagup.SetLineColor(rt.kRed)
    deltaR_l_faketagdown.SetLineColor(rt.kRed)
    deltaR_c.SetLineColor(rt.kBlue)
    deltaR_c_tag.SetLineColor(rt.kBlue)
    deltaR_c_faketag.SetLineColor(rt.kBlue)
    deltaR_c_faketagdown.SetLineColor(rt.kBlue)
    deltaR_c_faketagup.SetLineColor(rt.kBlue)

    njets_b = rt.TH1D("njets_b_"+filename[1],"",6,6,12)
    njets_frame = njets_b.Clone("njets_frame"+filename[1])
    njets_c = njets_b.Clone("njets_c_"+filename[1])
    njets_l = njets_b.Clone("njets_l_"+filename[1])
    njets_b_tag = njets_b.Clone("njets_b_tag_"+filename[1])
    njets_c_tag = njets_b.Clone("njets_c_tag_"+filename[1])
    njets_l_tag = njets_b.Clone("njets_l_tag_"+filename[1])
    njets_b_faketag = njets_b.Clone("njets_b_faketag_"+filename[1])
    njets_c_faketag = njets_b.Clone("njets_c_faketag_"+filename[1])
    njets_l_faketag = njets_b.Clone("njets_l_faketag_"+filename[1])
    njets_b_faketagup = njets_b.Clone("njets_b_faketagup_"+filename[1])
    njets_c_faketagup = njets_b.Clone("njets_c_faketagup_"+filename[1])
    njets_l_faketagup = njets_b.Clone("njets_l_faketagup_"+filename[1])
    njets_b_faketagdown = njets_b.Clone("njets_b_faketagdown_"+filename[1])
    njets_c_faketagdown = njets_b.Clone("njets_c_faketagdown_"+filename[1])
    njets_l_faketagdown = njets_b.Clone("njets_l_faketagdown_"+filename[1])
    njets_l.SetLineColor(rt.kRed)
    njets_l_tag.SetLineColor(rt.kRed)
    njets_l_faketag.SetLineColor(rt.kRed)
    njets_l_faketagup.SetLineColor(rt.kRed)
    njets_l_faketagdown.SetLineColor(rt.kRed)
    njets_c.SetLineColor(rt.kBlue)
    njets_c_tag.SetLineColor(rt.kBlue)
    njets_c_faketag.SetLineColor(rt.kBlue)
    njets_c_faketagup.SetLineColor(rt.kBlue)
    njets_c_faketagdown.SetLineColor(rt.kBlue)

    ch = rt.TChain("Craneen__Mu")
    print filename[0] , filename[1]
    ch.Add(filename[0])
    #ch.Print()
    #event loop
    newevent=0
    previouspuweight=0.0
    previousleptonsf=0.0
    
    histoeffworkb = rt.TH2D("histoeffworkb"+filename[1],"",5,0,2.5,20,0,400)
    histoeffworkb.Sumw2()
    histoeffworkb_tag = histoeffworkb.Clone("histoeffworkb_tag"+filename[1])
    histoeffworkc_tag = histoeffworkb.Clone("histoeffworkc_tag"+filename[1])
    histoeffworkl_tag = histoeffworkb.Clone("histoeffworkl_tag"+filename[1])
    histoeffworkc = histoeffworkb.Clone("histoeffworkc"+filename[1])
    histoeffworkl = histoeffworkb.Clone("histoeffworkl"+filename[1])
    
    ch.Draw("jetpT:abs(jeteta) >> histoeffworkb"+filename[1],"abs(jetFlav)==5","goff")
    ch.Draw("jetpT:abs(jeteta) >> histoeffworkb_tag"+filename[1],"abs(jetFlav)==5 && csvDisc>"+str(mediumcut),"goff")
    ch.Draw("jetpT:abs(jeteta) >> histoeffworkc"+filename[1],"abs(jetFlav)==4","goff")
    ch.Draw("jetpT:abs(jeteta) >> histoeffworkc_tag"+filename[1],"abs(jetFlav)==4 && csvDisc>"+str(mediumcut),"goff")
    ch.Draw("jetpT:abs(jeteta) >> histoeffworkl"+filename[1],"abs(jetFlav)!=5 && abs(jetFlav)!=4","goff")
    ch.Draw("jetpT:abs(jeteta) >> histoeffworkl_tag"+filename[1],"abs(jetFlav)!=5 && abs(jetFlav)!=4 && csvDisc>"+str(mediumcut),"goff")
    
    histoeffworkb_tag.Divide(histoeffworkb)
    histoeffworkl_tag.Divide(histoeffworkl)
    histoeffworkc_tag.Divide(histoeffworkc)
    
#    for ibin in range(0,histoeffworkc.GetNbinsX()) :
#        for jbin in range(0,histoeffworkc.GetNbinsY()) :
#            print ibin,jbin,histoeffworkb_tag.GetBinContent(ibin+1,jbin+1),histoeffworkc_tag.GetBinContent(ibin+1,jbin+1),histoeffworkl_tag.GetBinContent(ibin+1,jbin+1)

    for ev in ch:
        if (previouspuweight!=ev.eventID) :
            previouspuweight=ev.eventID
            newevent=1
            #            print tagarray
            # worker part of loop:
            ntags=0
            for ii in range(0,len(lvarray)) :
                if tagarray[ii] == 1:
                    ntags+=1
#                
#                if lvarray[ii].Pt() < 40 :
#                    continue
#                if lvarray[ii].Pt() > 60 :
#                    continue
#                if abs(lvarray[ii].Eta()) > 0.5 :
#                    continue

                njets=len(lvarray)
                #                print idarray[ii],tagarray[ii],faketagarray[ii]
                
                if idarray[ii]==5 :
                    njets_b.Fill(njets)
                    if tagarray[ii] == 1 :
                        njets_b_tag.Fill(njets)
                    if faketagarray[ii] == 1 :
                        njets_b_faketag.Fill(njets)
                        njets_b_faketagdown.Fill(njets,faketagarraydown[ii])
                        njets_b_faketagup.Fill(njets,faketagarrayup[ii])
                elif idarray[ii]==4 :
                    njets_c.Fill(njets)
                    if tagarray[ii] == 1 :
                        njets_c_tag.Fill(njets)
                    if faketagarray[ii] == 1 :
                        njets_c_faketag.Fill(njets)
                        njets_c_faketagdown.Fill(njets,faketagarraydown[ii])
                        njets_c_faketagup.Fill(njets,faketagarrayup[ii])
                else :
                    njets_l.Fill(njets)
                    if tagarray[ii] == 1 :
                        njets_l_tag.Fill(njets)
                    if faketagarray[ii] == 1 :
                        njets_l_faketag.Fill(njets)
                        njets_l_faketagdown.Fill(njets,faketagarraydown[ii])
                        njets_l_faketagup.Fill(njets,faketagarrayup[ii])


                for jj in range(0,len(lvarray)) :
                    if ii== jj :
                        continue
                    
                    #                    print ii,jj,lvarray[ii].DeltaR(lvarray[jj]),lvarray[ii].Phi(),lvarray[jj].Phi(),lvarray[ii].Eta(),lvarray[jj].Eta(),tagarray[ii],tagarray[jj],idarray[ii],idarray[jj]
                    workval=lvarray[ii].DeltaR(lvarray[jj])
                    if workval < 0.4:
                        continue
                    
                    if idarray[ii]==5 :
                        deltaR_b.Fill(workval)
                        if tagarray[ii] == 1 :
                            deltaR_b_tag.Fill(workval)
                        if faketagarray[ii] == 1 :
                            deltaR_b_faketag.Fill(workval)
                            deltaR_b_faketagdown.Fill(workval,faketagarraydown[ii])
                            deltaR_b_faketagup.Fill(workval,faketagarrayup[ii])
                    elif idarray[ii]==4 :
                        deltaR_c.Fill(workval)
                        if tagarray[ii] == 1 :
                            deltaR_c_tag.Fill(workval)
                        if faketagarray[ii] == 1 :
                            deltaR_c_faketag.Fill(workval)
                            deltaR_c_faketagdown.Fill(workval,faketagarraydown[ii])
                            deltaR_c_faketagup.Fill(workval,faketagarrayup[ii])
                    else :
                        deltaR_l.Fill(workval)
                        if tagarray[ii] == 1 :
                            deltaR_l_tag.Fill(workval)
                        if faketagarray[ii] == 1 :
                            deltaR_l_faketag.Fill(workval)
                            deltaR_l_faketagdown.Fill(workval,faketagarraydown[ii])
                            deltaR_l_faketagup.Fill(workval,faketagarrayup[ii])
        
            # delete objects
            del lvarray[:]
            del tagarray[:]
            del idarray[:]
            del faketagarray[:]
            del faketagarrayup[:]
            del faketagarraydown[:]

        lv1 = rt.TLorentzVector()
        lv1.SetPtEtaPhiE(ev.jetpT,ev.jeteta,ev.jetphi,ev.jetE)
        lvarray.append(lv1)
        tag=0
        if ev.csvDisc > mediumcut :
            tag=1
        
        workefficiency=0
        faketag=0
        ranval = ran.Uniform(0.,1.)
        jetbinx=histoeffworkb.GetXaxis().FindBin(abs(ev.jeteta))
        jetbiny=histoeffworkb.GetYaxis().FindBin(ev.jetpT)
        tagprob=histoeffworkl_tag.GetBinContent(jetbinx,jetbiny)
        faketagup = 1.
        faketagdown= 1.
        if histosfworkl.GetBinContent(jetbinx,jetbiny) > 0 :
            faketagup+=(histosfworklup.GetBinContent(jetbinx,jetbiny)-histosfworkl.GetBinContent(jetbinx,jetbiny))/histosfworkl.GetBinContent(jetbinx,jetbiny)
            faketagdown+=(histosfworkldown.GetBinContent(jetbinx,jetbiny)-histosfworkl.GetBinContent(jetbinx,jetbiny))/histosfworkl.GetBinContent(jetbinx,jetbiny)
        
        
        if abs(ev.jetFlav) == 5 :
            tagprob= histoeffworkb_tag.GetBinContent(jetbinx,jetbiny)
            faketagup = 1.
            faketagdown= 1.
            if histosfworkb.GetBinContent(jetbinx,jetbiny) > 0 :
                faketagup+=(histosfworkbup.GetBinContent(jetbinx,jetbiny)-histosfworkb.GetBinContent(jetbinx,jetbiny))/histosfworkb.GetBinContent(jetbinx,jetbiny)
                faketagdown+=(histosfworkbdown.GetBinContent(jetbinx,jetbiny)-histosfworkb.GetBinContent(jetbinx,jetbiny))/histosfworkb.GetBinContent(jetbinx,jetbiny)
        elif abs(ev.jetFlav) == 4 :
            tagprob = histoeffworkc_tag.GetBinContent(jetbinx,jetbiny)
            faketagup = 1.
            faketagdown= 1.
            if histosfworkb.GetBinContent(jetbinx,jetbiny) > 0 :
                faketagup+=2*(histosfworkbup.GetBinContent(jetbinx,jetbiny)-histosfworkb.GetBinContent(jetbinx,jetbiny))/histosfworkb.GetBinContent(jetbinx,jetbiny)
                faketagdown+=2*(histosfworkbdown.GetBinContent(jetbinx,jetbiny)-histosfworkb.GetBinContent(jetbinx,jetbiny))/histosfworkb.GetBinContent(jetbinx,jetbiny)
        #        print faketagdown,faketagup
        
        if tagprob > ranval :
            faketag=1

        if tagprob < 0.0001 :
            print tagprob,ranval,faketag,ev.jeteta,ev.jetpT
#        print ranval,histoeffworkb_tag.GetBinContent(jetbinx,jetbiny),histoeffworkc_tag.GetBinContent(jetbinx,jetbiny),histoeffworkl_tag.GetBinContent(jetbinx,jetbiny),ev.jetFlav,faketag

        faketagarray.append(faketag)
        faketagarrayup.append(faketagup)
        faketagarraydown.append(faketagdown)

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


    deltaR_b_eff = deltaR_b_tag.Clone("deltaR_b_eff_"+filename[1])
    deltaR_b_eff.Divide(deltaR_b_tag,deltaR_b,1,1,"b")
    deltaR_b_eff.SetLineColor(deltaR_b.GetLineColor())
    deltaR_b_eff.SetMarkerColor(deltaR_b.GetLineColor())
    deltaR_b_fakeeff = deltaR_b_faketag.Clone("deltaR_b_fakeeff_"+filename[1])
    deltaR_b_fakeeff.Divide(deltaR_b_faketag,deltaR_b,1,1,"b")
    deltaR_b_fakeeff.SetLineColor(deltaR_b.GetLineColor())
    deltaR_b_fakeeff.SetLineStyle(2)
    
    deltaR_b_fakeeffup = deltaR_b_faketagup.Clone("deltaR_b_fakeeffup_"+filename[1])
    deltaR_b_fakeeffup.Divide(deltaR_b_faketagup,deltaR_b,1,1,"b")
    deltaR_b_fakeeffup.SetLineColor(deltaR_b.GetLineColor())
    deltaR_b_fakeeffup.SetLineStyle(3)
    deltaR_b_fakeeffdown = deltaR_b_faketagdown.Clone("deltaR_b_fakeeffdown_"+filename[1])
    deltaR_b_fakeeffdown.Divide(deltaR_b_faketagdown,deltaR_b,1,1,"b")
    deltaR_b_fakeeffdown.SetLineColor(deltaR_b.GetLineColor())
    deltaR_b_fakeeffdown.SetLineStyle(3)

    deltaR_frame.SetMinimum(0.0)
    deltaR_frame.SetMaximum(1.05)
    deltaR_frame.SetXTitle("DeltaR(tag jet,other jet)")
    deltaR_frame.SetYTitle("efficiency")
    deltaR_frame.Draw("axis")
    deltaR_b_eff.Draw("same")
    deltaR_b_fakeeff.Draw("histsame")
    deltaR_b_fakeeffdown.Draw("histsame")
    deltaR_b_fakeeffup.Draw("histsame")


    deltaR_c_eff = deltaR_c_tag.Clone("deltaR_c_eff_"+filename[1])
    deltaR_c_eff.Divide(deltaR_c_tag,deltaR_c,1,1,"b")
    deltaR_c_eff.SetLineColor(deltaR_c.GetLineColor())
    deltaR_c_eff.SetMarkerColor(deltaR_c.GetLineColor())
    deltaR_c_fakeeff = deltaR_c_faketag.Clone("deltaR_c_fakeeff_"+filename[1])
    deltaR_c_fakeeff.Divide(deltaR_c_faketag,deltaR_c,1,1,"b")
    deltaR_c_fakeeff.SetLineColor(deltaR_c.GetLineColor())
    deltaR_c_fakeeff.SetLineStyle(2)
    deltaR_c_fakeeffup = deltaR_c_faketagup.Clone("deltaR_c_fakeeffup_"+filename[1])
    deltaR_c_fakeeffup.Divide(deltaR_c_faketagup,deltaR_c,1,1,"b")
    deltaR_c_fakeeffup.SetLineColor(deltaR_c.GetLineColor())
    deltaR_c_fakeeffup.SetLineStyle(3)
    deltaR_c_fakeeffdown = deltaR_c_faketagdown.Clone("deltaR_c_fakeeffdown_"+filename[1])
    deltaR_c_fakeeffdown.Divide(deltaR_c_faketagdown,deltaR_c,1,1,"b")
    deltaR_c_fakeeffdown.SetLineColor(deltaR_c.GetLineColor())
    deltaR_c_fakeeffdown.SetLineStyle(3)

    deltaR_c_eff.Draw("same")
    deltaR_c_fakeeff.Draw("histsame")
    deltaR_c_fakeeffdown.Draw("histsame")
    deltaR_c_fakeeffup.Draw("histsame")

    deltaR_l_eff = deltaR_l_tag.Clone("deltaR_l_eff_"+filename[1])
    deltaR_l_eff.Divide(deltaR_l_tag,deltaR_l,1,1,"b")
    deltaR_l_eff.SetLineColor(deltaR_l.GetLineColor())
    deltaR_l_eff.SetMarkerColor(deltaR_l.GetLineColor())
    deltaR_l_fakeeff = deltaR_l_faketag.Clone("deltaR_l_fakeeff_"+filename[1])
    deltaR_l_fakeeff.Divide(deltaR_l_faketag,deltaR_l,1,1,"b")
    deltaR_l_fakeeff.SetLineColor(deltaR_l.GetLineColor())
    deltaR_l_fakeeff.SetLineStyle(2)
    
    deltaR_l_fakeeffup = deltaR_l_faketagup.Clone("deltaR_l_fakeeffup_"+filename[1])
    deltaR_l_fakeeffup.Divide(deltaR_l_faketagup,deltaR_l,1,1,"b")
    deltaR_l_fakeeffup.SetLineColor(deltaR_l.GetLineColor())
    deltaR_l_fakeeffup.SetLineStyle(3)
    deltaR_l_fakeeffdown = deltaR_l_faketagdown.Clone("deltaR_l_fakeeffdown_"+filename[1])
    deltaR_l_fakeeffdown.Divide(deltaR_l_faketagdown,deltaR_l,1,1,"b")
    deltaR_l_fakeeffdown.SetLineColor(deltaR_l.GetLineColor())
    deltaR_l_fakeeffdown.SetLineStyle(3)


    deltaR_l_eff.Draw("same")
    deltaR_l_fakeeff.Draw("histsame")
    deltaR_l_fakeeffdown.Draw("histsame")
    deltaR_l_fakeeffup.Draw("histsame")



    leg = rt.TLegend(0.2,0.4,0.6,0.6)
    leg.SetBorderSize(0)
    leg.SetHeader("#mu + 6 jets, "+filename[1])
    leg.AddEntry(deltaR_b_eff,"b quark jets","lp")
    leg.AddEntry(deltaR_c_eff,"c quark jets","lp")
    leg.AddEntry(deltaR_l_eff,"l quark jets","lp")
    leg.AddEntry(deltaR_l_fakeeff,"expected + uncertainty","l")
    leg.Draw("same")

    canvas.Modified()
    canvas.Print("jet_dR_"+filename[1]+".root")
    canvas.Print("jet_dR_"+filename[1]+".pdf")

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


    njets_b_eff = njets_b_tag.Clone("njets_b_eff_"+filename[1])
    njets_b_eff.Divide(njets_b_tag,njets_b,1,1,"b")
    njets_b_eff.SetLineColor(njets_b.GetLineColor())
    njets_b_eff.SetMarkerColor(njets_b.GetLineColor())
    njets_b_fakeeff = njets_b_faketag.Clone("njets_b_fakeeff_"+filename[1])
    njets_b_fakeeff.Divide(njets_b_faketag,njets_b,1,1,"b")
    njets_b_fakeeff.SetLineColor(njets_b.GetLineColor())
    njets_b_fakeeff.SetLineStyle(2)
    
    njets_b_fakeeffup = njets_b_faketagup.Clone("njets_b_fakeeffup_"+filename[1])
    njets_b_fakeeffup.Divide(njets_b_faketagup,njets_b,1,1,"b")
    njets_b_fakeeffup.SetLineColor(njets_b.GetLineColor())
    njets_b_fakeeffup.SetLineStyle(3)
    njets_b_fakeeffdown = njets_b_faketagdown.Clone("njets_b_fakeeffdown_"+filename[1])
    njets_b_fakeeffdown.Divide(njets_b_faketagdown,njets_b,1,1,"b")
    njets_b_fakeeffdown.SetLineColor(njets_b.GetLineColor())
    njets_b_fakeeffdown.SetLineStyle(3)
    
    njets_frame.SetMinimum(0.0)
    njets_frame.SetMaximum(1.05)
    njets_frame.SetXTitle("DeltaR(tag jet,other jet)")
    njets_frame.SetYTitle("efficiency")
    njets_frame.Draw("axis")
    njets_b_eff.Draw("same")
    njets_b_fakeeff.Draw("histsame")
    njets_b_fakeeffdown.Draw("histsame")
    njets_b_fakeeffup.Draw("histsame")
    
    
    njets_c_eff = njets_c_tag.Clone("njets_c_eff_"+filename[1])
    njets_c_eff.Divide(njets_c_tag,njets_c,1,1,"b")
    njets_c_eff.SetLineColor(njets_c.GetLineColor())
    njets_c_eff.SetMarkerColor(njets_c.GetLineColor())
    njets_c_fakeeff = njets_c_faketag.Clone("njets_c_fakeeff_"+filename[1])
    njets_c_fakeeff.Divide(njets_c_faketag,njets_c,1,1,"b")
    njets_c_fakeeff.SetLineColor(njets_c.GetLineColor())
    njets_c_fakeeff.SetLineStyle(2)
    njets_c_fakeeffup = njets_c_faketagup.Clone("njets_c_fakeeffup_"+filename[1])
    njets_c_fakeeffup.Divide(njets_c_faketagup,njets_c,1,1,"b")
    njets_c_fakeeffup.SetLineColor(njets_c.GetLineColor())
    njets_c_fakeeffup.SetLineStyle(3)
    njets_c_fakeeffdown = njets_c_faketagdown.Clone("njets_c_fakeeffdown_"+filename[1])
    njets_c_fakeeffdown.Divide(njets_c_faketagdown,njets_c,1,1,"b")
    njets_c_fakeeffdown.SetLineColor(njets_c.GetLineColor())
    njets_c_fakeeffdown.SetLineStyle(3)
    
    njets_c_eff.Draw("same")
    njets_c_fakeeff.Draw("histsame")
    njets_c_fakeeffdown.Draw("histsame")
    njets_c_fakeeffup.Draw("histsame")
    
    njets_l_eff = njets_l_tag.Clone("njets_l_eff_"+filename[1])
    njets_l_eff.Divide(njets_l_tag,njets_l,1,1,"b")
    njets_l_eff.SetLineColor(njets_l.GetLineColor())
    njets_l_eff.SetMarkerColor(njets_l.GetLineColor())
    njets_l_fakeeff = njets_l_faketag.Clone("njets_l_fakeeff_"+filename[1])
    njets_l_fakeeff.Divide(njets_l_faketag,njets_l,1,1,"b")
    njets_l_fakeeff.SetLineColor(njets_l.GetLineColor())
    njets_l_fakeeff.SetLineStyle(2)
    
    njets_l_fakeeffup = njets_l_faketagup.Clone("njets_l_fakeeffup_"+filename[1])
    njets_l_fakeeffup.Divide(njets_l_faketagup,njets_l,1,1,"b")
    njets_l_fakeeffup.SetLineColor(njets_l.GetLineColor())
    njets_l_fakeeffup.SetLineStyle(3)
    njets_l_fakeeffdown = njets_l_faketagdown.Clone("njets_l_fakeeffdown_"+filename[1])
    njets_l_fakeeffdown.Divide(njets_l_faketagdown,njets_l,1,1,"b")
    njets_l_fakeeffdown.SetLineColor(njets_l.GetLineColor())
    njets_l_fakeeffdown.SetLineStyle(3)
    
    
    njets_l_eff.Draw("same")
    njets_l_fakeeff.Draw("histsame")
    njets_l_fakeeffdown.Draw("histsame")
    njets_l_fakeeffup.Draw("histsame")



    leg2 = rt.TLegend(0.2,0.4,0.6,0.6)
    leg2.SetBorderSize(0)
    leg2.SetHeader("#mu + 6 jets, "+filename[1])
    leg2.AddEntry(njets_b_eff,"b quark jets","lp")
    leg2.AddEntry(njets_c_eff,"c quark jets","lp")
    leg2.AddEntry(njets_l_eff,"l quark jets","lp")
    leg2.AddEntry(njets_l_fakeeff,"expected + uncertainty","l")
    leg2.Draw("same")

    canvas2.Modified()
    canvas2.Print("jet_multiplicity_"+filename[1]+".root")
    canvas2.Print("jet_multiplicity_"+filename[1]+".pdf")


file.Write()
raw_input("Press Enter to Continue: ")
