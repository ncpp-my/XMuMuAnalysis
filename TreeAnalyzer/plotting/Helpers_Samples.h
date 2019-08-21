#include <map>
#include <vector>
#include "TH1.h"
#include "TFile.h"
using namespace std;

struct Sample {
  string name;
  string nicename;
  string legend;

  int color; 

  vector<string> subsamples;
  TH1D* h = nullptr;
};
map<string, Sample*> GetMapOfSamples()
{ 
  Sample* Data16 = new Sample;
  Data16->name     = "Data16";
  Data16->nicename = "Data16";
  Data16->legend   = "Data";
  Data16->color    = kBlack;
  Data16->subsamples = {
    "Data16B_SingleMuon",
    "Data16C_SingleMuon",
    "Data16D_SingleMuon",
    "Data16E_SingleMuon",
    "Data16F_SingleMuon",
    "Data16G_SingleMuon",
    "Data16H_SingleMuon",
  };
  //
  //
  //
  Sample* MC2016_DYLL = new Sample;
  MC2016_DYLL->name     = "MC16_DYLL";
  MC2016_DYLL->nicename = "MC16_DYLL";
  MC2016_DYLL->legend   = "Z+jets";
  MC2016_DYLL->color    = kGreen;
  MC2016_DYLL->subsamples = {
    "MC16_DYLL",
  };
  //
  //
  //
  Sample* MC2016_TOP = new Sample;
  MC2016_TOP->name     = "MC16_TOP";
  MC2016_TOP->nicename = "MC16_TOP";
  MC2016_TOP->legend   = "t#bar{t}/t";
  MC2016_TOP->color    = kOrange;
  MC2016_TOP->subsamples = {
    "MC16_TTJets_DiLept",
    // "MC16_TTJets_SingleLept",
    // "MC16_ST_s-chan",
    // "MC16_ST_t-chan_antitop",
    // "MC16_ST_t-chan_top",
    // "MC16_ST_tW_antitop",
    // "MC16_ST_tW_top",
  };
  //
  //
  //
  Sample* MC2016_TT = new Sample;
  MC2016_TT->name     = "MC16_TT";
  MC2016_TT->nicename = "MC16_TT";
  MC2016_TT->legend   = "t#bar{t}";
  MC2016_TT->color    = kOrange;
  MC2016_TT->subsamples = {
    "MC16_TTJets_DiLept",
    // "MC16_TTJets_SingleLept",
  };
  //
  //
  //
  Sample* MC2016_ST = new Sample;
  MC2016_ST->name     = "MC16_ST";
  MC2016_ST->nicename = "MC16_ST";
  MC2016_ST->legend   = "Single-t";
  MC2016_ST->color    = kRed;
  MC2016_ST->subsamples = {
    "MC16_ST_s-chan",
    "MC16_ST_t-chan_antitop",
    "MC16_ST_t-chan_top",
    "MC16_ST_tW_antitop",
    "MC16_ST_tW_top",
  };
  //
  //
  //
  Sample* MC2016_VV = new Sample;
  MC2016_VV->name     = "MC16_VV";
  MC2016_VV->nicename = "MC16_VV";
  MC2016_VV->legend   = "VV";
  MC2016_VV->color    = kYellow;
  MC2016_VV->subsamples = {
    "MC16_WZTo2L2Q",
    "MC16_ZZTo2L2Q",
    "MC16_WWTo2L2Nu",
    "MC16_WWToLNuQQ",
    "MC16_WZTo1L1Nu2Q",
    "MC16_WZTo2Q2Nu",
    "MC16_WZTo3LNu",
    "MC16_ZZTo2L2Nu",
    "MC16_ZZTo4L",
  };
  //
  //
  //
  map<string, Sample*> samples;
  samples["Data16"]    = Data16;
  samples["MC16_DYLL"] = MC2016_DYLL;
  samples["MC16_TOP"]  = MC2016_TOP;
  // samples["MC16_TT"]   = MC2016_TT;
  // samples["MC16_ST"]   = MC2016_ST;
  // samples["MC16_VV"]   = MC2016_VV;

  return samples;
}
