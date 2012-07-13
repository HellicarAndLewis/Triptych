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
	int ticksPerBuffer = 8;
	pd = new ofxPd();
	if(!pd->init(2, 0, 44100, ticksPerBuffer)) {
		printf("Could not start ofxPd!\n");
	} else {
		printf("Running ofxPd\n");
	}
	
	pd->openPatch("../../../pd/"+appName+".pd");
	pd->start();
	// make sure to start the background loop
	pd->sendFloat("panBg", 0.5);
	pd->sendFloat("onBg", 1.0);
	

	ofSoundStreamSetup(2, 0, this, 44100, ofxPd::blockSize()*ticksPerBuffer, 3);
	printf("AudioSystem::setup()\n");
	ofSetLogLevel(OF_LOG_WARNING);
	
}

void AudioSystem::audioOut(float *buff, int buffSize, int channels) {
	//printf("Bum\n");
	pd->audioOut(buff, buffSize, channels);
	
}



/**
 * There's 5 channels - [0-4]
 */
void AudioSystem::enable(int channel) {
	if(channel<0 || channel>4) {
		ofLog(OF_LOG_ERROR, "AudioSystem::enable() - you can only use channels 0-4 - asking or channel %d\n", channel);
		return;
	}
	pd->sendFloat("on"+ofToString(channel), 1.0);
}

void AudioSystem::disable(int channel) {
	if(channel<0 || channel>4) {
		ofLog(OF_LOG_ERROR, "AudioSystem::disable() - you can only use channels 0-4 = asking for channel %d\n", channel);
		return;
	}
	pd->sendFloat("on"+ofToString(channel), 0.0);	
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
	pd->sendFloat("pan"+ofToString(channel), pan);
}

