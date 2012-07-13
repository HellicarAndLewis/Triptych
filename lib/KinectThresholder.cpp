/**
 *  KinectOutline.cpp
 *
 *  Created by Marek Bereza on 25/06/2012.
 */

#include "KinectThresholder.h"
#include "ofxSimpleGuiToo.h"
#ifdef _WIN32
#include "ofxKinectNuiDraw.h"
#endif
#include "TimeProfiler.h"
using namespace tricks::util;

void KinectThresholder::setup(bool useSkellingtons) {
#ifdef _WIN32
	/*ofxKinectNui::InitSetting initSetting;
	initSetting.grabVideo = true;
	initSetting.grabDepth = true;
	initSetting.grabAudio = false;
	initSetting.grabLabel = false;
	initSetting.grabSkeleton = true;
	initSetting.grabCalibratedVideo = false;
	initSetting.grabLabelCv = false;
	initSetting.depthResolution = NUI_IMAGE_RESOLUTION_640x480;
	initSetting.videoResolution = NUI_IMAGE_RESOLUTION_640x480;
	kinect.init(initSetting);
	kinect.open(false);*/

	kinect.setup(useSkellingtons);

	VISION_WIDTH  = 640;
	VISION_HEIGHT = 480;


//	ofxKinectNuiDrawSkeleton *skeletonDraw_ = new ofxKinectNuiDrawSkeleton();
//	kinect.setSkeletonDrawer(skeletonDraw_);


#else
	
	kinect.init();
	kinect.setRegistration(true);
	VISION_WIDTH  = 640;
	VISION_HEIGHT = 480;
	kinect.open();
#endif
	
		
	
	foundBlobs = false;

	blobTracker.addListener(&blobEvents);
	
	rgb.allocate(VISION_WIDTH, VISION_HEIGHT);
	depth.allocate(VISION_WIDTH, VISION_HEIGHT);
	background.allocate(VISION_WIDTH, VISION_HEIGHT);
	thresh.allocate(VISION_WIDTH, VISION_HEIGHT);
	
	hysteresis = 3;
	nearThreshold = 100;
	farThreshold = 200;
	erosions = 0;
	dilations = 0;
	blurs = 0;
	blurSize = 1;
	learnBackground = true;
#ifdef _WIN32
	doingSkeleton = useSkellingtons;
#endif
}

void KinectThresholder::setupGui() {
	gui.addContent("rgb", rgb);
	gui.addContent("bg", background);
	gui.addContent("depth", depth);
	gui.addContent("thresh", thresh);
	gui.addSlider("near threshold", nearThreshold, 0, 255);
	gui.addSlider("far threshold", farThreshold, 0, 255);
	gui.addSlider("hysteresis", hysteresis, 0, 20);
	gui.addToggle("learnBackground", learnBackground);
	gui.addSlider("erosions", erosions, 0, 5);
	gui.addSlider("dilations", dilations, 0, 5);
	gui.addSlider("blurs", blurs, 0, 5);
	gui.addSlider("blurSize", blurSize, 1, 5);

}

#ifdef _WIN32
void KinectThresholder::doSkeletons() {
	
	numSkeletons = kinect.getSkeletonPoints(skelly);
	//printf("Doing skels %d\n", numSkeletons);
}
#endif

#ifdef _WIN32
int KinectThresholder::getNumSkeletons() {



	
	// stuff goes here
	//printf("Skellies: %d\n", numSkeletons);
	return numSkeletons;

}
#endif


