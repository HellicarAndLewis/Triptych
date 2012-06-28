#include "testApp.h"

testApp::testApp()
	:gui("Settings", 400)
	,creator(ps)
	,ss(creator)

{
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(2);
	ofEnableNormalizedTexCoords();

	cam.setup(ofGetWidth(), ofGetHeight());
	ax.setup(10);
	
	gui.addBool("Enable perlin noise", app_settings.use_perlin);
	gui.addBool("Create particle trails", app_settings.use_particle_trail);
	gui.addInt("Particle trail length", app_settings.particle_trail_length).setMin(10).setMax(200);
	gui.addFloat("Min particle size", app_settings.min_particle_size).setMin(0.01f).setMax(3.0f);
	gui.addFloat("Max particle size", app_settings.max_particle_size).setMin(0.01).setMax(5.0f);
	gui.addFloat("Max particle trail width", app_settings.max_particle_trail_width).setMin(0.01).setMax(0.08f); 
	gui.addFloat("Perlin influence", app_settings.perlin_influence).setMin(0.0).setMax(0.5);
	gui.addFloat("Perlin scale", app_settings.perlin_scale).setMin(0.0).setMax(8.0);
	gui.addFloat("Eye separation", scam.eye_separation).setMin(-0.5f).setMax(0.5f);
	gui.addFloat("Convergence ",app_settings.convergence).setMin(-2.05f).setMax(2.05f);
	gui.addButton<testApp>("Remove particle trails",1,this);
	gui.addButton<testApp>("Reset particle sizes",2,this);
	gui.addButton<testApp>("save",0,this);
	gui.load(File::toDataPath("gui.bin"));
	
	creator.setup();
	
	ss.setup();
	cam.translate(0,4,5);
	debug = false;
	show_gui = true;
	
	scam.setNear(0.1);
}

void testApp::operator()(int i) {
	switch(i) {	
		case 1: {
			ss.removeTrails();
			break;
		}
		case 2: {
			ss.resetParticleSizes();
			break;
		}
		
		default:break;
	}
}

//--------------------------------------------------------------
void testApp::update(){
	scam.setConvergence(app_settings.convergence);
	scam.updateProjectionMatrices();

	gui.update();
	creator.update();
	ss.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-20);

	if(!debug) {
		Vec3 right, up;
		cam.getBillboardVectors(right, up);
		
		bool anaglyphic = false;
		if(anaglyphic) {
			scam.setupLeft();
			scam.view_matrix = cam.view_matrix;
			ss.draw(scam.pml(), cam.vm(), right, up);
			
			scam.setupRight();
			ss.draw(scam.pmr(), cam.vm(), right, up);
		}
		else {
			creator.draw(cam.pm(), cam.vm());
			ss.draw(cam.pm(), cam.vm(), right, up);
		}

		
	}
	else {
		cam.place();
		
		creator.debugDraw();
		ss.debugDraw();
	}
	
	if(show_gui) {
		gui.draw();
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
	if(key == 's') {
		gui.save(File::toDataPath("gui.bin"));
	}
	else if(key == 'l') {
		gui.load(File::toDataPath("gui.bin"));
	}
	else if(key == 'd') {
		debug = !debug;
	}
	else if(key == 'g') {
		show_gui = !show_gui;
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