//
//  Trail.h
//  gg
//
//  Created by Will Gallia on 09/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#pragma once

#include "ofMain.h"

class Node {
public:
	ofVec3f pos;
	ofVec3f getPosition() { return pos; }
	void setPosition(ofVec3f p) { pos = p; }

	ofFloatColor col;
	ofFloatColor getColor() { return col; }
	void setColor(ofFloatColor c) { col = c; }
};

class Trail {
public:
	Trail(ofVec3f p);
	~Trail();
	
	void update(ofVec3f p);
	void draw();
	
	void input(ofVec3f p);
	void clear();
	
	
	//put these here so they can be manipulated by the GUI
	int trailMaxLength;
	
	int reductionSpeed;
	float interpolationFactor;
	
	bool drawInfo, drawWireframe;
	
	float sineCounter, sineMultiplier, sineIncrement;
	
private:
	deque<Node> trail;
	
	
	ofVboMesh mesh;
	
	ofImage texImage, texImage2;
	
	ofVec3f headPos;
	
	
//	ofFbo fbo;
};

extern float maxImageSize;
extern float trailWidth;
extern bool variableWidth;
extern float variableFactor;
extern bool useGravity;
extern float gravityFactor;
extern bool useFade;