import numpy as np
import pandas as pd
import sklearn
import ROOT as rt
import os
import argparse
import pickle as pl

seed=4
np.random.seed(seed)

from root_numpy import root2array, tree2array
from root_numpy.testdata import get_filepath
from sklearn.metrics import accuracy_score

from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline

parser = argparse.ArgumentParser(description='Arguments for this BDT trainer, which uses adaBoost from scikit-learn.')
parser.add_argument('-s','--seed', help='random generator seed',required=False)

namejets ="njets6"
parser.add_argument('-nj','--njets',help='number of jets, used to pick the input files', required=True)
parser.add_argument('-e','--epochs',help='number of epochs', required=True)
parser.add_argument('-d','--depth',help='tree depth', required=False)

# parsing the arguments
args = parser.parse_args()
namejets = "njets"+str(args.njets)
if args.seed > 0:
    seed = int(args.seed)
n_iter=100
if args.epochs > 0:
    n_iter = int(args.epochs)
n_max_depth=3
if args.depth > 0:
    n_max_depth = int(args.depth)

import matplotlib.pyplot as plt

from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.datasets import make_gaussian_quantiles
from sklearn.ensemble import RandomForestClassifier

from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, roc_auc_score

from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import RandomizedSearchCV
from sklearn.metrics import roc_curve, auc

# useful info on how to get the data in the right format:
# https://betatim.github.io/posts/sklearn-for-TMVA-users/

##################################
# open the samples:


X_completeA = pl.load(open("Craneen__El_central"+namejets+"trainingdata.p","rb"))
y_completeA = pl.load(open("Craneen__El_central"+namejets+"trainingtruth.p","rb"))
X_completeB = pl.load(open("Craneen__Mu_central"+namejets+"trainingdata.p","rb"))
y_completeB = pl.load(open("Craneen__Mu_central"+namejets+"trainingtruth.p","rb"))

X_complete=np.concatenate((X_completeA,X_completeB))
y_complete=np.concatenate((y_completeA,y_completeB))
#print X_complete

X_train, X_test, y_train, y_test = train_test_split(pd.DataFrame(X_complete), y_complete, test_size=0.3) #use 32% for testing and the rest for training

#print len(X_train), len(X_test),len(y_train),len(y_test)

#scaler = StandardScaler()
#X_train = scaler.fit_transform(X_train)
#X_test = scaler.transform(X_test)

###################################

print "starting the training with ",n_iter," iterations and ",len(y_train)," events for training, ",len(y_test)," for testing"



estimator = AdaBoostClassifier(DecisionTreeClassifier(max_depth=n_max_depth,presort=True, random_state=True),
                         algorithm="SAMME",
                         n_estimators=n_iter)

estimator=estimator.fit(X_train,y_train)

print estimator.get_params()
#print bdt.ranking_
#print bdt.support_
#print bdt.feature_importances_
train_predictions = estimator.staged_predict(X_train)
test_predictions = estimator.staged_predict(X_test)
all_predictions = estimator.staged_predict(X_test)
scores_test = []
scores_train = []
for i in test_predictions:
    scores_test.append(accuracy_score(i,y_test))
for i in train_predictions:
    scores_train.append(accuracy_score(i,y_train))
plt.figure()
plt.title("learning curves "+namejets)
plt.xlabel("Training iterations")
plt.ylabel("accuracy")
plt.grid()
plt.plot(range(0,n_iter,1), scores_train, 'o-', color="r",label="Training accuracy")
plt.plot(range(0,n_iter,1), scores_test, 'o-', color="g",label="Testing accuracy")
plt.legend(loc="best")
#plt.show()
plt.savefig('training_curve_'+namejets+'.png')
plt.savefig('training_curve_'+namejets+'.pdf')
print "created training_curve plot"
y_predicted = estimator.predict(X_test)
print classification_report(y_test, y_predicted,
                            target_names=["background", "signal"])
print "Area under ROC curve: %.4f"%(roc_auc_score(y_test,
                                                  estimator.decision_function(X_test)))
y_trained = estimator.predict(X_train)
decisions = estimator.decision_function(X_test)
decisionstrain = estimator.decision_function(X_train)
print decisions

