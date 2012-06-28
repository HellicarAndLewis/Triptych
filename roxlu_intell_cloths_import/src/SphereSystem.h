#ifndef ROXLU_SPHERESYSTEMH
#define ROXLU_SPHERESYSTEMH

#include "ParticleMesh.h"
#include "SphereUnit.h"
#include "Roxlu.h"
#include "PBD.h"

using namespace pbd;
struct ChangeSpringStrength {
	ChangeSpringStrength(const float& f):f(f){}
	
	void operator()(Spring* s) {
		s->k = f;
	}
	float f;
};

class SphereSystem {
public:
	SphereSystem(ParticleMesh& pm);
	~SphereSystem();
	void setup();
	void update();
	void draw();
	void debugDraw();
	
	SphereUnit su;
	VertexData sphere_vd;
	Particles left_hand_ps;
	ParticleMesh& pm;
	
	
	//vector<Particle*> spheres;
};
#endif