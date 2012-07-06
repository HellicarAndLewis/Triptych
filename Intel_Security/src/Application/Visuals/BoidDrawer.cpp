#include "BoidDrawer.h"

BoidDrawer::BoidDrawer() {
}

void BoidDrawer::setup() {
	// Load boid data from obj file.
	OBJ obj;
	
	if(!obj.import(File::toDataPath("boid.obj"))) {
		printf("Cannot load boid obj.\n");
		::exit(0);
	}
	obj.print();
	
	vd = obj["boid"];
	if(vd.getNumVertices() == 0) {
		printf("No vertices found for boid.");
		::exit(0);
	}
	printf("Num vertices in boid: %zu\n", vd.getNumVertices());	
	
	for(int i = 0; i < vd.getNumTriangles(); ++i) {
		Triangle& tri = vd.triangles[i];
		vertices.add(VertexP(vd[tri.va]));
		vertices.add(VertexP(vd[tri.vb]));
		vertices.add(VertexP(vd[tri.vc]));
	}
	//printf("%d ...\n", vd.getNumTriangles());
	shader.load("boid_drawer");
	shader.a("a_pos",0);
	shader.link();
	shader.u("u_modelview_matrix").u("u_projection_matrix");
	
	vao.create();
	vao.bind();
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.numBytes(), vertices.getPtr(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), (GLvoid*)offsetof(VertexP, pos));
	
	vao.unbind();
	shader.disable();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//void BoidDrawer::draw(const float* pm, const float* vm, const float* nm) {
void BoidDrawer::draw(const float* pm, const float* vm, const float* nm, const float* coordinateSystem) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	vao.bind();
		shader.enable();
		shader.uniformMat4fv("u_modelview_matrix", vm);
		shader.uniformMat4fv("u_projection_matrix", pm);
		
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
		shader.disable();
	vao.unbind();
}

void BoidDrawer::debugDraw(const Mat3& cs) {
	float s = 0.3f;
	Vec3 ax;
	glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3fv(position.getPtr());  ax = cs.getXAxis(); glVertex3fv((position+ax*s).getPtr());
		glColor3f(0,1,0); glVertex3fv(position.getPtr());  ax = cs.getYAxis(); glVertex3fv((position+ax*s).getPtr());
		glColor3f(0,0,1); glVertex3fv(position.getPtr());  ax = cs.getZAxis(); glVertex3fv((position+ax*s).getPtr());
	glEnd();

	//glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glColor3f(1,1,0);
	glPointSize(1.0);
	glPushMatrix();
		static int t = 0;
		++t;
		glRotatef(t, 0,1,0.5);
		vd.debugDraw();

		glBegin(GL_TRIANGLES);
		for(int i =0; i < vd.getNumVertices(); ++i) {
			glVertex3fv(vd[i].getPtr());
		}
		glEnd();
		glColor3f(1,1,1);
	glPopMatrix();
}

void BoidDrawer::update() {
}
