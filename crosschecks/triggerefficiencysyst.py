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


# load information from Higgs text file:

#arrays with Higgs efficiency granularity
etaneg = []
etalow = []
etahigh = []
ptlow = []
pthigh = []
eff = []
deff = []

#arrays with absolute eta (so half) efficiency granularity
higeff = []
higdeff = []
myeff = []
mydeff = []
sf=[]
dsf=[]
namelist=[]
ptlist=[]
etalist=[]

with open('HLT_Ele23_WPLoose_76X.txt') as f:
    for line in f:
        data = line.split()
        #        print data
        

        # now: do operation to merge the appropriate bins:
        loweta=abs(float(data[0]))
        higheta=abs(float(data[1]))
        theswapper=0
        if loweta > higheta :
            #            print loweta, higheta
            loweta,higheta = higheta,loweta
            theswapper=1

        # only create new one if not existing yet:
        etaneg.append(theswapper)
        ptlow.append(float(data[2]))
        pthigh.append(float(data[3]))
        eff.append(float(data[4]))
        deff.append(float(data[5]))
        etalow.append(loweta)
        etahigh.append(higheta)

filenames=["/user/lbeck/CMSSW_7_6_3/src/V3/TopBrussels/FourTops/Craneens_El/Craneens12_7_2016/merge/*.root"]


for filename in filenames :
    ch = rt.TChain("Craneen__El")
    ch.Add(filename)
    #    ch.Print()
    leaflist = ch.GetListOfLeaves()
    #    print leaflist
    for ileaf in leaflist :
#        print ileaf.GetName()
        goodleaf = ileaf.GetName()
        if goodleaf.find("fTrigeta") >=0  :
            # should return something like:
            print "\n\n next point: looping over all events"
            countertrig=0
            counterall=0
            realeta=0.0
            realpt=0.0
            #            print goodleaf.find('etap8P')
            if goodleaf.find('etap8P') >=0  :
                realeta=0.8
            elif goodleaf.find('eta1p4')  >=0 :
                realeta=1.4
            elif goodleaf.find('eta1p6') >=0  :
                realeta=1.6
            elif goodleaf.find('eta2P') >=0  :
                realeta=2.0
            elif goodleaf.find('eta2p5') >=0  :
                realeta=2.5
            
            if goodleaf.find('Pt35') >=0 :
                realpt=35.
            elif goodleaf.find('Pt40') >=0 :
                realpt=40.
            elif goodleaf.find('Pt50') >=0 :
                realpt=50.
            elif goodleaf.find('Pt60')  >=0 :
                realpt=60.
            elif goodleaf.find('Pt100')  >=0  :
                realpt=100.
        
        
            
            #print goodleaf,realeta,realpt

            
            #            print goodleaf
            histoname="workhisto"+goodleaf
            histo=rt.TH1D(histoname,"",3,0,3)
            
            # here is the loop over all events in the chain...
            for trigval in ch:
                workval= trigval.GetLeaf(goodleaf).GetValue()
                if workval > 0.5 :
                    histo.Fill(2-workval)

            print goodleaf, " ran over ",histo.GetEntries()," good entries "
          
            workereff=0.
            workerdeff=0.
            workerdeffsq=0.
        # now do the comparison and match to
        
            myeff.append(histo.GetMean())
            mydeff.append(histo.GetMeanError())
            eff2=0.0
            eff3=0.0
            deff2=0.0
            deff3=0.0
            for ibin in range(0,len(eff)):
                #print etahigh[ibin],ptlow[ibin]
                if etahigh[ibin]!= realeta :
                    continue
                if pthigh[ibin]!=realpt :
                    continue
                print "matchig to Higgs:",etahigh[ibin],pthigh[ibin],realeta,realpt,eff[ibin],deff[ibin]

                if etaneg[ibin] == 0 :
                    eff3=eff[ibin]
                    deff3=deff[ibin]
                else:
                    eff2=eff[ibin]
                    deff2=deff[ibin]
        
        # calculate weighted averages
            value = ((eff2*deff2)+(eff3*deff3))/(deff2+deff3)
            higeff.append(value)
            dvalue = math.sqrt(((deff2*(value-eff2)**2)+(deff3*(value-eff3)**2))/(deff2+deff3))
            higdeff.append(dvalue)
            sf.append(value/histo.GetMean())
            dsfvalue=value/histo.GetMean()*math.sqrt((histo.GetMeanError()/histo.GetMean())**2 + (dvalue/value)**2)
            dsf.append(dsfvalue)
            
            namelist.append(goodleaf)
            ptlist.append(realpt)
            etalist.append(realeta)
            print goodleaf,realpt,realeta,value,dvalue,histo.GetMean(),histo.GetMeanError(),value/histo.GetMean(),dsfvalue




print myeff,mydeff,higeff,higdeff

higgshisto = rt.TH1D("higgshisto","",len(myeff),0,len(myeff))
higgshisto.Sumw2()
myhisto = higgshisto.Clone("myhisto")
ratiohisto= higgshisto.Clone("ratiohisto")
ratiohisto.SetLineColor(rt.kBlack)
ratiohisto.Divide(myhisto)

myhisto.SetLineColor(rt.kRed)
myhisto.SetMarkerColor(rt.kRed)
myhisto.SetMarkerStyle(21)

higgshisto.SetLineColor(rt.kBlue)
higgshisto.SetMarkerColor(rt.kBlue)
higgshisto.SetMarkerStyle(22)

for ibin in range(0,len(myeff)) :
    #    print ibin
    
    myhisto.SetBinContent(ibin+1,myeff[ibin])
    myhisto.SetBinError(ibin+1,mydeff[ibin])

    higgshisto.SetBinContent(ibin+1,higeff[ibin])
    higgshisto.SetBinError(ibin+1,higdeff[ibin])

    ratiohisto.SetBinContent(ibin+1,sf[ibin])
    ratiohisto.SetBinError(ibin+1,dsf[ibin])

higgshisto.Sumw2()
myhisto.Sumw2()

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

ratiohisto.Draw()
ratiohisto.SetXTitle("arbitrary p_{T},#eta bins")
ratiohisto.SetYTitle("SF or efficiency")
ratiohisto.SetMinimum(0)
ratiohisto.Fit("pol0")
higgshisto.Draw("same")
myhisto.Draw("same")

leg = rt.TLegend(0.2,0.2,0.5,0.5)
leg.SetFillStyle(0)
leg.SetBorderSize(0)
leg.AddEntry(ratiohisto,"ratio Higgs/our ttbar","lp")
leg.AddEntry(higgshisto,"efficiency Higgs","lp")
leg.AddEntry(myhisto,"efficiency our ttbar","lp")
leg.Draw("same")

canvas2 = canvas.Clone()
canvas2.cd()
gaus = rt.TH1D("gaus","",50,0.8,1.2)
for ibin in range(0,len(myeff)):
    print etalist[ibin],ptlist[ibin],sf[ibin],dsf[ibin]
    gaus.Fill(sf[ibin])

gaus.Fit("gaus")

canvas.Modified()

raw_input("Press Enter to Continue: ")
