//#define ROOT_H 0
#ifdef ROOT_H
#include "TH1F.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TPad.h"
#include "TMath.h"
#endif
#include <iostream>
#include "Pythia8/Pythia.h"
#include "ParticleRecord.hpp"
#include "EvtRecord.hpp"
#include "RunRecord.hpp"
#include <sstream>
#include <vector>
#include <utility>
using namespace Pythia8;
using namespace pythia_evt;
using namespace std;

int main(int argc, char* argv[]) {
    Pythia pythia;
    pythia.readFile(argv[1]);
    //pythia.SetFrame("CMS", 200.0);
    //pythia.SetBlue("proton");
    //pythia.SetYell("proton");
    //pythia.Set("SoftQCD:minBias = on");
    //pythia.readString("Top:gg2ttbar = on");
    //pythia.readString("Beams:eCM = 8000.");
    pythia.init();
    int nEvts = pythia.mode("Main:numberOfEvents"); 

    vector<pair<int, int>> evt_classifier;
    evt_classifier.push_back(make_pair(0, 11));
    evt_classifier.push_back(make_pair(11, 16));
    evt_classifier.push_back(make_pair(16, 21));
    evt_classifier.push_back(make_pair(21, 1000));
    RunRecord run_book(evt_classifier); 

#ifdef ROOT_H
    TH1F* h_mult = new TH1F("charged_mult.", "", 75, 0, 75); 
    TH1F* h_pt = new TH1F("pT", "pT", 60, 0, 15.);
    TH1F* h_diff_phi = new TH1F("diff_phi", "diff. phi", 50, -3.14, 3.14);
    TH1F* h_eta = new TH1F("eta", "eta", 50, -1.2, 1.2);
#endif

    for (int i = 0; i < nEvts; ++i) {
	pythia.next();
	Event& evt = pythia.event;
	int mult = 0;
	EvtRecord evt_record(i);
	int id_particle = 0;
	for (int j = 1; j < evt.size(); ++j) {
	    if (evt[j].isFinal() && evt[j].isCharged() && abs(evt[j].eta()) <= 1.0) {
		++mult;	
                
		evt_record.addParticle(ParticleRecord(id_particle++, 
						      evt[j].name(), 
						      evt[j].pT(),
						      evt[j].px(), 
						      evt[j].py(), 
						      evt[j].pz(), 
						      evt[j].phi(),
						      evt[j].eta(), 
						      evt[j].index()));

		// pions
		if (evt[j].id() == 211 || evt[j].id() == -221) {
		    if (evt[j].pT() > .15){
#ifdef ROOT_H
			h_pt->Fill(evt[j].pT(), 1. / evt[j].pT());
#endif
		    }
		}
		// phi diff
#ifdef ROOT_H
		h_diff_phi->Fill(TMath::ATan2(evt[j].py(), evt[j].px()) - evt[j].phi());
		h_eta->Fill(evt[j].eta());
#endif
	    }
	}
	evt_record.setMult(mult);
	ostringstream ss;
	evt.list(false, true, ss);
	cout << ss.str() << endl;
	run_book.addEvt(evt_record);
	//run_book.addEvtList(evt_record, ss.str());
	//cout << "==========this is the end==========" << endl;
#ifdef ROOT_H
	h_mult->Fill(mult);
#endif
    }
    
#ifdef ROOT_H
    h_mult->SetMarkerStyle(8);
    h_mult->SetMarkerColor(4);
    h_mult->SetLineWidth(h_mult->GetLineWidth() * 3);
    h_mult->GetXaxis()->SetTitle("Multiplicity");
    h_mult->GetXaxis()->SetRangeUser(0, 45);
    h_mult->GetXaxis()->SetTitleSize(0.045);
    h_mult->GetYaxis()->SetTitle("Counts");
    h_mult->GetYaxis()->SetTitleSize(0.045);
    h_mult->GetYaxis()->SetRangeUser(2, 13000);
    gStyle->SetOptStat(0);
    gStyle->SetFrameLineWidth(3);

    h_mult->Draw("E");
    TLatex* latex = new TLatex(18, 2000, "#splitline{PYTHIA 8.1 #sqrt{s} = 200GeV}{                   -1.0 < #eta < 1.0}");
    latex->SetTextSize(.06);
    latex->Draw("p9");

    gPad->SetLogy();
    gPad->SetTicks(1, 1);
    gPad->SaveAs("mult.pdf");
    gPad->SaveAs("mult.eps");
    gPad->SaveAs("mult.C");

    {
	h_pt->GetYaxis()->SetTitle("#frac{dN}{pTdpT}");
	ofstream table_pt("tabulated_pt_dist.txt");
	table_pt << "pT(center, GeV/c), dN/pTdpT" << endl;
	for (int i = 1; i <= 60; ++i) { 
            table_pt << h_pt->GetBinCenter(i) << ' ' << h_pt->GetBinContent(i) << endl; 
	}
	table_pt.close();
	h_pt->Draw();
	gPad->SaveAs("pion_pt.pdf");
    }

    h_diff_phi->Draw();
    gPad->SaveAs("diff_phi.pdf");
    h_eta->Draw();
    gPad->SaveAs("eta.pdf");
#endif 
    cout << "=================Output of evt_book====================" << endl;
    //cerr << run_book << endl;
    run_book.printClassifiedEvts("forWong_wo_hist");
    return 0;
}
