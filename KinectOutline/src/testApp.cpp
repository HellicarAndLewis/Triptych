#include "testApp.h"

#include "ofxSimpleGuiToo.h"

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
	
	blobTracker.addListener(&blobEvents);
}

//--------------------------------------------------------------
void testApp::update(){


	bool newFrame = kinect.update();
	if(newFrame) {
		int minBlobSize = 40;
		int maxBlobSize = kinect.getHeight();
	
		contours.findContours(kinect.getOutline(), minBlobSize*minBlobSize, maxBlobSize*maxBlobSize, 20, false, true);
		
		
		doPersonTracking();

	}
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
}


void testApp::doPersonTracking() {
	
//	people.clear();
	
	
	// run the blob tracker
	vector<ofVec2f> blobs;
	ofVec2f dims(kinect.getWidth(), kinect.getHeight()); 
	for(int i = 0; i < contours.blobs.size(); i++) {
		blobs.push_back(ofVec2f(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y)/dims);
	}
	
	blobTracker.track(blobs);
	
	
	/*if(contours.blobs.size()==people.size()) { // match the nearest
		for(int i = 0; i < contours.blobs.size(); i++) {
			people.push_back(BoundBlob());
			people.back().init(contours.blobs[i]);
		}
	} else if() {
		
	}*/
}
//--------------------------------------------------------------
void testApp::draw(){
	
	kinect.drawDebug();
	contours.draw();
	
	for(int i = 0; i < people.size(); i++) {
		people[i].draw();
	}
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