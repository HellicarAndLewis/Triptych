#include "ExplosionTrails.h"

ExplosionTrails::ExplosionTrails(Boids& ps)
	:ps(ps)
	,allocated_bytes(0)
{
}

void ExplosionTrails::setup() {
	shader.load("explosion_trail");
	shader.a("a_pos", 0).a("a_col", 1).link();
	shader.u("u_projection_matrix").u("u_modelview_matrix");
	shader.disable();
	
	vao.bind();
	glGenBuffers(1, &vbo);
	vao.unbind();
}

void ExplosionTrails::draw(const float* pm, const float* vm) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	vao.bind();

	shader.enable();
	shader.uniformMat4fv("u_projection_matrix", pm);
	shader.uniformMat4fv("u_modelview_matrix", vm);
	
	for(Trails2PC::iterator it = trails.begin(); it != trails.end(); ++it) {	
		Trails2PC::Info ti = *it;
		glDrawArrays(GL_TRIANGLE_STRIP, ti.start_index, ti.num_elements);
	}
	
	vao.unbind();
	shader.disable();
}

void ExplosionTrails::debugDraw() {
	::draw(trails);
}

void ExplosionTrails::update() {
	// @todo check buffer alloc 
	return;
	
	// Update trail traingles
	trails.clear();
	for(Boids::iterator it = ps.begin(); it != ps.end(); ++it) {
		Boid& b = **it;
		if(b.trail.size() > 0) {
			trails.addTrail(&b.trail);
		}
	}
	trails.createTriangleStrips(3.0f, vertices);
			
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
}

