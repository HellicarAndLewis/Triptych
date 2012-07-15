#ifndef ROXLU_EXPLOSION_TRAILSH
#define ROXLU_EXPLOSION_TRAILSH

#include "ofMain.h"
#include <roxlu/Roxlu.h>
#include <pbd/PBD.h>
#include <application/Settings.h>
#include <application/visuals/Trails.h>
#include <application/BoidTypes.h>

class TrailsDrawer {
public:
	TrailsDrawer(Boids& flockPS);
	void setup();
	void draw(const float* pm, const float* vm);
	void debugDraw();
	void update();
	size_t getBytesNeeded();
			
	float trail_color[4];		
	Boids& flock_ps;
	Trails3PT trails;
	VerticesPT vertices;
	Texture flow_tex;
	Texture trail_tex;
	VAO vao;
	GLuint vbo;
	Shader shader;
	size_t allocated_bytes;
};

#endif