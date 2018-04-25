#ifndef KEYFORMATTER_H
#define KEYFORMATTER_H
#include "ParticleRecord.hpp"
#include "Pythia8/Pythia.h"
#include <string>
using namespace std;
namespace pythia_evt{
    class KeyFormatter{
	public:
	virtual KeyFormatter* clone() const = 0;
	virtual ~KeyFormatter() {}
	virtual string operator()(int mult, const Pythia8::Particle& record) const = 0;
    };

    class KeyMultSpec : public KeyFormatter 
    {
	public:
	virtual KeyFormatter* clone() const
	{
	    return new KeyMultSpec(*this);
	} 

	virtual string operator()(int mult, const Pythia8::Particle& particle) const
        {
	    return to_string(mult) + particle.name(); 
	}
    };

    class KeyMult : public KeyFormatter 
    {
	public:
	virtual KeyFormatter* clone() const
	{
	    return new KeyMult(*this);
	}

	virtual string operator()(int mult, const Pythia8::Particle& particle) const
        {
	    return to_string(mult); 
	}
    };
}
#endif
