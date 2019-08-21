#include "TString.h"
#include "TVector2.h"
#include <map>
#include <vector>

using namespace std;

struct Variable{
  string varNameInTree;
  string xAxisName;
  string yAxisName;
  bool doLogY;
  int nbins; double xmin; double xmax;
  vector<double> rebins; 
};
map<string, Variable> GetMapOfVariables()
{ 
  int etabins = 120;
  double etamin = -6.0;
  double etamax = 6.0;

  int phibins = 80;
  double phimin = -4.0;
  double phimax = 4.0;

  int    njetbins = 12;
  double njetmin = 0.;
  double njetmax = 12.;

  map<string, Variable> variables;

  variables["neventsweighted"] = {
    "Count",
    "Event Count", 
    "Events", 
    false,
    1,  0,  2,
    {}
  };  
  variables["npv"] = {
    "nPVsGood",
    "Number of PVs",          
    "Events", 
    false,
    80, 0, 80,
    {}
  };  
  variables["mumu_pt"]  = {
    "mumu_pt",
    "Di-Muon p_{T} [GeV]",  
    "Events", 
    true,
    120, 0.0, 1200.0,
    {}
  };
  variables["mumu_eta"] = {
    "mumu_eta",
    "Di-Muon #eta",         
    "Events", 
    false,
    etabins,etamin,etamax,
    {}
  };
  variables["mumu_phi"] = {
    "mumu_phi",
    "Di-Muon #phi",         
    "Events", 
    false,
    phibins,phimin,phimax,
    {}
  };
  variables["mumu_mass"] = {
    "mumu_mass",
    "Di-Muon Mass [GeV]",   
    "Events", 
    true,
    100, 50.0, 250.0,
    {}
  };
  variables["mumu_mass2"] = {
    "mumu_mass",
    "Di-Muon Mass [GeV]",   
    "Events", 
    false,
    70, 70.0, 140.0,
    {}
  };
  variables["mumu_mass3"] = {
    "mumu_mass",
    "Di-Muon Mass [GeV]",   
    "Events", 
    true,
    95, 50.0, 1000.0,
    {}
  };
  variables["mumu_mu0pt"] = {
    "mumu_mu0pt",
    "1st Leading Muon p_{T} [GeV]",  
    "Events",
    true,
    100, 0.0, 500.0, 
    {}
  };
  variables["mumu_mu0eta"]   = {
    "mumu_mu0eta",
    "1st Leading Muon #eta",         
    "Events", 
    false,
    etabins,etamin,etamax,
    {}
  };
  variables["mumu_mu0phi"]   = {
    "mumu_mu0phi",
    "1st Leading Muon #phi",  
    "Events", 
    false,
    phibins,phimin,phimax,
    {}
  };
  variables["mumu_mu1pt"] = {
    "mumu_mu1pt",
    "2nd Leading Muon p_{T} [GeV]",  
    "Events", 
    true,
    100, 0.0, 500.0,
    {}
  };
  variables["mumu_mu1eta"]   = {
    "mumu_mu1eta",
    "2nd Leading Muon #eta",         
    "Events", 
    false,
    etabins,etamin,etamax,
    {}
  };
  variables["mumu_mu1phi"]   = {
    "mumu_mu1phi",
    "2nd Leading Muon #phi",  
    "Events", 
    false,
    phibins,phimin,phimax,
    {}
  };
  variables["met_pt"]   = {
    "MET_pt",
    "MET [GeV]",  
    "Events", 
    true,
    30,0.,300.,
    {}
  };
  variables["met_phi"]   = {
    "MET_phi",
    "MET #phi",  
    "Events", 
    false,
    phibins,phimin,phimax,
    {}
  };
  variables["nrecojets"] = {
    "nRecoJets",
    "Jet Multiplicity",
    "Events", 
    true,
    njetbins,njetmin,njetmax,
    {}
  };  
  variables["nrecobjets"] = {
    "nRecoBJets",
    "B-Jet Multiplicity",
    "Events", 
    true,
    njetbins,njetmin,njetmax,
    {}
  };  
  variables["nrecolightjets"] = {
    "nRecoLightJets",
    "Light-Jet Multiplicity",
    "Events", 
    true,
    njetbins,njetmin,njetmax,
    {}
  };  
  return variables;
}


