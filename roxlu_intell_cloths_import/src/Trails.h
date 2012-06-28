#ifndef ROXLU_TRAILH
#define ROXLU_TRAILS

#include "Roxlu.h"
#include "Trail.h"
#include <vector>
#include <deque>

using std::deque;
using std::vector;

class Trails {
public:
	Trails();
	void setup();
	void createTrail(deque<Vec3>& points, const float& agep);
	void update();
	void draw(const Mat4& pm, const Mat4& vm);
	void reset();
	
	vector<Trail> trails;
	
	VerticesPC verts;
	Shader shader;
	size_t allocated_nodes;
	size_t next_node;
	GLuint trail_vbo;
	GLuint trail_vao;
	
private:
	void allocateBuffer();
	void setVAO();
};

	// trail.
	/*
	void setupTrails();
	void allocateTrailBytes();
	
	VerticesPC trail_vertices;
	
	*/


/*

// Trail 
// -------------------------------------------
void SphereUnit::setupTrails() {
	glGenVertexArraysAPPLE(1, &trail_vao);
	glBindVertexArrayAPPLE(trail_vao);
	
	glGenBuffers(1, &trail_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, trail_vbo);
	
	trail_shader.load("trails");
	trail_shader.a("a_pos",0).a("a_color",1);
	trail_shader.link();
	trail_shader.u("u_projection_matrix").u("u_modelview_matrix");

	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArrayAPPLE(0);
	
	allocateTrailBytes();
}
void SphereUnit::allocateTrailBytes() {
	if(allocated_trail_nodes == next_free_trail_node) {
		size_t to_allocate = std::max<size_t>(2*allocated_trail_nodes, 512);
		
		size_t num_bytes = to_allocate * sizeof(VertexPC);
		glBindVertexArrayAPPLE(trail_vao);
		glBindBuffer(GL_ARRAY_BUFFER, trail_vbo);
		glBufferData(GL_ARRAY_BUFFER, num_bytes, trail_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);		
		allocated_trail_nodes = to_allocate; 	
	}
}
*/


#endif