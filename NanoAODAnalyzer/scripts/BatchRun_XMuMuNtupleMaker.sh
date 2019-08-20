#!/bin/bash

cd /afs/cern.ch/work/n/nbinnorj/AnaXMuMu/CMSSW_10_2_15/src
eval `scramv1 runtime -sh`

SAMPLENAME="${1}"
SAMPLENAMEFORXSEC="${2}"
CONFIG="XMuMuAnalysis/NanoAODAnalyzer/scripts/${3}"

echo "SAMPLENAME        = " ${SAMPLENAME}
echo "SAMPLENAMEFORXSEC = " {$SAMPLENAMEFORXSEC}
echo "CONFIG            = " ${CONFIG}
echo "Running ProcessNanoAOD_XMuMu"
# 
# Run ProcesssNanoAOD
#
ProcessNanoAOD_XMuMu ${CONFIG} ${SAMPLENAME} ${SAMPLENAMEFORXSEC}
