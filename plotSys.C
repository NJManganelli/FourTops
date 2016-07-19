{
	gStyle->SetOptStat(0);
	gStyle->SetLineStyleString(11,"400 200");

	TFile *_file0 = TFile::Open("shapefileMu_DATA_BDT_Mu3rdJune_adaBoost_alphaSTune_subAll_noMinEvents_inc.root");
	TH1F* nominal = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__nominal");
	TH1F* MLM = (TH1F*)_file0->Get("ttttmu__TTJets_MLM__nominal");
	TH1F* FXFX = (TH1F*)_file0->Get("ttttmu__TTJets_aMCatNLO__nominal");
	TH1F* MLMorig = (TH1F*)_file0->Get("ttttmu__TTJets_MLM__nominal");
	TH1F* FXFXorig = (TH1F*)_file0->Get("ttttmu__TTJets_aMCatNLO__nominal");

	TH1F* PUUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__PUUp");
	TH1F* PUDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__PUDown");
	TH1F* PUUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__PUUp");
	TH1F* PUDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__PUDown");

	TH1F* btag_lightDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btag_lightDown");
	TH1F* btag_lightUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btag_lightUp");
	TH1F* btag_lightDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btag_lightDown");
	TH1F* btag_lightUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btag_lightUp");

	TH1F* btag_heavyDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btag_heavyDown");
	TH1F* btag_heavyUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btag_heavyUp");
	TH1F* btag_heavyDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btag_heavyDown");
	TH1F* btag_heavyUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btag_heavyUp");

	TH1F* MEScaleDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ttMEScaleDown");
	TH1F* MEScaleUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ttMEScaleUp");
	TH1F* MEScaleDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ttMEScaleDown");
	TH1F* MEScaleUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ttMEScaleUp");

    TH1F* ScaleHDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ScaleHDown");
    TH1F* ScaleHUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ScaleHUp");
    TH1F* ScaleHDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ScaleHDown");
    TH1F* ScaleHUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ScaleHUp");

	TH1F* JESUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JESUp");
	TH1F* JESDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JESDown");
	TH1F* JESUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JESUp");
	TH1F* JESDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JESDown");

	TH1F* JERUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JERUp");
	TH1F* JERDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JERDown");
	TH1F* JERUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JERUp");
	TH1F* JERDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JERDown"); 				

	TH1F* ttGeneratorUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ttGeneratorUp");
	TH1F* ttGeneratorDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ttGeneratorDown");
	TH1F* ttGeneratorUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ttGeneratorUp");
	TH1F* ttGeneratorDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ttGeneratorDown"); 	
	
	// gStyle->SetHistLineStyle(3);

//ttbar alternatives

	TCanvas *c1ttbar = new TCanvas();
	TPad *ttbar_pad = new TPad("ttbar_pad","ttbar_pad",0,0.3,1,1);
	c1ttbar->cd();
	ttbar_pad->SetBottomMargin(0);
	ttbar_pad->Draw();
	ttbar_pad->cd();

	// nominal->Sumw2();
	// MLM->Sumw2();
	// FXFX->Sumw2();
	cout<<"powheg: "<<nominal->GetSumOfWeights()<<endl;
	cout<<"MLM: "<<MLM->GetSumOfWeights()<<endl;
	cout<<"aMCatNLOFXFX: "<<FXFX->GetSumOfWeights()<<endl;


	nominal->SetLineWidth(2);
	MLM->SetLineWidth(2);
	FXFX->SetLineWidth(2);

	MLM->SetLineColor(kRed);
	FXFX->SetLineColor(kGreen);	

	MLMorig->SetLineWidth(2);
	FXFXorig->SetLineWidth(2);

	MLMorig->SetLineColor(kRed);
	FXFXorig->SetLineColor(kGreen);


	MLM->SetTitle("TTbar BDT disciminator");
	MLMorig->Draw("hist");
	FXFXorig->Draw("histsame");

	nominal->SetLineWidth(2);

	nominal->Draw("histsame");

	TLegend *leg_ttbar = new TLegend(0.5,0.7,0.9,0.9);
	leg_ttbar->AddEntry(MLM,"MLM","l");
	leg_ttbar->AddEntry(nominal,"nominal ttbar","l");
	leg_ttbar->AddEntry(FXFX,"FXFX","l");
	leg_ttbar->Draw();
	c1ttbar->cd();

	TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);

	pad2->SetTopMargin(0);
	pad2->SetGridy();
	pad2->Draw();

	nominal->SetTitle("");

	pad2->cd();
	// btag_lightUp->Sumw2();
	MLM->SetStats(0);
	MLM->SetMinimum(0.6);
	MLM->SetMaximum(1.4);
	MLM->Divide(nominal);
	MLM->SetMarkerStyle(1);
	MLM->Draw("ep");
	// btag_lightDown>Sumw2();
	FXFX->SetStats(0);
	FXFX->Divide(nominal);
	FXFX->SetMarkerStyle(1);
	FXFX->Draw("epsame");
	c1ttbar->cd();
	c1ttbar->SaveAs("MLMFXFX.eps");

	cout<<"MLMFXFX"<<endl;


