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


#include "ofMain.h"


#include "ofxKinectNui.h"

// this isn't the number of skeletons that can be tracked
// its just the internal size for how many skeletons should
// be stored. It's slightly too big, but it doesn't matter, 
// the footprint is tiny.
#define MAX_NUM_SKELS 64

class KinectNuiThreaded: public ofThread {
public:
	KinectNuiThreaded();
	~KinectNuiThreaded();
	void setup(bool useSkellingtons = false);
	void update();
	bool isFrameNew();
	
	
	


	int getSkeletonPoints(const ofPoint *points[]);
	unsigned char *getPixels();
	unsigned char *getDepthPixels();
protected:
	void threadedFunction();

private:
	int numSkeletons;
	const ofPoint *skeletonPoints[MAX_NUM_SKELS];
	unsigned char *depth;
	unsigned char *rgb;
	
	ofxKinectNui kinect;
	bool doingSkeletons;

	bool gotNewFrame;
};
