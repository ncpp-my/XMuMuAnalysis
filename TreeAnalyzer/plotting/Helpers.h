#include "TCanvas.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH1.h"
#include "TGraphAsymmErrors.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TFile.h"
#include "TTree.h"
#include "THStack.h"
#include "TROOT.h"

#include <map>
#include <vector>
#include <iostream>

using namespace std;

TCanvas* MakeBoxCanvas(TString canvName)
{
  TCanvas* c = new TCanvas(canvName, canvName , 600, 600);
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.08);
  c->SetTopMargin(0.05);
  c->SetBottomMargin(0.15);
  
  return c;
}
TCanvas* MakeBoxCanvas_WithRatioPad(TString canvName)
{
  TCanvas* c = new TCanvas(canvName, canvName , 600, 600);
  c->SetFillStyle( 4000 );
  c->Draw();
  
  double the_low_margin = 0.3;
  
  TPad* pad1 = new TPad(canvName + "_pad1", canvName + "_pad1",0.0, the_low_margin, 1.0, 1.0);
  pad1->SetTopMargin( 0.06 );
  pad1->SetBottomMargin( 0.0 );
  pad1->SetFillStyle( 4000 );
  pad1->SetFillColor( 0 );
  pad1->SetFrameFillStyle( 4000 );
  pad1->SetFrameFillColor( 0 );
  pad1->Draw();

  TPad* pad2 = new TPad(canvName + "_pad2",canvName + "_pad2",0.0, 0.0, 1.0, the_low_margin);
  pad2->SetTopMargin( 0.05 );
  pad2->SetBottomMargin( 0.325 );
  pad2->SetFillStyle( 4000 );
  pad2->SetFillColor( 0 );
  pad2->SetFrameFillStyle( 4000 );
  pad2->SetFrameFillColor( 0 );
  pad2->Draw();
  
  return c;
}
TCanvas* MakeCanvas43(TString canvName)
{
  TCanvas* c = new TCanvas(canvName, canvName , 800, 600);
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.08);
  c->SetTopMargin(0.05);
  c->SetBottomMargin(0.15);
  
  return c;
}
TCanvas* MakeCanvas43_WithRatioPad(TString canvName)
{
  TCanvas* c = new TCanvas(canvName, canvName , 800, 600);
  c->SetFillStyle( 4000 );
  c->Draw();
  
  double the_low_margin = 0.3;
  
  TPad* pad1 = new TPad(canvName + "_pad1", canvName + "_pad1",0.0, the_low_margin, 1.0, 1.0);
  pad1->SetTopMargin( 0.06 );
  pad1->SetBottomMargin( 0.0 );
  pad1->SetFillStyle( 4000 );
  pad1->SetFillColor( 0 );
  pad1->SetFrameFillStyle( 4000 );
  pad1->SetFrameFillColor( 0 );
  pad1->Draw();

  TPad* pad2 = new TPad(canvName + "_pad2",canvName + "_pad2",0.0, 0.0, 1.0, the_low_margin);
  pad2->SetTopMargin( 0.05 );
  pad2->SetBottomMargin( 0.325 );
  pad2->SetFillStyle( 4000 );
  pad2->SetFillColor( 0 );
  pad2->SetFrameFillStyle( 4000 );
  pad2->SetFrameFillColor( 0 );
  pad2->Draw();
  
  return c;
}
void show_overflow(TH1D* hist)
{
  int nbins             = hist->GetNbinsX();
  double underflow      = hist->GetBinContent(   0   );
  double underflowerror = hist->GetBinError  (   0   );
  double overflow       = hist->GetBinContent(nbins+1);
  double overflowerror  = hist->GetBinError  (nbins+1);
  double firstbin       = hist->GetBinContent(   1   );
  double firstbinerror  = hist->GetBinError  (   1   );
  double lastbin        = hist->GetBinContent( nbins );
  double lastbinerror   = hist->GetBinError  ( nbins );

  if (underflow != 0){
    double newcontent = underflow + firstbin;
    double newerror = 0.;
    if (firstbin == 0){
      newerror = underflowerror;
    }
    else{
      newerror = sqrt( underflowerror * underflowerror + firstbinerror * firstbinerror );
    }
    hist->SetBinContent(1, newcontent);
    hist->SetBinError  (1, newerror);
    //
    // Set underflow to be 0
    //
    hist->SetBinContent(0, 0.0);
    hist->SetBinError  (0, 0.0);
  }

  if (overflow != 0){
    double newcontent = overflow + lastbin;
    double newerror = 0.;
    if (lastbin == 0){
      newerror = overflowerror;
    }
    else{
      newerror = sqrt( overflowerror * overflowerror + lastbinerror * lastbinerror );
    }
    hist->SetBinContent(nbins, newcontent);
    hist->SetBinError  (nbins, newerror);
    //
    // Set overflow to be 0
    //
    hist->SetBinContent(nbins+1, 0.0);
    hist->SetBinError  (nbins+1, 0.0);
  }
}
TPad* GetPadFromCanvas(TCanvas* canv, TString padName){
  TPad* pad = 0;
  pad = (TPad*) canv->GetListOfPrimitives()->FindObject(TString(canv->GetName()) + "_" + padName );
  return pad;
}
TH1D* MakeRatio(TH1D* denom, TH1D* numer, float yhi = 2.0, float ylo = 0.0)
{
  TH1D* numerC = 0;
  
  numerC = (TH1D*) numer->Clone("ratio");
  if(numerC != 0) numerC->Divide(denom);
  else {cout << "Clone pointer is null !!!" << endl;}
  numerC->GetYaxis()->SetRangeUser(ylo, yhi);
  
  return numerC;
}
void RatioCosmetics(TH1D* ratio, TString xaxisname, TString yaxisname){
  ratio->GetXaxis()->SetTitle(xaxisname);
  ratio->GetYaxis()->SetTitle(yaxisname);

  ratio->GetXaxis()->SetLabelFont( 43 );
  ratio->GetXaxis()->SetTitleFont( 43 );
  ratio->GetXaxis()->SetLabelSize( 20 );
  ratio->GetXaxis()->SetTitleSize( 20 );
  ratio->GetXaxis()->SetTitleOffset( 3.2 );
  
  ratio->GetYaxis()->SetLabelFont( 43 );
  ratio->GetYaxis()->SetTitleFont( 43 );
  ratio->GetYaxis()->SetLabelSize( 20 );
  ratio->GetYaxis()->SetTitleSize( 20 );
  ratio->GetYaxis()->SetTitleOffset( 1.1 );
  ratio->GetYaxis()->CenterTitle( 1 );
  
  ratio->GetYaxis()->SetNdivisions( 5, 3, 0 );
  
  ratio->SetMarkerStyle( 20 );
  ratio->SetMarkerSize( 0.8 );
}
TLegend* MakeLegend(float x1 = 0.6, float y1 = 0.5, float x2 = 0.9, float y2 = 0.9)
{
  TLegend* leg = new TLegend(x1, y1, x2, y2);
  leg->SetFillStyle( 0 );
  leg->SetBorderSize( 0 );
  leg->SetTextFont( 43 );
  leg->SetTextSize( 15 );
  
  return leg;
}
void SaveCanvasAsPDF(TString dir, TCanvas* canv){
  canv->SaveAs(dir + canv->GetName() + ".pdf");
}

