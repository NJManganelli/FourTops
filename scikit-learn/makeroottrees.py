import numpy as np
import pandas as pd
import sklearn
import ROOT as rt
import os
import freyalib
import pickle as pl

from root_numpy import root2array, tree2array, array2root
from root_numpy.testdata import get_filepath
from sklearn.metrics import accuracy_score


import matplotlib.pyplot as plt

from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.datasets import make_gaussian_quantiles
from sklearn.ensemble import RandomForestClassifier


# useful info on how to get the data in the right format:
# https://betatim.github.io/posts/sklearn-for-TMVA-users/

##################################
# make a numpy array:

# Convert a TTree in a ROOT file into a NumPy structured array
## definitions here: https://github.com/dlont/FourTops2016/blob/master/craneens.desc


# ALL CONFIGURABLES ARE HERE:
branchlist=['nJets','multitopness', 'HTb','HTH','LeptonPt','NjetsW','SumJetMassX','HTX','csvJetcsv3','csvJetcsv4'
            ]
treename="Craneen__El" # or: "Craneen__Mu"
infilename='./craneens_el/Craneen_ttttNLO_Run2_TopTree_Study.root'
outfilename='./craneens_el/Craneen_ttttNLO_Run2_TopTree_Study_newMVA.root'
cutselection='' # cuts on craneen level can be added here in root TCut format, be careful as then the tree cannot easily be friended...
inputpicklefile="BDTAdaBoost_100_3.p"
###################
# end of all configurables


# do the work: load into python array
X_in = root2array(infilename,treename,
                      branches=branchlist,selection=cutselection
                      )
# load the mva
estimator = pl.load(open(inputpicklefile,"rb"))

# calculate the MVA values for the python array
MVA_out = estimator.decision_function(pd.DataFrame(X_in)) # obviously needs to have the same members in branchlist variable as what was trained with
# print it to screen:
print MVA_out
# set the types so ROOT can save things correctly
MVA_out.dtype = [('MVAoutput', np.float64)]
# write out a root file
array2root(MVA_out, outfilename, "MVAoutput")
