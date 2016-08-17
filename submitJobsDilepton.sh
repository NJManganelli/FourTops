#! /bin/bash

ls SubmitScripts/11_7_2016/Dilep/submit_*.sh | awk '{print "qsub "$1"\n"}' | sh
