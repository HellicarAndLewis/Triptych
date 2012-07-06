#ifndef ROXLU_EXPLOSION_TRAILSH
#define ROXLU_EXPLOSION_TRAILSH
/*
#include "Roxlu.h"
#include "Trails.h"
#include "BoidTypes.h"
#include "PBD.h"
*/

#include <roxlu/Roxlu.h>
#include <application/visuals/Trails.h>
#include <application/BoidTypes.h>
#include <pbd/PBD.h>

class ExplosionTrails {
public:
	ExplosionTrails(Boids2& ps);
	void setup();
	void draw(const Mat4& pm, const Mat4& vm);
	void debugDraw();
	void update();
	
	size_t getBytesNeeded();
			
	Boids2& ps;
	Trails2PC trails;
	VerticesPC vertices;
	
	VAO vao;
	GLuint vbo;
	Shader shader;
	size_t allocated_bytes;
};

#endif