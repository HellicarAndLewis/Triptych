#ifndef ROXLU_VISUALS_BOIDDRAWERH
#define ROXLU_VISUALS_BOIDDRAWERH

//#include "Roxlu.h"
#include <roxlu/Roxlu.h>
class BoidDrawer {
public:
	BoidDrawer();
	void setup();
	void debugDraw(const Mat3& coordinateSystem);
	void draw(const float* pm, const float* vm, const float* nm, const float* coordinateSystem);
	void update();
	

	Shader shader;
	VAO vao;
	GLuint vbo;
	
	Vec3 position;
	Vec3 direction;
	VerticesP vertices;
	VertexData vd;
};

#endif