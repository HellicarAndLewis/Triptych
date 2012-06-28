#include "testApp.h"
#include "Room.h"
#include "ofxSimpleGuiToo.h"

ofCamera camera;
ofLight light;
Room room;
float fov;
float lightZ;
float ambient = 1.0;
float diffuse = 1.0;
ofVec3f camPos;
//--------------------------------------------------------------
void testApp::setup(){
	gui.addSlider("fov", fov, 30, 140);
	gui.addSlider("CamX", camPos.x, -1,1);
	gui.addSlider("CamY", camPos.y, -1,1);
	gui.addSlider("CamZ", camPos.z, -10,0);
	gui.addSlider("LightZ", lightZ, -5, 1);
	gui.addSlider("Ambient", ambient, 0, 1);
	gui.addSlider("Diffuse", diffuse, 0, 1);
	
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	light.setPointLight();
	fov = 60;
	room.setup();
	ofDisableSetupScreen();
	ofBackgroundHex(0);
	camera.setPosition(0, 0, -10);
	camera.lookAt(ofVec3f(0,0,0));
	camera.setNearClip(0.001);
}

//--------------------------------------------------------------
void testApp::update(){
	camera.setFov((int)fov);
	camera.setPosition(camPos.x, camPos.y, camPos.z);
	//printf("Fob: %f\n", fov);
	light.setPosition(
					  ofMap(mouseX, 0, ofGetWidth(), -1,1)
					  , 
					  ofMap(mouseY, 0, ofGetHeight(), 1,-1)
					  , lightZ);//sin(ofGetElapsedTimef())-3);
	light.setAmbientColor(ofFloatColor(ambient, ambient, ambient));
	light.setDiffuseColor(ofFloatColor(diffuse, diffuse, diffuse));
	ofDisableSetupScreen();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofEnableLighting();
	light.enable();
	camera.begin();
	
	room.draw();
	camera.end();
	glPushMatrix();
	ofSetupScreen();
	ofDisableLighting();
	gui.draw();
	glPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key) {
		case ' ':
			gui.toggleDraw();
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