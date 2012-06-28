#include "testApp.h"


void testApp::setup() {
	
	ofSetFrameRate(60);
	
	cam.setPosition(ofVec3f(0, -3.f, -111));
	cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
		
	world.setup();
	world.enableGrabbing();
	world.setCamera(&cam);
	world.setGravity( ofVec3f(0, 0, 0) );
	
	
	//this has been taken straight out of the ofxBulletCustomShapes example
	//doesn't seem to add the front wall though, which is pretty annoying...
	ofVec3f startLoc;
	ofPoint dimens;
	boundsWidth = 100;
	float hwidth = boundsWidth*.5;
	float depth = 1.;
	float hdepth = depth*.5;
	boundsShape = new ofxBulletCustomShape();
	boundsShape->create(world.world, ofVec3f(0, 0, 0), 10.);
	
	for(int i = 0; i < 6; i++) {
		bounds.push_back( new ofxBulletBox() );
		if(i == 0) { // ground //
			startLoc.set( 0., hwidth+hdepth, 0. );
			dimens.set(boundsWidth, depth, boundsWidth);
		} 
		else if (i == 1) { // back wall //
			startLoc.set(0, 0, hwidth+hdepth);
			dimens.set(boundsWidth, boundsWidth, depth);
		} 
		else if (i == 2) { // right wall //
			startLoc.set(hwidth+hdepth, 0, 0.);
			dimens.set(depth, boundsWidth, boundsWidth);
		} 
		else if (i == 3) { // left wall //
			startLoc.set(-hwidth-hdepth, 0, 0.);
			dimens.set(depth, boundsWidth, boundsWidth);
		} 
		else if (i == 4) { // ceiling //
			startLoc.set(0, -hwidth-hdepth, 0.);
			dimens.set(boundsWidth, depth, boundsWidth);
		} 
		else if (i == 5) { // front wall //         <-- this doesn't actually get addded...
			startLoc.set(0, 0, -hwidth-hdepth);
			dimens.set(boundsWidth, boundsWidth, depth);
		}
		btBoxShape* boxShape = ofBtGetBoxCollisionShape( dimens.x, dimens.y, dimens.z );
		boundsShape->addShape( boxShape, startLoc );
		
		bounds[i]->create( world.world, startLoc, 0., dimens.x, dimens.y, dimens.z );
		bounds[i]->setProperties(.25, .95);
		bounds[i]->add();
	}
	

	
	int nSpheres = 100;
	
	for (int i = 0; i < nSpheres; i++) {
		shapes.push_back( new ofxBulletSphere() );
		
		//mass of 0.1 diameter of 1.0
		((ofxBulletSphere*)shapes[i])->create(world.world, ofVec3f(0, -hwidth+5, -5), 0.1f, 1.0f);
		((ofxBulletSphere*)shapes[i])->setSphereResolution( 12 );
		((ofxBulletSphere*)shapes[i])->setActivationState( DISABLE_DEACTIVATION );
		((ofxBulletSphere*)shapes[i])->setRestitution(1.0);
		shapes[i]->add();
	}
	
	
	
	ofSetSmoothLighting(true);
	light.setAmbientColor(ofColor(.0, .0, .0));
	light.setDiffuseColor(ofColor(.0, .0, .0));
	light.setSpecularColor(ofColor(255, 255, 255));
	
	
	boundsMat.setAmbientColor(ofFloatColor(10, 9, 10));
	boundsMat.setDiffuseColor(ofFloatColor(12, 10, 12));
	boundsMat.setSpecularColor(ofFloatColor(1, 1, 1));
	boundsMat.setShininess(10);
	
	shapesMat.setAmbientColor(ofFloatColor(0, 0, 0));
	shapesMat.setDiffuseColor(ofFloatColor(0, 0, 0));
	shapesMat.setSpecularColor(ofFloatColor(1, 1, 1));
	shapesMat.setShininess(80);
	
	
	//make this a sphere for now
	lightBody = new ofxBulletSphere(); 
	
	//set mass to 1000 and radius to 20...
	((ofxBulletSphere*) lightBody)->create(world.world, ofVec3f(0, 0, 0), 1000, 20.0);
	lightBody->add() ;
	
}

testApp::~testApp() {
	delete lightBody;
}


void testApp::update() {
	

	world.update();
	
}


void testApp::draw() {

	glEnable( GL_DEPTH_TEST );
	
	cam.begin();
	
	
	ofEnableLighting();
	light.enable();
	light.setPosition(lightBody->getPosition());
	ofSetColor(255, 255, 255);
	lightBody->draw();

	lightBody->draw();
	
	ofSetColor(100., 100., 100.);
	
	boundsMat.begin();
	for(int i = 0; i < bounds.size()-1; i++) {
		bounds[i]->draw();
	}
	boundsMat.end();

	ofDisableAlphaBlending();
	ofDisableBlendMode();
	
	
	ofSetColor(255,0,138);
	ofPushStyle();
	shapesMat.begin();
	for(int i = 0; i < shapes.size(); i++) {
		shapes[i]->draw();
	}
	shapesMat.end();
	ofPopStyle();
	
	light.disable();
	ofDisableLighting();
	
	cam.end();
	glDisable(GL_DEPTH_TEST);
	
	
	stringstream ss;
	ss << "press space to attract spheres, and r to reset\n" <<
	"unfortunately, we don't have a front wall, which is somewhat strange as it should be there...\n" <<
	"light ball can be moved around with mouse";
	
	ofSetHexColor(0xffffff);
	ofDrawBitmapString(ss.str(), 10, 10);
	
}


void testApp::keyPressed(int key) {
	
	switch (key) {
		
		case ' ':
		{
			for (int i = 0; i < shapes.size(); i++) {
				ofVec3f dir = (lightBody->getPosition() - shapes[i]->getPosition()) *5;
				shapes[i]->applyCentralForce(dir);
			}
			
			break;
		}
			
		case 'r':
			for (int i = 0; i < shapes.size(); i++) {
				shapes[i]->remove();
				shapes[i]->create(world.world, ofVec3f(0, -50, -5), 0.1f, 1.0f);
				shapes[i]->add();
			}
			break;

			
		case 'q':
			for (int i = 0; i < shapes.size(); i++) {
				shapes[i]->applyCentralForce(ofRandom(100)-50,
																		 ofRandom(100),
																		 ofRandom(10));
			}
			break;
		default:
			break;
	}
	
}


void testApp::keyReleased(int key) {
	
}


void testApp::mouseMoved(int x, int y) {
	
	ofxBulletBaseShape *shape = shapes.back();
	
	
	
}


void testApp::mouseDragged(int x, int y, int button) {
	
}


void testApp::mousePressed(int x, int y, int button) {
			
}


void testApp::mouseReleased(int x, int y, int button){
	
}


void testApp::windowResized(int w, int h) {
	
}


void testApp::gotMessage(ofMessage msg) {
	
}


void testApp::dragEvent(ofDragInfo dragInfo) { 
	
}