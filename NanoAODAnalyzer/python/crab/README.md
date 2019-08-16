
To send CRAB jobs

## 1. Setup Grid proxy and CRAB3
```bash
voms-proxy-init --voms cms -valid 168:00:00
source /cvmfs/cms.cern.ch/crab3/crab.sh
```

## 2. Make symlinks to sample lists
```bash
ln -s ${CMSSW_BASE}/src/XMuMuAnalysis/SamplesList/Crab_SampleList_2016_Data.txt    .
ln -s ${CMSSW_BASE}/src/XMuMuAnalysis/SamplesList/Crab_SampleList_2016_MC_Bkgd.txt .
``` 


## 3. Send CRAB jobs
```bash
python crab_2016_data.py Crab_SampleList_2016_Data.txt
python crab_2016_mc_bkgd.py Crab_SampleList_2016_MC_Bkgd.txt
```