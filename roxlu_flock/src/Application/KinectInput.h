#ifndef ROXLU_KINECT_INPUTH
#define ROXLU_KINECT_INPUTH


#include "KinectThresholder.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"

#include <roxlu/io/File.h>
#include <application/KinectRecorder.h>
#include <application/Settings.h>

#include <vector>
#include <deque>

class KinectInput {
public:
	KinectInput();
	void setup();
	bool update();
	deque<KinectMesh>& getKinectMeshes();
	bool isUpdated();
	
	KinectRecorder recorder;
	ofxCvContourFinder contours;
	KinectThresholder kinect;
	deque<KinectMesh> kmeshes;
	bool is_updated;
	bool use_recording;
	
private:
	void setupInputFromKinect();
	
};

inline bool KinectInput::isUpdated() {
	return is_updated;
}

inline deque<KinectMesh>& KinectInput::getKinectMeshes() {
	return kmeshes;
}


#endif 