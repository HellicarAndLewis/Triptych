#ifndef ROXLU_KINECT_INPUTH
#define ROXLU_KINECT_INPUTH


#include "KinectThresholder.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"

#include <vector>
#include <deque>

class KinectInput {
public:

	void setup();
	bool update();
	deque<KinectMesh>& getKinectMeshes();
	bool isUpdated();
	
	ofxCvContourFinder contours;
	KinectThresholder kinect;
	deque<KinectMesh> kmeshes;
	bool is_updated;
};

inline bool KinectInput::isUpdated() {
	return is_updated;
}

inline deque<KinectMesh>& KinectInput::getKinectMeshes() {
	return kmeshes;
}


#endif 