//  	//btag_light

// 	TCanvas *c1 = new TCanvas();
// 	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
// 	c1->cd();
// 	pad1->SetBottomMargin(0);
// 	pad1->Draw();
// 	pad1->cd();

// 	btag_lightUp->SetLineColor(kRed);
// 	btag_lightDown->SetLineColor(kCyan);
// 	btag_lightUp->SetLineStyle(1);
// 	btag_lightDown->SetLineStyle(1);
// 	btag_lightUpOrig->SetLineStyle(1);
// 	btag_lightDownOrig->SetLineStyle(1);

// 	btag_lightUp->SetLineWidth(2);
// 	btag_lightDown->SetLineWidth(2);
// 	btag_lightUpOrig->SetLineWidth(2);
// 	btag_lightDownOrig->SetLineWidth(2);

// 	btag_lightUpOrig->SetLineColor(kRed);
// 	btag_lightDownOrig->SetLineColor(kCyan);

// 	btag_lightUpOrig->SetTitle("TTbar BDT disciminator");
// 	btag_lightUpOrig->Draw("hist");
// 	btag_lightDownOrig->Draw("histsame");
// 	nominal->SetLineWidth(2);

// 	nominal->Draw("histsame");

// 	TLegend *leg = new TLegend(0.5,0.7,0.9,0.9);
// 	leg->AddEntry(btag_lightUp,"btag_light Up","l");
// 	leg->AddEntry(nominal,"nominal ttbar","l");
// 	leg->AddEntry(btag_lightDown,"btag_light Down","l");
// 	leg->Draw();
// 	c1->cd();

// 	TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);

// 	pad2->SetTopMargin(0);
// 	pad2->SetGridy();
// 	pad2->Draw();

// 	btag_lightUp->SetTitle("");

// 	pad2->cd();
// 	// btag_lightUp->Sumw2();
// 	btag_lightUp->SetStats(0);
// 	btag_lightUp->SetMinimum(0.5);
// 	btag_lightUp->SetMaximum(1.5);
// 	btag_lightUp->Divide(nominal);
// 	btag_lightUp->SetMarkerStyle(1);
// 	btag_lightUp->Draw("ep");
// 	// btag_lightDown>Sumw2();
// 	btag_lightDown->SetStats(0);
// 	btag_lightDown->Divide(nominal);
// 	btag_lightDown->SetMarkerStyle(1);
// 	btag_lightDown->Draw("epsame");
// 	c1->cd();
// 	c1->SaveAs("btag_lightsystt.eps");

// 	cout<<"btag_light done"<<endl;


//  	//btag_heavy

// 	TCanvas *c1h = new TCanvas();
// 	TPad *pad1h = new TPad("pad1h","pad1h",0,0.3,1,1);
// 	c1h->cd();
// 	pad1h->SetBottomMargin(0);
// 	pad1h->Draw();
// 	pad1h->cd();

// 	btag_heavyUp->SetLineColor(kRed);
// 	btag_heavyDown->SetLineColor(kCyan);
// 	btag_heavyUp->SetLineStyle(1);
// 	btag_heavyDown->SetLineStyle(1);
// 	btag_heavyUpOrig->SetLineStyle(1);
// 	btag_heavyDownOrig->SetLineStyle(1);
// 	btag_heavyUp->SetLineWidth(2);
// 	btag_heavyDown->SetLineWidth(2);
// 	btag_heavyUpOrig->SetLineWidth(2);
// 	btag_heavyDownOrig->SetLineWidth(2);
// 	btag_heavyUpOrig->SetLineColor(kRed);
// 	btag_heavyDownOrig->SetLineColor(kCyan);

// 	btag_heavyUpOrig->SetTitle("TTbar BDT disciminator");
// 	btag_heavyUpOrig->Draw("hist");
// 	btag_heavyDownOrig->Draw("histsame");
// 	nominal->Draw("histsame");

