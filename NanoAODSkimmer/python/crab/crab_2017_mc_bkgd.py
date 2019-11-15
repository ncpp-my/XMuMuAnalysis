import sys
import crab_common 

crab_common.config.General.requestName = 'XMuMuPostNanoMC17_'+crab_common.version

crab_common.config.JobType.maxJobRuntimeMin = 300

crab_common.config.JobType.scriptArgs = [
'isMC=1',
'isSig=0',
'era=2017',
]

crab_common.config.Data.inputDataset     = '/WprimeToWZToWhadZlep_width0p1_M-800_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer17NanoAODv5-PUMoriond17_Nano1June2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM' #Dummy
crab_common.config.Data.outputDatasetTag = 'XMuMuPostNanoMC17_Test_'+crab_common.version  # Dummy

if __name__ == '__main__':
  #
  # Read in txt file with list of samples
  #
  f = open(sys.argv[1]) 
  samplelist =  crab_common.GetSampleList(f)
  print "Will send crab jobs for the following samples:"
  for dataset in samplelist:
    print dataset
  print "\n\n"

  from CRABClient.UserUtilities import getUsernameFromSiteDB
  from CRABAPI.RawCommand import crabCommand
  for i, dataset in enumerate(samplelist):
    print "%d/%d:Sending CRAB job: %s" % (i+1,len(samplelist), dataset)
    crab_common.config.Data.inputDataset = dataset
    #
    # Have to make unique requestName. pain in the ass really
    # Sample naming convention is a bit dumb and makes this more difficult.
    #
    primaryName   = dataset.split('/')[1].split("_")[0:3]
    primaryName   = "_".join(primaryName)
    secondaryName = dataset.split('/')[2]
    secondaryName = secondaryName.replace("RunIIFall17NanoAODv5-PU2017_12Apr2018","MC17NanoAODv5")#CHECK
    secondaryName = secondaryName.replace("Nano1June2019_","")#CHECK
    #
    requestName = primaryName + "_" + secondaryName.replace("102X_mc2017_realistic_v7-v1","")#CHECK
    requestName = "XZV_" + requestName + "_" + crab_common.version
    crab_common.config.General.requestName   = requestName
    #  
    outputDatasetTag = "XZV_" + secondaryName + "_" + crab_common.version
    crab_common.config.Data.outputDatasetTag = outputDatasetTag 
    #
    print requestName , " | ", outputDatasetTag, "\n"
    crabCommand('submit', config = crab_common.config)