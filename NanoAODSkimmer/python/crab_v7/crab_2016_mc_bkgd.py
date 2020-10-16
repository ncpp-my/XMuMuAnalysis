import sys
import crab_2016_common 

crab_2016_common.config.JobType.maxJobRuntimeMin = 600

crab_2016_common.config.JobType.scriptArgs = [
'isMC=1',
'isSig=0',
'era=2016',
]

if __name__ == '__main__':
  #
  # Read in txt file with list of samples
  #
  f = open(sys.argv[1]) 
  samplelist =  crab_2016_common.GetSampleList(f)
  print "Will send crab jobs for the following samples:"
  for dataset in samplelist:
    print dataset
  print "\n\n"

  from CRABAPI.RawCommand import crabCommand
  for i, dataset in enumerate(samplelist):
    print "%d/%d:Sending CRAB job: %s" % (i+1,len(samplelist), dataset)
    crab_2016_common.config.Data.inputDataset = dataset
    #
    # Have to make unique requestName. pain in the ass really
    # Sample naming convention is a bit dumb and makes this more difficult.
    #
    primaryName   = dataset.split('/')[1]
    primaryName   = primaryName.replace("_13TeV","")
    primaryName   = primaryName.replace("_TuneCUETP8M1","")
    primaryName   = primaryName.replace("_TuneCP5","")
    primaryName   = primaryName.replace("pythia","py")
    #
    secondaryName = crab_2016_common.TrimSecondaryNameMC(dataset)
    #
    requestName = primaryName + "_" + secondaryName
    requestName = "XMuMu_" + requestName + "_" + crab_2016_common.version
    crab_2016_common.config.General.requestName   = requestName
    #  
    outputDatasetTag = "XMuMu_" + secondaryName + "_" + crab_2016_common.version
    crab_2016_common.config.Data.outputDatasetTag = outputDatasetTag 
    #
    print "requestName: ", requestName 
    print "outputDatasetTag: ", outputDatasetTag
    crabCommand('submit', config = crab_2016_common.config)
    print ""
