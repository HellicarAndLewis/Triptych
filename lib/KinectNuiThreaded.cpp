#include "KinectNuiThreaded.h"
#include "TimeProfiler.h"
using namespace tricks::util;

KinectNuiThreaded::KinectNuiThreaded() {
	rgb = NULL;
	depth = NULL;
}


void KinectNuiThreaded::setup(bool useSkeletons) {
	ofxKinectNui::InitSetting initSetting;
	initSetting.grabVideo = true;
	initSetting.grabDepth = true;
	initSetting.grabAudio = false;
	initSetting.grabLabel = false;
	initSetting.grabSkeleton = useSkeletons;
	initSetting.grabCalibratedVideo = false;
	initSetting.grabLabelCv = false;
	initSetting.depthResolution = NUI_IMAGE_RESOLUTION_640x480;
	initSetting.videoResolution = NUI_IMAGE_RESOLUTION_640x480;
	kinect.init(initSetting);
	kinect.open(false);
	doingSkeletons = useSkeletons;
	gotNewFrame = false;

	numSkeletons = 0;
	rgb = new unsigned char[640*480*3];
	depth = new unsigned char[640*480];

}

KinectNuiThreaded::~KinectNuiThreaded() {
	if(rgb!=NULL) delete rgb;
	if(depth!=NULL) delete depth;
}


void KinectNuiThreaded::update() {
	if(isThreadRunning()) {
		gotNewFrame = false;
	} else {

		ScopedTimer timer("Kinect Locked Copy");
		gotNewFrame = true;
		
		lock();
		// copy skeleton, depth and rgb to threadsafe buffer
		memcpy(depth, kinect.getDepthPixels().getPixels(), 640*480);
		memcpy(rgb, kinect.getVideoPixels().getPixels(), 640*480*3);
<<<<<<< HEAD
		if(doingSkeletons) {

			numSkeletons = kinect.getSkeletonPoints(skeletonPoints);
			
		}
=======
		
>>>>>>> cb5d56879cb0fbbd2dca7928ff5e1a3a5827d0a9
		unlock();


		startThread(true, false); 
	}
}

bool KinectNuiThreaded::isFrameNew() {
	return gotNewFrame;
}

int KinectNuiThreaded::getSkeletonPoints(const ofPoint *points[]) {
	lock();
	for(int i = 0; i < numSkeletons; i++) {
		points[i] = skeletonPoints[i];
	}
	int ns = numSkeletons;
	unlock();
	return ns;
}

unsigned char *KinectNuiThreaded::getPixels() {

	return rgb;
}
unsigned char *KinectNuiThreaded::getDepthPixels() { 
	return depth;
}

void KinectNuiThreaded::threadedFunction() {


	ScopedTimer timer("Kinect thread\n");
	int flags = ofxKinectNui::UPDATE_FLAG_SKELETON;
	if(!doingSkeletons) flags = 0;
	kinect.update(ofxKinectNui::UPDATE_FLAG_VIDEO	
		| ofxKinectNui::UPDATE_FLAG_DEPTH	
		| flags
		//| ofxKinectNui::UPDATE_FLAG_CALIBRATED_VIDEO
		);

	lock();
	if(doingSkeletons) {
		numSkeletons = kinect.getSkeletonPoints(skeletonPoints);
	}
	unlock();
}