import sys
import crab_2018_common 

def GetData2018Run(name):
  if "Run2018A" in name: return "2018A"
  if "Run2018B" in name: return "2018B"
  if "Run2018C" in name: return "2018C"
  if "Run2018D" in name: return "2018D"

crab_2018_common.config.JobType.maxJobRuntimeMin = 600

crab_2018_common.config.JobType.scriptArgs = [
'isMC=0',
'isSig=0',
'era=2018',
]

if __name__ == '__main__':
  #
  # Read in txt file with list of samples
  #
  f = open(sys.argv[1]) 
  samplelist =  crab_2018_common.GetSampleList(f)
  print "Will send crab jobs for the following samples:"
  for dataset in samplelist:
    print dataset
  print "\n\n"

  from CRABAPI.RawCommand import crabCommand
  for i, dataset in enumerate(samplelist):
    print "%d/%d:Sending CRAB job: %s" % (i+1,len(samplelist), dataset)
    crab_2018_common.config.Data.inputDataset = dataset
    #
    # Get run name from dataset name and set it as argument for era flag
    #
    dataRunName=GetData2018Run(dataset)
    crab_2018_common.config.JobType.scriptArgs = [
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
    secondaryName = secondaryName.replace("02Apr2020","Data18NanoAODv7") #CHECK
    #
    requestName = primaryName + "_" + secondaryName
    requestName = "XMuMu_" + requestName + "_" + crab_2018_common.version
    crab_2018_common.config.General.requestName   = requestName
    #  
    outputDatasetTag = "XMuMu_" + secondaryName + "_" + crab_2018_common.version 
    crab_2018_common.config.Data.outputDatasetTag = outputDatasetTag 
    #
    print "requestName: ", requestName 
    print "outputDatasetTag: ", outputDatasetTag
    crabCommand('submit', config = crab_2018_common.config)
    print ""
