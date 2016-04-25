#! /bin/bash

ls SubmitScripts/9_3_2016/Dilep/*.sh | awk '{print "qsub "$1"\n"}' | sh
