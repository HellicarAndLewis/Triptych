#include "SphereUnit.h"

SphereUnit::SphereUnit() 
	:centerp(NULL)
{

}


void SphereUnit::setup(Particle* c) {
	centerp = c;
}

void SphereUnit::addParticles(const unsigned int& num) {
	if(centerp == NULL) {
		printf("Cannot add particles because no center particle was set yet.\n");
		return;
	}
	
	Vec3 pos;
	Vec3 lhpos = centerp->position;
	Spring* s;
	for(int i = 0; i < num; ++i) {
		pos = lhpos + randomVec3() * 2;
		Particle* p = ps.createParticle(pos);
		ps.addParticle(p);
		p->lifespan = random(30.0f, 150.0f);
		s = ps.addSpring(ps.createSpring(p, centerp));
		s->k = 1.1;
	}

}

void SphereUnit::update() {	
	float s = Timer::millis()* 0.0001;
	s = noise1(s);
	ps.addForce(0,13*s,0);
	ps.repel(0.3);
	ps.update(0.3);
	
	for(vector<Particle*>::iterator it = ps.particles.begin(); it != ps.particles.end(); ++it) {
		Particle& p = *(*it);
		if(p.agep > 0.6) {
			if(p.agep > 0.95) {
			//	p.springs[0]->enable();
			}
			else {
			//	p.springs[0]->disable();
			}
		}
		
	}
}

void SphereUnit::debugDraw() {
	ps.draw();
}