#include "XMuMuAnalysis/NanoAODAnalyzer/interface/EventReader.h"

using namespace XMuMuAnalysis;

EventReader::EventReader(TChain* t, bool mc, std::string era, bool isSig, bool d){
  std::cout << "EventReader::EventReader()" << std::endl;
  tree     = t;
  isMC     = mc;
  eraName  = era;
  isSignal = isSig;
  debug    = d;

  std::cout << "EventReader::EventReader() tree->LoadTree(0)" << std::endl;
  tree->LoadTree(0);

  InitBranch(tree, "run",             run);
  InitBranch(tree, "luminosityBlock", lumiBlock);
  InitBranch(tree, "event",           event);
  InitBranch(tree, "PV_npvs",         nPVs);
  InitBranch(tree, "PV_npvsGood",     nPVsGood);
  InitBranch(tree, "genWeight",       genWeight);
  //
  //
  //
  InitBranch(tree, "HLT_IsoMu24",   HLT_IsoMu24);
  InitBranch(tree, "HLT_IsoTkMu24", HLT_IsoTkMu24);
  InitBranch(tree, "HLT_IsoMu27",   HLT_IsoMu27);
  InitBranch(tree, "HLT_IsoTkMu27", HLT_IsoTkMu27);
  InitBranch(tree, "HLT_Mu50",      HLT_Mu50);
  InitBranch(tree, "HLT_TkMu50",    HLT_TkMu50);

  std::cout << "EventReader::EventReader() Initialize objects" << std::endl;

  readerMuons       = new RecoMuonReader("Muon",        tree, isMC, eraName);
  readerJets        = new RecoJetReader("Jet",          tree, isMC, eraName);
  readerMET         = new RecoMETReader("MET",          tree, isMC, eraName);
  readerTrigObjects = new TrigObjectReader("TrigObj",   tree);
  if(isMC){
    readerGenParticles = new GenParticleReader("GenPart",   tree);
  }
} 
bool EventReader::LoadEventFromTTree(int e)
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
  
  if(debug) std::cout<<"Reset EventReader"<<std::endl;
  
  recoMuons.clear();
  recoJets.clear();
  recoBJets.clear();
  recoLightJets.clear();
  trigObjectsMuon.clear();

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

  nRecoJets  = 0;
  nRecoBJets = 0;
  nRecoLightJets = 0;

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
bool EventReader::PassPreselection()
{
  if(debug) std::cout<<"Do PassPreselection()"<<std::endl;
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
bool EventReader::ConstructEventHypothesis()
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
    passMuMuHighPtIdIso &= muon0->pfRelIso04_all < 0.25; 
    //
    passMuMuHighPtIdIso &= muon1->p4.Pt() > 53.;
    passMuMuHighPtIdIso &= abs(muon1->p4.Eta()) < 2.4;
    passMuMuHighPtIdIso &= muon1->pfRelIso04_all < 0.25; 
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
  //
  // Get b-jets and light-jets
  //
  for (unsigned int i=0; i < nRecoJets; i++){
    RecoJetPtr jet = recoJets.at(i);
    if(jet->CSVv2 > jetCSVv2Cut){
      recoBJets.push_back(jet);
    }else{
      recoLightJets.push_back(jet);
    }
  }
  nRecoBJets     = recoBJets.size();
  nRecoLightJets = recoLightJets.size();
  //##############################################################
  //
  // Get MET variables
  //
  //##############################################################
  MET_pt  = readerMET->pt;
  MET_phi = readerMET->phi;

  return true;
}
EventReader::~EventReader(){} 