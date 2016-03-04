{
	gStyle->SetOptStat(0);
	TFile *_file0 = TFile::Open("shapefileMu_DATA_BDT_inc.root");
	TH1F* nominal = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__nominal");

	TH1F* PUUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__PUUp");
	TH1F* PUDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__PUDown");
	TH1F* PUUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__PUUp");
	TH1F* PUDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__PUDown");

	TH1F* btagDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btagDown");
	TH1F* btagUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btagUp");
	TH1F* btagDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btagDown");
	TH1F* btagUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__btagUp");

	TH1F* scaleDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__scaleDown");
	TH1F* scaleUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__scaleUp");
	TH1F* scaleDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__scaleDown");
	TH1F* scaleUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__scaleUp");

    TH1F* scaleHDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ScaleHDown");
    TH1F* scaleHUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ScaleHUp");
    TH1F* scaleHDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ScaleHDown");
    TH1F* scaleHUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__ScaleHUp");

	TH1F* JESUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JESUp");
	TH1F* JESDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JESDown");
	TH1F* JESUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JESUp");
	TH1F* JESDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JESDown");

	TH1F* JERUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JERUp");
	TH1F* JERDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JERDown");
	TH1F* JERUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JERUp");
	TH1F* JERDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__JERDown"); 				

	TH1F* MatchingUp = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__matchingUp");
	TH1F* MatchingDown = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__matchingDown");
	TH1F* MatchingUpOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__matchingUp");
	TH1F* MatchingDownOrig = (TH1F*)_file0->Get("ttttmu__TTJets_powheg__matchingDown"); 	
	
	// gStyle->SetHistLineStyle(3);

 	//btag

	TCanvas *c1 = new TCanvas();
	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
	c1->cd();
	pad1->SetBottomMargin(0);
	pad1->Draw();
	pad1->cd();

	btagUp->SetLineColor(kRed);
	btagDown->SetLineColor(kCyan);
	btagUp->SetLineStyle(1);
	btagDown->SetLineStyle(1);
	btagUpOrig->SetLineStyle(1);
	btagDownOrig->SetLineStyle(1);

	btagUpOrig->SetLineColor(kRed);
	btagDownOrig->SetLineColor(kCyan);

	btagUpOrig->SetTitle("TTbar BDT disciminator");
	btagUpOrig->Draw("hist");
	btagDownOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *leg = new TLegend(0.5,0.7,0.9,0.9);
	leg->AddEntry(btagUp,"btag Up","l");
	leg->AddEntry(nominal,"nominal ttbar","l");
	leg->AddEntry(btagDown,"btag Down","l");
	leg->Draw();
	c1->cd();

	TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);

	pad2->SetTopMargin(0);
	pad2->SetGridy();
	pad2->Draw();

	btagUp->SetTitle("");

	pad2->cd();
	// btagUp->Sumw2();
	btagUp->SetStats(0);
	btagUp->Divide(nominal);
	btagUp->SetMarkerStyle(1);
	btagUp->Draw("ep");
	// btagDown>Sumw2();
	btagDown->SetStats(0);
	btagDown->Divide(nominal);
	btagDown->SetMarkerStyle(1);
	btagDown->Draw("epsame");
	c1->cd();
	c1->SaveAs("btagsys.pdf");

	cout<<"btag done"<<endl;


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

	PUDownOrig->SetTitle("TTbar BDT disciminator");
	PUDownOrig->Draw("hist");
	PUUpOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *legPU = new TLegend(0.5,0.7,0.9,0.9);
	legPU->AddEntry(PUUp,"PU Up","l");
	legPU->AddEntry(nominal,"nominal ttbar","l");
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
	PUUp->Divide(nominal);
	PUUp->SetMarkerStyle(1);
	PUUp->Draw("ep");
	// PUDown->Sumw2();
	PUDown->SetStats(0);
	PUDown->Divide(nominal);
	PUDown->SetMarkerStyle(1);
	PUDown->Draw("epsame");
	cPU1->cd();
	cPU1->SaveAs("PUsys.pdf");

	cout<<"PU done"<<endl;


	//scale

	TCanvas *cscale1 = new TCanvas();
	TPad *padscale1 = new TPad("pad1","pad1",0,0.3,1,1);
	cscale1->cd();
	padscale1->SetBottomMargin(0);
	padscale1->Draw();
	padscale1->cd();

	scaleUp->SetLineColor(kRed);
	scaleDown->SetLineColor(kCyan);

	scaleUpOrig->SetLineColor(kRed);
	scaleDownOrig->SetLineColor(kCyan);

	scaleUp->SetLineStyle(1);
	scaleDown->SetLineStyle(1);
	scaleUpOrig->SetLineStyle(1);
	scaleDownOrig->SetLineStyle(1);

	scaleUpOrig->SetTitle("TTbar BDT disciminator");
	scaleUpOrig->Draw("hist");
	scaleDownOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *legscale = new TLegend(0.5,0.7,0.9,0.9);
	legscale->AddEntry(scaleUp,"scale Up","l");
	legscale->AddEntry(nominal,"nominal ttbar","l");
	legscale->AddEntry(scaleDown,"scale Down","l");
	legscale->Draw();
	cscale1->cd();

	TPad *pad2scale = new TPad("pad2","pad2",0,0,1,0.3);

	pad2scale->SetTopMargin(0);
	pad2scale->SetGridy();
	pad2scale->Draw();

	scaleUp->SetTitle("");

	pad2scale->cd();
	// scaleUp->Sumw2();
	scaleUp->SetStats(0);
	scaleUp->Divide(nominal);
	scaleUp->SetMarkerStyle(1);
	scaleUp->Draw("ep");
	// scaleDown>Sumw2();
	scaleDown->SetStats(0);
	scaleDown->Divide(nominal);
	scaleDown->SetMarkerStyle(1);
	scaleDown->Draw("epsame");
	cscale1->cd();
	cscale1->SaveAs("scalesys.pdf");
	cout<<"scale done"<<endl;

	//scale hadronisation

	TCanvas *cscaleH1 = new TCanvas();
	TPad *padscaleH1 = new TPad("pad1","pad1",0,0.3,1,1);
	cscaleH1->cd();
	padscaleH1->SetBottomMargin(0);
	padscaleH1->Draw();
	padscaleH1->cd();

	scaleHUp->SetLineColor(kRed);
	scaleHDown->SetLineColor(kCyan);

	scaleHUpOrig->SetLineColor(kRed);
	scaleHDownOrig->SetLineColor(kCyan);

	scaleHUp->SetLineStyle(1);
	scaleHDown->SetLineStyle(1);
	scaleHUpOrig->SetLineStyle(1);
	scaleHDownOrig->SetLineStyle(1);


	scaleHUpOrig->SetTitle("TTbar BDT disciminator");
	scaleHUpOrig->Draw("hist");
	scaleHDownOrig->Draw("histsame");
	nominal->Draw("histsame");
	TLegend *legscaleH = new TLegend(0.5,0.7,0.9,0.9);
	legscaleH->AddEntry(scaleHUp,"scaleH Up","l");
	legscaleH->AddEntry(nominal,"nominal ttbar","l");
	legscaleH->AddEntry(scaleHDown,"scaleH Down","l");
	legscaleH->Draw();
	cscaleH1->cd();

	TPad *pad2scaleH = new TPad("pad2","pad2",0,0,1,0.3);

	pad2scaleH->SetTopMargin(0);
	pad2scaleH->SetGridy();
	pad2scaleH->Draw();

	scaleHUp->SetTitle("");

	pad2scaleH->cd();
	// scaleHUp->Sumw2();
	scaleHUp->SetStats(0);
	scaleHUp->Divide(nominal);
	scaleHUp->SetMarkerStyle(1);
	scaleHUp->Draw("ep");
	// scaleHDown>Sumw2();
	scaleHDown->SetStats(0);
	scaleHDown->Divide(nominal);
	scaleHDown->SetMarkerStyle(1);
	scaleHDown->Draw("epsame");
	cscaleH1->cd();
	cscaleH1->SaveAs("scaleHsys.pdf");
	cout<<"had scale done"<<endl;


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

	JESUpOrig->SetTitle("TTbar BDT disciminator");
	JESUpOrig->Draw("hist");
	JESDownOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *legJES = new TLegend(0.5,0.7,0.9,0.9);
	legJES->AddEntry(JESUp,"JES Up","l");
	legJES->AddEntry(nominal,"nominal ttbar","l");
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
	JESUp->Divide(nominal);
	JESUp->SetMarkerStyle(1);
	JESUp->Draw("ep");
	// JESDown>Sumw2();
	JESDown->SetStats(0);
	JESDown->Divide(nominal);
	JESDown->SetMarkerStyle(1);
	JESDown->Draw("epsame");
	cJES1->cd();
	cJES1->SaveAs("JESsys.pdf");

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

	JERDownOrig->SetTitle("TTbar BDT disciminator");
	JERDownOrig->Draw("hist");
	JERUpOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *legJER = new TLegend(0.5,0.7,0.9,0.9);
	legJER->AddEntry(JERUp,"JER Up","l");
	legJER->AddEntry(nominal,"nominal ttbar","l");
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
	JERUp->Divide(nominal);
	JERUp->SetMarkerStyle(1);
	JERUp->Draw("ep");
	// JERDown>Sumw2();
	JERDown->SetStats(0);
	JERDown->Divide(nominal);
	JERDown->SetMarkerStyle(1);
	JERDown->Draw("epsame");
	cJER1->cd();
	cJER1->SaveAs("JERsys.pdf");	
	cout<<"JER done"<<endl;


	//Matching
	TCanvas *cMatching1 = new TCanvas();
	TPad *padMatching1 = new TPad("pad1","pad1",0,0.3,1,1);
	cMatching1->cd();
	padMatching1->SetBottomMargin(0);
	padMatching1->Draw();
	padMatching1->cd();

	MatchingUp->SetLineColor(kRed);
	MatchingDown->SetLineColor(kCyan);

	MatchingUpOrig->SetLineColor(kRed);
	MatchingDownOrig->SetLineColor(kCyan);

	MatchingUp->SetLineStyle(1);
	MatchingDown->SetLineStyle(1);
	MatchingUpOrig->SetLineStyle(1);
	MatchingDownOrig->SetLineStyle(1);
	MatchingUpOrig->SetTitle("TTbar BDT disciminator");
	MatchingUpOrig->Draw("hist");
	MatchingDownOrig->Draw("histsame");
	nominal->Draw("histsame");

	TLegend *legMatching = new TLegend(0.5,0.7,0.9,0.9);
	legMatching->AddEntry(MatchingUp,"Matching Up","l");
	legMatching->AddEntry(nominal,"nominal ttbar","l");
	legMatching->AddEntry(MatchingDown,"Matching Down","l");

	legMatching->Draw();
	cMatching1->cd();

	TPad *padMatching2 = new TPad("pad2","pad2",0,0,1,0.3);

	padMatching2->SetTopMargin(0);
	padMatching2->SetGridy();
	padMatching2->Draw();

	MatchingUp->SetTitle("");

	padMatching2->cd();
	// MatchingUp->Sumw2();
	MatchingUp->SetStats(0);
	MatchingUp->Divide(nominal);
	MatchingUp->SetMarkerStyle(1);
	MatchingUp->Draw("ep");
	// MatchingDown>Sumw2();
	MatchingDown->SetStats(0);
	MatchingDown->Divide(nominal);
	MatchingDown->SetMarkerStyle(1);
	MatchingDown->Draw("epsame");
	cMatching1->cd();
	cMatching1->SaveAs("Matchingsys.pdf");	
	cout<<"matching done"<<endl;

}
