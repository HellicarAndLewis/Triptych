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
	gui.addSlider("Zone radius squared", flock.zone_radius_sq, 0.01, 10.0f);
	gui.addSlider("Low threshold", flock.low, 0.0f, 1.0f);
	gui.addSlider("High threshold", flock.high, 0.0f, 1.0f);
	gui.addSlider("Align energy", flock.align_energy, 0.0f, 0.001);
	gui.addSlider("Separate energy", flock.separate_energy, 0.0f, 0.001);
	gui.addSlider("Attract energy", flock.attract_energy, 0.0f, 0.001);
	gui.addSlider("Center energy", center_energy, 0.0f, 0.1f);
	gui.addSlider("Bounding sphere size", sphere_size, 0.0f, 1.5f);

}

void FlockController::update(unsigned char *img) {
	flock.update();
	ps.update(0.3);
	checkBounds();
	
	int numDeaders = 0;
	for(int i = 0; i < ps.size(); i++) {
		float x = ps[i]->position.x;
		float y = ps[i]->position.y;
		if(x<-1 || x>1 || y<-1 || y>1) {
			// particle offscreen
		} else {
			x = ofMap(x, 1, -1, 0, 640);
			y = ofMap(y, 1, -1, 0, 480);
			int offset = ((int)x) + ((int)y)*640;
			if(img[offset]>0) {
				ps[i]->kill();
				explosions.push_back(Explosion());
				explosions.back().init(ps[i]->position);
				numDeaders++;
//			} else {
//				ps[i]->intersecting = false;
			}
		}
	}
	
	ps.removeDeadParticles();
	for(int i = 0; i < numDeaders; i++){
		ofVec3f pos(2, 0);
		pos.rotate(ofRandom(0, 360), ofVec3f(0, 0, 1));
		ps.addParticle(pos);
	}
}

void FlockController::debugDraw() {
	ofDisableLighting();
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	glColor3f(1,1,1);
	
	
	for(int i = 0; i < ps.size(); ++i) {
		/*if(ps[i]->intersecting) {
			glColor3f(1,0,0);
		} else {
			glColor3f(1,1,1);
		}*/
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
			float F = 1.0 - (1.0f/ls);
			ofVec3f dir = -p.position;
			dir.normalize();
			dir *= F * center_energy;
			p.addForce(dir);
			
		}
	}
}