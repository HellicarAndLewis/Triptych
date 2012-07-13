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
sineIncrement(0.1),
width(30) {
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh.enableTextures();
	mesh.enableColors();
	colour = ofFloatColor(0.9, 0.8, 0.2);
	
	texImage.loadImage("grad3_3.png");
	texImage2.loadImage("grad3_22.png");
	
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);

	fbo.begin();
	ofClear(0, 0, 0);
	fbo.end();
	
	z = 0;
	
}


Trail::~Trail() {}


void Trail::update() {
	
	cout << trail[0].width << endl;
	
	//keep the trail at max length...
	while (trail.size() > trailMaxLength) {
		trail.pop_back();
	}
	
	
//	trail[0].width = 0;
	
	mesh.clear();
	
	sineCounter = 0;
	
	//for (deque<ofNode>::iterator it = trail.begin(); it != trail.end(); it++) {
	for (int i = 0; i < trail.size(); i++) {
				
		trail[i].width*= reductionSpeed;
		
		cNode node = trail[i];
		ofVec3f p = node.getPosition();
		
		node.vel = ofMap(node.vel, 0, 1000, 50, width, true);
		node.vel = width - node.vel;
		
		float nwidth = node.vel * widthMultiplier;//node.width;  //width - (width * (((float) i) / trail.size()));
		
		if (!useVel) {
			nwidth = node.width;
		}

		ofVec3f top(0, nwidth, 0);
		top = top + p;
		
		ofFloatColor tcolour = colour;
		tcolour.a = 1.0 - ((((float) i) / trail.size()));
		
		mesh.addVertex(top);
		mesh.addColor(tcolour);

		ofVec3f bottom(0, -nwidth, 0);
		bottom = bottom + p;
		
		mesh.addVertex(bottom);
		mesh.addColor(tcolour);
//		mesh.addTexCoord(ofVec2f(texImage.width, texImage.height));
		
	}
	
	headPos = trail.begin()->getPosition();
}

float theta = 0;
float tt = 0;

void Trail::draw() {

	ofSetHexColor(0xffffff);

	ofEnableAlphaBlending();
	if (drawWireframe) {
		ofSetHexColor(0xffffff);
		mesh.drawWireframe();
	}
	else {
//		texImage.bind();
		
//		for (int i = 0; i < 50; i++) {
		ofFill();
			mesh.draw();
//		}
		
		
//		texImage.unbind();
	}
	

	
	if (drawInfo) {
		ofSetHexColor(0xffffff);
		ofDrawBitmapString("trail size: " + ofToString(trail.size()), 10, 10);
	}
	
}

void Trail::input(ofVec3f p) {
	
	cNode node;
	node.setWidth(width);
	cout << width << endl;
	
	
	if (trail.empty()) {
		node.setPosition(p);
		trail.push_front(node);
	}
	
	//if we are not empty then linearly interpolate from last one
	else {

		ofVec3f pos = trail.begin()->getPosition()*(1-m) + p*m;
		float vel = (trail.begin()->getPosition() - pos).lengthSquared();
//		cout << vel << endl;
		node.setVelocity(vel);
		node.setPosition(pos);
		trail.push_front(node);
		return;
		
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