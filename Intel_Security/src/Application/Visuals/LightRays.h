#ifndef ROXLU_LIGHTRAYSH
#define ROXLU_LIGHTRAYSH

/**
 
Drop in light rays
------------------
Basic usage:
- create a member "LightRays lr", then

// setup()
lr.setup(width, height);

// call every draw()
lr.bind();
  // draw your scene with 'white' lights and black occulding objects
lr.unbind()
lr.draw() 

// resize
lr.resize(w, h);

*/

#include <roxlu/opengl/OpenGL.h>
#include <string>
const std::string LR_VS = " \
	attribute vec4 a_pos; \
	attribute vec2 a_tex; \
	varying vec2 v_tex; \
	\
	void main() { \
		mat4 ident = mat4(1.0); \
		v_tex = a_tex; \
		gl_Position = ident * a_pos; \
	}\
";

const std::string LR_FS = " \
	uniform int u_mode;\n \
	uniform sampler2D u_texture;\n \
	uniform float u_exposure;\n \
	uniform float u_decay;\n \
	uniform float u_density;\n \
	uniform float u_weight;\n \
	uniform float u_light_x;\n \
	uniform float u_light_y;\n \
	const int NUM_SAMPLES = 50;\n \
	varying vec2 v_tex;\n \
	void main() {\n \
		if (u_mode == 0) {\n \
			gl_FragColor.rgb = vec3(0.0, 0.0, 0.0); \
			vec2 pos_on_screen = vec2(u_light_x, u_light_y);\n \
			vec2 delta_texc = vec2(v_tex.st - pos_on_screen.xy);\n \
			vec2 texc = v_tex; \n\
			delta_texc *= 1.0 / float(NUM_SAMPLES) * u_density;\n \
			float illum_decay = 1.0;\n \
			for(int i = 0; i < NUM_SAMPLES; i++) {\n \
				texc -= delta_texc;  \n\
				vec4 sample = texture2D(u_texture, texc);\n \
				sample *= illum_decay * u_weight; \n\
				gl_FragColor += sample; \n\
				illum_decay *= u_decay; \n\
			}\n \
			gl_FragColor *= u_exposure; \n  \
		} \n\
		else if(u_mode == 1) { \n\
			gl_FragColor = texture2D(u_texture, v_tex);\n \
			gl_FragColor.a = 1.0;\n \
		} \n\
		\n\
	}\n ";
	
struct LightRayVertex {
	float pos[3];
	float tex[2];
	
	LightRayVertex(){
		pos[0] = pos[1] = pos[2] = 0.0f;
		tex[0] = tex[1] = 0.0f;
	}
	
	LightRayVertex(const float x, const float y, const float z, const float s, const float t)	{
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
		tex[0] = s;
		tex[1] = t;
	}
	
	void set(const float x, const float y, const float z, const float s, const float t) {
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
		tex[0] = s;
		tex[1] = t;
	}
	
	const float* getPtr() {
		return &pos[0];
	}
};

class LightRays {
public:
	LightRays();
	void setup(int w, int h);
	void resize(int w, int h);
	void bind();
	void unbind();
	void draw();
	
	void createTexture(int texW, int texH, GLuint& texID);
	
	float exposure;
	float decay;
	float density;
	float weight;
	float light_x;
	float light_y;
	
	int w;
	int h;
	int dw;
	int dh;
	GLuint fbo;
	GLuint tex;
	GLuint fx_tex;
	GLuint depth_buffer;
	GLuint vbo;
	GLuint vao;
	GLuint prog;
	GLuint frag_shader;
	GLuint vert_shader;
};
#endif