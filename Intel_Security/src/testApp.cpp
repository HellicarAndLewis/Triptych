#include "testApp.h"
#include "ofxSimpleGuiToo.h"

testApp::testApp()
	:flock_gui("Settings", 400)
	,app(ofGetWidth(), ofGetHeight())
{
}

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(3);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableNormalizedTexCoords();
	show_gui = false;
	debug = false;
	
	app.setup();
	room.setup((float)ofGetWidth()/(float)ofGetHeight());
	room.setupGui();
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	float attack_col[3] = {0.5, 0.0, 0.3};
	float flock_col[3] = {0.38,0.38,0.06};
	flock_gui.addFloat("Flock radius SQ", app.flock.zone_radius_sq).setMin(0.0f).setMax(5.4).setColor(flock_col);
	flock_gui.addFloat("Flock high threshold (align)", app.flock.high).setMin(0.0f).setMax(1.0f).setColor(flock_col);
	flock_gui.addFloat("Flock low threshold (separate)", app.flock.low).setMin(0.0f).setMax(1.0f).setColor(flock_col);
	flock_gui.addFloat("Flock align energy", app.flock.align_energy).setMin(0.0f).setMax(0.01f).setColor(flock_col);
	flock_gui.addFloat("Flock separate energy", app.flock.separate_energy).setMin(0.0f).setMax(0.01f).setColor(flock_col);
	flock_gui.addFloat("Flock attract energy", app.flock.attract_energy).setMin(0.0f).setMax(0.01f).setColor(flock_col);
	flock_gui.addFloat("Flock center energy", settings.flocking_center_energy).setMin(0.0f).setMax(5.0f).setColor(flock_col);
	flock_gui.addFloat("Flock sphere size", settings.flocking_sphere_size).setMin(3.0f).setMax(10.0f).setColor(flock_col);
	flock_gui.addFloat("Boid glow size", settings.boid_glow_size).setMin(0.0f).setMax(2.0f);
	flock_gui.addFloat("Boid scale",settings.boid_scale).setMin(0.0f).setMax(1.0f);
	flock_gui.addFloat("Boid trail width", settings.boid_trail_width).setMin(0.01f).setMax(2.1f);
	flock_gui.addInt("Boid trail length", settings.boid_trail_length).setMin(0).setMax(30);
	flock_gui.addBool("Boid create trails", settings.boid_create_trails);
	flock_gui.addFloat("Boid attract to user energy", settings.attract_to_user_energy).setMin(0.00f).setMax(15.5f);
	flock_gui.addFloat("Boid attract to user radius", settings.attract_to_user_radius).setMin(0.00f).setMax(15.0f);
	flock_gui.addFloat("Boid attack energy", settings.boid_attack_energy).setMin(0.0f).setMax(2.1f);
	flock_gui.addFloat("Boid attack duration min", settings.boid_attack_duration_min).setMin(0.0f).setMax(10000.0f);
	flock_gui.addFloat("Boid attack duration max", settings.boid_attack_duration_max).setMin(0.0f).setMax(20000.0f);
	flock_gui.addFloat("Boid attack delay min (ms)", settings.boid_attack_delay_min).setMin(0.0f).setMax(10000.0f);
	flock_gui.addFloat("Boid attack delay max (ms)", settings.boid_attack_delay_max).setMin(0.0f).setMax(20000.0f);
	flock_gui.addFloat("Boids: mix of normal in boid shader", settings.boid_shader_normal_mix).setMin(0.0f).setMax(1.0f);
	flock_gui.addFloat("Boids: specular component (lower is more)", settings.boid_shader_specular).setMin(0.0f).setMax(10.0f);
	flock_gui.addBool("Record kinect", settings.must_record_kinect);
	flock_gui.addBool("Draw boid glows", settings.boid_draw_glows);
	flock_gui.addBool("Draw grid", settings.draw_axis);
	flock_gui.addBool("Draw flock", settings.draw_flock);
	flock_gui.addBool("Draw room", settings.draw_room);
	flock_gui.addBool("DebugDraw attackers", settings.debugdraw_attackers);
	flock_gui.addFloat("Kinect scale", settings.kinect_scale).setMin(0.0f).setMax(15.0f);
	flock_gui.addButton<testApp>("Save kinect recording", 0, this);	
	flock_gui.addFloat("Percentage of visible boids", settings.boids_percentage_visible).setMin(0.0f).setMax(1.0f).setColor(attack_col);
	flock_gui.addFloat("Percentage of attack boids", settings.boids_percentage_attackers).setMin(0.0f).setMax(1.0f).setColor(attack_col);
	flock_gui.addButton<testApp>("Update number of visible boids", 1, this).setColor(attack_col);
	flock_gui.load(ofToDataPath("gui.bin",true));

	cam.setup(ofGetWidth(), ofGetHeight());
	cam.translate(0,0,5);
	ax.setup(10);
}

void testApp::operator()(const int n) {
	switch(n) {
		case 0: {
			settings.must_record_kinect = false;
			app.kinect.recorder.save(File::toDataPath("kinect.bin"));
			break;
		}
		case 1: {
			app.control.setVisibleBoidsPercentage(settings.boids_percentage_visible);
			app.control.setAttackPercentage(settings.boids_percentage_attackers);
			break;
		}
		default:break;
	}
}

//--------------------------------------------------------------
void testApp::update(){
	if(show_gui) {
		flock_gui.update();
	}
	app.update();
	room.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString("Particles: " +ofToString(app.fx_ps.size()), 10, ofGetHeight()-40);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-20);

	if(settings.draw_room) {
		room.draw();
	}
	
	gui.draw();
	if(!debug) {
		Vec3 right, up;
		cam.getBillboardVectors(right, up);
		Mat3 nm(cam.vm());
        nm.inverse();
        nm.transpose();
		cam.place();
		if(settings.draw_axis) {
			ax.draw();
		}
		app.draw(cam.pm().getPtr(), cam.vm().getPtr(), nm.getPtr(), right.getPtr(), up.getPtr());
	}
	else {
		cam.place();
		if(settings.draw_axis) {
			ax.draw();
		}
		app.debugDraw();		
	}
	
	if(show_gui) {
		flock_gui.draw();
	}

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
	}
	else if(key==' ') {
		gui.toggleDraw();
	}
	else if(key == 'f') {
		ofToggleFullscreen();
	}
	else if(key == 'r') {
		settings.rotate_scene = !settings.rotate_scene;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	if(show_gui) {
		flock_gui.onMouseMoved(x,y);
	}

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(show_gui) {
		flock_gui.onMouseMoved(x,y);
	}
	if(settings.rotate_scene) {
		cam.onMouseDragged(x,y);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(show_gui) {
		flock_gui.onMouseDown(x,y);
	}
	if(settings.rotate_scene) {
		cam.onMouseDown(x,y);
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(show_gui) {
		flock_gui.onMouseUp(x,y);
	}
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