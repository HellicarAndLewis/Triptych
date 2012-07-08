/**     ___           ___           ___                         ___           ___     
 *     /__/\         /  /\         /  /\         _____         /  /\         /__/|    
 *    |  |::\       /  /::\       /  /::|       /  /::\       /  /::\       |  |:|    
 *    |  |:|:\     /  /:/\:\     /  /:/:|      /  /:/\:\     /  /:/\:\      |  |:|    
 *  __|__|:|\:\   /  /:/~/::\   /  /:/|:|__   /  /:/~/::\   /  /:/  \:\   __|__|:|    
 * /__/::::| \:\ /__/:/ /:/\:\ /__/:/ |:| /\ /__/:/ /:/\:| /__/:/ \__\:\ /__/::::\____
 * \  \:\~~\__\/ \  \:\/:/__\/ \__\/  |:|/:/ \  \:\/:/~/:/ \  \:\ /  /:/    ~\~~\::::/
 *  \  \:\        \  \::/          |  |:/:/   \  \::/ /:/   \  \:\  /:/      |~~|:|~~ 
 *   \  \:\        \  \:\          |  |::/     \  \:\/:/     \  \:\/:/       |  |:|   
 *    \  \:\        \  \:\         |  |:/       \  \::/       \  \::/        |  |:|   
 *     \__\/         \__\/         |__|/         \__\/         \__\/         |__|/   
 *
 *  Description: 
 *				 
 *  KinectMesh.h, created by Marek Bereza on 29/06/2012.
 */

#pragma once

#include "ofMain.h"
#include "poly2tri.h"
#include <roxlu/3d/VertexTypes.h>

struct KinectVertex {
	float pos[3];
	float col[3];
	
	
	KinectVertex() {
		pos[0] = pos[1] = pos[2] = 0.0f;
		col[0] = col[1] = col[2] = 1.0f;
	}
	
	KinectVertex(float* p) {
		setPos(p);
		col[0] = col[1] = col[2] = 1.0f;
	}
	
	KinectVertex(float x, float y, float z = 1.0) {
		setPos(x,y,z);
	}
	
	KinectVertex(float x, float y, float z, float r, float g, float b) {
		setPos(x,y,z);
		setCol(r,g,b);
	}
	
	KinectVertex(float* p, float* c) {
		setPos(p);
		setCol(c);
	}
	
	void setPos(float x, float y, float z = 0.0) {
		pos[0] = x;
		pos[1] = y;
		pos[2] = 0.0;
	}
	
	void setPos(float* p) {
		pos[0] = p[0];
		pos[1] = p[1];
		pos[2] = 0.0;
	}
	
	
	void setCol(float r, float g, float b) {
		col[0] = r;
		col[1] = g;
		col[2] = b;
	}
	
	void setCol(float* c) {
		col[0] = c[0];
		col[1] = c[1];
		col[2] = c[2];
	}
};


typedef roxlu::Vertices_Template<KinectVertex>	KinectVertices;



struct KinectTriangle {
	int va;
	int vb;
	int vc;
		
	// the 3 points in the triangle
	ofVec2f points[3];
	
	// the computed centre (average of the 3 points)
	ofVec2f centre;
	
	
	// if the triangle is on the edge of the contour
	// and falls outside the contour (e.g. it's over
	// a concave part) this will be true.
	// usually, just ignore hollow ones.
	bool hollow;
	
	
	// The colour from the rgb input from the kinect.
	ofFloatColor colors[3];
	
	
	
	void set(p2t::Triangle *t) {
		hollow = false;
		for(int i = 0; i < 3; i++) {
			points[i].set(t->GetPoint(i)->x, t->GetPoint(i)->y);
		}
		centre = (points[0] + points[1] + points[2])/3.f;
	}
	
	void sharpenEdges(float amt) {
		ofFloatColor avg = colors[0]/3.f + colors[1]/3.f + colors[2]/3.f;
		avg *= amt;
		amt = 1-amt;
		colors[0] = colors[0]*amt + avg;
		colors[1] = colors[1]*amt + avg;
		colors[2] = colors[2]*amt + avg;
		
	}
};