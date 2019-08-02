import sys
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'XMuMuPostNanoData16_Test_v0'
config.General.workArea        = '/afs/cern.ch/work/n/nbinnorj/private/crab_projects_4/'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName   = 'PSet.py'
config.JobType.scriptExe  = 'crab_script.sh'
config.JobType.maxJobRuntimeMin = 450
config.JobType.scriptArgs = [
'isMC=0',
'isSig=0',
'era=2016',
]
config.JobType.inputFiles = [
'../../scripts/keep_and_drop_branches_input.txt',
'../../scripts/keep_and_drop_branches_output.txt',
'../ProcessSampleCrab.py',
'../../../../PhysicsTools/NanoAODTools/scripts/haddnano.py' #hadd nano will not be needed once nano tools are in cmssw
]
config.JobType.sendPythonFolder  = True
config.JobType.outputFiles = ['tree.root','histo.root']

config.Data.inputDataset = '/SingleMuon/Run2016B_ver1-Nano1June2019_ver1-v1/NANOAOD'
config.Data.splitting    = 'FileBased'
config.Data.unitsPerJob  = 1

config.Data.publication = False
config.Data.outputDatasetTag = 'XMuMuPostNanoData16_Test_v0'
config.Data.allowNonValidInputDataset = True

config.JobType.allowUndistributedCMSSW = True

config.Data.outLFNDirBase  = '/store/user/nbinnorj/CRABOUTPUT_XMuMu/'
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

if __name__ == '__main__':
  #
  # Read in txt file with list of samples
  #
  f = open(sys.argv[1]) 
  samplelist = f.readlines()
  samplelist = [x.strip() for x in samplelist] 

  from CRABAPI.RawCommand import crabCommand
  for dataset in samplelist:
    print "\n\n"
    print "======================================================================================"
    if dataset.startswith( '#' ): 
      print "Skipping : ", dataset
      continue
    print "Send CRAB job for ", dataset
    config.Data.inputDataset = dataset
    #
    # Have to make unique requestName. pain in the ass really
    # Sample naming convention is a bit dumb and makes this more difficult.
    #
    primaryName   = dataset.split('/')[1]
    secondaryName = dataset.split('/')[2]
    secondaryName = secondaryName.replace("Nano1June2019","Data16NanoAODv5")
    #
    requestName = primaryName + "_" + secondaryName
    requestName = "XZV_" + requestName + "_PostProcv0" 
    outputDatasetTag = "XZV_" + secondaryName + "_PostProcv0"
    print requestName , " | ", outputDatasetTag
    config.General.requestName   = requestName
    config.Data.outputDatasetTag = outputDatasetTag 
    crabCommand('submit', config = config)