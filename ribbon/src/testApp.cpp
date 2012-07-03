#include "testApp.h"
#include "ofxSimpleGuiToo.h"

bool clearedFbo = false;

void testApp::setup() {
	bloom.setup(true);
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
	ofSetFrameRate(30);
	
	room.setup((float)ofGetWidth()/(float)ofGetHeight());
	room.setupGui();
	
	
	gui.setAutoSave(true);
	gui.loadFromXML();
	ribbon = new Ribbon(60, &cam);

	
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
	room.update();
}


void testApp::draw() {
	ofEnableAlphaBlending();
	ofBackground(0);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetHexColor(0xFF0000);
	glPushMatrix();
	//glTranslatef(0, 0, 1);
	ofCircle(100, 100, 100);
	glPopMatrix();
	ofSetHexColor(0xFFFFFF);
	bloom.begin();
	ribbon->draw();
	bloom.end();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);	
	ofSetHexColor(0xFFFFFF);
	bloom.getOutput()->draw(0,0);
	/*
	fbo.begin();
	if(!clearedFbo) {
		clearedFbo = true;
		ofClear(0,0,0,0);
	}
	glColor4f(0,0,0,0.03);
	ofRect(0,0,ofGetWidth(),ofGetHeight());
	bloom.getOutput()->draw(0, 0);
	fbo.end();
	
	room.draw();
	
	fbo.draw(0,0);*/
	gui.draw();
}


void testApp::keyPressed(int key) {
	
	switch (key) {
		case ' ':
			gui.toggleDraw();
			break;
		case 's':
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
	room.setup((float)w/(float)h);	
}


void testApp::gotMessage(ofMessage msg) {
	
}


void testApp::dragEvent(ofDragInfo dragInfo) { 
	
}