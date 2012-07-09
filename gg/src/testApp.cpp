#include "testApp.h"

void testApp::setup(){

	ofSetFrameRate(60);
	
	ofBackgroundHex(0x0);
	
	gui.addSlider("trail max length", trail.trailMaxLength, 0, 500);
	gui.addSlider("reduction speed", trail.reductionSpeed, 0, 100);
	gui.addSlider("interpolation factor", trail.interpolationFactor, 0, 1);
	gui.addSlider("sine multiplier", trail.sineMultiplier, 0, 100);
	gui.addSlider("sine increment", trail.sineIncrement, 0, 2);
	
	gui.addToggle("draw info", trail.drawInfo);
	gui.addToggle("draw wireframe", trail.drawWireframe);
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	ofEnableSmoothing();
}


void testApp::update(){

	trail.update();
}


void testApp::draw(){
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	trail.draw();
	
	gui.draw();
}


void testApp::keyPressed(int key){

	switch (key) {
		case ' ':
			gui.toggleDraw();
			break;
			
		default:
			break;
	}
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y){

}


void testApp::mouseDragged(int x, int y, int button){

	trail.input(ofVec3f(x, y, 0));
}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo){ 

}