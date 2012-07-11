//
//  Person.h
//  Intel_Design
//
//  Created by Will Gallia on 10/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "BoundBlob.h"
#include "Trail.h"
#include "Brush.h"

#define NUM_HANDS 2

class Person {
	
public:

	Person();
	~Person();
	
	void setup(const BoundBlob &blob);
	void update(const BoundBlob &blob);
	void draw();
	
	ofVec3f leftHand, rightHand;

	
	Trail trail[NUM_HANDS];
	Brush brushes[NUM_HANDS];
};

extern bool useMareks;
