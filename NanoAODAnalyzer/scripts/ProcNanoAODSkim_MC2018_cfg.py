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
era              = "2018"
debug            = False
nevents          = -1
#
#
# ParameterSets for use in bin/<script>.cc 
#
process = cms.PSet()
#
# LumiMask
#
process.inputs = cms.PSet(
  lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange()),
)
#
#
#
process.ProcessNanoAOD_XMuMu = cms.PSet(
  debug            = cms.bool(debug),
  maxEvents        = cms.int32(int(nevents)),
  inputListDir     = cms.string(inputListDir_2018),
  outputListDir    = cms.string(outputListDir),
  era              = cms.string(era),
  blind            = cms.bool(False),
  isMC             = cms.bool(True),
)
