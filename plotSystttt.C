{
	gStyle->SetOptStat(0);
	TFile *_file0 = TFile::Open("shapefileMu_DATA_BDT_Mu3rdJune_adaBoost_alphaSTune_subAll_noMinEvents_inc.root");
	TH1F* nominal = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__nominal");
	nominal->Draw();
	cout<<"PU"<<endl;
	TH1F* PUUp = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__PUUp");
	TH1F* PUDown = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__PUDown");
	TH1F* PUUpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__PUUp");
	TH1F* PUDownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__PUDown");
	cout<<"btag_light"<<endl;

	TH1F* btag_lightDown = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btag_lightDown");
	TH1F* btag_lightUp = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btag_lightUp");
	TH1F* btag_lightDownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btag_lightDown");
	TH1F* btag_lightUpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btag_lightUp");

	cout<<"btag_heavy"<<endl; 

	TH1F* btag_heavyDown = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btag_heavyDown");
	TH1F* btag_heavyUp = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btag_heavyUp");
	TH1F* btag_heavyDownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btag_heavyDown");
	TH1F* btag_heavyUpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btag_heavyUp");
	cout<<"scale"<<endl;

	TH1F* MEScaleDown = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__ttttMEScaleDown");
	TH1F* MEScaleUp = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__ttttMEScaleUp");
	TH1F* MEScaleDownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__ttttMEScaleDown");
	TH1F* MEScaleUpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__ttttMEScaleUp");
	cout<<"JES"<<endl;

	TH1F* JESUp = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__JESUp");
	TH1F* JESDown = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__JESDown");
	TH1F* JESUpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__JESUp");
	TH1F* JESDownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__JESDown");
	cout<<"JER"<<endl;

	TH1F* JERUp = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__JERUp");
	TH1F* JERDown = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__JERDown");
	TH1F* JERUpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__JERUp");
	TH1F* JERDownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__JERDown"); 				


	TH1F* btag_CSVLFDown = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFDown");
	TH1F* btag_CSVLFUp = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFUp");
	TH1F* btag_CSVLFDownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFDown");
	TH1F* btag_CSVLFUpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFUp");

	TH1F* btag_CSVHFDown = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFDown");
	TH1F* btag_CSVHFUp = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFUp");
	TH1F* btag_CSVHFDownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFDown");
	TH1F* btag_CSVHFUpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFUp");

	TH1F* btag_CSVHFStats1Down = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFStats1Down");
	TH1F* btag_CSVHFStats1Up = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFStats1Up");
	TH1F* btag_CSVHFStats1DownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFStats1Down");
	TH1F* btag_CSVHFStats1UpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFStats1Up");

	TH1F* btag_CSVHFStats2Down = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFStats2Down");
	TH1F* btag_CSVHFStats2Up = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFStats2Up");
	TH1F* btag_CSVHFStats2DownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFStats2Down");
	TH1F* btag_CSVHFStats2UpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVHFStats2Up");

	TH1F* btag_CSVLFStats1Down = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFStats1Down");
	TH1F* btag_CSVLFStats1Up = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFStats1Up");
	TH1F* btag_CSVLFStats1DownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFStats1Down");
	TH1F* btag_CSVLFStats1UpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFStats1Up");

	TH1F* btag_CSVLFStats2Down = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFStats2Down");
	TH1F* btag_CSVLFStats2Up = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFStats2Up");
	TH1F* btag_CSVLFStats2DownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFStats2Down");
	TH1F* btag_CSVLFStats2UpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVLFStats2Up");

	TH1F* btag_CSVCFErr1Down = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVCFErr1Down");
	TH1F* btag_CSVCFErr1Up = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVCFErr1Up");
	TH1F* btag_CSVCFErr1DownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVCFErr1Down");
	TH1F* btag_CSVCFErr1UpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVCFErr1Up");

	TH1F* btag_CSVCFErr2Down = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVCFErr2Down");
	TH1F* btag_CSVCFErr2Up = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVCFErr2Up");
	TH1F* btag_CSVCFErr2DownOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVCFErr2Down");
	TH1F* btag_CSVCFErr2UpOrig = (TH1F*)_file0->Get("ttttmu__NP_overlay_ttttNLO__btagWeightCSVCFErr2Up");


	// gStyle->SetHistLineStyle(3);


	//btag_CSVLF

	TCanvas *cCSVLF = new TCanvas();
	TPad *pad1CSVLF = new TPad("pad1CSVLF","pad1CSVLF",0,0.3,1,1);
	cCSVLF->cd();
	pad1CSVLF->SetBottomMargin(0);
	pad1CSVLF->Draw();
	pad1CSVLF->cd();

	btag_CSVLFUp->SetLineColor(kRed);
	btag_CSVLFDown->SetLineColor(kCyan);
	btag_CSVLFUp->SetLineStyle(1);
	btag_CSVLFDown->SetLineStyle(1);
	btag_CSVLFUpOrig->SetLineStyle(1);
	btag_CSVLFDownOrig->SetLineStyle(1);

	btag_CSVLFUp->SetLineWidth(2);
	btag_CSVLFDown->SetLineWidth(2);
	btag_CSVLFUpOrig->SetLineWidth(2);
	btag_CSVLFDownOrig->SetLineWidth(2);

	btag_CSVLFUpOrig->SetLineColor(kRed);
	btag_CSVLFDownOrig->SetLineColor(kCyan);

	btag_CSVLFUpOrig->SetTitle("TTbar BDT disciminator");
	btag_CSVLFUpOrig->Draw("hist");
	btag_CSVLFDownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *legCSVLF = new TLegend(0.1,0.7,0.5,0.9);
	legCSVLF->AddEntry(btag_CSVLFUp,"btag_CSVLF Up","l");
	legCSVLF->AddEntry(nominal,"nominal ttbar","l");
	legCSVLF->AddEntry(btag_CSVLFDown,"btag_CSVLF Down","l");
	legCSVLF->Draw();
	cCSVLF->cd();

	TPad *pad2CSVLF = new TPad("pad2CSVLF","pad2CSVLF",0,0,1,0.3);

	pad2CSVLF->SetTopMargin(0);
	pad2CSVLF->SetGridy();
	pad2CSVLF->Draw();

	btag_CSVLFUp->SetTitle("");

	pad2CSVLF->cd();
	// btag_CSVLFUp->Sumw2();
	btag_CSVLFUp->SetStats(0);
	btag_CSVLFUp->SetMinimum(0.5);
	btag_CSVLFUp->SetMaximum(1.5);
	btag_CSVLFUp->Divide(nominal);
	btag_CSVLFUp->SetMarkerStyle(1);
	btag_CSVLFUp->Draw("ep");
	// btag_CSVLFDown>Sumw2();
	btag_CSVLFDown->SetStats(0);
	btag_CSVLFDown->Divide(nominal);
	btag_CSVLFDown->SetMarkerStyle(1);
	btag_CSVLFDown->Draw("epsame");
	cCSVLF->cd();
	cCSVLF->SaveAs("btag_CSVLFsystttt.png");

	cout<<"btag_CSVLF done"<<endl;

	//btag_CSVHF

	TCanvas *cCSVHF = new TCanvas();
	TPad *pad1CSVHF = new TPad("pad1CSVHF","pad1CSVHF",0,0.3,1,1);
	cCSVHF->cd();
	pad1CSVHF->SetBottomMargin(0);
	pad1CSVHF->Draw();
	pad1CSVHF->cd();

	btag_CSVHFUp->SetLineColor(kRed);
	btag_CSVHFDown->SetLineColor(kCyan);
	btag_CSVHFUp->SetLineStyle(1);
	btag_CSVHFDown->SetLineStyle(1);
	btag_CSVHFUpOrig->SetLineStyle(1);
	btag_CSVHFDownOrig->SetLineStyle(1);

	btag_CSVHFUp->SetLineWidth(2);
	btag_CSVHFDown->SetLineWidth(2);
	btag_CSVHFUpOrig->SetLineWidth(2);
	btag_CSVHFDownOrig->SetLineWidth(2);

	btag_CSVHFUpOrig->SetLineColor(kRed);
	btag_CSVHFDownOrig->SetLineColor(kCyan);

	btag_CSVHFUpOrig->SetTitle("TTbar BDT disciminator");
	btag_CSVHFUpOrig->Draw("hist");
	btag_CSVHFDownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *legCSVHF = new TLegend(0.1,0.7,0.5,0.9);
	legCSVHF->AddEntry(btag_CSVHFUp,"btag_CSVHF Up","l");
	legCSVHF->AddEntry(nominal,"nominal ttbar","l");
	legCSVHF->AddEntry(btag_CSVHFDown,"btag_CSVHF Down","l");
	legCSVHF->Draw();
	cCSVHF->cd();

	TPad *pad2CSVHF = new TPad("pad2CSVHF","pad2CSVHF",0,0,1,0.3);

	pad2CSVHF->SetTopMargin(0);
	pad2CSVHF->SetGridy();
	pad2CSVHF->Draw();

	btag_CSVHFUp->SetTitle("");

	pad2CSVHF->cd();
	// btag_CSVHFUp->Sumw2();
	btag_CSVHFUp->SetStats(0);
	btag_CSVHFUp->SetMinimum(0.5);
	btag_CSVHFUp->SetMaximum(1.5);
	btag_CSVHFUp->Divide(nominal);
	btag_CSVHFUp->SetMarkerStyle(1);
	btag_CSVHFUp->Draw("ep");
	// btag_CSVHFDown>Sumw2();
	btag_CSVHFDown->SetStats(0);
	btag_CSVHFDown->Divide(nominal);
	btag_CSVHFDown->SetMarkerStyle(1);
	btag_CSVHFDown->Draw("epsame");
	cCSVHF->cd();
	cCSVHF->SaveAs("btag_CSVHFsystttt.png");

	cout<<"btag_CSVHF done"<<endl;


	//btag_CSVHFStats1

	TCanvas *cCSVHFStats1 = new TCanvas();
	TPad *pad1CSVHFStats1 = new TPad("pad1CSVHFStats1","pad1CSVHFStats1",0,0.3,1,1);
	cCSVHFStats1->cd();
	pad1CSVHFStats1->SetBottomMargin(0);
	pad1CSVHFStats1->Draw();
	pad1CSVHFStats1->cd();

	btag_CSVHFStats1Up->SetLineColor(kRed);
	btag_CSVHFStats1Down->SetLineColor(kCyan);
	btag_CSVHFStats1Up->SetLineStyle(1);
	btag_CSVHFStats1Down->SetLineStyle(1);
	btag_CSVHFStats1UpOrig->SetLineStyle(1);
	btag_CSVHFStats1DownOrig->SetLineStyle(1);

	btag_CSVHFStats1Up->SetLineWidth(2);
	btag_CSVHFStats1Down->SetLineWidth(2);
	btag_CSVHFStats1UpOrig->SetLineWidth(2);
	btag_CSVHFStats1DownOrig->SetLineWidth(2);

	btag_CSVHFStats1UpOrig->SetLineColor(kRed);
	btag_CSVHFStats1DownOrig->SetLineColor(kCyan);

	btag_CSVHFStats1UpOrig->SetTitle("TTbar BDT disciminator");
	btag_CSVHFStats1UpOrig->Draw("hist");
	btag_CSVHFStats1DownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *legCSVHFStats1 = new TLegend(0.1,0.7,0.5,0.9);
	legCSVHFStats1->AddEntry(btag_CSVHFStats1Up,"btag_CSVHFStats1 Up","l");
	legCSVHFStats1->AddEntry(nominal,"nominal ttbar","l");
	legCSVHFStats1->AddEntry(btag_CSVHFStats1Down,"btag_CSVHFStats1 Down","l");
	legCSVHFStats1->Draw();
	cCSVHFStats1->cd();

	TPad *pad2CSVHFStats1 = new TPad("pad2CSVHFStats1","pad2CSVHFStats1",0,0,1,0.3);

	pad2CSVHFStats1->SetTopMargin(0);
	pad2CSVHFStats1->SetGridy();
	pad2CSVHFStats1->Draw();

	btag_CSVHFStats1Up->SetTitle("");

	pad2CSVHFStats1->cd();
	// btag_CSVHFStats1Up->Sumw2();
	btag_CSVHFStats1Up->SetStats(0);
	btag_CSVHFStats1Up->SetMinimum(0.5);
	btag_CSVHFStats1Up->SetMaximum(1.5);
	btag_CSVHFStats1Up->Divide(nominal);
	btag_CSVHFStats1Up->SetMarkerStyle(1);
	btag_CSVHFStats1Up->Draw("ep");
	// btag_CSVHFStats1Down>Sumw2();
	btag_CSVHFStats1Down->SetStats(0);
	btag_CSVHFStats1Down->Divide(nominal);
	btag_CSVHFStats1Down->SetMarkerStyle(1);
	btag_CSVHFStats1Down->Draw("epsame");
	cCSVHFStats1->cd();
	cCSVHFStats1->SaveAs("btag_CSVHFStats1systttt.png");

	cout<<"btag_CSVHFStats1 done"<<endl;

	//btag_CSVHFStats2
	TCanvas *cCSVHFStats2 = new TCanvas();
	TPad *pad1CSVHFStats2 = new TPad("pad1CSVHFStats2","pad1CSVHFStats2",0,0.3,1,1);
	cCSVHFStats2->cd();
	pad1CSVHFStats2->SetBottomMargin(0);
	pad1CSVHFStats2->Draw();
	pad1CSVHFStats2->cd();

	btag_CSVHFStats2Up->SetLineColor(kRed);
	btag_CSVHFStats2Down->SetLineColor(kCyan);
	btag_CSVHFStats2Up->SetLineStyle(1);
	btag_CSVHFStats2Down->SetLineStyle(1);
	btag_CSVHFStats2UpOrig->SetLineStyle(1);
	btag_CSVHFStats2DownOrig->SetLineStyle(1);

	btag_CSVHFStats2Up->SetLineWidth(2);
	btag_CSVHFStats2Down->SetLineWidth(2);
	btag_CSVHFStats2UpOrig->SetLineWidth(2);
	btag_CSVHFStats2DownOrig->SetLineWidth(2);

	btag_CSVHFStats2UpOrig->SetLineColor(kRed);
	btag_CSVHFStats2DownOrig->SetLineColor(kCyan);

	btag_CSVHFStats2UpOrig->SetTitle("TTbar BDT disciminator");
	btag_CSVHFStats2UpOrig->Draw("hist");
	btag_CSVHFStats2DownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *legCSVHFStats2 = new TLegend(0.1,0.7,0.5,0.9);
	legCSVHFStats2->AddEntry(btag_CSVHFStats2Up,"btag_CSVHFStats2 Up","l");
	legCSVHFStats2->AddEntry(nominal,"nominal ttbar","l");
	legCSVHFStats2->AddEntry(btag_CSVHFStats2Down,"btag_CSVHFStats2 Down","l");
	legCSVHFStats2->Draw();
	cCSVHFStats2->cd();

	TPad *pad2CSVHFStats2 = new TPad("pad2CSVHFStats2","pad2CSVHFStats2",0,0,1,0.3);

	pad2CSVHFStats2->SetTopMargin(0);
	pad2CSVHFStats2->SetGridy();
	pad2CSVHFStats2->Draw();

	btag_CSVHFStats2Up->SetTitle("");

	pad2CSVHFStats2->cd();
	// btag_CSVHFStats2Up->Sumw2();
	btag_CSVHFStats2Up->SetStats(0);
	btag_CSVHFStats2Up->SetMinimum(0.5);
	btag_CSVHFStats2Up->SetMaximum(1.5);
	btag_CSVHFStats2Up->Divide(nominal);
	btag_CSVHFStats2Up->SetMarkerStyle(1);
	btag_CSVHFStats2Up->Draw("ep");
	// btag_CSVHFStats2Down>Sumw2();
	btag_CSVHFStats2Down->SetStats(0);
	btag_CSVHFStats2Down->Divide(nominal);
	btag_CSVHFStats2Down->SetMarkerStyle(1);
	btag_CSVHFStats2Down->Draw("epsame");
	cCSVHFStats2->cd();
	cCSVHFStats2->SaveAs("btag_CSVHFStats2systttt.png");

	cout<<"btag_CSVHFStats2 done"<<endl;

	//btag_CSVHFStats1

	TCanvas *cCSVLFStats1 = new TCanvas();
	TPad *pad1CSVLFStats1 = new TPad("pad1CSVLFStats1","pad1CSVLFStats1",0,0.3,1,1);
	cCSVLFStats1->cd();
	pad1CSVLFStats1->SetBottomMargin(0);
	pad1CSVLFStats1->Draw();
	pad1CSVLFStats1->cd();

	btag_CSVLFStats1Up->SetLineColor(kRed);
	btag_CSVLFStats1Down->SetLineColor(kCyan);
	btag_CSVLFStats1Up->SetLineStyle(1);
	btag_CSVLFStats1Down->SetLineStyle(1);
	btag_CSVLFStats1UpOrig->SetLineStyle(1);
	btag_CSVLFStats1DownOrig->SetLineStyle(1);

	btag_CSVLFStats1Up->SetLineWidth(2);
	btag_CSVLFStats1Down->SetLineWidth(2);
	btag_CSVLFStats1UpOrig->SetLineWidth(2);
	btag_CSVLFStats1DownOrig->SetLineWidth(2);

	btag_CSVLFStats1UpOrig->SetLineColor(kRed);
	btag_CSVLFStats1DownOrig->SetLineColor(kCyan);

	btag_CSVLFStats1UpOrig->SetTitle("TTbar BDT disciminator");
	btag_CSVLFStats1UpOrig->Draw("hist");
	btag_CSVLFStats1DownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *legCSVLFStats1 = new TLegend(0.1,0.7,0.5,0.9);
	legCSVLFStats1->AddEntry(btag_CSVLFStats1Up,"btag_CSVLFStats1 Up","l");
	legCSVLFStats1->AddEntry(nominal,"nominal ttbar","l");
	legCSVLFStats1->AddEntry(btag_CSVLFStats1Down,"btag_CSVLFStats1 Down","l");
	legCSVLFStats1->Draw();
	cCSVLFStats1->cd();

	TPad *pad2CSVLFStats1 = new TPad("pad2CSVLFStats1","pad2CSVLFStats1",0,0,1,0.3);

	pad2CSVLFStats1->SetTopMargin(0);
	pad2CSVLFStats1->SetGridy();
	pad2CSVLFStats1->Draw();

	btag_CSVLFStats1Up->SetTitle("");

	pad2CSVLFStats1->cd();
	// btag_CSVLFStats1Up->Sumw2();
	btag_CSVLFStats1Up->SetStats(0);
	btag_CSVLFStats1Up->SetMinimum(0.5);
	btag_CSVLFStats1Up->SetMaximum(1.5);
	btag_CSVLFStats1Up->Divide(nominal);
	btag_CSVLFStats1Up->SetMarkerStyle(1);
	btag_CSVLFStats1Up->Draw("ep");
	// btag_CSVLFStats1Down>Sumw2();
	btag_CSVLFStats1Down->SetStats(0);
	btag_CSVLFStats1Down->Divide(nominal);
	btag_CSVLFStats1Down->SetMarkerStyle(1);
	btag_CSVLFStats1Down->Draw("epsame");
	cCSVLFStats1->cd();
	cCSVLFStats1->SaveAs("btag_CSVLFStats1systttt.png");

	cout<<"btag_CSVLFStats1 done"<<endl;


	//btag_CSVLFStats2

	TCanvas *cCSVLFStats2 = new TCanvas();
	TPad *pad1CSVLFStats2 = new TPad("pad1CSVLFStats2","pad1CSVLFStats2",0,0.3,1,1);
	cCSVLFStats2->cd();
	pad1CSVLFStats2->SetBottomMargin(0);
	pad1CSVLFStats2->Draw();
	pad1CSVLFStats2->cd();

	btag_CSVLFStats2Up->SetLineColor(kRed);
	btag_CSVLFStats2Down->SetLineColor(kCyan);
	btag_CSVLFStats2Up->SetLineStyle(1);
	btag_CSVLFStats2Down->SetLineStyle(1);
	btag_CSVLFStats2UpOrig->SetLineStyle(1);
	btag_CSVLFStats2DownOrig->SetLineStyle(1);

	btag_CSVLFStats2Up->SetLineWidth(2);
	btag_CSVLFStats2Down->SetLineWidth(2);
	btag_CSVLFStats2UpOrig->SetLineWidth(2);
	btag_CSVLFStats2DownOrig->SetLineWidth(2);

	btag_CSVLFStats2UpOrig->SetLineColor(kRed);
	btag_CSVLFStats2DownOrig->SetLineColor(kCyan);

	btag_CSVLFStats2UpOrig->SetTitle("TTbar BDT disciminator");
	btag_CSVLFStats2UpOrig->Draw("hist");
	btag_CSVLFStats2DownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *legCSVLFStats2 = new TLegend(0.1,0.7,0.5,0.9);
	legCSVLFStats2->AddEntry(btag_CSVLFStats2Up,"btag_CSVLFStats2 Up","l");
	legCSVLFStats2->AddEntry(nominal,"nominal ttbar","l");
	legCSVLFStats2->AddEntry(btag_CSVLFStats2Down,"btag_CSVLFStats2 Down","l");
	legCSVLFStats2->Draw();
	cCSVLFStats2->cd();

	TPad *pad2CSVLFStats2 = new TPad("pad2CSVLFStats2","pad2CSVLFStats2",0,0,1,0.3);

	pad2CSVLFStats2->SetTopMargin(0);
	pad2CSVLFStats2->SetGridy();
	pad2CSVLFStats2->Draw();

	btag_CSVLFStats2Up->SetTitle("");

	pad2CSVLFStats2->cd();
	// btag_CSVLFStats2Up->Sumw2();
	btag_CSVLFStats2Up->SetStats(0);
	btag_CSVLFStats2Up->SetMinimum(0.5);
	btag_CSVLFStats2Up->SetMaximum(1.5);
	btag_CSVLFStats2Up->Divide(nominal);
	btag_CSVLFStats2Up->SetMarkerStyle(1);
	btag_CSVLFStats2Up->Draw("ep");
	// btag_CSVLFStats2Down>Sumw2();
	btag_CSVLFStats2Down->SetStats(0);
	btag_CSVLFStats2Down->Divide(nominal);
	btag_CSVLFStats2Down->SetMarkerStyle(1);
	btag_CSVLFStats2Down->Draw("epsame");
	cCSVLFStats2->cd();
	cCSVLFStats2->SaveAs("btag_CSVLFStats2systttt.png");

	cout<<"btag_CSVLFStats2 done"<<endl;

	//btag_CSVHFStats1

	TCanvas *cCSVCFErr1 = new TCanvas();
	TPad *pad1CSVCFErr1 = new TPad("pad1CSVCFErr1","pad1CSVCFErr1",0,0.3,1,1);
	cCSVCFErr1->cd();
	pad1CSVCFErr1->SetBottomMargin(0);
	pad1CSVCFErr1->Draw();
	pad1CSVCFErr1->cd();

	btag_CSVCFErr1Up->SetLineColor(kRed);
	btag_CSVCFErr1Down->SetLineColor(kCyan);
	btag_CSVCFErr1Up->SetLineStyle(1);
	btag_CSVCFErr1Down->SetLineStyle(1);
	btag_CSVCFErr1UpOrig->SetLineStyle(1);
	btag_CSVCFErr1DownOrig->SetLineStyle(1);

	btag_CSVCFErr1Up->SetLineWidth(2);
	btag_CSVCFErr1Down->SetLineWidth(2);
	btag_CSVCFErr1UpOrig->SetLineWidth(2);
	btag_CSVCFErr1DownOrig->SetLineWidth(2);

	btag_CSVCFErr1UpOrig->SetLineColor(kRed);
	btag_CSVCFErr1DownOrig->SetLineColor(kCyan);

	btag_CSVCFErr1UpOrig->SetTitle("TTbar BDT disciminator");
	btag_CSVCFErr1UpOrig->Draw("hist");
	btag_CSVCFErr1DownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *legCSVCFErr1 = new TLegend(0.1,0.7,0.5,0.9);
	legCSVCFErr1->AddEntry(btag_CSVCFErr1Up,"btag_CSVCFErr1 Up","l");
	legCSVCFErr1->AddEntry(nominal,"nominal ttbar","l");
	legCSVCFErr1->AddEntry(btag_CSVCFErr1Down,"btag_CSVCFErr1 Down","l");
	legCSVCFErr1->Draw();
	cCSVCFErr1->cd();

	TPad *pad2CSVCFErr1 = new TPad("pad2CSVCFErr1","pad2CSVCFErr1",0,0,1,0.3);

	pad2CSVCFErr1->SetTopMargin(0);
	pad2CSVCFErr1->SetGridy();
	pad2CSVCFErr1->Draw();

	btag_CSVCFErr1Up->SetTitle("");

	pad2CSVCFErr1->cd();
	// btag_CSVCFErr1Up->Sumw2();
	btag_CSVCFErr1Up->SetStats(0);
	btag_CSVCFErr1Up->SetMinimum(0.5);
	btag_CSVCFErr1Up->SetMaximum(1.5);
	btag_CSVCFErr1Up->Divide(nominal);
	btag_CSVCFErr1Up->SetMarkerStyle(1);
	btag_CSVCFErr1Up->Draw("ep");
	// btag_CSVCFErr1Down>Sumw2();
	btag_CSVCFErr1Down->SetStats(0);
	btag_CSVCFErr1Down->Divide(nominal);
	btag_CSVCFErr1Down->SetMarkerStyle(1);
	btag_CSVCFErr1Down->Draw("epsame");
	cCSVCFErr1->cd();
	cCSVCFErr1->SaveAs("btag_CSVCFErr1systttt.png");

	cout<<"btag_CSVCFErr1 done"<<endl;

	//btag_CSVCFErr2

	TCanvas *cCSVCFErr2 = new TCanvas();
	TPad *pad1CSVCFErr2 = new TPad("pad1CSVCFErr2","pad1CSVCFErr2",0,0.3,1,1);
	cCSVCFErr2->cd();
	pad1CSVCFErr2->SetBottomMargin(0);
	pad1CSVCFErr2->Draw();
	pad1CSVCFErr2->cd();

	btag_CSVCFErr2Up->SetLineColor(kRed);
	btag_CSVCFErr2Down->SetLineColor(kCyan);
	btag_CSVCFErr2Up->SetLineStyle(1);
	btag_CSVCFErr2Down->SetLineStyle(1);
	btag_CSVCFErr2UpOrig->SetLineStyle(1);
	btag_CSVCFErr2DownOrig->SetLineStyle(1);

	btag_CSVCFErr2Up->SetLineWidth(2);
	btag_CSVCFErr2Down->SetLineWidth(2);
	btag_CSVCFErr2UpOrig->SetLineWidth(2);
	btag_CSVCFErr2DownOrig->SetLineWidth(2);

	btag_CSVCFErr2UpOrig->SetLineColor(kRed);
	btag_CSVCFErr2DownOrig->SetLineColor(kCyan);

	btag_CSVCFErr2UpOrig->SetTitle("TTbar BDT disciminator");
	btag_CSVCFErr2UpOrig->Draw("hist");
	btag_CSVCFErr2DownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *legCSVCFErr2 = new TLegend(0.1,0.7,0.5,0.9);
	legCSVCFErr2->AddEntry(btag_CSVCFErr2Up,"btag_CSVCFErr2 Up","l");
	legCSVCFErr2->AddEntry(nominal,"nominal ttbar","l");
	legCSVCFErr2->AddEntry(btag_CSVCFErr2Down,"btag_CSVCFErr2 Down","l");
	legCSVCFErr2->Draw();
	cCSVCFErr2->cd();

	TPad *pad2CSVCFErr2 = new TPad("pad2CSVCFErr2","pad2CSVCFErr2",0,0,1,0.3);

	pad2CSVCFErr2->SetTopMargin(0);
	pad2CSVCFErr2->SetGridy();
	pad2CSVCFErr2->Draw();

	btag_CSVCFErr2Up->SetTitle("");

	pad2CSVCFErr2->cd();
	// btag_CSVCFErr2Up->Sumw2();
	btag_CSVCFErr2Up->SetStats(0);
	btag_CSVCFErr2Up->SetMinimum(0.5);
	btag_CSVCFErr2Up->SetMaximum(1.5);
	btag_CSVCFErr2Up->Divide(nominal);
	btag_CSVCFErr2Up->SetMarkerStyle(1);
	btag_CSVCFErr2Up->Draw("ep");
	// btag_CSVCFErr2Down>Sumw2();
	btag_CSVCFErr2Down->SetStats(0);
	btag_CSVCFErr2Down->Divide(nominal);
	btag_CSVCFErr2Down->SetMarkerStyle(1);
	btag_CSVCFErr2Down->Draw("epsame");
	cCSVCFErr2->cd();
	cCSVCFErr2->SaveAs("btag_CSVCFErr2systttt.png");

	cout<<"btag_CSVCFErr2 done"<<endl;
 	//btag_light

	TCanvas *c1 = new TCanvas();
	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
	c1->cd();
	pad1->SetBottomMargin(0);
	pad1->Draw();
	pad1->cd();

	btag_lightUp->SetLineColor(kRed);
	btag_lightDown->SetLineColor(kCyan);
	btag_lightUp->SetLineStyle(1);
	btag_lightDown->SetLineStyle(1);
	btag_lightUpOrig->SetLineStyle(1);
	btag_lightDownOrig->SetLineStyle(1);
	btag_lightUp->SetLineWidth(2);
	btag_lightDown->SetLineWidth(2);
	btag_lightUpOrig->SetLineWidth(2);
	btag_lightDownOrig->SetLineWidth(2);
	btag_lightUpOrig->SetLineColor(kRed);
	btag_lightDownOrig->SetLineColor(kCyan);
	btag_lightUpOrig->SetTitle("tttt BDT disciminator");
	btag_lightUpOrig->Draw("hist");
	btag_lightDownOrig->Draw("histsame");
	nominal->SetLineWidth(2);

	nominal->Draw("histsame");
	TLegend *leg = new TLegend(0.1,0.7,0.5,0.9);
	leg->AddEntry(btag_lightUp,"btag_light Up","l");
	leg->AddEntry(nominal,"nominal tttt","l");
	leg->AddEntry(btag_lightDown,"btag_light Down","l");
	leg->Draw();
	c1->cd();
	TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);

	pad2->SetTopMargin(0);
	pad2->SetGridy();
	pad2->Draw();

	btag_lightUp->SetTitle("");
	pad2->cd();
	// btag_lightUp->Sumw2();
	btag_lightUp->SetStats(0);	
	btag_lightUp->SetMaximum(1.5);	
	btag_lightUp->SetMinimum(0.5);
	btag_lightUp->Divide(nominal);
	btag_lightUp->SetMarkerStyle(1);
	btag_lightUp->Draw("ep");
	// btag_lightDown>Sumw2();
	btag_lightDown->SetStats(0);
	btag_lightDown->Divide(nominal);
	btag_lightDown->SetMarkerStyle(1);
	btag_lightDown->Draw("epsame");
	c1->cd();
	c1->SaveAs("btag_lightsystttt.png");

	cout<<"btag_light done"<<endl;


 	//btag_heavy

	TCanvas *c1h = new TCanvas();
	TPad *pad1h = new TPad("pad1h","pad1h",0,0.3,1,1);
	c1h->cd();
	pad1h->SetBottomMargin(0);
	pad1h->Draw();
	pad1h->cd();

	btag_heavyUp->SetLineColor(kRed);
	btag_heavyDown->SetLineColor(kCyan);
	btag_heavyUp->SetLineStyle(1);
	btag_heavyDown->SetLineStyle(1);
	btag_heavyUpOrig->SetLineStyle(1);
	btag_heavyDownOrig->SetLineStyle(1);
	btag_heavyUp->SetLineWidth(2);
	btag_heavyDown->SetLineWidth(2);
	btag_heavyUpOrig->SetLineWidth(2);
	btag_heavyDownOrig->SetLineWidth(2);

	btag_heavyUpOrig->SetLineColor(kRed);
	btag_heavyDownOrig->SetLineColor(kCyan);
	btag_heavyUpOrig->SetTitle("tttt BDT disciminator");
	btag_heavyUpOrig->Draw("hist");
	btag_heavyDownOrig->Draw("histsame");
	nominal->Draw("histsame");
	TLegend *legh = new TLegend(0.1,0.7,0.5,0.9);
	legh->AddEntry(btag_heavyUp,"btag_heavy Up","l");
	legh->AddEntry(nominal,"nominal tttt","l");
	legh->AddEntry(btag_heavyDown,"btag_heavy Down","l");
	legh->Draw();
	c1h->cd();
	TPad *pad2h = new TPad("pad2h","pad2h",0,0,1,0.3);

	pad2h->SetTopMargin(0);
	pad2h->SetGridy();
	pad2h->Draw();

	btag_heavyUp->SetTitle("");
	pad2h->cd();
	// btag_heavyUp->Sumw2();
	btag_heavyUp->SetStats(0);
	btag_heavyUp->SetMaximum(1.5);
	btag_heavyUp->SetMinimum(0.5);


	btag_heavyUp->Divide(nominal);
	btag_heavyUp->SetMarkerStyle(1);
	btag_heavyUp->Draw("ep");
	// btag_heavyDown>Sumw2();
	btag_heavyDown->SetStats(0);
	btag_heavyDown->Divide(nominal);
	btag_heavyDown->SetMarkerStyle(1);
	btag_heavyDown->Draw("epsame");
	c1h->cd();
	c1h->SaveAs("btag_heavysystttt.png");

	cout<<"btag_heavy done"<<endl;
 	// PU
	TCanvas *cPU1 = new TCanvas();
	TPad *padPU1 = new TPad("pad1","pad1",0,0.3,1,1);
	cPU1->cd();
	padPU1->SetBottomMargin(0);
	padPU1->Draw();
	padPU1->cd();

	PUUp->SetLineColor(kRed);
	PUDown->SetLineColor(kCyan);

	PUUpOrig->SetLineColor(kRed);
	PUDownOrig->SetLineColor(kCyan);
	PUUp->SetLineStyle(1);
	PUDown->SetLineStyle(1);
	PUUpOrig->SetLineStyle(1);
	PUDownOrig->SetLineStyle(1);
	PUUp->SetLineWidth(2);
	PUDown->SetLineWidth(2);
	PUUpOrig->SetLineWidth(2);
	PUDownOrig->SetLineWidth(2);

	PUDownOrig->SetTitle("tttt BDT disciminator");
	PUDownOrig->Draw("hist");
	PUUpOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *legPU = new TLegend(0.1,0.7,0.5,0.9);
	legPU->AddEntry(PUUp,"PU Up","l");
	legPU->AddEntry(nominal,"nominal tttt","l");
	legPU->AddEntry(PUDown,"PU Down","l");

	legPU->Draw();
	cPU1->cd();

	TPad *padPU2 = new TPad("pad2","pad2",0,0,1,0.3);

	padPU2->SetTopMargin(0);
	padPU2->SetGridy();
	padPU2->Draw();

	PUUp->SetTitle("");

	padPU2->cd();
	// PUUp->Sumw2();
	PUUp->SetStats(0);
	PUUp->SetMaximum(1.5);
	PUUp->SetMinimum(0.5);
	PUUp->Divide(nominal);
	PUUp->SetMarkerStyle(1);
	PUUp->Draw("ep");
	// PUDown->Sumw2();
	PUDown->SetStats(0);
	PUDown->Divide(nominal);
	PUDown->SetMarkerStyle(1);
	PUDown->Draw("epsame");
	cPU1->cd();
	cPU1->SaveAs("PUsystttt.png");

	cout<<"PU done"<<endl;


	//scale

	TCanvas *cMEScale1 = new TCanvas();
	TPad *padMEScale1 = new TPad("pad1","pad1",0,0.3,1,1);
	cMEScale1->cd();
	padMEScale1->SetBottomMargin(0);
	padMEScale1->Draw();
	padMEScale1->cd();

	MEScaleUp->SetLineColor(kRed);
	MEScaleDown->SetLineColor(kCyan);

	MEScaleUpOrig->SetLineColor(kRed);
	MEScaleDownOrig->SetLineColor(kCyan);

	MEScaleUp->SetLineStyle(1);
	MEScaleDown->SetLineStyle(1);
	MEScaleUpOrig->SetLineStyle(1);
	MEScaleDownOrig->SetLineStyle(1);
	MEScaleUp->SetLineWidth(2);
	MEScaleDown->SetLineWidth(2);
	MEScaleUpOrig->SetLineWidth(2);
	MEScaleDownOrig->SetLineWidth(2);

	MEScaleUpOrig->SetTitle("tttt BDT disciminator");
	MEScaleUpOrig->Draw("hist");
	MEScaleDownOrig->Draw("histsame");
	nominal->Draw("histsame");


	TLegend *legMEScale = new TLegend(0.1,0.7,0.5,0.9);
	legMEScale->AddEntry(MEScaleUp,"MEScaleME Up","l");
	legMEScale->AddEntry(nominal,"nominal tttt","l");
	legMEScale->AddEntry(MEScaleDown,"MEScaleME Down","l");
	legMEScale->Draw();
	cMEScale1->cd();

	TPad *pad2MEScale = new TPad("pad2","pad2",0,0,1,0.3);

	pad2MEScale->SetTopMargin(0);
	pad2MEScale->SetGridy();
	pad2MEScale->Draw();

	MEScaleUp->SetTitle("");

	pad2MEScale->cd();

	// MEScaleUp->Sumw2();
	MEScaleUp->SetStats(0);
	MEScaleUp->SetMaximum(1.5);
	MEScaleUp->SetMinimum(0.5);

	MEScaleUp->Divide(nominal);
	MEScaleUp->SetMarkerStyle(1);

	MEScaleUp->Draw("ep");
	MEScaleUp->SetAxisRange(0,2,"Y");
	MEScaleUp->Draw("ep");

	// MEScaleDown>Sumw2();
	MEScaleDown->SetStats(0);
	MEScaleDown->Divide(nominal);
	MEScaleDown->SetMarkerStyle(1);
	MEScaleDown->Draw("epsame");
	cMEScale1->cd();
	cMEScale1->SaveAs("MEScalesystttt.png");
	cout<<"MEScale done"<<endl;



	//JES
	TCanvas *cJES1 = new TCanvas();
	TPad *padJES1 = new TPad("pad1","pad1",0,0.3,1,1);
	cJES1->cd();
	padJES1->SetBottomMargin(0);
	padJES1->Draw();
	padJES1->cd();

	JESUp->SetLineColor(kRed);
	JESDown->SetLineColor(kCyan);

	JESUpOrig->SetLineColor(kRed);
	JESDownOrig->SetLineColor(kCyan);
	JESUp->SetLineStyle(1);
	JESDown->SetLineStyle(1);
	JESUpOrig->SetLineStyle(1);
	JESDownOrig->SetLineStyle(1);
	JESUp->SetLineWidth(2);
	JESDown->SetLineWidth(2);
	JESUpOrig->SetLineWidth(2);
	JESDownOrig->SetLineWidth(2);

	JESUpOrig->SetTitle("tttt BDT disciminator");
	JESUpOrig->Draw("hist");
	JESDownOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *legJES = new TLegend(0.1,0.7,0.5,0.9);
	legJES->AddEntry(JESUp,"JES Up","l");
	legJES->AddEntry(nominal,"nominal tttt","l");
	legJES->AddEntry(JESDown,"JES Down","l");

	legJES->Draw();
	cJES1->cd();

	TPad *padJES2 = new TPad("pad2","pad2",0,0,1,0.3);

	padJES2->SetTopMargin(0);
	padJES2->SetGridy();
	padJES2->Draw();

	JESUp->SetTitle("");

	padJES2->cd();
	// JESUp->Sumw2();
	JESUp->SetStats(0);
	JESUp->SetMaximum(1.5);
	JESUp->SetMinimum(0.5);
	JESUp->Divide(nominal);
	JESUp->SetMarkerStyle(1);
	JESUp->Draw("ep");
	// JESDown>Sumw2();
	JESDown->SetStats(0);
	JESDown->Divide(nominal);
	JESDown->SetMarkerStyle(1);
	JESDown->Draw("epsame");
	cJES1->cd();
	cJES1->SaveAs("JESsystttt.png");

	cout<<"JES done"<<endl;

	//JER
	TCanvas *cJER1 = new TCanvas();
	TPad *padJER1 = new TPad("pad1","pad1",0,0.3,1,1);
	cJER1->cd();
	padJER1->SetBottomMargin(0);
	padJER1->Draw();
	padJER1->cd();

	JERUp->SetLineColor(kRed);
	JERDown->SetLineColor(kCyan);

	JERUpOrig->SetLineColor(kRed);
	JERDownOrig->SetLineColor(kCyan);

	JERUp->SetLineStyle(1);
	JERDown->SetLineStyle(1);
	JERUpOrig->SetLineStyle(1);
	JERDownOrig->SetLineStyle(1);
	JERUp->SetLineWidth(2);
	JERDown->SetLineWidth(2);
	JERUpOrig->SetLineWidth(2);
	JERDownOrig->SetLineWidth(2);
	JERDownOrig->SetTitle("tttt BDT disciminator");
	JERDownOrig->Draw("hist");
	JERUpOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *legJER = new TLegend(0.1,0.7,0.5,0.9);
	legJER->AddEntry(JERUp,"JER Up","l");
	legJER->AddEntry(nominal,"nominal tttt","l");
	legJER->AddEntry(JERDown,"JER Down","l");

	legJER->Draw();
	cJER1->cd();

	TPad *padJER2 = new TPad("pad2","pad2",0,0,1,0.3);

	padJER2->SetTopMargin(0);
	padJER2->SetGridy();
	padJER2->Draw();

	JERUp->SetTitle("");

	padJER2->cd();
	// JERUp->Sumw2();
	JERUp->SetStats(0);
	JERUp->SetMaximum(1.5);
	JERUp->SetMinimum(0.5);
	JERUp->Divide(nominal);
	JERUp->SetMarkerStyle(1);
	JERUp->Draw("ep");
	// JERDown>Sumw2();
	JERDown->SetStats(0);
	JERDown->Divide(nominal);
	JERDown->SetMarkerStyle(1);
	JERDown->Draw("epsame");
	cJER1->cd();
	cJER1->SaveAs("JERsystttt.png");	
	cout<<"JER done"<<endl;


	

}
