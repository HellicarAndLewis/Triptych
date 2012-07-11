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

	brushes[0].setPoint(leftHand);
	brushes[1].setPoint(rightHand);
	brushes[0].setInput(leftHand);
	brushes[1].setInput(rightHand);
}


void Person::update(const BoundBlob &blob) {
	
	leftHand = blob.left;
	rightHand = blob.right;

	brushes[0].setInput(leftHand);
	brushes[1].setInput(rightHand);

	//	if (useMareks) {
//		
//	}
//	else {
//		trail[0].input(leftHand);
//		trail[1].input(rightHand);
//	}
}

void Person::draw() {
	

//	ofSetColor(255, 100, 10);
//	
//	ofCircle(leftHand.x, leftHand.y, 4);
//	ofCircle(rightHand.x, rightHand.y, 4);

	
	for (int i = 0; i < NUM_HANDS; i++) {
		brushes[i].draw();
	}
	

	
//	if (useMareks) {
//		for (int i = 0; i < NUM_HANDS; i++) {
//			mtrail[i].draw();
//		}
//	}
//	else {
//		for (int i = 0; i < NUM_HANDS; i++) {
//			trail[i].update();
//			trail[i].draw();
//		}
//	}
	
	
}
