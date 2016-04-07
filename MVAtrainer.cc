//////////////////////////////////////////////////////////////////////////////
////     Stand-alone code to train the event-level BDT
////////////////////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#include "TStyle.h"
#include "TPaveText.h"
#include "TTree.h"
#include "TNtuple.h"
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TVectorD.h>
#include <ctime>

#include <cmath>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <errno.h>
#include "TRandom3.h"
#include "TRandom.h"
#include "TProfile.h"
#include <iostream>
#include <map>
#include <cstdlib>

// user code
#include "TopTreeProducer/interface/TRootRun.h"
#include "TopTreeProducer/interface/TRootEvent.h"
#include "TopTreeAnalysisBase/Selection/interface/SelectionTable.h"
#include "TopTreeAnalysisBase/Selection/interface/Run2Selection.h"

#include "TopTreeAnalysisBase/Content/interface/AnalysisEnvironment.h"
#include "TopTreeAnalysisBase/Content/interface/Dataset.h"
#include "TopTreeAnalysisBase/Tools/interface/JetTools.h"
#include "TopTreeAnalysisBase/Tools/interface/PlottingTools.h"
#include "TopTreeAnalysisBase/Tools/interface/MultiSamplePlot.h"
#include "TopTreeAnalysisBase/Tools/interface/TTreeLoader.h"
#include "TopTreeAnalysisBase/Tools/interface/AnalysisEnvironmentLoader.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/JetCorrectorParameters.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/JetCorrectionUncertainty.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/MakeBinning.h"
#include "TopTreeAnalysisBase/MCInformation/interface/LumiReWeighting.h"
#include "TopTreeAnalysisBase/MCInformation/interface/JetPartonMatching.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/MEzCalculator.h"
#include "TopTreeAnalysisBase/Tools/interface/LeptonTools.h"

#include "TopTreeAnalysisBase/Reconstruction/interface/TTreeObservables.h"

// This header file is taken directly from the BTV wiki. It contains
// to correctly apply an event level Btag SF. It is not yet on CVS
// as I hope to merge the functionality into BTagWeigtTools.h
//#include "TopTreeAnalysisBase/Tools/interface/BTagSFUtil.h"
#include "TopTreeAnalysisBase/Tools/interface/BTagWeightTools.h"

#include "TopTreeAnalysisBase/Tools/interface/JetCombiner.h"
#include "TopTreeAnalysisBase/Tools/interface/MVATrainer.h"
#include "TopTreeAnalysisBase/Tools/interface/MVAComputer.h"
#include "TopTreeAnalysisBase/Tools/interface/JetTools.h"
#include "TopTreeAnalysisBase/../FourTops/SingleLepAnalysis/interface/Trigger.h"


using namespace std;
using namespace TopTree;

