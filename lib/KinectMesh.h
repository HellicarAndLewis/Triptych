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


#include "ofxOpenCv.h"
#include "contourutils.h"
#include "poly2tri.h"
#include "KinectTriangle.h"
#include "KinectThresholder.h"


class KinectMesh {
public:
	
	// this is a simplified contour
	vector<ofVec2f> outline;
	
	// this is the random points that 
	// are inside the contour for triangulation
	vector<ofVec2f> insides;
	
	
	// the triangulated output as a vbo mesh
	ofVboMesh mesh;

	
	// the individual triangles.
	vector<KinectTriangle> triangles;
	KinectVertices vertices;
	

	// depth for a mesh is a single value - [0,255]
	float depth;

	
	/**
	 * Returns false if the blob is too small to create a mesh from
	 */
	bool setup(const ofxCvBlob &blob, KinectThresholder &thresholder);
	
	// draws the vbo
	void draw();

	
	// settings and gui
	static int borderResolution;
	static int fillResolution;
	static void setupGui();
	static bool perVertexColour;
	static float edgeColourSharpening;
private:
	size_t addVertex(const KinectVertex& kv);
	
	void addTriangle(const ofVec2f &a, const ofVec2f &b, const ofVec2f &c, 
					 const ofFloatColor &c0, const ofFloatColor &c1, const ofFloatColor &c2);
	void addTriangle(const KinectTriangle &tri);
	static p2t::CDT *delaunay;
	bool triangleTouchesCorner(p2t::Triangle *t);
};

inline size_t KinectMesh::addVertex(const KinectVertex& kv) {
	return vertices.add(kv);
//	vertices.push_back(kv);
//	return vertices.size()-1;
}