/**
 *  KinectOutline.cpp
 *
 *  Created by Marek Bereza on 25/06/2012.
 */

#include "KinectThresholder.h"
#include "ofxSimpleGuiToo.h"
void KinectThresholder::setup() {
#ifdef _WIN32
	kinect.init(//bool grabVideo = 
				true,
				//bool grabDepth = 
				true,
				//bool grabAudio = 
				false,
				//bool grabLabel = 
				false,
				//bool grabSkeleton = 
				false,
				//bool grabCalibratedVideo = 
				//true,
				false,
				//bool grabLabelCv = 
				false,
				//NUI_IMAGE_RESOLUTION videoResolution = 
				NUI_IMAGE_RESOLUTION_640x480,
				//NUI_IMAGE_RESOLUTION depthResolution = 
				NUI_IMAGE_RESOLUTION_640x480);
	VISION_WIDTH  = 640;
	VISION_HEIGHT = 480;
#else
	
	kinect.init();
	kinect.setRegistration(true);
	VISION_WIDTH  = 640;
	VISION_HEIGHT = 480;
#endif
	kinect.open();
		
	foundBlobs = false;
	listener = NULL;
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

bool KinectThresholder::update() {
	float t = ofGetElapsedTimef();
	
	
	
#ifdef _WIN32
	kinect.update(ofxKinectNui::UPDATE_FLAG_VIDEO	
		| ofxKinectNui::UPDATE_FLAG_DEPTH	 
		//| ofxKinectNui::UPDATE_FLAG_CALIBRATED_VIDEO
		);
#else
	kinect.update();
#endif

	//printf("Time Taken %.2f ms\n", (ofGetElapsedTimef() - t)*1000);
	if(kinect.isFrameNew()) {
		rgb.setFromPixels(kinect.getPixels(), VISION_WIDTH, VISION_HEIGHT);
		foundBlobs = false;
#ifdef _WIN32
		depth.setFromPixels(kinect.getDepthPixels().getPixels(), VISION_WIDTH, VISION_HEIGHT);
#else
		depth.setFromPixels(kinect.getDepthPixels(), VISION_WIDTH, VISION_HEIGHT);
#endif
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
		
		
		for(int i = 0; i < erosions; i++) {
			thresh.erode_3x3();
		}
		for(int i = 0; i < dilations; i++) {
			thresh.dilate_3x3();
		}
		for(int i = 0; i < blurs; i++) {
			thresh.blur(blurSize*2+1);
		}


		
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
			
			if(listener!=NULL) {
				listener->boundBlobEntered(people[e.blobId]);
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
			
			if(listener!=NULL) {
				listener->boundBlobMoved(people[e.blobId]);
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
			if(listener!=NULL) {
				listener->boundBlobExited(people[e.blobId]);
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


void KinectThresholder::setListener(BoundBlobListener *listener) {
	this->listener = listener;
}


unsigned char *KinectThresholder::getPixels() {
#ifdef _WIN32
	return kinect.getVideoPixels().getPixels();
#else
	return kinect.getPixels();
#endif
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