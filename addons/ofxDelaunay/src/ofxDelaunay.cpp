/*
 *  ofxDelaunay.cpp
 *  openFrameworks
 *
 *  Created by Pat Long on 29/10/09.
 *  Copyright 2009 Tangible Interaction. All rights reserved.
 *
 *  Some parts based on demo code by Gilles Dumoulin. 
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *
 */
#include "ofxDelaunay.h"

ofxDelaunay::ofxDelaunay(int maxPoints){
	p = NULL;
	v = NULL;
	this->init(maxPoints);
}

ofxDelaunay::~ofxDelaunay(){
	if(p != NULL){
		delete []p;
		p = NULL;
	}
	if(v != NULL){
		delete []v;
		v = NULL;
	}
}

void ofxDelaunay::init(int maxPoints){
	this->maxPoints = maxPoints;
	this->reset();
}

void ofxDelaunay::reset(){
	if(p != NULL)
		delete []p;
	p = new XYZ[this->maxPoints];
	if(v != NULL)
		delete []v;
	v = NULL;
	nv = 0;
	ntri = 0;
	
}

int ofxDelaunay::addPoint(float x, float y, float z){
	if (nv >= this->maxPoints){
		this->maxPoints = this->maxPoints * 2;            // double the size of the array if necessary
		XYZ *p_Temp = new XYZ[this->maxPoints];
		for (int i = 0; i < nv; i++)
			p_Temp[i] = p[i];
		delete []p;
		p = p_Temp; 
	}
	p[nv].x = x;
	p[nv].y = y;
	p[nv].z = z;
	nv++;
	return nv;
}

int ofxDelaunay::triangulate(){
	XYZ *p_Temp = new XYZ[nv + 3]; 
	for (int i = 0; i < nv; i++)
		p_Temp[i] = p[i];      
	delete []p;
	p = p_Temp;
	v = new ITRIANGLE[3 * nv]; 
	qsort(p, nv, sizeof(XYZ), XYZCompare);
	Triangulate(nv, p, v, ntri);
	return ntri;
}

int ofxDelaunay::getNumVertices()
{
	return this->nv;
}

int ofxDelaunay::getNumTriangles(){
	return this->ntri;
}

ITRIANGLE* ofxDelaunay::getTriangles(){
	return this->v;
}

XYZ* ofxDelaunay::getPoints(){
	return this->p;
}

void ofxDelaunay::outputTriangles(){
	cout << "triangle count: " << ntri << endl;
	for(int i = 0; i < ntri; i++){
		cout << "triangle #" << i << endl;;
		cout << "\t" << (int)p[v[i].p1].x << "," << (int)p[v[i].p1].y << " " << (int)p[v[i].p2].x << "," << (int)p[v[i].p2].y << endl;
		cout << "\t" << (int)p[v[i].p2].x << "," << (int)p[v[i].p2].y << " " << (int)p[v[i].p3].x << "," << (int)p[v[i].p3].y << endl;
		cout << "\t" << (int)p[v[i].p3].x << "," << (int)p[v[i].p3].y << " " << (int)p[v[i].p1].x << "," << (int)p[v[i].p1].y << endl;
		cout << endl;
	}
}

void ofxDelaunay::drawTriangles(){
	for(int i = 0; i < ntri; i++){
		ofSetColor(0, 255, 0);
		ofFill();
		ofTriangle(p[v[i].p1].x, p[v[i].p1].y, p[v[i].p2].x, p[v[i].p2].y, p[v[i].p3].x, p[v[i].p3].y);
		ofSetColor(0, 0, 255);
		ofNoFill();
		ofTriangle(p[v[i].p1].x, p[v[i].p1].y, p[v[i].p2].x, p[v[i].p2].y, p[v[i].p3].x, p[v[i].p3].y);
	}
}
