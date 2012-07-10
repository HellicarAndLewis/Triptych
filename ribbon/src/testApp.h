#pragma once

#include "ofMain.h"
#include "KinectThresholder.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"
#include "Room.h"
#include "Bloom.h"



class Ribbon {
public:
	ofVec2f pos;

	void update(ofVec3f pos) { // pos's z-component isn't set (probably)
		this->pos.set(pos.x, pos.y);
	}
	
	
	void draw() {
		ofSetHexColor(0xFF00FF);
		ofCircle(pos, 20);
	}
};


class RibbonSkeleton {
public:
	Ribbon *ribbonLeft;
	Ribbon *ribbonRight;
	KinectSkeleton skeleton;
	bool alive;
	void setup(KinectSkeleton skel) {
		alive = true;
		ribbonLeft = new Ribbon();
		ribbonRight = new Ribbon();
		update(skel);
	}

	void update(KinectSkeleton skel) {

		this->skeleton = skel;
		
		// we want to rescale to screen coords - we also want to remove the z component
		// (for now)
		ofVec3f factor = ofVec3f((float)ofGetWidth()/640.f, (float)ofGetHeight()/480.f, 0);
		ribbonLeft->update(skeleton.leftHand*factor);
		ribbonRight->update(skeleton.rightHand*factor);
	}

	void draw() {
		ribbonLeft->draw();
		ribbonRight->draw();
	}
	
};


class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

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
};
