#ifndef ROXLU_VISUALS_KINECTH
#define ROXLU_VISUALS_KINECTH


#include "ofMain.h"
#include "KinectMesh.h"

#include <application/Settings.h>
#include <roxlu/opengl/VAO.h>
#include <roxlu/opengl/Shader.h>
#include <roxlu/math/Mat4.h>

class KinectDrawer {
public:
	KinectDrawer();
	void update(KinectMesh& kmeshes);
	void setup();
	void draw(const float* pm, const float* vm);
	void debugDraw();
	void debugDrawMesh(KinectMesh& m);
	void setUserDetected(bool detected);
	
	bool user_detected;
	int num_vertices;
	roxlu::Shader shader;
	roxlu::VAO vao;
	GLuint vbo;
	size_t allocated_bytes;
};

inline void KinectDrawer::setUserDetected(bool d) {
	user_detected = d;
}


#endif