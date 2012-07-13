//
//  Ribbon.cpp
//  gg
//
//  Created by Will Gallia on 13/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "Ribbon.h"


Ribbon::Ribbon()
{
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh.enableColors();
#ifdef NORMALS	
	mesh.enableNormals();
#endif
}

Ribbon::~Ribbon()
{
	//clean it up
	for(int i = 0; i < segments.size(); i++){
		delete segments[i];
	}
}

void Ribbon::update()
{
	for(int i = 0; i < 1; i++){
		
		
		RibbonSegment* ribbonSegment = new RibbonSegment();
		ribbonSegment->setPosition(head.getPosition());
		
		segments.push_back( ribbonSegment );
	}
	

	for(int i = 1; i < segments.size(); i++){
		RibbonSegment nextNode = (i == segments.size()-1) ? head : *(segments[i-1]);
		segments[i]->update( nextNode );
	}
	
	while (segments.size() > ribbonLength) {
		delete segments.front();
		segments.pop_front();
	}
	
}

ofVec3f findNormal(ofVec3f a, ofVec3f b, ofVec3f ref) {
	a = a - ref;
	b = b - ref;
	ofVec3f v = a.cross(b);
	v.normalize();
	return v;
}

void Ribbon::draw() {
	
	
	ofSetHexColor(0xffffff);
	ofDrawBitmapString(ofToString(segments.size()), 10, 10);
	
	vector<ofVec3f> middleLine;
	vector<ofFloatColor> colours;

	mesh.clear();
	
	ofVec3f lastLeftPoint, lastRightPoint;
	
	for(int i = 0; i < segments.size(); i++){
		ofNode thisNode = *(segments[i]);
		ofNode nextNode = (i == segments.size()-1) ? head : *(segments[i+1]);
		
		//the further away each node is the thinner the line
		float thickness = ofMap(thisNode.getPosition().distance( nextNode.getPosition() ),
														10, 100,
														maxThickness, minThickness, true);

		//attenuate thickness if we are near the tails
		int tailTaper = 100;
		thickness *= ofMap(i, segments.size() - MIN(frontTaper,segments.size()), segments.size(), 1.0, 0.0, true);
		thickness *= ofMap(i, 0, MAX(backTaper,segments.size()), 0.0, 1.0, true);
		
		ofVec3f bottomPoint  = thisNode.getPosition() + ofVec3f(0, 1, 0) * thickness / 2.0;
		ofVec3f topPoint = thisNode.getPosition() + ofVec3f(0, -1, 0) * thickness / 2.0;
		
		if (fadeInZ) {
			topPoint.z = i;
			bottomPoint.z = i;
		}
		
#ifdef NORMALS		
		ofVec3f bottomPoint2  = nextNode.getPosition() + ofVec3f(0, 1, 0) * thickness / 2.0;
		ofVec3f topPoint2 = nextNode.getPosition() + ofVec3f(0, -1, 0) * thickness / 2.0;
		if (fadeInZ) {
			topPoint2.z = i+1;
			bottomPoint2.z = i+1;
		}
#endif
		
		
		float alpha = ((float) i ) /segments.size();
		
		mesh.addVertex(bottomPoint);
		mesh.addColor(ofFloatColor(1, 1, 1, alpha));
		mesh.addVertex(topPoint);
		mesh.addColor(ofFloatColor(1, 1, 1, alpha));

#ifdef NORMALS		
		mesh.addNormal(findNormal(bottomPoint2, topPoint, bottomPoint));
		mesh.addNormal(findNormal(bottomPoint, topPoint2, topPoint));
#endif

		
		middleLine.push_back((bottomPoint + topPoint) * 0.5);
		colours.push_back(ofFloatColor(1.0, 0, 0.8, ((float) i ) /segments.size()));
		
	}

	mesh.draw();
	
	
	if (drawPink) {
		ofMesh tmesh;
		tmesh.setMode(OF_PRIMITIVE_LINE_STRIP);
		tmesh.addVertices(middleLine);
		tmesh.enableColors();
		tmesh.addColors(colours);
		tmesh.draw();
	}
	

}