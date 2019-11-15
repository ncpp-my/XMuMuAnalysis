#!/bin/bash

SAMPLES=(
"MC16_DYLL" 
"MC16_TT_2L_PW"         
"MC16_TT_1L_PW"
"MC16_ST_s-chan"
"MC16_ST_t-chan_antitop"
"MC16_ST_t-chan_top"
"MC16_ST_tW_antitop"
"MC16_ST_tW_top"
"MC16_WZTo2L2Q"
"MC16_ZZTo2L2Q"
"MC16_ZZTo2L2Nu"
"MC16_ZZTo4L"
"MC16_WZTo3LNu"
"MC16_WWTo2L2Nu"
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