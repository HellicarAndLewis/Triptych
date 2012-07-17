//
//  Ribbon.h
//  gg
//
//  Created by Will Gallia on 13/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//


#pragma once
#include "ofMain.h"


extern float frontTaper;
extern float backTaper;
extern bool drawPink;
extern int ribbonLength;
extern bool fadeInZ;

class RibbonSegment : public ofNode {
public:
//	void set(RibbonSegment *seg) {
//		set
//	}
	
	void update(RibbonSegment nextSegment) {
		setPosition(getPosition() + (nextSegment.getPosition() - getPosition()) * 0.05);
	}
};


class Ribbon {
public:
	Ribbon();
	~Ribbon();
	
	void update(ofVec3f p);
	void draw();
	RibbonSegment head;
	
	void flong();
	void getVels();
	bool initial;
	
	deque<RibbonSegment*> segments;
	
	float maxThickness;
	float minThickness;
	
	ofVboMesh mesh;

protected:
	float lastUpdate;
	
};

