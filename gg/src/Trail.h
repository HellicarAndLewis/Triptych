//
//  Trail.h
//  gg
//
//  Created by Will Gallia on 09/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#pragma once

#include "ofMain.h"

class cNode : public ofNode {
public:
	void setColour(ofFloatColor c) { colour = c; }
	ofFloatColor getColour() { return colour; }
	
	void setWidth(float w) { width = w; }
	float getWidth() { return width; }
	
	void setVelocity(float v) { vel = v; }
	
	
//private: fuck proper encapsulation...
	ofFloatColor colour;
	float width;
	float vel;
};


class Trail {
public:
	Trail();
	~Trail();
	
	void update();
	void draw();
	
	void input(ofVec3f p);
	
	float z;
	
	
	//put these here so they can be manipulated by the GUI
	int trailMaxLength;
	
	float reductionSpeed;
	float interpolationFactor;
	
	bool drawInfo, drawWireframe;
	
	float sineCounter, sineMultiplier, sineIncrement;
	
	float m;
	float widthMultiplier;
	bool inverseWidth;
	int interpolationAmount;
	
	bool useVel;
	
	float width;
	ofFloatColor colour;
	
private:
	deque<cNode> trail;
	
	
	ofVboMesh mesh;
	
	ofImage texImage, texImage2;
	
	ofVec3f headPos;
	
	
	ofFbo fbo;
};