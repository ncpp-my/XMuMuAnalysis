# Calculating Luminosity with BrilCalc

```bash
export PATH=$HOME/.local/bin:/cvmfs/cms-bril.cern.ch/brilconda/bin:$PATH
brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -u /fb -i Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt
brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -u /fb -i Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt
brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -u /fb -i Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt
brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -u /fb -i Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt
```

The JSON txt files were copied from

```bash
/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt
/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt
/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt
/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/ReReco/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt
```

Related CMS internal twikis:

https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2016Analysis#Re_reco_datasets_07Aug17
https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2017Analysis#13_TeV_pp_runs_ReReco
https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2018Analysis#Prompt_Reco
https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2018Analysis#Early2018Re_reco_17Sep2018_datas

