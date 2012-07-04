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

struct Triangle {
	
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
	ofFloatColor color;
	
	
	
	void set(p2t::Triangle *t) {
		hollow = false;
		for(int i = 0; i < 3; i++) {
			points[i].set(t->GetPoint(i)->x, t->GetPoint(i)->y);
		}
		centre = (points[0] + points[1] + points[2])/3.f;
	}
};