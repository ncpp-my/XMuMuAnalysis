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
era              = "2016"
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
  '2016':'XMuMuAnalysis/NanoAODAnalyzer/data/lumi/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt'
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
  inputListDir     = cms.string(inputListDir_2016),
  outputListDir    = cms.string(outputListDir),
  era              = cms.string(era),
  blind            = cms.bool(True),
  isMC             = cms.bool(False),
)

