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
#include <roxlu/experimental/Timer.h>

#include <vector>
#include <deque>


using namespace roxlu;

class KinectInput : public BoundBlobListener {
public:
	enum KinectInputModes {
		 K_INPUT_NONE
		,K_INPUT_RECORDING
		,K_INPUT_KINECT
	};
	
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
	int input_mode;
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