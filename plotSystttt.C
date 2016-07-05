{
	gStyle->SetOptStat(0);
	TFile *_file0 = TFile::Open("shapefileEl_DATA_BDT_El3rdJune_adaBoost_alphaSTune_subAll_noMinEvents_inc.root");
	TH1F* nominal = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__nominal");
	cout<<"PU"<<endl;
	TH1F* PUUp = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__PUUp");
	TH1F* PUDown = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__PUDown");
	TH1F* PUUpOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__PUUp");
	TH1F* PUDownOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__PUDown");
	cout<<"btag_light"<<endl;

	TH1F* btag_lightDown = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__btag_lightDown");
	TH1F* btag_lightUp = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__btag_lightUp");
	TH1F* btag_lightDownOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__btag_lightDown");
	TH1F* btag_lightUpOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__btag_lightUp");

	cout<<"btag_heavy"<<endl; 

	TH1F* btag_heavyDown = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__btag_heavyDown");
	TH1F* btag_heavyUp = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__btag_heavyUp");
	TH1F* btag_heavyDownOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__btag_heavyDown");
	TH1F* btag_heavyUpOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__btag_heavyUp");
	cout<<"scale"<<endl;

	TH1F* MEScaleDown = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__ttttMEScaleDown");
	TH1F* MEScaleUp = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__ttttMEScaleUp");
	TH1F* MEScaleDownOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__ttttMEScaleDown");
	TH1F* MEScaleUpOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__ttttMEScaleUp");
	cout<<"JES"<<endl;

	TH1F* JESUp = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__JESUp");
	TH1F* JESDown = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__JESDown");
	TH1F* JESUpOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__JESUp");
	TH1F* JESDownOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__JESDown");
	cout<<"JER"<<endl;

	TH1F* JERUp = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__JERUp");
	TH1F* JERDown = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__JERDown");
	TH1F* JERUpOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__JERUp");
	TH1F* JERDownOrig = (TH1F*)_file0->Get("ttttel__NP_overlay_ttttNLO__JERDown"); 				

	// gStyle->SetHistLineStyle(3);

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
	c1->SaveAs("btag_lightsystttt.eps");

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
	c1h->SaveAs("btag_heavysystttt.eps");

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
	cPU1->SaveAs("PUsystttt.eps");

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
	cMEScale1->SaveAs("MEScalesystttt.eps");
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
	cJES1->SaveAs("JESsystttt.eps");

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
	cJER1->SaveAs("JERsystttt.eps");	
	cout<<"JER done"<<endl;


	

}
