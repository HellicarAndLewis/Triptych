/*
 *  Ribbon.cpp
 *  ribbon
 *
 *  Created by Joel Lewis on 28/06/2012.
 *  Copyright 2012 Hellicar & Lewis. All rights reserved.
 *
 */

#include "Ribbon.h"

Ribbon::Ribbon(): nNodes(10) {}

Ribbon::Ribbon(int nNodes, ofCamera *cam, ofxBulletWorldRigid *world): nNodes(nNodes) {
	this->world = world;
	
	
	for (int i = 0; i < nNodes; i++) {
		nodes.push_back(new ofxBulletBox());

		nodes[i]->init(10, 10, 10);
		nodes[i]->create(world->world, ofVec3f(i*5, 0, 0), 10, 1, 1, 1);
		
		nodes[i]->add();
	}
	
	
	for (int i = 1; i < nodes.size(); i++) {
		joints.push_back(new ofxBulletJoint());
		joints[i-1]->create(world->world, nodes[i-1], nodes[i]);
		
		//these values are so arbitrary!!!
		//edit: i don't think changing them does fuck all!!!
		joints[i-1]->setLinearLowerLimit(-0, 0, 0);
		joints[i-1]->setLinearUpperLimit(0, 0, 0);
		joints[i-1]->setAngularUpperLimit(0.1, 0, 0);
		joints[i-1]->setAngularLowerLimit(0, 0, 0);
		joints[i-1]->setParam(BT_CONSTRAINT_STOP_CFM, 0.5);
		joints[i-1]->setParam(BT_CONSTRAINT_STOP_ERP, 0.05);
		joints[i-1]->add();
	}
	
	refPoints.push_back(ofVec4f(0, -1, 0, 1));
//	refPoints.push_back(ofVec4f(0, 0, 0, 1));
	refPoints.push_back(ofVec4f(0, 1, 0, 1));

	
	points.resize(nodes.size() * refPoints.size());
	mesh = ofMesh(OF_PRIMITIVE_TRIANGLE_STRIP, points);

	vector<ofVec3f> normals;
	normals.resize(nodes.size() * refPoints.size());
	mesh.addNormals(normals);
	
	
	material.setAmbientColor(ofFloatColor(0, 0, 0));
	material.setDiffuseColor(ofFloatColor(0, 0, 0));
	material.setSpecularColor(ofFloatColor(1, 1, 1));
	material.setShininess(80);

	
	//not using a texture anymore...
	
//	img.loadImage("grad3.png");
//	cout << img.bpp << endl;
//	tex.allocate(img.width, img.height, GL_RGBA);
//	tex.loadData(img.getPixels(), img.width, img.height, GL_RGBA);
//	
//	for (int i = 1; i < nodes.size(); i++) {
//		float wtcoord = ((float)i-1)/nodes.size();
//
//		mesh.addTexCoord(ofVec2f(img.width * wtcoord, 0));
//		mesh.addTexCoord(ofVec2f(img.width * wtcoord, img.height));
//		wtcoord = ((float)i)/nodes.size();
//		mesh.addTexCoord(ofVec2f(img.width * wtcoord, 0));
//		mesh.addTexCoord(ofVec2f(img.width * wtcoord, img.height));
//	}
	
	mesh.enableNormals();
	setupAnchorPoint();
	
	width = 10;
}

Ribbon::~Ribbon() {
	world->world->removeConstraint(anchorPoint);
	delete anchorPoint;
}

ofVec3f findNormal(ofVec3f a, ofVec3f b, ofVec3f ref) {
	a = a - ref;
	b = b - ref;
	ofVec3f v = a.cross(b);
	v.normalize();
	return v;
}

