#include "testApp.h"
#include "Ribbon.h"

Ribbon *ribbon;
ofLight light;

float frontTaper, backTaper;
bool drawPink;
int ribbonLength;
bool fadeInZ;

bool go = false;

void testApp::setup(){

	ofSetFrameRate(60);
	
	ofBackgroundHex(0x333333);
	
	
	gui.addSlider("trail max length", trail.trailMaxLength, 0, 500);
	gui.addSlider("reduction factor", trail.reductionSpeed, 0.6, 1);
	gui.addSlider("interpolation factor", trail.m, 0, 0.3);
	gui.addSlider("interpolation amount", trail.interpolationAmount, 0, 15);
//	gui.addSlider("sine multiplier", trail.sineMultiplier, 0, 100);
//	gui.addSlider("sine increment", trail.sineIncrement, 0, 2);
	
	gui.addSlider("trail width", trail.width, 0, 500);
	gui.addSlider("width multiplier", trail.widthMultiplier, 0, 1);
	gui.addToggle("inverse width", trail.inverseWidth);
	gui.addToggle("use velocity", trail.useVel);
	
	gui.addToggle("draw info", trail.drawInfo);
	gui.addToggle("draw wireframe", trail.drawWireframe);
	
	
	gui.addTitle("new");
	gui.addSlider("front taper", frontTaper, 0, 100);
	gui.addSlider("back taper", backTaper, 0, 100);
	gui.addToggle("draw pink", drawPink);
	gui.addSlider("ribbon length", ribbonLength, 0, 500);
	gui.addToggle("fade in z", fadeInZ);
	
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	ofEnableSmoothing();
	
	
	ribbon = new Ribbon();
	ribbon->maxThickness = 40;
	ribbon->minThickness = 5;
	ribbon->head = RibbonSegment();
	
//	ofEnableLighting();
//	light.setPointLight();
//	light.setPosition(400, 400, 0);
//	light.enable();

}


void testApp::update(){

//	trail.input(ofVec3f(mouseX, mouseY, 0));	
//	trail.update();
	
	if (!go) {
		ofVec3f p(mouseX, mouseY, 0);	
		ribbon->update(p);
	}
	else {
		ribbon->flong();
	}
}


void testApp::draw(){
	
//	ofEnableAlphaBlending();
//	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
//	trail.draw();
	
	ofEnableAlphaBlending();

	ribbon->draw();
	
	gui.draw();
}


void testApp::keyPressed(int key){

	switch (key) {
		case ' ':
			gui.toggleDraw();
			break;
		
		case 'a':
			go = !go;
			if (go) ribbon->counter = 0;
			break;
			
		default:
			break;
	}
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y){

}


void testApp::mouseDragged(int x, int y, int button){

	
}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo){ 

}