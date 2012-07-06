#ifndef ROXLU_VISUALS_KINECTH
#define ROXLU_VISUALS_KINECTH


#include "ofMain.h"
#include "KinectMesh.h"

#include <roxlu/opengl/VAO.h>
#include <roxlu/opengl/Shader.h>

class KinectDrawer {
public:
	KinectDrawer();
	void update(KinectMesh& kmeshes);
	void setup();
	void draw(const float* pm, const float* vm);
	void debugDraw();
	void debugDrawMesh(KinectMesh& m);

	int num_vertices;
	roxlu::Shader shader;
	roxlu::VAO vao;
	GLuint vbo;
	size_t allocated_bytes;
};

#endif