void Ribbon::update() {


	
	//only have an even length...
	assert(!(nodes.size() % 2));
	
	for (int i = 0; i < nodes.size()-1; i+=2) {
		
		//			 2     4
		//		  / \   /
		//		 /   \ /
		//		1     3

		//this was used for a variable width... 
//		ofVec3f newPos = ofMatrix4x4::getTransposedOf(transMat) * refPoints[0];
//		ofVec3f lastPos = mesh.getVertex(i * refPoints.size());
//		float vel = (newPos - lastPos).length();
//		vel = 20;
		
		ofVec3f subpoints[4];
		
		//this is horrible....
		for (int j = 0; j < 2; j++) {
			ofMatrix4x4 transMat = nodes[i+j]->getTransformationMatrix();
			for (int k = 0; k < 2; k++) {
				
				ofVec3f multiplier = refPoints[k] * width;
				ofVec3f v = ofMatrix4x4::getTransposedOf(transMat) * multiplier;	
				mesh.setVertex((i+j) * refPoints.size() + k, v);
				subpoints[j*2+k] = v;
				
			}
			
		}
		
		
		mesh.setNormal((i-1) * refPoints.size() + 0, findNormal(subpoints[2], subpoints[1], subpoints[0]));
		mesh.setNormal((i-1) * refPoints.size() + 1, findNormal(subpoints[0], subpoints[2], subpoints[1]));
		mesh.setNormal((i) * refPoints.size() + 0, findNormal(subpoints[3], subpoints[1], subpoints[2]));
		mesh.setNormal((i) * refPoints.size() + 1, findNormal(subpoints[1], subpoints[2], subpoints[3]));
		
		
		
//		//just to debug...
//		ofVec3f p = nodes[i]->getPosition();
//		
//		ofVec3f rotAxis = nodes[i]->getRotationAxis();
//		float rotAngle = nodes[i]->getRotationAngle();
//		
//		if (drawCubes) {
//			ofSetHexColor(0xff0000);
//			if (!i) ofSetHexColor(0x0);
//			ofVec4f v(0, 0, 0, 1);
//			v = ofMatrix4x4::getTransposedOf(transMat) * v;
//			ofPushMatrix();
//			ofTranslate(v.x, v.y, v.z);
//			ofRotate(ofRadToDeg(rotAngle), rotAxis.x, rotAxis.y, rotAxis.z);
//			ofBox(0, 0, 0, 10);
//			ofPopMatrix();
//		}
		
	}
	
}

void Ribbon::draw() {


	
//	ofEnableBlendMode(OF_BLENDMODE_ADD);
//	ofEnableAlphaBlending();
//	glDisable(GL_DEPTH_TEST);
	
//	img.bind();

//	ofSetHexColor(0xff00ff);
	
	ofSetColor(250, 20, 140);
	ofFill();
	material.begin();
	mesh.draw();
	material.end();
	
	
//	img.unbind();
	
//	ofSetHexColor(0xffffff);
//	mesh.drawWireframe();

}


void Ribbon::setAnchorPoint(ofVec3f p) {


	anchorPoint->getFrameOffsetA().setOrigin(btVector3(p.x, p.y, p.z));

	
}

ofVec3f Ribbon::getAnchorPoint() {
	btVector3 &b = anchorPoint->getFrameOffsetA().getOrigin();
	ofVec3f a(b.x(), b.y(), b.z());
	return a;
}

void Ribbon::setupAnchorPoint() {
	btRigidBody *firstNode = nodes[0]->getRigidBody();
	firstNode->setActivationState(DISABLE_DEACTIVATION);
	
	btTransform tr;
	tr.setIdentity();
	btVector3 origin = firstNode->getCenterOfMassTransform().inverse() * btVector3(0, 0, 0);
	tr.setOrigin(origin);
	
	anchorPoint = new btGeneric6DofConstraint(*firstNode, tr, false);
	anchorPoint->setLinearLowerLimit(btVector3(0,0,0));
	anchorPoint->setLinearUpperLimit(btVector3(0,0,0));
	anchorPoint->setAngularLowerLimit(btVector3(0,0,0));
	anchorPoint->setAngularUpperLimit(btVector3(0,0,0));
	
	world->world->addConstraint(anchorPoint);
	
	for (int i = 0; i < 6; i++) {
		anchorPoint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,i);
	}
	
	for (int i = 0; i < 6; i++) {
		anchorPoint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,i);
	}
}