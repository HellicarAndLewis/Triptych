/**
 *  Room.cpp
 *
 *  Created by Marek Bereza on 28/06/2012.
 */

#include "Room.h"
#include "ofxSimpleGuiToo.h"


void Room::setup(float aspect) {
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	tex.loadImage("room.png");
	this->aspect = aspect;	
	aoAmt = 0.5;
	createMesh(aspect);
	
	ambient = 1.0;
	diffuse = 1.0;
	light.setPointLight();
	fov = 60;
	
	camera.setPosition(0, 0, -30);
	camera.lookAt(ofVec3f(0,0,0));
	camera.setNearClip(0.001);
	
	
	shader.load("pointlight.vert", "pointlight.frag");

}

void Room::setupGui() {
	gui.addTitle("Room");
	gui.addSlider("fov", fov, 30, 140);
	gui.addSlider("CamX", camPos.x, -1,1);
	gui.addSlider("CamY", camPos.y, -1,1);
	gui.addSlider("CamZ", camPos.z, -10,0);
	
	gui.addSlider("LightX", lightPos.x, -1, 1);
	gui.addSlider("LightY", lightPos.y, -1, 1);
	gui.addSlider("LightZ", lightPos.z, -5, 1);
	
	gui.addSlider("Ambient", ambient, 0, 1);
	gui.addSlider("Diffuse", diffuse, 0, 1);
	gui.addSlider("aoAmount", aoAmt, 0, 1);
}

void Room::setAspect(float aspect) {
	createMesh(aspect);
}

void Room::update() {
	camera.setFov((int)fov);
	camera.setPosition(camPos.x, camPos.y, camPos.z);

	light.setPosition(lightPos.x, lightPos.y, lightPos.z);


	light.setAmbientColor(ofFloatColor(ambient, ambient, ambient));
	light.setDiffuseColor(ofFloatColor(diffuse, diffuse, diffuse));
}


void Room::begin() {
	ofEnableLighting();
	light.enable();
	
	camera.begin();
	
		
		
		ofSetHexColor(0xFFFFFF);
		
		shader.begin();
		shader.setUniform1f("amt", aoAmt);
		shader.setUniformTexture("tex", tex.getTextureReference(), 0);
		
		mesh.draw();	
		shader.end();
}

void Room::end() {

	

	camera.end();
	ofDisableLighting();
}


void Room::draw() {
	begin();
	end();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
////////////////////////////////////////////////////////////////////////////////////////////////////



void Room::createMesh(float aspect) {
	mesh.clear();
	ofVec3f quad[4];
	
	// back face
	quad[0].set(-aspect, 1, 1);
	quad[1].set(aspect, 1,  1);
	quad[2].set(aspect, -1,   1);
	quad[3].set(-aspect, -1,  1);
	
	addQuad(quad);
	
	// bottom face
	quad[0].set(-aspect, -1, 1);
	quad[1].set(aspect, -1,  1);
	quad[2].set(aspect, -1, -1);
	quad[3].set(-aspect, -1,  -1);
	
	addQuad(quad);
	
	// top face
	quad[0].set(-aspect, 1, -1);
	quad[1].set(aspect, 1,  -1);
	quad[2].set(aspect, 1, 1);
	quad[3].set(-aspect, 1,  1);
	
	addQuad(quad);
	
	// left face
	quad[0].set(-aspect, 1, -1);
	quad[1].set(-aspect, 1,  1);
	quad[2].set(-aspect, -1, 1);
	quad[3].set(-aspect, -1,-1);
	
	addQuad(quad);
	
	// right face
	quad[0].set(aspect, 1,  1);
	quad[1].set(aspect, 1, -1);
	quad[2].set(aspect, -1,-1);
	quad[3].set(aspect, -1, 1);
	
	addQuad(quad);
	
}
void Room::addQuad(ofVec3f *quad) {
	
	ofVec2f dims = ofVec2f(tex.getWidth(), tex.getHeight());
	mesh.addTexCoord(ofVec2f(0, 0)*dims);
	mesh.addVertex(quad[0]);
	
	mesh.addTexCoord(ofVec2f(1, 0)*dims);
	mesh.addVertex(quad[1]);
	
	
	
	mesh.addTexCoord(ofVec2f(1, 1)*dims);
	mesh.addVertex(quad[2]);
	
	mesh.addTexCoord(ofVec2f(1, 1)*dims);
	mesh.addVertex(quad[2]);
	
	mesh.addTexCoord(ofVec2f(0, 1)*dims);
	mesh.addVertex(quad[3]);
	
	
	mesh.addTexCoord(ofVec2f(0, 0)*dims);
	mesh.addVertex(quad[0]);
	
	
	// calculate normal
	ofVec3f u = quad[1] - quad[0];
	ofVec3f v = quad[3] - quad[0];
	ofVec3f n;
	n.x = u.y*v.z - u.z*v.y;
	n.y = u.z*v.x - u.x*v.z;
	n.z = u.x*v.y - u.y*v.x;
	
	n = n.normalize();
	mesh.addNormal(n);
	mesh.addNormal(n);
	mesh.addNormal(n);
	mesh.addNormal(n);
	mesh.addNormal(n);
	mesh.addNormal(n);
}