// 	TLegend *legh = new TLegend(0.5,0.7,0.9,0.9);
// 	legh->AddEntry(btag_heavyUp,"btag_heavy Up","l");
// 	legh->AddEntry(nominal,"nominal ttbar","l");
// 	legh->AddEntry(btag_heavyDown,"btag_heavy Down","l");
// 	legh->Draw();
// 	c1h->cd();

// 	TPad *pad2h = new TPad("pad2h","pad2h",0,0,1,0.3);

// 	pad2h->SetTopMargin(0);
// 	pad2h->SetGridy();
// 	pad2h->Draw();

// 	btag_heavyUp->SetTitle("");

// 	pad2h->cd();
// 	// btag_heavyUp->Sumw2();
// 	btag_heavyUp->SetStats(0);
// 	btag_heavyUp->SetMinimum(0.5);
// 	btag_heavyUp->SetMaximum(1.5);
// 	btag_heavyUp->Divide(nominal);
// 	btag_heavyUp->SetMarkerStyle(1);
// 	btag_heavyUp->Draw("ep");
// 	// btag_heavyDown>Sumw2();
// 	btag_heavyDown->SetStats(0);
// 	btag_heavyDown->Divide(nominal);
// 	btag_heavyDown->SetMarkerStyle(1);
// 	btag_heavyDown->Draw("epsame");
// 	c1h->cd();
// 	c1h->SaveAs("btag_heavysystt.eps");

// 	cout<<"btag_heavy done"<<endl;

//  	// PU
// 	TCanvas *cPU1 = new TCanvas();
// 	TPad *padPU1 = new TPad("pad1","pad1",0,0.3,1,1);
// 	cPU1->cd();
// 	padPU1->SetBottomMargin(0);
// 	padPU1->Draw();
// 	padPU1->cd();

// 	PUUp->SetLineColor(kRed);
// 	PUDown->SetLineColor(kCyan);

// 	PUUpOrig->SetLineColor(kRed);
// 	PUDownOrig->SetLineColor(kCyan);
// 	PUUp->SetLineStyle(1);
// 	PUDown->SetLineStyle(1);
// 	PUUpOrig->SetLineStyle(1);
// 	PUDownOrig->SetLineStyle(1);
// 	PUUp->SetLineWidth(2);
// 	PUDown->SetLineWidth(2);
// 	PUUpOrig->SetLineWidth(2);
// 	PUDownOrig->SetLineWidth(2);
// 	PUDownOrig->SetTitle("TTbar BDT disciminator");
// 	PUDownOrig->Draw("hist");
// 	PUUpOrig->Draw("histsame");
// 	nominal->Draw("histsame");

// 	TLegend *legPU = new TLegend(0.5,0.7,0.9,0.9);
// 	legPU->AddEntry(PUUp,"PU Up","l");
// 	legPU->AddEntry(nominal,"nominal ttbar","l");
// 	legPU->AddEntry(PUDown,"PU Down","l");

// 	legPU->Draw();
// 	cPU1->cd();

// 	TPad *padPU2 = new TPad("pad2","pad2",0,0,1,0.3);

// 	padPU2->SetTopMargin(0);
// 	padPU2->SetGridy();
// 	padPU2->Draw();

// 	PUUp->SetTitle("");

// 	padPU2->cd();
// 	// PUUp->Sumw2();
// 	PUUp->SetStats(0);
// 	PUUp->SetMinimum(0.5);
// 	PUUp->SetMaximum(1.5);
// 	PUUp->Divide(nominal);
// 	PUUp->SetMarkerStyle(1);
// 	PUUp->Draw("ep");
// 	// PUDown->Sumw2();
// 	PUDown->SetStats(0);
// 	PUDown->Divide(nominal);
// 	PUDown->SetMarkerStyle(1);
// 	PUDown->Draw("epsame");
// 	cPU1->cd();
// 	cPU1->SaveAs("PUsystt.eps");

// 	cout<<"PU done"<<endl;


// 	//MEScale

// 	TCanvas *cMEScale1 = new TCanvas();
// 	TPad *padMEScale1 = new TPad("pad1","pad1",0,0.3,1,1);
// 	cMEScale1->cd();
// 	padMEScale1->SetBottomMargin(0);
// 	padMEScale1->Draw();
// 	padMEScale1->cd();

// 	MEScaleUp->SetLineColor(kRed);
// 	MEScaleDown->SetLineColor(kCyan);

// 	MEScaleUpOrig->SetLineColor(kRed);
// 	MEScaleDownOrig->SetLineColor(kCyan);

