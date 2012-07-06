#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "Ribbon.h"
#include "Room.h"
#include "Bloom.h"
#include "KinectSkeletons.h"


class RibbonSkeleton {
public:
	Ribbon *ribbonLeft;
	Ribbon *ribbonRight;
	KinectSkeleton skeleton;
	bool alive;
	void setup(KinectSkeleton skel, ofCamera &cam, ofxBulletWorldRigid *world) {
		alive = true;
		ribbonLeft = new Ribbon(60, &cam, world);
		ribbonRight = new Ribbon(60, &cam, world);
		update(skel);
		printf("\n\n\nCRATING RIBBONS!!!!!\n\n\n");
		
	}
	void update(KinectSkeleton skel) {

		this->skeleton = skel;
		
		// we want to rescale to screen coords - we also want to remove the z component
		// (for now)
		ofVec3f factor = ofVec3f((float)ofGetWidth()/640.f, (float)ofGetHeight()/480.f, 0);
		ribbonLeft->setAnchorPoint(skeleton.leftHand*factor);
		ribbonRight->setAnchorPoint(skeleton.rightHand*factor);
	}
	
};

class testApp : public ofBaseApp{

public:
	~testApp();
	
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
		
	
	ofCamera cam;
	void updateRibbons();
	void drawRibbons();
	void updateFromSkeletons();
	ofLight light;
	
	bool stopMoving;
//	ofFbo fbo;
//	ofFbo roomFbo;
	Room room;
	//tricks::gl::effects::Bloom bloom;
	KinectSkeletons kinect;
	
	// these are the skeletons currently on the scene
	map<int,RibbonSkeleton> skeletons;
	
	// these are the ribbons that have been disassocated with any skeleton.
	vector<Ribbon*> deadRibbons;
	
	ofxBulletWorldRigid	world;
	
};
