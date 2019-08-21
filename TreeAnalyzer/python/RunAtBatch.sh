#!/bin/bash

cd /afs/cern.ch/work/n/nbinnorj/AnaXZVllqq/CMSSW_10_2_15/src/XZVllqqAnalysis/TreeAnalyzer/python
source sourceRecentROOT.sh
python ProcessXZVllqqNtuple.py ${1} 

