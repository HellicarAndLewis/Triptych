#pragma once

#include "ofMain.h"
#include "KinectThresholder.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"
#include "Room.h"
//#include "FlockController.h"
#include "Bloom.h"

#include "AudioSystem.h"

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
	void drawLayer(vector<KinectMesh> &mesh, float z, int layer);
	
	Room room;
	ofxCvContourFinder contours;
	KinectThresholder kinect;
	ofShader meshShader;
	deque<vector<KinectMesh> > meshes;
	//FlockController flock;	
	
	float otherChannelAttenuation;
	float channelPower;
	float channelAlpha;
	

	tricks::gl::effects::Bloom bloom;
	
	AudioSystem audioSystem;
	bool showTimeProfiler;
};
