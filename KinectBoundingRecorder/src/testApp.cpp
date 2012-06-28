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
	vector<ofVec3f> blobs;
	ofVec2f dims(kinect.getWidth(), kinect.getHeight()); 
	
	// use the z coordinate.
	for(int i = 0; i < contours.blobs.size(); i++) {
		ofVec3f b = ofVec3f(contours.blobs[i].centroid.x/(float)kinect.getWidth(), 
							contours.blobs[i].centroid.y/(float)kinect.getHeight(), i);
		blobs.push_back(b);
	}
	
	
	
	blobTracker.track(blobs);
	
	ofxBlobEvent e;
	while(blobEvents.getNextEvent(e)) {
		if(e.eventType==ofxBlobTracker_entered) {
			people[e.blobId] = BoundBlob();
			people[e.blobId].init(contours.blobs[(int)e.pos.z]);
			people[e.blobId].setDepth(kinect.getDepth(contours.blobs[(int)e.pos.z]));
			
			if(recording) {
				ofVec3f bounds(640,480, 255);

				ofVec3f f = people[e.blobId].left/bounds;
				anim.addFrame("hand_left", (const float*)&f.x);
				
				f = people[e.blobId].right/bounds;

				anim.addFrame("hand_right", (const float*)&f.x);
				
				f = people[e.blobId].top/bounds;
				anim.addFrame("top", (const float*)&f.x);
				
				f = people[e.blobId].bottom/bounds;
				anim.addFrame("bottom", (const float*)&f.x);
			}
		} else if(e.eventType==ofxBlobTracker_moved) {
			people[e.blobId].update(contours.blobs[(int)e.pos.z]);
			
			if(recording) {
				ofVec3f bounds(640,480, 255);
				
				ofVec3f f = people[e.blobId].left/bounds;
				anim.addFrame("hand_left", (const float*)&f.x);
				
				f = people[e.blobId].right/bounds;
				anim.addFrame("hand_right", (const float*)&f.x);
				
				f = people[e.blobId].top/bounds;
				anim.addFrame("top", (const float*)&f.x);
				
				f = people[e.blobId].bottom/bounds;
				anim.addFrame("bottom", (const float*)&f.x);
			}
			
			
		} else if(e.eventType==ofxBlobTracker_exited) {
			people.erase(e.blobId);
		}
	}
	
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
	
	
	map<int,BoundBlob>::iterator it;
	for(it = people.begin(); it != people.end(); it++) {
		(*it).second.draw();
		ofDrawBitmapString(ofToString((*it).first), (*it).second.centroid);
		
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
		case 'b': kinect.learnBackground = true; break;
			
		case 'r':
			recording ^= true;
			if(!recording) { // just finished recording
				
				anim.save(ofToDataPath(ofGetTimestampString()+".txt"));
				anim.animations.clear();
				
			}
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