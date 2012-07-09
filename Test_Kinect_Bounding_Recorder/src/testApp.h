#pragma once

#include "ofMain.h"
#include "KinectThresholder.h"
#include "BoundBlob.h"
#include "ofxBlobTracker.h"
//#include "SimpleAnim.h"
class testApp : public ofBaseApp, public BoundBlobListener {

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
	void doPersonTracking();
	KinectThresholder kinect;
	map<int,BoundBlob> people;
	
	void boundBlobEntered(const BoundBlob &blob);
	void boundBlobMoved(const BoundBlob &blob);
	void boundBlobExited(const BoundBlob &blob);
	//SimpleAnim anim;
};
