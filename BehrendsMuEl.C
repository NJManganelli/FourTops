#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"

void BehrendsMuEl()
{
	gROOT->SetBatch(kTRUE);
	gStyle->SetOptStat(0);
	TCanvas *c1 = new TCanvas("c1","transparent pad",200,15,700,500);
	TPad *pad1 = new TPad("pad1","",0,0,1,1);
   	TPad *pad2 = new TPad("pad2","",0,0,1,1);
   	pad2->SetFillStyle(4000); //will be transparent
	TFile *nJetsDataMuFile = TFile::Open("shapefileMu_DATA_nJets_inc.root", "READ");  //Getting Files
	TFile *nJetsDataElFile = TFile::Open("shapefileEl_DATA_nJets_inc.root", "READ");
//	TFile *HTDataFile = TFile::Open("shapefileCombined_DATA_HT_inc.root");
	TFile *nJetsMCMuFile = TFile::Open("ScaleFilesMu_light/ErrornJets.root", "READ");
	TFile *nJetsMCElFile = TFile::Open("ScaleFilesEl_light/ErrornJets.root", "READ");
//	TFile *HTMCFile = TFile::Open("ScaleFiles_MuEl_light/ErrorHT.root");

	cout << "Getting nJets Histos" << endl;
	TH1F* histnJetsData = (TH1F*)nJetsDataMuFile->Get("MultiSamplePlot_nJets/nJets_Data");  //Getting nJets Histograms
	// TH1F* histnJetsData = (TH1F*)nJetsDataElFile->Get("MultiSamplePlot_nJets/nJets_Data");  //Getting nJets Histograms

	histnJetsData->Add((TH1F*)nJetsDataElFile->Get("MultiSamplePlot_nJets/nJets_Data"));
	TH1F* nJetsNom = (TH1F*)nJetsMCMuFile->Get("MultiSamplePlot_nJets/Nominal");
	TH1F* nJetsUp = (TH1F*)nJetsMCMuFile->Get("MultiSamplePlot_nJets/Plus");
	TH1F* nJetsDown = (TH1F*)nJetsMCMuFile->Get("MultiSamplePlot_nJets/Minus");
	nJetsNom->Add((TH1F*)nJetsMCElFile->Get("MultiSamplePlot_nJets/Nominal"));
	nJetsUp->Add((TH1F*)nJetsMCElFile->Get("MultiSamplePlot_nJets/Plus"));
	nJetsDown->Add((TH1F*)nJetsMCElFile->Get("MultiSamplePlot_nJets/Minus"));
	// TH1F* nJetsNom = (TH1F*)nJetsMCElFile->Get("MultiSamplePlot_nJets/Nominal");
	// TH1F* nJetsUp = (TH1F*)nJetsMCElFile->Get("MultiSamplePlot_nJets/Plus");
	// TH1F* nJetsDown = (TH1F*)nJetsMCElFile->Get("MultiSamplePlot_nJets/Minus");	
	// TH1F* nJetsMC = (TH1F*)nJetsMu->Clone();
	// nJetsMC->Add(nJetsEl);

	cout << "Scaling" << endl;
	//Scaling for Behrend's Scaling
	// cout<<"njets bi content: "<<nJetsNom->GetBinContent(7)<<endl;
	float scalingMC = histnJetsData->GetBinContent(8)/nJetsNom->GetBinContent(8);
	// float scalingMC = 1.0/nJetsNom->GetBinContent(7);

	// float scalingData = 1.0/histnJetsData->GetBinContent(7);
	// float scalingMC = 1.0/nJetsNom->Integral();
	// float scalingData=1.0/histnJetsData->Integral();
	// float scalingMC = 1.0;
	float scalingData = 1.0;
	nJetsNom->Scale(scalingMC);
	nJetsUp->Scale(scalingMC);
	nJetsDown->Scale(scalingMC);

	int nfilledMC = 0, nfilledData = 0, longerHist = 0;
	cout << "Getting number of filled Bins" << endl;
	longerHist = (nJetsNom->GetNbinsX() > histnJetsData->GetNbinsX() ? histnJetsData->GetNbinsX() : nJetsNom->GetNbinsX());
	for(int i = 1; i<=longerHist; i++)
	{
	    if(nJetsNom->GetBinContent(i) > 0) nfilledMC++;
	    if(histnJetsData->GetBinContent(i) > 0) nfilledData++;
	}

	Float_t xMC[nfilledMC];
	Float_t yMC[nfilledMC];
	Float_t eylMC[nfilledMC];
	Float_t eyhMC[nfilledMC];
	Float_t xData[nfilledData];
	Float_t yData[nfilledData];
	Float_t eylData[nfilledData];
	Float_t eyhData[nfilledData];
	// Float_t xMCshift[nfilledMC];
	// Float_t yMCshift[nfilledMC];
	// Float_t eylMCshift[nfilledMC];
	// Float_t eyhMCshift[nfilledMC];
	// Float_t xDatashift[nfilledData];
	// Float_t yDatashift[nfilledData];
	// Float_t eylDatashift[nfilledData];
	// Float_t eyhDatashift[nfilledData];	
	int jetStart = 3;

	cout << "Setting up Arrays" <<jetStart+nfilledMC<< endl;
	for(int j = jetStart; j<(jetStart+nfilledMC); j++)
	{
	    xMC[j-jetStart] = (j-1);
	    yMC[j-jetStart] = nJetsNom->GetBinContent(j)/nJetsNom->GetBinContent(j-1);
	    eylMC[j-jetStart] = (yMC[j-jetStart]-nJetsDown->GetBinContent(j));
	    eyhMC[j-jetStart] = (nJetsUp->GetBinContent(j)-yMC[j-jetStart])/(nJetsUp->GetBinContent(j-1)-yMC[j-1-jetStart]);
	    cout << xMC[j-jetStart] << " " << yMC[j-jetStart] << " " << eylMC[j-jetStart] << " " << eyhMC[j-jetStart] << endl;

	    // k=j-1;
	    // xMCshift[j-jetStart] = (k-1);
	    // yMCshift[j-jetStart] = nJetsNom->GetBinContent(k);
	    // eylMCshift[j-jetStart] = (yMC[k-jetStart]-nJetsDown->GetBinContent(k));
	    // eyhMCshift[j-jetStart] = (nJetsUp->GetBinContent(k)-yMC[k-jetStart]);
	    // cout << xMC[k-jetStart] << " " << yMC[k-jetStart] << " " << eylMC[k-jetStart] << " " << eyhMC[k-jetStart] << endl;
	}

	for(int j = jetStart; j<(jetStart+nfilledData); j++)
	{
	    xData[j-jetStart] = (j-1);
	    yData[j-jetStart] = histnJetsData->GetBinContent(j)/histnJetsData->GetBinContent(j-1);
	    eylData[j-jetStart] = (sqrt(yData[j-jetStart]));
	    eyhData[j-jetStart] = (sqrt(yData[j-jetStart]))/(sqrt(yData[j-jetStart-1]));

	    yData[j-jetStart] *= scalingData;
	    eylData[j-jetStart] *= scalingData;
	    eyhData[j-jetStart] *= scalingData;

	    cout << xData[j-jetStart] << " " << yData[j-jetStart] << " " << eylData[j-jetStart] << " " << eyhData[j-jetStart] << endl;
	    // k=j-1;
	    // xData[j-jetStart] = (k-1);
	    // yData[j-jetStart] = histnJetsData->GetBinContent(k);
	    // eylData[j-jetStart] = (sqrt(yData[k-jetStart]));
	    // eyhData[j-jetStart] = (sqrt(yData[k-jetStart]));

	    // yData[j-jetStart] *= scalingData;
	    // eylData[j-jetStart] *= scalingData;
	    // eyhData[j-jetStart] *= scalingData;

	    // cout << xData[k-jetStart] << " " << yData[k-jetStart] << " " << eylData[k-jetStart] << " " << eyhData[k-jetStart] << endl;	    
	}

	TGraphAsymmErrors* nJetsMC = new TGraphAsymmErrors(nfilledMC, xMC, yMC, 0, 0, eylMC, eyhMC);
	TGraphAsymmErrors* nJetsData = new TGraphAsymmErrors(nfilledData, xData, yData, 0, 0, eylData, eyhData);
	cout << "TGraph made" << endl;

	nJetsMC->SetLineColor(kRed);
	nJetsData->SetLineColor(kBlue);
	nJetsMC->SetMarkerColor(kRed);
	nJetsData->SetMarkerColor(kBlue);
	nJetsMC->SetMarkerStyle(22);
	nJetsData->SetMarkerStyle(21);
	nJetsMC->SetMarkerSize(1);
	nJetsData->SetMarkerSize(1);
	cout<<"njetsdata : "<<yData[12]<<endl;
	TGraphAsymmErrors* nJetsData2 = (TGraphAsymmErrors*)nJetsData->Clone();
	TGraphAsymmErrors* nJetsMC2 = (TGraphAsymmErrors*)nJetsMC->Clone();
	TGraphAsymmErrors* nJetsData3 = (TGraphAsymmErrors*)nJetsData->Clone();
	TGraphAsymmErrors* nJetsMC3 = (TGraphAsymmErrors*)nJetsMC->Clone();

//	TCanvas *c1 = new TCanvas("c1");


   	Double_t ymin = 0;
   	Double_t ymax = 1;
   	Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
   	Double_t xmin =5;
   	Double_t xmax = 15;
   	Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right

	pad1->Draw();
	// pad1->Range(0,0.00001,15,1.7);

    pad1->Draw();
	pad1->cd();
	// pad1->SetLogy();


	gStyle->SetOptFit(111);

	//Fitting w/ first point

//	TF1 *DataFit = new TF1("DataFit","[0]*[1]**(x-2)",2,10);
//	DataFit->SetParameters(1.0, 0.1);
//	DataFit->SetLineColor(kBlue);
//	nJetsData->Fit(DataFit,"","",2,10);
	//Drawing first version of Data with first Fit
//	nJetsData->SetTitle("Behrends Scaling");
//	nJetsData->Draw("AP");
//	c1->Update();
//	TPaveStats *stD = (TPaveStats*)nJetsData->FindObject("stats");
//	cout << "Got Stats" << endl;
//	stD->SetX1NDC(0.15);
//	stD->SetY1NDC(0.6);
//	stD->SetX2NDC(0.35);
//	stD->SetY2NDC(0.7);
//	stD->Draw();
	

	//stD->SetX1(.15);
	//stD->SetX2(.5);
	cout << "Stat Box Width Set" << endl;

//	TF1 *MCFit = new TF1("MCFit","[0]*[1]**(x-2)",2,10);
//	MCFit->SetParameters(1.0, 0.1);
//	nJetsMC->Fit(MCFit,"","",2,10);
//	nJetsMC->Draw("SAME P");
//	c1->Update();
//	TPaveStats *stM = (TPaveStats*)nJetsMC->FindObject("stats");
//	stM->SetX1NDC(0.15);
//	stM->SetY1NDC(0.5);
//	stM->SetX2NDC(0.35);
//	stM->SetY2NDC(0.6);
//	stM->Draw();
	
	//Fitting w/out first point
	int startpoint = 7;
	// TF1 *MCFit2 = new TF1("MCFit2","6150*[0]**(x-6)",startpoint,12);

	// TF1 *MCFit2 = new TF1("MCFit2","[0]*[1]**(x-6)",7,11);
	TF1 *MCFit2 = new TF1("MCFit2","[0]+[1]*(x)",7,11);

	MCFit2->SetParameters(0.7,-0.3);
	// MCFit2->SetParameters(1.0,0.1);
	MCFit2->SetLineStyle(2);
	MCFit2->SetLineColor(kRed);
	// nJetsMC2->GetYaxis()->SetRangeUser(0.1,40000);
	nJetsMC2->GetYaxis()->SetRangeUser(0.17,1.4);
	// nJetsMC2->GetYaxis()->SetRangeUser(0.000001,1.3);
	nJetsMC2->GetXaxis()->SetRangeUser(1,15);
	nJetsMC2->Fit(MCFit2,"","",7,11);
	nJetsMC2->Draw("AP");
	c1->Update();
	TPaveStats *stM2 = (TPaveStats*)nJetsMC2->FindObject("stats");

	stM2->SetX1NDC(0.13);
	stM2->SetY1NDC(0.13);
	stM2->SetX2NDC(0.33);
	stM2->SetY2NDC(0.23);

	stM2->Draw();


	// TF1 *DataFit2 = new TF1("DataFit2","6150*[0]**(x-6)",startpoint,11);
	// DataFit2->SetParameter(0,0.1);

	// TF1 *DataFit2 = new TF1("DataFit2","[0]*[1]**(x-6)",startpoint,11);
	TF1 *DataFit2 = new TF1("DataFit2","[0]+[1]*(x)",startpoint,11);
	DataFit2->SetParameters(0.7, -0.3);
	// DataFit2->SetParameters(1.0, 0.1);
	DataFit2->SetLineColor(kBlue);
	DataFit2->SetLineStyle(2);
    // nJetsData2->Draw("AP");


	nJetsData2->Fit(DataFit2,"","",startpoint,11);
    nJetsData2->Draw("SAME P");
	//nJetsData2->Draw("AP");
	c1->Update();
	TPaveStats *stD2 = (TPaveStats*)nJetsData2->FindObject("stats");
	stD2->SetX1NDC(0.13);
	stD2->SetY1NDC(0.23);
	stD2->SetX2NDC(0.33);
	stD2->SetY2NDC(0.33);
	cout << "Got Stats" << endl;
	stD2->Draw();

	//Fitting Only later points


	// TF1 *MCFit3 = new TF1("MCFit3","0.11535*[0]**(x-5)",5,11);
	// MCFit3->SetParameters(1.0, 0.1);
	// MCFit3->SetLineColor(kRed);
	// MCFit3->SetLineStyle(2);
	// nJetsMC3->SetTitle("Behrends Scaling");
	// nJetsMC3->Fit(MCFit3,"","",5,11);
	// nJetsMC3->GetXaxis()->SetTitle("nJets");
	// nJetsMC3->GetYaxis()->SetTitle("Normalized Events");
	// nJetsMC3->Draw("AP");
	// c1->Update();
	// TPaveStats *stM3 = (TPaveStats*)nJetsMC3->FindObject("stats");
	// stM3->SetTextColor(kRed);
	// stM3->SetX1NDC(0.35);
	// stM3->SetY1NDC(0.17);
	// stM3->SetX2NDC(0.55);
	// stM3->SetY2NDC(0.27);
	// stM3->Draw();


	// TF1 *DataFit3 = new TF1("DataFit3","0.11535*[0]**(x-5)",5,11);
	// DataFit3->SetParameters(1.0, 0.1);
	// DataFit3->SetLineColor(kBlue);
	// DataFit3->SetLineStyle(2);
	// nJetsData3->Fit(DataFit3,"","",5,11);
	// nJetsData3->Draw("SAME P");
	// c1->Update();
	// TPaveStats *stD3 = (TPaveStats*)nJetsData3->FindObject("stats");
	// cout << "Got Stats" << endl;
	// stD3->SetTextColor(kBlue);
	// stD3->SetX1NDC(0.15);
	// stD3->SetY1NDC(0.17);
	// stD3->SetX2NDC(0.35);
	// stD3->SetY2NDC(0.27);
	// stD3->Draw();

   	cout<<"ymin: "<<ymin<<"  dy: "<<dy<<" tot: "<<ymin-0.1*dy<<endl;
   	pad2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
   	pad2->Draw();
   	pad2->cd();

	TF1 *SF = new TF1("Scale Factor","(0.2397**(x-5))/(0.2888**(x-5))",startpoint,15);
	TF1 *SFu = new TF1("Scale Factor Up","((0.2397**(x-5))/(0.2888**(x-5)))+((((0.2558**(x-5)-0.2397**(x-5))**2)+((0.2956**(x-5)-0.2888**(x-5))**2))**(1/2))+0.0131",startpoint,15);
	TF1 *SFd = new TF1("Scale Factor Down","((0.2397**(x-5))/(0.2888**(x-5)))-((((0.2397**(x-5)-0.2236**(x-5))**2)+((0.2888**(x-5)-0.2820**(x-5))**2))**(1/2))-0.0131",startpoint,15);

	// SFu->SetLineColor(kGreen);
	// SFu->SetLineStyle(2);
	// SFu->Draw("SAME");

	// SFd->SetLineColor(kGreen);
	// SFd->SetLineStyle(2);
	// SFd->Draw("SAME");

	// SF->SetLineColor(kGreen);
	// SF->SetLineStyle(1);
	// SF->Draw("SAME");

	c1->Update();

   	TGaxis *axis = new TGaxis(xmax,ymin,xmax,ymax,ymin,ymax,510,"+L");
   	axis->SetLabelColor(kBlack);
   	axis->Draw();
	// axis->SetTitle("Scale Factor");
//	DataFit->Draw("SAME");
	
//	DataFit2->Draw("SAME");
//	MCFit->Draw("SAME");
//	MCFit2->Draw("SAME");

	TLegend *leg = new TLegend(0.5,0.7,0.9,0.9);
	leg->AddEntry(nJetsData,"Data","p");
	leg->AddEntry(nJetsMC,"TTbar","p");
//	leg->AddEntry(DataFit,"Data Fit","l");
//	leg->AddEntry(MCFit,"TTbar Fit","l");
	leg->AddEntry(DataFit2,"Data Fit ","l");
	leg->AddEntry(MCFit2,"TTbar Fit ","l");
	// leg->AddEntry(DataFit3,"Data Fit","l");
	// leg->AddEntry(MCFit3,"TTbar Fit","l");
	// leg->AddEntry(SF,"Scale Factor","l");
	leg->Draw();

	gROOT->cd();

	c1->Print("Behrends511Log.eps");
	c1->Print("Behrends511Log.C");

}
