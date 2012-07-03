#ifndef ROXLU_VISUALS_BOIDH
#define ROXLU_VISUALS_BOIDH

#include "Roxlu.h"

class Boid {
public:
	Boid();
	void setup();
	void debugDraw();
	void draw(const Mat4& pm, const Mat4& vm, const Mat3& nm);
	void update();
	

	Vec3 position;
	Vec3 direction;
	VerticesP vertices;
	VertexData vd;
};

#endif