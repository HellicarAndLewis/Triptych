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
	,input_mode(K_INPUT_KINECT)
{
}

void KinectInput::setup() {
	switch(input_mode) {
		case K_INPUT_RECORDING: {
			if(!recorder.load(roxlu::File::toDataPath("kinect.bin"))) {
				printf("Cannot use recording.\n");
				setupInputFromKinect();			
			}
			else {	
				recorder.play();		
			}
			break;
		}
		case K_INPUT_KINECT: {
			setupInputFromKinect();					
			KinectMesh::setupGui();
			break;
		}
		case K_INPUT_NONE: {
			break;
		}
	};
}

void KinectInput::setupInputFromKinect() {
	kinect.setup();
	kinect.setupGui();
	kinect.addListener(this);
}

bool KinectInput::update() {
	is_updated = false;

	switch(input_mode) {	
		// GET FROM RECORDER
		case K_INPUT_RECORDING: {
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
			break;
		}
		
		// GET FROM KINECT	
		case K_INPUT_KINECT: {
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
			break;
		};
	
		// ONLY SIMULATE
		case K_INPUT_NONE: {
			float y = sin(Timer::millis() * 0.001 * 0.3) * 1.7;
			float z = cos(Timer::millis() * 0.001 * 0.3) * 1.7;
			Vec3 pos(0, y, z);
			interactive_points.clear();
			interactive_points.push_back(pos);
			interactive_points.push_back(pos * -1);
			break;
			return true;
		}
		
	};
	
	return false;
}


void KinectInput::boundBlobEntered(const BoundBlob &blob) {
}

void KinectInput::boundBlobMoved(const BoundBlob &blob) {
//	printf("left: %f, %f\n", blob.left.x, blob.left.y);
//	printf("right: %f, %f\n", blob.right.x, blob.right.y);
	float x = -0.5 + blob.right.x/640;
	float y = 0.5 + -blob.right.y/480;
	
	x *= settings.kinect_scale;
	y *= settings.kinect_scale;
	
	interactive_points.push_back(roxlu::Vec3(x, y, 0.0));
}

void KinectInput::boundBlobExited(const BoundBlob &blob) {
	
}