// 	MEScaleUp->SetLineStyle(1);
// 	MEScaleDown->SetLineStyle(1);
// 	MEScaleUpOrig->SetLineStyle(1);
// 	MEScaleDownOrig->SetLineStyle(1);
// 	MEScaleUp->SetLineWidth(2);
// 	MEScaleDown->SetLineWidth(2);
// 	MEScaleUpOrig->SetLineWidth(2);
// 	MEScaleDownOrig->SetLineWidth(2);

// 	MEScaleUpOrig->SetTitle("TTbar BDT disciminator");
// 	MEScaleUpOrig->Draw("hist");
// 	MEScaleDownOrig->Draw("histsame");
// 	nominal->Draw("histsame");


// 	TLegend *legMEScale = new TLegend(0.5,0.7,0.9,0.9);
// 	legMEScale->AddEntry(MEScaleUp,"MEScale Up","l");
// 	legMEScale->AddEntry(nominal,"nominal ttbar","l");
// 	legMEScale->AddEntry(MEScaleDown,"MEScale Down","l");
// 	legMEScale->Draw();
// 	cMEScale1->cd();

// 	TPad *pad2MEScale = new TPad("pad2","pad2",0,0,1,0.3);

// 	pad2MEScale->SetTopMargin(0);
// 	pad2MEScale->SetGridy();
// 	pad2MEScale->Draw();

// 	MEScaleUp->SetTitle("");

// 	pad2MEScale->cd();

// 	// MEScaleUp->Sumw2();
// 	MEScaleUp->SetStats(0);
// 	MEScaleUp->SetMinimum(0.5);
// 	MEScaleUp->SetMaximum(1.5);
// 	MEScaleUp->Divide(nominal);
// 	MEScaleUp->SetMarkerStyle(1);

// 	MEScaleUp->Draw("ep");
// 	MEScaleUp->SetAxisRange(0,2,"Y");
// 	MEScaleUp->Draw("ep");

// 	// MEScaleDown>Sumw2();
// 	MEScaleDown->SetStats(0);
// 	MEScaleDown->Divide(nominal);
// 	MEScaleDown->SetMarkerStyle(1);
// 	MEScaleDown->Draw("epsame");
// 	cMEScale1->cd();
// 	cMEScale1->SaveAs("MEScalesystt.eps");
// 	cout<<"MEScale done"<<endl;

	

// 	//JES
// 	TCanvas *cJES1 = new TCanvas();
// 	TPad *padJES1 = new TPad("pad1","pad1",0,0.3,1,1);
// 	cJES1->cd();
// 	padJES1->SetBottomMargin(0);
// 	padJES1->Draw();
// 	padJES1->cd();

// 	JESUp->SetLineColor(kRed);
// 	JESDown->SetLineColor(kCyan);

// 	JESUpOrig->SetLineColor(kRed);
// 	JESDownOrig->SetLineColor(kCyan);
// 	JESUp->SetLineStyle(1);
// 	JESDown->SetLineStyle(1);
// 	JESUpOrig->SetLineStyle(1);
// 	JESDownOrig->SetLineStyle(1);
// 	JESUp->SetLineWidth(2);
// 	JESDown->SetLineWidth(2);
// 	JESUpOrig->SetLineWidth(2);
// 	JESDownOrig->SetLineWidth(2);
// 	JESUpOrig->SetTitle("TTbar BDT disciminator");
// 	JESUpOrig->Draw("hist");
// 	JESDownOrig->Draw("histsame");
// 	nominal->Draw("histsame");

// 	TLegend *legJES = new TLegend(0.5,0.7,0.9,0.9);
// 	legJES->AddEntry(JESUp,"JES Up","l");
// 	legJES->AddEntry(nominal,"nominal ttbar","l");
// 	legJES->AddEntry(JESDown,"JES Down","l");

// 	legJES->Draw();
// 	cJES1->cd();

// 	TPad *padJES2 = new TPad("pad2","pad2",0,0,1,0.3);

// 	padJES2->SetTopMargin(0);
// 	padJES2->SetGridy();
// 	padJES2->Draw();

// 	JESUp->SetTitle("");

// 	padJES2->cd();
// 	// JESUp->Sumw2();
// 	JESUp->SetStats(0);
// 	JESUp->SetMinimum(0.5);
// 	JESUp->SetMaximum(1.5);	
// 	JESUp->Divide(nominal);

