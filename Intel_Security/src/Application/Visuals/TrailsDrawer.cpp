#include "TrailsDrawer.h"

TrailsDrawer::TrailsDrawer(Boids& flockPS)
	:flock_ps(flockPS)
	,allocated_bytes(0)
{
}

void TrailsDrawer::setup() {
	shader.load("trails_drawer");
	
	shader	.a("a_pos", 0)
			.a("a_tex", 1)
			.link();
			
	shader	.u("u_projection_matrix")
			.u("u_modelview_matrix")
			.u("u_texture")
			.u("u_flow")
			.u("u_time");
			
	shader.disable();
	
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

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//glDepthMask(GL_FALSE);
	glColor3f(1,1,1);
	
	vao.bind();

	shader.enable();
	shader.uniformMat4fv("u_projection_matrix", pm);
	shader.uniformMat4fv("u_modelview_matrix", vm);
	
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
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

}

void TrailsDrawer::debugDraw() {
	if(settings.boid_create_trails) {
		VerticesPT trail_verts;
		for(Boids::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
			trail_verts.clear();
			Boid& b = **it;
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
	
	// update buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.numBytes(), vertices.getPtr());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//printf("Update! %zu\n", vertices.numBytes());
	return;
	
	// Update trail traingles
	/*
	trails.clear();
	for(Boids::iterator it = ps.begin(); it != ps.end(); ++it) {
		Boid& b = **it;
		if(b.trail.size() > 0) {
			// @todo make trails a Trails3PT
			//trails.addTrail(&b.trail);
		}
	}
	//trails.createTriangleStrips(3.0f, vertices);
			
	// fill buffer
	size_t needed = trails.getNumBytesForTriangleStrips();
	if(allocated_bytes < needed ) {
		while(true) {
			size_t to_allocate = std::max<size_t>(allocated_bytes * 2, 1024 * 512);
			allocated_bytes = to_allocate;
			if(to_allocate < needed) {	
				printf("We need more bytes for triangle buffer.\n");
			}
			else {
				break;
			}
		}
		printf("Allocated for trail buffer: %zu\n", allocated_bytes);
		
		vao.bind();
		// @todo fix, buffer is corrupt
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, allocated_bytes, vertices.getPtr(), GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0); // pos
		glEnableVertexAttribArray(1); // col
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid*)offsetof(VertexPC, pos));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid*)offsetof(VertexPC, col));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		vao.unbind();
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.numBytes(), vertices.getPtr());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	*/
}

