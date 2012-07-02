#include "testApp.h"

#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
ofxCvGrayscaleImage depthImg;
ofxCvGrayscaleImage currImg;
int depthMin = 0;
int depthMax = 255;
int blurs = 2;
int blurSize = 2;
float accum = 0.5;
float gain = 1;
float atten = 1;
//--------------------------------------------------------------
void testApp::setup(){
	
	kinect.init();
	kinect.open();
	
	gui.addSlider("Depth Min", depthMin, 0, 255);
	gui.addSlider("Depth Max", depthMax, 0, 255);
	gui.addSlider("Blurs", blurs, 0, 5);
	gui.addSlider("Blur Size", blurSize, 1, 5);
	gui.addContent("depth img", depthImg);
	gui.addContent("curr img", currImg);
	gui.addSlider("accum", accum, 0, 1);
	gui.addSlider("gain", gain, 0.5, 1.5);
	gui.addSlider("atten", atten, 0.5, 1.5);
	gui.loadFromXML();
	gui.setAutoSave(true);
	
	depthMax = 100;
	gui.setAlignRight(true);
	
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	int size = kinect.getWidth()*kinect.getHeight()*3;
	for(int i = 0; i < 30; i++) {
		unsigned char *buff = new unsigned char[size];
		memset(buff, 0, size);
		framebuffer.push_back(buff);
	}
	tex.allocate(kinect.getWidth(), kinect.getHeight(), GL_RGB);
	pix = new unsigned char[size];
	
	depthImg.allocate(kinect.getWidth(), kinect.getHeight());
	currImg.allocate(kinect.getWidth(), kinect.getHeight());
}

//--------------------------------------------------------------
void testApp::update(){


	kinect.update();
	
	int numPix = kinect.getWidth()*kinect.getHeight()*3;
	
	if(kinect.isFrameNew()) {
		// save rgb to buffer
		unsigned char *frame = framebuffer.back();
		memcpy(frame, kinect.getPixels(), numPix);
		framebuffer.pop_back();
		framebuffer.push_front(frame);
		
		numPix/=3;
		
		depthImg.setFromPixels(kinect.getDepthPixels(), kinect.getWidth(), kinect.getHeight());
		
		
		
		for(int i = 0; i < blurs; i++) {
			depthImg.blur(blurSize*2+1);
		}
		
		unsigned char *depth = depthImg.getPixels();
		
		unsigned char *frm = currImg.getPixels();
		float ml = (1.f - accum)*gain;
		for(int i = 0; i < numPix; i++) {
			frm[i] = ofClamp((frm[i]*accum + depth[i]*ml)*atten, 0, 255)	;

		}
		currImg.setFromPixels(frm, kinect.getWidth(), kinect.getHeight());
		
		for(int i = 0; i < numPix; i++) {
			int r = ofMap(frm[i], depthMin, depthMax, 0, 29, true);
			
			pix[i*3] = framebuffer[r][i*3];
			pix[i*3+1] = framebuffer[r][i*3+1];
			pix[i*3+2] = framebuffer[r][i*3+2];
		}
		tex.loadData(pix, kinect.getWidth(), kinect.getHeight(), GL_RGB);
	}

	
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
}


//--------------------------------------------------------------
void testApp::draw(){
	tex.draw(0,0, ofGetWidth(), ofGetHeight());
	gui.draw();
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

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}