// 	JESUp->SetMarkerStyle(1);
// 	JESUp->Draw("ep");
// 	// JESDown>Sumw2();
// 	JESDown->SetStats(0);
// 	JESDown->Divide(nominal);
// 	JESDown->SetMarkerStyle(1);
// 	JESDown->Draw("epsame");
// 	cJES1->cd();
// 	cJES1->SaveAs("JESsystt.eps");

// 	cout<<"JES done"<<endl;

// 	//JER
// 	TCanvas *cJER1 = new TCanvas();
// 	TPad *padJER1 = new TPad("pad1","pad1",0,0.3,1,1);
// 	cJER1->cd();
// 	padJER1->SetBottomMargin(0);
// 	padJER1->Draw();
// 	padJER1->cd();

// 	JERUp->SetLineColor(kRed);
// 	JERDown->SetLineColor(kCyan);

// 	JERUpOrig->SetLineColor(kRed);
// 	JERDownOrig->SetLineColor(kCyan);

// 	JERUp->SetLineStyle(1);
// 	JERDown->SetLineStyle(1);
// 	JERUpOrig->SetLineStyle(1);
// 	JERDownOrig->SetLineStyle(1);
// 	JERUp->SetLineWidth(2);
// 	JERDown->SetLineWidth(2);
// 	JERUpOrig->SetLineWidth(2);
// 	JERDownOrig->SetLineWidth(2);
// 	JERDownOrig->SetTitle("TTbar BDT disciminator");
// 	JERDownOrig->Draw("hist");
// 	JERUpOrig->Draw("histsame");
// 	nominal->Draw("histsame");

// 	TLegend *legJER = new TLegend(0.5,0.7,0.9,0.9);
// 	legJER->AddEntry(JERUp,"JER Up","l");
// 	legJER->AddEntry(nominal,"nominal ttbar","l");
// 	legJER->AddEntry(JERDown,"JER Down","l");

// 	legJER->Draw();
// 	cJER1->cd();

// 	TPad *padJER2 = new TPad("pad2","pad2",0,0,1,0.3);

// 	padJER2->SetTopMargin(0);
// 	padJER2->SetGridy();
// 	padJER2->Draw();

// 	JERUp->SetTitle("");

// 	padJER2->cd();
// 	// JERUp->Sumw2();
// 	JERUp->SetMinimum(0.5);
// 	JERUp->SetMaximum(1.5);
// 	JERUp->SetStats(0);
// 	JERUp->Divide(nominal);

// 	JERUp->SetMarkerStyle(1);
// 	JERUp->Draw("ep");
// 	// JERDown>Sumw2();
// 	JERDown->SetStats(0);
// 	JERDown->Divide(nominal);
// 	JERDown->SetMarkerStyle(1);
// 	JERDown->Draw("epsame");
// 	cJER1->cd();
// 	cJER1->SaveAs("JERsystt.eps");	
// 	cout<<"JER done"<<endl;


// 	//ttGenerator
// 	TCanvas *cttGenerator1 = new TCanvas();
// 	TPad *padttGenerator1 = new TPad("pad1","pad1",0,0.3,1,1);
// 	cttGenerator1->cd();
// 	padttGenerator1->SetBottomMargin(0);
// 	padttGenerator1->Draw();
// 	padttGenerator1->cd();

// 	ttGeneratorUp->SetLineColor(kRed);
// 	ttGeneratorDown->SetLineColor(kCyan);

// 	ttGeneratorUpOrig->SetLineColor(kRed);
// 	ttGeneratorDownOrig->SetLineColor(kCyan);

// 	ttGeneratorUp->SetLineStyle(1);
// 	ttGeneratorDown->SetLineStyle(1);
// 	ttGeneratorUpOrig->SetLineStyle(1);
// 	ttGeneratorDownOrig->SetLineStyle(1);
// 	ttGeneratorUp->SetLineWidth(2);
// 	ttGeneratorDown->SetLineWidth(2);
// 	ttGeneratorUpOrig->SetLineWidth(2);
// 	ttGeneratorDownOrig->SetLineWidth(2);
// 	ttGeneratorUpOrig->SetTitle("TTbar BDT disciminator");
// 	ttGeneratorUpOrig->Draw("hist");
// 	ttGeneratorDownOrig->Draw("histsame");
// 	nominal->Draw("histsame");

// 	TLegend *legttGenerator = new TLegend(0.5,0.7,0.9,0.9);
// 	legttGenerator->AddEntry(ttGeneratorUp,"ttGenerator Up","l");
// 	legttGenerator->AddEntry(nominal,"nominal ttbar","l");
// 	legttGenerator->AddEntry(ttGeneratorDown,"ttGenerator Down","l");

