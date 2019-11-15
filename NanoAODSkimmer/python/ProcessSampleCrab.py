#!/usr/bin/env python
#
# InspiredBy https://github.com/vhbb/vhbb-nano/blob/master/postproc.py
#
import os,sys
import ROOT
import argparse
ROOT.PyConfig.IgnoreCommandLineOptions = True

from PhysicsTools.NanoAODTools.postprocessing.framework.postprocessor import * 
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2016_mc_sig 
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2016_mc_bkgd
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2016_data   

from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2017_mc_sig 
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2017_mc_bkgd
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2017_data   

from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2018_mc_sig 
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2018_mc_bkgd
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2018_data   


print "args are: ",sys.argv

isMC = True
isSig = True
era = "2016"

parser = argparse.ArgumentParser("")
parser.add_argument('-jobNum', '--jobNum', type=int, default=1,      help="") #NOTE: This will be given by  condor on the grid. not by us
parser.add_argument('-isMC',     '--isMC', type=int, default=1,      help="")
parser.add_argument('-isSig',   '--isSig', type=int, default=1,      help="")
parser.add_argument('-era',       '--era', type=str, default="2018", help="")

args = parser.parse_args()
print "args = ",args

isMC    = args.isMC
isSig   = args.isSig
era     = args.era

print "isMC = ",isMC," isSig = ",isSig, "era = ",era

varTxtFileIn="keep_and_drop_branches.txt"
varTxtFileOut="keep_and_drop_branches.txt"

selection=""

modules = []
if era == "2016":
  if isMC: 
    if isSig: 
      modules=[XMuMuNanoSkimmer_2016_mc_sig()]
    else:     
      modules=[XMuMuNanoSkimmer_2016_mc_bkgd()]
  else:              
    modules=[XMuMuNanoSkimmer_2016_data()]
elif era == "2017":
  if isMC: 
    if isSig: 
      modules=[XMuMuNanoSkimmer_2017_mc_sig()]
    else:     
      modules=[XMuMuNanoSkimmer_2017_mc_bkgd()]
  else:              
    modules=[XMuMuNanoSkimmer_2017_data()]
elif era == "2018":
  if isMC: 
    if isSig: 
      modules=[XMuMuNanoSkimmer_2018_mc_sig()]
    else:     
      modules=[XMuMuNanoSkimmer_2018_mc_bkgd()]
  else:              
    modules=[XMuMuNanoSkimmer_2018_data()]


# 
#this takes care of converting the input files from CRAB
#
from PhysicsTools.NanoAODTools.postprocessing.framework.crabhelper import inputFiles, runsAndLumis

if isMC:
  p=PostProcessor(
    ".", 
    inputFiles(),
    cut=selection,
    branchsel=varTxtFileIn,
    outputbranchsel=varTxtFileOut,
    modules=modules,
    provenance=True,
    fwkJobReport=True,
    histFileName="histo.root",
    histDirName="cutflow"
  )
else:
  p=PostProcessor(
    ".", 
    inputFiles(),
    cut=selection,
    branchsel=varTxtFileIn,
    outputbranchsel=varTxtFileOut,
    modules=modules,
    provenance=True,
    fwkJobReport=True,
    histFileName="histo.root",
    histDirName="cutflow",
    jsonInput=runsAndLumis(),
  )

p.run()

print "DONE"
os.system("ls -lR")