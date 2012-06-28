#include "testApp.h"


void testApp::setup() {
	
	ofSetFrameRate(60);
	
	
	ribbon = new Ribbon(30, &cam);

	
	glEnable(GL_LIGHTING);
	light.setPointLight();
	
//	light.setSpecularColor(ofFloatColor(1, 1, 1));
	light.enable();
	
//	glEnable(GL_NORMALIZE);
	
	stopMoving = false;
}

testApp::~testApp() {
	
	delete ribbon;

}


void testApp::update() {
	ribbon->update();
}


void testApp::draw() {
	ofBackgroundHex(0x0);
	
	ribbon->draw();

}


void testApp::keyPressed(int key) {
	
	switch (key) {
		case ' ':
			stopMoving = !stopMoving;
			break;
		default:
			break;
	}
	
}


void testApp::keyReleased(int key) {
	
}


void testApp::mouseMoved(int x, int y) {
	
	
	if (!stopMoving) {
		ribbon->setAnchorPoint(ofVec3f(x, y, 0));
	}
	else {
		light.setPosition(mouseX, 300, mouseY-200);
	}

	
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