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

Brush::~Brush() { 
	
}

void Brush::update() {
	
}

void Brush::draw() {
		
	for(int i = 0; i < iterations; i++) {
		
		point = point*(1-m) + input*m;
		
		ofSetHexColor(0xffffff);
		thing.draw(point, 60, 60);
		
	}
}

