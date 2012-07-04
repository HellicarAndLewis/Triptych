#include "testApp.h"
#include "ofxSimpleGuiToo.h"

testApp::testApp()
	:flock_gui("Settings", 300)
	,app(ofGetWidth(), ofGetHeight())
{
}

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(3);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableNormalizedTexCoords();
	show_gui = true;
	debug = true;
	app.setup();
	

	room.setup((float)ofGetWidth()/(float)ofGetHeight());
	room.setupGui();
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	
	flock_gui.addFloat("Flock high threshold (align)", app.flock.high).setMin(0.0f).setMax(1.0f);
	flock_gui.addFloat("Flock low threshold (separate)", app.flock.low).setMin(0.0f).setMax(1.0f);
	flock_gui.addFloat("Flock radius SQ", app.flock.zone_radius_sq).setMin(0.0f).setMax(20000.0f);
	flock_gui.addFloat("Flock max speed", settings.flocking_max_speed).setMin(0.0f).setMax(50.0f).setColor(0.6,0.1,0.1);
	flock_gui.addFloat("Flock align energy", app.flock.align_energy).setMin(0.0f).setMax(100.0f);
	flock_gui.addFloat("Flock separate energy", app.flock.separate_energy).setMin(0.0f).setMax(100.0f);
	flock_gui.addFloat("Flock attract energy", app.flock.attract_energy).setMin(0.0f).setMax(100.0f);
	flock_gui.addBool("Flock apply perlin noise", settings.flocking_apply_perlin);//.setColor(0.4, 0.03, 0.1);
	flock_gui.addFloat("Flock perlin scale", settings.flocking_perlin_scale).setMin(0.0f).setMax(10.0f); //.setColor(0.4, 0.03, 0.1);
	flock_gui.addFloat("Flock perlin influence", settings.flocking_perlin_influence).setMin(0.0f).setMax(100.0f); //.setColor(0.4, 0.03, 0.1);
	flock_gui.addFloat("Player repel energy", settings.player_repel_energy).setMin(0.0f).setMax(100000.0f).setColor(0.1,0.4,0.0);
	flock_gui.addFloat("Player repel radius", settings.player_repel_radius).setMin(0.0f).setMax(1000.0f).setColor(0.1,0.4,0.0);

	flock_gui.addFloat("Boid trail duration (millis)", settings.boid_trail_duration_millis).setMin(0.0f).setMax(5000.0f);
	flock_gui.addFloat("Boid glow duration (millis)", settings.boid_glow_duration_millis).setMin(0.0f).setMax(5000.0f);

	flock_gui.addFloat("Explosion random x velocity", settings.explosion_random_x_vel).setMin(0.0f).setMax(15.0f);
	flock_gui.addFloat("Explosion random y velocity", settings.explosion_random_y_vel).setMin(0.0f).setMax(15.0f);
	flock_gui.addFloat("Explosion min lifespan", settings.explosion_min_lifespan).setMin(0.0f).setMax(100.0f);
	flock_gui.addFloat("Explosion max lifespan", settings.explosion_max_lifespan).setMin(0.0f).setMax(100.0f);
	flock_gui.addFloat("Explosion perlin scale", settings.explosion_perlin_scale).setMin(0.0f).setMax(10.0f);
	flock_gui.addFloat("Explosion perlin influence", settings.explosion_perlin_influence).setMin(0.0f).setMax(1.0f);
	flock_gui.addInt("Explosion trail length", settings.explosion_trail_length).setMin(0).setMax(20);

	flock_gui.load(ofToDataPath("gui.bin",true));


	
	cam.translate(0,0,5);
	cam.orthoBottomLeft(ofGetWidth(), ofGetHeight(), 0.0, 100.0f);
}

//--------------------------------------------------------------
void testApp::update(){
	flock_gui.update();
	app.update();
	room.update();
}

//--------------------------------------------------------------
void testApp::draw(){
/*	ofColor start_color(33,33,53);
	ofBackgroundGradient(start_color, ofColor::black, OF_GRADIENT_CIRCULAR);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, ofGetWidth()/ofGetHeight(), 0.1, 100);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0,0,-10);
		app.debugDraw();		
		return;
		*/
	room.draw();
	if(!debug) {
		Mat3 nm(cam.vm());
        nm.inverse();
        nm.transpose();
		
		
        
		app.draw(cam.pm(), cam.vm(), nm);
	}
	else {
		
	}
	if(show_gui) {
		flock_gui.draw();
	}
	gui.draw();
	
	ofDrawBitmapString("Particles: " +ofToString(app.fx_ps.size()), 10, ofGetHeight()-40);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'd') {
		debug = !debug;
	}
	else if(key == 's') {
		flock_gui.save(ofToDataPath("gui.bin", true));
	}
	else if(key == 'l') {
		flock_gui.load(ofToDataPath("gui.bin",true));
	}
	else if(key == 'g') {
		show_gui = !show_gui;
	} else if(key==' ') {
		gui.toggleDraw();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	flock_gui.onMouseMoved(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	flock_gui.onMouseMoved(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	flock_gui.onMouseDown(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	flock_gui.onMouseUp(x,y);
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