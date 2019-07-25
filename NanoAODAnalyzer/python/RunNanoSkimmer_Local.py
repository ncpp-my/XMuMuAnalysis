import os,sys
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from PhysicsTools.NanoAODTools.postprocessing.framework.postprocessor import * 
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2016_mc
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2016_data

CMSSW_BASE  = os.environ['CMSSW_BASE']
CMSXROOTD   = "root://xrootd-cms.infn.it/"
# CMSXROOTD   = "root://cms-xrd-global.cern.ch"

isMC=True

sampleDir = CMSXROOTD
sampleDir += "/store/mc/NANOAODSIM/PUMoriond17_Nano14Dec2018_102X_mcRun2_asymptotic_v6-v1"
sampleDir += "/RunIISummer16NanoAODv4/MSSMggA-HiggsToMuMu_MA-400_Tanb-30_13TeV_pythia8/100000/"
infile = [sampleDir+"B7225465-4A05-1246-BC7E-B67019BB7ECE.root"]

# infile = ["C5D5AF70-6B15-6746-8DD5-E20B1A5609B2.root"]
# isMC=True

# infile = ["152878C4-A061-9048-9739-F46D95A2D975.root"]
# isMC=False


if len(sys.argv)>2: 
  outputDir = sys.argv[2]
else:
  if isMC: 
    outputDir = "outputMC" 
  else:
    outputDir = "outputData" 

varTxtFileDir = CMSSW_BASE+"/src/XMuMuAnalysis/NanoAODAnalyzer/scripts/"
varTxtFileIn  = varTxtFileDir+"keep_and_drop_branches_input.txt"
varTxtFileOut = varTxtFileDir+"keep_and_drop_branches_output.txt"

if isMC:
  p=PostProcessor(
    outputDir, 
    infile,
    cut="",
    branchsel=varTxtFileIn,
    outputbranchsel=varTxtFileOut,
    modules=[XMuMuNanoSkimmer_2016_mc()],
    provenance=False,
    fwkJobReport=False,
    histFileName="histo.root",
    histDirName="cutflow"
  )
else:
  p=PostProcessor(
    outputDir, 
    infile,
    cut="",
    branchsel=varTxtFileIn,
    outputbranchsel=varTxtFileOut,
    modules=[XMuMuNanoSkimmer_2016_data()],
    provenance=False,
    fwkJobReport=False,
    histFileName="histo.root",
    histDirName="cutflow"
  )
p.run()