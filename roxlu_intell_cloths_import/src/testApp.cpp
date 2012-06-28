#include "testApp.h"

testApp::testApp()
	:gui("Settings", 300)
	,creator(ps)
{
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(33);
	
	vector<string> options;
	options.push_back("Add particles");
	options.push_back("Select particles");
	options.push_back("Create strength");
	modes = &gui.addRadio<testApp>("Mode", 1, this, options, creator.mode);
	gui.addButton<testApp>("save",0,this);
	gui.load("gui.bin");
	
	cam.setup(ofGetWidth(), ofGetHeight());
	ax.setup(10);
	creator.setup();
	cam.translate(0,0,5);
	
	debug = false;
}

void testApp::operator()(int i) {
	switch(i) {	
		case 0: {
			printf("0\n");
			break;
		}
		case 1: {
			printf("mode changed1\n");
			break;
		}
		default:break;
	}
}

//--------------------------------------------------------------
void testApp::update(){
	gui.update();
	creator.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-20);

	if(!debug) {
		creator.draw(cam.pm(), cam.vm());
	}
	else {
		cam.place();
		ax.draw();
		creator.debugDraw();
	}
	//gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == 's') {
		gui.save("gui.bin");
	}
	else if(key == 'l') {
		gui.load("gui.bin");
	}
	else if(key == 'd') {
		debug = !debug;
	}
	else if(key == 'a') {
		modes->selected = 0;
		modes->updateSelected();
		modes->needsRedraw();
		gui.update();
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	gui.onMouseMoved(x,y);

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	gui.onMouseMoved(x,y);
	cam.onMouseDragged(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	gui.onMouseDown(x,y);
	cam.onMouseDown(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	gui.onMouseUp(x,y);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}