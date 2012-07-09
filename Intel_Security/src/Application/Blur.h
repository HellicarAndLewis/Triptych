#ifndef ROXLU_BLURH
#define ROXLU_BLURH

#include "ofMain.h"

#include <roxlu/Roxlu.h>

struct BlurVertex {
	float pos[3];
	float tex[2];
	
	void set(const float& x, const float& y, const float& s, const float& t) {
		pos[0] = x;
		pos[1] = y;
		pos[2] = 0.0;
		tex[0] = s;
		tex[1] = t;
	}
};

class Blur {
public:
	void bind();
	void unbind();
	void blur();
	
	void createFBO(const unsigned int w, const unsigned int h);
	void createShader();
	void debugDraw();
	int w;
	int h;
	Shader shader;
	GLuint tex;
	GLuint fbo;
	GLuint tex_rbo;
	GLuint depth_rbo;
	GLuint vao;
	GLuint vbo;

};

#endif