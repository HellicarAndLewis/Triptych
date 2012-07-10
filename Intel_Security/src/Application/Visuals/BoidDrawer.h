#ifndef ROXLU_VISUALS_BOIDDRAWERH
#define ROXLU_VISUALS_BOIDDRAWERH

//#include "Roxlu.h"
#include "ofMain.h"
#include <application/Settings.h>
#include <roxlu/Roxlu.h>

class BoidDrawer {
public:
	BoidDrawer();
	void setup();
	void debugDraw(const Mat3& coordinateSystem);
	void draw(const float* pm, const float* vm, const float* nm, const Mat3& coordinateSystem);
	void update();
	

	Shader shader;
	VAO vao;
	GLuint vbo;
	Texture tex;	

	float size;
	Vec3 position;
	Vec3 direction;
	VerticesPTN vertices;
	VertexData vd;
};

#endif