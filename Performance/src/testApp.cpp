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

	/*if(ofGetElapsedTimef() - lastTimeShaderLoaded>0.5) {
		lastTimeShaderLoaded = ofGetElapsedTimef();
		KinectMesh::shader->load("mesh.vert", "mesh.frag");
	}*/
	ofBackground(0);
	
	ofEnableAlphaBlending();
	

	room.draw();
	
	glPushMatrix();
	{
		
		ofSetupScreen();

	
	
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();
		{
			glScalef((float)ofGetWidth()/(float)kinect.getWidth(), (float)ofGetHeight()/(float)kinect.getHeight(), 1);
			//glColor4f(1, 1,1, 0.2);
			ofSetHexColor(0xFFFFFF);

			deque<vector<KinectMesh> >::iterator it;
			meshShader.begin();
			int x = 0;
			int pos = 0;
			int which = 0;
			for(it = meshes.begin(); it != meshes.end(); it++) {

				if((pos++)%35==0) {
					x -= 20;
					glPushMatrix();
					{
						
						glTranslatef(0, 0, x);
						
						
						for(int i = 0; i < (*it).size(); i++) {
							(*it)[i].draw();
						}
						which++;
					}
					glPopMatrix();
				}
			}
			meshShader.end();
		}
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
			
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