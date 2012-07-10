//
//  Person.cpp
//  Intel_Design
//
//  Created by Will Gallia on 10/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "Person.h"


Person::Person() {
	
}


Person::~Person() {
	
}


void Person::setup(const BoundBlob &blob) {
	
	leftHand = blob.left;
	rightHand = blob.right;
	
}


void Person::update(const BoundBlob &blob) {
	
	leftHand = blob.left;
	rightHand = blob.right;

	
	
	if (useMareks) {
		mtrail[0].setInput(leftHand);
		mtrail[1].setInput(rightHand);
	}
	else {
		trail[0].input(leftHand);
		trail[1].input(rightHand);
	}
}

void Person::draw() {
	
	ofPushMatrix();
	ofScale(ofGetWidth()/640.0, ofGetHeight()/480.0);
	ofSetColor(255, 100, 10);
	
	ofCircle(leftHand.x, leftHand.y, 4);
	ofCircle(rightHand.x, rightHand.y, 4);
	
	

	
	if (useMareks) {
		for (int i = 0; i < NUM_HANDS; i++) {
			mtrail[i].draw();
		}
	}
	else {
		for (int i = 0; i < NUM_HANDS; i++) {
			trail[i].update();
			trail[i].draw();
		}
	}
	
	ofPopMatrix();
	
}
