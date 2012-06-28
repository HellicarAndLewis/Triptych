#ifndef ROXLU_SPHERE_UNITH
#define ROXLU_SPHERE_UNITH

#include "AppSettings.h"
#include "Roxlu.h"
#include "PBD.h"

using namespace pbd;

class SphereUnit {
public:
	SphereUnit(Billboard& bb);
	void setup(Particle* center);
	void addParticles(const unsigned int& num);
	void update();
	void draw(const Mat4& pm, const Mat4& vm, const Vec3& right, const Vec3& up);
	void debugDraw();
	
	
	Billboard& particle_bb;
	Particles ps;
	Particle* centerp;
};

#endif