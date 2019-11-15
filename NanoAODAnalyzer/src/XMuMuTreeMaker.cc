#include <iostream>
#include <iomanip>
#include <cstdio>
#include <TROOT.h>
#include <boost/bind.hpp>

#include "XMuMuAnalysis/NanoAODAnalyzer/interface/XMuMuTreeMaker.h"

using namespace NanoAODAnalysis;
using namespace XMuMuAnalysis;

XMuMuTreeMaker::XMuMuTreeMaker(TChain* _events, TChain* _runs, TChain* _lumiBlocks,  bool _isMC, bool _isSignal, bool _blind, std::string _era, bool _debug){
  if(_debug) std::cout<<"In XMuMuTreeMaker constructor"<<std::endl;

  clock_begin = std::chrono::steady_clock::now();

  debug           = _debug;
  isMC            = _isMC;
  isSignal        = _isSignal;
  blind           = _blind;
  eraName         = _era;
  eventsTree      = _events;
  runsTree        = _runs;
  lumiBlocksTree  = _lumiBlocks;
  // Switch off all branches
  eventsTree->SetBranchStatus("*", 0);

  //Calculate MC weight denominator
  if(isMC){
    runsTree->SetBranchStatus("*", 0);
    runsTree->LoadTree(0);
    InitBranch(runsTree, "genEventCount", genEventCount);
    InitBranch(runsTree, "genEventSumw",  genEventSumw);
    InitBranch(runsTree, "genEventSumw2", genEventSumw2);
    for(unsigned int r = 0; r < runsTree->GetEntries(); r++){
      runsTree->GetEntry(r);
      mcEventCount += genEventCount;
      mcEventSumw  += genEventSumw;
      mcEventSumw2 += genEventSumw2;
    }
    std::cout << "mcEventCount " << mcEventCount << " | mcEventSumw " << mcEventSumw << std::endl;
  }
  //
  // brilcalc lumi -u /fb --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -i
  // /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt
  //
  if(eraName == "2016"){
    _lumi = 35921.88; //ipb
  }
  //
  // brilcalc lumi -u /fb --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -i
  // /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt
  //
  else if(eraName == "2017"){
    _lumi = 41529.15; //ipb
  }
  //
  // brilcalc lumi -u /fb --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -i
  // /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/ReReco/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt
  //
  else if(eraName == "2018"){
    _lumi = 59740.00; //ipb. NOTE:FIKRI: Taken from twiki. Should run brilcalc to get the exact value
  }

  eventAna = new XMuMuAnalysis::EventAna(eventsTree, isMC, eraName, isSignal,  debug);
  treeEvents  = eventsTree->GetEntries();
} 
void XMuMuTreeMaker::CreateNtupleTree(std::string fileName){
  
  std::cout << "Output File created: " << fileName << std::endl;

  ntupleFile = TFile::Open(fileName.c_str() , "RECREATE");
  
  ntupleTree = new TTree("XMuMuTree", "XMuMuTree");
  ntupleTree->Branch("run",                     &b_run,                  "run/I");
  ntupleTree->Branch("lumiBlock",               &b_lumiBlock,            "lumiBlock/I");
  ntupleTree->Branch("event",                   &b_event,                "event/l");
  ntupleTree->Branch("nPVs",                    &b_nPVs,                 "nPVs/I");
  ntupleTree->Branch("nPVsGood",                &b_nPVsGood,             "nPVsGood/I");
  ntupleTree->Branch("evtWeight",               &b_evtWeight,            "evtWeight/F");
  ntupleTree->Branch("mcXS",                    &b_mcXS,                 "mcXS/F");
  ntupleTree->Branch("mcKFactor",               &b_mcKFactor,            "mcKFactor/F");
  ntupleTree->Branch("mcGenWeight",             &b_mcGenWeight,          "mcGenWeight/F");
  ntupleTree->Branch("passMuMu",                &b_passMuMu,             "passMuMu/O");
  ntupleTree->Branch("passMuMuTightIdIso",      &b_passMuMuTightIdIso,   "passMuMuTightIdIso/O");
  ntupleTree->Branch("passMuMuHighPtIdIso",     &b_passMuMuHighPtIdIso,  "passMuMuHighPtIdIso/O");
  ntupleTree->Branch("passTrigTightHLT",        &b_passTrigTightHLT,     "passTrigTightHLT/O");
  ntupleTree->Branch("passTrigHighPtHLT",       &b_passTrigHighPtHLT,    "passTrigHighPtHLT/O");
  ntupleTree->Branch("passTrigTightMatch",      &b_passTrigTightMatch,   "passTrigTightMatch/O");
  ntupleTree->Branch("passTrigHighPtMatch",     &b_passTrigHighPtMatch,  "passTrigHighPtMatch/O");
  ntupleTree->Branch("passTrigger",             &b_passTrigger,          "passTrigger/O");
  ntupleTree->Branch("nRecoMuons",              &b_nRecoMuons,           "nRecoMuons/I");
  ntupleTree->Branch("mumu_pt",                 &b_mumu_pt,              "mumu_pt/F");
  ntupleTree->Branch("mumu_eta",                &b_mumu_eta,             "mumu_eta/F");
  ntupleTree->Branch("mumu_phi",                &b_mumu_phi,             "mumu_phi/F");
  ntupleTree->Branch("mumu_mass",               &b_mumu_mass,            "mumu_mass/F");
  ntupleTree->Branch("mumu_mu0pt",              &b_mumu_mu0pt,           "mumu_mu0pt/F");
  ntupleTree->Branch("mumu_mu0eta",             &b_mumu_mu0eta,          "mumu_mu0eta/F");
  ntupleTree->Branch("mumu_mu0phi",             &b_mumu_mu0phi,          "mumu_mu0phi/F");
  ntupleTree->Branch("mumu_mu0SF",              &b_mumu_mu0SF,           "mumu_mu0SF/F");
  ntupleTree->Branch("mumu_mu0IsTrigMatch",     &b_mumu_mu0IsTrigMatch,  "mumu_mu0IsTrigMatch/O");
  ntupleTree->Branch("mumu_mu1pt",              &b_mumu_mu1pt,           "mumu_mu1pt/F");
  ntupleTree->Branch("mumu_mu1eta",             &b_mumu_mu1eta,          "mumu_mu1eta/F");
  ntupleTree->Branch("mumu_mu1phi",             &b_mumu_mu1phi,          "mumu_mu1phi/F");
  ntupleTree->Branch("mumu_mu1SF",              &b_mumu_mu1SF,           "mumu_mu1SF/F");
  ntupleTree->Branch("mumu_mu1IsTrigMatch",     &b_mumu_mu1IsTrigMatch,  "mumu_mu1IsTrigMatch/O");
  ntupleTree->Branch("nSignalJets",             &b_nSignalJets,          "nSignalJets/I");
  ntupleTree->Branch("nSignalBJets",            &b_nSignalBJets,         "nSignalBJets/I");
  ntupleTree->Branch("nSignalLightJets",        &b_nSignalLightJets,     "nSignalLightJets/I");
  ntupleTree->Branch("MET_pt",                  &b_MET_pt,               "MET_pt/F");
  ntupleTree->Branch("MET_phi",                 &b_MET_phi,              "MET_phi/F");
  ntupleTree->Branch("passMETFilters",          &b_passMETFilters,       "passMETFilters/O");
}
void XMuMuTreeMaker::StoreNtupleTree(){
  ntupleFile->Write();
  ntupleFile->Close();
  std::cout << "Output File Saved " << std::endl;
}
int XMuMuTreeMaker::EventLoop(int maxEvents){

  //Set Number of events to process. Take manual maxEvents if maxEvents is > 0 and less than the total number of events in the input files. 
  nEvents = (maxEvents > 0 && maxEvents < treeEvents) ? maxEvents : treeEvents;
  
  float eventWeightScale = _lumi * _xs * _kFactor / mcEventSumw;
  std::cout << "eventWeightScale = lumi * xs * kFactor / mcEventSumw = " << _lumi << " * " << _xs << " * " << _kFactor << " / " << mcEventSumw << " = " << eventWeightScale << std::endl;
  std::cout << "\nProcess " << nEvents << " of " << treeEvents << " events.\n";

  bool passLoadEvent       = true;
  bool passPreselection    = true;
  bool passEventHypothesis = true;
  bool passEvent           = true;

  for(long int e = 0; e < nEvents; e++)
  {
    // Periodically update status
    if( (e+1)%10000 == 0 || e+1==nEvents || debug){
      Monitor(e);
    }
    //
    // Reset Ntuple Variables
    //
    ResetNtupleVariables();
    //
    // Load event from ttree
    //
    passLoadEvent = eventAna->LoadEventFromTTree(e);
    if(!passLoadEvent) continue;
    //
    // passPreselection
    //   
    passPreselection = eventAna->PassPreselection();
    if (!passPreselection) continue;
    //
    // passEventHypothesis
    //
    passEventHypothesis = eventAna->ConstructEventHypothesis(); 
    if (!passEventHypothesis) continue;
    //
    // passEvent
    //
    passEvent = ProcessEvent();
    if(!passEvent) continue;
    //
    // Event pass all. Fill in ntuple.
    //
    FillNtupleVariables();
    ntupleTree->Fill();
  }
  std::cout << std::endl;
  minutes = static_cast<int>(duration/60);
  seconds = static_cast<int>(duration - minutes*60);             
  if(isMC){
    fprintf(stdout,"---------------------------\nProcessed %li events in %02i:%02i\n", nEvents, minutes, seconds);
  }else{
    fprintf(stdout,"---------------------------\nProcessed %li events (%.2f/fb) in %02i:%02i\n", nEvents, intLumi/1000, minutes, seconds);
  }
  return 0;
}
bool XMuMuTreeMaker::ProcessEvent(){
  if(debug) std::cout << "ProcessEvent start" << std::endl;
  //
  // If MC
  //
  if(isMC){
    eventAna->evtWeight = eventAna->genWeight * (_lumi * _xs * _kFactor / mcEventSumw);
    if(debug) std::cout << "event->genWeight * (lumi * xs * kFactor / mcEventSumw) = " << eventAna->genWeight << " * (" << _lumi << " * " << _xs << " * " << _kFactor << " / " << mcEventSumw << ") = " << eventAna->evtWeight << std::endl;
    //
    // If its signal, just save all events
    //
    if(isSignal){
      return true;
    }
    else{
      if(!(eventAna->passMuMu)){
        if(debug) std::cout << "Fail ProcessEvent: MC" << std::endl;
        return false;
      }
    }
  }
  //
  //if we are processing data, first apply lumiMask and trigger
  //
  if(!isMC){
    if(!PassLumiMask()){
      if(debug) std::cout << "Fail lumiMask" << std::endl;
      return false;
    }
    if(!(eventAna->passMuMu)){
      if(debug) std::cout << "Fail ProcessEvent: data" << std::endl;
      return false;
    }
  }
  if(debug) std::cout << "ProcessEvent end" << std::endl;
  return true;
}
void XMuMuTreeMaker::Monitor(long int e){
  //Monitor progress
  clock_current = std::chrono::steady_clock::now();
  time_span     = clock_current - clock_begin;
  nseconds      = double(time_span.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
  elapsed_seconds = static_cast<int>(int(nseconds)%60);
  elapsed_minutes = static_cast<int>(nseconds/60); 
  
  percent        = (e+1)*100/nEvents;
  duration       = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  eventRate      = (e+1)/duration;
  timeRemaining  = (nEvents-e)/eventRate;
  minutes = static_cast<int>(timeRemaining/60);
  seconds = static_cast<int>(timeRemaining - minutes*60);
  getrusage(who, &usage);
  usageMB = usage.ru_maxrss/1024;
  //print status and flush stdout so that status bar only uses one line
  fprintf(stdout, "\rProcessed: %8li of %li ( %2li%% | %.0f events/s | done in %02i:%02i | time elapsed %02i:%02i | memory usage: %li MB)  ", 
                                e+1, nEvents, percent,   eventRate,    minutes, seconds, elapsed_minutes, elapsed_seconds, usageMB);
  fflush(stdout);
}
bool XMuMuTreeMaker::PassLumiMask(){
  // if the lumiMask is empty, then no JSON file was provided so all
  // events should pass
  if(_lumiMask.empty()) return true;

  //make lumiID run:lumiBlock
  edm::LuminosityBlockID lumiID(eventAna->run, eventAna->lumiBlock);

  //define function that checks if a lumiID is contained in a lumiBlockRange
  bool (*funcPtr) (edm::LuminosityBlockRange const &, edm::LuminosityBlockID const &) = &edm::contains;

  //Loop over the lumiMask and use funcPtr to check for a match
  std::vector< edm::LuminosityBlockRange >::const_iterator iter = std::find_if (_lumiMask.begin(), _lumiMask.end(), boost::bind(funcPtr, _1, lumiID) );

  return _lumiMask.end() != iter; 
}
void XMuMuTreeMaker::FillNtupleVariables(){
  b_run                       = eventAna->run;
  b_lumiBlock                 = eventAna->lumiBlock;
  b_event                     = eventAna->event;
  
  b_nPVs                      = eventAna->nPVs;
  b_nPVsGood                  = eventAna->nPVsGood;
  
  if(isMC){
    b_mcKFactor                 = _kFactor;
    b_mcXS                      = _xs;
    b_mcGenWeight               = eventAna->genWeight;
  }

  b_evtWeight                 = eventAna->evtWeight;

  b_passMuMu                  = eventAna->passMuMu;
  b_passMuMuTightIdIso        = eventAna->passMuMuTightIdIso;
  b_passMuMuHighPtIdIso       = eventAna->passMuMuHighPtIdIso;

  b_passTrigTightHLT          = eventAna->passTrigTightHLT;
  b_passTrigHighPtHLT         = eventAna->passTrigHighPtHLT;
  b_passTrigTightMatch        = eventAna->passTrigTightMatch;
  b_passTrigHighPtMatch       = eventAna->passTrigHighPtMatch;
  b_passTrigger               = eventAna->passTrigger;

  b_nRecoMuons                = eventAna->nRecoMuons;

  if (eventAna->passMuMu){
    b_mumu_pt                 = eventAna->tlv_MuMu.Pt();
    b_mumu_eta                = eventAna->tlv_MuMu.Eta();
    b_mumu_phi                = eventAna->tlv_MuMu.Phi();
    b_mumu_mass               = eventAna->tlv_MuMu.M();

    b_mumu_mu0pt              = eventAna->tlv_MuMu_Mu0.Pt();
    b_mumu_mu0eta             = eventAna->tlv_MuMu_Mu0.Eta();
    b_mumu_mu0phi             = eventAna->tlv_MuMu_Mu0.Phi();
    b_mumu_mu0SF              = eventAna->mu0SF;
    b_mumu_mu0IsTrigMatch     = eventAna->mu0IsTrigMatch;

    b_mumu_mu1pt              = eventAna->tlv_MuMu_Mu1.Pt();
    b_mumu_mu1eta             = eventAna->tlv_MuMu_Mu1.Eta();
    b_mumu_mu1phi             = eventAna->tlv_MuMu_Mu1.Phi();
    b_mumu_mu1SF              = eventAna->mu1SF;
    b_mumu_mu1IsTrigMatch     = eventAna->mu1IsTrigMatch;
  }
  
  b_nSignalJets              = eventAna->nSignalJets;
  b_nSignalLightJets         = eventAna->nSignalLightJets;
  b_nSignalBJets             = eventAna->nSignalBJets;
  
  b_MET_pt  = eventAna->MET_pt;
  b_MET_phi = eventAna->MET_phi;

  b_passMETFilters            = eventAna->passMETFilters;
}
void XMuMuTreeMaker::ResetNtupleVariables(){

  b_run = -9;
  b_lumiBlock = -9;
  b_event = -9;

  b_nPVs = -9;
  b_nPVsGood = -9;

  b_mcKFactor = 1.0;
  b_mcXS = 1.0;
  b_mcGenWeight = 1.0;

  b_evtWeight = 1.0;

  b_passMuMu = false;
  b_passMuMuTightIdIso = false;
  b_passMuMuHighPtIdIso = false;

  b_passTrigTightHLT = false;
  b_passTrigHighPtHLT = false;
  b_passTrigTightMatch = false;
  b_passTrigHighPtMatch = false;
  b_passTrigger = false;
  b_passMETFilters = false;

  b_nRecoMuons = -9;

  b_mumu_pt = -9.;
  b_mumu_eta = -9.;
  b_mumu_phi = -9.;
  b_mumu_mass = -9.;

  b_mumu_mu0pt = -9.;
  b_mumu_mu0eta = -9.;
  b_mumu_mu0phi = -9.;
  b_mumu_mu0SF = 1.0;
  b_mumu_mu0IsTrigMatch = false;

  b_mumu_mu1pt = -9.;
  b_mumu_mu1eta = -9.;
  b_mumu_mu1phi = -9.;
  b_mumu_mu1SF = 1.0;
  b_mumu_mu1IsTrigMatch = false;

  b_nSignalJets = -9;
  b_nSignalLightJets = -9;
  b_nSignalBJets = -9;

  b_MET_pt = -99.;
  b_MET_phi = -99.;

  b_passMETFilters = false;
}

XMuMuTreeMaker::~XMuMuTreeMaker(){} 