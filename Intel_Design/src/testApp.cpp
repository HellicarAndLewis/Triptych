#include "testApp.h"
#include "ofxSimpleGuiToo.h"

//bool clearedFbo = false;

void testApp::setup() {

	world.setup();
	world.enableGrabbing();
	world.setCamera(&cam);
	world.setGravity( ofVec3f(0, 0, 0) );
	
	
	//bloom.setup(true);
	//fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
	ofSetFrameRate(30);
	
	room.setup((float)ofGetWidth()/(float)ofGetHeight());
	room.setupGui();
	
	
	gui.setAutoSave(true);
	gui.loadFromXML();


	
	glEnable(GL_LIGHTING);
	light.setPointLight();
	
//	light.setSpecularColor(ofFloatColor(1, 1, 1));
	light.enable();
	
//	glEnable(GL_NORMALIZE);
	
	stopMoving = false;
	kinect.setup();

}

testApp::~testApp() {
	kinect.exit();


}


void testApp::updateFromSkeletons() {
	// mark all skeletons as dead, only unmark them if they 
	// still exist in the next bit of code (when updating the skels)
	map<int,RibbonSkeleton>::iterator it;
	for(it = skeletons.begin(); it != skeletons.end(); it++) {
		(*it).second.alive = false;
	}
	
	
	// check for new or updated skeletons
	for(int i = 0; i < kinect.getNumSkeletons(); i++) {
		
		KinectSkeleton s = kinect.getSkeleton(i);
		if(skeletons.find(s.id)==skeletons.end()) {
			
			// we have a new skeleton
			skeletons[s.id] = RibbonSkeleton();
			skeletons[s.id].setup(s, cam, &world);
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
			
			deadRibbons.push_back((*it).second.ribbonLeft);
			deadRibbons.push_back((*it).second.ribbonRight);
			
			skeletons.erase((*it).first);
		}
	}
}

void testApp::updateRibbons() {
	world.update();
	map<int,RibbonSkeleton>::iterator it;
	// now update all the ribbons.
	for(it = skeletons.begin(); it != skeletons.end(); it++) {
		(*it).second.ribbonLeft->update();
		(*it).second.ribbonRight->update();
	}
	
	
	// this includes any ribbons that have detached themselves
	// from a skeleton (i.e. the skeleton disappeared).
	for(int i = 0; i < deadRibbons.size(); i++) {
		// head south if detached.
		deadRibbons[i]->setAnchorPoint(deadRibbons[i]->getAnchorPoint()+ofVec3f(0, 3, 0));
		deadRibbons[i]->update();
	}
}

void testApp::update() {
	
	kinect.update();
	
	
	updateFromSkeletons();
	updateRibbons();
	room.update();
}


void testApp::drawRibbons() {
	map<int,RibbonSkeleton>::iterator it;
	// now update all the ribbons.
	for(it = skeletons.begin(); it != skeletons.end(); it++) {
		(*it).second.ribbonLeft->draw();
		(*it).second.ribbonRight->draw();
	}
	
	
	// this includes any ribbons that have detached themselves
	// from a skeleton (i.e. the skeleton disappeared).
	for(int i = 0; i < deadRibbons.size(); i++) {
		deadRibbons[i]->draw();
	}
}


void testApp::draw() {
	ofEnableAlphaBlending();
	ofBackground(0);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	
	ofSetHexColor(0xFFFFFF);
	room.draw();
	//bloom.begin();
	//kinect.drawDebug();
	glPushMatrix();
	glScalef((float)ofGetWidth()/640.f, (float)ofGetHeight()/480.f, 1);
	kinect.kinect.drawSkeletons();
	glPopMatrix();
	drawRibbons();
	
	
	//bloom.end();

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);	
	ofSetHexColor(0xFFFFFF);
	//bloom.getOutput()->draw(100,100);
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
		case 'f':
			ofToggleFullscreen();
			break;
		default:
			break;
	}
	
}


void testApp::keyReleased(int key) {
	
}


void testApp::mouseMoved(int x, int y) {
	
	
	if (!stopMoving) {
		//ribbon->setAnchorPoint(ofVec3f(x, y, 0));
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