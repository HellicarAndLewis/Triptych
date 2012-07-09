#include "ofMain.h"
#include "Blur.h"

void Blur::createShader() {
	
	shader.load("blur");
	shader.a("pos", 0).a("tex",1);
	shader.link();
	shader.u("u_modelview_matrix").u("u_projection_matrix").u("u_texture").u("u_mode").u("u_width").u("u_height");
	
	BlurVertex vertices[6];
	float s = 1.0f;
	vertices[0].set(-s, -s, 0.0f, 0.0f);
	vertices[1].set(s, -s, 1.0f, 0.0f);
	vertices[2].set(s, s, 1.0f, 1.0f);
	
	vertices[3].set(-s, -s, 0.0f, 0.0f);
	vertices[4].set(s, s, 1.0f, 1.0f);
	vertices[5].set(-s, s, 0.0f, 1.0f);
	
	
	glGenVertexArraysAPPLE(1, &vao); eglGetError();
	glBindVertexArrayAPPLE(vao); eglGetError();
	
	glGenBuffers(1, &vbo); eglGetError();
	glBindBuffer(GL_ARRAY_BUFFER, vbo); eglGetError();
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlurVertex) * 6, &(vertices[0].pos[0]), GL_STATIC_DRAW);
		
	glEnableVertexAttribArray(0); eglGetError();
	glEnableVertexAttribArray(1);eglGetError();
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlurVertex), (GLvoid*)offsetof(BlurVertex, pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BlurVertex), (GLvoid*)offsetof(BlurVertex, tex));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArrayAPPLE(0);
	shader.disable();
}


void Blur::createFBO(const unsigned int w, const unsigned int h) {
	
	glGenFramebuffers(1, &fbo); eglGetError();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); eglGetError();

	glEnable(GL_TEXTURE_2D); eglGetError();
	glGenTextures(1, &tex); eglGetError();
	glActiveTexture(GL_TEXTURE0); eglGetError();
	glBindTexture(GL_TEXTURE_2D, tex); eglGetError();
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); eglGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); eglGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); eglGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); eglGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); eglGetError();
		
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0); eglGetError();
	
	glGenRenderbuffers(1, &depth_rbo); eglGetError();
	glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo); eglGetError();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);eglGetError();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo); eglGetError();


	GLenum fbo_buff[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, fbo_buff); eglGetError();

	// checking statii	
	eglCheckFramebufferStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	this->w = w;
	this->h = h;
}

void Blur::bind() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); eglGetError();
	glClearColor(0,0,0,1.0);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Blur::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Blur::blur() {
	glColor3f(1,1,1);
	glColorMask(true, true, true, true);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	
	shader.enable();
	glEnable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	shader.uniform1i("u_texture", 0);
	
	shader.uniform1i("u_mode", 0);
	shader.uniform1f("u_height", h);
	shader.uniform1f("u_width", w);
	
	Mat4 ident;
	shader.uniformMat4fv("u_modelview_matrix", ident.getPtr());
	shader.uniformMat4fv("u_projection_matrix", ident.getPtr());
	
	glBindVertexArrayAPPLE(vao); eglGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6); eglGetError();
		
	shader.uniform1i("u_mode", 1);
	glDrawArrays(GL_TRIANGLES, 0, 6); eglGetError();
	
	glBindVertexArrayAPPLE(0);
	shader.disable();
}


void Blur::debugDraw() {
	glColorMask(true, true, true, true);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_COLOR);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glUseProgram(0);
	
	glPointSize(10);
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
		glVertex3f(0.8,0,0);
	glEnd();

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D); 
 	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, tex); eglGetError();
	glColor3f(1,1,1);
	
	float ww;
	float hh;
	ww = hh = 1.0;
	glBegin(GL_QUADS); 
		glTexCoord2f(0,0); glVertex3f(-ww,-hh, -1.0); 
		glTexCoord2f(1,0); glVertex3f(ww,-hh, -1.0); 
		glTexCoord2f(1,1); glVertex3f(ww,hh, -1.0);  
		glTexCoord2f(0,1); glVertex3f(-ww,hh, -1.0); 
	glEnd(); eglGetError();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

}