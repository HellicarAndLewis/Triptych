/**     ___           ___           ___                         ___           ___     
 *     /__/\         /  /\         /  /\         _____         /  /\         /__/|    
 *    |  |::\       /  /::\       /  /::|       /  /::\       /  /::\       |  |:|    
 *    |  |:|:\     /  /:/\:\     /  /:/:|      /  /:/\:\     /  /:/\:\      |  |:|    
 *  __|__|:|\:\   /  /:/~/::\   /  /:/|:|__   /  /:/~/::\   /  /:/  \:\   __|__|:|    
 * /__/::::| \:\ /__/:/ /:/\:\ /__/:/ |:| /\ /__/:/ /:/\:| /__/:/ \__\:\ /__/::::\____
 * \  \:\~~\__\/ \  \:\/:/__\/ \__\/  |:|/:/ \  \:\/:/~/:/ \  \:\ /  /:/    ~\~~\::::/
 *  \  \:\        \  \::/          |  |:/:/   \  \::/ /:/   \  \:\  /:/      |~~|:|~~ 
 *   \  \:\        \  \:\          |  |::/     \  \:\/:/     \  \:\/:/       |  |:|   
 *    \  \:\        \  \:\         |  |:/       \  \::/       \  \::/        |  |:|   
 *     \__\/         \__\/         |__|/         \__\/         \__\/         |__|/   
 *
 *  Description: 
 *				 
 *  KinectThresholder.h, created by Marek Bereza on 25/06/2012.
 */

#pragma once


#include "ofxOpenCv.h"
#ifdef _WIN32
#	include "ofxKinectNui.h"
#else
#	include "ofxKinect.h"
#endif
class KinectThresholder {
public:
	
	void setup();
	
	void setupGui();

	
	// call this every frame.
	// it returns false if it could 
	// not update from the kinect
	// (i.e. there wasn't a new frame yet)
	bool update();
	
	vector<ofxCvBlob> &getContours();
	
	
/*
 
 
 // your update function should look something like this.
 
 
 if(kinectThresholder.update()) {
 
	// get your contours
	vector<ofxCvBlob> &contours = kinectThresholder.getContours();
 
	// loop through each contour and create a kinect mesh from each one.
	for(int i = 0; i < contours.size(); i++) {
		KinectMesh mesh;
	
 
		if(mesh.setup(contours[i], kinectThresholder)) {
 
	
			// do what you will with the contour.
 
			mesh.this();
			mesh.that();
			mesh.whatnot();
 
 
 
		} else {
			// if mesh.setup() returns false, it means the contour is too
			// small to bother turning into a contour.
			continue;	
		}
	}
 }
 
 
 
 
 
 
 */
	
	
	void drawDebug();

	ofxCvGrayscaleImage &getOutline();

	int getWidth() {   /*return kinect.getWidth();*/ return VISION_WIDTH; }
	int getHeight() { /*return kinect.getHeight();*/ return VISION_HEIGHT; }
	
	unsigned char getDepth(const ofxCvBlob &blob);
	unsigned char *getPixels();
	
	
	
	
#ifdef _WIN32
	ofxKinectNui kinect;
#else
	ofxKinect kinect;
#endif
	ofxCvGrayscaleImage depth;
	ofxCvGrayscaleImage thresh;
	ofxCvGrayscaleImage background;
	ofxCvContourFinder  contourFinder;
	
	bool learnBackground;
	float nearThreshold;
	float farThreshold;
	int hysteresis;
	
	int erosions;
	int dilations;
	int blurs;
	int blurSize;
	
	
	int VISION_WIDTH;
	int VISION_HEIGHT;
};
