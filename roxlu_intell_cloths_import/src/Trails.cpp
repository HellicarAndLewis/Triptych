#include "Trails.h"

Trails::Trails() 
	:allocated_nodes(0)
	,next_node(0)
{
}

void Trails::setup() {
	glGenVertexArraysAPPLE(1, &trail_vao);
	glBindVertexArrayAPPLE(trail_vao);
	
	glGenBuffers(1, &trail_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, trail_vbo);
	
	shader.load("trails");
	shader.a("a_pos",0).a("a_color",1);
	shader.link();
	
	shader.u("u_projection_matrix").u("u_modelview_matrix");

	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArrayAPPLE(0);
	
	allocateBuffer();
}

void Trails::createTrail(deque<Vec3>& points, const float& agep) {
	size_t new_nodes =((points.size()-1) * 2);
	next_node += new_nodes;
	allocateBuffer();
	
	Trail trail;
	trail.createVertices(verts, points, agep);
	trails.push_back(trail);
	
}

void Trails::reset() {
	verts.clear();
	trails.clear();
	next_node = 0;
}

void Trails::update() {
	glBindBuffer(GL_ARRAY_BUFFER, trail_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, verts.numBytes(), verts.getPtr());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Trails::draw(const Mat4& pm, const Mat4& vm) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindVertexArrayAPPLE(trail_vao);
	
	shader.enable();
	shader.uniformMat4fv("u_projection_matrix", pm.getPtr());
	shader.uniformMat4fv("u_modelview_matrix", vm.getPtr());
	
	for(vector<Trail>::iterator it = trails.begin(); it != trails.end(); ++it) {
		Trail& tr = (*it);
		if(tr.start_index == 0 || tr.num_nodes == 0) {
			continue;
		}
		//if(tr.end_index > 0 || tr.start_index < 512 || tr.end_index < 512) {
		//	printf("%d - %d\n", tr.start_index, tr.num_nodes);
		
			//glDrawArrays(GL_POINTS, tr.start_index, tr.num_nodes);
			//glDrawArrays(GL_POINTS, tr.start_index, tr.num_nodes);
			glDrawArrays(GL_TRIANGLE_STRIP, tr.start_index, tr.num_nodes);
		//}
	}
	
	shader.disable();
	glBindVertexArrayAPPLE(0);
}


void Trails::allocateBuffer() {
	if(next_node >= allocated_nodes) {
		size_t to_allocate = std::max<size_t>(2 * allocated_nodes, 1024);
		allocated_nodes = to_allocate;
		printf("allocated for: %zu nodes.\n", allocated_nodes);
		
		// update vbo.
		size_t num_bytes = to_allocate * sizeof(VertexPC);
		glBindBuffer(GL_ARRAY_BUFFER, trail_vbo);
		glBufferData(GL_ARRAY_BUFFER, num_bytes, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);		
		
		// reset vao
		setVAO();
	}
}

void Trails::setVAO() {
	glBindVertexArrayAPPLE(trail_vao);
	glBindBuffer(GL_ARRAY_BUFFER, trail_vbo);
	
	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // col
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid*)offsetof(VertexPC, pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid*)offsetof(VertexPC, col));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArrayAPPLE(0);
}