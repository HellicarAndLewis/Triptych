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
#ifdef USE_LIGHT_RAYS
	,rays_gui("Rays", 300)
#endif
	,ia_gui("Interaction",400)
	,bloom_gui("Bloom", 400)

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
	must_take_screenshot = false;

#ifdef USE_APP
	app.setup();
#endif

	room.setup((float)ofGetWidth()/(float)ofGetHeight());
	room.setupGui();
	gui.loadFromXML();
	gui.setAutoSave(true);

	#ifdef USE_FLOCK_GUI
	
		float attack_col[3] = {0.5, 0.0, 0.3};
		float flock_col[3] = {0.38,0.38,0.06};
	
		#ifdef USE_APP	
	
			flock_gui.addFloat("Flock radius SQ", app.flock.zone_radius_sq).setMin(0.0f).setMax(8.4);
			flock_gui.addFloat("Flock high threshold (align)", app.flock.high).setMin(0.0f).setMax(1.0f);
			flock_gui.addFloat("Flock low threshold (separate)", app.flock.low).setMin(0.0f).setMax(1.0f);
			flock_gui.addFloat("Flock align energy", app.flock.align_energy).setMin(0.0f).setMax(0.01f);
			flock_gui.addFloat("Flock separate energy", app.flock.separate_energy).setMin(0.0f).setMax(0.01f);
			flock_gui.addFloat("Flock attract energy", app.flock.attract_energy).setMin(0.0f).setMax(0.01f);
	
		#endif
		
		flock_gui.addFloat("Flock center energy", settings.flocking_center_energy).setMin(0.0f).setMax(5.0f);
		flock_gui.addFloat("Flock sphere size", settings.flocking_sphere_size).setMin(3.0f).setMax(10.0f);
		flock_gui.addFloat("Boid glow size", settings.boid_glow_size).setMin(0.0f).setMax(2.0f);
		flock_gui.addFloat("Boid scale",settings.boid_scale).setMin(0.0f).setMax(1.0f);
		flock_gui.addFloat("Boid trail width", settings.boid_trail_width).setMin(0.01f).setMax(2.1f);
		flock_gui.addInt("Boid trail length", settings.boid_trail_length).setMin(0).setMax(30);
		flock_gui.addBool("Boid create trails", settings.boid_create_trails);
		flock_gui.addFloat("Boids: mix of normal in boid shader", settings.boid_shader_normal_mix).setMin(0.0f).setMax(1.0f);
		flock_gui.addFloat("Boids: specular component (lower is more)", settings.boid_shader_specular).setMin(0.0f).setMax(10.0f);
		flock_gui.addFloat("Boids: fog min", settings.boid_shader_fog_min).setMin(-30.0f).setMax(30.0f);
		flock_gui.addFloat("Boids: fog max", settings.boid_shader_fog_max).setMin(-30.0f).setMax(30.0f);
		flock_gui.addColor("Boids trail color", app.viz.trails_drawer.trail_color);
		flock_gui.addBool("Record kinect", settings.must_record_kinect);
		flock_gui.addBool("Draw boid glows", settings.boid_draw_glows);
		flock_gui.addBool("Draw grid", settings.draw_axis);
		flock_gui.addBool("Draw flock", settings.draw_flock);
		flock_gui.addBool("Draw room", settings.draw_room);
		flock_gui.addBool("DebugDraw attackers", settings.debugdraw_attackers);
		flock_gui.addFloat("Kinect scale", settings.kinect_scale).setMin(0.0f).setMax(15.0f);
		flock_gui.addButton<testApp>("Save kinect recording", 0, this);	
		flock_gui.addFloat("Percentage of visible boids", settings.boids_percentage_visible).setMin(0.0f).setMax(1.0f);
		flock_gui.addFloat("Percentage of attack boids", settings.boids_percentage_attackers).setMin(0.0f).setMax(1.0f);
		flock_gui.addButton<testApp>("Update number of visible boids", 1, this);
		flock_gui.setColor(0.63, 0.5, 0.4);
		flock_gui.load(ofToDataPath("gui.bin",true));
		
		
		#ifdef USE_LIGHT_RAYS		
			rays_gui.addFloat("Exposure", app.viz.light_rays.exposure).setMin(0.0f).setMax(0.05f);
			rays_gui.addFloat("Decay", app.viz.light_rays.decay).setMin(0.7f).setMax(1.2f);
			rays_gui.addFloat("Density", app.viz.light_rays.density).setMin(0.0f).setMax(1.0f);
			rays_gui.addFloat("Weight", app.viz.light_rays.weight).setMin(0.0f).setMax(10.0f);
			rays_gui.addFloat("Light.X", app.viz.light_rays.light_x).setMin(-1.0f).setMax(1.0f);
			rays_gui.addFloat("Light.Y", app.viz.light_rays.light_y).setMin(-1.0f).setMax(1.0f);
			rays_gui.load(ofToDataPath("rays.bin",true));
	//		rays_gui.setPosition(300,300);
	
		#endif
		ia_gui.addBool("Random force", settings.flocking_random_apply);
		ia_gui.addFloat("Random scale", settings.flocking_random_scale).setMin(0.0).setMax(0.001);
		ia_gui.addFloat("Random influence", settings.flocking_random_influence).setMin(0.0).setMax(0.1);
		ia_gui.addFloat("Boid repel from user ENERGY", settings.repel_from_user_energy).setMin(0.00f).setMax(1.0f);
		ia_gui.addFloat("Boid repel from user RADIUS", settings.repel_from_user_radius).setMin(0.00f).setMax(15.0f);
		ia_gui.addFloat("Boid attract to user ENERGY", settings.attract_to_user_energy).setMin(0.00f).setMax(1.0f);
		ia_gui.addFloat("Boid attract to user RADIUS", settings.attract_to_user_radius).setMin(0.00f).setMax(15.0f);
		ia_gui.addFloat("Boid attack duration min", settings.boid_attack_duration_min).setMin(0.0f).setMax(10000.0f);
		ia_gui.addFloat("Boid attack duration max", settings.boid_attack_duration_max).setMin(0.0f).setMax(20000.0f);
		ia_gui.addFloat("Boid attack delay min (ms)", settings.boid_attack_delay_min).setMin(0.0f).setMax(10000.0f);
		ia_gui.addFloat("Boid attack delay max (ms)", settings.boid_attack_delay_max).setMin(0.0f).setMax(20000.0f);
		ia_gui.setColor(0.73, 0.5, 0.4);
		ia_gui.load(ofToDataPath("ia.bin",true));
		

		bloom_gui.addFloat("Bloom amount", app.viz.bloom.amount).setMin(0.0f).setMax(2.0f);
		bloom_gui.addFloat("Bloom brightness", app.viz.bloom.brightness).setMin(0.0f).setMax(2.0f);
		bloom_gui.addFloat("Bloom blur.x", app.viz.bloom.blurx_scale).setMin(0.0f).setMax(1.0f);
		bloom_gui.addFloat("Bloom blur.y", app.viz.bloom.blury_scale).setMin(0.0f).setMax(1.0f);
		bloom_gui.setColor(0.68, 0.5, 0.5);
		bloom_gui.load(ofToDataPath("bloom.bin",true));
	#endif
	
	cam.setup(ofGetWidth(), ofGetHeight());
	cam.translate(0,0,5);
	ax.setup(10);
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

	if(show_gui) {
		#ifdef USE_FLOCK_GUI	
		flock_gui.update();
		#endif
		#ifdef USE_LIGHT_RAYS
		rays_gui.update();
		#endif
		ia_gui.update();
		bloom_gui.update();
	}


	#ifdef USE_APP
		
		app.update();
		
	#endif

	room.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString("Particles: " +ofToString(app.fx_ps.size()), 10, ofGetHeight()-40);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-20); // crashes in msvc++

	if(settings.draw_room) {
		room.draw();
	}
	
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
	
	
		if(show_gui) {
			#ifdef USE_FLOCK_GUI
			flock_gui.draw();
			#endif
			#ifdef USE_LIGHT_RAYS
			rays_gui.draw();
			#endif
			ia_gui.draw();
			bloom_gui.draw();
		}
	

		

	glPushMatrix();
		ofSetupScreen();
		gui.draw();
	glPopMatrix();

	if(must_take_screenshot) {
		must_take_screenshot = false;
		ofSaveScreen("screengrabs/"+ofGetTimestampString()+".tif");
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	 if(key==' ') {
		gui.toggleDraw();
	}
	
	if(key == 'd') {
		debug = !debug;
	}
	else if(key == 'g') {
		show_gui = !show_gui;
	}
	
	else if(key == 'w') {
		#ifdef USE_FLOCK_GUI
		flock_gui.save(ofToDataPath("gui.bin", true));
		#endif
		#ifdef USE_LIGHT_RAYS
		rays_gui.save(ofToDataPath("rays.bin",true));
		#endif
		ia_gui.save(ofToDataPath("ia.bin",true));
		bloom_gui.save(ofToDataPath("bloom.bin",true));
	}
	else if(key == 's') {
		must_take_screenshot = true;
	}
	else if(key == 'l') {
		#ifdef USE_FLOCK_GUI
		flock_gui.load(ofToDataPath("gui.bin",true));
		#endif
		#ifdef USE_LIGHT_RAYS
		rays_gui.load(ofToDataPath("rays.bin",true));
		#endif
		ia_gui.load(ofToDataPath("ia.bin",true));
		bloom_gui.load(ofToDataPath("bloom.bin",true));
	}

	else if(key == 'f') {
		ofToggleFullscreen();
	}
	else if(key == 'r') {
		settings.rotate_scene = !settings.rotate_scene;
	}
	else if(key == 't') {
		printf("Width: %d Height: %d\n", ofGetWidth(), ofGetHeight());
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	if(show_gui) {
		#ifdef USE_FLOCK_GUI
		flock_gui.onMouseMoved(x,y);
		#endif
		#ifdef USE_LIGHT_RAYS
		rays_gui.onMouseMoved(x,y);
		#endif
		ia_gui.onMouseMoved(x,y);
		bloom_gui.onMouseMoved(x,y);
	}

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

	if(show_gui) {
		#ifdef USE_FLOCK_GUI
		flock_gui.onMouseMoved(x,y);
		#endif
		#ifdef USE_LIGHT_RAYS
		rays_gui.onMouseMoved(x,y);
		#endif
		ia_gui.onMouseMoved(x,y);
		bloom_gui.onMouseMoved(x,y);
	}

	
	if(settings.rotate_scene) {
		cam.onMouseDragged(x,y);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	if(show_gui) {
		#ifdef USE_FLOCK_GUI
		flock_gui.onMouseDown(x,y);
		#endif
		#ifdef USE_LIGHT_RAYS
		rays_gui.onMouseDown(x,y);
		#endif
		ia_gui.onMouseDown(x,y);
		bloom_gui.onMouseDown(x,y);
	}

	
	if(settings.rotate_scene) {
		cam.onMouseDown(x,y);
	}
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	if(show_gui) {
		#ifdef USE_FLOCK_GUI
		flock_gui.onMouseUp(x,y);
		#endif
		#ifdef USE_LIGHT_RAYS
		rays_gui.onMouseUp(x,y);
		#endif
		ia_gui.onMouseUp(x,y);
		bloom_gui.onMouseUp(x,y);
	}

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	room.setAspect((float)w/(float)h);
	app.resize(w, h);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}