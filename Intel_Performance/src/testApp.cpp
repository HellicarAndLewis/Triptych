#include "testApp.h"

#include "ofxSimpleGuiToo.h"
#include "TimeProfiler.h"
using namespace tricks::util;

//--------------------------------------------------------------
void testApp::setup(){
	showTimeProfiler = false;
	
	audioSystem.setup("performance", kinect);
	bloom.setup(true);
	room.setup(640.f/480.f);
	otherChannelAttenuation = 0.5;
	channelPower = 0.5;	
	channelAlpha = 0.5;
	
	gui.addPage("Program");
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	kinect.setup();
	kinect.setupGui();
	room.setupGui();	
	
	
	//flock.setup();
	//flock.setupGui();
	gui.addPage("Appearance");
	KinectMesh::setupGui();
	gui.addTitle("Mesh Shader");
	gui.addSlider("other channel atten", otherChannelAttenuation, 0, 1);
	gui.addSlider("channel power", channelPower, 0, 1);
	gui.addSlider("channel alpha", channelAlpha, 0, 1);
	gui.addSlider("bloom amount", bloom.amount, 0, 2);
	gui.addSlider("bloom brightness", bloom.brightness, 0, 2);
	gui.loadFromXML();
	gui.setAutoSave(true);
	

	//ofDisableSetupScreen();
	ofBackgroundHex(0);

	//ofEnableNormalizedTexCoords();
	meshShader.load("mesh.vert", "mesh.frag");
}

//--------------------------------------------------------------
void testApp::update() {
	//flock.update(kinect.getOutline().getPixels());
	ScopedTimer timer("testApp::update()");
	//float s = ofGetElapsedTimef();
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
		kinect.trackBlobs();
	}

	//printf("Update time: %f\n", (ofGetElapsedTimef()-s)*1000);

	
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
	
	room.update();
	//ofDisableSetupScreen();
	if(gui.isOn()) {
		ofShowCursor();
	} else {
		ofHideCursor();
	}
}

float lastTimeShaderLoaded = 0;
//--------------------------------------------------------------
void testApp::draw(){


	ScopedTimer drawTimer("testApp::draw()");
	
	//if(ofGetElapsedTimef() - lastTimeShaderLoaded>0.5) {
	//	lastTimeShaderLoaded = ofGetElapsedTimef();
	//	meshShader.load("mesh.vert", "mesh.frag");
	//}
	ofBackground(0);
	
	ofEnableAlphaBlending();
	


	room.draw();

	bloom.begin();
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
				drawLayer(meshes[50], -50, 2);
			}
			
			if(meshes.size()>25) {
				drawLayer(meshes[25], -25, 1);
			}
			//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			//if(meshes.size()>0) {
			//	drawLayer(meshes[0], 0, 0);
			//}
			
		}
		glPopMatrix();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		glDisable(GL_DEPTH_TEST);
			
		
	}
	glPopMatrix();
	
	bloom.end();
	bloom.getOutput()->draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
	glColor4f(1,1,1,1);
	
	
	glPushMatrix();
	{
		ofSetupScreen();
		gui.draw();
		if(showTimeProfiler) {
			TimeProfiler::draw();
		}
	}
	glPopMatrix();
	
	
	//	output.getTextureReference(0).unbind();
}



void testApp::drawLayer(vector<KinectMesh> &mesh, float z, int layer) {
	
	meshShader.begin();
	meshShader.setUniform1i("layer", layer);
	meshShader.setUniform1f("atten", otherChannelAttenuation);
	meshShader.setUniform1f("colorPower", channelPower);
	meshShader.setUniform1f("channelAlpha", channelAlpha);
	for(int i = 0; i < mesh.size(); i++) {
		glPushMatrix();
		float ss = sin(layer*ofGetElapsedTimef()*0.3)*10*(layer>0?1:0);
		glTranslatef(ss, 0, 0);
		//glTranslatef(0, 0, z*5-mesh[i].depth);
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
		case 'b':
			kinect.learnBackground = true;
			break;
		case 't':
			showTimeProfiler ^= true;
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