void NormUnity(TH1D* histo)
{
  int binmax = histo->GetNbinsX();
  double integral = histo->Integral(1,binmax);
  histo->Scale(1.0/integral);
  
  return;
}
TH1D* makeTH1DVarRebin(TH1D* hist, int nrebin, double* bins, bool divideByWidth = false){
  
  TH1D* hist_rebin = 0;
  hist_rebin = (TH1D*) hist->Rebin(nrebin, hist->GetName() + TString("_Rebin"), bins);
  if(divideByWidth) hist_rebin->Scale(1,"width");
  return hist_rebin;
}
TH1D* makeTH1DVarRebin2(TH1D* hist, std::vector<double> bins, bool divideByWidth = false){
  
  TH1D* hist_rebin = 0;
  hist_rebin = (TH1D*) hist->Rebin(bins.size() - 1, hist->GetName() + TString("_Rebin"), bins.data());
  if(divideByWidth) hist_rebin->Scale(1,"width");
  return hist_rebin;
}
TH1D* makeTH1DVarRebin3(TH1D* hist, std::vector<double> bins, bool divideByWidth = false){
    
  float yield_preRebin = hist->Integral(0, hist->GetNbinsX()+1);
    
  TH1D* hist_rebin = 0;
  hist_rebin = (TH1D*) hist->Rebin(bins.size() - 1, hist->GetName() + TString("_Rebin"), bins.data());
  if(divideByWidth){
    hist_rebin->Scale(1,"width");
    float yield_postRebinAndDivide = hist_rebin->Integral(0, hist_rebin->GetNbinsX()+1);
    if(yield_postRebinAndDivide > 0.000){
      hist_rebin->Scale(yield_preRebin / yield_postRebinAndDivide);
    }
  }
  return hist_rebin;
}
TH1D* GetTH1DromFile(TFile* file, TString histoName, int color = 1, bool doNormUnity = false)
{
  TH1D* h = 0;
  h = (TH1D*) file->Get(histoName);
  if(h == 0){
    std::cout << "Warning :: Failed to retrieve TH1 named : " << histoName << std::endl; 
  }
  TH1D* hC = 0;
  hC = (TH1D*) h->Clone(histoName + "_Clone"); 
  
  hC->SetDirectory(0);
  hC->SetLineColor(color);
  hC->SetMarkerColor(color);
  hC->SetLineWidth(2);
  
  if (doNormUnity){
    NormUnity(hC); 
  }
  
  return hC;
}
TH1D* GetTH1DromFile2(TFile* file, TString histoName)
{
  TH1D* h = 0;
  h = (TH1D*) file->Get(histoName);
  if(h == 0){
    std::cout << "Warning :: Failed to retrieve TH1 named : " << histoName << std::endl; 
  }
  TH1D* hC = 0;
  hC = (TH1D*) h->Clone(histoName + "_Clone"); 
  
  return hC;
}
TH1D* GetTH1D(TString histoName, int color = 1, bool doNormUnity = false)
{
  TH1D* h = 0;
  h = (TH1D*) gDirectory->Get(histoName); 
  if(h == 0){
    std::cout << "Warning :: Failed to retrieve TH1 named : " << histoName << std::endl; 
  }
  
  h->SetDirectory(0);
  
  h->SetLineColor(color);
  h->SetMarkerColor(color);
  h->SetLineWidth(2);
  
  if (doNormUnity){
    NormUnity(h); 
  }
  
  return h;
}
TH1D* MakeTH1D(TString histoName, int nbins, float xlo, float xup, int color = 1, bool doNormUnity = false)
{
  TH1D* h = new TH1D(histoName,histoName,nbins,xlo,xup);
  h->Sumw2();
  h->StatOverflows();
  
  if(h == 0){
    std::cout << "Warning :: Failed to create TH1D : " << histoName << std::endl; 
  }
  
  //h->SetDirectory(0);
  h->SetLineColor(color);
  h->SetMarkerColor(color);
  h->SetLineWidth(2);
  
  if (doNormUnity){
    NormUnity(h); 
  }
  
  return h;
}
int FindIndexInVector( vector<string> vec, string name)
{ 
  auto it = std::find(vec.begin(), vec.end(), name);
  auto index = std::distance(vec.begin(), it);
  return index;
}
void addInQuadrature(float sys,float nom,float &up, float &down)
{
  float sys_diff = sys - nom;
  if (sys_diff > 0)
    up += sys_diff*sys_diff;
  else if (sys_diff < 0)
    down += sys_diff*sys_diff;

}
void addInStatBin(int i, TH1D* nom, float& up, float& down)
{
  float mcstat = nom->GetBinError(i);
  up += mcstat * mcstat;
  down += mcstat * mcstat;
  
  return;
}
TGraphAsymmErrors* makeSystBand(TH1D* nom, vector<TH1D*> systematics)
{
  int n_bins_combined = nom->GetNbinsX();
  vector<float> x; vector<float> ex_up; std::vector<float> ex_down;
  vector<float> y; vector<float> ey_up; std::vector<float> ey_down;
  
  for(int ibin = 1; ibin <= n_bins_combined; ibin++)
  {
    float up = 0.0;
    float down = 0.0;

    x.push_back(nom->GetBinCenter(ibin));
    float bin_width = nom->GetBinWidth(ibin);
    ex_down.push_back(bin_width/2.);
    ex_up.push_back(bin_width/2.);
    y.push_back(nom->GetBinContent(ibin));
    
    for (int i = 0; i < systematics.size(); i++)
    {
      TH1D* hist = systematics.at(i); 
      float nominal = nom->GetBinContent(ibin);
      float this_syst = hist->GetBinContent(ibin);

      addInQuadrature(this_syst, nominal, up, down);
    }
    
    addInStatBin(ibin, nom, up, down);
    
    if(up > 0)
      ey_up.push_back(sqrt(up));
    else 
      ey_up.push_back(0);
    if (down > 0)
      ey_down.push_back(sqrt(down));
    else
      ey_down.push_back(0);     
    
    // cout << ibin << " | " << nom->GetBinCenter(ibin) << " | " << nom->GetBinContent(ibin) << " | " << sqrt(up) << " | " << sqrt(down) << endl;
  }
  TGraphAsymmErrors* errorband = new TGraphAsymmErrors(n_bins_combined, &x[0], &y[0], &ex_down[0], &ex_up[0], &ey_down[0], &ey_up[0]);
  return errorband;
}
// UNDER DEVELOPMENT
// Can only be symmetric
//
/*
TH1D* makeSystBandTH1D(TH1D* nom, vector<TH1D*> systematics)
{
  int n_bins_combined = nom->GetNbinsX();
  vector<float> x; vector<float> ex_up; std::vector<float> ex_down;
  vector<float> y; vector<float> ey_up; std::vector<float> ey_down;
  
  TH1D* errorband = (TH1D*) nom->Clone(nom->GetTitle() + "_syst");
  
  for(int ibin = 1; ibin <= n_bins_combined; ibin++)
  {
    float up = 0.0;
    float down = 0.0;

    x.push_back(nom->GetBinCenter(ibin));
    float bin_width = nom->GetBinWidth(ibin);
    ex_down.push_back(bin_width/2.);
    ex_up.push_back(bin_width/2.);
    y.push_back(nom->GetBinContent(ibin));
    
    for (int i = 0; i < systematics.size(); i++)
    {
      TH1D* hist = systematics.at(i); 
      float nominal = nom->GetBinContent(ibin);
      float this_syst = hist->GetBinContent(ibin);

      addInQuadrature(this_syst, nominal, up, down);
    }
    
    addInStatBin(ibin, nom, up, down);
    
    if(up > 0)
      ey_up.push_back(sqrt(up));
    else 
      ey_up.push_back(0);
    if (down > 0)
      ey_down.push_back(sqrt(down));
    else
      ey_down.push_back(0);     
  }

  return errorband;
}
*/
TH1D* GetRatioHistogram(TH1D* Data, TH1D* Bkgd )
{
  TH1D* Result = (TH1D*) Data->Clone( TString("RatioHistogramFor") + Data->GetName() );
  
  float AverageRatio = 0;
  float NumberOfPoints = 0;
  
  for(int i = 0; i < Result->GetNbinsX(); i++)
  {
    int bin = i + 1;
    if (Bkgd->GetBinContent( bin ) > 0){
      Result->SetBinContent( bin, Data->GetBinContent( bin ) / Bkgd->GetBinContent( bin ) );
      Result->SetBinError( bin, 
                          sqrt( pow( Data->GetBinError( bin ) / Bkgd->GetBinContent( bin ), 2 ) + pow( Data->GetBinContent( bin ) * Bkgd->GetBinError( bin ) 
                          / pow( Bkgd->GetBinContent( bin ), 2 ), 2 ) ) 
                        );
      AverageRatio += Data->GetBinContent( bin ) / Bkgd->GetBinContent( bin );
      NumberOfPoints += 1.;
    }
  } 
  Result->SetLineStyle( 1 );
  Result->SetLineColor( kBlack );
  Result->SetMarkerSize( 0.8 );
  Result->GetYaxis()->SetNdivisions( 5, 3, 0 );
  Result->SetMaximum( 2.00 );
  Result->SetMinimum( 0.00 );
  Result->GetYaxis()->SetTitle( "Data/Pred" );
  Result->GetYaxis()->CenterTitle( true );
  
  return Result;
}
TGraphAsymmErrors* GetRatioHistogramErrorBand(TH1D* Data, TH1D* Bkgd, TGraphAsymmErrors* Syst )
{
  TH1D* Result = (TH1D*) Data->Clone( TString("DummyRatioHistogramFor") + Data->GetName() );
  TGraphAsymmErrors* SystBand = (TGraphAsymmErrors*) Syst->Clone( TString("RatioHistogramFor") + Bkgd->GetName() );
  TGraphAsymmErrors* SystBandFinal = (TGraphAsymmErrors*) Syst->Clone( TString("FinalRatioHistogramFor") + Bkgd->GetName() );
  
  // cout << "NBins = " << Result->GetNbinsX() << endl;
  for(int i = 0; i < Result->GetNbinsX(); i++)
  {
    int bin = i + 1;
    // cout << bin << endl;
    
    float x = SystBand->GetX()[i];
    float y = SystBand->GetY()[i];
    
    float xtemp = SystBandFinal->GetX()[i];
    float ytemp = SystBandFinal->GetY()[i];
    
    float x_err_up   = SystBand->GetEXhigh()[i];
    float x_err_down = SystBand->GetEXlow()[i];
    float y_err_up   = SystBand->GetEYhigh()[i];
    float y_err_down = SystBand->GetEYlow()[i];
    
    SystBandFinal->SetPoint(i, x, 1.);
    if(y > 0){
      SystBandFinal->SetPointEYhigh(i, y_err_up / y);
      SystBandFinal->SetPointEYlow(i, y_err_down / y);
    }
    else{
      SystBandFinal->SetPointEYhigh(i, 0);
      SystBandFinal->SetPointEYlow(i, 0);
    }
    
    SystBandFinal->SetPointEXhigh(i, x_err_up );
    SystBandFinal->SetPointEXlow(i, x_err_down );
  }   
  return SystBandFinal;
}
TH1D* MakeRatioHisto(TH1D* h1, TH1D* h2)
{
  TH1D* Result = (TH1D*) h1->Clone( h1->GetName() + TString("Clone") );
  
  float AverageRatio = 0;
  float NumberOfPoints = 0;
  
  for(int i = 0; i < Result->GetNbinsX(); i++)
  {
    int bin = i + 1;
    if (h2->GetBinContent( bin ) > 0){
      Result->SetBinContent( bin, h1->GetBinContent( bin ) / h2->GetBinContent( bin ) );
      Result->SetBinError( bin, 
                          sqrt( pow( h1->GetBinError( bin ) / h2->GetBinContent( bin ), 2 ) + pow( h1->GetBinContent( bin ) * h2->GetBinError( bin ) 
                          / pow( h2->GetBinContent( bin ), 2 ), 2 ) ) 
                        );
      AverageRatio += h1->GetBinContent( bin ) / h2->GetBinContent( bin );
      NumberOfPoints += 1.;
    }
  } 
  Result->GetYaxis()->SetNdivisions( 5, 3, 0 );
  Result->SetMaximum( 2.00 );
  Result->SetMinimum( 0.00 );
  Result->GetYaxis()->SetTitle( "Ratio" );
  Result->GetYaxis()->CenterTitle( true );
  
  return Result;
}
void SetDataStyle(TH1D* h)
{
  TAxis* x = h->GetXaxis();
  x->SetLabelFont( 43 );
  x->SetTitleFont( 43 );
  x->SetLabelSize( 20 );
  x->SetTitleSize( 20 );
  
  TAxis* y= h->GetYaxis();
  y->SetLabelFont( 43 );
  y->SetTitleFont( 43 );
  y->SetLabelSize( 20 );
  y->SetTitleSize( 20 );
  
  h->SetMarkerStyle( 20 );
  h->SetMarkerSize( 0.8 );
  h->SetLineColor( 1 );
  
  h->SetTitleOffset( 1.6, "Y" );
  h->SetTitleOffset( 3.2, "X" );
  
}
void DrawArrows(TH1D* Ratio)
{
  TArrow* Arrow = new TArrow();
  Arrow->SetFillColor(kRed+1);
  Arrow->SetLineColor(kRed+1);
  Arrow->SetLineWidth( 0 );
  for(int i = 1; i < Ratio->GetNbinsX() + 1; i++)
  {  
    float x = Ratio->GetBinCenter( i );
    float ArrRatio = Ratio->GetBinContent( i );
    if (ArrRatio > 2.00)
      Arrow->DrawArrow(x,2.00-0.1,x,2.00,0.02,"|>");
    else if (ArrRatio < 0.01 && ArrRatio > 0)
      Arrow->DrawArrow(x,0.00+0.1,x,0.25,0.02,"|>");
    else{}
  }
  return;
}
bool sortbysecDescending(const pair<string,float> &a, const pair<string,float> &b){
  return (a.second < b.second);
}

