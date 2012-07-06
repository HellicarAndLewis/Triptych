#ifndef ROXLU_CLOAKH
#define ROXLU_CLOAKH

//#include "Roxlu.h"
//#include "PBD.h"
#include <roxlu/Roxlu.h>
#include <pbd/PBD.h>

class Cloak {
public:
	Cloak(Particles3& ps, vector<Triangle>& tris);
	
	void setup();
	void update();
	void debugDraw();
	void draw(const Mat4& pm, const Mat4& vm, const Mat3& nm);
	
	Particles3& ps;
	vector<Triangle>& tris;
	VerticesPNC vertices;
	
	VAO vao;
	GLuint vbo;
	Shader shader;
};

#endif