//
//  MarekTrail.h
//  Intel_Design
//
//  Created by Will Gallia on 10/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "ofMain.h"

class MarekTrail {
	
public:
	MarekTrail();
	~MarekTrail();
	
	void update();
	void draw();
	
	void setInput(ofVec3f p) { input = p; }
	void setInput(ofVec2f p) { input.set(p.x, p.y, 0); }
	
private:
	ofFbo fbo;
	ofImage thing;
	
	ofVec3f point, input;
	
	int iterations;
	float m;
	
};
