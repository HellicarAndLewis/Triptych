#include "testApp.h"

#include "ofxSimpleGuiToo.h"

#include "ofxKinectNuiDraw.h"
//--------------------------------------------------------------
void testApp::setup(){
	
	bloom.setup(true);
	room.setup(640.f/480.f);
	
	
	gui.addPage("Program");
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	kinect.setup(true);
	kinect.setupGui();
	room.setupGui();	
	
	
	gui.addPage("Appearance");
	KinectMesh::setupGui();
	gui.addSlider("bloom amount", bloom.amount, 0, 2);
	gui.addSlider("bloom brightness", bloom.brightness, 0, 2);
	gui.loadFromXML();
	gui.setAutoSave(true);
	

	ofBackgroundHex(0);

}




void testApp::updateFromSkeletons() {
	// mark all skeletons as dead, only unmark them if they 
	// still exist in the next bit of code (when updating the skels)
	map<int,RibbonSkeleton>::iterator it;
	for(it = skeletons.begin(); it != skeletons.end(); it++) {
		(*it).second.alive = false;
	}
	
	
	int numSkells = kinect.getNumSkeletons();
	// check for new or updated skeletons
	for(int i = 0; i < numSkells; i++) {
		
		KinectSkeleton s;
		kinect.getSkeleton(i, s);
		if(skeletons.find(s.id)==skeletons.end()) {
			
			// we have a new skeleton
			skeletons[s.id] = RibbonSkeleton();
			skeletons[s.id].setup(s);
			skeletons[s.id].alive = true;
		} else {
			// we have a skeleton to update.
			skeletons[s.id].update(s);
			skeletons[s.id].alive = true;
		}
	}
	
	
	// remove any skeletons that weren't marked as still alive
	// in the previous bit of code.
	for(it = skeletons.begin(); it != skeletons.end(); it++) {
		if(!(*it).second.alive) {
			

			//deadRibbons.push_back((*it).second.ribbonLeft);
			//deadRibbons.push_back((*it).second.ribbonRight);

			delete (*it).second.ribbonLeft;
			delete (*it).second.ribbonRight;
			
			skeletons.erase((*it).first);
		}
	}
}

//--------------------------------------------------------------
void testApp::update() {
	
	//float s = ofGetElapsedTimef();
	if(kinect.update()) {
		
		meshes.clear();
		vector<ofxCvBlob> &blobs = kinect.getContours();
		for(int i = 0; i < blobs.size(); i++) {
			meshes.push_back(KinectMesh());
			meshes.back().setup(blobs[i], kinect);
		}
			
		// store 100 sets of meshes of history.
		if(meshes.size()>100) {
			meshes.pop_back();
		}
		updateFromSkeletons();
	}

	//printf("Update time: %f\n", (ofGetElapsedTimef()-s)*1000);

	
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
	
	room.update();
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(0);
	
	ofEnableAlphaBlending();
	


	room.draw();

	//bloom.begin();
	glPushMatrix();
	{
		
		ofSetupScreen();

		
		
		glPushMatrix();
		{
			glScalef((float)ofGetWidth()/(float)kinect.getWidth(), (float)ofGetHeight()/(float)kinect.getHeight(), 1);
			ofSetHexColor(0xFFFFFF);

			for(int i = 0; i < meshes.size(); i++) {
				meshes[i].draw();
			}

			map<int,RibbonSkeleton>::iterator it;
			for(it = skeletons.begin(); it != skeletons.end(); it++) {
				(*it).second.draw();
			}
	
			
		}
		glPopMatrix();
		
		glDisable(GL_DEPTH_TEST);
			
		
	}
	glPopMatrix();
	
	//bloom.end();
	//bloom.getOutput()->draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
	glColor4f(1,1,1,1);
	
	
	glPushMatrix();
	{
		ofSetupScreen();
	
	//	kinect.drawDebug();
		gui.draw();
	}
	glPopMatrix();
	
	
	//	output.getTextureReference(0).unbind();
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
		case 'b':
			kinect.learnBackground = true;
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
	bloom.resize(w, h);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}