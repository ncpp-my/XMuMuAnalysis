# XMuMuAnalysis

## Setting up framework
### 0. Setup CMSSW for the first time

Make a working directory (any name would do):
```bash
mkdir AnaXMuMu
cd AnaXMuMu
```

Setup a CMSSW release.

```bash
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_2_15
cd CMSSW_10_2_15/src
cmsenv
```

### 1. Checkout framework

```bash
git clone git@github.com:ncpp-my/XMuMuAnalysis.git XMuMuAnalysis
```

### 2. Checkout framework which the parent NanoAODAnalysis framework

```bash
git clone git@github.com:nurfikri89/NanoAODAnalysis.git 
```

### 3. Checkout NanoAOD-Tools

Get NanoAODTools framework

```bash
git clone git@github.com:cms-nanoAOD/nanoAOD-tools.git PhysicsTools/NanoAODTools

```

Instruction from to https://github.com/cms-nanoAOD/nanoAOD-tools#checkout-instructions-cmssw

### 4. Compile

```bash
scram b -j4
cmsenv
```

### 5. Change path to output

You should specify the path to the directory where you want to store your output in [ProcNanoAODSkim_Common.py](NanoAODAnalyzer/scripts/ProcNanoAODSkim_Common.py) file at the following [line](https://github.com/ncpp-my/XMuMuAnalysis/blob/b7fb85a5e58192e9ca41714b2239fe83c77a0112/NanoAODAnalyzer/scripts/ProcNanoAODSkim_Common.py#L4):

```python
outputListDir         = "root://eosuser.cern.ch//eos/user/n/nbinnorj/XMuMuAna/Ntuples/"
```

## Run jobs locally

### 1. Setup

```bash
ln -s XMuMuAnalysis/NanoAODAnalyzer/scripts/LocalRun_XMuMuNtupleMaker_2016.sh . 
```

### 2. Run the bash script

```bash
source ./LocalRun_XMuMuNtupleMaker_2016.sh
```

## Sending jobs to the batch farm

This is for running the NtupleMaker on the batch farm.

### 1. Setup

Make a symlink of the source file (.sh) and its submission file (.sh.sub) in the main source code directory.

```bash
cd ${CMSSW_BASE}/src
ln -s XMuMuAnalysis/NanoAODAnalyzer/scripts/BatchRun_XMuMuNtupleMaker_2016.sh.sub .
ln -s XMuMuAnalysis/NanoAODAnalyzer/scripts/BatchRun_XMuMuNtupleMaker_2016.sh . 
```
Make a directory to store the batch logs from the jobs.

```bash
mkdir BatchLog
cd BatchLog
ln -s ../XMuMuAnalysis/NanoAODAnalyzer/scripts/CheckJobStatus.py
cd ${CMSSW_BASE}/src
```

### 2. Send the job
Its always good practice to compile and run ```cmsenv``` before sending jobs to lxplus's HTCondor batch farm.

```bash
scram b -j4
cmsenv
condor_submit ./BatchRun_XMuMuNtupleMaker_2016.sh.sub
```

### 3. To check the status of the jobs

```bash
cd BatchLog
python CheckJobStatus.py
```

