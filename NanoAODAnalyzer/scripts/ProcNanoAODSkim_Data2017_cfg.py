import sys
import optparse
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
sys.path.insert(0, 'XMuMuAnalysis/NanoAODAnalyzer/scripts/')
from ProcNanoAODSkim_Common import *
#
# Basic Configuration
#
era              = "2017"
debug            = False
nevents          = -1
#
# ParameterSets for use in bin/<script>.cc 
#
process = cms.PSet()
#
# LumiMask
#
JSONLumiFiles  = {
  '2017':'XMuMuAnalysis/NanoAODAnalyzer/data/lumi/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt'
} 
process.inputs = cms.PSet(
  lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange()),
)
lumiList = LumiList.LumiList(filename = JSONLumiFiles[era]).getCMSSWString().split(',')
process.inputs.lumisToProcess.extend(lumiList)
#
#
#
process.ProcessNanoAOD_XMuMu = cms.PSet(
  debug            = cms.bool(debug),
  maxEvents        = cms.int32(int(nevents)),
  inputListDir     = cms.string(inputListDir_2017),
  outputListDir    = cms.string(outputListDir),
  era              = cms.string(era),
  blind            = cms.bool(True),
  isMC             = cms.bool(False),
)

