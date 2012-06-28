#include "testApp.h"

testApp::testApp()
	:gui("Settings", 300)
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

	glEnable(GL_TEXTURE_2D);
	
	cam.setup(ofGetWidth(), ofGetHeight());
	ax.setup(10);
	
	/*
	vector<string> options;
	options.push_back("Add particles");
	options.push_back("Select particles");
	options.push_back("Create strength");
	modes = &gui.addRadio<testApp>("Mode", 1, this, options, creator.mode);
	*/
	printf("USE: %d\n", app_settings.use_perlin);
	gui.addBool("Enable perlin noise", app_settings.use_perlin);
	gui.addFloat("Min particle size", app_settings.min_particle_size).setMin(0.01f).setMax(3.0f);
	gui.addFloat("Max particle size", app_settings.max_particle_size).setMin(0.01).setMax(5.0f);
	gui.addFloat("Perlin influence", app_settings.perlin_influence).setMin(0.0).setMax(0.5);
	gui.addFloat("Perlin scale", app_settings.perlin_scale).setMin(0.0).setMax(1.0);
	gui.addButton<testApp>("save",0,this);
	gui.load(File::toDataPath("gui.bin"));
	
	
	creator.setup();
	
	ss.setup();
	debug = false;
	
	cam.translate(0,4,5);
	
//	
//	ofImage particle_img;
//	if(!particle_img.loadImage("particle_trans.png")) {
//		printf("Cannot load particle image.\n");
//		::exit(0);
//	}
//	Texture particle_tex;
//	particle_tex.setPixels(particle_img.getPixels(), particle_img.getWidth(), particle_img.getHeight(), GL_RGBA);
//	bb.setTexture(particle_tex.getID());
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
	ss.update();
}

//--------------------------------------------------------------
void testApp::draw(){
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-20);
//	Vec3 right, up;
//	cam.place();
//	cam.getBillboardVectors(right, up);
	//sax.draw();
//	bb.start(cam.pm(), cam.vm(), right, up);
//	Vec3 p(0,3,0);
//	bb.draw(p, 3.0, 0.0);
//	bb.stop();
//	ss.draw(cam.pm(), cam.vm(), right, up);
//	return;

	if(!debug) {
		Vec3 right, up;
		cam.getBillboardVectors(right, up);
		creator.draw(cam.pm(), cam.vm());
		ss.draw(cam.pm(), cam.vm(), right, up);
	}
	else {
		cam.place();
		
		creator.debugDraw();
		ss.debugDraw();
	}
	gui.draw();
	
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