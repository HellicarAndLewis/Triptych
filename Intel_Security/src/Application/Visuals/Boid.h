#ifndef ROXLU_VISUALS_BOIDH
#define ROXLU_VISUALS_BOIDH

//#include "Roxlu.h"
#include <roxlu/Roxlu.h>
class Boid {
public:
	Boid();
	void setup();
	void debugDraw();
	void draw(const float* pm, const float* vm, const float* nm);
	void update();
	

	Vec3 position;
	Vec3 direction;
	VerticesP vertices;
	VertexData vd;
};

#endif