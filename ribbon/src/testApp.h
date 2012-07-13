#pragma once

#include "ofMain.h"
#include "KinectThresholder.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"
#include "Room.h"
#include "Bloom.h"
#include "Brush.h"
#include "Trail.h"

#include "Ribbon.h"

#include "NuiSensor.h"

//class Ribbon {
//public:
//	ofVec2f pos;
//
//	void update(ofVec3f pos) { // pos's z-component isn't set (probably)
//		this->pos.set(pos.x, pos.y);
//	}
//	
//	
//	void draw() {
//		ofSetHexColor(0xFF00FF);
//		ofCircle(pos, 20);
//	}
//};


class RibbonSkeleton {
public:
	Trail *leftBrush, *rightBrush, *head;
	Ribbon *ribbons[NUI_SKELETON_POSITION_COUNT];

	KinectSkeleton skeleton;
	bool alive;
	void setup(KinectSkeleton skel) {
		alive = true;

		//NB by we are zeroing z
		ofVec3f factor = ofVec3f((float)ofGetWidth()/640.f, (float)ofGetHeight()/480.f, 0);
		//leftBrush = new Trail(skel.leftHand * factor);
		//rightBrush = new Trail(skel.rightHand * factor);
		//head = new Trail(skel.head * factor);

		

		for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++) {
			ribbons[i] = new Ribbon(skel.points[i] * factor);
		}

		update(skel);
	}

	void update(KinectSkeleton skel) {

		this->skeleton = skel;
		
		// we want to rescale to screen coords - we also want to remove the z component
		// (for now)
		ofVec3f factor = ofVec3f((float)ofGetWidth()/640.f, (float)ofGetHeight()/480.f, 0);
		//leftBrush->update(skeleton.leftHand * factor);
		//rightBrush->update(skeleton.rightHand * factor);
		//head->update(skeleton.head * factor);
		for (int  i = 0; i < ribbons.size(); i++) {
			ribbons[i]->update(skeleton.points[i] * factor);
		}
	}

	void setColor(ofFloatColor col) {
		//leftBrush->setColour(col);
		//rightBrush->setColour(col);
		//head->setColour(col);
		for (int  i = 0; i < ribbons.size(); i++) {
			ribbons[i]->setColour(col);
		}
	}

	void draw() {
		//leftBrush->draw();
		//rightBrush->draw();
		//head->draw();
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
};
