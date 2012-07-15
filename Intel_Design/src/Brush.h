//
//  Brush.h
//  Intel_Design
//
//  Created by Will Gallia on 10/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "ofMain.h"

class Brush {
	
public:
	Brush();
	Brush(ofVec3f start);
	~Brush();
	
	//this is a wrapper for setInput
	void update(ofVec3f p);
	void draw();
	
	void setPoint(ofVec3f p) { point = p; }

	void setInput(ofVec3f p) { input = p; }
	void setInput(ofVec2f p) { input.set(p.x, p.y, 0); }
	
private:
	ofFbo fbo;
	ofImage thing;
	
	ofVec3f point, input;
	
//	int iterations;
//	float m;
	
};

extern int iterations;
extern float m;
extern float maxImageSize;
extern float minImageSize;
//extern bool enlargeImage;