#!/bin/bash
#PBS -q localgrid
#PBS -l walltime=4:00:00
# setting up your code and your env
source /user/heilman/.bash_profile
cd /user/heilman/CMSSW_7_6_5/src
eval `scramv1 runtime -sh`
cd TopBrussels/FourTops

# want you really want to do!!

