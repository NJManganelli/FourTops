//////////////////////////////////////////////////////////////////////////////
////         Analysis code for search for Four Top Production.                  ////
////////////////////////////////////////////////////////////////////////////////////

// ttbar @ NLO 13 TeV:
// all-had ->679 * .46 = 312.34
// semi-lep ->679 *.45 = 305.55
// di-lep-> 679* .09 = 61.11

// ttbar @ NNLO 8 TeV:
// all-had -> 245.8 * .46 = 113.068
// semi-lep-> 245.8 * .45 = 110.61
// di-lep ->  245.8 * .09 = 22.122

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
//#include "TopTreeAnalysisBase/Selection/interface/FourTopSelectionTable.h"

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
#include "TopTreeAnalysisBase/Tools/interface/BTagCalibrationStandalone.h"

#include "TopTreeAnalysisBase/Tools/interface/JetCombiner.h"
#include "TopTreeAnalysisBase/Tools/interface/MVATrainer.h"
#include "TopTreeAnalysisBase/Tools/interface/MVAComputer.h"
#include "TopTreeAnalysisBase/Tools/interface/TopologyWorker.h"
//#include "TopTreeAnalysisBase/Tools/interface/JetTools.h"

using namespace std;
using namespace TopTree;
using namespace reweight;

bool split_ttbar = false;
bool debug = false;
float topness;

pair<float, vector<unsigned int> > MVAvals1;
pair<float, vector<unsigned int> > MVAvals2;
pair<float, vector<unsigned int> > MVAvals2ndPass;
pair<float, vector<unsigned int> > MVAvals3rdPass;

int nMVASuccesses = 0;
int nMatchedEvents = 0;

/// Normal Plots (TH1F* and TH2F*)
map<string, TH1F*> histo1D;
map<string, TH2F*> histo2D;
map<string, TProfile*> histoProfile;

/// MultiSamplePlot
map<string, MultiSamplePlot*> MSPlot;

/// MultiPadPlot
map<string, MultiSamplePlot*> MultiPadPlot;


struct HighestCVSBtag {
    bool operator()(TRootJet* j1, TRootJet* j2) const
    {
        return j1->btag_combinedInclusiveSecondaryVertexV2BJetTags() >
            j2->btag_combinedInclusiveSecondaryVertexV2BJetTags();
    }
};
struct pair_pt {
    bool operator()(const std::pair<float, std::pair<TRootParticle*, TRootParticle*> >& left,
        const std::pair<float, std::pair<TRootParticle*, TRootParticle*> >& right) const
    {
        return left.first > right.first;
    }
};

bool match;

// To cout the Px, Py, Pz, E and Pt of objects
int Factorial(int N);
float Sphericity(vector<TLorentzVector> parts);
float Centrality(vector<TLorentzVector> parts);
float ElectronRelIso(TRootElectron* el, float rho);
float PythiaTune(int jets);

