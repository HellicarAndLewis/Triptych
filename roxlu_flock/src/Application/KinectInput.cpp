#include "KinectInput.h"

void KinectInput::setup() {

	kinect.setup();
	kinect.setupGui();
	KinectMesh::setupGui();
}

bool KinectInput::update() {
	is_updated = false;
	if(kinect.update()) {
		contours.findContours(kinect.getOutline(), 30*30, 480*480, 20, false);

		kmeshes.clear();
		for(int i = 0; i < contours.blobs.size(); ++i) {
			kmeshes.push_back(KinectMesh());
			kmeshes.back().setup(contours.blobs[i], kinect);
		}
		is_updated = true;
		return true;
//		// store 100 sets of meshes of history.
//		if(meshes.size()>100) {
//			meshes.pop_back();
//		}
//		
//		meshes.push_front(vector<KinectMesh>());
//		
//		//meshes.clear();
//		for(int i = 0; i < contours.blobs.size(); i++) {
//		
//			
//			meshes.front().push_back(KinectMesh());
//			if(!meshes.front().back().setup(contours.blobs[i], kinect)) {
//				meshes.front().pop_back();
//
//			}
//		}
	}
	return false;
}



