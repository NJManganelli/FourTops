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
	TFile *Data = TFile::Open("Craneens_Comb/Data_Run2_TopTree_Study.root", "UPDATE");  //Getting Files
	TFile *TTDileptMG = TFile::Open("Craneens_Comb/TTDileptMG_Run2_TopTree_Study.root", "UPDATE");
	TFile *TTDileptMG_JERUp = TFile::Open("Craneens_Comb/TTDileptMG_JERUp_Run2_TopTree_Study.root", "UPDATE");
	TFile *TTDileptMG_JERDown = TFile::Open("Craneens_Comb/TTDileptMG_JERDown_Run2_TopTree_Study.root", "UPDATE");
	TFile *TTDileptMG_JESUp = TFile::Open("Craneens_Comb/TTDileptMG_JESUp_Run2_TopTree_Study.root", "UPDATE");
	TFile *TTDileptMG_JESDown = TFile::Open("Craneens_Comb/TTDileptMG_JESDown_Run2_TopTree_Study.root", "UPDATE");
	TFile *T_tW = TFile::Open("Craneens_Comb/T_tW_Run2_TopTree_Study.root", "UPDATE");
	TFile *Tbar_tW = TFile::Open("Craneens_Comb/Tbar_tW_Run2_TopTree_Study.root", "UPDATE");
	TFile *DYJets = TFile::Open("Craneens_Comb/DYJets_Run2_TopTree_Study.root", "UPDATE");
	TFile *NP_overlay_ttttNLO = TFile::Open("Craneens_Comb/NP_overlay_ttttNLO_Run2_TopTree_Study.root", "UPDATE");
	TFile *TTDileptPowheg = TFile::Open("Craneens_Comb/TTDileptPowheg_Run2_TopTree_Study.root", "UPDATE");

	vector < TFile* > files;

	files.push_back(Data);
	files.push_back(TTDileptMG);
	files.push_back(TTDileptMG_JERUp);
	files.push_back(TTDileptMG_JERDown);
	files.push_back(TTDileptMG_JESUp);
	files.push_back(TTDileptMG_JESDown);
	files.push_back(T_tW);
	files.push_back(Tbar_tW);
	files.push_back(DYJets);
	files.push_back(NP_overlay_ttttNLO);
	files.push_back(TTDileptPowheg);

	for(int i = 0; i < files.size(); i++){
		files[i]->cd();
                cout << i << " : 1 " << files[i]->GetPath() << endl;
		TTree* MuEl = (TTree*)files[i]->Get("Craneen__MuEl")->Clone();
                cout << i << " : 1.1" << endl;
		TTree* MuMu = (TTree*)files[i]->Get("Craneen__MuMu")->Clone();
                cout << i << " : 1.2" << endl;
		TTree* ElEl = (TTree*)files[i]->Get("Craneen__ElEl")->Clone();
                cout << i << " : 2" << endl;
		TList *list = new TList;
		list->Add(MuEl);
		list->Add(MuMu);
		list->Add(ElEl);
                cout << i << " : 3" << endl;
		TTree *Comb = TTree::MergeTrees(list);
		Comb->SetName("Craneen__Combined");
		Comb->Write();
                cout << i << " : 4" << endl;
		delete MuEl;
		delete MuMu;
		delete ElEl;
		delete Comb;
		files[i]->Close();
                cout << i << " : 5" << endl;		
	}

}
