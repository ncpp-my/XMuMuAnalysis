import sys
import os
import glob
import ROOT
from collections import OrderedDict 

ROOT.gROOT.SetBatch()
ROOT.gROOT.LoadMacro("Helpers.C")
ROOT.gROOT.LoadMacro("../plotting/Helpers_Vars.h")
ROOT.ROOT.EnableImplicitMT(4)

#
# Only on lxplus7
# source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.14.00/x86_64-centos7-gcc48-opt/root/bin/thisroot.sh
# source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.16.00/x86_64-centos7-gcc48-opt/bin/thisroot.sh

EOSUSER="root://eosuser.cern.ch/"
INDIR="/eos/user/n/nbinnorj/XMuMuAna/Ntuples/"
OUTDIR="/eos/user/n/nbinnorj/XMuMuAna/Histos/"
treeName="XMuMuTree"

def main():
  
  if len(sys.argv) < 2:
    print "Need an argument:"
    print "python ProcessNtuple <sampleGroup>"
    return
  
  groupName  = sys.argv[1]
  sampleNames = []
  
  if groupName == "Top" or groupName == "All":
    sampleNames += [
      "MC16_TTJets_DiLept",
    ]
  if groupName == "DYLL" or groupName == "All":
    sampleNames += [
      "MC16_DYLL",
    ]
  if groupName == "Data" or groupName == "All":
    sampleNames += [
      "Data16B_SingleMuon",
      "Data16C_SingleMuon",
      "Data16D_SingleMuon",
      "Data16E_SingleMuon",
      "Data16F_SingleMuon",
      "Data16G_SingleMuon",
      "Data16H_SingleMuon",
    ]

  #
  #
  #
  histoInfos = MakeVarsForHistoDict()
  
  #
  # Run over each sample
  #  
  for sampleName in sampleNames:
    ProcessNtuple(histoInfos,sampleName)

def MakeVarsForHistoDict():

  #
  # This function is defined in ../plotting/Helpers_Vars.h
  #
  varMap = ROOT.GetMapOfVariables()

  #
  # Loop over  varMap and make a dictionary
  #
  histoInfos = OrderedDict() 
  for varName, v in varMap:
    histoInfos[varName]   = [v.varNameInTree, v.yAxisName, (v.nbins, v.xmin, v.xmax)] 
  
  #
  # List what we store in dictionary
  #
  print "===================================================================="
  print "Here are the variables we are going to make histograms of"
  for hist in histoInfos:
    print histoInfos[hist]
  print "===================================================================="

  return histoInfos

def ProcessNtuple(histoInfos, sampleName=""):

  print "=============================="
  print "ProcessNtuple START"
  print "Processing sampleName : ", sampleName
  
  timer = ROOT.TStopwatch()
  timer.Start()
  #
  # Get list of files
  #
  inFileList  = [EOSUSER+f for f in glob.glob(INDIR+"Ntuple_"+sampleName+".root")]
  #
  # Save in a vector of string
  #
  inFileListVec = ROOT.std.vector('string')()
  print "List of file(s):"
  for inFile in inFileList:
    print inFile
    inFileListVec.push_back(inFile)
  #
  # throw an exception if vector is empty
  #
  if len(inFileListVec) < 1:
    raise Exception('inFileList is empty. Please check for sampleName: ' + sampleName)
  #
  # Setup RDataFrame
  #
  df = ROOT.ROOT.RDataFrame(treeName,inFileListVec)
  df = df.Define("Count","1") # Dummy for single bin yields
  # df = df.Define("evtWeightWithPUWeight","evtWeight*mcPUWeight")
  #
  # Define filters
  #
  df_filters  = OrderedDict()
  df_filters["passMuMu"]            = df.Filter("passMuMu")
  df_filters["passMuMuTightIdIso"]  = df_filters["passMuMu"].Filter("passMuMuTightIdIso")
  df_filters["passTrigTightHLT"]    = df_filters["passMuMuTightIdIso"].Filter("passTrigTightHLT")
  df_filters["passTrigTightMatch"]  = df_filters["passTrigTightHLT"].Filter("passTrigTightMatch")
  df_filters["passMETFilters"]      = df_filters["passTrigTightMatch"].Filter("passMETFilters")

  #
  # Define the cut levels that we want to apply.
  #
  cutLevels = []
  cutLevels += [
    "passMuMu",
    "passMuMuTightIdIso",
    "passTrigTightHLT",
    "passTrigTightMatch",
    "passMETFilters"
  ]
  #
  weightName = ""
  if "Data" in sampleName:
    weightName = "evtWeight"
  else:
    # weightName = "evtWeightWithPUWeight"
    weightName = "evtWeight"
  #
  # Declare histograms
  #
  histograms  = OrderedDict()
  for cutLevel in cutLevels:
    for histoName in histoInfos:
      histoInfo       = histoInfos[histoName]
      histoNameFinal  = "h_%s_%s" %(cutLevel,histoName)
      histoDec        = (histoNameFinal, histoNameFinal + "_" + cutLevel + ";" + histoInfo[0] + ";" + histoInfo[1]) + histoInfo[2]
      histograms[histoNameFinal] = df_filters[cutLevel].Histo1D(histoDec, histoInfo[0], weightName)
  #
  # Save all histo in rootfile
  #
  # outFileName = "./rootfiles/histo_"+sampleName+".root"
  outFileName = EOSUSER+OUTDIR+"Histo_"+sampleName+".root"
  outFile = ROOT.TFile(outFileName,"RECREATE")
  for histoName in histograms:
    histograms[histoName].Write()
  outFile.Close()
  print "Outputfile = ", outFileName 
  #
  #
  timer.Stop()
  timer.Print()
  print "ProcessXZVllqqNtuple END"
  print "==============================\n"

if __name__== "__main__":
  main()