#ifdef _WIN32
void KinectThresholder::getSkeleton(int index, KinectSkeleton &s) {

	

	// stuff goes here

	s.id = index;
	for(int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++) {
		s.points[i] = ofVec3f(skelly[index][i]);
	}

	//s.leftHand = ofVec3f(skelly[index][NUI_SKELETON_POSITION_HAND_LEFT].x, skelly[index][NUI_SKELETON_POSITION_HAND_LEFT].y, skelly[index][NUI_SKELETON_POSITION_HAND_LEFT].z);
	//s.rightHand = ofVec3f(skelly[index][NUI_SKELETON_POSITION_HAND_RIGHT].x, skelly[index][NUI_SKELETON_POSITION_HAND_RIGHT].y, skelly[index][NUI_SKELETON_POSITION_HAND_RIGHT].z);
	//s.head = ofVec3f(skelly[index][NUI_SKELETON_POSITION_HEAD].x, skelly[index][NUI_SKELETON_POSITION_HEAD].y, skelly[index][NUI_SKELETON_POSITION_HEAD].z);

}
#endif


bool KinectThresholder::update() {
	ScopedTimer updateTime("KinectThresh::update()");
	
	
	
#ifdef _WIN32
	{
		ScopedTimer kinectUpdate("kinectNui::update()");
		kinect.update();
	}
#else
	kinect.update();
#endif

	//printf("Time Taken %.2f ms\n", (ofGetElapsedTimef() - t)*1000);
	if(kinect.isFrameNew()) {

		ReusableTimer timer;

		timer.start("acquire");
		rgb.setFromPixels(kinect.getPixels(), VISION_WIDTH, VISION_HEIGHT);
		foundBlobs = false;

		depth.setFromPixels(kinect.getDepthPixels(), VISION_WIDTH, VISION_HEIGHT);

		if(learnBackground) {
			learnBackground = false;
			background = depth;
		}
		int numPix = VISION_WIDTH * VISION_HEIGHT;
		unsigned char *bg = background.getPixels();
		unsigned char *d = depth.getPixels();
		for(int i = 0; i < numPix; i++) {
		
			if(d[i]>nearThreshold) {
				d[i] = 0;
			} else if(d[i]<farThreshold) {
				d[i] = 0;
			} else if(d[i]<bg[i]+hysteresis) {
				d[i] = 0;
			}
		}
		
		thresh.setFromPixels(d, VISION_WIDTH, VISION_HEIGHT);
		//thresh.convertToRange(nearThreshold, farThreshold);
		
		timer.start("process");
		for(int i = 0; i < erosions; i++) {
			thresh.erode_3x3();
		}
		for(int i = 0; i < dilations; i++) {
			thresh.dilate_3x3();
		}
		for(int i = 0; i < blurs; i++) {
			thresh.blur(blurSize*2+1);
		}
		
#ifdef _WIN32
		timer.start("Skellies");
		if(doingSkeleton) {
			doSkeletons();
		} else {
			//printf("Dropped frame\n");
		}
#endif
		timer.stop();
		return true;
	}
	return true;
}


void KinectThresholder::drawDebug() {
	thresh.draw(0, 0);
	if(foundBlobs) {
		contourFinder.draw();
		
		map<int,BoundBlob>::iterator it;
		for(it = people.begin(); it != people.end(); it++) {
			(*it).second.draw();
			ofDrawBitmapString(ofToString((*it).first), (*it).second.centroid);
			
		}
	}
	#ifdef _WIN32
	if(doingSkeleton) {


	//	kinect.drawSkeleton(0, 0, 1024, 768);	// draw skeleton images on video images

	}
	#endif
}

ofxCvGrayscaleImage &KinectThresholder::getOutline() {
	return thresh;
}


vector<ofxCvBlob> &KinectThresholder::getContours() {
	if(!foundBlobs) {
		
		int minBlobSize = 40;
		float w = 640;
		float h = 480;
#ifndef _WIN32
		w = kinect.getWidth();
		h = kinect.getHeight();
#endif
		int maxBlobSize = h;
		
		contourFinder.findContours(thresh, minBlobSize*minBlobSize, maxBlobSize*maxBlobSize, 20, false, true);
		

		foundBlobs = true;
	}
	return contourFinder.blobs;
}

