#ifndef PARTICLERECORD_H
#define PARTICLERECORD_H
#include <iostream>
#include <fstream>
#include "Pythia8/Pythia.h"
using namespace std;
namespace pythia_evt{
    struct ParticleRecord{
	ParticleRecord(int id, const string& name, double pt,
		double px, double py, double pz,
		double phi, double eta, int index):m_id(id), m_name(name), m_pt(pt),
	m_px(px), m_py(py), m_pz(pz),
	m_phi(phi), m_eta(eta), m_index(index) {}

	ParticleRecord(int id, const Pythia8::Particle& particle) {
	    m_id = id; 
	    m_name = particle.name();	
	    m_pt = particle.pT();
	    m_px = particle.px();
	    m_py = particle.py();
	    m_pz = particle.pz();
            m_phi = particle.phi();
            m_eta = particle.eta();
	    m_index = particle.index();
	}

	static string getHeader() {
	    return "evt_id evt_mult particle_id particle_name pT px py pz phi eta index";
	}

	int m_id;
	string m_name;
	double m_pt;
	double m_px;
	double m_py;
	double m_pz;
	double m_phi;
	double m_eta;
	int m_index;
    };

    ostream& operator<<(ostream& os, const ParticleRecord& particle) {
	os << particle.m_id << ' ' << particle.m_name << ' '
	    << particle.m_pt << ' ' << particle.m_px
	    << ' ' << particle.m_py << ' ' << particle.m_pz << ' '
	    << particle.m_phi << ' ' << particle.m_eta << ' ' << particle.m_index;
	return os;
    }

    ofstream& operator<<(ofstream& of, const ParticleRecord& particle) {
	of << particle.m_id << ' ' << particle.m_name << ' '
	    << particle.m_pt << ' ' << particle.m_px
	    << ' ' << particle.m_py << ' ' << particle.m_pz << ' '
	    << particle.m_phi << ' ' << particle.m_eta << ' ' << particle.m_index;
	return of;
    }
}
#endif
