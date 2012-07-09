//
//  Trail.cpp
//  gg
//
//  Created by Will Gallia on 09/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "Trail.h"

#define TRAIL_WIDTH 30
#define INTERPOLATION_SIZE 4.0

Trail::Trail():
trailMaxLength(250),
reductionSpeed(8),
interpolationFactor(1),
drawInfo(false),
drawWireframe(false),
sineCounter(0),
sineMultiplier(20),
sineIncrement(0.1) {
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh.enableTextures();
	
	texImage.loadImage("grad3.png");
	texImage2.loadImage("grad3_22.png");
	
}


Trail::~Trail() {}


void Trail::update() {

	//keep the trail at max length...
	while (trail.size() > trailMaxLength) {
		trail.pop_back();
	}
	
	//if the head hasn't moved since the last frame then reduce
	if (trail.begin()->getPosition() == headPos) {
		for (int i = 0; i < reductionSpeed && trail.size(); i++) {
			trail.pop_back();
		}
		
	}
	
	mesh.clear();
	
	sineCounter = 0;
	
	for (deque<ofNode>::iterator it = trail.begin(); it != trail.end(); it++) {
		
		ofMatrix4x4 rot = ofMatrix4x4::newRotationMatrix(sineCounter, 1, 0, 0);
		
		ofNode node = *it;
		ofVec3f p = node.getPosition();
		
		float sine = sin(sineCounter) * sineMultiplier;
		sineCounter+= sineIncrement;
		
//		ofVec3f top(p.x, p.y + TRAIL_WIDTH + sine, p.z);
//		top = top * rot;

		ofVec3f top(0, TRAIL_WIDTH + sine, 0);
		top = top * rot;
		top = top + p;
		
		mesh.addVertex(top);
		mesh.addTexCoord(ofVec2f(0, 0));
		
//		ofVec3f bottom(p.x, p.y - TRAIL_WIDTH + sine, p.z);
//		bottom = bottom * rot;

		ofVec3f bottom(0, -TRAIL_WIDTH + sine, 0);
		bottom = bottom * rot;
		bottom = bottom + p;
		
		mesh.addVertex(bottom);
		mesh.addTexCoord(ofVec2f(texImage.width, texImage.height));
		
	}
	
	headPos = trail.begin()->getPosition();
}

float theta = 0;
float tt = 0;

void Trail::draw() {
	
	if (drawInfo) {
		ofSetHexColor(0xffffff);
		ofDrawBitmapString("trail size: " + ofToString(trail.size()), 10, 10);
	}
	

	if (drawWireframe) {
		ofSetHexColor(0xffffff);
		mesh.drawWireframe();
	}
	else {
		texImage.bind();
		mesh.draw();
		
		texImage.unbind();
	}
	
//	vector<ofVec3f> points;
//	points.resize(trail.size());
	
	theta = tt;
	tt+= 0.1;

	ofVboMesh mesh2;
	mesh2.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	
	ofSetHexColor(0x00ffff);
	for (int i = 0; i < trail.size(); i++) {
//		ofCircle(trail[i].getX(), trail[i].getY() + (sin(theta+=0.1) * 20), 5);
		
		ofVec3f p = trail[i].getPosition();
		
		ofVec3f top(0, TRAIL_WIDTH + (sin(theta) * 20), 0);
		top = top + p;
		
		mesh2.addVertex(top);
		mesh2.addTexCoord(ofVec2f(0, 0));
		
		//		ofVec3f bottom(p.x, p.y - TRAIL_WIDTH + sine, p.z);
		//		bottom = bottom * rot;
		
		ofVec3f bottom(0, -TRAIL_WIDTH+ (sin(theta) * 20), 0);
		bottom = bottom + p;
		
		mesh2.addVertex(bottom);
		mesh2.addTexCoord(ofVec2f(texImage.width, texImage.height));
		
		theta+=0.07;
		
	}
	
	texImage2.bind();
	mesh2.draw();
	texImage2.unbind();
	
}

void Trail::input(ofVec3f p) {
	
	ofNode node;
	
	
	if (trail.empty()) {
		node.setPosition(p);
		trail.push_front(node);
	}
	
	//if we are not empty then linearly interpolate from last one
	else {
		ofVec3f old = trail.begin()->getPosition();
		ofVec3f diff = p - old;
				
		float interpolationLimit = diff.length() * interpolationFactor;
		
		for (int i = 0; i < interpolationLimit; i++) {
			ofVec3f inter = old + (diff * (i/interpolationLimit));
						
			node.setPosition(inter);
			trail.push_front(node);
			
		}
	}
}