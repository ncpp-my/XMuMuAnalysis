import os,sys
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from PhysicsTools.NanoAODTools.postprocessing.framework.postprocessor import * 
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2016_mc_sig
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2016_mc_bkgd
from XMuMuAnalysis.NanoAODAnalyzer.postproc.XMuMuNanoSkimmer import XMuMuNanoSkimmer_2016_data

CMSSW_BASE  = os.environ['CMSSW_BASE']
CMSXROOTD   = "root://cms-xrd-global.cern.ch/"

sampleDir = CMSXROOTD
sampleDir += "/store/mc/RunIISummer16NanoAODv4/"
sampleDir += "MSSMggA-HiggsToMuMu_MA-400_Tanb-30_13TeV_pythia8/"
sampleDir += "NANOAODSIM/PUMoriond17_Nano14Dec2018_102X_mcRun2_asymptotic_v6-v1/100000/"
infile = [sampleDir+"B7225465-4A05-1246-BC7E-B67019BB7ECE.root"]
isMC=True

if len(sys.argv)>2: 
  outputDir = sys.argv[2]
else:
  if isMC: 
    outputDir = "outputMC" 
  else:
    outputDir = "outputData" 

varTxtFileDir = CMSSW_BASE+"/src/XMuMuAnalysis/NanoAODAnalyzer/scripts/"
varTxtFileIn  = varTxtFileDir+"keep_and_drop_branches.txt"
varTxtFileOut = varTxtFileDir+"keep_and_drop_branches.txt"

modules = []

if era == "2016":
  if isMC: 
    if isSig: 
      modules=[XMuMuNanoSkimmer_2016_mc_sig()]
    else:     
      modules=[XMuMuNanoSkimmer_2016_mc_bkgd()]
  else:              
    modules=[XMuMuNanoSkimmer_2016_data()]

p=PostProcessor(
  outputDir, 
  infile,
  cut=selection,
  branchsel=varTxtFileIn,
  outputbranchsel=varTxtFileOut,
  modules=modules,
  provenance=False,
  fwkJobReport=False,
  histFileName="histo.root",
  histDirName="cutflow"
)
p.run()


