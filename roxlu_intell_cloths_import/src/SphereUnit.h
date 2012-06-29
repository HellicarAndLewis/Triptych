#ifndef ROXLU_SPHERE_UNITH
#define ROXLU_SPHERE_UNITH

#include "AppSettings.h"
#include "Roxlu.h"
#include "PBD.h"
#include "Trails.h"

using namespace pbd;

class SphereUnit {
public:
	SphereUnit(Billboard& bb);
	void setup(Particle* center, Particle* leftH, Particle* rightH);
	void addParticles(const unsigned int& num);
	void update();
	void draw(const Mat4& pm, const Mat4& vm, const Vec3& right, const Vec3& up);
	void debugDraw();
	
	// tweaking....
	void removeTrails();
	void resetParticleSizes();
	
	Particle* left_hand;
	Particle* right_hand;
	Particle* between_hands;
	
	Billboard& particle_bb;
	Particles ps;
	Particle* centerp;
	Trails trails;
	Particle* repeller;
	Particle* repeller_bottom;
	

};

#endif