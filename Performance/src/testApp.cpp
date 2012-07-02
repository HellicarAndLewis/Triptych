#include "testApp.h"

#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
#include "KinectMesh.h"
#include "Room.h"

float nearThreshold = 255;
float farThreshold = 0;

//deque<
vector<KinectMesh> 
//>
meshes;




//--------------------------------------------------------------
void testApp::setup(){
	
	room.setup(640.f/480.f);
	
	
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	kinect.setup();
	kinect.setupGui();
	room.setupGui();	
	
	
	
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	
	
	
	

	ofDisableSetupScreen();
	ofBackgroundHex(0);

	
	
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
			
	/*	
		if(meshes.size()>0) {
			meshes.pop_back();
		}
		
		meshes.push_front(vector<KinectMesh>());
		*/
		meshes.clear();
		for(int i = 0; i < contours.blobs.size(); i++) {
			meshes.push_back(KinectMesh());
			if(!meshes.back().setup(contours.blobs[i], kinect.getOutline(), rgb)) {
				meshes.pop_back();
				printf("Mesh too small\n");
			}
			/*
			meshes.front().push_back(KinectMesh());
			if(!meshes.front().back().setup(contours.blobs[i], kinect.getOutline(), rgb)) {
				meshes.front().pop_back();
				printf("Mesh too small\n");
			}*/
		}
	}

	
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
	
	room.update();
	ofDisableSetupScreen();
}


//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(0);
	
	ofEnableAlphaBlending();
	

	room.draw();
	
	glPushMatrix();
	{
		
		ofSetupScreen();

	
	
		glPushMatrix();
		{
			glScalef((float)ofGetWidth()/(float)kinect.getWidth(), (float)ofGetHeight()/(float)kinect.getHeight(), 1);
			//glColor4f(1, 1,1, 0.2);
			ofSetHexColor(0xFFFFFF);

			/*deque<vector<KinectMesh> >::reverse_iterator it;
			
			int x = -400;
			for(it = meshes.rbegin(); it != meshes.rend(); it++) {
				x += 20;
				glPushMatrix();
				{
					
					glTranslatef(0, 0, x);
					for(int i = 0; i < (*it).size(); i++) {
						(*it)[i].draw();
					}
				}
				glPopMatrix();
			}*/
			for(int i = 0; i < meshes.size(); i++) {
				meshes[i].draw();
			}

		}
		glPopMatrix();
			
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
	room.setAspect((float)w/(float)h);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}