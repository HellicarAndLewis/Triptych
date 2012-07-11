//
//  Trail.cpp
//  gg
//
//  Created by Will Gallia on 09/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "Trail.h"
#include "ofxSimpleGuiToo.h"

#define TRAIL_WIDTH 1
#define INTERPOLATION_SIZE 4.0

static vector<ofFloatColor> colours;
bool done = false;
 
Trail::Trail(ofVec3f p):
trailMaxLength(250),
reductionSpeed(8),
interpolationFactor(1),
drawInfo(false),
drawWireframe(true),
sineCounter(0),
sineMultiplier(20),
sineIncrement(0.1) {
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	//mesh.enableTextures();
	mesh.enableColors();
//	texImage.loadImage("grad3_3.png");
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
	
	if(!done) {
		colours.push_back(ofFloatColor(1.0, 0.3, 0.16, 1));
		colours.push_back(ofFloatColor(0.78, 0.0, 0.09, 1));
		colours.push_back(ofFloatColor(1.0, 0.86, 0.75, 1));
		colours.push_back(ofFloatColor(1.0, 0.0, 0.11, 1));
		done = true;
	}
	
	input(p);
}


Trail::~Trail() {}


void Trail::update(ofVec3f p) {

	input(p);

	//keep the trail at max length...
	while (trail.size() > maxImageSize) {
		trail.pop_back();
	}
	

	//if the head hasn't moved since the last frame then reduce
	if (trail.begin()->getPosition() == headPos) {
		for (int i = 0; i < reductionSpeed && trail.size(); i++) {
			//trail.pop_back();
		}
		
	}
	
	mesh.clear();
	
	sineCounter = 0;
	int i = 0;
	for (deque<Node>::iterator it = trail.begin(); it != trail.end(); ++it) {


		
		ofMatrix4x4 rot = ofMatrix4x4::newRotationMatrix(sineCounter, 0, 0, 0);
		
		Node node = *it;
		ofVec3f p = node.getPosition();
		float d = trailWidth;
		
		if(variableWidth && it+ 1 != trail.end()) {
			d = (((*(it+1)).getPosition()) - p).length(); 
			d*= variableFactor;
		}
		ofFloatColor col = node.getColor();
		if(useFade) col.a = (trail.size() - i)/(float)trail.size();

		mesh.addVertex(ofVec3f(p.x, p.y + d, p.z));
		mesh.addColor(col);

		mesh.addVertex(ofVec3f(p.x, p.y - d, p.z));
		mesh.addColor(col);

		i++;
		continue;

		
//		float sine = sin(sineCounter) * sineMultiplier;
//		sineCounter+= 0;
//		
////		ofVec3f top(p.x, p.y + TRAIL_WIDTH + sine, p.z);
////		top = top * rot;
//
//		ofVec3f top(0, TRAIL_WIDTH + sine, 0);
//		top = top * rot;
//		top = top + p;
//		
//		mesh.addVertex(top);
//		mesh.addTexCoord(ofVec2f(0, 0));
//		
////		ofVec3f bottom(p.x, p.y - TRAIL_WIDTH + sine, p.z);
////		bottom = bottom * rot;
//
//		ofVec3f bottom(0, -TRAIL_WIDTH + sine, 0);
//		bottom = bottom * rot;
//		bottom = bottom + p;
//		
//		mesh.addVertex(bottom);
//		mesh.addTexCoord(ofVec2f(texImage.width, texImage.height));
		
	}
	
	if(useGravity) {

		for(int i = 0; i < trail.size(); i++) {
			trail[i].pos.y+= gravityFactor * i;
		}

	}

	headPos = trail.begin()->getPosition();
}

float theta = 0;
float tt = 0;

void Trail::draw() {
	
//	cout << trail.size() << endl;
	//ofSetWindowTitle(ofToString(trail.size()));
	ofSetLineWidth(2);
//	fbo.begin();
	
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//	glColor4f(0, 0, 0, 0.2);
//	ofRect(0, 0, ofGetWidth(), ofGetHeight());
//	
//	
//	
		
//	glColor4f(1,1,1,0.);
//	ofEnableBlendMode(OF_BLENDMODE_ADD);

	mesh.draw();
	return;


	if (drawWireframe) {
		mesh.drawWireframe();
	}
	else {
//		texImage.bind();
		
//		for (int i = 0; i < 50; i++) {
			mesh.draw();
//		}
		
		
//		texImage.unbind();
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
	
	Node node;
	
	
	if (true || trail.empty()) {
		node.setPosition(p);
		node.setColor(colours[((int)rand()) % colours.size()]);
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
			node.setColor(colours[((int)rand()) % colours.size()]);
			trail.push_front(node);
			
		}
	}
}

void Trail::clear() {
	trail.clear();
}