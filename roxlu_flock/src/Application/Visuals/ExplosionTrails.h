#ifndef ROXLU_EXPLOSION_TRAILSH
#define ROXLU_EXPLOSION_TRAILSH

/*
#include "Roxlu.h"
#include "Trails.h"
#include "BoidTypes.h"
#include "PBD.h"
*/

#include <roxlu/Roxlu.h>
#include <pbd/PBD.h>
#include <application/visuals/Trails.h>
#include <application/BoidTypes.h>

class ExplosionTrails {
public:
	ExplosionTrails(Boids& ps);
	void setup();
	void draw(const float* pm, const float* vm);
	void debugDraw();
	void update();
	
	size_t getBytesNeeded();
			
	Boids& ps;
	Trails2PC trails;
	VerticesPC vertices;
	
	VAO vao;
	GLuint vbo;
	Shader shader;
	size_t allocated_bytes;
};

#endif