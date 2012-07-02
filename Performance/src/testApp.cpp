#include "testApp.h"

#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"
#include "Room.h"

float nearThreshold = 255;
float farThreshold = 0;

deque<vector<KinectMesh> > meshes;




//--------------------------------------------------------------
void testApp::setup(){
	
	
	ambient = 1.0;
	diffuse = 1.0;
	
	ofSetFrameRate(30);
	kinect.setup();
	kinect.setupGui();
	
	
	gui.addTitle("Room");
	gui.addSlider("fov", fov, 30, 140);
	gui.addSlider("CamX", camPos.x, -1,1);
	gui.addSlider("CamY", camPos.y, -1,1);
	gui.addSlider("CamZ", camPos.z, -10,0);
	gui.addSlider("LightZ", lightZ, -5, 1);
	gui.addSlider("Ambient", ambient, 0, 1);
	gui.addSlider("Diffuse", diffuse, 0, 1);
	
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	//grey.allocate(kinect.getWidth(), kinect.getHeight());
	
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

	
	if(nearThreshold==farThreshold) {
		nearThreshold = farThreshold+1;
	}

	kinect.update();
	
	int numPix = kinect.getWidth()*kinect.getHeight()*3;
	

	if(kinect.update()) {
		
		contours.findContours(kinect.getOutline(), 30*30, 480*480, 20, false);
		unsigned char *rgb = kinect.getPixels();
			
			
		if(meshes.size()>20) {
			meshes.pop_back();
		}
		
		meshes.push_front(vector<KinectMesh>());
		for(int i = 0; i < contours.blobs.size(); i++) {
			meshes.front().push_back(KinectMesh());
			if(!meshes.front().back().setup(contours.blobs[i], grey, rgb)) {
				meshes.front().pop_back();
				printf("Mesh too small\n");
			}
		}
	}

	
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
	
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

	ofBackground(0);
	
	ofEnableAlphaBlending();
	
	ofEnableLighting();
	light.enable();
	
	camera.begin();
	{
		room.draw();
	}
	camera.end();
	
	
	glPushMatrix();
	{
		
		ofSetupScreen();

	
	
		glPushMatrix();
		{
			glScalef(ofGetWidth()/kinect.getWidth(), ofGetHeight()/kinect.getHeight(), 1);
			glColor4f(1, 1,1, 0.2);

			deque<vector<KinectMesh> >::reverse_iterator it;
			
			int x = -400;
			for(it = meshes.rbegin(); it != meshes.rend(); it++) {
				x += 20;
				glPushMatrix();
				glTranslatef(0, 0, x);
				for(int i = 0; i < (*it).size(); i++) {
					(*it)[i].draw();
				}
			glPopMatrix();
			}

		}
		glPopMatrix();
			
		gui.draw();
		
		ofDisableLighting();
		gui.draw();
	}
	glPopMatrix();
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