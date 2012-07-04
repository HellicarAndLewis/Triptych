#include "testApp.h"

#include "ofxSimpleGuiToo.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	room.setup(640.f/480.f);
	
	
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	kinect.setup();
	kinect.setupGui();
	room.setupGui();	
	KinectMesh::setupGui();
	
	
	gui.loadFromXML();
	gui.setAutoSave(true);
	

	ofDisableSetupScreen();
	ofBackgroundHex(0);

	
	meshShader.load("mesh.vert", "mesh.frag");
}

//--------------------------------------------------------------
void testApp::update() {
	
	if(kinect.update()) {
		
		contours.findContours(kinect.getOutline(), 30*30, 480*480, 20, false);
			
		// store 100 sets of meshes of history.
		if(meshes.size()>100) {
			meshes.pop_back();
		}
		
		meshes.push_front(vector<KinectMesh>());
		
		//meshes.clear();
		for(int i = 0; i < contours.blobs.size(); i++) {
		
			
			meshes.front().push_back(KinectMesh());
			if(!meshes.front().back().setup(contours.blobs[i], kinect)) {
				meshes.front().pop_back();

			}
		}
	}

	
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
	
	room.update();
	ofDisableSetupScreen();
}

float lastTimeShaderLoaded = 0;
//--------------------------------------------------------------
void testApp::draw(){

	if(ofGetElapsedTimef() - lastTimeShaderLoaded>0.5) {
		lastTimeShaderLoaded = ofGetElapsedTimef();
		meshShader.load("mesh.vert", "mesh.frag");
	}
	ofBackground(0);
	
	ofEnableAlphaBlending();
	

	room.draw();
	
	glPushMatrix();
	{
		
		ofSetupScreen();

	
		
		
		//glEnable(GL_DEPTH_TEST);
		glPushMatrix();
		{
			glScalef((float)ofGetWidth()/(float)kinect.getWidth(), (float)ofGetHeight()/(float)kinect.getHeight(), 1);
			//glColor4f(1, 1,1, 0.2);
			ofSetHexColor(0xFFFFFF);

			ofEnableBlendMode(OF_BLENDMODE_ADD);
			if(meshes.size()>75) {
				drawLayer(meshes[75], -75, 3);
			}
			if(meshes.size()>50) {
				drawLayer(meshes[50], -80, 2);
			}
			
			if(meshes.size()>25) {
				drawLayer(meshes[25], -40, 1);
			}
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			if(meshes.size()>0) {
				drawLayer(meshes[0], 0, 0);
			}
			
		}
		glPopMatrix();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		glDisable(GL_DEPTH_TEST);
			
		gui.draw();
	}
	glPopMatrix();
}

void testApp::drawLayer(vector<KinectMesh> &mesh, float z, int layer) {
	
	meshShader.begin();
	meshShader.setUniform1i("layer", layer);
	for(int i = 0; i < mesh.size(); i++) {
		glPushMatrix();
		glTranslatef(0, 0, z-mesh[i].depth);
		mesh[i].draw();				
		glPopMatrix();
	}
	meshShader.end();

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