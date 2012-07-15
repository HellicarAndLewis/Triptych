#pragma once

#include "ofMain.h"

#include "Room.h"
#include "Bloom.h"

#include "KinectThresholder.h"
#include "KinectMesh.h"
#include "BoundBlobListener.h"

#include "BoundBlob.h"

#include "Person.h"



class testApp : public ofBaseApp, BoundBlobListener {

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
	ofLight light;
	
	Room room;
	

	
	KinectThresholder kinect;

	vector<KinectMesh> meshes;
	
	std::map<int, Person> people;
	
	void boundBlobEntered(const BoundBlob &blob);
	void boundBlobMoved(const BoundBlob &blob);
	void boundBlobExited(const BoundBlob &blob);
	
	ofFbo fbo;
};
