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
		Particle<T>::lifespan = FLT_MAX;

	}
	
	void kill() {
		Particle<T>::lifespan = Particle<T>::age;
	}
	

	
	void update(float dt) {

		// is called after each integration step.
	}
};

class Explosion {
public:
	ofVec3f pos;
	void init(const ofVec3f &pos) {
		this->pos = pos;
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
	void update(unsigned char *thresh);
	void checkBounds();
	
	BoidParticles3 ps;
	BoidFlocking3 flock;
	float center_energy;
	float sphere_size;
	ofImage fluff;
	vector<Explosion> explosions;
	
};

#endif