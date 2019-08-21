import glob
import subprocess



def HaddSamples(ntupleName,ntupleNameToGlob):
  
  EOSUSER ="root://eosuser.cern.ch/"
  INDIR   ="/eos/user/n/nbinnorj/XZVllqqAna/Ntuples/"
  OUTDIR  ="/eos/user/n/nbinnorj/XZVllqqAna/Ntuples/"

  inFileList  = [EOSUSER+f for f in glob.glob(INDIR+ntupleNameToGlob)]
  outFile     =  EOSUSER+OUTDIR+ntupleName

  cmd  =  ["hadd"]
  cmd +=  [outFile]
  cmd +=  inFileList

  subprocess.call(cmd)


samples =[ 
  # "MC16_DYLL_Pt-50To100",
  "MC16_DYLL_Pt-100To250",
]

for sample in samples:
  ntupleName="Ntuple_"+sample+".root"
  ntupleNameToGlob="Ntuple_"+sample+"_*.root"
  HaddSamples(ntupleName,ntupleNameToGlob)
