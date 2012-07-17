//
//  AudioSystem.cpp
//  Performance
//

#include "AudioSystem.h"





void AudioSystem::boundBlobEntered(const BoundBlob &blob) {
	int pos = (blob.id%5);
	enable(pos);
	setPan(pos, blob.centroid.x/640.f);
	
}
void AudioSystem::boundBlobMoved(const BoundBlob &blob) {
	int pos = (blob.id%5);
	setPan(pos, blob.centroid.x/640.f);
}
void AudioSystem::boundBlobExited(const BoundBlob &blob) {
	int pos = (blob.id%5);
	disable(pos);	
}


void AudioSystem::setup(string appName, KinectThresholder &kinect) {
	kinect.addListener(this);
	ofSetLogLevel(OF_LOG_VERBOSE);
	osc.setup("localhost", 12345);

	
	ofxOscMessage m;
	m.setAddress("/bg/on");
	osc.sendMessage(m);

	ofSetLogLevel(OF_LOG_WARNING);
	
}

void AudioSystem::audioOut(float *buff, int buffSize, int channels) {
	//printf("Bum\n");
	//pd->audioOut(buff, buffSize, channels);
	
}



/**
 * There's 5 channels - [0-4]
 */
void AudioSystem::enable(int channel) {
	if(channel<0 || channel>4) {
		ofLog(OF_LOG_ERROR, "AudioSystem::enable() - you can only use channels 0-4 - asking or channel %d\n", channel);
		return;
	}
	ofxOscMessage m;
	m.setAddress("/"+ofToString(channel+1)+"/on");
	osc.sendMessage(m);

	//pd->sendFloat("on"+ofToString(channel), 1.0);
}

void AudioSystem::disable(int channel) {
	if(channel<0 || channel>4) {
		ofLog(OF_LOG_ERROR, "AudioSystem::disable() - you can only use channels 0-4 = asking for channel %d\n", channel);
		return;
	}

	ofxOscMessage m;
	m.setAddress("/"+ofToString(channel+1)+"/off");
	osc.sendMessage(m);

	//pd->sendFloat("on"+ofToString(channel), 0.0);	
}


/**
 * 0 is left, 1 is right.
 */
void AudioSystem::setPan(int channel, float pan) {
	if(channel<0 || channel>4) {
		ofLog(OF_LOG_ERROR, "AudioSystem::setPan() - you can only use channels 0-4\n");
		return;
	}
	
	pan = ofClamp(pan, 0, 1);
	ofxOscMessage m;
	m.setAddress("/"+ofToString(channel+1)+"/pan");
	m.addFloatArg(pan);
	osc.sendMessage(m);


	//pd->sendFloat("pan"+ofToString(channel), pan);
}

