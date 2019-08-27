#include "Helpers.h"
#include "Helpers_Vars.h"
#include "Helpers_Samples.h"
#include "TH1.h"
#include <iomanip>

#include "tdrstyle.C"
using namespace std;

//
// Path to rootfiles containing the histograms
//
TString dirHistos;

map<string,TFile*> mapToTFiles;

void DataMC(string cutLevel="mumu_passPresel", string variableName="zcand_pt")
{

  
  //
  // Get variable info
  //
  map<string, Variable> variables;
  variables = GetMapOfVariables();

  if ( variables.find(variableName) == variables.end() ) 
  {
    cout << "Can't find variable in map. variableName = " << variableName << ". Exiting!!!"<< endl;
    return; 
  } 
  Variable variable = variables[variableName];
  string varName    = variableName;
  string xaxisName  = variable.xAxisName;
  string yaxisName  = variable.yAxisName;
  bool doLogY       = variable.doLogY;

  //
  // Specify name of plot that serves as a yield plot.
  //
  bool isNormPlot = false;
  isNormPlot = variableName == "neventsweighted";
  
  //
  // construct histoName
  //
  string histoName = "h_"+cutLevel+"_"+varName;

  //==============================================================================
  //
  // Get histogram for all samples. Merge if they have subsamples
  //
  //==============================================================================
  map<string, Sample*> samples;
  samples = GetMapOfSamples();

  map<string, Sample*>::iterator it;
  for ( it = samples.begin(); it != samples.end(); it++)
  { 
    string  key    = it->first;
    Sample* sample = it->second;
    vector<string>  subsamples = sample->subsamples;
    
    THStack* stack = new THStack("h_"+TString(key.c_str()),"");
    
    for(vector<string>::iterator itS = subsamples.begin(); itS != subsamples.end(); ++itS) {
      TFile* file = mapToTFiles[(*itS)];
      TH1D*  h    = (TH1D*) file->Get(histoName.c_str());
      TH1D*  hC   = (TH1D*) h->Clone(h->GetName() + TString("_C"));
      stack->Add(hC);
    }
    sample->h = (TH1D*) stack->GetStack()->Last()->Clone(histoName + "_" + TString(key.c_str()));
    delete stack;
  }

  //==============================================================================
  //
  // Data
  //
  //==============================================================================
  cout <<"Processing data " << endl;
  //
  string dataName = "Data16";
  //
  // Data histogram
  //
  TH1D* h_data = nullptr;
  h_data = samples[dataName]->h;
  SetDataStyle(h_data);
  //
  // Data yield
  //
  float yield_data = h_data->Integral(0, 1000);
  std::cout << dataName << " = " << yield_data << endl;
  cout <<"Data Processed : " << dataName << ", Yields = " << yield_data << ", Nbins = " << h_data->GetNbinsX() << endl;

  
  //==============================================================================
  //
  // MC Backgrounds
  //
  //==============================================================================
  map<string, float > bkgdYields;

  vector<string> bkgdNames;
  bkgdNames.push_back("MC16_DYLL"); 
  bkgdNames.push_back("MC16_TOP");
  bkgdNames.push_back("MC16_VV"); 


  THStack* stack_bkgd = new THStack("stack_bkgd","");
  for(unsigned int iBkgd = 0; iBkgd < bkgdNames.size(); iBkgd++ )
  {
    string bkgdName = bkgdNames[iBkgd];

    TH1D* h_bkgd = nullptr;
    h_bkgd = samples[bkgdName]->h;
    //
    // Cosmetics
    //
    h_bkgd->SetFillColor(samples[bkgdName]->color + 1);
    h_bkgd->SetLineColor(kBlack);
    //
    // Get yields
    //
    bkgdYields[bkgdName] = h_bkgd->Integral(0, 1000);
    //
    // Get histogram and add the to the stack
    //
    stack_bkgd->Add(h_bkgd);
    //
    cout <<"Bkgd Processed : " << bkgdName << ", Yields = " << bkgdYields[bkgdName] << ", Entries = " << h_bkgd->GetEntries() << ", Nbins = " << h_bkgd->GetNbinsX() << endl;
  }


  //==============================================================================
  //
  // Total Background Template
  //
  //==============================================================================
  TH1D* h_totalbkgd = (TH1D*) stack_bkgd->GetStack()->Last()->Clone(histoName + TString("_MC16_BKGD"));
  float yield_totalbkgd = h_totalbkgd->Integral(0, 200);

  bkgdYields["MC16_BKGD"] = yield_totalbkgd;
  cout <<"Total Bkgd Yield = " << yield_totalbkgd << endl;
  
  //
  // Calculate the fraction of each background wrt to total background
  //
  vector< pair<string, float > > bkgdFractions;

  for(int iBkgd = 0; iBkgd < bkgdNames.size(); iBkgd++ )
  {
    string bkgdName = bkgdNames[iBkgd];
    if (bkgdYields.find(bkgdName) != bkgdYields.end())
    {
      float yield = bkgdYields[bkgdName];
      pair<string, float> sampleFraction = make_pair(bkgdName, yield/yield_totalbkgd);
      bkgdFractions.push_back(sampleFraction);    
    }
  }
  
  //
  // Make new background stack. Stack from lowest to highest
  //
  sort(bkgdFractions.begin(), bkgdFractions.end(), sortbysecDescending);
  THStack* stack_bkgd_sorted = new THStack("stack_bkgd_sorted","");
  
  for(int iBkgd = 0; iBkgd < bkgdFractions.size(); iBkgd++ )
  {
    pair<string, float> bkgdFraction = bkgdFractions.at(iBkgd);
    string bkgdName = bkgdFraction.first;
    TH1D* hist = samples[bkgdName]->h;
    stack_bkgd_sorted->Add(hist);
  } 
  TH1D* h_totalbkgd_sorted = (TH1D*) stack_bkgd_sorted->GetStack()->Last()->Clone(histoName + TString("_MC16_BKGDSORTED"));
  float yield_totalbkgd_sorted = h_totalbkgd_sorted->Integral(0, 200);

  std::cout << "Data/Pred = " << yield_data/yield_totalbkgd_sorted << endl;
  
  //===============================================================================================================
  //
  // Creating the uncertainty bands 
  //
  //===============================================================================================================
  //
  // Make stat only error band
  //
  TGraphAsymmErrors* errorband_stat = makeSystBand(h_totalbkgd_sorted, {});
  errorband_stat->SetFillColor( kBlack );
  errorband_stat->SetFillStyle( 3245 );
  errorband_stat->SetLineColor( kWhite );
  errorband_stat->SetMarkerStyle( 1 );

  //=======================================
  //
  // Make canvas
  //
  //=======================================
  TCanvas* canv  =  MakeBoxCanvas_WithRatioPad("canv");
  TPad* pad1 = GetPadFromCanvas(canv, "pad1");
  TPad* pad2 = GetPadFromCanvas(canv, "pad2");
  
  //=======================================
  //
  // Upper pad
  //
  //=======================================
  pad1->cd();

  float xLat = 0.2;
  float yLat = 0.92;
  float xLeg = xLat + 0.28;
  float yLeg = yLat;
  //
  // Make legend. Add from biggest to smallest
  //
  float leg_h =  0.031 * (1 + bkgdNames.size()); // Data + Bkgds
 if(isNormPlot) leg_h =  0.075 * (1 + bkgdNames.size()); // Data + Bkgds

  TLegend* leg =  MakeLegend(xLeg, yLeg - leg_h, xLeg + 0.425, yLeg );
  leg->SetNColumns( 2 );

  leg->SetFillStyle( 0 );
  leg->SetBorderSize( 0 );
  leg->SetTextFont( 43 );
  leg->SetTextSize( 18 );

  if(isNormPlot){
    leg->SetNColumns( 1 );
    leg->SetTextSize( 15 );
  }
  sort(bkgdFractions.begin(), bkgdFractions.end(), sortbysecAscending);
  //
  // Add data to TLegend
  //
  leg->AddEntry(h_data, "Data","p");
  //
  // Add backgrounds to TLegend
  //
  for(unsigned int iBkgd = 0; iBkgd < bkgdFractions.size(); iBkgd++)
  {
    pair<string, float> bkgdFraction = bkgdFractions.at(iBkgd);
    string bkgdName = bkgdFraction.first;
    TH1D* hist = samples[bkgdName]->h;
    if(!isNormPlot){
      leg->AddEntry(hist, samples[bkgdName]->legend.c_str(),"f");
    }
    else {
      TString frac = TString::Format(" (%.1f%%)", bkgdFraction.second * 100.);
      leg->AddEntry(hist, samples[bkgdName]->legend.c_str() + frac, "f");
    }
  } 
  //
  // Add Uncertainty bands to TLegend
  //
  leg->AddEntry( errorband_stat, "Stat Unc.", "F" );
  //
  // Draw total background stack
  //
  stack_bkgd_sorted->Draw("HIST");
  errorband_stat->Draw("E2same");
  //
  // Draw data
  //
  h_data->Draw("P SAME");
  //
  // Draw TLegend
  //
  leg->Draw();
  //
  // Cosmetics
  //
  stack_bkgd_sorted->GetXaxis()->SetTitle(xaxisName.c_str());
  stack_bkgd_sorted->GetYaxis()->SetTitle(yaxisName.c_str());
  if(isNormPlot){
    stack_bkgd_sorted->GetXaxis()->SetTitle("");
    stack_bkgd_sorted->GetXaxis()->SetBinLabel(1, "Event Count");
  }
  float maximum = max(h_data->GetMaximum(),h_totalbkgd_sorted->GetMaximum());
  stack_bkgd_sorted->SetMaximum(maximum * 1.5);
  if(isNormPlot) stack_bkgd_sorted->SetMaximum(maximum * 2.25);
  stack_bkgd_sorted->SetMinimum(0.0001);
  
  if(doLogY){
    stack_bkgd_sorted->SetMinimum(0.1);
    stack_bkgd_sorted->SetMaximum(maximum * 30);
    pad1->SetLogy();
  }

  pad1->Update();
  canv->Update();
  //
  // Info on Canvas
  //
  TLatex* latex = InfoOnCanvas(canv, pad1, yield_data, yield_totalbkgd_sorted);
  
  //==========================================
  //
  // Lower pad
  //
  //==========================================
  pad2->cd();
  pad2->SetGridy();
  //
  // Draw ratio
  //
  TH1D* ratio = GetRatioHistogram(h_data, h_totalbkgd_sorted);
  SetDataStyle(ratio);
  ratio->GetXaxis()->SetTitle(xaxisName.c_str());
  if(isNormPlot){
    ratio->GetXaxis()->SetTitle("");
    ratio->GetXaxis()->SetBinLabel(1,"Event Count"); 
  }
  if(variableName == "zcand_mass2"){
    ratio->SetMinimum(0.85);
    ratio->SetMaximum(1.15);
  }
  ratio->Draw();
  //
  // Draw stat only error band
  //
  TGraphAsymmErrors* ratio_errorband_stat = GetRatioHistogramErrorBand(h_data, h_totalbkgd_sorted, errorband_stat );
  ratio_errorband_stat->SetFillColor( kBlack );
  ratio_errorband_stat->SetFillStyle( 3245 );
  ratio_errorband_stat->SetLineColor( kWhite );
  ratio_errorband_stat->SetMarkerStyle( 1 );
  ratio_errorband_stat->Draw( "E2same" );
  pad2->Update();
  
  //
  // Draw line
  //
  TLine* line = new TLine( pad2->GetUxmin(), 1, pad2->GetUxmax(), 1 );
  line->SetLineColor( kRed + 1 );
  line->SetLineWidth( 4 );
  line->Draw();
  ratio->Draw("same"); // Draw ratio again
  //
  // Draw arrows
  //
  // DrawArrows(ratio);

  //
  // Print Canvas
  //
  gPad->RedrawAxis();
  canv->Update();
  canv->SaveAs("./plots/DataMC_"+TString(histoName)+".pdf");

  delete stack_bkgd;
  delete stack_bkgd_sorted;
  delete canv;
}

