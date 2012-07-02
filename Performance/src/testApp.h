#pragma once

#include "ofMain.h"
#include "KinectOutline.h"
#include "ofxOpenCv.h"
#include "Room.h"

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

//	ofxKinect kinect;
	ofxCvGrayscaleImage grey;
	ofxCvContourFinder contours;
	KinectOutline kinect;
	
	ofCamera camera;
	ofLight light;
	Room room;
	float fov;
	float lightZ;
	float ambient;
	float diffuse;
	ofVec3f camPos;
};
