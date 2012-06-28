/*
 *  Ribbon.h
 *  ribbon
 *
 *  Created by Joel Lewis on 28/06/2012.
 *  Copyright 2012 Hellicar & Lewis. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxBullet.h"

class Ribbon {

public:
	Ribbon();
	Ribbon(int nNodes, ofCamera *cam);
	~Ribbon();
	
	void update();
	void draw();
	
	void setAnchorPoint(ofVec3f p);
	
	ofxBulletWorldRigid	world;
	
	vector<ofxBulletBox*> nodes;
	vector<ofxBulletJoint*> joints;
	
	ofMesh mesh;
	vector<ofVec3f> points;
	vector<ofVec4f> refPoints;
	
	ofImage img;
	ofTexture tex;
	
	float width;
	
protected:
	int nNodes;
	
	btGeneric6DofConstraint  *anchorPoint;
	
	
private:
	void setupAnchorPoint();
	
	
	ofMaterial material;
};