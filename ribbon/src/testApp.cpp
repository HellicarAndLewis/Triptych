#include "testApp.h"


void testApp::setup() {
	
	ofSetFrameRate(60);
	
	
	ribbon = new Ribbon(30, &cam);
		
}

testApp::~testApp() {
	
	delete ribbon;

}


void testApp::update() {
	ribbon->update();
}


void testApp::draw() {
	ofBackgroundHex(0x0);

//	glEnable(GL_LIGHTING);
	
	ribbon->draw();

}


void testApp::keyPressed(int key) {
	

	
}


void testApp::keyReleased(int key) {
	
}


void testApp::mouseMoved(int x, int y) {
	
	ribbon->setAnchorPoint(ofVec3f(x, y, 0));
	
}


void testApp::mouseDragged(int x, int y, int button) {
	

							 

}


void testApp::mousePressed(int x, int y, int button) {
			
}


void testApp::mouseReleased(int x, int y, int button){
}


void testApp::windowResized(int w, int h) {
	
}


void testApp::gotMessage(ofMessage msg) {
	
}


void testApp::dragEvent(ofDragInfo dragInfo) { 
	
}