void KinectThresholder::trackBlobs() {
	
	//	people.clear();
	
	float w = 640;
	float h = 480;
#ifndef _WIN32
		w = kinect.getWidth();
		h = kinect.getHeight();
#endif
	// run the blob tracker
	vector<ofVec3f> blobs;
	ofVec2f dims(w,h); 
	vector<ofxCvBlob> &contoursBlobs = getContours();
	// use the z coordinate.
	for(int i = 0; i < contoursBlobs.size(); i++) {
		ofVec3f b = ofVec3f(contoursBlobs[i].centroid.x/(float)w, 
							contoursBlobs[i].centroid.y/(float)h, i);
		blobs.push_back(b);
	}
	
	
	
	blobTracker.track(blobs);
	
	ofxBlobEvent e;
	while(blobEvents.getNextEvent(e)) {
		if(e.eventType==ofxBlobTracker_entered) {
			people[e.blobId] = BoundBlob();
			people[e.blobId].init(contoursBlobs[(int)e.pos.z], e.blobId);
			people[e.blobId].setDepth(getDepth(contoursBlobs[(int)e.pos.z]));
			

			for(int i = 0; i < listeners.size(); i++) {
				listeners[i]->boundBlobEntered(people[e.blobId]);
			}
			/*if(recording) {
			 ofVec3f bounds(640,480, 255);
			 
			 ofVec3f f = people[e.blobId].left/bounds;
			 anim.addFrame("hand_left", (const float*)&f.x);
			 
			 f = people[e.blobId].right/bounds;
			 
			 anim.addFrame("hand_right", (const float*)&f.x);
			 
			 f = people[e.blobId].top/bounds;
			 anim.addFrame("top", (const float*)&f.x);
			 
			 f = people[e.blobId].bottom/bounds;
			 anim.addFrame("bottom", (const float*)&f.x);
			 
			 f = people[e.blobId].centroid/bounds;
			 anim.addFrame("centroid", (const float*)&f.x);
			 }*/
		} else if(e.eventType==ofxBlobTracker_moved) {
			people[e.blobId].update(contoursBlobs[(int)e.pos.z]);
			
			for(int i = 0; i < listeners.size(); i++) {
				listeners[i]->boundBlobMoved(people[e.blobId]);
			}
			/*if(recording) {
			 ofVec3f bounds(640,480, 255);
			 
			 ofVec3f f = people[e.blobId].left/bounds;
			 anim.addFrame("hand_left", (const float*)&f.x);
			 
			 f = people[e.blobId].right/bounds;
			 anim.addFrame("hand_right", (const float*)&f.x);
			 
			 f = people[e.blobId].top/bounds;
			 anim.addFrame("top", (const float*)&f.x);
			 
			 f = people[e.blobId].bottom/bounds;
			 anim.addFrame("bottom", (const float*)&f.x);
			 
			 f = people[e.blobId].centroid/bounds;
			 anim.addFrame("centroid", (const float*)&f.x);
			 }
			 */
			
		} else if(e.eventType==ofxBlobTracker_exited) {
			for(int i = 0; i < listeners.size(); i++) {
				listeners[i]->boundBlobExited(people[e.blobId]);
			}
			people.erase(e.blobId);
		}
	}
	
	
	
	/*if(contours.blobs.size()==people.size()) { // match the nearest
	 for(int i = 0; i < contours.blobs.size(); i++) {
	 people.push_back(BoundBlob());
	 people.back().init(contours.blobs[i]);
	 }
	 } else if() {
	 
	 }*/
}


void KinectThresholder::addListener(BoundBlobListener *listener) {
	listeners.push_back(listener);
}


unsigned char *KinectThresholder::getPixels() {
	return kinect.getPixels();
}

unsigned char KinectThresholder::getDepth(const ofxCvBlob &blob) {
	unsigned char *c = depth.getPixels();
	unsigned char p = 0;
	ofRectangle r = blob.boundingRect;
	for(int x = r.x; x < r.x+r.width; x++) {
		for(int y = r.y; y < r.y+r.height; y++) {
			unsigned char pix = c[x + (int)(y*r.width)];
			if(pix>p) p = pix;
		}
	}
	
	return p;
}

