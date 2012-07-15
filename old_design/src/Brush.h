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
	~Brush();
	
	void update();
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