void PlotDataMC()
{
  gROOT->SetBatch();

  TH1::SetDefaultSumw2();
  setTDRStyle();

  vector<string> cutLevels;
  cutLevels.push_back("passMuMu");
  cutLevels.push_back("passMuMuTightIdIso");
  cutLevels.push_back("passTrigTightHLT");
  cutLevels.push_back("passTrigTightMatch");
  cutLevels.push_back("passMETFilters");
  
  vector<string> varNames;
  varNames.push_back("neventsweighted");
  varNames.push_back("npv");
  varNames.push_back("mumu_pt");
  varNames.push_back("mumu_eta");
  varNames.push_back("mumu_phi");
  varNames.push_back("mumu_mass");
  varNames.push_back("mumu_mass2");
  varNames.push_back("mumu_mass3");
  varNames.push_back("nrecojets");
  varNames.push_back("nrecobjets");
  varNames.push_back("nrecolightjets");
  varNames.push_back("met_pt");
  varNames.push_back("met_phi");

  //==============================================================================
  //
  // Open TFiles
  //
  //==============================================================================
  // dirHistos ="/afs/cern.ch/user/n/nbinnorj/XMuMu/XMuMuAnalysis/TreeAnalyzer/plotting/rootfiles/";
  dirHistos ="root://eosuser.cern.ch//eos/user/n/nbinnorj/XMuMuAna/Histos/";

  map<string, Sample*> samples;
  samples = GetMapOfSamples();

  map<string, Sample*>::iterator it;
  for ( it = samples.begin(); it != samples.end(); it++){ 
    Sample* sample = it->second;
    vector<string>  subsamples = sample->subsamples;
    for(vector<string>::iterator itS = subsamples.begin(); itS != subsamples.end(); ++itS) {
      mapToTFiles[(*itS)] = TFile::Open(dirHistos + "Histo_" + (*itS) + ".root");
    }
  }


  for(unsigned int iCut = 0;  iCut < cutLevels.size(); ++iCut) {
    for (unsigned int iVar = 0;  iVar < varNames.size(); ++iVar){
      DataMC(cutLevels.at(iCut),varNames.at(iVar));
    }
  }
}

