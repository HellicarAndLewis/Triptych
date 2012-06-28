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
#include "ofxKinect.h"

class KinectOutline {
public:
	
	void setup();
	
	void setupGui();

	bool update();
	
	void drawDebug();

	ofxCvGrayscaleImage &getOutline();

	int getWidth() { return kinect.getWidth(); }
	int getHeight() { return kinect.getHeight(); }
	
	unsigned char getDepth(const ofxCvBlob &blob);
	
	
	
	ofxKinect kinect;
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
