#ifndef ROXLU_SPHERE_UNITH
#define ROXLU_SPHERE_UNITH

#include "Roxlu.h"
#include "PBD.h"

using namespace pbd;

class SphereUnit {
public:
	SphereUnit();
	void setup(Particle* center);
	void addParticles(const unsigned int& num);
	void update();
	void debugDraw();
	
	Particles ps;
	Particle* centerp;
};

#endif