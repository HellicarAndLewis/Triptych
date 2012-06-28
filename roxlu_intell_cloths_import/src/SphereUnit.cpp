#include "SphereUnit.h"

SphereUnit::SphereUnit(Billboard& bb) 
	:centerp(NULL)
	,particle_bb(bb)
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
//		p->lifespan = random(30.0f, 150.0f);
		p->lifespan = FLT_MAX;
		p->size = random(0.1f, 1.4f);
		s = ps.addSpring(ps.createSpring(p, centerp));
		s->k = 1.1;
	}
	
}

void SphereUnit::update() {	
	ps.removeDeadParticles();
	
	// add some new particles
	int num = 3;
	Vec3 pos;
	Vec3& center = centerp->position;
	Particle* p;
	for(int i = 0; i < num; ++i) {
		pos = randomVec3();
		pos.y = 1;
		p = ps.createParticle(pos + center);
		p->velocity = pos;
		p->velocity.x = random(0.0025,0.0013);
		p->velocity.z = random(0.0025,0.0013);

		p->velocity.y = -random(0.2, 0.04);
		p->size = random(app_settings.minParticleSize(), app_settings.maxParticleSize());
		//printf("%f\n", p->velocity.y);
		p->lifespan = random(10.0f, 15.0f);
		ps.addParticle(p);
	}
	//printf("%zu\n", ps.particles.size());
	float s = Timer::millis()* 0.0001;
	float f = app_settings.perlinInfluence();
	float perlin_scale = app_settings.perlinScale();
	if(app_settings.usePerlin()) {
		for(vector<Particle*>::iterator it = ps.particles.begin(); it != ps.particles.end(); ++it) {
			Particle& p = *(*it);
			Vec3& pos = p.position;
			float pf = noise3(pos.x * f, pos.y * f, pos.z * f);
			p.addForce(pf * perlin_scale, pf *perlin_scale , pf * perlin_scale);
		}

	}
	
//	ps.addForce(0,13*s,0);
	ps.repel(0.0003);
	ps.update(0.3);

	// test destroying springs
	/*	
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
	*/
}

void SphereUnit::draw(const Mat4& pm, const Mat4& vm, const Vec3& right, const Vec3& up) {
	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor3f(1,1,1);
	glBindVertexArrayAPPLE(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
//	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	
	particle_bb.start(pm, vm, right, up);
	for(vector<Particle*>::iterator it = ps.particles.begin(); it != ps.particles.end(); ++it) {
		Particle& p = *(*it);
		if(p.springs.size() == 0) {
			particle_bb.draw(p.position, p.size, 0.0, 1.0 - p.agep);
		}
	}
	particle_bb.stop();
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}


void SphereUnit::debugDraw() {
	glDisable(GL_BLEND);
	ps.draw();
}