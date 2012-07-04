#include "KinectDrawer.h"

KinectDrawer::KinectDrawer() {

}

void KinectDrawer::update(deque<KinectMesh>& kmeshes) {
	tmp_meshes = kmeshes; // just testing	
}


void KinectDrawer::draw() {
}


void KinectDrawer::debugDraw() {
	for(int i = 0; i < tmp_meshes.size(); ++i) {
		debugDrawMesh(tmp_meshes[i]);
	}
}

void KinectDrawer::debugDrawMesh(KinectMesh& m) {
	glBegin(GL_TRIANGLES);
	for(vector<KinectTriangle>::iterator it = m.triangles.begin(); it != m.triangles.end(); ++it) {
		KinectTriangle& tri = *it;
		if(tri.hollow) {
			continue;
		}
		
		glColor3fv(&tri.colors[0].r);;
		glVertex2fv(tri.points[0].getPtr());
		glVertex2fv(tri.points[1].getPtr());
		glVertex2fv(tri.points[2].getPtr());
		//printf("%f, f\n", tri.points[0][0], tri.points[0][1
	}
	glEnd();
}
