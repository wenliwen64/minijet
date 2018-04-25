#ifndef EVTRECORD_H
#define EVTRECORD_H
#include <iostream>
#include <fstream>
#include <vector>
#include "ParticleRecord.hpp"
using namespace std;
namespace pythia_evt{
    class EvtRecord {
	public:
	    EvtRecord(int id):m_id(id), m_particle_vec() {
		m_particle_vec.reserve(100);
	    }

	    void setMult(int mult) {
		m_mult = mult;
	    }

	    int getMult() const {
		return m_particle_vec.size();
	    }

	    void addParticle(const ParticleRecord& record) {
		m_particle_vec.push_back(record);
	    }

	    size_t nParticles() const {
		return m_particle_vec.size();
	    }

	    const ParticleRecord& particleAt(int i) const {
		return m_particle_vec[i];
	    }
	private:
	    int m_id;
	    int m_mult;
	    vector<ParticleRecord> m_particle_vec;
	    friend ostream& operator<<(ostream& os, const EvtRecord& evt);
//	    friend ofstream& operator<<(ofstream& of, const EvtRecord& evt);
    };

    ostream& operator<<(ostream& os, const EvtRecord& evt) {
	for (size_t i = 0, n = evt.nParticles(); i < n; ++i) {
	    os << evt.m_id << ' ' << evt.getMult() << ' ' << evt.particleAt(i) << endl;
	}
	return os;
    }

    /*
    ofstream& operator<<(ofstream& of, const EvtRecord& evt) {
	for (size_t i = 0, n = evt.nParticles(); i < n; ++i) {
	    of << evt.m_id << ' ' << evt.m_mult << ' ' << evt.particleAt(i) << endl;
	}
	return of;
    }
    */
}
#endif
