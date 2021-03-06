#include "XMuMuAnalysis/NanoAODAnalyzer/interface/EventAna.h"

using namespace XMuMuAnalysis;

EventAna::EventAna(TChain* t, bool mc, std::string era, bool isSig, bool d){
  std::cout << "EventAna::EventAna()" << std::endl;
  tree     = t;
  isMC     = mc;
  eraName  = era;
  isSignal = isSig;
  debug    = d;

  InitBranch(tree, "HLT_IsoMu24",   HLT_IsoMu24);
  InitBranch(tree, "HLT_IsoTkMu24", HLT_IsoTkMu24);
  InitBranch(tree, "HLT_IsoMu27",   HLT_IsoMu27);
  InitBranch(tree, "HLT_IsoTkMu27", HLT_IsoTkMu27);
  InitBranch(tree, "HLT_Mu50",      HLT_Mu50);
  InitBranch(tree, "HLT_TkMu50",    HLT_TkMu50);
  InitBranch(tree, "HLT_OldMu100",  HLT_OldMu100);//
  InitBranch(tree, "HLT_TkMu100",   HLT_TkMu100);//
  //
  //
  // https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2#2018_data
  // https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2#2016_data
  // https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2#2017_data
  //
  //
  // 
  InitBranch(tree,"Flag_goodVertices",                       Flag_goodVertices);//2016,2017
  InitBranch(tree,"Flag_globalSuperTightHalo2016Filter",     Flag_globalSuperTightHalo2016Filter);//2016,2017
  InitBranch(tree,"Flag_HBHENoiseFilter",                    Flag_HBHENoiseFilter);//2016,2017
  InitBranch(tree,"Flag_HBHENoiseIsoFilter",                 Flag_HBHENoiseIsoFilter);//2016,2017
  InitBranch(tree,"Flag_EcalDeadCellTriggerPrimitiveFilter", Flag_EcalDeadCellTriggerPrimitiveFilter);//2016,2017
  InitBranch(tree,"Flag_BadPFMuonFilter",                    Flag_BadPFMuonFilter);//2016,2017
  InitBranch(tree,"Flag_eeBadScFilter",                      Flag_eeBadScFilter);//2016,2017

  std::cout << "EventAna::EventAna() Initialize event reader" << std::endl;
  readerEvent       = new EventReader(tree, isMC, eraName);

  //
  // Turn-off rochester corrections now
  // Seems that the analysis don't use this.
  //
  std::cout << "EventAna::EventAna() Initialize object readers" << std::endl;
  bool doRoccorForMuons = false; 
  readerMuons       = new RecoMuonReader("Muon",        tree, isMC, eraName,doRoccorForMuons);
  readerJets        = new RecoJetReader("Jet",          tree, isMC, eraName);
  readerMET         = new RecoMETReader("MET",          tree, isMC, eraName);
  readerTrigObjects = new TrigObjectReader("TrigObj",   tree);
  if(isMC){
    readerGenParticles = new GenParticleReader("GenPart",   tree);
  }
} 
bool EventAna::LoadEventFromTTree(int e)
{
  if(debug){
    std::cout<<"Get Entry "<<e<<std::endl;
    std::cout<<tree->GetCurrentFile()->GetName()<<std::endl;
    tree->Show(e);
  }

  Long64_t loadStatus = tree->LoadTree(e);
  
  if(loadStatus<0){
    std::cout << "Error "<<loadStatus<<" getting event "<<e<<std::endl; 
    return false;
  }
  tree->GetEntry(e);
  
  if(debug) std::cout<<"Got Entry "<<e<<std::endl;
  
  if(debug) std::cout<<"Reset EventAna"<<std::endl;

  recoMuons.clear();
  recoJets.clear();
  signalJets.clear();
  signalBJets.clear();
  signalLightJets.clear();
  trigObjectsMuon.clear();

  run       =  0;
  lumiBlock =  0;
  event     =  0;
  nPVs      =  0;
  nPVsGood  =  0;
  genWeight =  1.0;


  nRecoMuons = 0;
  muon0 = nullptr;
  muon1 = nullptr;

  trigMuon0  = nullptr;
  trigMuon1  = nullptr;

  oppSign   = false;

  mu0SF = 1.0;
  mu1SF = 1.0;

  mu0RecoJetIdx = -1;
  mu1RecoJetIdx = -1;

  mu0IsTrigMatch = false;
  mu1IsTrigMatch = false;

  nRecoJets = 0;
  nSignalJets = 0;
  nSignalBJets = 0;
  nSignalLightJets = 0;

  passPreselTriggers = false;
  passPreselMuons  = false;
  
  passMuMu              = false;
  passMuMuTightIdIso    = false;
  passMuMuHighPtIdIso   = false;
  
  passTrigTightHLT    = false;
  passTrigHighPtHLT   = false;
  passTrigTightMatch  = false;
  passTrigHighPtMatch = false;
  passTrigger         = false;
  passMETFilters      = false;

  tlv_MuMu          = TLorentzVector();
  tlv_MuMu_Mu0      = TLorentzVector();
  tlv_MuMu_Mu1      = TLorentzVector();
  
  MET_pt = -99.;
  MET_phi = -99.;

  //
  // Load objects from NanoAOD
  //
  readerMuons->GetRecoMuonsFromNano();
  readerJets->GetRecoJetsFromNano();
  readerTrigObjects->GetTrigObjectsFromNano();

  return true;
}
bool EventAna::PassPreselection()
{
  if(debug) std::cout<<"Do PassPreselection()"<<std::endl;
  //
  // Get
  //
  run = readerEvent->run;
  lumiBlock = readerEvent->lumiBlock;
  event = readerEvent->event;
  nPVs = readerEvent->nPVs;
  nPVsGood = readerEvent->nPVsGood;
  genWeight = readerEvent->genWeight;
  Pileup_nTrueInt = readerEvent->Pileup_nTrueInt;
  //
  // Must have at least one PV
  //
  if(nPVsGood < 1 && !isSignal) {
    if(debug) std::cout<<"Fail nPVsGood. nPVsGood = "<< nPVsGood <<std::endl;
    return false;
  }
  // 
  // Pass any of the single-muon triggers
  //
  if(eraName=="2016"){
    passPreselTriggers = HLT_IsoMu24 || HLT_IsoTkMu24 || HLT_Mu50 || HLT_TkMu50;
  }
  if (!passPreselTriggers && !isSignal) {
    if(debug) std::cout<<"Fail passPreselTriggers. passPreselTriggers = "<< passPreselTriggers <<std::endl;
    return false;
  }
  // 
  // Pass >=2 muons multiplicity requirement
  //
  // NOTE:
  // Let us apply only pt and eta cut. We'll check their ID and ISO later 
  //
  recoMuons      = readerMuons->SelectRecoMuons(muonPtMin,muonEtaMax,-1,-1);
  nRecoMuons     = recoMuons.size();

  passPreselMuons = (nRecoMuons >= 2);
  if (!passPreselMuons && !isSignal) return false;

  if(debug) std::cout<<"Done PassPreselection()"<<std::endl;
  return true;
}
bool EventAna::ConstructEventHypothesis()
{
  if(debug) std::cout<<"Do ConstructEventHypothesis()"<<std::endl;
  //################################################
  //
  // X->MuMu reconstruction
  //
  //#################################################
  if(debug) std::cout<<"Do XMuMu Reconstruction"<<std::endl;
  //
  // Trigger match the muons
  //
  trigObjectsMuon      = readerTrigObjects->SelectTrigObjects(0.0, 1e6, TrigObjectID::Muon);
  HelperFunctions::MatchTrigObjectsMuon(recoMuons,trigObjectsMuon,trigMatchDR);

  //
  // Form the MuMu candidate
  //
  if (nRecoMuons >= 2){
    if(debug){
      std::cout<<"At-least 2 muons"<<std::endl;
      std::cout<<"Take the 2 highest pT muons"<<std::endl;
    }
    muon0 = recoMuons.at(0);
    muon1 = recoMuons.at(1);
    tlv_MuMu_Mu0 = muon0->p4;
    tlv_MuMu_Mu1 = muon1->p4;
    tlv_MuMu     = tlv_MuMu_Mu0 + tlv_MuMu_Mu1;

    oppSign = (muon0->charge * muon1->charge) < 0; 

    passMuMu = true;
    
    mu0RecoJetIdx = muon0->jetIdx;
    mu1RecoJetIdx = muon1->jetIdx;
  }
  else{
    passMuMu = false;
  }
  // MuMu must be opposite sign
  passMuMu = passMuMu && oppSign;
  // MuMu mass must be more than 70.
  passMuMu = passMuMu && (tlv_MuMu.M() > 70.);

  //
  // Check if muons passes "Tight" criteria
  //
  if(passMuMu){
    passMuMuTightIdIso = true; // Assume it passes
    //
    passMuMuTightIdIso &= muon0->p4.Pt() > 26.;
    passMuMuTightIdIso &= abs(muon0->p4.Eta()) < 2.4;
    passMuMuTightIdIso &= muon0->pfRelIso04_all < 0.25; 
    //
    passMuMuTightIdIso &= muon1->p4.Pt() > 26.;
    passMuMuTightIdIso &= abs(muon1->p4.Eta()) < 2.4;
    passMuMuTightIdIso &= muon1->pfRelIso04_all < 0.25; 
  }
  //
  // Check if muons passes "HighPt" criteria
  //
  if(passMuMu){
    passMuMuHighPtIdIso = true; // Assume it passes
    //
    passMuMuHighPtIdIso &= muon0->p4.Pt() > 53.;
    passMuMuHighPtIdIso &= abs(muon0->p4.Eta()) < 2.4;
    passMuMuHighPtIdIso &= muon0->tkRelIso < 0.1; 
    //
    passMuMuHighPtIdIso &= muon1->p4.Pt() > 53.;
    passMuMuHighPtIdIso &= abs(muon1->p4.Eta()) < 2.4;
    passMuMuHighPtIdIso &= muon1->tkRelIso< 0.1; 
  }

  if(debug) std::cout<<"Done XMuMu Reconstruction"<<std::endl;
  //################################################
  //
  // Trigger
  //
  //#################################################
  if(debug) std::cout<<"Do Trigger"<<std::endl;

  //
  // Check if event fires "Tight" triggers
  //
  if(passMuMu){
    if(eraName=="2016"){
      passTrigTightHLT = HLT_IsoMu24 || HLT_IsoTkMu24;
    }
  }
  //
  // Check if event fires "HighPt" triggers
  //
  if(passMuMu){
    if(eraName=="2016"){
      passTrigHighPtHLT = HLT_Mu50 || HLT_TkMu50;
    }
  }
  //
  // Check if offline muons are trigger matched to HLT. 
  // Retrieve the HLT muons if they're matched
  // The checked if they are the muons that fire the trigger
  //
  if(passMuMu){
    //
    // Check if offline muons match to HLT muons
    //
    if(muon0->isTrigMatch) {
      mu0IsTrigMatch = true;
      trigMuon0 = muon0->trigMuon;
    }
    if(muon1->isTrigMatch) {
      mu1IsTrigMatch = true;
      trigMuon1 = muon1->trigMuon;
    }
    //
    // Check if matched HLT muons fire the required triggers
    // TODO:Fikri: How do we EXACTLY check that if a HLT
    // muon fires a specific trigger? Is it simply just 
    // the pt cut?
    //
    if(eraName=="2016"){
      passTrigTightMatch = false;
      if(trigMuon0) passTrigTightMatch |= trigMuon0->pt > 24;
      if(trigMuon1) passTrigTightMatch |= trigMuon1->pt > 24;
    }
    if(eraName=="2016"){
      passTrigHighPtMatch = false;
      if(trigMuon0) passTrigHighPtMatch |= trigMuon0->pt > 50;
      if(trigMuon1) passTrigHighPtMatch |= trigMuon1->pt > 50;
    }
  }

  //
  // passTrigger is true only if fire any HLT and any of the muon is trig match
  //
  passTrigger = false;
  passTrigger |= (passTrigTightHLT  && passTrigTightMatch);
  passTrigger |= (passTrigHighPtHLT && passTrigHighPtMatch);

  if(debug) std::cout<<"Done Trigger"<<std::endl;
  //##############################################################
  //
  // Select jets
  //
  //##############################################################
  if(debug) std::cout<<"Do RecoJet selection "<<std::endl;
  //
  // Get jets that passes pT and eta cut
  //
  recoJets = readerJets->SelectRecoJets(jetPtMin, jetEtaMax, doJetCleaning);
  //
  // Get jets that do not overlap with the two muons
  //
  recoJets  = readerJets->SelectRecoJetsLepCleaned(recoJets,mu0RecoJetIdx,mu1RecoJetIdx);
  nRecoJets = recoJets.size();

  //##############################################################
  // Small-R jets
  //##############################################################
  bool isSignalJet  = false;
  bool passJetID    = false;

  for (unsigned int i = 0; i < nRecoJets; i++)
  {
    RecoJetPtr jet = recoJets.at(i);
    passJetID   = jet->jetId & 1;
    if(eraName=="2016"){
      passJetID   = jet->jetId&1;
    }else{
      passJetID   = jet->jetId&2;
    }
    // Check if it is central jet
    isSignalJet = passJetID; // passID
    if(isSignalJet) signalJets.push_back(jet);
  }
  nSignalJets  = signalJets.size();

  //
  // Get b-jets and light-jets
  //
  for (unsigned int i=0; i < nSignalJets; i++){
    RecoJetPtr jet = signalJets.at(i);
    if(jet->CSVv2 > jetCSVv2Cut){
      signalBJets.push_back(jet);
    }else{
      signalLightJets.push_back(jet);
    }
  }
  nSignalBJets     = signalBJets.size();
  nSignalLightJets = signalLightJets.size();
  //##############################################################
  //
  // Get MET variables
  //
  //##############################################################
  MET_pt  = readerMET->pt;
  MET_phi = readerMET->phi;
  //
  // Assume event passMETFilters
  //
  passMETFilters = true;
  if(eraName=="2016"){
    passMETFilters &= Flag_goodVertices;
    passMETFilters &= Flag_globalSuperTightHalo2016Filter;
    passMETFilters &= Flag_HBHENoiseFilter;
    passMETFilters &= Flag_HBHENoiseIsoFilter;
    passMETFilters &= Flag_EcalDeadCellTriggerPrimitiveFilter;
    passMETFilters &= Flag_BadPFMuonFilter;
    if(!isMC){
      passMETFilters &= Flag_eeBadScFilter;
    }
  }
  else if(eraName=="2017"){
    passMETFilters &= Flag_goodVertices;
    passMETFilters &= Flag_globalSuperTightHalo2016Filter;
    passMETFilters &= Flag_HBHENoiseFilter;
    passMETFilters &= Flag_HBHENoiseIsoFilter;
    passMETFilters &= Flag_EcalDeadCellTriggerPrimitiveFilter;
    passMETFilters &= Flag_BadPFMuonFilter;
    if(!isMC){
      passMETFilters &= Flag_eeBadScFilter;
    }
  }

  return true;
}
EventAna::~EventAna(){} 