int main(int argc, char* argv[])
{

    // Checking Passed Arguments to ensure proper execution of MACRO
    if(argc < 12) {
        std::cerr << "TOO FEW INPUTs FROM XMLFILE.  CHECK XML INPUT FROM SCRIPT.  " << argc
                  << " ARGUMENTS HAVE BEEN PASSED." << std::endl;
        return 1;
    }

    // Placing arguments in properly typed variables for Dataset creation

    const string dName = argv[1];
    const string dTitle = argv[2];
    const int color = strtol(argv[4], NULL, 10);
    const int ls = strtol(argv[5], NULL, 10);
    const int lw = strtol(argv[6], NULL, 10);
    const float normf = strtod(argv[7], NULL);
    const float EqLumi = strtod(argv[8], NULL);
    const float xSect = strtod(argv[9], NULL);
    const float PreselEff = strtod(argv[10], NULL);
    string fileName = argv[11];
    //    const int startEvent            = strtol(argv[argc-2], NULL, 10);
    //    const int endEvent              = strtol(argv[argc-1], NULL, 10);
    vector<string> vecfileNames;
    for(int args = 11; args < argc; args++) {
        vecfileNames.push_back(argv[args]);
    }

    cout << "---Dataset accepted from command line---" << endl;
    cout << "Dataset Name: " << dName << endl;
    cout << "Dataset Title: " << dTitle << endl;
    cout << "Dataset color: " << color << endl;
    cout << "Dataset ls: " << ls << endl;
    cout << "Dataset lw: " << lw << endl;
    cout << "Dataset normf: " << normf << endl;
    cout << "Dataset EqLumi: " << EqLumi << endl;
    cout << "Dataset xSect: " << xSect << endl;
    for(int files = 0; files < vecfileNames.size(); files++) {
        cout << "Dataset File Names: " << vecfileNames[files] << endl;
    }

    //    cout << "Beginning Event: " << startEvent << endl;
    //    cout << "Ending Event: " << endEvent << endl;
    cout << "----------------------------------------" << endl;
    //    cin.get();

    ofstream eventlist;
    eventlist.open("interesting_events_mu.txt");

    int passed = 0;
    int ndefs = 0;
    int negWeights = 0;
    float weightCount = 0.0;
    int eventCount = 0, trigCount = 0;

    string btagger = "CSVM";
    float scalefactorbtageff, mistagfactor;
    float workingpointvalue = 0.800; // working points updated to 2012 BTV-POG recommendations.

    if(btagger == "CSVL")
        workingpointvalue = .460;
    else if(btagger == "CSVM")
        workingpointvalue = .800;
    else if(btagger == "CSVT")
        workingpointvalue = .935;

    clock_t start = clock();

    // BTagWeightTools * bTool = new BTagWeightTools("SFb-pt_NOttbar_payload_EPS13.txt", "CSVM") ;

    int doJESShift = 0; // 0: off 1: minus 2: plus
    cout << "doJESShift: " << doJESShift << endl;

    int doJERShift = 0; // 0: off (except nominal scalefactor for jer) 1: minus 2: plus
    cout << "doJERShift: " << doJERShift << endl;

    int dobTagEffShift = 0; // 0: off (except nominal scalefactor for btag eff) 1: minus 2: plus
    cout << "dobTagEffShift: " << dobTagEffShift << endl;

    int domisTagEffShift = 0; // 0: off (except nominal scalefactor for mistag eff) 1: minus 2: plus
    cout << "domisTagEffShift: " << domisTagEffShift << endl;

    int mvaNegWeight = 0; // 0: Using MVA trained without neg weights 1: Using MVA trained with Neg Weights
    cout << "mvaNegWeight: " << mvaNegWeight << endl;

    cout << "*************************************************************" << endl;
    cout << " Beginning of the program for the FourTop search ! " << endl;
    cout << "*************************************************************" << endl;

    string postfix = "_Run2_TopTree_Study"; // to relabel the names of the output file

    if(doJESShift == 1)
        postfix = postfix + "_JESMinus";
    if(doJESShift == 2)
        postfix = postfix + "_JESPlus";
    if(doJERShift == 1)
        postfix = postfix + "_JERMinus";
    if(doJERShift == 2)
        postfix = postfix + "_JERPlus";
    if(dobTagEffShift == -1)
        postfix = postfix + "_bTagMinus";
    if(dobTagEffShift == 1)
        postfix = postfix + "_bTagPlus";
    if(domisTagEffShift == -1)
        postfix = postfix + "_misTagMinus";
    if(domisTagEffShift == 1)
        postfix = postfix + "_misTagPlus";
    if(mvaNegWeight == 1)
        postfix = postfix + "_MVANegWeight";

    ///////////////////////////////////////
    //      Configuration                //
    ///////////////////////////////////////

    string channelpostfix = "";
    string xmlFileName = "";

    bool dilepton = true;
    bool Muon = true;
    bool Electron = false;
    bool HadTopOn = true;
    bool EventBDTOn = true;
    bool TrainMVA = false; // If false, the previously trained MVA will be used to calculate stuff
    bool bx25 = true;
    bool bTagReweight = true;
    bool bTagCSVReweight = true;
    bool bLeptonSF = true;
    bool debug = false;
    bool applyJER = true;
    bool applyJEC = true;
    bool JERNom = false;
    bool JERUp = false;
    bool JERDown = false;
    bool JESUp = false;
    bool JESDown = false;
    bool fillingbTagHistos = false;
    string MVAmethod = "BDT";  // MVAmethod to be used to get the good jet combi calculation (not for training! this is
                               // chosen in the jetcombiner class)
    float Luminosity = 2628.0; // pb^-1 shown is C+D, D only is 2094.08809124; silverJson

    if(dName.find("MuEl") != std::string::npos) {
        Muon = true;
        Electron = true;
    } else if(dName.find("MuMu") != std::string::npos) {
        Muon = true;
        Electron = false;
    } else if(dName.find("ElEl") != std::string::npos) {
        Muon = false;
        Electron = true;
    } else
        cout << "Boolean setting by name failed" << endl;

    if(Muon && Electron && dilepton) {
        cout << " --> Using the Muon-Electron channel..." << endl;
        channelpostfix = "_MuEl";
        xmlFileName = "config/Run2_Samples.xml";
        Luminosity = 2629.548;
    } else if(Muon && !Electron && dilepton) {
        cout << " --> Using the Muon-Muon channel..." << endl;
        channelpostfix = "_MuMu";
        xmlFileName = "config/Run2_Samples.xml";
        Luminosity = 2629.406;
    } else if(!Muon && Electron && dilepton) {
        cout << " --> Using the Electron-Electron channel..." << endl;
        channelpostfix = "_ElEl";
        xmlFileName = "config/Run2_Samples.xml";
        Luminosity = 2627.712;
    } else {
        cerr << "Correct Di-lepton Channel not selected." << endl;
        exit(1);
    }

    bool trainEventMVA = false; // If false, the previously trained MVA will be used to calculate stuff
    bool computeEventMVA = false;

    const char* xmlfile = xmlFileName.c_str();
    cout << "used config file: " << xmlfile << endl;

    /////////////////////////////
    //  Set up AnalysisEnvironment
    /////////////////////////////

    AnalysisEnvironment anaEnv;
    cout << " - Creating environment ..." << endl;
    //    AnalysisEnvironmentLoader anaLoad(anaEnv,xmlfile);
    anaEnv.PrimaryVertexCollection = "PrimaryVertex";
    anaEnv.JetCollection = "PFJets_slimmedJets";
    anaEnv.FatJetCollection = "FatJets_slimmedJetsAK8";
    anaEnv.METCollection = "PFMET_slimmedMETs";
    anaEnv.MuonCollection = "Muons_slimmedMuons";
    anaEnv.ElectronCollection = "Electrons_slimmedElectrons";
    anaEnv.GenJetCollection = "GenJets_slimmedGenJets";
    //    anaEnv.TrackMETCollection = "";
    //    anaEnv.GenEventCollection = "GenEvent";
    anaEnv.NPGenEventCollection = "NPGenEvent";
    anaEnv.MCParticlesCollection = "MCParticles";
    anaEnv.loadFatJetCollection = true;
    anaEnv.loadGenJetCollection = true;
    //    anaEnv.loadGenEventCollection = false;
    anaEnv.loadNPGenEventCollection = false;
    anaEnv.loadMCParticles = true;
    //    anaEnv.loadTrackMETCollection = false;
    anaEnv.JetType = 2;
    anaEnv.METType = 2;
    int verbose = 2; // anaEnv.Verbose;

    ////////////////////////////////
    //  Load datasets
    ////////////////////////////////

    TTreeLoader treeLoader;
    vector<Dataset*> datasets;
    cout << " - Creating Dataset ..." << endl;
    Dataset* theDataset = new Dataset(dName, dTitle, true, color, ls, lw, normf, xSect, vecfileNames);
    theDataset->SetEquivalentLuminosity(EqLumi * normf);
    datasets.push_back(theDataset);
    string dataSetName = theDataset->Name();

    ////////////////////////////////
    // Setting Up Scaling Objects //
    ////////////////////////////////

    //////////////////////////////////////////////////////
    //     bTag calibration reader and weight tools     //
    //////////////////////////////////////////////////////

    BTagCalibration * bTagCalib;   
    BTagCalibrationReader * bTagReader;
    BTagCalibrationReader * bTagReaderUp;
    BTagCalibrationReader * bTagReaderDown;
    BTagCalibrationReader * reader_csvv2; //for csv reshaping 
    BTagCalibrationReader * reader_JESUp;
    BTagCalibrationReader * reader_JESDown;
    BTagCalibrationReader * reader_LFUp;
    BTagCalibrationReader * reader_LFDown;
    BTagCalibrationReader * reader_HFUp;
    BTagCalibrationReader * reader_HFDown;
    BTagCalibrationReader * reader_HFStats1Up;
    BTagCalibrationReader * reader_HFStats1Down;
    BTagCalibrationReader * reader_HFStats2Up;
    BTagCalibrationReader * reader_HFStats2Down;
    BTagCalibrationReader * reader_LFStats1Up;
    BTagCalibrationReader * reader_LFStats1Down;
    BTagCalibrationReader * reader_LFStats2Up;
    BTagCalibrationReader * reader_LFStats2Down;
    BTagCalibrationReader * reader_CFErr1Up;
    BTagCalibrationReader * reader_CFErr1Down;
    BTagCalibrationReader * reader_CFErr2Up;
    BTagCalibrationReader * reader_CFErr2Down;

    BTagWeightTools* btwt;
    BTagWeightTools* btwtUp;
    BTagWeightTools* btwtDown;
    bool isData = false;
    if(dataSetName.find("Data") != string::npos) {
        isData = true;
    }

    if(bTagReweight && dataSetName.find("Data") == string::npos) {
        // Btag documentation : http://mon.iihe.ac.be/~smoortga/TopTrees/BTagSF/BTaggingSF_inTopTrees.pdf //v2 or _v2
        bTagCalib =
            new BTagCalibration("CSVv2", "../TopTreeAnalysisBase/Calibrations/BTagging/CSVv2_76X_combToMujets.csv");
        bTagReader = new BTagCalibrationReader(bTagCalib, BTagEntry::OP_MEDIUM, "mujets", "central");  // mujets
        bTagReaderUp = new BTagCalibrationReader(bTagCalib, BTagEntry::OP_MEDIUM, "mujets", "up");     // mujets
        bTagReaderDown = new BTagCalibrationReader(bTagCalib, BTagEntry::OP_MEDIUM, "mujets", "down"); // mujets

        if(fillingbTagHistos) {
            btwt =
                new BTagWeightTools(bTagReader, "bTagWeightHistosPtEta_" + dataSetName + ".root", false, 20, 670, 2.4);
            btwtUp = new BTagWeightTools(
                bTagReader, "bTagWeightHistosPtEta_" + dataSetName + "_Up.root", false, 20, 670, 2.4);
            btwtDown = new BTagWeightTools(
                bTagReader, "bTagWeightHistosPtEta_" + dataSetName + "_Down.root", false, 20, 670, 2.4);
        } else {
            btwt = new BTagWeightTools(bTagReader, "HistosPtEta_Dilep_Summer16_HadronFlavour.root", false, 20, 670, 2.4);
            btwtUp = new BTagWeightTools(bTagReaderUp, "HistosPtEta_Dilep_Summer16_HadronFlavour.root", false, 20, 670, 2.4);
            btwtDown = new BTagWeightTools(bTagReaderDown, "HistosPtEta_Dilep_Summer16_HadronFlavour.root", false, 20, 670, 2.4);
        }
    }

    if(bTagCSVReweight && !isData){
        BTagCalibration calib_csvv2("csvv2", "../TopTreeAnalysisBase/Calibrations/BTagging/CSVv2_76X_combToMujets.csv");
        reader_csvv2 = new BTagCalibrationReader(&calib_csvv2, // calibration instance
              BTagEntry::OP_RESHAPING, // operating point
              "iterativefit", // measurement type
              "central"); // systematics type

        // JESUp
        reader_JESUp = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_jes"); // systematics type
        // JESDown
        reader_JESDown = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_jes"); // systematics type

        // LFUp
        reader_LFUp = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_lf"); // systematics type
        // LFDown
        reader_LFDown = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_lf"); // systematics type

        // HFUp
        reader_HFUp = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_hf"); // systematics type
        // HFDown
        reader_HFDown = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_hf"); // systematics type

        // HFStats1Up
        reader_HFStats1Up = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_hfstats1"); // systematics type
        // HFStats1Down
        reader_HFStats1Down = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_hfstats1"); // systematics type

        // HFStats2Up
        reader_HFStats2Up = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_hfstats2"); // systematics type
        // HFStats2Down
        reader_HFStats2Down = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_hfstats2"); // systematics type

        // LFStats1Up
        reader_LFStats1Up = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_lfstats1"); // systematics type
        // LFStats1Down
        reader_LFStats1Down = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_lfstats1"); // systematics type

        // LFStats2Up
        reader_LFStats2Up = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_lfstats2"); // systematics type
        // LFStats2Down
        reader_LFStats2Down = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_lfstats2"); // systematics type

        // CFErr1Up
        reader_CFErr1Up = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_cferr1"); // systematics type
        // CFErr1Down
        reader_CFErr1Down = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_cferr1"); // systematics type

        // CFErr2Up
        reader_CFErr2Up = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "up_cferr2"); // systematics type
        // CFErr2Down
        reader_CFErr2Down = new BTagCalibrationReader(&calib_csvv2, // calibration instance
                       BTagEntry::OP_RESHAPING, // operating point
                       "iterativefit", // measurement type
                       "down_cferr2"); // systematics type        
    }

    /////////////////////////////////////////////////
    //                   Lepton SF                 //
    /////////////////////////////////////////////////

    MuonSFWeight* muonSFWeightID;
    MuonSFWeight* muonSFWeightIso;
    ElectronSFWeight* electronSFWeight;
    ElectronSFWeight* electronSFWeightReco;
    if(bLeptonSF) {
        if(Muon) {
            muonSFWeightID =
                new MuonSFWeight("../TopTreeAnalysisBase/Calibrations/LeptonSF/MuonID_Z_RunCD_Reco76X_Feb15.root",
                    "MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/abseta_pt_ratio", true, false, false);
            muonSFWeightIso =
                new MuonSFWeight("../TopTreeAnalysisBase/Calibrations/LeptonSF/MuonIso_Z_RunCD_Reco76X_Feb15.root",
                    "MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/abseta_pt_ratio", true, false, false);
        }
        if(Electron) {
            electronSFWeight = new ElectronSFWeight(
                "../TopTreeAnalysisBase/Calibrations/LeptonSF/CutBasedID_MediumWP_76X_18Feb.txt_SF2D.root",
                "EGamma_SF2D", true, false, false);
            electronSFWeightReco =
                new ElectronSFWeight("../TopTreeAnalysisBase/Calibrations/LeptonSF/eleRECO.txt.egamma_SF2D.root",
                    "EGamma_SF2D", true, false);
        }
    }

    /////////////////////////////////////////////////
    //               Pu reweighting                //
    /////////////////////////////////////////////////
    cout << "Getting lumi weights" << endl;
    LumiReWeighting LumiWeights;
    LumiReWeighting LumiWeights_up;
    LumiReWeighting LumiWeights_down;

    LumiWeights = LumiReWeighting(
        "../TopTreeAnalysisBase/Calibrations/PileUpReweighting/pileup_MC_RunIIFall15DR76-Asympt25ns.root",
        "../TopTreeAnalysisBase/Calibrations/PileUpReweighting/"
        "pileup_2015Data76X_25ns-Run246908-260627Cert_Silver.root",
        "pileup", "pileup");
    LumiWeights_up = LumiReWeighting(
        "../TopTreeAnalysisBase/Calibrations/PileUpReweighting/pileup_MC_RunIIFall15DR76-Asympt25ns.root",
        "../TopTreeAnalysisBase/Calibrations/PileUpReweighting/"
        "pileup_2015Data76X_25ns-Run246908-260627Cert_Silver_up.root",
        "pileup", "pileup");
    LumiWeights_down = LumiReWeighting(
        "../TopTreeAnalysisBase/Calibrations/PileUpReweighting/pileup_MC_RunIIFall15DR76-Asympt25ns.root",
        "../TopTreeAnalysisBase/Calibrations/PileUpReweighting/"
        "pileup_2015Data76X_25ns-Run246908-260627Cert_Silver_down.root",
        "pileup", "pileup");

    // cin.get();

    // TFile* ISRSFFile = TFile::Open("SFHist.root");
    // TH1F* ISRSFHist = (TH1F*)ISRSFFile->Get("nISRJets_TT_Powheg_ScaleUp__1");

    vector<string> MVAvars;

    MVAvars.push_back("topness");
    MVAvars.push_back("LeadLepPt");
    MVAvars.push_back("LeadLepEta");
    MVAvars.push_back("HTH");
    MVAvars.push_back("HTRat");
    MVAvars.push_back("HTb");
    MVAvars.push_back("nLtags");
    MVAvars.push_back("nMtags");
    MVAvars.push_back("nTtags");
    MVAvars.push_back("nJets");
    //    MVAvars.push_back("Jet3Pt");
    //    MVAvars.push_back("Jet4Pt");
    MVAvars.push_back("HT2M");
    MVAvars.push_back("EventSph");
    //    MVAvars.push_back("EventCen");
    //    MVAvars.push_back("DiLepSph");
    //    MVAvars.push_back("DiLepCen");
    //    MVAvars.push_back("TopDiLepSph");
    //    MVAvars.push_back("TopDiLepCen");

    MVAComputer* Eventcomputer_;

    if(dilepton && Muon && Electron) {
        if(mvaNegWeight == 1)
            Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_DiLep_Combined_6thApril2016_NegWeight.root",
                "MasterMVA_DiLep_Combined_6thApril2016_NegWeight", MVAvars, "_DilepCombined6thApril2016_NegWeight");
        else
            Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_DiLep_Combined_6thApril2016_NoWeight.root",
                "MasterMVA_DiLep_Combined_6thApril2016_NoWeight", MVAvars, "_DilepCombined6thApril2016_NoWeight");
        //        Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_MuEl_26thOctober.root",
        //        "MasterMVA_MuEl_26thOctober",
        //            MVAvars, "_MuElOctober26th2015");
    } else if(dilepton && Muon && !Electron) {
        //        Eventcomputer_ = new
        //        MVAComputer("BDT","MVA/MasterMVA_MuMu_9thJuly.root","MasterMVA_MuMu_9thJuly",MVAvars,
        //        "_MuMuJuly9th2015");
        //        Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_MuMu_26thOctober.root",
        //        "MasterMVA_MuMu_26thOctober",
        //            MVAvars, "_MuMuOctober26th2015");
        if(mvaNegWeight == 1)
            Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_DiLep_Combined_6thApril2016_NegWeight.root",
                "MasterMVA_DiLep_Combined_6thApril2016_NegWeight", MVAvars, "_DilepCombined6thApril2016_NegWeight");
        else
            Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_DiLep_Combined_6thApril2016_NoWeight.root",
                "MasterMVA_DiLep_Combined_6thApril2016_NoWeight", MVAvars, "_DilepCombined6thApril2016_NoWeight");

    } else if(dilepton && !Muon && Electron) {
        //        Eventcomputer_ = new
        //        MVAComputer("BDT","MVA/MasterMVA_ElEl_9thJuly.root","MasterMVA_ElEl_9thJuly",MVAvars,
        //        "_ElElJuly9th2015");
        //        Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_ElEl_26thOctober.root",
        //        "MasterMVA_ElEl_26thOctober",
        //            MVAvars, "_ElElOctober26th2015");
        if(mvaNegWeight == 1)
            Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_DiLep_Combined_6thApril2016_NegWeight.root",
                "MasterMVA_DiLep_Combined_6thApril2016_NegWeight", MVAvars, "_DilepCombined6thApril2016_NegWeight");
        else
            Eventcomputer_ = new MVAComputer("BDT", "MVA/MasterMVA_DiLep_Combined_6thApril2016_NoWeight.root",
                "MasterMVA_DiLep_Combined_6thApril2016_NoWeight", MVAvars, "_DilepCombined6thApril2016_NoWeight");
    }

    cout << " Initialized Eventcomputer_" << endl;

    cout << "Instantiating jet combiner..." << endl;

    JetCombiner* jetCombiner = new JetCombiner(TrainMVA, Luminosity, datasets, MVAmethod, false, "", "_13TeV");
    cout << "Instantiated jet combiner..." << endl;

    /////////////////////////////////
    //  Loop over Datasets
    /////////////////////////////////

    cout << "found sample with equivalent lumi " << theDataset->EquivalentLumi() << endl;
    //    if(dataSetName.find("Data")<=0 || dataSetName.find("data")<=0 || dataSetName.find("DATA")<=0)
    //    {
    //        Luminosity = theDataset->EquivalentLumi();
    //        cout <<"found DATA sample with equivalent lumi "<<  theDataset->EquivalentLumi() <<endl;
    //    }

    cout << "Rescaling to an integrated luminosity of " << Luminosity << " pb^-1" << endl;
    int ndatasets = datasets.size() - 1;

    double currentLumi;
    double newlumi;

    // Output ROOT file
    string outputDirectory("MACRO_Output" + channelpostfix);
    mkdir(outputDirectory.c_str(), 0777);
    string rootFileName(outputDirectory + "/FourTop" + postfix + channelpostfix + ".root");
    TFile* fout = new TFile(rootFileName.c_str(), "RECREATE");

    // vector of objects
    cout << " - Variable declaration ..." << endl;
    vector<TRootVertex*> vertex;
    vector<TRootMuon*> init_muons;
    vector<TRootElectron*> init_electrons;
    vector<TRootJet*> init_jets;
    vector<TRootJet*> init_fatjets;
    vector<TRootMET*> mets;

    // Global variable
    TRootEvent* event = 0;

    ////////////////////////////////////////////////////////////////////
    ////////////////// MultiSample plots  //////////////////////////////
    ////////////////////////////////////////////////////////////////////

    MSPlot["NbOfVertices"] = new MultiSamplePlot(datasets, "NbOfVertices", 60, 0, 60, "Nb. of vertices");
    // Muons
    MSPlot["MuonPt"] = new MultiSamplePlot(datasets, "MuonPt", 30, 0, 300, "PT_{#mu}");
    MSPlot["MuonEta"] = new MultiSamplePlot(datasets, "MuonEta", 40, -4, 4, "Muon #eta");
    MSPlot["MuonRelIsolation"] = new MultiSamplePlot(datasets, "MuonRelIsolation", 10, 0, .25, "RelIso");
    MSPlot["InitMuonCutFlow"] = new MultiSamplePlot(datasets, "InitMuonCutFlow", 12, 0, 12, "CutNumber");
    // Electrons
    MSPlot["ElectronRelIsolation"] = new MultiSamplePlot(datasets, "ElectronRelIsolation", 10, 0, .25, "RelIso");
    MSPlot["ElectronPt"] = new MultiSamplePlot(datasets, "ElectronPt", 30, 0, 300, "PT_{e}");
    MSPlot["ElectronEta"] = new MultiSamplePlot(datasets, "ElectronEta", 40, -4, 4, "Jet #eta");
    MSPlot["NbOfElectronsPreSel"] = new MultiSamplePlot(datasets, "NbOfElectronsPreSel", 10, 0, 10, "Nb. of electrons");
    MSPlot["NbOfLooseElectronsPreSel"] =
        new MultiSamplePlot(datasets, "NbOfLooseElectronsPreSel", 10, 0, 10, "Nb. of electrons");
    MSPlot["NbOfMediumElectronsPreSel"] =
        new MultiSamplePlot(datasets, "NbOfMediumElectronsPreSel", 10, 0, 10, "Nb. of electrons");
    MSPlot["NbOfTightElectronsPreSel"] =
        new MultiSamplePlot(datasets, "NbOfTightElectronsPreSel", 10, 0, 10, "Nb. of electrons");
    // Init Electron Plots
    MSPlot["InitElectronPt"] = new MultiSamplePlot(datasets, "InitElectronPt", 30, 0, 300, "PT_{e}");
    MSPlot["InitElectronEta"] = new MultiSamplePlot(datasets, "InitElectronEta", 40, -4, 4, "#eta");
    MSPlot["NbOfElectronsInit"] = new MultiSamplePlot(datasets, "NbOfElectronsInit", 10, 0, 10, "Nb. of electrons");
    MSPlot["InitElectronRelIsolation"] =
        new MultiSamplePlot(datasets, "InitElectronRelIsolation", 10, 0, .25, "RelIso");
    MSPlot["InitElectronSuperClusterEta"] =
        new MultiSamplePlot(datasets, "InitElectronSuperClusterEta", 10, 0, 2.5, "#eta");
    MSPlot["InitElectrondEtaI"] = new MultiSamplePlot(datasets, "InitElectrondEtaI", 20, 0, .05, "#eta");
    MSPlot["InitElectrondPhiI"] = new MultiSamplePlot(datasets, "InitElectrondPhiI", 20, 0, .2, "#phi");
    MSPlot["InitElectronHoverE"] = new MultiSamplePlot(datasets, "InitElectronHoverE", 10, 0, .15, "H/E");
    MSPlot["InitElectrond0"] = new MultiSamplePlot(datasets, "InitElectrond0", 20, 0, .1, "d0");
    MSPlot["InitElectrondZ"] = new MultiSamplePlot(datasets, "InitElectrondZ", 10, 0, .25, "dZ");
    MSPlot["InitElectronEminusP"] = new MultiSamplePlot(datasets, "InitElectronEminusP", 10, 0, .25, "1/GeV");
    MSPlot["InitElectronConversion"] =
        new MultiSamplePlot(datasets, "InitElectronConversion", 2, 0, 2, "Conversion Pass");
    MSPlot["InitElectronMissingHits"] =
        new MultiSamplePlot(datasets, "InitElectronMissingHits", 10, 0, 10, "MissingHits");
    MSPlot["InitElectronCutFlow"] = new MultiSamplePlot(datasets, "InitElectronCutFlow", 12, 0, 12, "CutNumber");
    MSPlot["InitElectronDiagRelIso"] = new MultiSamplePlot(datasets, "InitElectronDiagRelIso", 100, 0, 1, "RelIso");
    MSPlot["InitElectronDiagChIso"] = new MultiSamplePlot(datasets, "InitElectronDiagChIso", 100, 0, 10, "RelIso");
    MSPlot["InitElectronDiagNIso"] = new MultiSamplePlot(datasets, "InitElectronDiagNIso", 100, 0, 10, "RelIso");
    MSPlot["InitElectronDiagPhIso"] = new MultiSamplePlot(datasets, "InitElectronDiagPhIso", 100, 0, 10, "RelIso");
    MSPlot["InitElectronDiagPUChIso"] = new MultiSamplePlot(datasets, "InitElectronDiagPUChIso", 100, 0, 10, "RelIso");
    MSPlot["CloseElRelIso"] = new MultiSamplePlot(datasets, "CloseElRelIso", 20, 0, 0.2, "RelIso");
    MSPlot["CloseElChIso"] = new MultiSamplePlot(datasets, "CloseElChIso", 100, 0, 10, "ChIso");
    MSPlot["CloseElNIso"] = new MultiSamplePlot(datasets, "CloseElNIso", 100, 0, 10, "NIso");
    MSPlot["CloseElPhIso"] = new MultiSamplePlot(datasets, "CloseElPhIso", 100, 0, 10, "PhIso");
    MSPlot["CloseElPUChIso"] = new MultiSamplePlot(datasets, "CloseElPUChIso", 100, 0, 10, "PUChIso");
    // General Lepton Plots
    MSPlot["CloseLepPt"] = new MultiSamplePlot(datasets, "CloseLepPt", 10, -0, 300, "GeV");
    MSPlot["CloseLepEta"] = new MultiSamplePlot(datasets, "CloseLepEta", 50, -2.5, 2.5, "#eta");

    // B-tagging discriminators
    MSPlot["BdiscBJetCand_CSV"] = new MultiSamplePlot(datasets, "BdiscBJetCand_CSV", 20, 0, 1, "CSV b-disc.");
    // Jets
    MSPlot["JetEta"] = new MultiSamplePlot(datasets, "JetEta", 40, -4, 4, "Jet #eta");
    MSPlot["3rdJetPt"] = new MultiSamplePlot(datasets, "3rdJetPt", 30, 0, 300, "PT_{jet3}");
    MSPlot["4thJetPt"] = new MultiSamplePlot(datasets, "4thJetPt", 30, 0, 300, "PT_{jet4}");
    MSPlot["5thJetPt"] = new MultiSamplePlot(datasets, "5thJetPt", 30, 0, 300, "PT_{jet5}");
    MSPlot["6thJetPt"] = new MultiSamplePlot(datasets, "6thJetPt", 30, 0, 300, "PT_{jet6}");
    MSPlot["HT_SelectedJets"] = new MultiSamplePlot(datasets, "HT_SelectedJets", 30, 0, 1500, "HT");
    MSPlot["HTExcess2M"] = new MultiSamplePlot(datasets, "HTExcess2M", 30, 0, 1500, "HT_{Excess 2 M b-tags}");
    MSPlot["HTH"] = new MultiSamplePlot(datasets, "HTH", 20, 0, 1, "HTH");
    MSPlot["PreselJetLepdR"] = new MultiSamplePlot(datasets, "PreselJetLepdR", 40, 0, 0.4, "#Delta R");
    MSPlot["PreselJetLepdPhi"] = new MultiSamplePlot(datasets, "PreselJetLepdPhi", 40, 0, 0.4, "#Delta #phi");
    MSPlot["PreselDirtyJetLepdR"] = new MultiSamplePlot(datasets, "PreselDirtyJetLepdR", 40, 0, 0.4, "#Delta R");
    MSPlot["PreselDirtyJetLepdPhi"] = new MultiSamplePlot(datasets, "PreselDirtyJetLepdPhi", 40, 0, 0.4, "#Delta #phi");
    MSPlot["CloseJetLepRat"] = new MultiSamplePlot(datasets, "CloseJetLepRat", 20, 0, 10, "p^{jet}_{T}/p^{l}_{T}");
    MSPlot["CloseJetMC"] = new MultiSamplePlot(datasets, "CloseJetMC", 28, -6, 22, "PDG ID");
    MSPlot["CloseJetHOverE"] = new MultiSamplePlot(datasets, "CloseJetHOverE", 25, -0, 1.0, "Jet H/E");
    // Init Jet Plots
    MSPlot["ISRJetPt"] = new MultiSamplePlot(datasets, "ISRJetPt", 30, 0, 300, "PT");
    MSPlot["nISRJets"] = new MultiSamplePlot(datasets, "nISRJets", 25, 0, 25, "nJets_{ISR}");
    // MET
    MSPlot["MET"] = new MultiSamplePlot(datasets, "MET", 70, 0, 700, "MET");
    MSPlot["METCutAcc"] = new MultiSamplePlot(datasets, "METCutAcc", 30, 0, 150, "MET cut pre-jet selection");
    MSPlot["METCutRej"] = new MultiSamplePlot(datasets, "METCutRej", 30, 0, 150, "MET cut pre-jet selection");
    // Topology Plots
    MSPlot["TotalSphericity"] = new MultiSamplePlot(datasets, "TotalSphericity", 20, 0, 1, "Sphericity_{all}");
    MSPlot["TotalCentrality"] = new MultiSamplePlot(datasets, "TotalCentrality", 20, 0, 1, "Centrality_{all}");
    MSPlot["DiLepSphericity"] = new MultiSamplePlot(datasets, "DiLepSphericity", 20, 0, 1, "Sphericity_{ll}");
    MSPlot["DiLepCentrality"] = new MultiSamplePlot(datasets, "DiLepCentrality", 20, 0, 1, "Centrality_{ll}");
    MSPlot["TopDiLepSphericity"] = new MultiSamplePlot(datasets, "TopDiLepSphericity", 20, 0, 1, "Sphericity_{tll}");
    MSPlot["TopDiLepCentrality"] = new MultiSamplePlot(datasets, "TopDiLepCentrality", 20, 0, 1, "Centrality_{tll}");
    // MVA Top Roconstruction Plots
    MSPlot["MVA1TriJet"] = new MultiSamplePlot(datasets, "MVA1TriJet", 30, -1.0, 0.2, "MVA1TriJet");
    MSPlot["MVA1TriJetMass"] = new MultiSamplePlot(datasets, "MVA1TriJetMass", 75, 0, 500, "m_{bjj}");
    MSPlot["MVA1DiJetMass"] = new MultiSamplePlot(datasets, "MVA1DiJetMass", 75, 0, 500, "m_{bjj}");
    MSPlot["MVA1PtRat"] = new MultiSamplePlot(datasets, "MVA1PtRat", 25, 0, 2, "P_{t}^{Rat}");
    MSPlot["MVA1BTag"] = new MultiSamplePlot(datasets, "MVA1BTag", 35, 0, 1, "BTag");
    MSPlot["MVA1AnThBh"] = new MultiSamplePlot(datasets, "MVA1AnThBh", 35, 0, 3.14, "AnThBh");
    MSPlot["MVA1AnThWh"] = new MultiSamplePlot(datasets, "MVA1AnThWh", 35, 0, 3.14, "AnThWh");
    MSPlot["MVA1dPhiThDiLep"] = new MultiSamplePlot(datasets, "MVA1dPhiThDiLep", 35, 0, 3.14, "dPhiThDiLep");
    MSPlot["MVA1dRThDiLep"] = new MultiSamplePlot(datasets, "MVA1dRThDiLep", 35, 0, 3.14, "dRThDiLep");

    // ZMass window plots
    MSPlot["ZMassWindowWidthAcc"] =
        new MultiSamplePlot(datasets, "ZMassWindowWidthAcc", 20, 0, 100, "Z mass window width");
    MSPlot["ZMassWindowWidthRej"] =
        new MultiSamplePlot(datasets, "ZMassWindowWidthRej", 20, 0, 100, "Z mass window width");
    MSPlot["PreselDiLepMass"] = new MultiSamplePlot(datasets, "PreselDiLepMass", 30, 0, 150, "m_{ll}");
    MSPlot["PostselDiLepMass"] = new MultiSamplePlot(datasets, "PostselDiLepMass", 30, 0, 150, "m_{ll}");

    // n-1 Cut Plots
    MSPlot["NMinusOne"] = new MultiSamplePlot(datasets, "NMinusOne", 8, 0, 8, "CutNumber");

    ///////////////////
    // 1D histograms //
    ///////////////////
    //	TH1F* puHisto = new TH1F("pileup","pileup",60,0,60);
    ///////////////////
    // 2D histograms //
    ///////////////////
    histo2D["HTLepSep"] = new TH2F("HTLepSep", "dR_{ll}:HT", 50, 0, 1000, 20, 0, 4);
    histo2D["nTightnEl"] = new TH2F("nElnTight", "nElectrons:nTight", 5, 0, 5, 8, 0, 8);
    histo2D["SumXnEl"] = new TH2F("SumXnEl", "nElectrons:Sum(nXEl)", 8, 0, 8, 8, 0, 8);
    histo2D["CloseJetdRvsIso"] = new TH2F("CloseJetdRvsIso", "PFIso:#Delta R", 20, 0, 1, 20, 0, 0.15);
    histo2D["CloseJetdRvsdPhi"] = new TH2F("CloseJetdRvsdPhi", "#Delta #Phi:#Delta R", 60, 0, 3, 20, 0, 0.4);
    histo2D["CloseDirtyJetdRvsdPhi"] = new TH2F("CloseDirtyJetdRvsdPhi", "#Delta #Phi:#Delta R", 60, 0, 3, 20, 0, 0.4);

    // Plots
    string pathPNG = "MSPlots_FourTop" + postfix + channelpostfix;
    pathPNG += "_MSPlots/";
    // pathPNG = pathPNG +"/";
    mkdir(pathPNG.c_str(), 0777);

    cout << "Making directory :" << pathPNG << endl;
    vector<string> CutsselecTable;
    if(dilepton) {
        /////////////////////////////////
        // Selection table: Dilepton + jets
        /////////////////////////////////
        if(Muon && Electron) {
            CutsselecTable.push_back(string("initial"));
            CutsselecTable.push_back(string("Event cleaning and Trigger"));
            CutsselecTable.push_back(string("At Least 1 Loose Isolated Muon"));
            CutsselecTable.push_back(string("At Least 1 Tight Electron"));
            CutsselecTable.push_back(string("At least 4 Jets"));
            CutsselecTable.push_back(string("At least 1 CSVM Jet"));
            CutsselecTable.push_back(string("At least 2 CSVM Jets"));
            CutsselecTable.push_back(string("At Least 500 GeV HT"));
        }
        if(Muon && !Electron) {
            CutsselecTable.push_back(string("initial"));
            CutsselecTable.push_back(string("Event cleaning and Trigger"));
            CutsselecTable.push_back(string("At Least 2 Loose Isolated Muon"));
            CutsselecTable.push_back(string("Z Mass Veto"));
            CutsselecTable.push_back(string("At least 4 Jets"));
            CutsselecTable.push_back(string("At least 1 CSVM Jet"));
            CutsselecTable.push_back(string("At least 2 CSVM Jets"));
            CutsselecTable.push_back(string("At Least 500 GeV HT"));
        }
        if(!Muon && Electron) {
            CutsselecTable.push_back(string("initial"));
            CutsselecTable.push_back(string("Event cleaning and Trigger"));
            CutsselecTable.push_back(string("At Least 2 Electrons (1 Tight)"));
            CutsselecTable.push_back(string("Z Mass Veto"));
            CutsselecTable.push_back(string("At least 4 Jets"));
            CutsselecTable.push_back(string("At least 1 CSVM Jet"));
            CutsselecTable.push_back(string("At least 2 CSVM Jets"));
            CutsselecTable.push_back(string("At Least 500 GeV HT"));
        }
    }

    SelectionTable selecTable(CutsselecTable, datasets);
    selecTable.SetLuminosity(Luminosity);
    selecTable.SetPrecision(1);

    /////////////////////////////////
    // Loop on datasets
    /////////////////////////////////

    cout << " - Loop over datasets ... " << datasets.size() << " datasets !" << endl;

    for(unsigned int d = 0; d < datasets.size(); d++) {
        cout << "Load Dataset" << endl;
        treeLoader.LoadDataset(datasets[d], anaEnv); // open files and load dataset
        string previousFilename = "";
        int iFile = -1;
        bool nlo = false;
        dataSetName = datasets[d]->Name();

        //////////////////////////////////////////////
        // Steering based on DataSet Name	    //
        //////////////////////////////////////////////

        if(dataSetName.find("JERDown") != string::npos) {
            JERDown = true;
        } else if(dataSetName.find("JERUp") != string::npos) {
            JERUp = true;
        } else {
            JERNom = true;
        }

        if(dataSetName.find("JESDown") != string::npos) {
            JESDown = true;
        } else if(dataSetName.find("JESUp") != string::npos) {
            JESUp = true;
        }

        if(dataSetName.find("bx50") != std::string::npos)
            bx25 = false;
        else
            bx25 = true;

        if(dataSetName.find("NLO") != std::string::npos || dataSetName.find("nlo") != std::string::npos)
            nlo = true;
        else
            nlo = false;

        if(bx25)
            cout << "Dataset with 25ns Bunch Spacing!" << endl;
        else
            cout << "Dataset with 50ns Bunch Spacing!" << endl;
        if(nlo)
            cout << "NLO Dataset!" << endl;
        else
            cout << "LO Dataset!" << endl;

        //////////////////////////////////////////////
        // Setup Date string and nTuple for output  //
        //////////////////////////////////////////////

        time_t t = time(0); // get time now
        struct tm* now = localtime(&t);

        int year = now->tm_year + 1900;
        int month = now->tm_mon + 1;
        int day = now->tm_mday;
        int hour = now->tm_hour;
        int min = now->tm_min;
        int sec = now->tm_sec;

        string year_str;
        string month_str;
        string day_str;
        string hour_str;
        string min_str;
        string sec_str;

        ostringstream convert; // stream used for the conversion
        convert << year;       // insert the textual representation of 'Number' in the characters in the stream
        year_str = convert.str();
        convert.str("");
        convert.clear();
        convert << month; // insert the textual representation of 'Number' in the characters in the stream
        month_str = convert.str();
        convert.str("");
        convert.clear();
        convert << day; // insert the textual representation of 'Number' in the characters in the stream
        day_str = convert.str();
        convert.str("");
        convert.clear();
        convert << hour; // insert the textual representation of 'Number' in the characters in the stream
        hour_str = convert.str();
        convert.str("");
        convert.clear();
        convert << min; // insert the textual representation of 'Number' in the characters in the stream
        min_str = convert.str();
        convert.str("");
        convert.clear();
        convert << day; // insert the textual representation of 'Number' in the characters in the stream
        sec_str = convert.str();
        convert.str("");
        convert.clear();

        string date_str = day_str + "_" + month_str + "_" + year_str;

        cout << "DATE STRING   " << date_str << endl;

        // string dataSetName = datasets[d]->Name();
        string channel_dir = "Craneens" + channelpostfix;
        string date_dir = channel_dir + "/Craneens" + date_str + "/";
        int mkdirstatus = mkdir(channel_dir.c_str(), 0777);
        mkdirstatus = mkdir(date_dir.c_str(), 0777);

        //     string Ntupname = "Craneens/Craneen_" + dataSetName +postfix + "_" + date_str+  ".root";

        string Ntupname = "Craneens" + channelpostfix + "/Craneens" + date_str + "/" + dataSetName + postfix + ".root";
        string Ntuptitle = "Craneen_" + channelpostfix;
        string elTuptitle = "Craneen_" + channelpostfix + "_ElecID";
        string muTuptitle = "Craneen_" + channelpostfix + "_MuonID";
        string jetTuptitle = "Craneen_" + channelpostfix + "_JetID";
        string cutTuptitle = "Craneen_" + channelpostfix + "_CutFlow";
        string posTuptitle = "posCraneen_" + channelpostfix;
        string negTuptitle = "negCraneen_" + channelpostfix;
        string sfTuptitle = "ScaleFactors_" + channelpostfix;

        TFile* tupfile = new TFile(Ntupname.c_str(), "RECREATE");
        //        TFile * tupMfile = new TFile(NMtupname.c_str(),"RECREATE");

        // TNtuple * tup = new
        // TNtuple(Ntuptitle.c_str(),Ntuptitle.c_str(),"nJets:nLtags:nMtags:nTtags:HT:LeadingMuonPt:LeadingMuonEta:LeadingElectronPt:LeadingBJetPt:HT2M:HTb:HTH:HTRat:topness:ScaleFactor:PU:NormFactor:Luminosity:GenWeight");

        TNtuple* tup = new TNtuple(Ntuptitle.c_str(), Ntuptitle.c_str(),
            "BDT:nJets:nFatJets:nWTags:nTopTags:nLtags:nMtags:nTtags:1stJetPt:2ndJetPt:3rdJetPt:4thJetPt:5thJetPt:"
            "6thJetPt:7thJetPt:8thJetPt:MET:HT:"
            "LeadingLepPt:LeadingLepEta:SubLeadingLepPt:dRLep:AbsSumCharge:LepFlavor:nLep:LeadingBJetPt:dRbb:HT2M:HTb:"
            "HTH:HTRat:topness:EventSph:EventCen:"
            "DiLepSph:DiLepCen:TopDiLepSph:TopDiLepCen:fnjetW:ttbar_flav:SFlepton:SFbtagCSV:SFbtag_light:SFbtag_lightUp:SFbtag_"
            "lightDown:SFbtag_heavy:SFbtag_heavyUp:SFbtag_heavyDown:SFPU:SFPU_up:SFPU_"
            "down:"
            "btagWeightCSVLFUp:btagWeightCSVLFDown:btagWeightCSVHFUp:btagWeightCSVHFDown:btagWeightCSVHFStats1Up:btagWeightCSVHFStats1Down:btagWeightCSVHFStats2Up:btagWeightCSVHFStats2Down:btagWeightCSVLFStats1Up:"
            "btagWeightCSVLFStats1Down:btagWeightCSVLFStats2Up:btagWeightCSVLFStats2Down:btagWeightCSVCFErr1Up:btagWeightCSVCFErr1Down:btagWeightCSVCFErr2Up:btagWeightCSVCFErr2Down:"
            "SFalphaTune:SFtopPt:SFISR:ScaleFactor:PU:NormFactor:Luminosity:GenWeight:weight1:weight2:weight3:"
            "weight4:weight5:weight6:weight7:weight8:diLepMass:RunNumber:LumiBlock:EventNumber");
        TNtuple* eltup = new TNtuple(elTuptitle.c_str(), elTuptitle.c_str(),
            "ScaleFactor:NormFactor:Luminosity:ElSuperclusterEta:Elfull5x5:EldEdatIn:EldPhiIn:ElhOverE:ElRelIso:ElEmP:"
            "Eld0:Eldz:ElMissingHits:ElPt");
        TNtuple* mutup = new TNtuple(
            muTuptitle.c_str(), muTuptitle.c_str(), "ScaleFactor:NormFactor:Luminosity:MuPt:MuEta:MuRelIso:nMuons");
        TNtuple* jettup = new TNtuple(jetTuptitle.c_str(), jetTuptitle.c_str(),
            "ScaleFactor:NormFactor:Luminosity:NHF:NEMF:nConstituents:CHF:CMultiplicity:CEMF");
        TNtuple* cuttup = new TNtuple(cutTuptitle.c_str(), cutTuptitle.c_str(),
            "ScaleFactor:NormFactor:Luminosity:nJets:ttbar_flav:trigger:isGoodPV:Lep1:Lep2:nJets2:nJets3:nJets4:nTags:HT");
        TNtuple* posTup = new TNtuple(
            posTuptitle.c_str(), posTuptitle.c_str(), "nJets:HT:ScaleFactor:NormFactor:Luminosity:CentralWeight");
        TNtuple* negTup = new TNtuple(
            negTuptitle.c_str(), negTuptitle.c_str(), "nJets:HT:ScaleFactor:NormFactor:Luminosity:CentralWeight");
        TNtuple* sfTup = new TNtuple(sfTuptitle.c_str(), sfTuptitle.c_str(),
            "sfLep1:sfLep2:sfBtag:sfPU:sfTopPt:sfISR:ScaleFactor:NormFactor:Luminosity");

        //////////////////////////////////////////////////
        /// Initialize JEC factors ///////////////////////
        //////////////////////////////////////////////////

        vector<JetCorrectorParameters> vCorrParam;
        string pathCalJEC = "../TopTreeAnalysisBase/Calibrations/JECFiles/";

        if(isData) {
            JetCorrectorParameters* L1JetCorPar =
                new JetCorrectorParameters(pathCalJEC + "Fall15_25nsV2_DATA_L1FastJet_AK4PFchs.txt");
            vCorrParam.push_back(*L1JetCorPar);
            JetCorrectorParameters* L2JetCorPar =
                new JetCorrectorParameters(pathCalJEC + "Fall15_25nsV2_DATA_L2Relative_AK4PFchs.txt");
            vCorrParam.push_back(*L2JetCorPar);
            JetCorrectorParameters* L3JetCorPar =
                new JetCorrectorParameters(pathCalJEC + "Fall15_25nsV2_DATA_L3Absolute_AK4PFchs.txt");
            vCorrParam.push_back(*L3JetCorPar);
            JetCorrectorParameters* L2L3ResJetCorPar =
                new JetCorrectorParameters(pathCalJEC + "Fall15_25nsV2_DATA_L2L3Residual_AK4PFchs.txt");
            vCorrParam.push_back(*L2L3ResJetCorPar);
        } else {
            JetCorrectorParameters* L1JetCorPar =
                new JetCorrectorParameters(pathCalJEC + "Fall15_25nsV2_MC_L1FastJet_AK4PFchs.txt");
            vCorrParam.push_back(*L1JetCorPar);
            JetCorrectorParameters* L2JetCorPar =
                new JetCorrectorParameters(pathCalJEC + "Fall15_25nsV2_MC_L2Relative_AK4PFchs.txt");
            vCorrParam.push_back(*L2JetCorPar);
            JetCorrectorParameters* L3JetCorPar =
                new JetCorrectorParameters(pathCalJEC + "Fall15_25nsV2_MC_L3Absolute_AK4PFchs.txt");
            vCorrParam.push_back(*L3JetCorPar);
        }
        JetCorrectionUncertainty* jecUnc =
            new JetCorrectionUncertainty(pathCalJEC + "Fall15_25nsV2_MC_Uncertainty_AK4PFchs.txt");

        JetTools* jetTools = new JetTools(vCorrParam, jecUnc, true);

        //////////////////////////////////////////////////
        // Loop on events
        /////////////////////////////////////////////////

        vector<int> itrigger;
        int previousRun = -1;

        int start = 0;
        cout << "teh bugz!" << endl;
        unsigned int ending = datasets[d]->NofEvtsToRunOver();

        cout << "Number of events in total dataset = " << ending << endl;

        int event_start = 0;
        if(verbose > 1)
            cout << " - Loop over events " << endl;

        float BDTScore, MHT, MHTSig, STJet, muoneta, muonpt, electronpt, bjetpt, EventMass, EventMassX, SumJetMass,
            SumJetMassX, H, HX, HTHi, HTRat, HT, HTX, HTH, HTXHX, sumpx_X, sumpy_X, sumpz_X, sume_X, sumpx, sumpy,
            sumpz, sume, jetpt, PTBalTopEventX, PTBalTopSumJetX, PTBalTopMuMet, dRLep, dRbb, lepFlavor;

        double currentfrac = 0.;
        double end_d = ending;
        //        if(endEvent > ending)
        //            end_d = ending;
        //        else
        //            end_d = endEvent;
        //
        // end_d = 10000; //artifical ending for debug
        int nEvents = end_d - event_start;
        cout << "Will run over " << (end_d - event_start) << " events..." << endl;
        cout << "Starting event = = = = " << event_start << endl;
        if(end_d < event_start) {
            cout << "Starting event larger than number of events.  Exiting." << endl;
            return 1;
        }

        TRootRun* runInfos = new TRootRun();
        datasets[d]->runTree()->SetBranchStatus("runInfos*", 1);
        datasets[d]->runTree()->SetBranchAddress("runInfos", &runInfos);

        TopologyWorker* topologyW = new TopologyWorker(false);

        ////////////////////////////////////////
        /// Set up Behrends Scaling Functions //
        ////////////////////////////////////////

        float dataPar1 = 0.11535, dataPar2 = 0.2397, mcPar1 = 0.11535, mcPar2 = 0.2888;

        TF1* DataFit = new TF1("DataFit", "0.11535*0.2397**(x-5)", 5, 15);
        TF1* MCFit = new TF1("DataFit", "0.11535*0.2888**(x-5)", 5, 15);

        //////////////////////////////////////
        // Begin Event Loop
        //////////////////////////////////////

        for(unsigned int ievt = event_start; ievt < end_d; ievt++) {

            // define object containers
            vector<TRootElectron *> selectedElectrons, selectedLooseElectrons, selectedMediumElectrons,
                selectedTightElectrons;
            vector<TRootElectron *> selectedLooseXElectrons, selectedMediumXElectrons, selectedPosTightElectrons,
                selectedNegTightElectrons, selectedNTightElectrons;
            vector<TRootPFJet *> selectedJets, selectedUncleanedJets;
            vector<TRootPFJet*> selectedLooseJets;
            vector<TRootSubstructureJet*> selectedFatJets;
            vector<TRootPFJet*> MVASelJets1;
            vector<TRootMuon *> selectedMuons, selectedPosMuons, selectedNegMuons;
            vector<TRootElectron*> selectedExtraElectrons;
            vector<TRootMuon*> selectedExtraMuons;
            selectedElectrons.reserve(10);
            selectedMuons.reserve(10);

            BDTScore = -99999.0, MHT = 0., MHTSig = 0., muoneta = 0., muonpt = 0., electronpt = 0., bjetpt = 0.,
            STJet = 0., EventMass = 0., EventMassX = 0., SumJetMass = 0., SumJetMassX = 0., HTHi = 0., HTRat = 0;
            H = 0., HX = 0., HT = 0., HTX = 0., HTH = 0., HTXHX = 0., sumpx_X = 0., sumpy_X = 0., sumpz_X = 0.,
            sume_X = 0., sumpx = 0., sumpy = 0., sumpz = 0., sume = 0., jetpt = 0., PTBalTopEventX = 0.,
            PTBalTopSumJetX = 0., dRLep = 0, dRbb = 0, lepFlavor = 0;

            double ievt_d = ievt;
            float centralWeight = 1, scaleUp = 1, scaleDown = 1, weight1 = 1, weight2 = 1, weight3 = 1, weight4 = 1,
                  weight5 = 1, weight6 = 1, weight7 = 1, weight8 = 1;
            currentfrac = ievt_d / end_d;
            if(debug)
                cout << "event loop 1" << endl;

            if(ievt % 1000 == 0) {
                std::cout << "Processing the " << ievt
                          << "th event, time = " << ((double)clock() - start) / CLOCKS_PER_SEC << " ("
                          << 100 * (ievt - start) / (ending - start) << "%)" << flush << "\r" << endl;
            }

            float scaleFactor = 1.; // scale factor for the event
            event = treeLoader.LoadEvent(
                ievt, vertex, init_muons, init_electrons, init_jets, init_fatjets, mets, debug); // load event

            float nvertices = vertex.size();
            float normfactor = 1.0;
            if(dataSetName.find("tttt") != string::npos)
                normfactor = 1.0 / 0.4095;
            datasets[d]->eventTree()->LoadTree(ievt);
            string currentFilename = datasets[d]->eventTree()->GetFile()->GetName();
            if(previousFilename != currentFilename) {
                previousFilename = currentFilename;
                iFile++;
                cout << "File changed!!! => " << currentFilename << endl;
            }

            //	    if(dataSetName.find("Data")!=std::string::npos)
            //	    {
            //		puHisto->Fill((int)nvertices);
            //	    }
            //	    else
            //	    {
            //		puHisto->Fill((int)event->nTruePU());
            //	    }

            // cout<<"SetBranchAddress(runInfos,&runInfos) :
            // "<<datasets[d]->runTree()->SetBranchAddress("runInfos",&runInfos)<<endl;
            int rBytes = datasets[d]->runTree()->GetEntry(iFile);

            int currentRun = event->runId();
            int currentEvent = event->eventId();
            int LumiBlock = event->lumiBlockId();

            if(dataSetName.find("TTDileptMG") != std::string::npos ||
                dataSetName.find("TTJetsMG") != std::string::npos) {
                centralWeight = (event->getWeight(1)) / (abs(event->originalXWGTUP()));
                weight1 = event->getWeight(2) / (abs(event->originalXWGTUP()));
                weight2 = event->getWeight(3) / (abs(event->originalXWGTUP()));
                weight3 = event->getWeight(4) / (abs(event->originalXWGTUP()));
                weight4 = event->getWeight(5) / (abs(event->originalXWGTUP()));
                weight5 = event->getWeight(6) / (abs(event->originalXWGTUP()));
                weight6 = event->getWeight(7) / (abs(event->originalXWGTUP()));
                weight7 = event->getWeight(8) / (abs(event->originalXWGTUP()));
                weight8 = event->getWeight(9) / (abs(event->originalXWGTUP()));

                // cout <<"Central Weight Index: " << runInfos->getWeightInfo(currentRun).weightIndex("Central scale
                // variation 1") << " Weight : " << centralWeight <<endl;
                // cout <<"Scale Up Weight Index: " << runInfos->getWeightInfo(currentRun).weightIndex("Central scale
                // variation 5") << " Weight : " << scaleUp <<endl;
                // cout <<"Scale Down Weight Index: " << runInfos->getWeightInfo(currentRun).weightIndex("Central scale
                // variation 9") << " Weight : " << scaleDown <<endl;
            } else if(dataSetName.find("TTJetsPowheg") != std::string::npos ||
                dataSetName.find("ttttNLO") != std::string::npos) {
                centralWeight = (event->getWeight(1001)) / (abs(event->originalXWGTUP()));
                weight1 = event->getWeight(1002) / (abs(event->originalXWGTUP()));
                weight2 = event->getWeight(1003) / (abs(event->originalXWGTUP()));
                weight3 = event->getWeight(1004) / (abs(event->originalXWGTUP()));
                weight4 = event->getWeight(1005) / (abs(event->originalXWGTUP()));
                weight5 = event->getWeight(1006) / (abs(event->originalXWGTUP()));
                weight6 = event->getWeight(1007) / (abs(event->originalXWGTUP()));
                weight7 = event->getWeight(1008) / (abs(event->originalXWGTUP()));
                weight8 = event->getWeight(1009) / (abs(event->originalXWGTUP()));

                //                cout << "Unscaled Central Weight: " << event->getWeight(1001) << " originalXWGTUP: "
                //                << event->originalXWGTUP() << endl;

                // cout <<"Central Weight Index: " << runInfos->getWeightInfo(currentRun).weightIndex("scale_variation
                // 1") << " Weight : " << centralWeight <<endl;
                // cout <<"Scale Up Weight Index: " << runInfos->getWeightInfo(currentRun).weightIndex("scale_variation
                // 5") << " Weight : " << scaleUp <<endl;
                // cout <<"Scale Down Weight Index: " <<
                // runInfos->getWeightInfo(currentRun).weightIndex("scale_variation 9") << " Weight : " << scaleDown
                // <<endl;
            }

            float rho = event->fixedGridRhoFastjetAll();
            if(debug)
                cout << "Rho: " << rho << endl;

            if(debug)
                cout << "Number of Muons Loaded: " << init_muons.size() << endl;

            for(Int_t initmu = 0; initmu < init_muons.size(); initmu++) {
                float initreliso = init_muons[initmu]->relPfIso(4, 0.5);
                MSPlot["InitMuonCutFlow"]->Fill(0, datasets[d], true, Luminosity * scaleFactor);
                if(init_muons[initmu]->isGlobalMuon() && init_muons[initmu]->isPFMuon()) {
                    MSPlot["InitMuonCutFlow"]->Fill(1, datasets[d], true, Luminosity * scaleFactor);
                    if(fabs(init_muons[initmu]->Pt()) < 20) {
                        MSPlot["InitMuonCutFlow"]->Fill(2, datasets[d], true, Luminosity * scaleFactor);
                        if(fabs(init_muons[initmu]->Eta()) < 2.4) {
                            MSPlot["InitMuonCutFlow"]->Fill(3, datasets[d], true, Luminosity * scaleFactor);
                            if(fabs(init_muons[initmu]->chi2()) < 10) {
                                MSPlot["InitMuonCutFlow"]->Fill(4, datasets[d], true, Luminosity * scaleFactor);
                                if(fabs(init_muons[initmu]->nofTrackerLayersWithMeasurement()) > 5) {
                                    MSPlot["InitMuonCutFlow"]->Fill(5, datasets[d], true, Luminosity * scaleFactor);
                                    if(fabs(init_muons[initmu]->nofValidMuHits()) > 0) {
                                        MSPlot["InitMuonCutFlow"]->Fill(6, datasets[d], true, Luminosity * scaleFactor);
                                        if(fabs(init_muons[initmu]->d0()) < 0.2) {
                                            MSPlot["InitMuonCutFlow"]->Fill(
                                                7, datasets[d], true, Luminosity * scaleFactor);
                                            if(fabs(init_muons[initmu]->dz()) < 0.5) {
                                                MSPlot["InitMuonCutFlow"]->Fill(
                                                    8, datasets[d], true, Luminosity * scaleFactor);
                                                if(init_muons[initmu]->nofValidPixelHits() > 0) {
                                                    MSPlot["InitMuonCutFlow"]->Fill(
                                                        9, datasets[d], true, Luminosity * scaleFactor);
                                                    if(init_muons[initmu]->nofMatchedStations() > 1) {
                                                        MSPlot["InitMuonCutFlow"]->Fill(
                                                            10, datasets[d], true, Luminosity * scaleFactor);
                                                        if(initreliso < 0.12) {
                                                            MSPlot["InitMuonCutFlow"]->Fill(
                                                                11, datasets[d], true, Luminosity * scaleFactor);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if(debug)
                cout << "Number of Electrons Loaded: " << init_electrons.size() << endl;
            MSPlot["NbOfElectronsInit"]->Fill(init_electrons.size(), datasets[d], true, Luminosity * scaleFactor);

            for(Int_t initel = 0; initel < init_electrons.size(); initel++) {
                float initreliso = ElectronRelIso(init_electrons[initel], rho);
                MSPlot["InitElectronPt"]->Fill(
                    init_electrons[initel]->Pt(), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectronEta"]->Fill(
                    init_electrons[initel]->Eta(), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectronRelIsolation"]->Fill(initreliso, datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectronSuperClusterEta"]->Fill(
                    fabs(init_electrons[initel]->superClusterEta()), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectrondEtaI"]->Fill(
                    fabs(init_electrons[initel]->deltaEtaIn()), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectrondPhiI"]->Fill(
                    fabs(init_electrons[initel]->deltaPhiIn()), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectronHoverE"]->Fill(
                    init_electrons[initel]->hadronicOverEm(), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectrond0"]->Fill(
                    fabs(init_electrons[initel]->d0()), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectrondZ"]->Fill(
                    fabs(init_electrons[initel]->dz()), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectronEminusP"]->Fill(
                    fabs(1 / init_electrons[initel]->E() - 1 / init_electrons[initel]->P()), datasets[d], true,
                    Luminosity * scaleFactor);
                MSPlot["InitElectronConversion"]->Fill(
                    init_electrons[initel]->passConversion(), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectronMissingHits"]->Fill(
                    init_electrons[initel]->missingHits(), datasets[d], true, Luminosity * scaleFactor);
                MSPlot["InitElectronCutFlow"]->Fill(0, datasets[d], true, Luminosity * scaleFactor);
                if(init_electrons[initel]->Pt() > 30) {
                    MSPlot["InitElectronCutFlow"]->Fill(1, datasets[d], true, Luminosity * scaleFactor);
                    if(fabs(init_electrons[initel]->Eta()) < 2.5) {
                        MSPlot["InitElectronCutFlow"]->Fill(2, datasets[d], true, Luminosity * scaleFactor);
                        if(fabs(init_electrons[initel]->deltaEtaIn()) < 0.009277) {
                            MSPlot["InitElectronCutFlow"]->Fill(3, datasets[d], true, Luminosity * scaleFactor);
                            if(fabs(init_electrons[initel]->deltaPhiIn()) < 0.094739) {
                                MSPlot["InitElectronCutFlow"]->Fill(4, datasets[d], true, Luminosity * scaleFactor);
                                if(fabs(init_electrons[initel]->hadronicOverEm()) < 0.093068) {
                                    MSPlot["InitElectronCutFlow"]->Fill(5, datasets[d], true, Luminosity * scaleFactor);
                                    if(fabs(init_electrons[initel]->d0()) < 0.035904) {
                                        MSPlot["InitElectronCutFlow"]->Fill(
                                            6, datasets[d], true, Luminosity * scaleFactor);
                                        if(fabs(init_electrons[initel]->dz()) < 0.075496) {
                                            MSPlot["InitElectronCutFlow"]->Fill(
                                                7, datasets[d], true, Luminosity * scaleFactor);
                                            if(fabs((1 / init_electrons[initel]->E()) -
                                                   (1 / init_electrons[initel]->P())) < 0.189968) {
                                                MSPlot["InitElectronCutFlow"]->Fill(
                                                    8, datasets[d], true, Luminosity * scaleFactor);
                                                MSPlot["InitElectronDiagRelIso"]->Fill(
                                                    init_electrons[initel]->relPfIso(3, 0.5), datasets[d], true,
                                                    Luminosity * scaleFactor);
                                                MSPlot["InitElectronDiagChIso"]->Fill(
                                                    init_electrons[initel]->chargedHadronIso(3), datasets[d], true,
                                                    Luminosity * scaleFactor);
                                                MSPlot["InitElectronDiagNIso"]->Fill(
                                                    init_electrons[initel]->neutralHadronIso(3), datasets[d], true,
                                                    Luminosity * scaleFactor);
                                                MSPlot["InitElectronDiagPhIso"]->Fill(
                                                    init_electrons[initel]->photonIso(3), datasets[d], true,
                                                    Luminosity * scaleFactor);
                                                MSPlot["InitElectronDiagPUChIso"]->Fill(
                                                    init_electrons[initel]->puChargedHadronIso(3), datasets[d], true,
                                                    Luminosity * scaleFactor);
                                                if(ElectronRelIso(init_electrons[initel], rho) < 0.130136) {
                                                    MSPlot["InitElectronCutFlow"]->Fill(
                                                        9, datasets[d], true, Luminosity * scaleFactor);
                                                    if(init_electrons[initel]->passConversion()) {
                                                        MSPlot["InitElectronCutFlow"]->Fill(
                                                            10, datasets[d], true, Luminosity * scaleFactor);
                                                        if(fabs(init_electrons[initel]->missingHits()) <= 1) {
                                                            MSPlot["InitElectronCutFlow"]->Fill(
                                                                11, datasets[d], true, Luminosity * scaleFactor);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            string graphName;

            //////////////////
            // Loading Gen jets
            //////////////////

            float numOfbb = 0;
            float numOfcc = 0;
            float numOfll = 0;
            float ttbar_flav = -1;

            vector<TRootGenJet*> genjets;
            if(dataSetName.find("Data") == string::npos) {
                // loading GenJets as I need them for JER
                genjets = treeLoader.LoadGenJet(ievt, false);
                // cout << "number genjets: " << genjets.size() << endl;
            }
            //            string currentFilename = datasets[d]->eventTree()->GetFile()->GetName();
            //            if(previousFilename != currentFilename)
            //            {
            //                previousFilename = currentFilename;
            //                iFile++;
            //                cout<<"File changed!!! => "<<currentFilename<<endl;
            //            }
            if(dataSetName.find("TTJets") != string::npos || dataSetName.find("TTScale") != string::npos) {
                // genEvt_flav = treeLoader.LoadGenEvent(ievt,false);
                //treeLoader.LoadMCEvent(ievt, 0, mcParticles_flav, false);
                for(int gj = 0; gj < genjets.size(); gj++) {
                    if(genjets[gj]->BHadron().Et() > 0) {
                        if(genjets[gj]->Pt() > 20 && abs(genjets[gj]->Eta() < 2.5))
                            numOfbb++;
                    }
                    if(genjets[gj]->CHadron().Et() > 0) {
                        if(genjets[gj]->Pt() > 20 && abs(genjets[gj]->Eta() < 2.5))
                            numOfcc++;
                    }
                }
            }

            if(numOfbb > 2) {
                ttbar_flav = 2;
            } else if(numOfcc > 0) {
                ttbar_flav = 1;
            } else {
                ttbar_flav = 0;
            }

            ///////////////////////////////////////////
            //  Trigger
            ///////////////////////////////////////////
            bool trigged = false;
            std::string filterName = "";

            if(previousRun != currentRun) {
                cout << "What run? " << currentRun << endl;
                previousRun = currentRun;
                cout << "HLT Debug output" << endl;

                // runInfos->getWeightInfo(currentRun).getweightNameList();

                // runInfos->getHLTinfo(currentRun).gethltNameList();

                //                treeLoader.ListTriggers(currentRun, iFile);

                // int weightIdx = runInfos->getWeightInfo(currentRun).weightIndex("Central scale variation 1");

                if(nlo) {
                    if(centralWeight < 0.0) {
                        // scaleFactor = -1.0;  //Taking into account negative weights in NLO Monte Carlo
                        negWeights++;
                    }
                }

                if(dataSetName.find("Data") != string::npos || dataSetName.find("data") != string::npos ||
                    dataSetName.find("DATA") != string::npos) {
                    if(debug)
                        cout << "event loop 6a" << endl;

                    // cout << " RUN " << event->runId() << endl;

                    if(Muon && Electron) {
                        itrigger.push_back(treeLoader.iTrigger(
                            "HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v*", currentRun, iFile));
                        itrigger.push_back(treeLoader.iTrigger(
                            "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v*", currentRun, iFile));
                    } else if(Muon && !Electron) {
                        itrigger.push_back(
                            treeLoader.iTrigger("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*", currentRun, iFile));
                        // itrigger.push_back(treeLoader.iTrigger ("HLT_IsoMu20_v*", currentRun, iFile));
                    }

                    else if(!Muon && Electron)
                        itrigger.push_back(
                            treeLoader.iTrigger("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*", currentRun, iFile));

                    for(int tr = 0; tr < itrigger.size(); tr++) {
                        if(itrigger[tr] == 9999) {
                            cerr << "NO VALID TRIGGER FOUND FOR THIS EVENT (" << dataSetName << ") IN RUN "
                                 << event->runId() << endl;
                            exit(1);
                        }
                    }
                } else {
                    if(Muon && Electron) {
                        itrigger.push_back(treeLoader.iTrigger(
                            "HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v*", currentRun, iFile));
                        itrigger.push_back(treeLoader.iTrigger(
                            "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v*", currentRun, iFile));
                        //                        cout << "iTrigger : " << itrigger << " iFile: " << iFile << endl;
                        //                        cout << "runInfos Trigger : " <<
                        //                        runInfos->getHLTinfo(currentRun).hltPath("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1")
                        //                        << endl;
                    } else if(Muon && !Electron) {
                        itrigger.push_back(
                            treeLoader.iTrigger("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*", currentRun, iFile));
                        // itrigger.push_back(treeLoader.iTrigger ("HLT_IsoMu20_v*", currentRun, iFile));
                    } else if(!Muon && Electron)
                        itrigger.push_back(
                            treeLoader.iTrigger("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*", currentRun, iFile));
                    for(int tr = 0; tr < itrigger.size(); tr++) {
                        if(itrigger[tr] == 9999) {
                            cerr << "NO VALID TRIGGER FOUND FOR THIS EVENT (" << dataSetName << ") IN RUN "
                                 << event->runId() << endl;
                            exit(1);
                        }
                    }
                }

            } // end previousRun != currentRun

            //////////////////////////////////////
            ///  Jet Energy Scale Corrections  ///
            //////////////////////////////////////

            if(applyJER && !isData) {
                if(JERNom) {
                    // cout << "precor jet1: "<< init_jets[0]->Pt() << endl;
                    jetTools->correctJetJER(init_jets, genjets, mets[0], "nominal", false);
                    // cout << "postcor jet1: "<< init_jets[0]->Pt() << endl;
                } else if(JERDown)
                    jetTools->correctJetJER(init_jets, genjets, mets[0], "minus", false);
                else if(JERUp)
                    jetTools->correctJetJER(init_jets, genjets, mets[0], "plus", false);
                /// Example how to apply JES systematics

                // cout << "JER smeared!!! " << endl;
            }
            // cout<<"after JER smearing" <<endl;

            // for(int jj=0; jj<init_jets.size();jj++){
            //     cout<<jj<<" jet pt : "<<init_jets[jj]->Pt()<<endl;
            // }

            if(JESDown)
                jetTools->correctJetJESUnc(init_jets, "minus", 1);
            else if(JESUp)
                jetTools->correctJetJESUnc(init_jets, "plus", 1);

            if(applyJEC) /// should this have  && dataSetName.find("Data")==string::npos
            {
                // cout<<"apply JEC"<<endl;
                jetTools->correctJets(init_jets, event->fixedGridRhoFastjetAll(), isData);
            }

            /////////////////////////////////////
            // Init Jet Plots for ISR reweight //
            /////////////////////////////////////

            int nInitJets = 0;
            float ISRsf = 1.0;
            // cout << "Number of initJets : " << init_jets.size() << endl;

            for(int iJet = 0; iJet < init_jets.size(); iJet++) {
                if(init_jets[iJet]->genParticleIndex() == -1) {
                    MSPlot["ISRJetPt"]->Fill(init_jets[iJet]->Pt(), datasets[d], true, Luminosity * scaleFactor);
                    nInitJets++;
                }
            }
            MSPlot["nISRJets"]->Fill(nInitJets, datasets[d], true, Luminosity * scaleFactor);

            //	    if(dataSetName.find("TTJets")!=string::npos || dataSetName.find("TTDilept")!=string::npos ||
            // dataSetName.find("TT_")!=string::npos)
            //            {
            //	        ISRsf = ISRSFHist->GetBinContent(nInitJets);
            //	        scaleFactor *= ISRsf;
            //	    }

            ///////////////////////////////////////////////////////////
            // Event selection
            ///////////////////////////////////////////////////////////

            // Apply trigger selection
            //            trigged = treeLoader.EventTrigged (221);  //artifical HLT for Mirena
            for(int tr = 0; tr < itrigger.size(); tr++) {
                bool temp = (treeLoader.EventTrigged(itrigger[tr]) || trigged);
                trigged = temp;
            }
            //            trigged = treeLoader.EventTrigged (itrigger);
            // trigged = true;  // Disabling the HLT requirement
            if(debug)
                cout << "triggered? Y/N?  " << trigged << endl;
            //            if(itrigger == 9999 ) cout << "Lumi Block: " << event->lumiBlockId() << " Event: " <<
            //            event->eventId() << endl;
            //            if(!trigged)		   continue;  //If an HLT condition is not present, skip this event in
            //            the loop. this is present to cut down on compute time.
            // Declare selection instance
            Run2Selection selection(init_jets, init_fatjets, init_muons, init_electrons, mets, rho);

            // Getting Event Weight

            // Define object selection cuts
            if(Muon && Electron && dilepton) {

                if(debug)
                    cout << "Getting Loose Muons" << endl;
                selectedMuons = selection.GetSelectedMuons(20, 2.4, 0.25, "Loose", "Spring15");
                selectedExtraMuons = selection.GetSelectedMuons(0, 2.4, 1, "Loose", "Spring15");
                if(debug)
                    cout << "Getting Loose Electrons" << endl;
                if(bx25)
                    selectedElectrons =
                        selection.GetSelectedElectrons(25, 2.5, "Loose", "Spring15_25ns", true); // MVA ID WP80
                else
                    selectedElectrons =
                        selection.GetSelectedElectrons(20, 2.5, "Loose", "Spring15_50ns", true); // VBTF ID
            }
            if(Muon && !Electron && dilepton) {
                if(debug)
                    cout << "Getting Loose Muons" << endl;
                selectedMuons = selection.GetSelectedMuons(20, 2.4, 0.25, "Loose", "Spring15");
                if(debug)
                    cout << "Getting Loose Electrons" << endl;
                if(bx25)
                    selectedElectrons =
                        selection.GetSelectedElectrons(25, 2.5, "Loose", "Spring15_25ns", true); // MVA ID WP80
                else
                    selectedElectrons =
                        selection.GetSelectedElectrons(20, 2.5, "Loose", "Spring15_50ns", true); // VBTF ID
            }
            if(!Muon && Electron && dilepton) {
                if(debug)
                    cout << "Getting Medium Muons" << endl;
                selectedMuons = selection.GetSelectedMuons(20, 2.4, 0.25, "Loose", "Spring15");
                if(debug)
                    cout << "Getting Loose Electrons" << endl;
                if(bx25)
                    selectedElectrons =
                        selection.GetSelectedElectrons(25, 2.5, "Loose", "Spring15_25ns", true); // MVA ID WP80
                else
                    selectedElectrons =
                        selection.GetSelectedElectrons(20, 2.5, "Loose", "Spring15_50ns", true); // VBTF ID
            }

            if(debug)
                cout << "Getting Jets" << endl;
            selectedUncleanedJets =
                selection.GetSelectedJets(30, 2.5, true, "Loose"); // Relying solely on cuts defined in setPFJetCuts()
            selectedLooseJets = selection.GetSelectedJets();       // Relying solely on cuts defined in setPFJetCuts()
            selectedFatJets = selection.GetSelectedFatJets();      // Relying solely on cuts defined in setPFJetCuts()
            for(int unj = 0; unj < selectedUncleanedJets.size(); unj++) {
                bool isClose = false;
                for(int e = 0; e < selectedElectrons.size(); e++) {
                    if(selectedElectrons[e]->DeltaR(*selectedUncleanedJets[unj]) < 0.3)
                        isClose = true;
                }
                for(int mu = 0; mu < selectedMuons.size(); mu++) {
                    if(selectedMuons[mu]->DeltaR(*selectedUncleanedJets[unj]) < 0.3)
                        isClose = true;
                }
                if(!isClose)
                    selectedJets.push_back(selectedUncleanedJets[unj]);
            }

            vector<TRootJet*> selectedLBJets;
            vector<TRootJet*> selectedMBJets;
            vector<TRootJet*> selectedTBJets;
            vector<TRootPFJet*> selectedLightJets;
            vector<TRootPFJet*> selectedHeavyJets;

            for(int selj = 0; selj < selectedJets.size(); selj++) {
                if(abs(selectedJets[selj]->hadronFlavour()) >= 4)
                    selectedHeavyJets.push_back(selectedJets[selj]);
                else
                    selectedLightJets.push_back(selectedJets[selj]);
            }

            int JetCut = 0;
            int nMu = 0, nEl = 0, nLooseIsoMu = 0, nLep = 0, nLooseEl = 0, nMedEl = 0, nTightEl = 0;

            nMu = selectedMuons.size();     // Number of Muons in Event
            nEl = selectedElectrons.size(); // Number of Loose Electrons in Event

            bool isTagged = false;
            vector<TLorentzVector> selectedMuonsTLV_JC, selectedElectronsTLV_JC, selectedLooseIsoMuonsTLV;
            vector<TLorentzVector> mcParticlesTLV, selectedJetsTLV, mcMuonsTLV, mcPartonsTLV;
            vector<TRootMCParticle*> mcParticlesMatching_;
            vector<int> mcMuonIndex, mcPartonIndex;
            JetPartonMatching muonMatching, jetMatching;

            //////////////////////////////////
            // Preselection Lepton Operations //
            //////////////////////////////////

            float diLepMass = 0, diMuMass = 0;
            bool ZVeto = false, sameCharge = false;
            float ZMass = 91, ZMassWindow = 15;
            int cj1 = 0, cj2 = 0, lidx1 = 0, lidx2 = 0;
            TLorentzVector lep1, lep2, diLep;
            vector<pair<float, pair<TRootLepton*, TRootLepton*> > > LeptonPairs;

            for(int selmu = 0; selmu < selectedMuons.size(); selmu++) {
                selectedMuonsTLV_JC.push_back(*selectedMuons[selmu]);
            }
            sort(selectedPosMuons.begin(), selectedPosMuons.end(), HighestPt());
            sort(selectedNegMuons.begin(), selectedNegMuons.end(), HighestPt());
            for(int selel = 0; selel < selectedElectrons.size(); selel++) {
                selectedElectronsTLV_JC.push_back(*selectedElectrons[selel]);
            }
            for(Int_t seljet = 0; seljet < selectedJets.size(); seljet++) {
                selectedJetsTLV.push_back(*selectedJets[seljet]);
            }
            if(nMu == 2 && nEl == 0 && Muon && !Electron) // Di-Muon Selection
            {

                lep1 = selectedMuonsTLV_JC[0];
                lep2 = selectedMuonsTLV_JC[1];
                if(selectedMuons[0]->charge() == selectedMuons[1]->charge())
                    sameCharge = true;
                nLep = nMu;
            } else if(nEl == 2 && nMu == 0 && Electron && !Muon) // Di-Electron Selection criteria
            {
                lep1 = selectedElectronsTLV_JC[0];
                lep2 = selectedElectronsTLV_JC[1];
                if(selectedElectrons[0]->charge() == selectedElectrons[1]->charge())
                    sameCharge = true;
                nLep = nEl;
            } else if(nEl == 1 && nMu == 1 && Electron && Muon) // Muon-Electron Selection
            {
                lep1 = selectedMuonsTLV_JC[0];
                lep2 = selectedElectronsTLV_JC[0];
                if(selectedMuons[0]->charge() == selectedElectrons[0]->charge())
                    sameCharge = true;
                nLep = nMu + nEl;
            }

            /////////////////////////////////////////////////
            //               Pu reweighting                //
            /////////////////////////////////////////////////

            float lumiWeight, lumiWeight_up, lumiWeight_down;
            if(dataSetName.find("Data") != string::npos || dataSetName.find("data") != string::npos ||
                dataSetName.find("DATA") != string::npos) {
                lumiWeight = 1;
                lumiWeight_up = 1;
                lumiWeight_down = 1;
            } else {
                // lumiWeight = LumiWeights.ITweight( vertex.size() );
                lumiWeight = LumiWeights.ITweight((int)event->nTruePU());
                lumiWeight_up = LumiWeights_up.ITweight((int)event->nTruePU());
                lumiWeight_down = LumiWeights_down.ITweight((int)event->nTruePU());
            }
            // if(lumiWeight<0.2)     cout<<"PU:  "<<(int)event->nTruePU()<<"    LUMI WEIGHT   :   "<<lumiWeight<<" !
            // "<<endl;

            /////////////////////////////////////////////////
            //                    bTag SF                  //
            /////////////////////////////////////////////////

            float bTagEff(-1);
            float bTagEffUp(-1);
            float bTagEffDown(-1);
            if(fillingbTagHistos) {
                if(bTagReweight && dataSetName.find("Data") == string::npos) {
                    // get btag weight info
                    for(int jetbtag = 0; jetbtag < selectedJets.size(); jetbtag++) {
                        float jetpt = selectedJets[jetbtag]->Pt();
                        float jeteta = selectedJets[jetbtag]->Eta();
                        float jetdisc = selectedJets[jetbtag]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
                        BTagEntry::JetFlavor jflav;
                        int jetpartonflav = std::abs(selectedJets[jetbtag]->hadronFlavour());
                        if(debug)
                            cout << "parton flavour: " << jetpartonflav << "  jet eta: " << jeteta
                                 << " jet pt: " << jetpt << "  jet disc: " << jetdisc << endl;
                        if(jetpartonflav == 5) {
                            jflav = BTagEntry::FLAV_B;
                        } else if(jetpartonflav == 4) {
                            jflav = BTagEntry::FLAV_C;
                        } else {
                            jflav = BTagEntry::FLAV_UDSG;
                        }
                        bTagEff = bTagReader->eval(jflav, jeteta, jetpt, jetdisc);
                        bTagEffUp = bTagReaderUp->eval(jflav, jeteta, jetpt, jetdisc);
                        bTagEffDown = bTagReaderDown->eval(jflav, jeteta, jetpt, jetdisc);

                        if(debug)
                            cout << "btag efficiency = " << bTagEff << endl;
                    }
                    btwt->FillMCEfficiencyHistos(selectedJets);
                    btwtUp->FillMCEfficiencyHistos(selectedJets);
                    btwtDown->FillMCEfficiencyHistos(selectedJets);
                }
            }

            if (debug) cout<<"getMCEventWeight for btag"<<endl;
            float btagWeight = 1;
            float btagWeightCSV = 1;
            float btagWeightCSVLFUp = 1;
            float btagWeightCSVLFDown = 1;
            float btagWeightCSVHFUp = 1;
            float btagWeightCSVHFDown = 1;
            float btagWeightCSVHFStats1Up = 1;
            float btagWeightCSVHFStats1Down = 1;
            float btagWeightCSVHFStats2Up = 1;
            float btagWeightCSVHFStats2Down = 1;            
            float btagWeightCSVLFStats1Up = 1;
            float btagWeightCSVLFStats1Down = 1;
            float btagWeightCSVLFStats2Up = 1;
            float btagWeightCSVLFStats2Down = 1; 
            float btagWeightCSVCFErr1Up = 1;
            float btagWeightCSVCFErr1Down = 1;
            float btagWeightCSVCFErr2Up = 1;
            float btagWeightCSVCFErr2Down = 1; 

            float btagWeight_light=1;
            float btagWeight_lightUp = 1;
            float btagWeight_lightDown = 1;
            float btagWeight_heavy=1;
            float btagWeight_heavyUp = 1;
            float btagWeight_heavyDown = 1;            
            if(bTagReweight && dataSetName.find("Data")==string::npos){
                if(!fillingbTagHistos){
                    btagWeight_light =  btwt->getMCEventWeight(selectedLightJets, false);
                    btagWeight_lightUp =  btwtUp->getMCEventWeight(selectedLightJets, false);
                    btagWeight_lightDown =  btwtDown->getMCEventWeight(selectedLightJets, false);
                    btagWeight_heavy =  btwt->getMCEventWeight(selectedHeavyJets, false);
                    btagWeight_heavyUp =  btwtUp->getMCEventWeight(selectedHeavyJets, false);
                    btagWeight_heavyDown =  btwtDown->getMCEventWeight(selectedHeavyJets, false);                    
                }
                
                if(debug) cout<<"btag weight "<<btagWeight_light<<"  btag weight Up "<<btagWeight_lightUp<<"   btag weight Down "<<btagWeight_lightDown<<endl;
            }
            // if(ievt<1000)
            // {
            //     cout<<"btagweight: "<<btagWeight<<endl;
            // }

            ////csv discriminator reweighting
            // float TotalCSVbtagweight=1;
            if(bTagCSVReweight && !isData){
            //get btag weight info
                for(int jetbtag = 0; jetbtag<selectedJets.size(); jetbtag++){
                    float bTagEff_LFUp, bTagEff_LFDown, bTagEff_HFUp, bTagEff_HFDown, bTagEff_HFStats1Up, bTagEff_HFStats1Down, bTagEff_HFStats2Up,
                    bTagEff_HFStats2Down, bTagEff_LFStats1Up, bTagEff_LFStats1Down, bTagEff_LFStats2Up, bTagEff_LFStats2Down, bTagEff_CFErr1Up, 
                    bTagEff_CFErr1Down, bTagEff_CFErr2Up, bTagEff_CFErr2Down;

                    float jetpt = selectedJets[jetbtag]->Pt();
                    float jeteta = selectedJets[jetbtag]->Eta();
                    float jetdisc = selectedJets[jetbtag]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
                    bool isBFlav = false;
                    bool isLFlav = false;
                    bool isCFlav = false;
                    if(jetdisc<0.0) jetdisc = -0.05;
                    if(jetdisc>1.0) jetdisc = 1.0;
                    BTagEntry::JetFlavor jflav;
                    int jethadronflav = std::abs(selectedJets[jetbtag]->hadronFlavour());
                    if(debug) cout<<"hadron flavour: "<<jethadronflav<<"  jet eta: "<<jeteta<<" jet pt: "<<jetpt<<"  jet disc: "<<jetdisc<<endl;
                    if(jethadronflav == 5){
                        jflav = BTagEntry::FLAV_B;
                        isBFlav =true;
                    }
                    else if(jethadronflav == 4){
                        jflav = BTagEntry::FLAV_C;
                        isCFlav=true;
                    }
                    else{
                        jflav = BTagEntry::FLAV_UDSG;
                        isLFlav = true;
                    }
                    if( dataSetName.find("JESUp") !=string::npos)        bTagEff = reader_JESUp->eval(jflav, jeteta, jetpt, jetdisc);
                    else if( dataSetName.find("JESDown") !=string::npos) bTagEff = reader_JESDown->eval(jflav, jeteta, jetpt, jetdisc);
                    else bTagEff = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);

                    if( isBFlav ) bTagEff_LFUp = reader_LFUp->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isBFlav ) bTagEff_LFDown = reader_LFDown->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isLFlav ) bTagEff_HFUp = reader_HFUp->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isLFlav ) bTagEff_HFDown = reader_HFDown->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isBFlav ) bTagEff_HFStats1Up = reader_HFStats1Up->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isBFlav ) bTagEff_HFStats1Down = reader_HFStats1Down->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isBFlav ) bTagEff_HFStats2Up = reader_HFStats2Up->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isBFlav ) bTagEff_HFStats2Down = reader_HFStats2Down->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isLFlav ) bTagEff_LFStats1Up = reader_LFStats1Up->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isLFlav ) bTagEff_LFStats1Down = reader_LFStats1Down->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isLFlav ) bTagEff_LFStats2Up = reader_LFStats2Up->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isLFlav ) bTagEff_LFStats2Down = reader_LFStats2Down->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isCFlav ) bTagEff_CFErr1Up = reader_CFErr1Up->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isCFlav ) bTagEff_CFErr1Down = reader_CFErr1Down->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isCFlav ) bTagEff_CFErr2Up = reader_CFErr2Up->eval(jflav, jeteta, jetpt, jetdisc);
                    if( isCFlav ) bTagEff_CFErr2Down = reader_CFErr2Down->eval(jflav, jeteta, jetpt, jetdisc);
                    
                    //If jet is not the appropriate flavor for that systematic, use the nominal reader so that all weights will be on the same
                    //jet multiplicity footing.
                    if( !isBFlav ) bTagEff_LFUp = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isBFlav ) bTagEff_LFDown = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isLFlav ) bTagEff_HFUp = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isLFlav ) bTagEff_HFDown = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isBFlav ) bTagEff_HFStats1Up = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isBFlav ) bTagEff_HFStats1Down = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isBFlav ) bTagEff_HFStats2Up = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isBFlav ) bTagEff_HFStats2Down = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isLFlav ) bTagEff_LFStats1Up = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isLFlav ) bTagEff_LFStats1Down = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isLFlav ) bTagEff_LFStats2Up = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isLFlav ) bTagEff_LFStats2Down = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isCFlav ) bTagEff_CFErr1Up = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isCFlav ) bTagEff_CFErr1Down = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isCFlav ) bTagEff_CFErr2Up = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    if( !isCFlav ) bTagEff_CFErr2Down = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);
                    // bTagEff = reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);   
                    // cout<<bTagEff<<endl;
                    btagWeightCSVLFUp *= bTagEff_LFUp;
                    btagWeightCSVLFDown *= bTagEff_LFDown;
                    btagWeightCSVHFUp *= bTagEff_HFUp;
                    btagWeightCSVHFDown *= bTagEff_HFDown;
                    btagWeightCSVHFStats1Up *= bTagEff_HFStats1Up;
                    btagWeightCSVHFStats1Down *= bTagEff_HFStats1Down;
                    btagWeightCSVHFStats2Up *= bTagEff_HFStats2Up;
                    btagWeightCSVHFStats2Down *= bTagEff_HFStats2Down;            
                    btagWeightCSVLFStats1Up *= bTagEff_LFStats1Up;
                    btagWeightCSVLFStats1Down *= bTagEff_LFStats1Down;
                    btagWeightCSVLFStats2Up *= bTagEff_LFStats2Up;
                    btagWeightCSVLFStats2Down *= bTagEff_LFStats2Down; 
                    btagWeightCSVCFErr1Up *= bTagEff_CFErr1Up;
                    btagWeightCSVCFErr1Down *= bTagEff_CFErr1Down;
                    btagWeightCSVCFErr2Up *= bTagEff_CFErr2Up;
                    btagWeightCSVCFErr2Down *= bTagEff_CFErr2Down; 

                    btagWeightCSV*=bTagEff;
                    if (debug){
                        cout<<"hadron flavour: "<<jethadronflav<<"  jet eta: "<<jeteta<<" jet pt: "<<jetpt<<"  jet disc: "<<jetdisc<<endl;
                        cout << " isBFlav " << isBFlav;
                        cout << " isLFlav" << isLFlav;
                        cout << " isCFlav" << isCFlav << endl;
                        cout << " reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc);  " << reader_csvv2->eval(jflav, jeteta, jetpt, jetdisc) << endl;
                        cout << " bTagEff " << bTagEff;
                        cout << " bTagEff_LFUp " << bTagEff_LFUp;
                        cout << " bTagEff_LFDown " << bTagEff_LFDown << endl;
                        cout << " bTagEff_HFUp " << bTagEff_HFUp;
                        cout << " bTagEff_HFDown " << bTagEff_HFDown << endl;
                        cout << " bTagEff_HFStats1Up " << bTagEff_HFStats1Up;
                        cout << " bTagEff_HFStats1Down " << bTagEff_HFStats1Down << endl;
                        cout << " bTagEff_HFStats2Up " << bTagEff_HFStats2Up;
                        cout << " bTagEff_HFStats2Down " << bTagEff_HFStats2Down << endl;
                        cout << " bTagEff_LFStats1Up " << bTagEff_LFStats1Up;
                        cout << " bTagEff_LFStats1Down " << bTagEff_LFStats1Down << endl;
                        cout << " bTagEff_LFStats2Up " << bTagEff_LFStats2Up;
                        cout << " bTagEff_LFStats2Down " << bTagEff_LFStats2Down << endl;
                        cout << " bTagEff_CFErr1Up " << bTagEff_CFErr1Up;
                        cout << " bTagEff_CFErr1Down " << bTagEff_CFErr1Down << endl;
                        cout << " bTagEff_CFErr2Up " << bTagEff_CFErr2Up;
                        cout << " bTagEff_CFErr2Down " << bTagEff_CFErr2Down << endl;
                    }
                    if(debug)cout<<"btag efficiency = "<<bTagEff<<endl;       
                }      


            }

            ///////////////////////////
            //  Lepton Scale Factors //
            ///////////////////////////

            float fleptonSF1 = 1, fleptonSF2 = 1, fleptonSF = 1;
            if(bLeptonSF) {
                if(Muon && !Electron && nMu == 2 && nEl == 0) {
                    fleptonSF1 = muonSFWeightID->at(selectedMuons[0]->Eta(), selectedMuons[0]->Pt(), 0);
                    fleptonSF2 = muonSFWeightID->at(selectedMuons[1]->Eta(), selectedMuons[1]->Pt(), 0);
                    fleptonSF1 *= muonSFWeightIso->at(selectedMuons[0]->Eta(), selectedMuons[0]->Pt(), 0);
                    fleptonSF2 *= muonSFWeightIso->at(selectedMuons[1]->Eta(), selectedMuons[1]->Pt(), 0);
                } else if(nEl == 2 && nMu == 0 && Electron && !Muon) // Di-Electron Selection criteria
                {
                    fleptonSF1 = electronSFWeight->at(selectedElectrons[0]->Eta(), selectedElectrons[0]->Pt(), 0) *
                        electronSFWeightReco->at(selectedElectrons[0]->Eta(), selectedElectrons[0]->Pt(),
                            0); // Always set the first lepton to the highest Pt Tight Electron
                    fleptonSF2 = electronSFWeight->at(selectedElectrons[1]->Eta(), selectedElectrons[1]->Pt(), 0) *
                        electronSFWeightReco->at(selectedElectrons[1]->Eta(), selectedElectrons[1]->Pt(),
                            0); // Always set the first lepton to the highest Pt Tight Electron
                } else if(Electron && Muon && nEl == 1 && nMu == 1) {
                    // scaleFactor *= 0.91; // Trigger scale factor for EMu channel
                    fleptonSF1 = electronSFWeight->at(selectedElectrons[0]->Eta(), selectedElectrons[0]->Pt(), 0) *
                        electronSFWeightReco->at(selectedElectrons[0]->Eta(), selectedElectrons[0]->Pt(), 0);
                    fleptonSF2 = muonSFWeightID->at(selectedMuons[0]->Eta(), selectedMuons[0]->Pt(), 0);
                    fleptonSF2 *= muonSFWeightIso->at(selectedMuons[0]->Eta(), selectedMuons[0]->Pt(), 0);
                }
            }
            if(debug)
                cout << "lepton1 SF:  " << fleptonSF1 << endl;

            fleptonSF = fleptonSF1 * fleptonSF2;

            ////////////////////////////
            //  Trigger Scale Factors //
            ////////////////////////////

            float fTriggerSF = 1.0;
            if(Electron && Muon)
                fleptonSF *= 0.971;
            else if(!Electron && Muon)
                fleptonSF *= 0.931;
            else if(Electron && !Muon)
                fleptonSF *= 0.958;

            ///////////////////////
            // Getting Gen Event //
            ///////////////////////

            if(!(dataSetName.find("Data") != string::npos || dataSetName.find("data") != string::npos ||
                   dataSetName.find("DATA") != string::npos)) {
                vector<TRootMCParticle*> mcParticles;
                vector<TRootMCParticle*> mcTops;
                mcParticlesMatching_.clear();
                mcParticlesTLV.clear();
                // selectedJetsTLV.clear();
                mcParticles.clear();
                mcTops.clear();

                int leptonPDG, muonPDG = 13, electronPDG = 11;
                leptonPDG = muonPDG;

                treeLoader.LoadMCEvent(ievt, 0, mcParticlesMatching_, false);
                if(debug)
                    cout << "size   " << mcParticlesMatching_.size() << endl;
            }

            /////////////////////////////////////////////////
            //           heavy flav  reweighting           //
            /////////////////////////////////////////////////
