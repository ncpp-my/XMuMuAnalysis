using namespace ROOT::VecOps;

int getN(const RVec<float> &vec, float cut)
{
  int n = std::count_if(vec.begin(), vec.end(), [cut](float x){return x > cut;});
  return n;
}

RVec<ROOT::Math::PtEtaPhiEVector> ConstructP4Vector(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& e){
  RVec<ROOT::Math::PtEtaPhiEVector> vec;
  const auto size = pt.size();
  vec.reserve(size);
  for (auto i = 0; i < size; ++i) {
    vec.emplace_back(pt.at(i),eta.at(i),phi.at(i),e.at(i));
  }
  return vec;
}

struct JetPair {
  // int    refpdgid;
  // int    refpdgid_algorithmicDef;
  // int    refpdgid_physicsDef;
  float  refe;
  float  refpt;
  float  refeta;
  float  refphi;
  float  refy;
  float  refdrjt;
  float  refarea;
  float  jte;
  float  jtpt;
  float  jteta;
  float  jtphi;
  float  jty;
  float  jtjec;
  float  jtarea;
  float  jtchf;
  float  jtnhf;
  float  jtnef;
  float  jtcef;
  float  jtmuf;
  float  jthfhf;
  float  jthfef;
  float  refdzvtx;
};

RVec<JetPair> ConstructJetPairVector(
  RVec<float>& refe,  RVec<float>& refpt,   RVec<float>& refeta, RVec<float>& refphi,
  RVec<float>& refy,  RVec<float>& refdrjt, RVec<float>& refarea,
  RVec<float>& jte,   RVec<float>& jtpt,    RVec<float>& jteta,  RVec<float>& jtphi,  RVec<float>& jty, 
  RVec<float>& jtjec, RVec<float>& jtarea, RVec<float>& jtchf,  RVec<float>& jtnhf,  RVec<float>& jtnef, 
  RVec<float>& jtcef, RVec<float>& jtmuf,  RVec<float>& jthfhf, RVec<float>& jthfef, RVec<float>& refdzvtx
  )
{
  RVec<JetPair> vec;
  const auto size = refe.size();
  vec.reserve(size);
  for (auto i = 0; i < size; ++i) 
  {
    // JetPair pair = {refe.at(i),refpt.at(i),refeta.at(i),refphi.at(i),
    //     refy.at(i),refdrjt.at(i),refarea.at(i),
    //     jte.at(i),jtpt.at(i),jteta.at(i),jtphi.at(i),jty.at(i),
    //     jtjec.at(i),jtarea.at(i),jtchf.at(i),jtnhf.at(i),jtnef.at(i),
    //     jtcef.at(i),jtmuf.at(i),jthfhf.at(i),jthfef.at(i),refdzvtx.at(i)};
    // vec.emplace_back(pair);
    vec.emplace_back(
      JetPair{refe.at(i),refpt.at(i),refeta.at(i),refphi.at(i),
        refy.at(i),refdrjt.at(i),refarea.at(i),
        jte.at(i),jtpt.at(i),jteta.at(i),jtphi.at(i),jty.at(i),
        jtjec.at(i),jtarea.at(i),jtchf.at(i),jtnhf.at(i),jtnef.at(i),
        jtcef.at(i),jtmuf.at(i),jthfhf.at(i),jthfef.at(i),refdzvtx.at(i)
      }
    );
  }
  return vec;
}