int main(int argc, char** argv)
{
    string chan;

    if(argc > 0) {
        int iarg = 1;
        if(argc > 1) {
            iarg = 1;
            string val = argv[iarg];
            if(val.find("--chan") != std::string::npos) {
                iarg++;
                chan = argv[iarg];
                cout << "now selecting channel : " << chan << endl;
            }
        } else {
            cout << "no arguments supplied" << endl;
            return 0;
        }
    }


    clock_t start = clock();

    bool debug = true;
    bool singlelep = true;
    bool Muon = true;
    bool Electron = true;
    bool Combined = false;
    int nPassed = 0;

    if(chan.find("el") != std::string::npos)
    {
        Muon = false;
        Electron = true;
    }
    else if(chan.find("mu") != std::string::npos)
    {
        Muon = true;
        Electron = false;
    }        

    // Luminosity = 2640;
    int targetNSignalEvents = 300000;

    // Global variable
    TRootEvent* event = 0;
    MVATrainer* Eventtrainer_;

    Eventtrainer_ = new MVATrainer("BDT", "MasterMVA_SingleMuon_6thApril12pm", "MVA/MasterMVA_SingleMuon_6thApril12pm.root");
    Eventtrainer_->bookWeight("Weight");
    Eventtrainer_->bookInputVar("multitopness");
    Eventtrainer_->bookInputVar("leptonpt");
    // Eventtrainer_->bookInputVar("leptonEta");
    // Eventtrainer_->bookInputVar("HTH");
    Eventtrainer_->bookInputVar("HTRat");
    Eventtrainer_->bookInputVar("HTb");
    Eventtrainer_->bookInputVar("nLtags");
    Eventtrainer_->bookInputVar("nMtags");
    // Eventtrainer_->bookInputVar("nTtags");
    Eventtrainer_->bookInputVar("nJets");
    // Eventtrainer_->bookInputVar("jet5and6pt");
    // Eventtrainer_->bookInputVar("met");
    // Eventtrainer_->bookInputVar("csvJetcsv1");
    // Eventtrainer_->bookInputVar("csvJetcsv2");
    Eventtrainer_->bookInputVar("csvJetcsv3");
    Eventtrainer_->bookInputVar("csvJetcsv4");
    // Eventtrainer_->bookInputVar("anglebjet1bjet2");
    Eventtrainer_->bookInputVar("angleBestTopAllJet");
    Eventtrainer_->bookInputVar("bestTopPt");
    // Eventtrainer_->bookInputVar("angletop1top2");
    // Eventtrainer_->bookInputVar("angletoplep");
    // Eventtrainer_->bookInputVar("Sphericity");
    // Eventtrainer_->bookInputVar("Aplanarity");
    // Eventtrainer_->bookInputVar("Oblateness");
    Eventtrainer_->bookInputVar("fnjetW");
    // Eventtrainer_->bookInputVar("geoMean56");
    // Eventtrainer_->bookInputVar("h10");
    // Eventtrainer_->bookInputVar("h20");
    // Eventtrainer_->bookInputVar("h30"); 
    // Eventtrainer_->bookInputVar("h40");
    // Eventtrainer_->bookInputVar("h50");
    // Eventtrainer_->bookInputVar("h60");            
    // Eventtrainer_->bookInputVar("centrality");            
    Eventtrainer_->bookInputVar("SumJetMassX");            
    // Eventtrainer_->bookInputVar("leptonIso");
    Eventtrainer_->bookInputVar("LeadingBJetPt");
    // Eventtrainer_->bookInputVar("Jet5Pt");
    // Eventtrainer_->bookInputVar("Jet6Pt");

    bool isSignal;

    int event_start = 0;

    string nTuplename;

    string filepath_tttt, filepath_ttbar;
    if(Muon){
        filepath_tttt = "/user/lbeck/CMSSW_7_6_3/src/TopBrussels/FourTops/Craneens_Mu/Craneens5_4_2016/merge/Craneen_NP_overlay_ttttNLO_Run2_TopTree_Study.root";
        filepath_ttbar = "/user/lbeck/CMSSW_7_6_3/src/TopBrussels/FourTops/Craneens_Mu/Craneens5_4_2016/merge/Craneen_TTJets_powheg_Run2_TopTree_Study.root";
        nTuplename = "Craneen__Mu";
    }
    else if (Electron){
        filepath_tttt = "/user/lbeck/CMSSW_7_6_3/src/TopBrussels/FourTops/Craneens_El/Craneens30_3_2016/merge/Craneen_NP_overlay_ttttNLO_Run2_TopTree_Study.root";
        filepath_ttbar = "/user/lbeck/CMSSW_7_6_3/src/TopBrussels/FourTops/Craneens_El/Craneens30_3_2016/merge/Craneen_TTJets_powheg_Run2_TopTree_Study.root";
        nTuplename = "Craneen__El";
    }
    TFile * ttbar = new TFile(filepath_ttbar.c_str());
    TFile * tttt = new TFile(filepath_tttt.c_str());

    TNtuple* ttbar_tup = (TNtuple*)ttbar->Get(nTuplename.c_str()); 
    TNtuple* tttt_tup = (TNtuple*)tttt->Get(nTuplename.c_str()); 
    float multitopness_tt, multitopness_tttt, leptonpt_tt, leptonpt_tttt, leptonEta_tt, leptonEta_tttt, HTb_tt, HTb_tttt, HTRat_tt, HTRat_tttt, HTH_tt, HTH_tttt;
    float nLtags_tt, nLtags_tttt, nMtags_tt, nMtags_tttt, nTtags_tt, nTtags_tttt, nJets_tt, nJets_tttt, jet5and6pt_tt, jet5and6pt_tttt, met_tt, met_tttt;
    float csvJetcsv1_tt, csvJetcsv2_tt, csvJetcsv3_tt, csvJetcsv4_tt, anglebjet1bjet2_tt, angleBestTopAllJet_tt, bestTopPt_tt,csvJetcsv1_tttt, csvJetcsv2_tttt;
    float csvJetcsv3_tttt, csvJetcsv4_tttt, anglebjet1bjet2_tttt, angleBestTopAllJet_tttt, bestTopPt_tttt, angletop1top2_tt, angletoplep_tt, angletop1top2_tttt, angletoplep_tttt;
    float Sphericity_tt,Aplanarity_tt,Oblateness_tt,fnjet_tt,geo_tt,h10_tt,h20_tt,h30_tt,h40_tt,h50_tt,h60_tt,centrality_tt,SumJetMassX_tt;
    float Sphericity_tttt,Aplanarity_tttt,Oblateness_tttt,fnjet_tttt,geo_tttt,h10_tttt,h20_tttt,h30_tttt,h40_tttt,h50_tttt,h60_tttt,centrality_tttt,SumJetMassX_tttt;
    float leptonIso_tt, leptonIso_tttt, LeadingBJetPt_tt, LeadingBJetPt_tttt;

    ttbar_tup->SetBranchAddress("multitopness", &multitopness_tt);
    ttbar_tup->SetBranchAddress("LeptonPt", &leptonpt_tt);
    ttbar_tup->SetBranchAddress("LeptonEta", &leptonEta_tt);
    ttbar_tup->SetBranchAddress("leptonIso", &leptonIso_tt);
    ttbar_tup->SetBranchAddress("HTb", &HTb_tt);
    ttbar_tup->SetBranchAddress("HTH", &HTH_tt);
    ttbar_tup->SetBranchAddress("HTRat", &HTRat_tt);
    ttbar_tup->SetBranchAddress("nLtags", &nLtags_tt);
    ttbar_tup->SetBranchAddress("nMtags", &nMtags_tt);
    ttbar_tup->SetBranchAddress("nTtags", &nTtags_tt);
    ttbar_tup->SetBranchAddress("nJets", &nJets_tt);
    ttbar_tup->SetBranchAddress("jet5and6pt", &jet5and6pt_tt);
    ttbar_tup->SetBranchAddress("met", &met_tt);
    ttbar_tup->SetBranchAddress("csvJetcsv1", &csvJetcsv1_tt);
    ttbar_tup->SetBranchAddress("csvJetcsv2", &csvJetcsv2_tt);
    ttbar_tup->SetBranchAddress("csvJetcsv3", &csvJetcsv3_tt);
    ttbar_tup->SetBranchAddress("csvJetcsv4", &csvJetcsv4_tt);
    ttbar_tup->SetBranchAddress("angleBestTopAllJet", &angleBestTopAllJet_tt);
    ttbar_tup->SetBranchAddress("anglebjet1bjet2", &anglebjet1bjet2_tt);
    ttbar_tup->SetBranchAddress("bestTopPt", &bestTopPt_tt);
    ttbar_tup->SetBranchAddress("angletop1top2", &angletop1top2_tt);
    ttbar_tup->SetBranchAddress("angletoplep", &angletoplep_tt);
    ttbar_tup->SetBranchAddress("fSphericity", &Sphericity_tt);
    ttbar_tup->SetBranchAddress("Aplanarity", &Aplanarity_tt);
    ttbar_tup->SetBranchAddress("fOblateness", &Oblateness_tt);
    ttbar_tup->SetBranchAddress("fnjetW", &fnjet_tt);
    ttbar_tup->SetBranchAddress("fet56", &geo_tt);
    ttbar_tup->SetBranchAddress("fh10", &h10_tt);
    ttbar_tup->SetBranchAddress("fh20", &h20_tt);
    ttbar_tup->SetBranchAddress("fh30", &h30_tt); 
    ttbar_tup->SetBranchAddress("fh40", &h40_tt);
    ttbar_tup->SetBranchAddress("fh50", &h50_tt);
    ttbar_tup->SetBranchAddress("fh60", &h60_tt);            
    ttbar_tup->SetBranchAddress("fcentrality", &centrality_tt);            
    ttbar_tup->SetBranchAddress("SumJetMassX", &SumJetMassX_tt);     
    ttbar_tup->SetBranchAddress("LeadingBJetPt", &LeadingBJetPt_tt);     

    tttt_tup->SetBranchAddress("multitopness", &multitopness_tttt);
    tttt_tup->SetBranchAddress("LeptonPt", &leptonpt_tttt);
    tttt_tup->SetBranchAddress("LeptonEta", &leptonEta_tttt);
    tttt_tup->SetBranchAddress("leptonIso", &leptonIso_tttt);    
    tttt_tup->SetBranchAddress("HTb", &HTb_tttt);
    tttt_tup->SetBranchAddress("HTH", &HTH_tttt);
    tttt_tup->SetBranchAddress("HTRat", &HTRat_tttt);
    tttt_tup->SetBranchAddress("nLtags", &nLtags_tttt);
    tttt_tup->SetBranchAddress("nMtags", &nMtags_tttt);
    tttt_tup->SetBranchAddress("nTtags", &nTtags_tttt);
    tttt_tup->SetBranchAddress("nJets", &nJets_tttt);
    tttt_tup->SetBranchAddress("jet5and6pt", &jet5and6pt_tttt);
    tttt_tup->SetBranchAddress("met", &met_tttt);
    tttt_tup->SetBranchAddress("csvJetcsv1", &csvJetcsv1_tttt);
    tttt_tup->SetBranchAddress("csvJetcsv2", &csvJetcsv2_tttt);
    tttt_tup->SetBranchAddress("csvJetcsv3", &csvJetcsv3_tttt);
    tttt_tup->SetBranchAddress("csvJetcsv4", &csvJetcsv4_tttt);
    tttt_tup->SetBranchAddress("angleBestTopAllJet", &angleBestTopAllJet_tttt);
    tttt_tup->SetBranchAddress("anglebjet1bjet2", &anglebjet1bjet2_tttt);
    tttt_tup->SetBranchAddress("bestTopPt", &bestTopPt_tttt);    
    tttt_tup->SetBranchAddress("angletop1top2", &angletop1top2_tttt);
    tttt_tup->SetBranchAddress("angletoplep", &angletoplep_tttt);
    tttt_tup->SetBranchAddress("fSphericity", &Sphericity_tttt);
    tttt_tup->SetBranchAddress("Aplanarity", &Aplanarity_tttt);
    tttt_tup->SetBranchAddress("fOblateness", &Oblateness_tttt);
    tttt_tup->SetBranchAddress("fnjetW", &fnjet_tttt);
    tttt_tup->SetBranchAddress("fet56", &geo_tttt);
    tttt_tup->SetBranchAddress("fh10", &h10_tttt);
    tttt_tup->SetBranchAddress("fh20", &h20_tttt);
    tttt_tup->SetBranchAddress("fh30", &h30_tttt); 
    tttt_tup->SetBranchAddress("fh40", &h40_tttt);
    tttt_tup->SetBranchAddress("fh50", &h50_tttt);
    tttt_tup->SetBranchAddress("fh60", &h60_tttt);            
    tttt_tup->SetBranchAddress("fcentrality", &centrality_tttt);            
    tttt_tup->SetBranchAddress("SumJetMassX", &SumJetMassX_tttt);     
    tttt_tup->SetBranchAddress("LeadingBJetPt", &LeadingBJetPt_tttt);    

    // for(int idatasets=0;idatasets<2;idatasets++){
    //     if(idatasets == 0){
    //         isSignal=true;
    //     }
    //     else{
    //         isSignal = false;
    //     }
    for(int j = 0; j < targetNSignalEvents; j++) {
        float scaleFactor=1.0;

        ttbar_tup->GetEntry(j);
	// cout<<nJets_tt<<"     "<<jet5and6pt_tttt<<endl;

        if(nJets_tt>6.5){

            //cout << " fill ttjets tree" << endl;
            Eventtrainer_->FillWeight("B","Weight", scaleFactor);
            Eventtrainer_->Fill("B", "multitopness", multitopness_tt);
            Eventtrainer_->Fill("B", "leptonpt",leptonpt_tt);
            // Eventtrainer_->Fill("B", "leptonEta",leptonEta_tt);
            Eventtrainer_->Fill("B", "HTb", HTb_tt);
            Eventtrainer_->Fill("B", "HTRat", HTRat_tt);
            // Eventtrainer_->Fill("B", "HTH", HTH_tt);
            Eventtrainer_->Fill("B", "nLtags", nLtags_tt);
            Eventtrainer_->Fill("B", "nMtags", nMtags_tt);
            // Eventtrainer_->Fill("B", "nTtags", nTtags_tt);
            Eventtrainer_->Fill("B", "nJets", nJets_tt);
            // Eventtrainer_->Fill("B", "leptonIso", leptonIso_tt);
            // Eventtrainer_->Fill("B", "jet5and6pt",jet5and6pt_tt);
            // Eventtrainer_->Fill("B", "met", met_tt);
            // Eventtrainer_->Fill("B", "csvJetcsv1", csvJetcsv1_tt);
            // Eventtrainer_->Fill("B", "csvJetcsv2", csvJetcsv2_tt);
            Eventtrainer_->Fill("B", "csvJetcsv3", csvJetcsv3_tt);
            Eventtrainer_->Fill("B", "csvJetcsv4", csvJetcsv4_tt);
            Eventtrainer_->Fill("B", "angleBestTopAllJet", angleBestTopAllJet_tt);
            // Eventtrainer_->Fill("B", "anglebjet1bjet2", fabs(anglebjet1bjet2_tt));
            Eventtrainer_->Fill("B", "bestTopPt", bestTopPt_tt);        
            // Eventtrainer_->Fill("B", "angletop1top2",  angletop1top2_tt);
            // Eventtrainer_->Fill("B", "angletoplep", angletoplep_tt);
            // Eventtrainer_->Fill("B","Sphericity", Sphericity_tt);
            // Eventtrainer_->Fill("B","Aplanarity", Aplanarity_tt);
            // Eventtrainer_->Fill("B","Oblateness", Oblateness_tt);
            Eventtrainer_->Fill("B","fnjetW", fnjet_tt);
            // Eventtrainer_->Fill("B","geoMean56", geo_tt);
            // Eventtrainer_->Fill("B","h10", h10_tt);
            // Eventtrainer_->Fill("B","h20", h20_tt);
            // Eventtrainer_->Fill("B","h30", h30_tt); 
            // Eventtrainer_->Fill("B","h40", h40_tt);
            // Eventtrainer_->Fill("B","h50", h50_tt);
            // Eventtrainer_->Fill("B","h60", h60_tt);            
            // Eventtrainer_->Fill("B","centrality", centrality_tt);            
            Eventtrainer_->Fill("B","SumJetMassX", SumJetMassX_tt); 
            Eventtrainer_->Fill("B", "LeadingBJetPt", LeadingBJetPt_tt);

            // Eventtrainer_->Fill("B", "Jet5Pt", selectedJets[4]->Pt());
            // Eventtrainer_->Fill("B", "Jet6Pt", selectedJets[5]->Pt());
        }
        if(j>90000)continue;

        tttt_tup->GetEntry(j);

        if(nJets_tttt>6.5){
            Eventtrainer_->FillWeight("S","Weight",scaleFactor);
            Eventtrainer_->Fill("S", "multitopness", multitopness_tttt);
            Eventtrainer_->Fill("S", "leptonpt",leptonpt_tttt);
            // Eventtrainer_->Fill("S", "leptonEta",leptonEta_tttt);
            Eventtrainer_->Fill("S", "HTb", HTb_tttt);
            // Eventtrainer_->Fill("S", "HTH", HTH_tttt);
            Eventtrainer_->Fill("S", "HTRat", HTRat_tttt);
            Eventtrainer_->Fill("S", "nLtags", nLtags_tttt);
            Eventtrainer_->Fill("S", "nMtags", nMtags_tttt);
            // Eventtrainer_->Fill("S", "nTtags", nTtags_tttt);
            Eventtrainer_->Fill("S", "nJets", nJets_tttt);
            // Eventtrainer_->Fill("S", "jet5and6pt", jet5and6pt_tttt);
            // Eventtrainer_->Fill("S", "met", met_tttt);
            // Eventtrainer_->Fill("S", "csvJetcsv1", csvJetcsv1_tttt);
            // Eventtrainer_->Fill("S", "csvJetcsv2", csvJetcsv2_tttt);
            Eventtrainer_->Fill("S", "csvJetcsv3", csvJetcsv3_tttt);
            Eventtrainer_->Fill("S", "csvJetcsv4", csvJetcsv4_tttt);
            Eventtrainer_->Fill("S", "angleBestTopAllJet", angleBestTopAllJet_tttt);
            // Eventtrainer_->Fill("S", "anglebjet1bjet2", fabs(anglebjet1bjet2_tttt));
            Eventtrainer_->Fill("S", "bestTopPt", bestTopPt_tttt);
            // Eventtrainer_->Fill("S", "Jet5Pt", selectedJets[4]->Pt());
            // Eventtrainer_->Fill("S", "Jet6Pt", selectedJets[5]->Pt());
            // Eventtrainer_->Fill("S", "angletop1top2",  angletop1top2_tttt);
            // Eventtrainer_->Fill("S", "angletoplep", angletoplep_tttt);
            // Eventtrainer_->Fill("S","Sphericity", Sphericity_tttt);
            // Eventtrainer_->Fill("S","Aplanarity", Aplanarity_tttt);
            // Eventtrainer_->Fill("S","Oblateness", Oblateness_tttt);
            Eventtrainer_->Fill("S","fnjetW", fnjet_tttt);
            // Eventtrainer_->Fill("S","geoMean56", geo_tttt);
            // Eventtrainer_->Fill("S","h10", h10_tttt);
            // Eventtrainer_->Fill("S","h20", h20_tttt);
            // Eventtrainer_->Fill("S","h30", h30_tttt); 
            // Eventtrainer_->Fill("S","h40", h40_tttt);
            // Eventtrainer_->Fill("S","h50", h50_tttt);
            // Eventtrainer_->Fill("S","h60", h60_tttt);            
            // Eventtrainer_->Fill("S","centrality", centrality_tttt);            
            Eventtrainer_->Fill("S","SumJetMassX", SumJetMassX_tttt); 
            // Eventtrainer_->Fill("S", "leptonIso", leptonIso_tttt);
            Eventtrainer_->Fill("S", "LeadingBJetPt", LeadingBJetPt_tttt);

        }

    }
    // }
    if(Muon){
        Eventtrainer_->TrainMVA("Random", "", 0, 0, "", 0, 0, "_MuApril4th2016", true);
    }
    else if(Electron){
        Eventtrainer_->TrainMVA("Random", "", 0, 0, "", 0, 0, "_ElApril4th2016", true);
    }
    cout << "MVA Trained!" << endl;
    delete Eventtrainer_;
}