//            float numOfbb = 0;
//            float numOfcc = 0;
//            float numOfll = 0;
//            float ttbar_flav = -1;
            vector<TRootMCParticle*> mcParticles_flav;
            // TRootGenEvent* genEvt_flav = 0;
//            if(dataSetName.find("TTJets") != string::npos || dataSetName.find("TTScale") != string::npos) {
//                // genEvt_flav = treeLoader.LoadGenEvent(ievt,false);
//                treeLoader.LoadMCEvent(ievt, 0, mcParticles_flav, false);
//                for(int gj = 0; gj < genjets.size(); gj++) {
//                    if(genjets[gj]->BHadron().Et() > 0) {
//                        if(genjets[gj]->Pt() > 20 && abs(genjets[gj]->Eta() < 2.5))
//                            numOfbb++;
//                   }
//                  if(genjets[gj]->CHadron().Et() > 0) {
//                        if(genjets[gj]->Pt() > 20 && abs(genjets[gj]->Eta() < 2.5))
//                            numOfcc++;
//                   }
//                }
//            }

//            if(numOfbb > 2) {
//                ttbar_flav = 2;
//            } else if(numOfcc > 0) {
//                ttbar_flav = 1;
//            } else {
//                ttbar_flav = 0;
//            }
            // cout << "ttbar_flav: " << ttbar_flav << endl;
            //            cin.get();

            ///////////////////////
            //  Top PT Reweight  //
            ///////////////////////

            float fTopPtsf = 1, fAntitopPtsf = 1, fTopPtReWeightsf = 1;
            int nTops = 0;

            if(dataSetName.find("TTJets") != string::npos || dataSetName.find("TTDilept") != string::npos ||
                dataSetName.find("TT_") != string::npos) {
                for(int part = 0; part < mcParticlesMatching_.size(); part++) {
                    //		    if(abs(mcParticlesMatching_[part]->type()) == 6 &&
                    //mcParticlesMatching_[part]->status()
                    //<
                    // 30) cout << "Type: " << mcParticlesMatching_[part]->type() << " Status: " <<
                    // mcParticlesMatching_[part]->status() << endl;
                    //		    if(abs(mcParticlesMatching_[part]->type()) == 5) cout << "Type: " <<
                    // mcParticlesMatching_[part]->type() << " Status: " << mcParticlesMatching_[part]->status() <<
                    // endl;
                    //		    if(abs(mcParticlesMatching_[part]->type()) <= 6 ||
                    // abs(mcParticlesMatching_[part]->type())
                    //== 21) cout << "Type: " << mcParticlesMatching_[part]->type() << " Status: " <<
                    // mcParticlesMatching_[part]->status() << endl;
                    //		    if(abs(mcParticlesMatching_[part]->status()) <= 59 &&
                    // abs(mcParticlesMatching_[part]->status()) >=30) cout << "Type: " <<
                    // mcParticlesMatching_[part]->type() << " Status: " << mcParticlesMatching_[part]->status() <<
                    // endl;
                    if(mcParticlesMatching_[part]->type() == 6 && mcParticlesMatching_[part]->status() == 22) {
                        if(mcParticlesMatching_[part]->Pt() < 400)
                            fTopPtsf = exp(0.148 - (0.00129 * mcParticlesMatching_[part]->Pt()));
                        else
                            fTopPtsf = exp(0.148 - (0.00129 * 400));
                        nTops++;
                    } else if(mcParticlesMatching_[part]->type() == -6 && mcParticlesMatching_[part]->status() == 22) {
                        if(mcParticlesMatching_[part]->Pt() < 400)
                            fAntitopPtsf = exp(0.148 - (0.00129 * mcParticlesMatching_[part]->Pt()));
                        else
                            fAntitopPtsf = exp(0.148 - (0.00129 * 400));
                        nTops++;
                    }
                }
                //		cin.get();
                fTopPtReWeightsf = sqrt(fTopPtsf * fAntitopPtsf);

                //		if(nTops > 2)
                //		{
                //		    cout << fTopPtsf << " " << fAntitopPtsf << " " << fTopPtReWeightsf <<" nTops: " <<
                // nTops << endl;
                //		    for(int part = 0; part < mcParticlesMatching_.size(); part++)
                //		    {
                //			cout << "Type: " << mcParticlesMatching_[part]->type() << " Status: " <<
                // mcParticlesMatching_[part]->status() << endl;
                //		    }
                //		    cin.get();
                //		}
            }

            //	    if((dataSetName.find("TTJets")!=string::npos || dataSetName.find("TTDilept")!=string::npos) && nTops
            //!= 2) continue;            //If there are not 2 top partons, skip this event.

            sfTup->Fill(fleptonSF1, fleptonSF2, btagWeight_heavy, lumiWeight, fTopPtReWeightsf, ISRsf, scaleFactor,
                normfactor, Luminosity);

            if(!sameCharge) {
                diLep = lep1 + lep2;
                diLepMass = diLep.M();
                MSPlot["PreselDiLepMass"]->Fill(diLepMass, datasets[d], true, Luminosity * scaleFactor);
                for(int mass = 0; mass < 20; mass++) {
                    float windowRes = 2.5;
                    if(((diLepMass < (ZMass - (mass * windowRes))) || (diLepMass > (ZMass + (mass * windowRes)))) &&
                        diLepMass > 20)
                        MSPlot["ZMassWindowWidthAcc"]->Fill(
                            (2 * mass * windowRes), datasets[d], true, Luminosity * scaleFactor);
                    else
                        MSPlot["ZMassWindowWidthRej"]->Fill(
                            (2 * mass * windowRes), datasets[d], true, Luminosity * scaleFactor);
                }
                for(Int_t seljet = 0; seljet < selectedJetsTLV.size(); seljet++) {
                    if(lep1.DeltaR(selectedJetsTLV[seljet]) < lep1.DeltaR(selectedJetsTLV[cj1]))
                        cj1 = seljet;
                    if(lep2.DeltaR(selectedJetsTLV[seljet]) < lep2.DeltaR(selectedJetsTLV[cj2]))
                        cj2 = seljet;
                }
                if(selectedJetsTLV.size() > 0) {
                    if(lep1.DeltaPhi(selectedJetsTLV[cj1]) < 0.05) {
                        MSPlot["CloseLepPt"]->Fill(lep1.Pt(), datasets[d], true, Luminosity * scaleFactor);
                        MSPlot["CloseLepEta"]->Fill(lep1.Eta(), datasets[d], true, Luminosity * scaleFactor);
                        float HOverE = (selectedJets[cj1]->chargedHadronEnergyFraction() +
                                           selectedJets[cj1]->neutralHadronEnergyFraction()) /
                            (selectedJets[cj1]->chargedEmEnergyFraction() +
                                           selectedJets[cj1]->neutralEmEnergyFraction());
                        MSPlot["CloseJetHOverE"]->Fill(HOverE, datasets[d], true, Luminosity * scaleFactor);
                    }
                    if(lep2.DeltaPhi(selectedJetsTLV[cj2]) < 0.05) {
                        MSPlot["CloseLepPt"]->Fill(lep2.Pt(), datasets[d], true, Luminosity * scaleFactor);
                        MSPlot["CloseLepEta"]->Fill(lep2.Eta(), datasets[d], true, Luminosity * scaleFactor);
                        float HOverE = (selectedJets[cj2]->chargedHadronEnergyFraction() +
                                           selectedJets[cj2]->neutralHadronEnergyFraction()) /
                            (selectedJets[cj2]->chargedEmEnergyFraction() +
                                           selectedJets[cj2]->neutralEmEnergyFraction());
                        MSPlot["CloseJetHOverE"]->Fill(HOverE, datasets[d], true, Luminosity * scaleFactor);
                    }
                    if(nTightEl >= 1 && nEl >= 2 && Electron && !Muon) {
                        histo2D["CloseJetdRvsIso"]->Fill(
                            lep1.DeltaR(selectedJetsTLV[cj1]), ElectronRelIso(selectedElectrons[0], rho));
                        histo2D["CloseJetdRvsIso"]->Fill(
                            lep2.DeltaR(selectedJetsTLV[cj2]), ElectronRelIso(selectedElectrons[1], rho));
                        if(lep1.DeltaPhi(selectedJetsTLV[cj1]) < 0.05) {
                            MSPlot["CloseElRelIso"]->Fill(
                                ElectronRelIso(selectedElectrons[0], rho), datasets[d], true, Luminosity * scaleFactor);
                            MSPlot["CloseElChIso"]->Fill(
                                selectedElectrons[0]->chargedHadronIso(3), datasets[d], true, Luminosity * scaleFactor);
                            MSPlot["CloseElNIso"]->Fill(
                                selectedElectrons[0]->neutralHadronIso(3), datasets[d], true, Luminosity * scaleFactor);
                            MSPlot["CloseElPhIso"]->Fill(
                                selectedElectrons[0]->photonIso(3), datasets[d], true, Luminosity * scaleFactor);
                            MSPlot["CloseElPUChIso"]->Fill(selectedElectrons[0]->puChargedHadronIso(3), datasets[d],
                                true, Luminosity * scaleFactor);
                        }
                        if(lep1.DeltaPhi(selectedJetsTLV[cj2]) < 0.05) {
                            MSPlot["CloseElRelIso"]->Fill(
                                ElectronRelIso(selectedElectrons[1], rho), datasets[d], true, Luminosity * scaleFactor);
                            MSPlot["CloseElChIso"]->Fill(
                                selectedElectrons[1]->chargedHadronIso(3), datasets[d], true, Luminosity * scaleFactor);
                            MSPlot["CloseElNIso"]->Fill(
                                selectedElectrons[1]->neutralHadronIso(3), datasets[d], true, Luminosity * scaleFactor);
                            MSPlot["CloseElPhIso"]->Fill(
                                selectedElectrons[1]->photonIso(3), datasets[d], true, Luminosity * scaleFactor);
                            MSPlot["CloseElPUChIso"]->Fill(selectedElectrons[1]->puChargedHadronIso(3), datasets[d],
                                true, Luminosity * scaleFactor);
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////////
            // Preselection looping over Jet Collection                                      //
            // Summing HT and calculating leading, lagging, and ratio for Selected and BJets //
            ///////////////////////////////////////////////////////////////////////////////////
            float temp_HT = 0., HTb = 0.;

            double p_tags_tagged_mc = 1.;
            double p_tags_untagged_mc = 1.;
            double p_tags_tagged_data = 1.;
            double p_tags_untagged_data = 1.;
            double p_mc = 1., p_data = 1.;
            int jet_flavor;
            float eff = 1;
            float scaled_eff = 1;
            float a_eff = 1;
            float sf_a_eff = 1;
            double LightJeteff;
            double JetPt, JetEta;
            double SF_tag = 1.;
            double event_weight = 1.;

            // Uncleaned jet preselection plots
            for(Int_t selujet = 0; selujet < selectedUncleanedJets.size(); selujet++) {
                if(nTightEl >= 1 && nEl >= 2 && nMu == 0 && Electron && !Muon) {
                    MSPlot["PreselDirtyJetLepdR"]->Fill(lep1.DeltaR((TLorentzVector)*selectedUncleanedJets[selujet]),
                        datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["PreselDirtyJetLepdPhi"]->Fill(
                        lep1.DeltaPhi((TLorentzVector)*selectedUncleanedJets[selujet]), datasets[d], true,
                        Luminosity * scaleFactor);
                    MSPlot["PreselDirtyJetLepdR"]->Fill(lep2.DeltaR((TLorentzVector)*selectedUncleanedJets[selujet]),
                        datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["PreselDirtyJetLepdPhi"]->Fill(
                        lep2.DeltaPhi((TLorentzVector)*selectedUncleanedJets[selujet]), datasets[d], true,
                        Luminosity * scaleFactor);
                    histo2D["CloseDirtyJetdRvsdPhi"]->Fill(lep1.DeltaR((TLorentzVector)*selectedUncleanedJets[selujet]),
                        lep1.DeltaPhi((TLorentzVector)*selectedUncleanedJets[selujet]));
                    histo2D["CloseDirtyJetdRvsdPhi"]->Fill(lep2.DeltaR((TLorentzVector)*selectedUncleanedJets[selujet]),
                        lep2.DeltaPhi((TLorentzVector)*selectedUncleanedJets[selujet]));
                }
            }

            for(Int_t seljet = 0; seljet < selectedJets.size(); seljet++) {
                selectedJetsTLV.push_back(*selectedJets[seljet]);
                if(nTightEl >= 1 && nEl >= 2 && nMu == 0 && Electron && !Muon) {
                    MSPlot["PreselJetLepdR"]->Fill(lep1.DeltaR((TLorentzVector)*selectedJets[seljet]), datasets[d],
                        true, Luminosity * scaleFactor);
                    MSPlot["PreselJetLepdPhi"]->Fill(lep1.DeltaPhi((TLorentzVector)*selectedJets[seljet]), datasets[d],
                        true, Luminosity * scaleFactor);
                    if(lep1.DeltaR(selectedJetsTLV[seljet]) < 0.05) {
                        MSPlot["CloseJetLepRat"]->Fill(
                            selectedJetsTLV[seljet].Pt() / lep1.Pt(), datasets[d], true, Luminosity * scaleFactor);
                        MSPlot["CloseJetMC"]->Fill(
                            selectedJets[seljet]->hadronFlavour(), datasets[d], true, Luminosity * scaleFactor);
                    }
                    MSPlot["PreselJetLepdR"]->Fill(lep2.DeltaR((TLorentzVector)*selectedJets[seljet]), datasets[d],
                        true, Luminosity * scaleFactor);
                    MSPlot["PreselJetLepdPhi"]->Fill(lep2.DeltaPhi((TLorentzVector)*selectedJets[seljet]), datasets[d],
                        true, Luminosity * scaleFactor);
                    if(lep2.DeltaR(selectedJetsTLV[seljet]) < 0.05) {
                        MSPlot["CloseJetLepRat"]->Fill(
                            selectedJetsTLV[seljet].Pt() / lep2.Pt(), datasets[d], true, Luminosity * scaleFactor);
                        MSPlot["CloseJetMC"]->Fill(
                            selectedJets[seljet]->hadronFlavour(), datasets[d], true, Luminosity * scaleFactor);
                    }
                    histo2D["CloseJetdRvsdPhi"]->Fill(lep1.DeltaR((TLorentzVector)*selectedJets[seljet]),
                        lep1.DeltaPhi((TLorentzVector)*selectedJets[seljet]));
                    histo2D["CloseJetdRvsdPhi"]->Fill(lep2.DeltaR((TLorentzVector)*selectedJets[seljet]),
                        lep2.DeltaPhi((TLorentzVector)*selectedJets[seljet]));
                } else if(nEl == 0 && nMu == 2 && !Electron && Muon) {
                    MSPlot["PreselJetLepdR"]->Fill(
                        lep1.DeltaR(selectedJetsTLV[seljet]), datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["PreselJetLepdR"]->Fill(
                        lep2.DeltaR(selectedJetsTLV[seljet]), datasets[d], true, Luminosity * scaleFactor);
                    if(lep1.DeltaR(selectedJetsTLV[seljet]) < 0.05)
                        MSPlot["CloseJetLepRat"]->Fill(
                            selectedJetsTLV[seljet].Pt() / lep1.Pt(), datasets[d], true, Luminosity * scaleFactor);
                    if(lep2.DeltaR(selectedJetsTLV[seljet]) < 0.05)
                        MSPlot["CloseJetLepRat"]->Fill(
                            selectedJetsTLV[seljet].Pt() / lep2.Pt(), datasets[d], true, Luminosity * scaleFactor);
                }
                jet_flavor = selectedJets[seljet]->hadronFlavour();
                JetPt = selectedJets[seljet]->Pt();
                JetEta = selectedJets[seljet]->Eta();
                temp_HT += JetPt;
                if(JetPt > 800.)
                    JetPt = 800;
                if(JetEta > 2.4) {
                    JetEta = 2.4;
                } else if(JetEta < -2.4) {
                    JetEta = -2.4;
                }

                if(fabs(jet_flavor) == 5 || fabs(jet_flavor) == 4) {
                    // SF_tag =
                    // bTool->getSF(selectedJets[seljet]->Pt(),selectedJets[seljet]->Eta(),jet_flavor,dobTagEffShift );
                    //  cout <<" "<<endl;
                    ////cout <<"jet SF nom "<<
                    /// bTool->getWeight(selectedJets[seljet]->Pt(),selectedJets[seljet]->Eta(),jet_flavor,0 ) <<endl;
                    // cout <<"jet SF minus "<<
                    // bTool->getWeight(selectedJets[seljet]->Pt(),selectedJets[seljet]->Eta(),jet_flavor,-1 ) <<endl;
                    // cout <<"jet SF plus "<<
                    // bTool->getWeight(selectedJets[seljet]->Pt(),selectedJets[seljet]->Eta(),jet_flavor,1 )    <<endl;
                } else {
                    //  cout <<" light jet... "<<endl;
                    // SF_tag =
                    // bTool->getSF(selectedJets[seljet]->Pt(),selectedJets[seljet]->Eta(),jet_flavor,domisTagEffShift);
                }
                if(selectedJets[seljet]->btag_combinedInclusiveSecondaryVertexV2BJetTags() > 0.460) {

                    selectedLBJets.push_back(selectedJets[seljet]);
                    if(selectedJets[seljet]->btag_combinedInclusiveSecondaryVertexV2BJetTags() > 0.800) {
                        selectedMBJets.push_back(selectedJets[seljet]);

                        if(selectedJets[seljet]->btag_combinedInclusiveSecondaryVertexV2BJetTags() > 0.935) {
                            selectedTBJets.push_back(selectedJets[seljet]);
                        }
                    }
                } else {
                    selectedLightJets.push_back(selectedJets[seljet]);
                }
            }

            float nJets = selectedJets.size();    // Number of Jets in Event
            float nMtags = selectedMBJets.size(); // Number of CSVM tags in Event
            float nLtags = selectedLBJets.size(); // Number of CSVL tags in Event (includes jets that pass CSVM)
            float nTtags = selectedTBJets.size(); // Number of CSVL tags in Event (includes jets that pass CSVM)
            float nFatJets = selectedFatJets.size();

            //////////////////////////////
            //  Behrends Scaling Factor //
            //////////////////////////////

            float fbehrendsSF = 1.0;

            if(nJets >= 5 && dataSetName.find("TTDileptMG") != std::string::npos)
                fbehrendsSF = DataFit->Eval(nJets) / MCFit->Eval(nJets);

            //            cout <<" med tags ...   "<< nMtags   <<endl;

            //////////////////////////////
            //  AlphaS Scaling Factor //
            //////////////////////////////

            float alphaTune = 1, falphaTuneSF = 1.0;
            if(dataSetName.find("TTJetsPowheg") != string::npos) {
                alphaTune = PythiaTune(nJets + 2);
            } else if(dataSetName.find("TTJetsMG") != string::npos) {
                alphaTune = PythiaTune(nJets + 2);
            } else if(dataSetName.find("tttt") != string::npos) {
                alphaTune = PythiaTune(nJets - 4);
            } else if(dataSetName.find("DYJets") != string::npos) {
                alphaTune = PythiaTune(nJets + 4);
            } else if(dataSetName.find("T_tW") != string::npos) {
                alphaTune = PythiaTune(nJets + 3);
            } else if(dataSetName.find("Tbar_tW") != string::npos) {
                alphaTune = PythiaTune(nJets + 3);
            } else if(dataSetName.find("WW") != string::npos) {
                alphaTune = PythiaTune(nJets + 4);
            } else if(dataSetName.find("WZ") != string::npos) {
                alphaTune = PythiaTune(nJets + 4);
            } else if(dataSetName.find("ZZ") != string::npos) {
                alphaTune = PythiaTune(nJets + 4);
            } else if(dataSetName.find("TTH") != string::npos) {
                alphaTune = PythiaTune(nJets);
            } else if(dataSetName.find("TTZ") != string::npos) {
                alphaTune = PythiaTune(nJets);
            } else if(dataSetName.find("TTW") != string::npos) {
                alphaTune = PythiaTune(nJets);
            }

            falphaTuneSF = (1.0 / alphaTune);

            float nTopTags = 0;
            float nWTags = 0;
            ////
            //// W/Top tagging
            ////

            for(int fatjet = 0; fatjet < nFatJets; fatjet++) {

                float tau1 = selectedFatJets[fatjet]->Tau1();
                float tau2 = selectedFatJets[fatjet]->Tau2();
                float prunedmass = selectedFatJets[fatjet]->PrunedMass();
                float nsubjets = selectedFatJets[fatjet]->CmsTopTagNsubjets();
                float minmass = selectedFatJets[fatjet]->CmsTopTagMinMass();
                float topmass = selectedFatJets[fatjet]->CmsTopTagMass();

                // cout <<"llop in fat jet "<< " tau1 "   <<  tau1  << " tau2 "<< tau2  << " prunedmass " << prunedmass
                // << " nsubjets " << nsubjets  << " minmass "  <<minmass<< " topmass "  << topmass <<endl;

                // W-tagging
                if((tau2 / tau1) > 0.6 && prunedmass > 50.0) {

                    nWTags++;
                    // cout <<"W-TAG!"<<endl;
                }

                // Top-tagging
                if(nsubjets > 2 && minmass > 50.0 && topmass > 150.0) {
                    // cout <<"TOP-TAG!"<<endl;
                    nTopTags++;
                }
            }

            //////////////////////
            // Sync'ing cutflow //
            //////////////////////

            if(debug)
                cout << " applying baseline event selection for cut table..." << endl;
            // Apply primary vertex selection
            bool isGoodPV = selection.isPVSelected(vertex, 4, 24., 2);
            if(debug)
                cout << "PrimaryVertexBit: " << isGoodPV << " TriggerBit: " << trigged << endl;
            selecTable.Fill(d, 0, scaleFactor);
            weightCount += scaleFactor;
            eventCount++;
            if(trigged)
                trigCount++;

            int cfTrigger = 0, cfPV = 0, cfLep1 = 0, cfLep2 = 0, cfJets2 = 0, cfJets3 = 0, cfJets4 = 0, cfTags = 0,
                cfHT = 0;
            if(Muon && Electron && dilepton) // Muon-Electron Selection Table
            {
                if(trigged) {
                    cfTrigger = 1;
                    if(isGoodPV) {
                        cfPV = 1;
                        if(nMu == 1) {
                            cfLep1 = 1;
                            if(nEl == 1) {
                                cfLep2 = 1;
                                if(nJets >= 2) {
                                    cfJets2 = 1;
                                    if(nJets >= 3) {
                                        cfJets3 = 1;
                                        if(nJets >= 4) {
                                            cfJets4 = 1;
                                            if(nMtags >= 2) {
                                                cfTags = 1;
                                                if(temp_HT >= 500) {
                                                    cfHT = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(Muon && !Electron && dilepton) // Muon-Muon Selection Table
            {
                if(!sameCharge &&
                    (diLepMass < 20 || (diLepMass > (ZMass - ZMassWindow) && diLepMass < (ZMass + ZMassWindow))))
                    ZVeto = true;
                if(trigged) {
                    cfTrigger = 1;
                    if(isGoodPV) {
                        cfPV = 1;
                        if(nMu == 2 && nEl == 0) {
                            cfLep1 = 1;
                            if(!ZVeto) {
                                cfLep2 = 1;
                                if(nJets >= 2) {
                                    cfJets2 = 1;
                                    if(nJets >= 3) {
                                        cfJets3 = 1;
                                        if(nJets >= 4) {
                                            cfJets4 = 1;
                                            if(nMtags >= 2) {
                                                cfTags = 1;
                                                if(temp_HT >= 500) {
                                                    cfHT = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(!Muon && Electron && dilepton) // Electron-Electron Selection Table
            {
                if(!sameCharge &&
                    (diLepMass < 20 || (diLepMass > (ZMass - ZMassWindow) && diLepMass < (ZMass + ZMassWindow))))
                    ZVeto = true;
                if(trigged) {
                    cfTrigger = 1;
                    if(isGoodPV) {
                        cfPV = 1;
                        if(nEl == 2 && nMu == 0) {
                            cfLep1 = 1;
                            if(!ZVeto) {
                                cfLep2 = 1;
                                if(nJets >= 2) {
                                    cfJets2 = 1;
                                    if(nJets >= 3) {
                                        cfJets3 = 1;
                                        if(nJets >= 4) {
                                            cfJets4 = 1;
                                            if(nMtags >= 2) {
                                                cfTags = 1;
                                                if(temp_HT >= 500) {
                                                    cfHT = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            /////////////////////////////
            // Aggregating ScaleFactor //
            /////////////////////////////

            scaleFactor *= centralWeight;
            scaleFactor *= lumiWeight;
            scaleFactor *= fleptonSF;
            scaleFactor *= (btagWeight_light * btagWeight_heavy);
            scaleFactor *= falphaTuneSF;
            scaleFactor *= fTopPtReWeightsf;

            cuttup->Fill(scaleFactor, normfactor, Luminosity, nJets, ttbar_flav, cfTrigger, cfPV, cfLep1, cfLep2, cfJets2, cfJets3,
                cfJets4, cfTags, cfHT);

            //////////////
            // N-1 Plot //
            //////////////

            //            MSPlot["NMinusOne"]->Fill(0, datasets[d], true, Luminosity*scaleFactor );
            if(Muon && Electron && dilepton) {
                if(nMu >= 1 && nTightEl >= 1 && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(1, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nTightEl >= 1 && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(2, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu >= 1 && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(3, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu >= 1 && nTightEl >= 1 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(4, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu >= 1 && nTightEl >= 1 && nJets >= 4 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(5, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu >= 1 && nTightEl >= 1 && nJets >= 4 && nMtags >= 2)
                    MSPlot["NMinusOne"]->Fill(6, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu >= 1 && nTightEl >= 1 && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(7, datasets[d], true, Luminosity * scaleFactor);
            }
            if(Muon && !Electron && dilepton) {
                if(nMu == 2 && nEl == 0 && !ZVeto && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(1, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && !ZVeto && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(2, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu == 2 && nEl == 0 && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(3, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu == 2 && nEl == 0 && !ZVeto && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(4, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu == 2 && nEl == 0 && !ZVeto && nJets >= 4 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(5, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu == 2 && nEl == 0 && !ZVeto && nJets >= 4 && nMtags >= 2)
                    MSPlot["NMinusOne"]->Fill(6, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nMu == 2 && nEl == 0 && !ZVeto && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(7, datasets[d], true, Luminosity * scaleFactor);
            }
            if(!Muon && Electron && dilepton) {
                if(nTightEl >= 1 && nEl >= 2 && !ZVeto && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(1, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && !ZVeto && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(2, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nTightEl >= 1 && nEl >= 2 && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(3, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nTightEl >= 1 && nEl >= 2 && !ZVeto && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(4, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nTightEl >= 1 && nEl >= 2 && !ZVeto && nJets >= 4 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(5, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nTightEl >= 1 && nEl >= 2 && !ZVeto && nJets >= 4 && nMtags >= 2)
                    MSPlot["NMinusOne"]->Fill(6, datasets[d], true, Luminosity * scaleFactor);
                if(isGoodPV && nTightEl >= 1 && nEl >= 2 && !ZVeto && nJets >= 4 && nMtags >= 2 && temp_HT >= 500)
                    MSPlot["NMinusOne"]->Fill(7, datasets[d], true, Luminosity * scaleFactor);
            }

            /////////////////////////////////
            // Applying baseline selection //
            /////////////////////////////////

            // Filling Histogram of the number of vertices before Event Selection

            if(!trigged)
                continue; // check that an HLT was triggered
            if(!isGoodPV)
                continue; // Check that there is a good Primary Vertex
            ////            if (!(selectedJets.size() >= 6)) continue; //Selection of a minimum of 6 Jets in Event
            //
            //          cout <<"Number of Muons, Electrons, Jets, BJets, JetCut, MuonChannel, ElectronChannel ===>  "<<
            //          nMu <<"  "  <<nEl<<" "<< selectedJets.size()   <<"  " <<  nLtags   <<"  "<<JetCut  <<"
            //          "<<Muon<<" "<<Electron<<endl;

            MSPlot["NbOfElectronsPreSel"]->Fill(nEl, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["NbOfLooseElectronsPreSel"]->Fill(nLooseEl, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["NbOfMediumElectronsPreSel"]->Fill(nMedEl, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["NbOfTightElectronsPreSel"]->Fill(nTightEl, datasets[d], true, Luminosity * scaleFactor);
            histo2D["nTightnEl"]->Fill(nTightEl, nEl);
            histo2D["SumXnEl"]->Fill(nTightEl + nMedEl + nLooseEl, nEl);

            if(debug)
                cout << " applying baseline event selection... nMu = " << nMu << " nEl = " << nEl << " ZVeto: " << ZVeto
                     << " sameCharge: " << sameCharge << endl;
            // Apply the lepton, btag and HT selections
            if(Muon && Electron && dilepton) {
                if(!(nMu == 1 && nEl == 1 && !sameCharge))
                    continue; // Muon-Electron Channel Selection
                lepFlavor = 2;
            } else if(Muon && !Electron && dilepton) {
                if(!(nMu == 2 && nEl == 0 && !ZVeto && !sameCharge))
                    continue; // Muon-Electron Channel Selection
                lepFlavor = 1;
            } else if(!Muon && Electron && dilepton) {
                if(!(nMu == 0 && nEl == 2 && !ZVeto && !sameCharge))
                    continue; // Muon-Electron Channel Selection
                lepFlavor = 3;
            } else {
                cerr << "Correct Channel not selected." << endl;
                exit(1);
            }

            if(scaleFactor < 0.0) {
                negTup->Fill(nJets, temp_HT, scaleFactor, normfactor, Luminosity, centralWeight);
            } else {
                posTup->Fill(nJets, temp_HT, scaleFactor, normfactor, Luminosity, centralWeight);
            }

            sort(selectedJets.begin(), selectedJets.end(), HighestCVSBtag());

            // Scan for best MET Cut before Jet requirements
            for(int metCut = 0; metCut < 100; metCut = metCut + 5) {
                if(mets[0]->Et() > metCut)
                    MSPlot["METCutAcc"]->Fill(metCut, datasets[d], true, Luminosity * scaleFactor);
                else
                    MSPlot["METCutRej"]->Fill(metCut, datasets[d], true, Luminosity * scaleFactor);
            }

            if(debug)
                cout << "HT: " << temp_HT << " nJets: " << nJets << " nMTags: " << nMtags << endl;
            vector<TLorentzVector*> selectedLeptonTLV_JC;

            if(dilepton && Muon && Electron) {
                if(!(nJets >= 2 && nMtags >= 2))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                if(!(nJets >= 4))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                if(!(temp_HT >= 500))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                selectedLeptonTLV_JC.push_back(selectedMuons[0]);
            } else if(dilepton && Muon && !Electron) {
                if(!(nJets >= 2 && nMtags >= 2))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                if(!(nJets >= 4))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                if(!(temp_HT >= 500))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                selectedLeptonTLV_JC.push_back(selectedMuons[0]);
            } else if(dilepton && !Muon && Electron) {
                if(!(nJets >= 2 && nMtags >= 2))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                if(!(nJets >= 4))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                if(!(temp_HT >= 500))
                    continue; // Jet Tag Event Selection Requirements for Mu-El dilepton channel
                selectedLeptonTLV_JC.push_back(selectedElectrons[0]);
            }
            if(debug) {
                cout << "Selection Passed." << endl;
            }

            passed++;

            // cout << "nJets: " << nJets <<  " alphaTune: " << alphaTune << " alphaTuneSF: " << falphaTuneSF << endl;

            // cout<< "ScaleFactor " << scaleFactor << " btag weight "<<btagWeight<< " PU weight " << lumiWeight << "
            // LepWeights " << fleptonSF1 << " " << fleptonSF2 << " nJets: " << selectedJets.size() << endl;

            ///////////////////
            // Control Tuple //
            ///////////////////
            //            if(selectedMuons.size() >= && Muon && !Electron)
            //            {
            //                float reliso1 = selectedMuons[0]->relPfIso(4, 0.5);
            //                float reliso2 = selectedMuons[1]->relPfIso(4, 0.5);
            //                controlTup->Fill(selectedMuons[0]->Pt(), reliso1, selectedMuons[0]->Eta(),
            //                selectedMuons[1]->Pt(), reliso2, selectedMuons[1]->Eta(), selectedJets[0]->Pt(),
            //                selectedMBJets[0]->Pt(), nJets, nMtags, scaleFactor, nvertices, normfactor, Luminosity);
            //            }
            //            else if(selectedMuons.size() >= 1 && selectedTightElectrons.size() >= 1 && Muon && Electron)
            //            {
            //                float reliso1 = selectedMuons[0]->relPfIso(4, 0.5);
            //                float reliso2 = ElectronRelIso(selectedElectrons[0], rho);
            //                controlTup->Fill(selectedMuons[0]->Pt(), reliso1, selectedMuons[0]->Eta(),
            //                selectedElectrons[0]->Pt(), reliso2, selectedElectrons[0]->Eta(), selectedJets[0]->Pt(),
            //                selectedMBJets[0]->Pt(), nJets, nMtags, scaleFactor, nvertices, normfactor, Luminosity);
            //            }
            //            else if(selectedTightElectrons.size() >= 1 && selectedElectrons.size() >= 2 && !Muon &&
            //            Electron)
            //            {
            //                float reliso1 = ElectronRelIso(selectedElectrons[0], rho);
            //                float reliso2 = ElectronRelIso(selectedElectrons[1], rho);
            //                controlTup->Fill(selectedElectrons[0]->Pt(), reliso1, selectedElectrons[0]->Eta(),
            //                selectedElectrons[1]->Pt(), reliso2, selectedElectrons[1]->Eta(), selectedJets[0]->Pt(),
            //                selectedMBJets[0]->Pt(), nJets, nMtags, scaleFactor, nvertices, normfactor, Luminosity);
            //            }

            //////////////////////////////////////
            // MVA Hadronic Top Reconstructions //
            //////////////////////////////////////

            double TriJetMass, DiJetMass;
            TLorentzVector Wh, Bh, Th;
            int wj1;
            int wj2;
            int bj1;

            if(nJets >= 4) {
                jetCombiner->ProcessEvent_SingleHadTop(
                    datasets[d], mcParticlesMatching_, selectedJets, selectedLeptonTLV_JC[0], scaleFactor);

                if(!TrainMVA) {
                    MVAvals1 = jetCombiner->getMVAValue(MVAmethod, 1); // 1 means the highest MVA value
                    MSPlot["MVA1TriJet"]->Fill(MVAvals1.first, datasets[d], true, Luminosity * scaleFactor);
                    topness = MVAvals1.first;
                    for(Int_t seljet1 = 0; seljet1 < selectedJets.size(); seljet1++) {
                        if(seljet1 == MVAvals1.second[0] || seljet1 == MVAvals1.second[1] ||
                            seljet1 == MVAvals1.second[2]) {
                            MVASelJets1.push_back(selectedJets[seljet1]);
                        }
                    }

                    // check data-mc agreement of kin. reco. variables.
                    float mindeltaR = 100.;
                    float mindeltaR_temp = 100.;

                    // define the jets from W as the jet pair with smallest deltaR
                    for(int m = 0; m < MVASelJets1.size(); m++) {
                        for(int n = 0; n < MVASelJets1.size(); n++) {
                            if(n == m)
                                continue;
                            TLorentzVector lj1 = *MVASelJets1[m];
                            TLorentzVector lj2 = *MVASelJets1[n];
                            mindeltaR_temp = lj1.DeltaR(lj2);
                            if(mindeltaR_temp < mindeltaR) {
                                mindeltaR = mindeltaR_temp;
                                wj1 = m;
                                wj2 = n;
                            }
                        }
                    }
                    // find the index of the jet not chosen as a W-jet
                    for(unsigned int p = 0; p < MVASelJets1.size(); p++) {
                        if(p != wj1 && p != wj2)
                            bj1 = p;
                    }

                    if(debug)
                        cout << "Processing event with jetcombiner : 3 " << endl;

                    // now that putative b and W jets are chosen, calculate the six kin. variables.
                    Wh = *MVASelJets1[wj1] + *MVASelJets1[wj2];
                    Bh = *MVASelJets1[bj1];
                    Th = Wh + Bh;

                    TriJetMass = Th.M();

                    DiJetMass = Wh.M();
                    // DeltaR
                    float AngleThWh = fabs(Th.DeltaPhi(Wh));
                    float AngleThBh = fabs(Th.DeltaPhi(Bh));

                    float btag = MVASelJets1[bj1]->btag_combinedInclusiveSecondaryVertexV2BJetTags();

                    double PtRat = ((*MVASelJets1[0] + *MVASelJets1[1] + *MVASelJets1[2]).Pt()) /
                        (MVASelJets1[0]->Pt() + MVASelJets1[1]->Pt() + MVASelJets1[2]->Pt());
                    double diLepThdR = fabs(Th.DeltaR(diLep));
                    double diLepThdPhi = fabs(Th.DeltaPhi(diLep));
                    if(debug)
                        cout << "Processing event with jetcombiner : 4 " << endl;

                    MSPlot["MVA1TriJetMass"]->Fill(TriJetMass, datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["MVA1DiJetMass"]->Fill(DiJetMass, datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["MVA1BTag"]->Fill(btag, datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["MVA1PtRat"]->Fill(PtRat, datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["MVA1AnThWh"]->Fill(AngleThWh, datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["MVA1AnThBh"]->Fill(AngleThBh, datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["MVA1dRThDiLep"]->Fill(diLepThdR, datasets[d], true, Luminosity * scaleFactor);
                    MSPlot["MVA1dPhiThDiLep"]->Fill(diLepThdPhi, datasets[d], true, Luminosity * scaleFactor);

                    if(debug)
                        cout << "Processing event with jetcombiner : 8 " << endl;
                }
            }

            ///////////////////////////////////
            // Filling histograms / plotting //
            ///////////////////////////////////

            MSPlot["NbOfVertices"]->Fill(vertex.size(), datasets[d], true, Luminosity * scaleFactor);

            //////////////////////
            // Muon Based Plots //
            //////////////////////
            for(Int_t selmu = 0; selmu < selectedMuons.size(); selmu++) {
                MSPlot["MuonPt"]->Fill(selectedMuons[selmu]->Pt(), datasets[d], true, Luminosity * scaleFactor);
                float reliso = selectedMuons[selmu]->relPfIso(4, 0.5);
                MSPlot["MuonRelIsolation"]->Fill(reliso, datasets[d], true, Luminosity * scaleFactor);
            }
            if(Muon && !Electron && nMu == 2 && sameCharge && !ZVeto) {
                MSPlot["PostselDiLepMass"]->Fill(diLepMass, datasets[d], true, Luminosity * scaleFactor);
            }

            //////////////////////////
            // Electron Based Plots //
            //////////////////////////

            for(Int_t selel = 0; selel < selectedElectrons.size(); selel++) {
                float reliso = ElectronRelIso(selectedElectrons[selel], rho);
                MSPlot["ElectronRelIsolation"]->Fill(reliso, datasets[d], true, Luminosity * scaleFactor);
                MSPlot["ElectronPt"]->Fill(selectedElectrons[selel]->Pt(), datasets[d], true, Luminosity * scaleFactor);
            }
            if(!Muon && Electron && nEl >= 2 && sameCharge && !ZVeto) {
                MSPlot["PostselDiLepMass"]->Fill(diLepMass, datasets[d], true, Luminosity * scaleFactor);
            }

            if(nEl >= 1) {
                float epRat = selectedElectrons[0]->E() / selectedElectrons[0]->P();
                //                cout << "Electron E/P Ratio : " << epRat << endl;
            }

            ////////////////////////
            // Lepton Information //
            ////////////////////////

            dRLep = lep1.DeltaR(lep2);

            //////////////////////
            // Jets Based Plots //
            //////////////////////

            HT = 0;
            float HT1M2L = 0, H1M2L = 0, HTbjets = 0, HT2M = 0, H2M = 0, dRbb = 0;

            for(Int_t seljet1 = 0; seljet1 < selectedJets.size(); seljet1++) {
                if(nMtags >= 2 && seljet1 >= 2) {
                    jetpt = selectedJets[seljet1]->Pt();
                    HT2M = HT2M + jetpt;
                    H2M = H2M + selectedJets[seljet1]->P();
                }
                if(selectedJets[seljet1]->btag_combinedInclusiveSecondaryVertexV2BJetTags() > 0.800) {
                    HTb += selectedJets[seljet1]->Pt();
                }
                MSPlot["BdiscBJetCand_CSV"]->Fill(
                    selectedJets[seljet1]->btag_combinedInclusiveSecondaryVertexV2BJetTags(), datasets[d], true,
                    Luminosity * scaleFactor);
                MSPlot["JetEta"]->Fill(selectedJets[seljet1]->Eta(), datasets[d], true, Luminosity * scaleFactor);
                // Event-level variables
                jetpt = selectedJets[seljet1]->Pt();
                HT = HT + jetpt;
                H = H + selectedJets[seljet1]->P();
                jettup->Fill(scaleFactor, normfactor, Luminosity, selectedJets[seljet1]->neutralHadronEnergyFraction(),
                    selectedJets[seljet1]->neutralEmEnergyFraction(), selectedJets[seljet1]->nConstituents(),
                    selectedJets[seljet1]->chargedHadronEnergyFraction(), selectedJets[seljet1]->chargedMultiplicity(),
                    selectedJets[seljet1]->chargedEmEnergyFraction());
            }
            dRbb = fabs(selectedJets[0]->DeltaR(*selectedJets[1]));
            HTRat = (selectedJets[0]->Pt() + selectedJets[1]->Pt()) / HT;
            HTH = HT / H;

            MSPlot["HTExcess2M"]->Fill(HT2M, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["HTH"]->Fill(HTH, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["HT_SelectedJets"]->Fill(HT, datasets[d], true, Luminosity * scaleFactor);
            histo2D["HTLepSep"]->Fill(HT, lep1.DeltaR(lep2));
            sort(selectedJets.begin(), selectedJets.end(), HighestPt()); // order Jets wrt Pt for tuple output

            if(selectedJets.size() >= 3)
                MSPlot["3rdJetPt"]->Fill(selectedJets[2]->Pt(), datasets[d], true, Luminosity * scaleFactor);
            if(selectedJets.size() >= 4)
                MSPlot["4thJetPt"]->Fill(selectedJets[3]->Pt(), datasets[d], true, Luminosity * scaleFactor);
            if(selectedJets.size() >= 5)
                MSPlot["5thJetPt"]->Fill(selectedJets[4]->Pt(), datasets[d], true, Luminosity * scaleFactor);
            if(selectedJets.size() >= 6)
                MSPlot["6thJetPt"]->Fill(selectedJets[5]->Pt(), datasets[d], true, Luminosity * scaleFactor);

            if(dilepton && Muon && Electron) {
                muonpt = selectedMuons[0]->Pt();
                muoneta = selectedMuons[0]->Eta();
                electronpt = selectedElectrons[0]->Pt();
            }
            if(dilepton && Muon && !Electron) {
                muonpt = selectedMuons[0]->Pt();
                muoneta = selectedMuons[0]->Eta();
            }
            if(dilepton && !Muon && Electron) {
                muonpt = selectedElectrons[0]->Pt();
                muoneta = selectedElectrons[0]->Eta();
            }

            if(nMtags >= 1) {
                bjetpt = selectedMBJets[0]->Pt();
            }

            ///////////////////
            // MET Based Plots//
            ///////////////////

            MSPlot["MET"]->Fill(mets[0]->Et(), datasets[d], true, Luminosity * scaleFactor);

            //////////////////
            // Topology Plots//
            /////////////////
            float tSph = 0, dSph = 0, tdSph = 0, tCen = 0, dCen = 0, tdCen = 0;

            topologyW->setPartList(selectedJetsTLV, selectedJetsTLV);
            float fSphericity = topologyW->get_sphericity();
            float fOblateness = topologyW->oblateness();
            float fAplanarity = topologyW->get_aplanarity();
            float fh10 = topologyW->get_h10();
            float fh20 = topologyW->get_h20();
            float fh30 = topologyW->get_h30();
            float fh40 = topologyW->get_h40();
            float fh50 = topologyW->get_h50();
            float fh60 = topologyW->get_h60();
            float fht = topologyW->get_ht();
            float fht3 = topologyW->get_ht3();
            float fet0 = topologyW->get_et0();
            float fsqrts = topologyW->get_sqrts();
            float fnjetW = topologyW->get_njetW();
            float fet56 = topologyW->get_et56();
            float fcentrality = topologyW->get_centrality();

            vector<TLorentzVector> selectedParticlesTLV, diLepSystemTLV, topDiLepSystemTLV;
            // collection Total Event TLVs
            selectedParticlesTLV.insert(
                selectedParticlesTLV.end(), selectedElectronsTLV_JC.begin(), selectedElectronsTLV_JC.end());
            selectedParticlesTLV.insert(
                selectedParticlesTLV.end(), selectedMuonsTLV_JC.begin(), selectedMuonsTLV_JC.end());
            selectedParticlesTLV.insert(selectedParticlesTLV.end(), selectedJetsTLV.begin(), selectedJetsTLV.end());
            selectedParticlesTLV.push_back(*mets[0]);
            // collecting diLep TLVs
            diLepSystemTLV.push_back(lep1);
            diLepSystemTLV.push_back(lep2);
            diLepSystemTLV.push_back(*mets[0]);
            // collecting topDiLep TLVs
            topDiLepSystemTLV.insert(topDiLepSystemTLV.end(), diLepSystemTLV.begin(), diLepSystemTLV.end());
            if(!TrainMVA) {
                if(nJets >= 4) {
                    topDiLepSystemTLV.push_back(*MVASelJets1[wj1]);
                    topDiLepSystemTLV.push_back(*MVASelJets1[wj2]);
                    topDiLepSystemTLV.push_back(*MVASelJets1[bj1]);
                }

                tSph = Sphericity(selectedParticlesTLV), tCen = Centrality(selectedParticlesTLV);
                dSph = Sphericity(diLepSystemTLV), dCen = Centrality(diLepSystemTLV);
                tdSph = Sphericity(topDiLepSystemTLV), tdCen = Centrality(topDiLepSystemTLV);
            }

            MSPlot["TotalSphericity"]->Fill(tSph, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["TotalCentrality"]->Fill(tCen, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["DiLepSphericity"]->Fill(dSph, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["DiLepCentrality"]->Fill(dCen, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["TopDiLepSphericity"]->Fill(tdSph, datasets[d], true, Luminosity * scaleFactor);
            MSPlot["TopDiLepCentrality"]->Fill(tdCen, datasets[d], true, Luminosity * scaleFactor);

            /////////////////////
            // Calculating BDT //
            /////////////////////

            if(nJets >= 4) {
                Eventcomputer_->FillVar("topness", topness);
                Eventcomputer_->FillVar("LeadLepPt", muonpt);
                Eventcomputer_->FillVar("LeadLepEta", muoneta);
                Eventcomputer_->FillVar("HTH", HTH);
                Eventcomputer_->FillVar("HTRat", HTRat);
                Eventcomputer_->FillVar("HTb", HTb);
                Eventcomputer_->FillVar("nLtags", nLtags);
                Eventcomputer_->FillVar("nMtags", nMtags);
                Eventcomputer_->FillVar("nTtags", nTtags);
                Eventcomputer_->FillVar("nJets", selectedJets.size());
                //                Eventcomputer_->FillVar("Jet3Pt", selectedJets[2]->Pt());
                //                Eventcomputer_->FillVar("Jet4Pt", selectedJets[3]->Pt());
                Eventcomputer_->FillVar("HT2M", HT2M);
                Eventcomputer_->FillVar("EventSph", tSph);
                //                Eventcomputer_->FillVar("EventCen", tCen);
                //               Eventcomputer_->FillVar("DiLepSph", dSph);
                //                Eventcomputer_->FillVar("DiLepCen", dCen);
                //                Eventcomputer_->FillVar("TopDiLepSph", tdSph);
                //                Eventcomputer_->FillVar("TopDiLepCen", tdCen);

                std::map<std::string, Float_t> MVAVals = Eventcomputer_->GetMVAValues();

                for(std::map<std::string, Float_t>::const_iterator it = MVAVals.begin(); it != MVAVals.end(); ++it) {

                    //  cout <<"MVA Method : "<< it->first    <<" Score : "<< it->second <<endl;
                    BDTScore = it->second;
                }
            }

            if((dataSetName.find("Data") <= 0 || dataSetName.find("data") <= 0 || dataSetName.find("DATA") <= 0) &&
                Muon && Electron) {
                cout << "Data Event Passed Selection.  Run: " << event->runId()
                     << " LumiSection: " << event->lumiBlockId() << " Event: " << event->eventId() << " HT: " << HT
                     << " nMTags: " << nMtags << endl;
                cout << "Muon Eta: " << selectedMuons[0]->Eta() << " Muon Pt: " << selectedMuons[0]->Pt()
                     << " Electron Eta: " << selectedElectrons[0]->Eta()
                     << " Electron Pt: " << selectedElectrons[0]->Pt() << endl;
            }

            if((dataSetName.find("Data") <= 0 || dataSetName.find("data") <= 0 || dataSetName.find("DATA") <= 0) &&
                Muon && !Electron) {
                cout << "Data Event Passed Selection.  Run: " << event->runId()
                     << " LumiSection: " << event->lumiBlockId() << " Event: " << event->eventId() << " HT: " << HT
                     << " nMTags: " << nMtags << endl;
                cout << "Muon1 Eta: " << selectedMuons[0]->Eta() << " Muon1 Pt: " << selectedMuons[0]->Pt()
                     << " Muon2 Eta: " << selectedMuons[1]->Eta() << " Muon2 Pt: " << selectedMuons[1]->Pt() << endl;
            }
            if((dataSetName.find("Data") <= 0 || dataSetName.find("data") <= 0 || dataSetName.find("DATA") <= 0) &&
                !Muon && Electron) {
                cout << "Data Event Passed Selection.  Run: " << event->runId()
                     << " LumiSection: " << event->lumiBlockId() << " Event: " << event->eventId() << " HT: " << HT
                     << " nMTags: " << nMtags << endl;
                cout << "Electron1 Eta: " << selectedElectrons[0]->Eta()
                     << " Electron1 Pt: " << selectedElectrons[0]->Pt()
                     << " Electron2 Eta: " << selectedElectrons[1]->Eta()
                     << " Electron2 Pt: " << selectedElectrons[1]->Pt() << endl;
            }

            //////////////////
            // Filling nTuple//
            //////////////////

            //	  tup->Fill(nJets,nLtags,nMtags,nTtags,HT,muonpt,muoneta,electronpt,bjetpt,HT2M,HTb,HTH,HTRat,topness,scaleFactor,nvertices,normfactor,Luminosity,weight_0);

            float vals[90] = { BDTScore, nJets, nFatJets, nWTags, nTopTags, nLtags, nMtags, nTtags,
                (float)(nJets > 0 ? selectedJets[0]->Pt() : -9999), (float)(nJets > 1 ? selectedJets[1]->Pt() : -9999),
                (float)(nJets > 2 ? selectedJets[2]->Pt() : -9999), (float)(nJets > 3 ? selectedJets[3]->Pt() : -9999),
                (float)(nJets > 4 ? selectedJets[4]->Pt() : -9999), (float)(nJets > 5 ? selectedJets[5]->Pt() : -9999),
                (float)(nJets > 6 ? selectedJets[6]->Pt() : -9999), (float)(nJets > 7 ? selectedJets[7]->Pt() : -9999),
                (float)mets[0]->Et(), HT, 0, 0, 0, dRLep, (float)(sameCharge ? 2 : 0), lepFlavor, (float)nLep, bjetpt,
                dRbb, HT2M, HTb, HTH, HTRat, topness, tSph, tCen, dSph, dCen, tdSph, tdCen, fnjetW, ttbar_flav,
                fleptonSF, btagWeightCSV, btagWeight_light, btagWeight_lightUp, btagWeight_lightDown, btagWeight_heavy, btagWeight_heavyUp,
                btagWeight_heavyDown, lumiWeight, lumiWeight_up, lumiWeight_down, btagWeightCSVLFUp ,btagWeightCSVLFDown ,btagWeightCSVHFUp ,btagWeightCSVHFDown ,btagWeightCSVHFStats1Up, btagWeightCSVHFStats1Down,
                btagWeightCSVHFStats2Up, btagWeightCSVHFStats2Down, btagWeightCSVLFStats1Up, btagWeightCSVLFStats1Down, btagWeightCSVLFStats2Up,
                btagWeightCSVLFStats2Down, btagWeightCSVCFErr1Up, btagWeightCSVCFErr1Down, btagWeightCSVCFErr2Up, btagWeightCSVCFErr2Down, falphaTuneSF, fTopPtReWeightsf, ISRsf,
                scaleFactor, nvertices, normfactor, Luminosity, centralWeight, weight1, weight2, weight3, weight4,
                weight5, weight6, weight7, weight8, diLepMass, currentRun, LumiBlock, currentEvent };
            //                "BDT:nJets:nFatJets:nWTags:nTopTags:nLtags:nMtags:nTtags:HT:LeadingMuonPt:LeadingMuonEta:LeadingElectronPt:LeadingBJetPt:HT2L:HTb:HTH:HTRat:topness:ScaleFactor:PU:NormFactor:Luminosity:GenWeight");
            if(Muon && Electron) {
                vals[18] = muonpt;
                vals[19] = muoneta;
                vals[20] = selectedElectrons[0]->Pt();
            }
            if(Muon && !Electron) {
                vals[18] = muonpt;
                vals[19] = muoneta;
                vals[20] = selectedMuons[1]->Pt();
            }
            if(!Muon && Electron) {
                vals[18] = muonpt;
                vals[19] = muoneta;
                vals[20] = selectedElectrons[1]->Pt();
            }

            tup->Fill(vals);
            if(nEl >= 1)
                eltup->Fill(scaleFactor, normfactor, Luminosity, selectedElectrons[0]->superClusterEta(),
                    selectedElectrons[0]->sigmaIEtaIEta_full5x5(), fabs(selectedElectrons[0]->deltaEtaIn()),
                    fabs(selectedElectrons[0]->deltaPhiIn()), selectedElectrons[0]->hadronicOverEm(),
                    ElectronRelIso(selectedElectrons[0], rho), selectedElectrons[0]->ioEmIoP(),
                    fabs(selectedElectrons[0]->d0()), fabs(selectedElectrons[0]->dz()),
                    selectedElectrons[0]->missingHits(), selectedElectrons[0]->Pt());
            if(nMu >= 1)
                mutup->Fill(
                    scaleFactor, normfactor, Luminosity, muonpt, muoneta, selectedMuons[0]->relPfIso(4, 0.5), nMu);

        } // End Loop on Events

        tupfile->cd();
        tup->Write();
        //        negTup->Write();
        //        posTup->Write();
        //        sfTup->Write();
        //	eltup->Write();
        //	mutup->Write();
        //	jettup->Write();
        cuttup->Write();
        tupfile->Close();

        //        tupMfile->cd();
        //        Mtup->Write();
        //        tupMfile->Close();
        cout << "n events passed  =  " << passed << endl;
        cout << "n events with negative weights = " << negWeights << endl;
        cout << "n events passing Trigger = " << trigCount << endl;
        cout << "Event Count: " << eventCount << endl;
        cout << "Weight Count: " << weightCount << endl;
        // important: free memory
        treeLoader.UnLoadDataset();
    } // End Loop on Datasets

    eventlist.close();
    if(fillingbTagHistos)
        delete btwt;

    /////////////
    // Writing //
    /////////////

    cout << " - Writing outputs to the files ..." << endl;

    //////////////////////
    // Selection tables //
    //////////////////////

    //(bool mergeTT, bool mergeQCD, bool mergeW, bool mergeZ, bool mergeST)
    selecTable.TableCalculator(true, true, true, true, true);

    // Options : WithError (false), writeMerged (true), useBookTabs (false), addRawsyNumbers (false), addEfficiencies
    // (false), addTotalEfficiencies (false), writeLandscape (false)
    selecTable.Write(outputDirectory + "/FourTop" + postfix + "_Table" + dataSetName + "_" + channelpostfix + ".tex",
        false, true, true, true, false, false, true);

    fout->cd();
    TFile* foutmva = new TFile("foutMVA.root", "RECREATE");
    cout << " after cd .." << endl;

    string pathPNGJetCombi = pathPNG + "JetCombination/";
    mkdir(pathPNGJetCombi.c_str(), 0777);
    if(TrainMVA)
        jetCombiner->Write(foutmva, true, pathPNGJetCombi.c_str());

    // Output ROOT file
    for(map<string, MultiSamplePlot*>::const_iterator it = MSPlot.begin(); it != MSPlot.end(); it++) {
        string name = it->first;
        MultiSamplePlot* temp = it->second;
        temp->Draw(name.c_str(), 0, false, false, false, 1);
        temp->Write(fout, name, false, pathPNG, "pdf");
    }

    TDirectory* th2dir = fout->mkdir("Histos2D");
    th2dir->cd();

    for(map<std::string, TH2F*>::const_iterator it = histo2D.begin(); it != histo2D.end(); it++) {

        TH2F* temp = it->second;
        temp->Write();
        // TCanvas* tempCanvas = TCanvasCreator(temp, it->first);
        // tempCanvas->SaveAs( (pathPNG+it->first+".png").c_str() );
    }

    TDirectory* th1dir = fout->mkdir("Histos1D");
    th1dir->cd();
    //    puHisto->Write();

    delete fout;
    cout << "It took us " << ((double)clock() - start) / CLOCKS_PER_SEC << " to run the program" << endl;
    cout << "********************************************" << endl;
    cout << "           End of the program !!            " << endl;
    cout << "********************************************" << endl;

    return 0;
}

int Factorial(int N = 1)
{
    int fact = 1;
    for(int i = 1; i <= N; i++)
        fact = fact * i; // OR fact *= i;
    return fact;
}

float Sphericity(vector<TLorentzVector> parts)
{
    if(parts.size() > 0) {
        double spTensor[3 * 3] = { 0., 0., 0., 0., 0., 0., 0., 0., 0. };
        int counter = 0;
        float tensorNorm = 0, y1 = 0, y2 = 0, y3 = 0;

        for(int tenx = 0; tenx < 3; tenx++) {
            for(int teny = 0; teny < 3; teny++) {
                for(int selpart = 0; selpart < parts.size(); selpart++) {

                    spTensor[counter] += ((parts[selpart][tenx]) * (parts[selpart][teny]));
                    //                    if((tenx == 0 && teny == 2) || (tenx == 2 && teny == 1))
                    //                    {
                    //                    cout << "nan debug term " << counter+1 << ": " <<
                    //                    (parts[selpart][tenx])*(parts[selpart][teny]) << endl;
                    //                    cout << "Tensor Building Term " << counter+1 << ": " << spTensor[counter] <<
                    //                    endl;
                    //                    }
                    if(tenx == 0 && teny == 0) {
                        tensorNorm += parts[selpart].Vect().Mag2();
                    }
                }
                if((tenx == 0 && teny == 2) || (tenx == 2 && teny == 1)) {
                    //                    cout << "Tensor term pre-norm " << counter+1 << ": " << spTensor[counter] <<
                    //                    endl;
                }
                spTensor[counter] /= tensorNorm;
                //                cout << "Tensor Term " << counter+1 << ": " << spTensor[counter] << endl;
                counter++;
            }
        }
        TMatrixDSym m(3, spTensor);
        // m.Print();
        TMatrixDSymEigen me(m);
        TVectorD eigenval = me.GetEigenValues();
        vector<float> eigenVals;
        eigenVals.push_back(eigenval[0]);
        eigenVals.push_back(eigenval[1]);
        eigenVals.push_back(eigenval[2]);
        sort(eigenVals.begin(), eigenVals.end());
        // cout << "EigenVals: "<< eigenVals[0] << ", " << eigenVals[1] << ", " << eigenVals[2] << ", " << endl;
        float sp = 3.0 * (eigenVals[0] + eigenVals[1]) / 2.0;
        // cout << "Sphericity: " << sp << endl;
        return sp;
    } else {
        return 0;
    }
}
float Centrality(vector<TLorentzVector> parts)
{
    float E = 0, ET = 0;
    for(int selpart = 0; selpart < parts.size(); selpart++) {
        E += parts[selpart].E();
        ET += parts[selpart].Et();
    }
    return ET / E;
}

float ElectronRelIso(TRootElectron* el, float rho)
{
    double EffectiveArea = 0.;
    // Updated to Spring 2015 EA from
    // https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_14/RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt#L8
    if(fabs(el->superClusterEta()) >= 0.0 && fabs(el->superClusterEta()) < 1.0)
        EffectiveArea = 0.1752;
    if(fabs(el->superClusterEta()) >= 1.0 && fabs(el->superClusterEta()) < 1.479)
        EffectiveArea = 0.1862;
    if(fabs(el->superClusterEta()) >= 1.479 && fabs(el->superClusterEta()) < 2.0)
        EffectiveArea = 0.1411;
    if(fabs(el->superClusterEta()) >= 2.0 && fabs(el->superClusterEta()) < 2.2)
        EffectiveArea = 0.1534;
    if(fabs(el->superClusterEta()) >= 2.2 && fabs(el->superClusterEta()) < 2.3)
        EffectiveArea = 0.1903;
    if(fabs(el->superClusterEta()) >= 2.3 && fabs(el->superClusterEta()) < 2.4)
        EffectiveArea = 0.2243;
    if(fabs(el->superClusterEta()) >= 2.4 && fabs(el->superClusterEta()) < 5.0)
        EffectiveArea = 0.2687;

    double isoCorr = 0;
    isoCorr = el->neutralHadronIso(3) + el->photonIso(3) - rho * EffectiveArea;
    float isolation = (el->chargedHadronIso(3) + (isoCorr > 0.0 ? isoCorr : 0.0)) / (el->Pt());

    return isolation;
}

float PythiaTune(int jets)
{
    float sf = 1;

    if(jets == 0)
        sf = 0.9747749;
    else if(jets == 1)
        sf = 0.9764329;
    else if(jets == 2)
        sf = 0.9733197;
    else if(jets == 3)
        sf = 0.9815515;
    else if(jets == 4)
        sf = 0.9950933;
    else if(jets == 5)
        sf = 1.0368650;
    else if(jets == 6)
        sf = 1.1092038;
    else if(jets == 7)
        sf = 1.1842445;
    else if(jets == 8)
        sf = 1.3019452;
    else if(jets == 9)
        sf = 1.1926751;
    else if(jets >= 10)
        sf = 1.5920859;

    return sf;
}