// 	legttGenerator->Draw();
// 	cttGenerator1->cd();

// 	TPad *padttGenerator2 = new TPad("pad2","pad2",0,0,1,0.3);

// 	padttGenerator2->SetTopMargin(0);
// 	padttGenerator2->SetGridy();
// 	padttGenerator2->Draw();

// 	ttGeneratorUp->SetTitle("");

// 	padttGenerator2->cd();
// 	// ttGeneratorUp->Sumw2();
// 	ttGeneratorUp->SetStats(0);
// 	ttGeneratorUp->SetMinimum(0.5);
// 	ttGeneratorUp->SetMaximum(1.5);
// 	ttGeneratorUp->Divide(nominal);
// 	ttGeneratorUp->SetMarkerStyle(1);
// 	ttGeneratorUp->Draw("ep");
// 	// ttGeneratorDown>Sumw2();
// 	ttGeneratorDown->SetStats(0);
// 	ttGeneratorDown->Divide(nominal);
// 	ttGeneratorDown->SetMarkerStyle(1);
// 	ttGeneratorDown->Draw("epsame");
// 	cttGenerator1->cd();
// 	cttGenerator1->SaveAs("ttGeneratorsystt.eps");	
// 	cout<<"ttGenerator done"<<endl;

// 	//scale hadronisation

// 	TCanvas *cScaleH1 = new TCanvas();
// 	TPad *padScaleH1 = new TPad("pad1","pad1",0,0.3,1,1);
// 	cScaleH1->cd();
// 	padScaleH1->SetBottomMargin(0);
// 	padScaleH1->Draw();
// 	padScaleH1->cd();

// 	ScaleHUp->SetLineColor(kRed);
// 	ScaleHDown->SetLineColor(kCyan);

// 	ScaleHUpOrig->SetLineColor(kRed);
// 	ScaleHDownOrig->SetLineColor(kCyan);

// 	ScaleHUp->SetLineStyle(1);
// 	ScaleHDown->SetLineStyle(1);
// 	ScaleHUpOrig->SetLineStyle(1);
// 	ScaleHDownOrig->SetLineStyle(1);


// 	ScaleHUp->SetLineWidth(2);
// 	ScaleHDown->SetLineWidth(2);
// 	ScaleHUpOrig->SetLineWidth(2);
// 	ScaleHDownOrig->SetLineWidth(2);

// 	ScaleHUpOrig->SetTitle("TTbar BDT disciminator");
// 	ScaleHUpOrig->Draw("hist");
// 	ScaleHDownOrig->Draw("histsame");
// 	nominal->Draw("histsame");
// 	TLegend *legScaleH = new TLegend(0.5,0.7,0.9,0.9);
// 	legScaleH->AddEntry(ScaleHUp,"scaleH Up","l");
// 	legScaleH->AddEntry(nominal,"nominal ttbar","l");
// 	legScaleH->AddEntry(ScaleHDown,"scaleH Down","l");
// 	legScaleH->Draw();
// 	cScaleH1->cd();

// 	TPad *pad2ScaleH = new TPad("pad2","pad2",0,0,1,0.3);

// 	pad2ScaleH->SetTopMargin(0);
// 	pad2ScaleH->SetGridy();
// 	pad2ScaleH->Draw();

// 	ScaleHUp->SetTitle("");

// 	pad2ScaleH->cd();
// 	// ScaleHUp->Sumw2();
// 	ScaleHUp->SetStats(0);
// 	ScaleHUp->SetMinimum(0.5);
// 	ScaleHUp->SetMaximum(1.5);
// 	ScaleHUp->Divide(nominal);
// 	ScaleHUp->SetMarkerStyle(1);
// 	ScaleHUp->SetAxisRange(0,2,"Y");

// 	ScaleHUp->Draw("ep");

// 	ScaleHUp->SetAxisRange(0,2,"Y");


// 	ScaleHUp->Draw("ep");
// 	// ScaleHDown>Sumw2();
// 	ScaleHDown->SetStats(0);
// 	ScaleHDown->Divide(nominal);
// 	ScaleHDown->SetMarkerStyle(1);
// 	ScaleHDown->SetAxisRange(0,2,"Y");

// 	ScaleHDown->Draw("epsame");
// 	cScaleH1->cd();
// 	cScaleH1->SaveAs("ScaleHsystt.eps");
// 	cout<<"had scale done"<<endl;


}
