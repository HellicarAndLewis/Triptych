#include "TrailsDrawer.h"

TrailsDrawer::TrailsDrawer(Boids& flockPS)
	:flock_ps(flockPS)
	,allocated_bytes(0)
{
	trail_color[0] = 1.0f;
	trail_color[1] = 0.0f;
	trail_color[2] = 0.0f;
	trail_color[3] = 1.0f;
	
}

void TrailsDrawer::setup() {
	if(!shader.load("trails_drawer")) {
		printf("Cannot setup trails drawer shader.\n");
		::exit(0);
	}

	shader	.a("a_pos", 0)
			.a("a_tex", 1)
			.link();
	
	shader	.u("u_projection_matrix")
			.u("u_modelview_matrix")
			.u("u_texture")
			.u("u_flow")
			.u("u_time")
			.u("u_trail_color");

	shader.disable();
	
	vao.create();
	vao.bind();
	glGenBuffers(1, &vbo);
	vao.unbind();
	
	ofImage img;
	if(!img.loadImage("trail.png")) {
		printf("Error, cannot load trail texture; Update git?\n");
		::exit(0);
	}
	trail_tex.setWrap(GL_REPEAT);
	trail_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGB);
	
	if(!img.loadImage("flowmap.png")) {
		printf("Cannot load flowmap.png. Update git?");
		::exit(0);
	}
	
	flow_tex.setWrap(GL_REPEAT);
	flow_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGB);
}

void TrailsDrawer::draw(const float* pm, const float* vm) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glColor3f(1,1,1);

	vao.bind();

	shader.enable();
	shader.uniformMat4fv("u_projection_matrix", pm);
	shader.uniformMat4fv("u_modelview_matrix", vm);
	shader.uniform4fv("u_trail_color", trail_color);

	float time = Timer::millis() * 0.001;
	shader.uniform1f("u_time", time);

	glActiveTexture(GL_TEXTURE1);
	flow_tex.bind();
	shader.uniform1i("u_flow", 1);
	
	glActiveTexture(GL_TEXTURE2);
	trail_tex.bind();
	shader.uniform1i("u_texture", 2);
	
	for(Trails3PT::iterator it = trails.begin(); it != trails.end(); ++it) {	
		Trails3PT::Info ti = *it;
		glDrawArrays(GL_TRIANGLE_STRIP, ti.start_index, ti.num_elements);
	}
	
	vao.unbind();
	shader.disable();

	glDepthMask(GL_TRUE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

}

void TrailsDrawer::debugDraw() {
	if(settings.boid_create_trails) {
		VerticesPT trail_verts;
		for(Boids::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
			trail_verts.clear();
			Boid& b = **it;
			if(!b.is_visible) {
				continue;
			}
			if(b.trail.size() == 0) {
				continue;
			}
			b.trail.createTriangleStripPT(settings.boid_trail_width, trail_verts);
			glBegin(GL_TRIANGLE_STRIP);
				for(int i = 0; i < trail_verts.size(); ++i) {
					glVertex3fv(trail_verts[i].getPtr());
				}
			glEnd();
		}
	}
}

void TrailsDrawer::update() {
	// @todo check buffer alloc 
	trails.clear();
	vertices.clear();
	
	// Add all trails to the trail creator
	for(Boids::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
		Boid& b = **it;
		if(!b.is_visible) {
			continue;
		}
		if(b.trail.size() > 0) {
			trails.addTrail(&b.trail);
		}
	}
	
	trails.createTriangleStrips3PT(settings.boid_trail_width, vertices);
	
	// check if we need to grow our VBO
	size_t needed_bytes = trails.getNumBytesForTriangleStrips();
	if(needed_bytes <= 0) {
		return;
	}
	
	if(needed_bytes > allocated_bytes) {
		size_t to_allocate = 0;
		while(allocated_bytes < needed_bytes) {
			allocated_bytes = std::max<size_t>(allocated_bytes * 2, 4096);
		}
		printf("TrailsDrawer::update() - allocate: %un",(unsigned int)allocated_bytes);
		 
		vao.bind();
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, allocated_bytes, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0); // pos
		glEnableVertexAttribArray(1); // tex
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid*)offsetof(VertexPT, pos));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid*)offsetof(VertexPT, tex));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		vao.unbind();
	}
	if(vertices.numBytes() <= 0) {
		return;
	}
	// update buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.numBytes(), vertices.getPtr());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return;
}

