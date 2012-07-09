#include "testApp.h"

#include "ofxSimpleGuiToo.h"

bool recording = false;
//--------------------------------------------------------------
void testApp::setup(){
	
	kinect.setup();
	kinect.setupGui();
	
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	gui.setAlignRight(true);
	
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	kinect.setListener(this);
}

//--------------------------------------------------------------
void testApp::update(){


	bool newFrame = kinect.update();
	if(newFrame) {
		
		kinect.trackBlobs();
		//doPersonTracking();

	}
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
}


//--------------------------------------------------------------
void testApp::draw(){
	
	kinect.drawDebug();
	//contours.draw();
	
	
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key) {

		case ' ':
			gui.toggleDraw();
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
		case 'b': 
			kinect.learnBackground = true; 
			break;
			
	/*	case 'r':
			recording ^= true;
			if(!recording) { // just finished recording
				
				anim.save(ofToDataPath(ofGetTimestampString()+".txt"));
				anim.animations.clear();
				
			}
			break;*/
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){


	
}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){


	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::boundBlobEntered(const BoundBlob &blob) {
	people[blob.id] = blob;
}

void testApp::boundBlobMoved(const BoundBlob &blob) {
	people[blob.id] = blob;
}

void testApp::boundBlobExited(const BoundBlob &blob) {
	if(people.find(blob.id)!=people.end()) people.erase(blob.id);
}
