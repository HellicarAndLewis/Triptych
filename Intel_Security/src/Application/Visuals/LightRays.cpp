#include "LightRays.h"
#include <roxlu/opengl/Error.h>

LightRays::LightRays() 
	:w(0)
	,h(0)
	,tex(0)
	,fbo(0)
	,depth_buffer(0)
	,frag_shader(0)
	,vert_shader(0)
	,prog(0)
	,vbo(0)
	,vao(0)
	,exposure(0.0034f)
	,decay(0.9999f)
	,density(0.84f)
	,weight(5.56f)
	,light_x(0.0)
	,light_y(0.0)
{
	
}

void LightRays::createTexture(int texW, int texH, GLuint& texID) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(
		GL_TEXTURE_2D
		,0
		,GL_RGBA
		,texW
		,texH
		,0
		,GL_RGBA
		,GL_UNSIGNED_BYTE
		,NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void LightRays::setup(int w, int h) {
	this->w = w;
	this->h = h;
	this->dw = w;
	this->dh = h;

	// Create our fbo.
	// ++++++++++++++++++++++++++++++++++++
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	// render to this texture.	
	createTexture(w,h,tex);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	
	
	// apply fx into this texture.	
	createTexture(w,h,fx_tex);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fx_tex, 0);
	
	// create depth buffer.
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	// Create shader
	// ++++++++++++++++++++++++++++++++++++
	const char* vsp = LR_VS.c_str();
	const char* fsp = LR_FS.c_str();
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &(vsp), NULL);
	glShaderSource(frag_shader, 1, &(fsp), NULL);
	glCompileShader(vert_shader);
	glCompileShader(frag_shader);

	GLint result;
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result);
	if(!result) {
		printf("Cannot compile vertex shader.\n");	
	}
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &result);
	if(!result) {
		printf("Cannot compile fragment shader.\n");	
	}
	
	prog = glCreateProgram();
	glAttachShader(prog, vert_shader);
	glAttachShader(prog, frag_shader);
	
	glBindAttribLocation(prog, 0, "a_pos");
	glBindAttribLocation(prog, 1, "a_tex");
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &result);
	if(!result) {
		printf("Cannot link shaders.\n");
	}
	

	// Create VAO + VBO for full screen quad
	// ++++++++++++++++++++++++++++++++++++++
	LightRayVertex verts[4];
	float s = 1.0f;
//	verts[0].set(-s, s, 0, 0, 0);
//	verts[1].set( s, s, 0, 1, 0);
//	verts[2].set( s,-s, 0, 1, 1);
//	verts[3].set(-s,-s, 0, 0, 1);
	verts[0].set(-s, s, 0, 0, 1);
	verts[1].set( s, s, 0, 1, 1);
	verts[2].set( s,-s, 0, 1, 0);
	verts[3].set(-s,-s, 0, 0, 0);
	

	#ifdef _WIN32 
		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);
	#elif defined(__APPLE__)
		glGenVertexArraysAPPLE(1,&vao);
		glBindVertexArrayAPPLE(vao);
	#else
		#error I need to figure out, how to create a vertex array on your system?
	#endif

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LightRayVertex) * 4, verts[0].getPtr(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // tex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LightRayVertex), (GLvoid*)offsetof(LightRayVertex, pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(LightRayVertex), (GLvoid*)offsetof(LightRayVertex, tex));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifdef _WIN32
	glBindVertexArray(0);
#elif define(__APPLE__)
	glBindVertexArrayAPPLE(0);
#else
#error Cannot compile LightRays on this OS
#endif
}

void LightRays::resize(int w, int h) {
	this->w = w;
	this->h = h;
	printf("New w/h: %d, %d\n", w,h);
}

void LightRays::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
//	GLenum bfs[] = {GL_COLOR_ATTACHMENT0};
	//glDrawBuffers(1, bfs);
	glViewport(0,0,this->dw,this->dh);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void LightRays::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,this->w,this->h);
}

void LightRays::draw() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//	int bfs = {GL_COLOR_ATTACHMENT1};
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
//	GLenum bfs[] = {GL_COLOR_ATTACHMENT1};
//	glDrawBuffers(1, bfs);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,this->dw,this->dh);
	
	// bind
	#ifdef _WIN32
		glBindVertexArray(vao);
	#elif defined(__APPLE__) 
		glBindVertexArrayAPPLE(vao);
	#else
		#error Don't now how to draw on your machine (1)
	#endif
	
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
		
	// set shader vars.
	glUseProgram(prog);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(prog, "u_texture"), 1);
	glUniform1f(glGetUniformLocation(prog, "u_exposure"), exposure);
	glUniform1f(glGetUniformLocation(prog, "u_decay"), decay);
	glUniform1f(glGetUniformLocation(prog, "u_density"), density);
	glUniform1f(glGetUniformLocation(prog, "u_weight"), weight);
	glUniform1f(glGetUniformLocation(prog, "u_light_x"), light_x);
	glUniform1f(glGetUniformLocation(prog, "u_light_y"), light_y);
	
	
	// apply effect
	glUniform1i(glGetUniformLocation(prog, "u_mode"), 0);
	glDrawArrays(GL_QUADS, 0, 4);

	
	// draw full screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fx_tex);
	
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);

	glViewport(0,0,this->w,this->h);
	glUniform1i(glGetUniformLocation(prog, "u_mode"), 1);
	glDrawArrays(GL_QUADS, 0, 4);

	glDepthMask(GL_TRUE);

	// unbind
	#ifdef _WIN32
		glBindVertexArray(0);
	#elif defined(__APPLE__) 
		glBindVertexArrayAPPLE(0);
	#else
		#error Don't now how to draw on your machine (2)
	#endif
		
	glUseProgram(0);
	
	// just make sure 0 and 1 unit are unbound.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

}


