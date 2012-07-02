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
 *  Room.h, created by Marek Bereza on 28/06/2012.
 */

#pragma once
#include "ofMain.h"

class Room {
public:
	void setup(float aspect = 1);
	void setupGui();
	
	
	void update();
	void draw();
	void begin();
	void end();
	
	
	
	float aoAmt;
	void setAspect(float aspect);
	
	ofCamera camera;
	ofLight light;

	float fov;
	float lightZ;
	float ambient;
	float diffuse;
	ofVec3f camPos;
	ofVec3f lightPos;

private:
	void createMesh(float aspect);
	float aspect;
	ofVboMesh mesh;
	ofShader shader;
	ofImage tex;
	void addQuad(ofVec3f *quad);
};
