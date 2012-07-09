#ifndef PERFORMANCE_FLOCKING_CONTROLLERH
#define PERFORMANCE_FLOCKING_CONTROLLERH

#include <roxlu/Roxlu.h>
#include <pbd/PBD.h>
#include "ofMain.h"

template<class T>
struct BoidParticle : public Particle<T> {
	BoidParticle(const T& pos, float mass)
		:Particle<T>(pos, mass)
	{
	}
	
	void update(float dt) {
		// is called after each integration step.
	}
};

typedef BoidParticle<ofVec3f> BoidParticle3;
typedef Particles<ofVec3f, BoidParticle<ofVec3f>, Spring<ofVec3f> >	BoidParticles3;
typedef Flocking<ofVec3f, BoidParticle<ofVec3f> > BoidFlocking3;

class FlockController {
public:
	FlockController();
	void setup();
	void setupGui();
	void debugDraw();
	void update();
	void checkBounds();
	
	BoidParticles3 ps;
	BoidFlocking3 flock;
	float center_energy;
	float sphere_size;
};

#endif