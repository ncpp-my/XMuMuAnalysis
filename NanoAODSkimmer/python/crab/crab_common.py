from CRABClient.UserUtilities import config
config = config()
#
# Set version number
#
version="XMuMuNano_v1"
#
# Change this PATH where the crab directories are stored
# Example: config.General.workArea = '/afs/cern.ch/work/n/nbinnorj/private/crab_projects/'
#
config.General.workArea        = '/afs/cern.ch/work/n/nbinnorj/private/crab_projects/' #CHECK
config.General.transferOutputs = True
config.General.transferLogs    = True
#
config.JobType.pluginName = 'Analysis'
config.JobType.psetName   = 'PSet.py'
config.JobType.scriptExe  = 'crab_script.sh'
#
config.JobType.inputFiles = [
'../../scripts/keep_and_drop_branches.txt',
'../ProcessSampleCrab.py',
'../../../../PhysicsTools/NanoAODTools/scripts/haddnano.py' #hadd nano will not be needed once nano tools are in cmssw
]
#
config.JobType.sendPythonFolder  = True
config.JobType.outputFiles = ['tree.root','histo.root']
#
config.Data.splitting    = 'FileBased'
config.Data.unitsPerJob  = 1
config.Data.publication  = False
config.Data.allowNonValidInputDataset = True
config.JobType.allowUndistributedCMSSW = True
#
config.Data.outLFNDirBase  = '/store/user/nbinnorj/CRABOUTPUT_XMuMu_'+version+'/' #CHECK
config.Site.storageSite    = 'T2_CH_CERNBOX'
config.Data.ignoreLocality   = True
whitelist_sites=[
'T2_CH_CERN',
'T2_US_*',
'T2_UK_*',
'T2_DE_*',
'T2_FR_*',
]
config.Site.whitelist = whitelist_sites

# config.Data.ignoreLocality   = False
# whitelist_sites=['T2_CH_CERN']
# config.Site.whitelist = whitelist_sites

def GetSampleList(file):
  samplelist = file.readlines()
  samplelist = [x.strip() for x in samplelist] 
  samplelist = [x for x in samplelist if x] # Choose lines that are not empty
  samplelist = [x for x in samplelist if not(x.startswith("#"))] # Choose lines that do not start with #

  return samplelist