import xml.etree.cElementTree as ET
import subprocess
import time
import os
import glob
#from __future__ import print_function
#import shutil

tree = ET.ElementTree(file='config/output_FullMuonTopTrees76.xml')

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
            execCommands = ["./SLMACROLocal", d.attrib['name'], d.attrib['title'], d.attrib['add'], d.attrib['color'], d.attrib['ls'], d.attrib['lw'], d.attrib['normf'], d.attrib['EqLumi'], d.attrib['xsection'], d.attrib['PreselEff'], rootfiles, f]
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
    command = row[0]+" "+row[1]+" "+row[2]+" "+row[3]+" "+row[4]+" "+row[5]+" "+row[6]+" "+row[7]+" "+row[8]+" "+row[9]+" "+row[10]#+" "+row[11]
    # print row[11]+'\n'
    # print row[12]+'\n'
    argsrow = row[11][len(row[11])-6]
    argsrow1 = row[11][len(row[11])-7]
    argsrow2 = row[11][len(row[11])-8]
    f = open("Submit_Scripts/MuSubmit"+row[1]+argsrow2+argsrow1+argsrow+".sh",'w')
    # print "Submit_Scripts/MuSubmit"+row[1]+argsrow2+argsrow1+argsrow+".sh"
    f.write('#PBS -q localgrid\n')
    f.write('#PBS -l walltime=1:00:00\n')
    # f.write('#PBS -l select=4:mpiprocs=8\n')
    f.write('source /user/lbeck/.bash_profile\n')
    f.write('export X509_USER_PROXY=/localgrid/lbeck/proxy\n')
    f.write('echo ">> got proxy"\n')
    f.write('cd /user/lbeck/CMSSW_7_6_3/src/\n')
    f.write('echo ">> moved into directory"\n')
    f.write('eval `scramv1 runtime -sh`\n')
    f.write('dccp '+row[11]+' $TMPDIR/TOPTREE_'+argsrow2+argsrow1+argsrow+'.root\n')
    f.write('cp -pr ./TopBrussels $TMPDIR/.\n')
#    f.write('cd TopBrussels/FourTops\n')
    f.write('echo $TMPDIR\n')
    f.write('echo ">> Copying work area"\n')
    f.write('cd $TMPDIR/TopBrussels/FourTops\n')
    f.write('ls -l $TMPDIR \n')
    f.write(command+' $TMPDIR'+'/TOPTREE_'+argsrow2+argsrow1+argsrow+'.root\n')
    f.write('echo ">> Training Complete"\n')
    f.write('ls -l\n')
    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/Craneens_* /user/lbeck/batchoutput\n')
    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/Histo_* /user/lbeck/batchoutput\n')
    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/SLMACRO_* /user/lbeck/batchoutput\n')
    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/*.tex /user/lbeck/batchoutput\n')
    f.write('cp -pr $TMPDIR/TopBrussels/FourTops/*.txt /user/lbeck/batchoutput\n')
