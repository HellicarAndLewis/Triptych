/**
 *  Room.cpp
 *
 *  Created by Marek Bereza on 28/06/2012.
 */

#include "Room.h"


void Room::setup() {
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	tex.loadImage("room.png");
	
	aoAmt = 0.5;
	ofVec3f quad[4];

	// back face
	quad[0].set(-1, 1, 1);
	quad[1].set(1, 1,  1);
	quad[2].set(1, -1,   1);
	quad[3].set(-1, -1,  1);
	
	addQuad(quad);
	
	// bottom face
	quad[0].set(-1, -1, 1);
	quad[1].set(1, -1,  1);
	quad[2].set(1, -1, -1);
	quad[3].set(-1, -1,  -1);
	
	addQuad(quad);
	
	// top face
	quad[0].set(-1, 1, -1);
	quad[1].set(1, 1,  -1);
	quad[2].set(1, 1, 1);
	quad[3].set(-1, 1,  1);
	
	addQuad(quad);
	
	// left face
	quad[0].set(-1, 1, -1);
	quad[1].set(-1, 1,  1);
	quad[2].set(-1, -1, 1);
	quad[3].set(-1, -1,-1);
	
	addQuad(quad);
	
	// right face
	quad[0].set(1, 1,  1);
	quad[1].set(1, 1, -1);
	quad[2].set(1, -1,-1);
	quad[3].set(1, -1, 1);
	
	addQuad(quad);
	
	
	
	shader.load("pointlight.vert", "pointlight.frag");

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

void Room::draw() {
	ofSetHexColor(0xFFFFFF);
	
	shader.begin();
	shader.setUniform1f("amt", aoAmt);
	shader.setUniformTexture("tex", tex.getTextureReference(), 0);
//	tex.bind();
	mesh.draw();
//	tex.unbind();
	shader.end();

	//ofSphere(0, 0, 0, 0.5);
}