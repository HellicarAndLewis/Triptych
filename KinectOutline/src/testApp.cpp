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
	
}

//--------------------------------------------------------------
void testApp::update(){


	kinect.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
}


//--------------------------------------------------------------
void testApp::draw(){
	
	kinect.drawDebug();
	
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