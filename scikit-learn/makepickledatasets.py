import numpy as np
import pandas as pd
import sklearn
import ROOT as rt
import os
import freyalib
import pickle as pl

from root_numpy import root2array, tree2array
from root_numpy.testdata import get_filepath
from sklearn.metrics import accuracy_score


import matplotlib.pyplot as plt

from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.datasets import make_gaussian_quantiles
from sklearn.ensemble import RandomForestClassifier
#
#from sklearn.model_selection import train_test_split
#from sklearn.preprocessing import StandardScaler
#from sklearn.datasets import make_moons, make_circles, make_classification
#from sklearn.neural_network import MLPClassifier
#from sklearn.neighbors import KNeighborsClassifier
#from sklearn.svm import SVC
#from sklearn.gaussian_process import GaussianProcessClassifier
#from sklearn.gaussian_process.kernels import RBF
#from sklearn.tree import DecisionTreeClassifier
#from sklearn.ensemble import RandomForestClassifier, AdaBoostClassifier
#from sklearn.naive_bayes import GaussianNB
#from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis

from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, roc_auc_score
from sklearn import cross_validation

# useful info on how to get the data in the right format:
# https://betatim.github.io/posts/sklearn-for-TMVA-users/

##################################
# make a numpy array:

#filename = get_filepath()
#rfile = rt.TFile('./craneens_mu/Craneen_ttttNLO_Run2_TopTree_Study.root')
#tttttree = rfile.Get('Craneen__Mu')
##tttttree.Print()
#ttfile = rt.TFile('./*/Craneen_TTJets_powheg_Run2_TopTree_Study.root')
#tttree = ttfile.Get('Craneen__Mu')
##tttree.Print()

samplesize=1

njetscut=9
ntagscut=2
# Convert a TTree in a ROOT file into a NumPy structured array
## definitions here: https://github.com/dlont/FourTops2016/blob/master/craneens.desc
branchlist=[#'max(6,min(nJets,10))'
            #            'nJets',
            'multitopness', 'HTb','HTH','LeptonPt','NjetsW','SumJetMassX','HTX','csvJetcsv3','csvJetcsv4'#order of the paper
            ,'1stjetpt','2ndjetpt','5thjetpt','6thjetpt',
#            'LeptonPt','LeptonEta',
            #            'leptonphi',
#            'NjetsW','HT','HTb','HTH','HTRat','HTX','SumJetMassX','multitopness',
            'HT',
            'jet5and6pt'
            #            'csvJetcsv1','csvJetcsv2',
#             'csvJetcsv3','csvJetcsv4'
            #            'BDT'
            #            ,'csvJetpt1','csvJetpt2'
            ,'csvJetpt3','csvJetpt4'
            ]
scalefactorlist=[
            'ScaleFactor','SFPU_up/SFPU','SFPU_down/SFPU','GenWeight','TMath::Max(weight1/GenWeight,TMath::Max(weight2/GenWeight,TMath::Max(weight3/GenWeight,TMath::Max(weight4/GenWeight,TMath::Max(weight6/GenWeight, weight8/GenWeight)))))','TMath::Min(weight1/GenWeight,TMath::Min(weight2/GenWeight,TMath::Min(weight3/GenWeight,TMath::Min(weight4/GenWeight,TMath::Min(weight6/GenWeight, weight8/GenWeight)))))','csvrsw[2]','csvrsw[3]'
            ]
#for ii in range(njetscut) :
#    branchlist+=['jetvec['+str(ii)+'][0]','jetvec['+str(ii)+'][1]',
#                 #'jetvec['+str(ii)+'][2]', # phi, not relevant
#                 'jetvec['+str(ii)+'][3]'
#                 #                 ,'jetvec['+str(ii)+'][4]'
#                 ]

print branchlist

triggercut=['(HLT_IsoMu24==1||HLT_IsoTkMu24==1)','(HLT_Ele32_eta2p1_WPTight_Gsf==1)']
craneenname=['Craneen__Mu','Craneen__El']
directories=['./craneens_mu/','./craneens_el/']
samplelist=[['central','Craneen_ttttNLO_Run2_TopTree_Study.root','Craneen_TTJets_powheg_Run2_TopTree_Study.root'],['jesup','Craneen_TTJets_powheg_jesup_Run2_TopTree_Study.root','Craneen_ttttNLO_jesup_Run2_TopTree_Study.root'],['jesdown','Craneen_TTJets_powheg_jesdown_Run2_TopTree_Study.root','Craneen_ttttNLO_jesdown_Run2_TopTree_Study.root']]
njetslist = ['nJets>=','nJets>=','nJets==','nJets==','nJets>=']
njetscutlist = [6,7,8,9,10]
for jj2 in range(2) :
    for jj3 in range(3):
        for jj4 in range(5):
            njetscut = njetscutlist[jj4];
            njetsstr= njetslist[jj4]+str(njetscut)
            cutselection=triggercut[jj2]+' && '+njetsstr+' && nLtags >='+str(ntagscut)
            outname = craneenname[jj2]+"_"+samplelist[jj3][0]
            outname.replace("/","")
            outname.replace(".root","")
            print outname,cutselection
            X_sig = root2array(directories[jj2]+samplelist[jj3][1],craneenname[jj2],
                              branches=branchlist,selection=cutselection, step=samplesize
                              )
            X_bg = root2array(directories[jj2]+samplelist[jj3][2],craneenname[jj2],
                              branches=branchlist,selection=cutselection, step=samplesize
                              )
            W_sig = root2array(directories[jj2]+samplelist[jj3][1],craneenname[jj2],
                               branches=scalefactorlist,selection=cutselection, step=samplesize
                               )
            W_bg = root2array(directories[jj2]+samplelist[jj3][2],craneenname[jj2],
                              branches=scalefactorlist,selection=cutselection,step=samplesize
                              )

            for ii in range(len(X_bg)):
                if ii > 0 :
                    if len(X_bg[ii])!=len(X_bg[ii-1]):
                        print "found erratic events: "
                        print "bg ", ii, " : ",X_bg[ii]
                        print "bg ", ii-1, " : ",X_bg[ii-1]

            for jj in range(len(X_sig)):
                if jj > 0 :
                    if len(X_sig[jj])!=len(X_sig[jj-1]):
                        print "found erratic events: "
                        print "sig ", jj, ": ",X_sig[jj]
                        print "sig ", jj-1, ": ",X_sig[jj-1]

            # make plots of the samples
            print jj2, jj3, samplelist[jj3][0],samplelist[jj3][1]," size: ",len(X_sig),", ",samplelist[jj3][2]," size : ",len(X_bg)
            
            freyalib.PlotVariableDistributions(X_sig,X_bg,os.getcwd()+"/inputvariablessummary/"+outname+"/")
            freyalib.PlotVariableDistributions(W_sig,W_bg,os.getcwd()+"/inputvariablessummary/"+outname+"/")



            ###################################
            ## make the classifyer values:
            ttval=0
            ttttval=1
            ###################################
            ## some printout
            ###################################
            ## make the data arrays

            print "Making training sample"
            X_complete = np.concatenate((X_sig,X_bg))
            W_complete = np.concatenate((W_sig,W_bg))
            y_complete = [ttttval]*len(X_sig)+[ttval]*len(X_bg)

            # save the samples:
            pl.dump(X_complete, open(outname+"njets"+str(njetscut)+"trainingdata.p","wb"))
            pl.dump(y_complete, open(outname+"njets"+str(njetscut)+"trainingtruth.p","wb"))
            pl.dump(W_complete, open(outname+"njets"+str(njetscut)+"trainingweights.p","wb"))
