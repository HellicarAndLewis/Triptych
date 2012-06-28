#ifndef ROXLU_SPHERESYSTEMH
#define ROXLU_SPHERESYSTEMH

#include "ofMain.h"
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
	void draw(const Mat4& pm, const Mat4& vm, const Vec3& right, const Vec3& up);
	void debugDraw();
	
	// Tweaking:
	void removeTrails();
	void resetParticleSizes();
	
	Billboard particle_bb;
	Texture particle_tex;
	
	SphereUnit su;
	VertexData sphere_vd;
	Particles left_hand_ps;
	ParticleMesh& pm;

	
	//vector<Particle*> spheres;
};
#endif