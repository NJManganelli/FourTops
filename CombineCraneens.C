#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"

void CombineCraneens()
{
	gROOT->SetBatch(kTRUE);
	
	cout << "Getting Files" << endl;
	string Data = "Craneens_Comb/Data_Run2_TopTree_Study.root";  //Getting Files
	string TTDileptMG = "Craneens_Comb/TTJetsMG_Run2_TopTree_Study.root";
	string TTDileptPowheg_JERUp = "Craneens_Comb/TTJetsPowheg_JERUp_Run2_TopTree_Study.root";
	string TTDileptPowheg_JERDown = "Craneens_Comb/TTJetsPowheg_JERDown_Run2_TopTree_Study.root";
	string TTDileptPowheg_JESUp = "Craneens_Comb/TTJetsPowheg_JESUp_Run2_TopTree_Study.root";
	string TTDileptPowheg_JESDown = "Craneens_Comb/TTJetsPowheg_JESDown_Run2_TopTree_Study.root";
	string TTDileptPowheg_ScaleHUp = "Craneens_Comb/TTJetsPowheg_ScaleHUp_Run2_TopTree_Study.root";
	string TTDileptPowheg_ScaleHDown = "Craneens_Comb/TTJetsPowheg_ScaleHDown_Run2_TopTree_Study.root";
	string ST_tW = "Craneens_Comb/ST_tW_Run2_TopTree_Study.root";

	string DYJets = "Craneens_Comb/DYJets_Run2_TopTree_Study.root";
	string NP_overlay_ttttNLO = "Craneens_Comb/NP_overlay_ttttNLO_Run2_TopTree_Study.root";
	string NP_overlay_ttttNLO_JESUp = "Craneens_Comb/ttttNLO_JESUp_Run2_TopTree_Study.root";
	string NP_overlay_ttttNLO_JESDown = "Craneens_Comb/ttttNLO_JESDown_Run2_TopTree_Study.root";
	string NP_overlay_ttttNLO_JERUp = "Craneens_Comb/ttttNLO_JERUp_Run2_TopTree_Study.root";
	string NP_overlay_ttttNLO_JERDown = "Craneens_Comb/ttttNLO_JERDown_Run2_TopTree_Study.root";
	string TTDileptPowheg = "Craneens_Comb/TTJetsPowheg_Run2_TopTree_Study.root";
	string TTH = "Craneens_Comb/TTH_Run2_TopTree_Study.root";
	string TTZ = "Craneens_Comb/TTZ_Run2_TopTree_Study.root";
	string TTW = "Craneens_Comb/TTW_Run2_TopTree_Study.root";
	string TTJets_SingleLept = "Craneens_Comb/TTJets_SingleLept_Run2_TopTree_Study.root";

	vector < string > files;

	files.push_back(Data);
	files.push_back(NP_overlay_ttttNLO);
	files.push_back(NP_overlay_ttttNLO_JERUp);
	files.push_back(NP_overlay_ttttNLO_JERDown);
	files.push_back(NP_overlay_ttttNLO_JESUp);
	files.push_back(NP_overlay_ttttNLO_JESDown);
	files.push_back(TTDileptMG);
	files.push_back(TTDileptPowheg);
	files.push_back(TTDileptPowheg_JERUp);
	files.push_back(TTDileptPowheg_JERDown);
	files.push_back(TTDileptPowheg_JESUp);
	files.push_back(TTDileptPowheg_JESDown);
	files.push_back(TTDileptPowheg_ScaleHUp);
	files.push_back(TTDileptPowheg_ScaleHDown);
	files.push_back(ST_tW);
	files.push_back(DYJets);
	files.push_back(TTH);
	files.push_back(TTZ);
	files.push_back(TTW);
	files.push_back(TTJets_SingleLept);



	for(int i = 0; i < files.size(); i++){
		TFile f1(files[i].c_str(), "update");
                cout << i << " : 1 " << f1.GetPath() << endl;
		TNtuple* oldMuEl = (TNtuple*)f1.Get("Craneen__MuEl");
		TNtuple* newMuEl = (TNtuple*)oldMuEl->CloneTree();
		delete oldMuEl;
                cout << i << " : 1.1" << endl;
		TNtuple* oldMuMu = (TNtuple*)f1.Get("Craneen__MuMu");
		TNtuple* newMuMu = (TNtuple*)oldMuMu->CloneTree();
		delete oldMuMu;
                cout << i << " : 1.2" << endl;
		TNtuple* oldElEl = (TNtuple*)f1.Get("Craneen__ElEl");
		TNtuple* newElEl = (TNtuple*)oldElEl->CloneTree();
		delete oldElEl;
                cout << i << " : 2" << endl;
		newMuEl->SetNameTitle("Craneen__Combined", "Craneen__Combined");
		TList *thelist = new TList;
		thelist->Add(newMuEl);
		thelist->Add(newMuMu);
		thelist->Add(newElEl);
                cout << i << " : 3" << endl;
		TNtuple *Comb = (TNtuple*)TTree::MergeTrees(thelist);
		Comb->SetNameTitle("Craneen__Combined", "Craneen__Combined");
		delete newMuEl;
		delete newMuMu;
		delete newElEl;
		Comb->Write();
                cout << i << " : 4" << endl;
		delete Comb;
		f1.Close();
                cout << i << " : 5" << endl;		
	}

}
