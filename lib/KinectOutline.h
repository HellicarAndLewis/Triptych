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
 *  KinectOutline.h, created by Marek Bereza on 25/06/2012.
 */

#pragma once
#include "ofxOpenCv.h"
#ifdef _WIN32
#include "ofxKinectNui.h"
#else
#include "ofxKinect.h"
#endif
class KinectOutline {
public:
	
	void setup();
	
	void setupGui();

	bool update();
	
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
