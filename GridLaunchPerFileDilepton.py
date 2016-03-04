import xml.etree.cElementTree as ET
import subprocess
import time
import os
import glob
#from __future__ import print_function
#import shutil

tree = ET.ElementTree(file='config/Run2DiLepton_TOPTREES.xml')

root = tree.getroot()
datasets = root.find('datasets')

print "found  "  + str(len(datasets)) + " datasets"

# numCores = 8
args = []
# topTrees = []
# jobSize = 200000
for d in datasets:
    if d.attrib['add'] == '1':
        print "found dataset to be added..." + str(d.attrib['name'])
        files = glob.glob(d.attrib['filenames'])
        # execCommands = []
        rootfiles=[]
        for f in files:
            rootfiles = 'dcap://maite.iihe.ac.be:'+f

        # for rf in rootfiles:
            execCommands = ["./MACRO", d.attrib['name'], d.attrib['title'], d.attrib['add'], d.attrib['color'], d.attrib['ls'], d.attrib['lw'], d.attrib['normf'], d.attrib['EqLumi'], d.attrib['xsection'], d.attrib['PreselEff'], rootfiles, f]
            args.append(execCommands)
        #     print ' '
        #     print 'print args in loop'
        #     print args
        # print ' '
        # print 'printing rootfiles'
        # print rootfiles

outfiles = []
fileNames = []
processes = []
tempList = []
if not os.path.exists("Submit_Scripts"):
    os.makedirs("Submit_Scripts")
# print ' '

# print 'final args list'
# print args

for i,row in enumerate(args):
    
    #print row[11]+'\n'
    splitArray = row[11].split('_')
    #print splitArray[len(splitArray)-1]
    fileNumber = splitArray[len(splitArray)-1].split('.')[0]
    #print fileNumber
    # print row[12]+'\n'
    argsrow = row[11][len(row[11])-6]
    argsrow1 = row[11][len(row[11])-7]
    argsrow2 = row[11][len(row[11])-8]
    command = row[0]+" "+row[1]+"_"+fileNumber+" "+row[2]+" "+row[3]+" "+row[4]+" "+row[5]+" "+row[6]+" "+row[7]+" "+row[8]+" "+row[9]+" "+row[10]#+" "+row[11]
    f = open("Submit_Scripts/DiLepSubmit"+row[1]+"_"+fileNumber+".sh",'w')
    #print argsrow2+" "+argsrow1+" "+argsrow
    f.write('#PBS -q localgrid\n')
    f.write('#PBS -l walltime=1:00:00\n')
    # f.write('#PBS -l select=4:mpiprocs=8\n')
    f.write('source /user/heilman/.bash_profile\n')
    f.write('export X509_USER_PROXY=/localgrid/heilman/proxy\n')
    f.write('echo ">> got proxy"\n')
    f.write('cd /user/heilman/CMSSW_7_6_3/src/\n')
    f.write('echo ">> moved into directory"\n')
    f.write('eval `scramv1 runtime -sh`\n')
    f.write('dccp '+row[11]+' $TMPDIR/TOPTREE_'+fileNumber+'.root\n')
    f.write('mkdir $TMPDIR/TopBrussels\n')
    f.write('cp -pr ./TopBrussels/TopTreeProducer $TMPDIR/TopBrussels/\n')
    f.write('cp -pr ./TopBrussels/TopTreeAnalysisBase $TMPDIR/TopBrussels/\n')
    f.write('mkdir $TMPDIR/TopBrussels/FourTops\n')
    f.write('cp -pr ./TopBrussels/FourTops/bTagWeightHistosPtEta_Dilep_Comb.root $TMPDIR/TopBrussels/FourTops/bTagWeightHistosPtEta_Dilep_Comb.root\n')
    f.write('cp -pr ./TopBrussels/FourTops/SFHist.root $TMPDIR/TopBrussels/FourTops/SFHist.root\n')
    f.write('cp -pr ./TopBrussels/FourTops/MACRO $TMPDIR/TopBrussels/FourTops/MACRO\n')
    f.write('cp -pr ./TopBrussels/FourTops/MVA $TMPDIR/TopBrussels/FourTops/\n')
    f.write('cp -pr ./TopBrussels/FourTops/MVAOutput_TTbarJES.root $TMPDIR/TopBrussels/FourTops/MVAOutput_TTbarJES.root\n')
#    f.write('cd TopBrussels/FourTops\n')
    f.write('echo $TMPDIR\n')
    f.write('echo ">> Copying work area"\n')
    f.write('cd $TMPDIR/TopBrussels/FourTops\n')
    f.write('ls -l $TMPDIR \n')
    f.write(command+' $TMPDIR'+'/TOPTREE_'+fileNumber+'.root\n')
    f.write('echo ">> Training Complete"\n')
    f.write('ls -l\n')
    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/Craneens_* /user/heilman/batchoutput\n')
#    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/Histo_* /user/heilman/batchoutput\n')
    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/MACRO_* /user/heilman/batchoutput\n')
#    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/*.tex /user/heilman/batchoutput\n')
#    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/*.txt /user/heilman/batchoutput\n')
