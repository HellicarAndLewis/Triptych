#include "testApp.h"

#include "ofxSimpleGuiToo.h"

#include "ofxKinectNuiDraw.h"


int iterations;
float m;
bool usingAdd = true;
float maxImageSize, minImageSize;
bool enlargeImage = true;

float trailWidth = 1;
bool variableWidth = false;
float variableFactor = 1;
bool useGravity = false;
float gravityFactor = 0.1;
bool useFade = false;

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

	gui.addSlider("iterations", iterations, 0, 500);
	gui.addSlider("m", m, 0, 0.01);
	gui.addSlider("background alpha", backgroundAlpha, 0, 0.2);
	gui.addSlider("image alpha", imageAlpha, 0, 0.2);
	gui.addSlider("max image size", maxImageSize, 40, 1000);
	gui.addSlider("min image size", minImageSize, 0, 100);
	gui.addToggle("use additive", usingAdd);
	gui.addToggle("enlarge image", enlargeImage);

	gui.addSlider("trail width", trailWidth, 0, 50);
	gui.addToggle("variabe width", variableWidth);
	gui.addSlider("variable factor", variableFactor, 0, 2);
	gui.addToggle("use gravity", useGravity);
	gui.addSlider("gravity factor", gravityFactor, 0, 2);
	gui.addToggle("use fade", useFade);

	gui.loadFromXML();
	gui.setAutoSave(true);
	

	ofBackgroundHex(0);

	brushFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
	brushFbo.begin();
	ofClear(0, 0, 0, 1);
	brushFbo.end();

	iterations = 15;
	m = 0.00655;
	backgroundAlpha = 0.085;
	imageAlpha = 0.005;
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
	
	it = skeletons.begin();
	while(it != skeletons.end()) {
	//for(it = skeletons.begin(); it != skeletons.end(); ++it) {
		
		
		if(!(*it).second.alive) {
			

			//deadRibbons.push_back((*it).second.ribbonLeft);
			//deadRibbons.push_back((*it).second.ribbonRight);

			delete (*it).second.leftBrush;
			delete (*it).second.rightBrush;
			
			skeletons.erase(it++);
			
		}
		else {
			++it;
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

	
	
	//ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
	
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




			//brushFbo.begin();
	
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			glColor4f(0, 0, 0, backgroundAlpha);
			ofRect(0, 0, ofGetWidth(), ofGetHeight());

			glColor4f(1,1,1,imageAlpha);
			//	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
			if (usingAdd) {
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}
			else {
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			map<int,RibbonSkeleton>::iterator it;
			for(it = skeletons.begin(); it != skeletons.end(); it++) {
				(*it).second.draw();
			}
	
			//brushFbo.end();

			//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			//glColor4f(1, 1, 1, 1);
			//brushFbo.draw(0, 0);


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