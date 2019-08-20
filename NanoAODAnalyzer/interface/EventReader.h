// -*- C++ -*-
#if !defined(EventReader_H)
#define EventReader_H

#include <iostream>

#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include "NanoAODAnalysis/NanoAODAnalyzer/interface/InitBranch.h"
#include "NanoAODAnalysis/NanoAODAnalyzer/interface/RecoMuonReader.h"
#include "NanoAODAnalysis/NanoAODAnalyzer/interface/RecoJetReader.h"
#include "NanoAODAnalysis/NanoAODAnalyzer/interface/RecoMETReader.h"
#include "NanoAODAnalysis/NanoAODAnalyzer/interface/TrigObjectReader.h"
#include "NanoAODAnalysis/NanoAODAnalyzer/interface/GenParticleReader.h"
#include "NanoAODAnalysis/NanoAODAnalyzer/interface/HelperFunctions.h"

using namespace NanoAODAnalysis;

namespace XMuMuAnalysis {

  class EventReader {

  public:
    // Member variables
    TChain* tree;
    bool isMC;
    std::string eraName;
    bool isSignal;
    bool debug;

    UInt_t    run       =  0;
    UInt_t    lumiBlock =  0;
    ULong64_t event     =  0;
    Int_t     nPVs      =  0;
    Int_t     nPVsGood  =  0;
    Float_t   genWeight =  1.0;

    Float_t   evtWeight =  1.0;

    RecoMuonReader*      readerMuons;
    RecoJetReader*       readerJets;
    RecoMETReader*       readerMET;
    TrigObjectReader*    readerTrigObjects;
    GenParticleReader*   readerGenParticles;

    std::vector< RecoMuonPtr >     recoMuons;
    std::vector< RecoJetPtr >      recoJets;
    std::vector< RecoJetPtr >      recoBJets;
    std::vector< RecoJetPtr >      recoLightJets;
    std::vector< TrigObjectPtr >   trigObjectsMuon;

    unsigned int nRecoMuons = 0;
    RecoMuonPtr muon0 = nullptr;
    RecoMuonPtr muon1 = nullptr;

    TrigObjectPtr trigMuon0 = nullptr;
    TrigObjectPtr trigMuon1 = nullptr;

    bool oppSign   = false;

    float mu0SF = 1.0;
    float mu1SF = 1.0;

    int mu0RecoJetIdx = -1;
    int mu1RecoJetIdx = -1;

    bool mu0IsTrigMatch = false;
    bool mu1IsTrigMatch = false;

    unsigned int nRecoJets  = 0;
    unsigned int nRecoBJets = 0;
    unsigned int nRecoLightJets = 0;

    bool passPreselTriggers = false;
    bool passPreselMuons    = false;
    
    bool passMuMu            = false;
    bool passMuMuTightIdIso  = false;
    bool passMuMuHighPtIdIso = false;

    bool passTrigTightHLT    = false;
    bool passTrigHighPtHLT   = false;
    bool passTrigTightMatch  = false;
    bool passTrigHighPtMatch = false;
    bool passTrigger         = false;

    bool HLT_IsoMu24 = false;
    bool HLT_IsoTkMu24 = false;
    bool HLT_IsoMu27 = false;
    bool HLT_IsoTkMu27 = false;
    bool HLT_Mu50   = false;
    bool HLT_TkMu50 = false;

    bool Flag_goodVertices = false;
    bool Flag_globalSuperTightHalo2016Filter = false;
    bool Flag_HBHENoiseFilter = false;
    bool Flag_HBHENoiseIsoFilter = false;
    bool Flag_EcalDeadCellTriggerPrimitiveFilter = false;
    bool Flag_BadPFMuonFilter = false;
    bool Flag_eeBadScFilter = false;

    bool passMETFilters     = false;

    TLorentzVector tlv_MuMu;
    TLorentzVector tlv_MuMu_Mu0;
    TLorentzVector tlv_MuMu_Mu1;

    float MET_pt;
    float MET_phi;

    const float muonPtMin      = 26;
    const float muonEtaMax     = 2.4;

    const float jetPtMin      = 20;
    const float jetEtaMax     = 2.4;
    const bool  doJetCleaning = false;
    const float jetCSVv2Cut   = 0.8484;

    const float trigMatchDR = 0.15;

    // Constructors and member functions
    EventReader(TChain*, bool, std::string, bool, bool); 
    bool LoadEventFromTTree(int);
    bool PassPreselection();
    bool ConstructEventHypothesis();
    void GetGenParticlesForSignalEvents();
    void LoadEvent(int e);

    ~EventReader(); 
  };
}
#endif // EventReader_H