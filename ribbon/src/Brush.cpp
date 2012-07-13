//
//  Brush.cpp
//  Intel_Design
//
//  Created by Will Gallia on 10/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "Brush.h"

Brush::Brush() { 

	thing.loadImage("thing.png");
	thing.setAnchorPercent(0.5, 0.5);

}

Brush::Brush(ofVec3f start) {
	thing.loadImage("thing.png");
	thing.setAnchorPercent(0.5, 0.5);

	point = start;
}

Brush::~Brush() { 
	
}

void Brush::update(ofVec3f p) {
	setInput(p);
}

void Brush::draw() {
		
	for(int i = 0; i < iterations; i++) {
		
		ofVec3f lastPoint = point;
		point = point*(1-m) + input*m;
		
		float vel = (point - lastPoint).lengthSquared();
		ofSetHexColor(0xffffff);
		
		
		if (enlargeImage) vel = ofMap(vel, 0, 3, minImageSize, maxImageSize, true);
		else vel = 55;
		thing.draw(point, vel, vel);
		
	}
}

