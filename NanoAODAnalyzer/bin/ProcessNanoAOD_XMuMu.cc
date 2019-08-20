#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <unordered_map>

#include <TROOT.h>
#include <TFile.h>
#include "TSystem.h"
#include "TChain.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"

#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "XMuMuAnalysis/NanoAODAnalyzer/interface/XMuMuTreeMaker.h"

using std::unordered_map;
using namespace XMuMuAnalysis;

float GetMCXS(std::string);
float GetMCkFactor(std::string);

int main(int argc, char * argv[])
{
  gSystem->Load( "libFWCoreFWLite" );
  FWLiteEnabler::enable();

  // parse arguments
  if ( argc < 4 ) 
  {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << " [sampleName] " << " [sampleNameForXS] " << std::endl;
    return 0;
  }

  //
  // get the python configuration
  //
  if( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ){
    std::cout << " ERROR: ParametersSet 'process' is missing in your configuration file" << std::endl; exit(0);
  }  
  const edm::ParameterSet& process    = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");
  //
  //
  //
  const edm::ParameterSet& parameters = process.getParameter<edm::ParameterSet>("ProcessNanoAOD_XMuMu"); 
  std::string inputListDir     = parameters.getParameter<std::string>("inputListDir");
  std::string outputListDir    = parameters.getParameter<std::string>("outputListDir");
  int maxEvents = parameters.getParameter<int>("maxEvents");
  bool debug    = parameters.getParameter<bool>("debug");
  bool isMC     = parameters.getParameter<bool>("isMC");
  std::string era = parameters.getParameter<std::string>("era");
  bool blind = false;
  //
  
  //
  //lumiMask: Only for data
  // 
  const edm::ParameterSet& inputs = process.getParameter<edm::ParameterSet>("inputs");   
  std::vector<edm::LuminosityBlockRange> lumiMask;
  if( !isMC && inputs.exists("lumisToProcess") ){
    std::vector<edm::LuminosityBlockRange> const & lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
    lumiMask.resize( lumisTemp.size() );
    copy( lumisTemp.begin(), lumisTemp.end(), lumiMask.begin() );
  }
  if(debug) for(auto lumiID: lumiMask) std::cout<<"lumiID "<<lumiID<<std::endl;

  //
  // NanoAOD input source
  // Get list of root files from txt file
  //
  std::string sampleName  = std::string(argv[2]); 
  std::string txtFileName = inputListDir + sampleName + ".txt";
  std::ifstream infile(txtFileName);
  std::cout<< txtFileName <<std::endl;
  
  TChain* eventsTree     = new TChain("Events");
  TChain* lumiBlocksTree = new TChain("LuminosityBlocks");
  
  for(std::string line; getline( infile, line ); ){
    //
    // Make the filename
    //
    std::string fileName = line;
    std::cout << "Input File: " << fileName.c_str() << std::endl;
    int e = eventsTree     ->AddFile(fileName.c_str());
    int l = lumiBlocksTree ->AddFile(fileName.c_str());
    if(e!=1 || l!=1){ std::cout << "ERROR1" << std::endl; return 1;}
    if(debug) std::cout<<"Added to TChain"<<std::endl;
  }
  
  //
  // Get total sumofweights
  //
  std::string sampleNameForXS           = std::string(argv[3]); 
  std::string txtFileNameSumOfWeights = inputListDir + sampleNameForXS + ".txt";
  std::ifstream infileSumOfWeights(txtFileNameSumOfWeights);
  std::cout<< txtFileNameSumOfWeights <<std::endl; 
  TChain* runsTree       = new TChain("Runs");
  for(std::string line; getline( infileSumOfWeights, line ); ){
    //
    // Make the filename
    //
    std::string fileName = line;
    std::cout << "Input File: " << fileName.c_str() << std::endl;
    int r = runsTree       ->AddFile(fileName.c_str());
    if(r!=1){ std::cout << "ERROR2" << std::endl; return 1;}
    if(debug) std::cout<<"Added to TChain"<<std::endl;
  }

  float MCXSec = GetMCXS(sampleNameForXS);
  float MCkFactor = GetMCkFactor(sampleNameForXS);
  bool  isSignal = false;

  if (sampleName.find("MSSM") != std::string::npos) {
    isSignal = true;
    std::cout << "This is a signal sample." << std::endl;
  }

  std::string outFileName      = "Ntuple_"+sampleName+".root";
  std::string outFileNameFinal = outputListDir + outFileName;

  XMuMuTreeMaker* treeMaker = new XMuMuTreeMaker(eventsTree,runsTree,lumiBlocksTree, isMC, isSignal, blind, era, debug);
  treeMaker->SetMCXS(MCXSec);
  treeMaker->SetMCkFactor(MCkFactor);
  treeMaker->SetLumiMask(lumiMask);
  treeMaker->CreateNtupleTree(outFileNameFinal);
  treeMaker->EventLoop(maxEvents);
  treeMaker->StoreNtupleTree();
  delete treeMaker;

  std::cout << "ProcessNanoAOD_XMuMu::DONE" << std::endl;

  return 0;
}

float GetMCXS(std::string sampleName){
  //
  // Cross-sections should be in picobarn (pb)
  //
  std::unordered_map<std::string, float> m_sampleXS;   
  //https://github.com/jkarancs/B2GTTrees/blob/cdc6ee2e2423ae3e3b67c5291ae25fdd2d42c4de/test/crab3/cross_sections.txt#L133
  m_sampleXS["MC16_DYLL"] = 4895;      
  //https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO#Top_quark_pair_cross_sections_at  
  //BR for W-boson from PDG(2018)
  m_sampleXS["MC16_TTJets_DiLept"] = 831.76*(3*0.1086)*(3*0.1086);               

  float xsec = 1.0;
  if ( m_sampleXS.find(sampleName) == m_sampleXS.end() ) {
    xsec = 1.0;
  } else {
    xsec = m_sampleXS[sampleName];
  }
  return xsec;
}
//
// NOTE: Dummy at the moment
//
float GetMCkFactor(std::string sampleName){
  std::unordered_map<std::string, float> m_samplekFact; 
  //https://github.com/jkarancs/B2GTTrees/blob/cdc6ee2e2423ae3e3b67c5291ae25fdd2d42c4de/test/crab3/cross_sections.txt#L133
  m_samplekFact["MC16_DYLL"] = 1.23;
  float kFact = 1.0;
  if ( m_samplekFact.find(sampleName) == m_samplekFact.end() ) {
    kFact = 1.0;
  } else {
    kFact = m_samplekFact[sampleName];
  }
  return kFact;
}