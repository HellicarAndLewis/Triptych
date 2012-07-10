//
//  Trail.h
//  gg
//
//  Created by Will Gallia on 09/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#pragma once

#include "ofMain.h"


class Trail {
public:
	Trail();
	~Trail();
	
	void update();
	void draw();
	
	void input(ofVec3f p);
	
	
	
	//put these here so they can be manipulated by the GUI
	int trailMaxLength;
	
	int reductionSpeed;
	float interpolationFactor;
	
	bool drawInfo, drawWireframe;
	
	float sineCounter, sineMultiplier, sineIncrement;
	
private:
	deque<ofNode> trail;
	
	
	ofVboMesh mesh;
	
	ofImage texImage, texImage2;
	
	ofVec3f headPos;
	
	
	ofFbo fbo;
};