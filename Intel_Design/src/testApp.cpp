#include "testApp.h"
#include "ofxSimpleGuiToo.h"

//bool clearedFbo = false;

bool useMareks;


void testApp::setup() {

	
	//bloom.setup(true);
	//fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
	ofSetFrameRate(30);
	
	room.setup((float)ofGetWidth()/(float)ofGetHeight());
	room.setupGui();
	
	
	gui.addToggle("switch trail", useMareks);
	
	kinect.setup();
	kinect.setupGui();
	kinect.setListener(this);
	
	gui.setAutoSave(true);
	gui.loadFromXML();


	useMareks = false;
	
	glEnable(GL_LIGHTING);
	light.setPointLight();
	
//	light.setSpecularColor(ofFloatColor(1, 1, 1));
	light.enable();
	
//	glEnable(GL_NORMALIZE);
	
	

	

}

testApp::~testApp() {



}



void testApp::update() {
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	
	if (kinect.update()) {
		
		vector<ofxCvBlob> &contours = kinect.getContours();
		
		meshes.clear();
		
		for (int i = 0; i < contours.size(); i++) {
			meshes.push_back(KinectMesh());
			meshes.back().setup(contours[i], kinect);
		}
		
		kinect.trackBlobs();
	}
	room.update();
}



void testApp::draw() {
	ofEnableAlphaBlending();
	ofBackground(0);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	room.draw();

	
	ofPushMatrix();
	ofScale(ofGetWidth()/640.0f, ofGetHeight()/480.0f);
	
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].draw();
	}
	
	ofPopMatrix();
	
	
	for (map<int, Person>::iterator it = people.begin(); it != people.end(); it++) {
		
		it->second.draw();
	}
	
	gui.draw();
}


void testApp::boundBlobEntered(const BoundBlob &blob) {
	people[blob.id] = Person();
	people[blob.id].setup(blob);
	
}

void testApp::boundBlobMoved(const BoundBlob &blob) {
	
	if(people.find(blob.id) != people.end()) {
		people[blob.id].update(blob);
	} else {
		boundBlobEntered(blob);
	}
}

void testApp::boundBlobExited(const BoundBlob &blob) {
	if(people.find(blob.id) != people.end()) {
		people.erase(blob.id);
	}
	
}


void testApp::keyPressed(int key) {
	
	switch (key) {
		case ' ':
			gui.toggleDraw();
			break;
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'b':
			kinect.learnBackground = true;

//			trail.clear();
			break;
			
		default:
			break;
	}
	
}


void testApp::keyReleased(int key) {
	
}


void testApp::mouseMoved(int x, int y) {
	
	

	
}


void testApp::mouseDragged(int x, int y, int button) {
	

							 
	
	

}


void testApp::mousePressed(int x, int y, int button) {
			
}


void testApp::mouseReleased(int x, int y, int button){
}


void testApp::windowResized(int w, int h) {
	room.setup((float)w/(float)h);	
}


void testApp::gotMessage(ofMessage msg) {
	
}


void testApp::dragEvent(ofDragInfo dragInfo) { 
	
}