plt.figure()
plot_colors = "br"
plot_step = 0.02
class_names = "AB"
twoclass_output = estimator.decision_function(X_test)
twoclass_output_train = estimator.decision_function(X_train)
plot_range = (twoclass_output_train.min(), twoclass_output_train.max())
#for i, n, c in zip(range(2), class_names, plot_colors):
#    print i, n, c
sigdist = []
bgdist = []
sigdisttrain = []
bgdisttrain = []
for ii in range(len(twoclass_output)) :
    #    print ii,twoclass_output[ii],y_test[ii]
    if y_test[ii]> 0:
        sigdist.append(twoclass_output[ii])
    else:
        bgdist.append(twoclass_output[ii])

for ii in  range(len(twoclass_output_train)) :
    #    print ii,twoclass_output_train[ii],y_train[ii]
    if y_train[ii]> 0:
        sigdisttrain.append(twoclass_output_train[ii])
    else:
        bgdisttrain.append(twoclass_output_train[ii])


plt.hist(sigdisttrain,
        bins=50,
        range=plot_range,
        facecolor='b',normed=True,
         alpha=.2)
plt.hist(bgdisttrain,
        bins=50,
        range=plot_range,
        facecolor='r',normed=True,
        alpha=.2)
plt.hist(sigdist,
         bins=50,
         range=plot_range,
         facecolor='b',normed=True,
         alpha=.2)
plt.hist(bgdist,
         bins=50,
         range=plot_range,
         facecolor='r',normed=True,
         alpha=.2)
x1, x2, y1, y2 = plt.axis()
plt.axis((x1, x2, y1, y2 * 1.2))
plt.legend(loc='best')
plt.ylabel('Arbitrary Units')
plt.xlabel('Score')
plt.title('Decision Scores '+namejets)
#plt.show()
plt.savefig('distribution'+namejets+'.png')
plt.savefig('distribution'+namejets+'.pdf')
print "created BDT dist plot"


# Compute ROC curve and area under the curve
fpr, tpr, thresholds = roc_curve(y_test, decisions)
fprT, tprT, thresholdsT = roc_curve(y_train, decisionstrain)
roc_auc = auc(fpr, tpr)
roc_aucT = auc(fprT, tprT)
plt.figure()
plt.plot(fpr, tpr, lw=1, label='test ROC (area = %0.2f)'%(roc_auc))
plt.plot(fprT, tprT, lw=2, label='train ROC (area = %0.2f)'%(roc_aucT))
plt.plot([0, 1], [0, 1], '--', color=(0.6, 0.6, 0.6), label='Luck')
plt.xlim([-0.05, 1.05])
plt.ylim([-0.05, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic '+namejets)
plt.legend(loc="best")
plt.grid()
#plt.show()
plt.savefig('roc_curve'+namejets+'.png')
plt.savefig('roc_curve'+namejets+'.pdf')

print "created roc curve plot"
#freyalib.PlotMVADistributions(estimator,X_train,y_train,X_test,y_test,"testdir","testmva")

pl.dump(estimator, open("BDTAdaBoost_"+namejets+"_"+str(n_iter)+"_"+str(n_max_depth)+".p","wb"))

def correlations(data,extrastring, **kwds):
    """Calculate pairwise correlation between features.
        
        Extra arguments are passed on to DataFrame.corr()
        """
    # simply call df.corr() to get a table of
    # correlation values if you do not need
    # the fancy plotting
    corrmat = data.corr(**kwds)

    plt.figure()
    fig, ax1 = plt.subplots(ncols=1, figsize=(6,5))
    
    opts = {'cmap': plt.get_cmap("RdBu"),
        'vmin': -1, 'vmax': +1}
    heatmap1 = ax1.pcolor(corrmat, **opts)
    plt.colorbar(heatmap1, ax=ax1)

    ax1.set_title("Correlations for "+extrastring+" "+namejets)
    
    labels = corrmat.columns.values
    for ax in (ax1,):
        # shift location of ticks to center of the bins
        ax.set_xticks(np.arange(len(labels))+0.5, minor=False)
        ax.set_yticks(np.arange(len(labels))+0.5, minor=False)
        ax.set_xticklabels(labels, minor=False, ha='right', rotation=70)
        ax.set_yticklabels(labels, minor=False)
    
    plt.tight_layout()
#    plt.show()
    plt.savefig('correlationplot'+namejets+extrastring+'.png')
    plt.savefig('correlationplot'+namejets+extrastring+'.pdf')


print X_train.corr()

correlations(X_train[y_train == 0],'train_background')
correlations(X_train[y_train == 1],'train_signal')
correlations(X_test[y_test == 0],'test_background')
correlations(X_test[y_test == 1],'test_signal')

