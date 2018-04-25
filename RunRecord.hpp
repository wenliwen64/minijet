#ifndef RUNRECORD_H
#define RUNRECORD_H
#include "EvtRecord.hpp"
#include "ParticleRecord.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
namespace pythia_evt{
    class RunRecord {
	public:
	    RunRecord(const vector<pair<int, int>>& evt_classifier = vector<pair<int, int>>(1, make_pair(0, 1000))):m_total_evts(0), m_evt_vec(evt_classifier.size()), m_evt_list_vec(evt_classifier.size()), m_evt_classifier(evt_classifier) {}

	    void addEvt(const EvtRecord& evt) {
		int i = 0;
		for (; i < m_evt_classifier.size(); ++i) {
		    if (m_evt_classifier[i].first <= evt.getMult() 
			&& 
			evt.getMult() < m_evt_classifier[i].second) {
			break;
		    }
		}
		m_evt_vec[i].push_back(evt);
		++m_total_evts;
	    }

	    void addEvtList(const EvtRecord& evt, const string& li) {
		int i = 0;
		for (; i < m_evt_classifier.size(); ++i) {
		    if (m_evt_classifier[i].first <= evt.getMult()
			&&
			evt.getMult() < m_evt_classifier[i].second) {
			break;
		    }
		}
		m_evt_list_vec[i].push_back(li);
	    }

	    size_t nEvts() const {
		return m_total_evts;
	    }

	    void printClassifiedEvts(const string& prefix = "forWong", bool hist_switch = false) const {
		for (size_t i = 0, n = m_evt_vec.size();  i < n; ++i) {
		    string file_name(prefix + "_" + to_string(i) + ".txt");
		    ofstream ofile(file_name);
		    //ofile << "evt_id evt_mult particle_id particle_name pT px py pz phi eta index" << endl;
		    ofile << ParticleRecord::getHeader() << endl;
		    for (size_t j = 0, m = m_evt_vec[i].size(); j < m; ++j) {
			ofile << m_evt_vec[i][j];
			if (hist_switch) {
			    ofile << m_evt_list_vec[i][j];
			    if (m_evt_list_vec[i][j].back() != '\n')
				ofile << endl;
			}
		    }
		    ofile.close();
		}
	    }
	private:
	    int m_total_evts;
	    vector<pair<int, int>> m_evt_classifier;
	    vector<vector<EvtRecord>> m_evt_vec;
	    vector<vector<string>> m_evt_list_vec;
	    friend ostream& operator<<(ostream& os, const RunRecord& run);
    };

    ostream& operator<<(ostream& os, const RunRecord& run) {
	os << "evt_id evt_mult particle_id particle_name pT px py pz phi eta" << endl;
	for (size_t i = 0, n = run.m_evt_vec.size(); i < n; ++i) {
	    for (size_t j = 0, m = run.m_evt_vec[i].size(); j < m; ++j) {
		os << run.m_evt_vec[i][j];
	    }
	}
	return os;
    }
}
#endif
