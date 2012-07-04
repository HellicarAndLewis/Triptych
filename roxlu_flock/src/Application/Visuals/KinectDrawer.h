#ifndef ROXLU_VISUALS_KINECTH
#define ROXLU_VISUALS_KINECTH


#include "ofMain.h"
#include "KinectMesh.h"
#include <deque>

class KinectDrawer {
public:
	KinectDrawer();
	void update(deque<KinectMesh>& kmeshes);
	void draw();
	void debugDraw();
	void debugDrawMesh(KinectMesh& m);
	deque<KinectMesh> tmp_meshes;
};

#endif