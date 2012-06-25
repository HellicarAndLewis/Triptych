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
 *  ofxKinectInpainterFloat.h, created by Marek Bereza on 16/02/2012.
 */



#pragma once
#include "ofxCvFloatImage.h"

class ofxKinectInpainterFloat {
public:
	ofxKinectInpainterFloat();
	/**
	 * The amount of downsampling will determine the quality.
	 * 4 is quarter-sized, but real-time.
	 */
	void setup(int width = 640, int height = 480, int downsampling = 8);
	
	/**
	 * This is a parameter of the inpainting algorithm, the higher the better
	 * but the higher the most computationally expensive.
	 */
	void setInpaintingRadius(int radius);
	void setDownsampling(int downsampling);
	
	int getInpaintingRadius();
	int getDownsampling();
	
	void inpaint(ofxCvFloatImage &img);
private:
	ofxCvFloatImage scaled;
	ofxCvFloatImage mask;
	ofxCvFloatImage scaledMask;
	
	ofxCvFloatImage inpainted;
	float scale;
	int width;
	int height;
	static const int DEFAULT_INPAINT_RADIUS = 3;
	int inpaintRadius;
};
