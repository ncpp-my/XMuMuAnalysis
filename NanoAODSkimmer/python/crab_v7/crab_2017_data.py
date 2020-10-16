import sys
import crab_2017_common 

def GetData2017Run(name):
  if "Run2017B" in name: return "2017B"
  if "Run2017C" in name: return "2017C"
  if "Run2017D" in name: return "2017D"
  if "Run2017E" in name: return "2017E"
  if "Run2017F" in name: return "2017F"

crab_2017_common.config.JobType.maxJobRuntimeMin = 600

crab_2017_common.config.JobType.scriptArgs = [
'isMC=0',
'isSig=0',
'era=2017',
]

if __name__ == '__main__':
  #
  # Read in txt file with list of samples
  #
  f = open(sys.argv[1]) 
  samplelist =  crab_2017_common.GetSampleList(f)
  print "Will send crab jobs for the following samples:"
  for dataset in samplelist:
    print dataset
  print "\n\n"

  from CRABAPI.RawCommand import crabCommand
  for i, dataset in enumerate(samplelist):
    print "%d/%d:Sending CRAB job: %s" % (i+1,len(samplelist), dataset)
    crab_2017_common.config.Data.inputDataset = dataset
    #
    # Get run name from dataset name and set it as argument for era flag
    #
    dataRunName=GetData2017Run(dataset)
    crab_2017_common.config.JobType.scriptArgs = [
    "isMC=0",
    "isSig=0",
    "era="+dataRunName,
    ]
    #
    # Have to make unique requestName. pain in the ass really
    # Sample naming convention is a bit dumb and makes this more difficult.
    #
    primaryName   = dataset.split('/')[1]
    secondaryName = dataset.split('/')[2]
    secondaryName = secondaryName.replace("02Apr2020","Data17NanoAODv7") #CHECK
    #
    requestName = primaryName + "_" + secondaryName
    requestName = "XMuMu_" + requestName + "_" + crab_2017_common.version
    crab_2017_common.config.General.requestName   = requestName
    #  
    outputDatasetTag = "XMuMu_" + secondaryName + "_" + crab_2017_common.version 
    crab_2017_common.config.Data.outputDatasetTag = outputDatasetTag 
    #
    print "requestName: ", requestName 
    print "outputDatasetTag: ", outputDatasetTag
    crabCommand('submit', config = crab_2017_common.config)
    print ""
