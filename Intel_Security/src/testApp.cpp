#include "testApp.h"
#include "ofxSimpleGuiToo.h"

testApp::testApp()
#ifdef USE_APP 
	#ifdef USE_FLOCK_GUI
		:app(ofGetWidth(), ofGetHeight()),flock_gui("Settings", 400)
	#else
		:app(ofGetWidth(), ofGetHeight())	
	#endif
#else 
	#ifdef USE_FLOCK_GUI
		:flock_gui("Settings", 400)
	#endif
#endif	
{	
	printf("testApp()\n");
}

//--------------------------------------------------------------
void testApp::setup(){
	printf("testApp::setup()\n");
	ofBackground(3);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableNormalizedTexCoords();
	show_gui = false;
	debug = false;


	printf("testApp::setup() - 1 \n");
#ifdef USE_APP
	app.setup();
#endif
	room.setup((float)ofGetWidth()/(float)ofGetHeight());
	room.setupGui();
	gui.loadFromXML();
	gui.setAutoSave(true);

#ifdef USE_FLOCK_GUI
	
	printf("testApp::setup() - 2 \n");
	float attack_col[3] = {0.5, 0.0, 0.3};
	float flock_col[3] = {0.38,0.38,0.06};
#ifdef USE_APP
	flock_gui.addFloat("Flock radius SQ", app.flock.zone_radius_sq).setMin(0.0f).setMax(5.4).setColor(flock_col);
	flock_gui.addFloat("Flock high threshold (align)", app.flock.high).setMin(0.0f).setMax(1.0f).setColor(flock_col);
	flock_gui.addFloat("Flock low threshold (separate)", app.flock.low).setMin(0.0f).setMax(1.0f).setColor(flock_col);
	flock_gui.addFloat("Flock align energy", app.flock.align_energy).setMin(0.0f).setMax(0.01f).setColor(flock_col);
	flock_gui.addFloat("Flock separate energy", app.flock.separate_energy).setMin(0.0f).setMax(0.01f).setColor(flock_col);
	flock_gui.addFloat("Flock attract energy", app.flock.attract_energy).setMin(0.0f).setMax(0.01f).setColor(flock_col);
#endif
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
	
	printf("testApp::setup() - 3 \n");

	flock_gui.load(ofToDataPath("gui.bin",true));
	
#endif
	cam.setup(ofGetWidth(), ofGetHeight());
	cam.translate(0,0,5);
	ax.setup(10);
	

	printf("Setup ready - ...\n");
}

void testApp::operator()(const int n) {
#ifdef USE_APP
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
#endif	
}

//--------------------------------------------------------------
void testApp::update(){
#ifdef USE_FLOCK_GUI	
	if(show_gui) {
		flock_gui.update();
	}
#endif

#ifdef USE_APP
	app.update();
#endif

	room.update();
}

//--------------------------------------------------------------
void testApp::draw(){
		


	//glColor3f(1,1,1);
#ifndef _WIN32
	ofDrawBitmapString("Particles: " +ofToString(app.fx_ps.size()), 10, ofGetHeight()-40);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-20); // crashes in msvc++
#else
	/*
	static int c = 0;
	++c;
	if(c % 120 == 1) {
	//	printf("FPS:%d\n", ofGetFrameRate());
	}
	*/
#endif



#ifdef USE_FLOCK_GUI
	//if(settings.draw_room) {
#endif
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glBindVertexArray(0);
		room.draw();
#ifdef USE_FLOCK_GUI		
//	}
#endif
	


//	return;s
	

//	return;
	
	if(!debug) {
		Vec3 right, up;
		cam.getBillboardVectors(right, up);
		Mat3 nm(cam.vm());
        nm.inverse();
        nm.transpose();
		cam.place();
	

		#ifndef _WIN32
			if(settings.draw_axis) {
				ax.draw();
			}
		#endif

#ifdef USE_APP
		app.draw(cam.pm().getPtr(), cam.vm().getPtr(), nm.getPtr(), right.getPtr(), up.getPtr());
#endif
	}
	else {

		cam.place();

		#ifndef _WIN32
			if(settings.draw_axis) {
				ax.draw();
			}
		#endif
#ifdef USE_APP
		app.debugDraw();		
#endif
	}
#ifdef USE_FLOCK_GUI
	if(show_gui) {
		flock_gui.draw();
	}
#endif

	glPushMatrix();
		ofSetupScreen();
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	gui.setDraw(show_gui);
	//	gui.draw();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();

	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	 if(key==' ') {
		printf("Draw gui\n"); 
		gui.toggleDraw();
	}
	
	if(key == 'd') {
		debug = !debug;
	}
#ifdef USE_FLOCK_GUI
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
		printf("Draw gui\n"); 
		gui.toggleDraw();
	}
#endif
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
#ifdef USE_FLOCK_GUI
	if(show_gui) {
		flock_gui.onMouseMoved(x,y);
	}
#endif
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
#ifdef USE_FLOCK_GUI
	if(show_gui) {
		flock_gui.onMouseMoved(x,y);
	}
#endif
	
	if(settings.rotate_scene) {
		cam.onMouseDragged(x,y);
	}
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
#ifdef USE_FLOCK_GUI
	if(show_gui) {
		flock_gui.onMouseDown(x,y);
	}
#endif
	
	if(settings.rotate_scene) {
		cam.onMouseDown(x,y);
	}
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
#ifdef USE_FLOCK_GUI
	if(show_gui) {
		flock_gui.onMouseUp(x,y);
	}
#endif
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