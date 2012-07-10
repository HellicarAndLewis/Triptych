//
//  Trail.cpp
//  gg
//
//  Created by Will Gallia on 09/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "Trail.h"
#include "ofxSimpleGuiToo.h"

#define TRAIL_WIDTH 50
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
	
	texImage.loadImage("grad3_3.png");
//	texImage2.loadImage("grad3_22.png");
	
//	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);
//
//	fbo.begin();
//	ofClear(0, 0, 0);
//	fbo.end();
	
//	gui.addTitle("Trail");	
//	gui.addSlider("trail max length", trailMaxLength, 0, 500);
//	gui.addSlider("reduction speed", reductionSpeed, 0, 100);
//	gui.addSlider("interpolation factor", interpolationFactor, 0, 1);
//	gui.addSlider("sine multiplier", sineMultiplier, 0, 100);
//	gui.addSlider("sine increment", sineIncrement, 0, 2);
//	
//	gui.addToggle("draw info", drawInfo);
//	gui.addToggle("draw wireframe", drawWireframe);
	
	
	
}


Trail::~Trail() {}


void Trail::update() {

	//keep the trail at max length...
	while (trail.size() > trailMaxLength) {
		trail.pop_back();
	}
	
	//if the head hasn't moved since the last frame then reduce
//	if (trail.begin()->getPosition() == headPos) {
//		for (int i = 0; i < reductionSpeed && trail.size(); i++) {
//			trail.pop_back();
//		}
//		
//	}
	
	mesh.clear();
	
	sineCounter = 0;
	
	for (deque<ofNode>::iterator it = trail.begin(); it != trail.end(); it++) {
		
		ofMatrix4x4 rot = ofMatrix4x4::newRotationMatrix(sineCounter, 0, 0, 0);
		
		ofNode node = *it;
		ofVec3f p = node.getPosition();
		
		float sine = sin(sineCounter) * sineMultiplier;
		sineCounter+= 0;
		
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
	

	
	
//	fbo.begin();
	
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//	glColor4f(0, 0, 0, 0.2);
//	ofRect(0, 0, ofGetWidth(), ofGetHeight());
//	
//	
//	
		
	glColor4f(1,1,1,0.);
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	ofSetHexColor(0xffffff);

	if (drawWireframe) {
		mesh.drawWireframe();
	}
	else {
		texImage.bind();
		
//		for (int i = 0; i < 50; i++) {
			mesh.draw();
//		}
		
		
		texImage.unbind();
	}
		
//	fbo.end();
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//	glColor4f(1,1,1,1);	
//	fbo.draw(0,0);
	
	
	
	if (drawInfo) {
		ofSetHexColor(0xffffff);
		ofDrawBitmapString("trail size: " + ofToString(trail.size()), 10, 10);
	}
	

	
}

void Trail::input(ofVec3f p) {
	
	ofNode node;
	
	
	if (trail.empty()) {
		node.setPosition(p);
		trail.push_front(node);
	}
	
	//if we are not empty then linearly interpolate from last one
	else {
		
//		float m = 0.1;
//		ofVec3f pos = trail.begin()->getPosition()*(1-m) + p*m;
//		node.setPosition(pos);
//		trail.push_front(node);
//		return;
		
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

void Trail::clear() {
	trail.clear();
}