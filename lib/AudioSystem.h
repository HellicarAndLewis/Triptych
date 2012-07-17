//
//  AudioSystem.h
//  Performance
//

#pragma once

#include "ofMain.h"
//#include "ofxPd.h"
#include "KinectThresholder.h"
#include "ofxOsc.h"

class AudioSystem: public ofBaseApp, public BoundBlobListener {
public:
	
	
	
	
	
	
	
	void boundBlobEntered(const BoundBlob &blob);
	void boundBlobMoved(const BoundBlob &blob);
	void boundBlobExited(const BoundBlob &blob);

	
	
	
	
	void setup(string appName, KinectThresholder &kinect);
	
	
	
	/**
	 * There's 5 channels - [0-4]
	 */
	void enable(int channel);
	
	void disable(int channel);
	
	
	/**
	 * 0 is left, 1 is right.
	 */
	void setPan(int channel, float pan);
	
	void audioOut(float *buff, int buffSize, int channels);
private:
	ofxOscSender osc;
	//ofxPd *pd;
};