// -*- C++ -*-
#if !defined(XMuMuTreeMaker_H)
#define XMuMuTreeMaker_H

#include <ctime>
#include <sys/resource.h>
#include <chrono>

#include <TChain.h>
#include <TTree.h>
#include <TSpline.h>

#include "DataFormats/FWLite/interface/InputSource.h" //for edm::LuminosityBlockRange
#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "NanoAODAnalysis/NanoAODAnalyzer/interface/InitBranch.h"
#include "XMuMuAnalysis/NanoAODAnalyzer/interface/EventReader.h"

using namespace NanoAODAnalysis;
using namespace XMuMuAnalysis;

namespace XMuMuAnalysis {

  class XMuMuTreeMaker {
  public:

    TChain* eventsTree;
    TChain* runsTree;
    TChain* lumiBlocksTree;
    Long64_t genEventCount;
    double_t genEventSumw;
    double_t genEventSumw2;
    Long64_t mcEventCount = 0;
    double_t mcEventSumw  = 0;
    double_t mcEventSumw2 = 0;

    std::chrono::steady_clock::time_point clock_begin; 
    std::chrono::steady_clock::time_point clock_current; 
    std::chrono::steady_clock::duration time_span; 
    double nseconds = 0.; 
    int elapsed_seconds = 0;
    int elapsed_minutes = 0;

    bool debug = false;
    std::string eraName;
    bool isMC  = false;
    bool blind = true;
    int treeEvents;

    bool isSignal = false;

    XMuMuAnalysis::EventReader* eventReader;
  
    long int nEvents = 0;
    float  intLumi = 0;

    float _lumi    = 1.;
    float _kFactor = 1.;
    float _xs      = 1.;
    std::vector<edm::LuminosityBlockRange> _lumiMask;

    bool writePicoAOD = false;
    TFile* picoAODFile;
    TTree* picoAODEvents;
    TTree* picoAODRuns;
    TTree* picoAODLumiBlocks;

    //Monitoring Variables
    long int percent = 0;
    std::clock_t start = 0;
    double duration  = 0.;
    double eventRate = 0.;
    double timeRemaining = 0.;
    int minutes = 0;
    int seconds = 0;
    int who = RUSAGE_SELF;
    struct rusage usage;
    long int usageMB = 0;

    TFile* ntupleFile;
    TTree* ntupleTree;

    UInt_t    b_run;
    UInt_t    b_lumiBlock;
    ULong64_t b_event;

    Int_t     b_nPVs;    
    Int_t     b_nPVsGood;

    Float_t   b_mcXS;
    Float_t   b_mcKFactor;
    Float_t   b_mcGenWeight;
    Float_t   b_evtWeight;

    Bool_t    b_passMuMu;
    Bool_t    b_passMuMuTightIdIso;
    Bool_t    b_passMuMuHighPtIdIso;

    Bool_t    b_passTrigTightHLT;
    Bool_t    b_passTrigHighPtHLT;
    Bool_t    b_passTrigTightMatch;
    Bool_t    b_passTrigHighPtMatch;
    Bool_t    b_passTrigger;

    Int_t     b_nRecoMuons;

    Float_t   b_mumu_pt;
    Float_t   b_mumu_eta;
    Float_t   b_mumu_phi;
    Float_t   b_mumu_mass;
    
    Float_t   b_mumu_mu0pt;
    Float_t   b_mumu_mu0eta;
    Float_t   b_mumu_mu0phi;
    Float_t   b_mumu_mu0SF;
    Bool_t    b_mumu_mu0IsTrigMatch;

    Float_t   b_mumu_mu1pt;
    Float_t   b_mumu_mu1eta;
    Float_t   b_mumu_mu1phi;
    Float_t   b_mumu_mu1SF;
    Bool_t    b_mumu_mu1IsTrigMatch;

    Int_t     b_nRecoJets;  
    Int_t     b_nRecoLightJets;  
    Int_t     b_nRecoBJets;  

    Float_t   b_MET_pt;
    Float_t   b_MET_phi;

    Bool_t    b_passMETFilters;

    XMuMuTreeMaker(TChain*, TChain*, TChain*, bool, bool, bool, std::string, bool);
    void CreateNtupleTree(std::string fileName);
    void StoreNtupleTree();
    int EventLoop(int);
    bool ProcessEvent();
    void Monitor(long int);
    bool PassLumiMask();
    void FillNtupleVariables();
    void ResetNtupleVariables();
    
    void SetLumiMask(std::vector<edm::LuminosityBlockRange> lumiMask){_lumiMask = lumiMask;}
    void SetMCXS(float xs){_xs = xs;}
    void SetMCkFactor(float kFact){_kFactor = kFact;}
    void SetIsSignal(bool isSig){isSignal = isSig;}
 
    ~XMuMuTreeMaker();
  };

}
#endif // XMuMuTreeMaker_H