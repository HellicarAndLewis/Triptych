/**
 *  BlobBounds.cpp
 *
 *  Created by Marek Bereza on 28/06/2012.
 */

#include "BoundBlob.h"
#include "Line.h"
#include "contourutils.h"


void BoundBlob::init(const ofxCvBlob &blob) {


	vector<ofVec2f> ps;
	ps.resize(blob.pts.size());
	
	for(int i = 0; i < blob.pts.size(); i++) {
		ps[i].x = blob.pts[i].x;
		ps[i].y = blob.pts[i].y;
	}
	
	
	top = ps[tricks::math::findTopmostPoint(ps)];
	left = ps[tricks::math::findLeftmostPoint(ps)];
	right = ps[tricks::math::findRightmostPoint(ps)];
	bottom = ps[tricks::math::findBottommostPoint(ps)];
	
	
	centroid = ofVec2f(blob.centroid.x, blob.centroid.y);
}

void BoundBlob::update(const ofxCvBlob &blob) { 
	vector<ofVec2f> ps;
	ps.resize(blob.pts.size());
	
	for(int i = 0; i < blob.pts.size(); i++) {
		ps[i].x = blob.pts[i].x;
		ps[i].y = blob.pts[i].y;
	}
	
	
	ofVec2f newTop = ps[tricks::math::findTopmostPoint(ps)];
	ofVec2f newLeft = ps[tricks::math::findLeftmostPoint(ps)];
	ofVec2f newRight = ps[tricks::math::findRightmostPoint(ps)];
	ofVec2f newBottom = ps[tricks::math::findBottommostPoint(ps)];
	
	float l = 0.7;
	top = top *l  + newTop*(1.f-l);
	left = left*l + newLeft*(1.f-l);
	right = right*l + newRight*(1.f-l);
	bottom = bottom*l + newBottom*(1.f-l);
	
	centroid = ofVec2f(blob.centroid.x, blob.centroid.y);
}

void BoundBlob::draw() {
	ofSetHexColor(0xFF0000);
	ofCircle(top, 5);
	ofSetHexColor(0x00ff00);
	ofCircle(left, 5);
	ofSetHexColor(0x0000ff);
	ofCircle(right, 5);
	
	ofSetHexColor(0xFF00ff);
	ofCircle(bottom, 5);

}