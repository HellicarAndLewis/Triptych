#ifndef ROXLU_KINECT_INPUTH
#define ROXLU_KINECT_INPUTH


#include "BoundBlobListener.h"
#include "KinectThresholder.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"

#include <roxlu/io/File.h>
#include <roxlu/math/Vec3.h>
#include <application/KinectRecorder.h>
#include <application/Settings.h>

#include <vector>
#include <deque>


class KinectInput : public BoundBlobListener {
public:
	KinectInput();
	void setup();
	bool update();
	bool isUpdated();
	deque<KinectMesh>& getKinectMeshes();
		
	// implementing blob listener
	void boundBlobEntered(const BoundBlob &blob);
	void boundBlobMoved(const BoundBlob &blob);
	void boundBlobExited(const BoundBlob &blob);	
		
	KinectRecorder recorder;
	ofxCvContourFinder contours;
	KinectThresholder kinect;
	deque<KinectMesh> kmeshes;
	bool is_updated;
	bool use_recording;

	vector<roxlu::Vec3> interactive_points;
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