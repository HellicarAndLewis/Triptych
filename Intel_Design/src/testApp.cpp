#include "testApp.h"
#include "ofxSimpleGuiToo.h"
#include "ofxKinectNuiDraw.h"



float frontTaper, backTaper;
bool drawPink;
int ribbonLength;
bool fadeInZ;
bool useBloom;

float fadeSpeed;
float colourSpeed = 0.01;

//--------------------------------------------------------------
void testApp::setup(){
	
	bloom.setup(false);
	room.setup(640.f/480.f);
	audioSystem.setup("design", kinect);
	
	gui.addPage("Program");
	//ofSetVerticalSync(true);
	ofSetFrameRate(30);
	kinect.setup(true);
	kinect.setupGui();
	room.setupGui();	
	
	
	gui.addPage("Appearance");
	KinectMesh::setupGui();
	gui.addSlider("bloom amount", bloom.amount, 0, 2);
	gui.addSlider("bloom brightness", bloom.brightness, 0, 2);
	gui.addToggle("use bloom", useBloom);

	gui.addTitle("new");
	gui.addSlider("front taper", frontTaper, 0, 100);
	gui.addSlider("back taper", backTaper, 0, 100);
	gui.addToggle("draw pink", drawPink);
	gui.addSlider("ribbon length", ribbonLength, 0, 500);
	gui.addToggle("fade in z", fadeInZ);

	fadeSpeed = 0.05;
	gui.addSlider("fade speed", fadeSpeed, 0, 0.5);
	gui.addSlider("colour speed", colourSpeed, 0, 5);

	gui.loadFromXML();
	gui.setAutoSave(true);
	

	ofBackgroundHex(0);

	brushFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
	brushFbo.begin();
	ofClear(0, 0, 0, 1);
	brushFbo.end();

	currentRibbonColour.setHsb(0, 255, 255, 255);
	colourRotator = 0;
		//these are the four colours in the colour pallette
	colours.resize(3);

		//reds
		colours[0].push_back(ofFloatColor(1.0, 0.3, 0.16, 1));
		colours[0].push_back(ofFloatColor(0.78, 0.0, 0.09, 1));
		colours[0].push_back(ofFloatColor(1.0, 0.86, 0.75, 1));
		colours[0].push_back(ofFloatColor(1.0, 0.0, 0.11, 1));

		//greens
		colours[1].push_back(ofFloatColor(0.0, 0.3, 0.05, 1));
		colours[1].push_back(ofFloatColor(0.75, 0.9, 0.54, 1));
		colours[1].push_back(ofFloatColor(0.46, 0.82, 0.19, 1));
		colours[1].push_back(ofFloatColor(0.0, 0.7, 0.29, 1));

		//yellows
		colours[2].push_back(ofFloatColor(1.0, 0.53, 0.0, 1));
		colours[2].push_back(ofFloatColor(1.0, 0.77, 0.0, 1));
		colours[2].push_back(ofFloatColor(0.99, 0.98, 0.84, 1));
		colours[2].push_back(ofFloatColor(1.0, 0.89, 0.0, 1));
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
			//skeletons[s.id].setColor(colours[((int) rand()) % colours.size()][((int) rand())%colours[0].size()]);
			skeletons[s.id].setColor(currentRibbonColour);
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
			
			//instead of just removing, shall we fade the existing ones????
			for (int  i = 0; i < (*it).second.ribbons.size(); i++) {
				//delete (*it).second.ribbons[i];
				deadRibbons.push_back((*it).second.ribbons[i]);
			}
			
			skeletons.erase(it++);
			
		}

		else {
			++it;
		}
	}

	//vector<Ribbon*>::iterator vit = deadRibbons.begin();
	////while(vit != deadRibbons.end()) {
	for(int i = 0; i < deadRibbons.size(); i++) {

		Ribbon *ribbon = deadRibbons[i];
		if (ribbon->baseAlpha < 1) {
			ribbon->baseAlpha += fadeSpeed;
		}
		else {
			delete ribbon;
			deadRibbons.erase(deadRibbons.begin()+i);
			i--;
		}
	}


	currentRibbonColour.setHue(colourRotator+=colourSpeed);
	if (colourRotator > 255) colourRotator = 0;
}

//--------------------------------------------------------------
void testApp::update() {
	
	//float s = ofGetElapsedTimef();
	if(kinect.update()) {
		
		//no depiction of the self now...

		//meshes.clear();
		//vector<ofxCvBlob> &blobs = kinect.getContours();
		//for(int i = 0; i < blobs.size(); i++) {
		//	meshes.push_back(KinectMesh());
		//	meshes.back().setup(blobs[i], kinect);
		//}
		//	
		//// store 100 sets of meshes of history.
		//if(meshes.size()>100) {
		//	meshes.pop_back();
		//}
		updateFromSkeletons();
		kinect.trackBlobs();
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

			//for(int i = 0; i < meshes.size(); i++) {
			//	meshes[i].draw();
			//}


			/*brushFbo.begin();
	
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			glColor4f(0, 0, 0, backgroundAlpha);
			ofRect(0, 0, ofGetWidth(), ofGetHeight());

			glColor4f(1,1,1,imageAlpha);
				ofEnableBlendMode(OF_BLENDMODE_ADD);
	
			if (usingAdd) {
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}
			else {
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}*/

			if(useBloom) bloom.begin();
			map<int,RibbonSkeleton>::iterator it;
			for(it = skeletons.begin(); it != skeletons.end(); it++) {
				(*it).second.draw();
			}

			for(int i = 0; i < deadRibbons.size(); i++) {
				deadRibbons[i]->draw();
			}

			if(useBloom) bloom.end();
	
			/*brushFbo.end();

			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			glColor4f(1, 1, 1, 1);
			brushFbo.draw(0, 0);*/


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
	
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
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