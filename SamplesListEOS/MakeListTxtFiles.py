import os
import glob
import collections

EOSUSERXROOTD="root://eosuser.cern.ch/"
EOSUSERDIR="/eos/user/z/zzolkapl/CRABOUTPUT_XMuMu/"

def main():

  samplesMC = collections.OrderedDict()
  samplesMC["MC16_DYLL"]               = "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_TT_2L"]              = "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_TT_2L_PW"]           = "TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_TT_1L_PW"]           = "TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_ST_s-chan"]          = "ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/*/*/*/tree_*.root"
  samplesMC["MC16_ST_t-chan_antitop"]  = "ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/*/*/*/tree_*.root"
  samplesMC["MC16_ST_t-chan_top"]      = "ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/*/*/*/tree_*.root"
  samplesMC["MC16_ST_tW_antitop"]      = "ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/*/*/*/tree_*.root"
  samplesMC["MC16_ST_tW_top"]          = "ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/*/*/*/tree_*.root"
  samplesMC["MC16_WZTo2L2Q"]           = "WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_ZZTo2L2Q"]           = "ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_ZZTo2L2Nu"]          = "ZZTo2L2Nu_13TeV_powheg_pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_ZZTo4L"]             = "ZZTo4L_13TeV-amcatnloFXFX-pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_WZTo3LNu"]           = "WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/*/*/*/tree_*.root"
  samplesMC["MC16_WWTo2L2Nu"]          = "WWTo2L2Nu_13TeV-powheg/*/*/*/tree_*.root"

  samplesData = collections.OrderedDict()
  samplesData["Data16B_SingleMuon"] = "SingleMuon/XMuMu_Run2016B*/*/*/tree_*.root"
  samplesData["Data16C_SingleMuon"] = "SingleMuon/XMuMu_Run2016C*/*/*/tree_*.root"
  samplesData["Data16D_SingleMuon"] = "SingleMuon/XMuMu_Run2016D*/*/*/tree_*.root"
  samplesData["Data16E_SingleMuon"] = "SingleMuon/XMuMu_Run2016E*/*/*/tree_*.root"
  samplesData["Data16F_SingleMuon"] = "SingleMuon/XMuMu_Run2016F*/*/*/tree_*.root"
  samplesData["Data16G_SingleMuon"] = "SingleMuon/XMuMu_Run2016G*/*/*/tree_*.root"
  samplesData["Data16H_SingleMuon"] = "SingleMuon/XMuMu_Run2016H*/*/*/tree_*.root"

  #
  # Make the text files
  #
  MakeLists(samplesMC)
  MakeLists(samplesData)

def MakeLists(samples):

  for sample in samples:
    print "Make list for ", sample
    #
    # For this sample, get the directory name
    #
    sampleDir = samples[sample]
    #
    # Get the files list by globbing
    #
    filesList = glob.glob(EOSUSERDIR+sampleDir)
    filesListFinal = [EOSUSERXROOTD+files for files in filesList]
    #
    #
    #
    with open(sample+".txt", 'w') as f:
      f.writelines("%s\n" % file for file in filesListFinal)


if __name__== "__main__":
  main()

