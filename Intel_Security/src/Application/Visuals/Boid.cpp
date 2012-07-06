#include "Boid.h"

Boid::Boid() {
}

void Boid::setup() {
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

	
}

void Boid::draw(const float* pm, const float* vm, const float* nm) {

}

void Boid::debugDraw() {

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

void Boid::update() {
}
