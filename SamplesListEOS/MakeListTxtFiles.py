import os
import glob
import collections

EOSUSERXROOTD="root://eosuser.cern.ch/"
EOSUSERDIR="/eos/user/z/zzolkapl/CRABOUTPUT_XMuMu/"

def main():

  samplesMC = collections.OrderedDict()
  samplesMC["MC16_DYLL"]               = "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_TTJets_DiLept"]      = "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*/*/*/tree_*.root"

  samplesData = collections.OrderedDict()
  samplesData["Data16B_SingleMuon"] = "SingleMuon/XMuMu_Run2016B*/*/*/tree_*.root"
  samplesData["Data16C_SingleMuon"] = "SingleMuon/XMuMu_Run2016C*/*/*/tree_*.root"
  samplesData["Data16D_SingleMuon"] = "SingleMuon/XMuMu_Run2016D*/*/*/tree_*.root"
  samplesData["Data16E_SingleMuon"] = "SingleMuon/XMuMu_Run2016E*/*/*/tree_*.root"
  samplesData["Data16F_SingleMuon"] = "SingleMuon/XMuMu_Run2016F*/*/*/tree_*.root"
  samplesData["Data16G_SingleMuon"] = "SingleMuon/XMuMu_Run2016G*/*/*/tree_*.root"
  samplesData["Data16H_SingleMuon"] = "SingleMuon/XMuMu_Run2016H*/*/*/tree_*.root"

  #
  # Make the text files
  #
  MakeLists(samplesMC)
  MakeLists(samplesData)

def MakeLists(samples):

  for sample in samples:
    print "Make list for ", sample
    #
    # For this sample, get the directory name
    #
    sampleDir = samples[sample]
    #
    # Get the files list by globbing
    #
    filesList = glob.glob(EOSUSERDIR+sampleDir)
    filesListFinal = [EOSUSERXROOTD+files for files in filesList]
    #
    #
    #
    with open(sample+".txt", 'w') as f:
      f.writelines("%s\n" % file for file in filesListFinal)


if __name__== "__main__":
  main()

