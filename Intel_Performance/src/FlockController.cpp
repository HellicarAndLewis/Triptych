#include "FlockController.h"
#include "ofxSimpleGuiToo.h"

FlockController::FlockController()
	:flock(ps.particles, 1.5, 0.9)
{
}

void FlockController::setup() {
	int n = 300;
	float r = 4.5;
	for(int i = 0; i < n; ++i) {
		ofVec3f pos(ofRandom(-r,r),ofRandom(-r,r),ofRandom(-r,r));
		ps.addParticle(pos);
	}

	flock.zone_radius_sq = 3.60f;
	flock.low = 0.4f;  
	flock.high = 0.6f;
	flock.align_energy = 0.0013f; 
	flock.separate_energy = 0.0012f;
	flock.attract_energy = 0.0011f;
	fluff.loadImage("fluff.png");
	fluff.setAnchorPercent(0.5, 0.5);
}

void FlockController::setupGui() {
	gui.addTitle("Flocking");
	gui.addSlider("Low threshold", flock.low, 0.0f, 1.0f);
	gui.addSlider("High threshold", flock.high, 0.0f, 1.0f);
	gui.addSlider("Align energy", flock.align_energy, 0.0f, 0.01);
	gui.addSlider("Separate energy", flock.separate_energy, 0.0f, 0.01);
	gui.addSlider("Attract energy", flock.attract_energy, 0.0f, 0.01);
	gui.addSlider("Center energy", center_energy, 0.0f, 4.0f);
	gui.addSlider("Bounding sphere size", sphere_size, 0.0f, 5.0f);
}

void FlockController::update() {
	flock.update();
	ps.update(0.3);
	checkBounds();
}

void FlockController::debugDraw() {
	ofDisableLighting();
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	glColor3f(1,1,1);
	/*glPointSize(3.0f);
	glBegin(GL_POINTS);
	for(int i = 0; i < ps.size(); ++i) {
		glVertex3fv(ps[i]->position.getPtr());	
	}
	glEnd();*/
	
	for(int i = 0; i < ps.size(); ++i) {
		fluff.draw(ps[i]->position, 0.06, 0.06);
	}
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

void FlockController::checkBounds() {
	float range_sq = sphere_size * sphere_size;;
	for(BoidParticles3::iterator it = ps.begin(); it != ps.end(); ++it) {
		BoidParticle3& p = **it;
		float ls = p.position.lengthSquared();
		if(ls > range_sq) {
			float F = 1.0f/ls;
			ofVec3f dir = -p.position;
			dir.normalize();
			dir *= F * center_energy;
			p.addForce(dir);
			
		}
	}
}