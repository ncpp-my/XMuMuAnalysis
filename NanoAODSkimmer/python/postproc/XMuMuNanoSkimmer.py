import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
import sys

from PhysicsTools.NanoAODTools.postprocessing.framework.datamodel import Collection,Object
from PhysicsTools.NanoAODTools.postprocessing.framework.eventloop import Module
from PhysicsTools.NanoAODTools.postprocessing.tools import * #deltaR, matching etc..

class XMuMuNanoSkimmer(Module):
  def __init__(self, isMC, era, doSkim=False):
    self.era = era
    self.isMC = isMC
    self.doSkim = doSkim
    self.writeHistFile=True

  def beginJob(self,histFile=None,histDirName=None):
    Module.beginJob(self,histFile,histDirName)
    self.h_cutflow_uw = ROOT.TH1F('h_cutflow_uw','h_cutflow_uw', 6, 0, 6)
    self.h_cutflow_w  = ROOT.TH1F('h_cutflow_w', 'h_cutflow_w',  6, 0, 6)
    self.h_cutflow_uw.GetXaxis().SetBinLabel(1,"Cut0:NoSelection")
    self.h_cutflow_uw.GetXaxis().SetBinLabel(2,"Cut1:>=2LooseMuon")
    self.h_cutflow_w.GetXaxis().SetBinLabel(1,"Cut0:NoSelection")
    self.h_cutflow_w.GetXaxis().SetBinLabel(2,"Cut1:>=2LooseMuon")

    self.addObject(self.h_cutflow_uw)
    self.addObject(self.h_cutflow_w)
  
  def endJob(self):
    Module.endJob(self)

  def beginFile(self, inputFile, outputFile, inputTree, wrappedOutputTree):
    pass

  def endFile(self, inputFile, outputFile, inputTree, wrappedOutputTree):
    pass

  def RegisterCut(self, cutName, weight):
    self.h_cutflow_uw.Fill(cutName,1)
    self.h_cutflow_w.Fill(cutName, weight)

  def analyze(self, event):
    """process event, return True (go to next module) or False (fail, go to next event)"""
    muons     = list(Collection(event, "Muon"))

    evtweight = 1.0
    if self.isMC:
      evtweight = event.genWeight

    #################################################
    # Register cut before ANY selection
    ##################################################
    self.RegisterCut("Cut0:NoSelection",evtweight)

    #################################################
    #
    # Muon selection
    #
    ##################################################
    #
    # Select muons
    #
    muonsLoose = []
    for mu in muons:
      #
      # Loose selection
      #
      if mu.pt < 20. or abs(mu.eta) > 2.4: continue
      if mu.looseId is False: continue
      muonsLoose.append(mu) 
    #
    # CHECK: AT LEAST 2 Loose muon.
    # Skip event if doSkim=True
    #
    if len(muonsLoose) < 2:
      if self.doSkim: 
        return False
    else:
      self.RegisterCut("Cut1:>=2LooseMuon",evtweight)

    return True

# define modules using the syntax 'name = lambda : constructor' to avoid having them loaded when not needed
XMuMuNanoSkimmer_2016_mc_sig   = lambda : XMuMuNanoSkimmer(isMC=True, era="2016",doSkim=False) 
XMuMuNanoSkimmer_2016_mc_bkgd  = lambda : XMuMuNanoSkimmer(isMC=True, era="2016",doSkim=True) 
XMuMuNanoSkimmer_2016_data     = lambda : XMuMuNanoSkimmer(isMC=False,era="2016",doSkim=True) 




