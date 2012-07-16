#pragma once

#include "ofMain.h"
#include "KinectThresholder.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"
#include "Room.h"
#include "Bloom.h"
#include "Ribbon.h"
#include "NuiSensor.h"
#include "AudioSystem.h"

class RibbonSkeleton {
public:

	vector<Ribbon*> ribbons;
	KinectSkeleton skeleton;
	bool alive;

	void setup(KinectSkeleton skel) {
		alive = true;

		//NB by we are zeroing z
		ofVec3f factor = ofVec3f((float)ofGetWidth()/640.f, (float)ofGetHeight()/480.f, 0);

		for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++) {
			//we are adding all the points on the skeleton
			ribbons.push_back(new Ribbon(skel.points[i] * factor));
		}

		update(skel);
	}

	void update(KinectSkeleton skel) {

		this->skeleton = skel;
		
		// we want to rescale to screen coords - we also want to remove the z component
		// (for now)
		ofVec3f factor = ofVec3f((float)ofGetWidth()/640.f, (float)ofGetHeight()/480.f, 0);

		for (int  i = 0; i < ribbons.size(); i++) {
			ribbons[i]->update(skeleton.points[i] * factor);
		}
	}

	void setColor(ofFloatColor col) {

		for (int  i = 0; i < ribbons.size(); i++) {
			ribbons[i]->setColour(col);
		}
	}

	void draw() {

		for (int  i = 0; i < ribbons.size(); i++) {
			ribbons[i]->draw();
		}
	}
	
};


class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	AudioSystem audioSystem;
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void updateFromSkeletons();
	Room room;
	
	KinectThresholder kinect;
	
	vector<KinectMesh> meshes;
	

	// these are the skeletons currently on the scene
	map<int,RibbonSkeleton> skeletons;
	
	// these are the ribbons that have been disassocated with any skeleton.
	vector<Ribbon*> deadRibbons;

	tricks::gl::effects::Bloom bloom;

	ofFbo brushFbo;
	float backgroundAlpha, imageAlpha;
	
	vector< vector<ofFloatColor> > colours;

	ofColor currentRibbonColour;
	float colourRotator;
};
