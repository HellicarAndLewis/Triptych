//#include "Cloak.h"
#include <application/visuals/Cloak.h>

Cloak::Cloak(Particles3& ps, vector<Triangle>& tris) 
	:ps(ps)
	,tris(tris)
{
	printf("Cloack.\n");
}

void Cloak::setup() {

	shader.load("cloak");
	shader.a("a_pos",0).a("a_col",1).a("a_norm", 2).link();
	shader.u("u_projection_matrix").u("u_modelview_matrix").u("u_normal_matrix");

	vao.create();
	vao.bind();
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	vertices.resize(tris.size() * 3);
	
	Vec4 col(1.0, 0.0, 1.0, 1.0);
	Vec3 norm(0,0,1);
	for(int i = 0; i < tris.size(); ++i) {
		Triangle& tri = tris[i];
		vertices.add(VertexPNC(ps[tri.va]->position, norm, col));	
		vertices.add(VertexPNC(ps[tri.vb]->position, norm, col));	
		vertices.add(VertexPNC(ps[tri.vc]->position, norm, col));	
		
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.numBytes(), vertices.getPtr(), GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // col
	glEnableVertexAttribArray(2); // norm
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNC), (GLvoid*)offsetof(VertexPNC,pos));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPNC), (GLvoid*)offsetof(VertexPNC,col));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNC), (GLvoid*)offsetof(VertexPNC,norm));
	
	vao.unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void Cloak::update() {
	// flatten the cloth triangles.
	vertices.clear();
	vertices.resize(tris.size() * 3);
	Vec4 col(1.0f, 0.0f, 1.0f, 1.0f);
	Vec3 norm(1.0f, 1.0f, 1.0f);
//	Vec3 up(0,1,0);
	for(int i = 0; i < tris.size(); ++i) {
		Triangle& tri = tris[i];
		
		Vec3 a = ps[tri.va]->position;
		Vec3 b = ps[tri.vb]->position;
		Vec3 c = ps[tri.vc]->position;
		
		Vec3 ab = b-c;
		Vec3 ac = c-a;
		norm = cross(ab, ac).getNormalized();
		
		vertices.add(VertexPNC(a, norm, col));	
		vertices.add(VertexPNC(b, norm, col));	
		vertices.add(VertexPNC(c, norm, col));	
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.numBytes(), vertices.getPtr());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void Cloak::draw(const Mat4& pm, const Mat4& vm, const Mat3& nm) {
	
	vao.bind();
	
	shader.enable();
	shader.uniformMat4fv("u_projection_matrix", pm.getPtr());
	shader.uniformMat4fv("u_modelview_matrix", vm.getPtr());
	shader.uniformMat3fv("u_normal_matrix", nm.getPtr());
	
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
	vao.unbind();
	shader.disable();
}

void Cloak::debugDraw() {
	glPointSize(30);
	
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < tris.size(); ++i) {
		Triangle& tri = tris[i];
		glVertex2fv(ps[tri.va]->position.getPtr());
		glVertex2fv(ps[tri.vb]->position.getPtr());
		glVertex2fv(ps[tri.vc]->position.getPtr());
		
	}
	glEnd();

	glColor3f(0.5,0.5,0.5);	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < tris.size(); ++i) {
		Triangle& tri = tris[i];
		glVertex2fv(ps[tri.va]->position.getPtr());
		glVertex2fv(ps[tri.vb]->position.getPtr());
		glVertex2fv(ps[tri.vc]->position.getPtr());
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glColor3f(1,1,1);

}



