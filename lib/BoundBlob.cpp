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