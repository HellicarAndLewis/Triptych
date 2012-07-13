#ifndef ROXLU_LIGHTRAYSH
#define ROXLU_LIGHTRAYSH

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
	uniform int u_mode; \
	uniform sampler2D u_texture; \
	uniform float u_exposure; \
	uniform float u_decay; \
	uniform float u_density; \
	uniform float u_weight; \
	uniform float u_light_x; \
	uniform float u_light_y; \
	const int NUM_SAMPLES = 50; \
	varying vec2 v_tex; \
	void main() { \
		if (u_mode == 0) { \
			vec2 pos_on_screen = vec2(u_light_x, u_light_y); \
			vec2 delta_texc = vec2(v_tex.st - pos_on_screen.xy); \
			vec2 texc = v_tex; \
			delta_texc *= 1.0 / float(NUM_SAMPLES) * u_density; \
			float illum_decay = 1.0; \
			for(int i = 0; i < NUM_SAMPLES; i++) { \
				texc -= delta_texc; \
				vec4 sample = texture2D(u_texture, texc); \
				sample *= illum_decay * u_weight; \
				gl_FragColor += sample; \
				illum_decay *= u_decay; \
			} \
			gl_FragColor *= u_exposure; \
		} \
		else if(u_mode == 1) { \
			gl_FragColor = texture2D(u_texture, v_tex); \
			gl_FragColor.a = 1.0; \
		} \
		\
	} ";
	
//		gl_FragColor = vec4(1.0, 0.1, 0.1, 1.0); 

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