bool sortbysecAscending(const pair<string,float> &a, const pair<string,float> &b){
  return (a.second > b.second);
}
// TLatex* InfoOnCanvas(TCanvas* canv, TPad* pad1, float xLat, float yLat, TString channel, TString regionName, TString cutlevelName, float yield_data, float yield_totalbkgd)
TLatex* InfoOnCanvas(TCanvas* canv, TPad* pad1, float yield_data, float yield_totalbkgd)
{
  canv->cd();
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextFont( 42 );
  latex->SetTextSize( 0.035 );
  latex->SetTextColor( 1 );
  latex->SetTextAlign( 12 );

  TString Internal = TString::Format("#bf{CMS} Internal");
  TString Lumi     = TString::Format("%.1f fb^{-1}", 35.9);
  TString CmsE     = TString::Format("(13 TeV)");
  
  latex->DrawLatex( 0.15, 0.92, Internal );
  latex->SetTextSize( 0.03 );
  latex->DrawLatex(0.7425, 0.98, Lumi + TString(" ") + CmsE );
  latex->SetTextSize( 0.025 );
  
  pad1->cd();
  
  latex->SetTextSize( 0.0375 );
  latex->SetTextColor( kBlack );
  latex->DrawLatex( 0.16, 0.97, TString::Format("Data / Pred = %.0f / %.1f = %.2f", yield_data, yield_totalbkgd , yield_data / yield_totalbkgd));
  
  return latex;
}
TH1D* GetHistoSyst(TFile* file, TString systName, TString varCutstring, TString selString, TString sampleName, int varNbins, float varBinLo, float varBinUp)
{
  TString treeName = systName;
  
  TString weight = " * eventweight ";
  if(systName.Contains("FT_EFF_Eigen") || systName.Contains("FT_EFF_extrapolation"))
  {    
    weight = " * eventweight_Sys" + systName;
    treeName = "Nominal";
  }
 
  TH1D* hist = nullptr;
  TTree* tree = nullptr;
  tree = (TTree*) file->Get(treeName);
  //
  //
  //
  hist = MakeTH1D("h_"+sampleName+"___Sys"+systName, varNbins, varBinLo, varBinUp, 1, false); 
  if(tree){
    tree->Draw(varCutstring + " >> " + "h_"+sampleName+"___Sys"+systName, selString + weight); 
  }
  else{
    return nullptr;
  }
  return hist;
}

