#include "Pythia8/Pythia.h"
#include "TH1F.h"
#include "TPad.h"
#include <iostream>
#include "TMath.h"
#include "ParticleRecord.hpp"
#include "EvtRecord.hpp"
#include "RunRecord.hpp"
#include "MixingEventBuffer.hpp"
#include <vector>
#include <utility>
#include "KeyFormatter.hpp"
using namespace Pythia8;
using namespace std;
using namespace pythia_evt;

int main(int argc, char* argv[]) 
{
    Pythia pythia;
    pythia.readFile(argv[1]);
    pythia.init();
    int nEvts = pythia.mode("Main:numberOfEvents");

    vector<pair<int, int>> evt_classifier;
    evt_classifier.push_back(make_pair(0, 11));
    evt_classifier.push_back(make_pair(11, 16));
    evt_classifier.push_back(make_pair(16, 21));
    evt_classifier.push_back(make_pair(21, 1000));
    RunRecord run_book(evt_classifier);

    // some QA Histograms...
    TH1F* px_sum_before = new TH1F("px_sum_before", "px_sum_before_mixing", 200, -30, 30);
    TH1F* eta_sum_before = new TH1F("eta_sum_before", "eta_sum_before_mixing", 200, -10, 10);
    TH1F* px_sum_after = new TH1F("px_sum_after", "px_sum_after_mixing", 200, -30, 30);
    TH1F* eta_sum_after = new TH1F("eta_sum_after", "eta_sum_after_mixing", 200, -10, 10);
    TH1F* hrnd_qa = new TH1F("rnd_qa", "rnd_qa", 40, -5, 5);

    // charge the event pool...
    KeyMult* keyFormatter = new KeyMult();
    MixingEventBuffer buffer(1000, 1, keyFormatter);
    for (int i = 0; i < nEvts; ++i) 
    {
        pythia.next();
        Event& evt = pythia.event;
        int mult = 0;
        int id_particle = 0;
	EvtRecord evt_record(i);
	double px_sum = 0;
	double eta_sum = 0;
        for (int j = 1; j < evt.size(); ++j) 
	{
            if (evt[j].isFinal() && evt[j].isCharged() && abs(evt[j].eta()) <= 1.0) 
	    {
                ++mult;
		evt_record.addParticle(ParticleRecord(id_particle++, evt[j].name(), evt[j].pT(),
			    evt[j].px(), evt[j].py(), evt[j].pz(), evt[j].phi(),
			    evt[j].eta(), evt[j].index()));
		px_sum += evt[j].px();
		eta_sum += evt[j].eta();
            }
        }

	buffer.ChargePool(evt_record);
	px_sum_before->Fill(px_sum);
	eta_sum_before->Fill(eta_sum);
    }

    // Gain randomized event
    int evt_id = 0;
    for (int i_mult = 1; i_mult < 30; ++i_mult) {
	vector<vector<ParticleRecord>> cell = buffer.GetEventsByMult(i_mult);
	for (int j = 0; j < cell.size(); ++j) {
	    EvtRecord evt_record(evt_id++);
	    if (cell[j].size() < i_mult)
		continue;
	    else {
		double px_sum = 0;
		double eta_sum = 0;
		for (int k = 0; k < i_mult; ++k){ 
		    evt_record.addParticle(cell[j][k]);
		    px_sum += cell[j][k].m_px;
		    eta_sum += cell[j][k].m_eta;
		}
		px_sum_after->Fill(px_sum);
		eta_sum_after->Fill(eta_sum);
		run_book.addEvt(evt_record);
	    }
	}
    } 

    TRandom3 rnd_generator(1);

    // do mixing... 
    /*
    Pythia pythia2;
    pythia2.readFile(argv[1]);
    pythia2.init();
    for (int i = 0; i < nEvts / 2; ++i) 
    {
	cout << "doing ok" << endl;
	pythia2.next();
	Event& evt = pythia2.event;
	int mult = 0;
	EvtRecord evt_record(i);
	int id_particle = 0;
	double px_sum = 0;
	for (int j = 1; j < evt.size(); ++j)
	{
	    if (evt[j].isFinal() && evt[j].isCharged() && abs(evt[j].eta()) <= 1.0) 
	    {
		++mult;
	    }
	}

	for (int j = 1; j < evt.size(); ++j) 
	{
	     if (evt[j].isFinal() && evt[j].isCharged() && abs(evt[j].eta()) <= 1.0) 
	     {
		if (rnd_generator.Rndm() > 0.001 && buffer.HaveMixedParticle(mult, evt[j])) 
		{
	            evt_record.addParticle(ParticleRecord(id_particle++, buffer.GetMixedParticle(mult, evt[j])));	
		    px_sum += buffer.GetMixedParticle(mult, evt[j]).px();
		}	
		else 
		{
		    evt_record.addParticle(ParticleRecord(id_particle++, evt[j].name(), evt[j].pT(),
                            evt[j].px(), evt[j].py(), evt[j].pz(), evt[j].phi(),
                            evt[j].eta()));
		     px_sum += evt[j].px();
		}
	     }
	}
	evt_record.setMult(mult);
	run_book.addEvt(evt_record);
	px_sum_after->Fill(px_sum);
    } 
*/
    run_book.printClassifiedEvts("forWong_mixed");

    for (int i = 0; i < 10000; ++i) {
	hrnd_qa->Fill(rnd_generator.Rndm());
    }

    buffer.PrintCells();
    px_sum_before->Draw();
    gPad->SaveAs("px_sum_before.pdf");
    eta_sum_before->Draw();
    gPad->SaveAs("eta_sum_before.pdf");
    px_sum_after->Draw();
    gPad->SaveAs("px_sum_after.pdf");
    eta_sum_after->Draw();
    gPad->SaveAs("eta_sum_after.pdf");
    hrnd_qa->Draw();
    gPad->SaveAs("hrnd_qa.pdf");
}
