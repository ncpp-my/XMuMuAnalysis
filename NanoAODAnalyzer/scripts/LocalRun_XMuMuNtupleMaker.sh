#!/bin/bash


SAMPLES=(
"MC16_TTJets_DiLept"
"MC16_DYLL"
)

CONFIG="XMuMuAnalysis/NanoAODAnalyzer/scripts/ProcNanoAODSkim_MC2016_cfg.py"

for SAMPLE in "${SAMPLES[@]}"; do
  SAMPLENAME="$SAMPLE"
  SAMPLENAMEFORXSEC="$SAMPLE"
  # 
  # Run ProcesssNanoAOD
  #
  ProcessNanoAOD_XMuMu ${CONFIG} ${SAMPLENAME} ${SAMPLENAMEFORXSEC}
done

SAMPLES=(
"Data16B_SingleMuon"
"Data16C_SingleMuon"
"Data16D_SingleMuon"
"Data16E_SingleMuon"
"Data16F_SingleMuon"
"Data16G_SingleMuon"
"Data16H_SingleMuon"
)

CONFIG="XMuMuAnalysis/NanoAODAnalyzer/scripts/ProcNanoAODSkim_Data2016_cfg.py"

for SAMPLE in "${SAMPLES[@]}"; do
  SAMPLENAME="$SAMPLE"
  SAMPLENAMEFORXSEC="$SAMPLE"
  # 
  # Run ProcesssNanoAOD
  #
  ProcessNanoAOD_XMuMu ${CONFIG} ${SAMPLENAME} ${SAMPLENAMEFORXSEC}
done