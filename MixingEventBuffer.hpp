#ifndef MIXINGEVENTBUFFER_H
#define MIXINGEVENTBUFFER_H
#include "ParticleRecord.hpp"
#include <unordered_map>
#include <utility>
#include <string>
#include <queue>
#include <vector>
#include "TRandom3.h"
#include "KeyFormatter.hpp"
using namespace std;
using namespace Pythia8;
namespace pythia_evt{
    class MixingEventBuffer 
    {
	public:
	    MixingEventBuffer(int cellsize, int seed, const KeyFormatter* formatter) : m_CellSize(cellsize), m_RndSeed(seed), m_RndGenerator(seed), m_KeyFormatter(formatter->clone()) {}

	    MixingEventBuffer(const MixingEventBuffer& buffer) {
		m_RndGenerator = buffer.m_RndGenerator;
		m_RndSeed = buffer.m_RndSeed;
		m_CellSize = buffer.m_CellSize;
		m_KeyFormatter = buffer.m_KeyFormatter->clone();
	    }

	    MixingEventBuffer& operator=(const MixingEventBuffer& buffer) 
	    {  
		if (this != &buffer) 
		{
		    m_RndGenerator = buffer.m_RndGenerator;
		    m_RndSeed = buffer.m_RndSeed;
		    m_CellSize = buffer.m_CellSize;
	            delete m_KeyFormatter;
		    m_KeyFormatter = buffer.m_KeyFormatter->clone();
		}
		return (*this);
	    } 

	    virtual ~MixingEventBuffer()
	    {
		delete m_KeyFormatter;
	    }

            void AddParticle(int mult, const Particle& particle) {
		string key = (*m_KeyFormatter)(mult, particle);
		if (m_ParticlePool.find(key) != m_ParticlePool.end()) 
		{
		    if(m_ParticlePool[key].size() == m_CellSize)
		    {
			return;
		    }
		    else 
                    {
			m_ParticlePool[key].push_back(particle);
		    }
		}
		else 
		{
		    vector<Particle> vec;
		    vec.reserve(m_CellSize);
		    vec.push_back(particle);
		    m_ParticlePool.insert(make_pair(key, vec));
		}
	    }

	    bool HaveMixedParticle(int mult, const Particle& particle) {
		string key = (*m_KeyFormatter)(mult, particle);
		return m_ParticlePool.find(key) != m_ParticlePool.end();
	    }

	    Particle GetMixedParticle(int mult, const Particle& particle) {
		string key = (*m_KeyFormatter)(mult, particle);
		int cellsize = m_ParticlePool[key].size();

		return m_ParticlePool[key][cellsize * (1.0 - m_RndGenerator.Rndm())];
	    }

	    void ChargePool(const EvtRecord& record) 
	    {
		pushEventIntoEventPool(record);
	    }

	    vector<vector<ParticleRecord>> GetEventsByMult(int mult) 
	    {
		return m_EventPool[to_string(mult)];
	    }

            void PrintCells() 
	    {
		for (auto cell : m_ParticlePool) {
		    cerr << cell.first << "-----" << cell.second.size() << endl;;
		}
	    }
	private:
	    unordered_map<string, vector<Particle>> m_ParticlePool;
	    unordered_map<string, vector<vector<ParticleRecord>>> m_EventPool;
	    int m_CellSize;
	    int m_RndSeed;
	    KeyFormatter* m_KeyFormatter;
	    TRandom3 m_RndGenerator;

	    /*
	    int getRndIdx(int size) { // Rndm() -> (0, 1)
		return m_RndGenerator->Rndm() * size;  
	    }
	    */

	    /*
	    vector<int> getNUniqueRandomNumber(int n, int mult) {
		vector<int> pool; 
		for (int i = 0; i < n; ++i) {
                    int k = getRndIdx(n);
		    while (pool.find(k) != pool.end()) {
			k = getRndIx(n);
		    }		     
		    pool.push_back(k);
		}

		return pool;
	    }
	    */

	    void pushEventIntoEventPool(const EvtRecord& record) {
		int mult = record.getMult();
		string key = to_string(mult);
		if (m_EventPool.find(key) != m_EventPool.end()) { // already there
		    int cellsize = m_EventPool[key].size();
		    int i = 0;
		    for (i = 0; i < cellsize; ++i) {
			if (m_EventPool[key][i].size() < mult) {
			    break;
			}
		    }

		    // need create a new matrix
                    if (i == cellsize) {
			for (int j = 0; j < mult; ++j) {
			    vector<ParticleRecord> vec(1, record.particleAt(j)); 
			    m_EventPool[key].push_back(vec);
			}
		    }
		    else {
			for (int j = 0; j < mult; ++j) {
			    m_EventPool[key][i].push_back(record.particleAt(j));
			    ++i;
			}
		    }
		}
		else {
		    vector<vector<ParticleRecord>> new_matrix(mult);
		    for (int j = 0; j < mult; ++j) {
                        new_matrix[j].push_back(record.particleAt(j));
		    }
		    m_EventPool.insert(make_pair(key, new_matrix));
		}
	    }
    };
}
#endif
