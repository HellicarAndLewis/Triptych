/*
 *  ofxDelaunay.h
 *  openFrameworks
 *
 *  Created by Pat Long on 29/10/09.
 *  Copyright 2009 Tangible Interaction. All rights reserved.
 *
 *  Some parts based on demo code by Gilles Dumoulin. 
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *
 */
#ifndef _OFX_DELAUNAY
#define _OFX_DELAUNAY

#include "ofMain.h"

#include "Delaunay.h"

#define DEFAULT_MAX_POINTS 500

class ofxDelaunay{
private:
	int maxPoints;
	ITRIANGLE *v;
	XYZ *p;
	int nv;
	int ntri;
	
public:
	ofxDelaunay(int maxPoints=DEFAULT_MAX_POINTS);
	~ofxDelaunay();
	
	void init(int maxPoints=DEFAULT_MAX_POINTS);
	void reset();
	
	int addPoint(float x=0.0, float y=0.0, float z=0.0);
	int triangulate();
	
	int getNumVertices();
	int getNumTriangles();
	ITRIANGLE* getTriangles();
	XYZ* getPoints();
	
	void outputTriangles();
	void drawTriangles();	
};

#endif
