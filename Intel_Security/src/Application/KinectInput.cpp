#include "KinectInput.h"

/**
 * This class provides triangles from the kinect for 
 * the visualizer (and KinectDrawer). Input can be based on real time 
 * kinect or from a recorded file. When using a pre recorded kinect file
 * we do not initialize the kinect, so you can work and test w/o 
 * it; this gives a great speed up while developing.
 *
 * We can only work with one mode at a time.
 *
 * Set "use_recording" to true is you want to load the previously recorded
 * kinect triangles.
 *
 */
KinectInput::KinectInput()
	:is_updated(false)
	,use_recording(false)
{
}

void KinectInput::setup() {
	if(use_recording) {
		if(!recorder.load(roxlu::File::toDataPath("kinect.bin"))) {
			printf("Cannot use recording.\n");
			use_recording = false;
			setupInputFromKinect();			
		}
		else {	
			recorder.play();		
		}
	}	
	else {
		setupInputFromKinect();					
	}
	
	KinectMesh::setupGui();
}

void KinectInput::setupInputFromKinect() {
	kinect.setup();
	kinect.setupGui();
	kinect.setListener(this);
}

bool KinectInput::update() {
	is_updated = false;

	if(use_recording) {
		// GET FROM RECORDER
		if(!recorder.update()) {
			return false;
		}
		if(kmeshes.size() > 0) {
			kmeshes.clear();
		}
		
		KinectRecorderFrame& frame = recorder.getCurrentFrame();
		KinectMesh km;
		km.vertices = frame.vertices;
		km.outline = frame.outline;
		kmeshes.push_back(km);
		is_updated = true;
		return true;
	}
	else {
		// GET FROM KINECT	
		if(!kinect.update()) {
			return false;
		}
		interactive_points.clear();
		kinect.trackBlobs();
		contours.findContours(kinect.getOutline(), 30*30, 480*480, 20, false);
		kmeshes.clear();
		for(int i = 0; i < contours.blobs.size(); ++i) {
			KinectMesh km = KinectMesh();
			km.setup(contours.blobs[i], kinect);
			kmeshes.push_back(km);
			
			if(settings.must_record_kinect) {
				recorder.addFrame(km.vertices, km.outline);
			}
		}
		
		is_updated = true;
		return true;
		
	}
	return false;
}


void KinectInput::boundBlobEntered(const BoundBlob &blob) {
}

void KinectInput::boundBlobMoved(const BoundBlob &blob) {
	interactive_points.push_back(roxlu::Vec3(-0.5 + blob.left.x/640, 0.5 + -blob.left.y/480, 0.0));
}

void KinectInput::boundBlobExited(const BoundBlob &blob) {
	
}


