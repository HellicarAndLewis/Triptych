#include "Controller.h"


Controller::Controller(Boids2& flockPS, Boids2& fxPS, vector<Player*>& players, int w, int h) 
	:flock_ps(flockPS)
	,fx_ps(fxPS)
	,w(w)
	,h(h)
	,players(players)
{
}

void Controller::setup() {
	// add some boids
	int num = 500;
	Vec2 pos;
	float cx = w * 0.5f;
	float cy = h * 0.5f;
	for(int i = 0; i < num; ++i) {
		pos = randomVec2() * 300.0f;
		pos.x += cx;
		pos.y += cy;
	
		Boid2* p = new Boid2(pos);
		p->size = random(1.0f, 10.0f);
		p->aging = false;
		flock_ps.addParticle(p);
	}
	
	//player1.setup();
}

void Controller::update() {
	

	checkBounds();
	
	vector<Boid2*> new_particles;
	float pf;
	for(vector<Boid2*>::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
		Boid2& b = **it;
		Vec2& pos = b.position;
		
		// PERLIN
		if(settings.flocking_apply_perlin) {
			pf = noise2(pos.x * settings.flocking_perlin_influence, pos.y * settings.flocking_perlin_influence);
			pf *= settings.flocking_perlin_scale;
			b.addForce(Vec2(pf, pf));
		}
		
		// TRAIL
		if(b.grow_trail_end != 0) {
			if(Timer::now() > b.grow_trail_end) {	
				b.grow_trail_end = 0;
			}
			else {	
				b.trail.push_back(b.position);
				if(b.trail.size() > 20) {
					b.trail.pop_front();
				}
			}
		}
		else if(b.trail.size() > 0){
			b.trail.push_back(b.position);
			b.trail.pop_front();
			b.trail.pop_front();
			
			if(b.trail.size() == 0) {
				b.glow_end = Timer::now() + (uint64_t)settings.boid_glow_duration_millis;
			}
		}
		
		// GLOW
		if(b.glow_end != 0 && b.glow_end < Timer::now()) {
			b.glow_end = 0;
			
			// CREATE EXPLOSION PARTICLES
			int num = 10;
			float range = 20.0f;
			
			for(int i = 0; i < num; ++i) {
				Vec2 npos = b.position + randomVec2()*range;
				Boid2* exp = new Boid2(npos);
				exp->lifespan = random(settings.explosion_min_lifespan, settings.explosion_max_lifespan);
				exp->velocity = -b.velocity ;
			//	exp->velocity.x *= random(-settings.explosion_random_x_vel, settings.explosion_random_x_vel);
			//	exp->velocity.y *= random(-settings.explosion_random_y_vel, settings.explosion_random_y_vel);
				//exp->disable();
				exp->size = random(1.0f, 3.5f);
				fx_ps.addParticle(exp);
			} 
		}
	}
	
	// EXPLOSIONS
	float t = Timer::now() * 0.001;
	for(Boids2::iterator it = fx_ps.begin(); it != fx_ps.end(); ++it) {  
		Boid2& b = **it;
		Vec2& pos = b.position;
		
		// apply perlin.
		float nx1 = noise2(pos.x * settings.explosion_perlin_influence, t);
		float nx2 = noise2(pos.x * settings.explosion_perlin_influence, t * 1.1);
		float nx = nx1-nx2;
		float ny1 = noise2(pos.y * settings.explosion_perlin_influence, t);
		float ny2 = noise2(pos.y * settings.explosion_perlin_influence, t * 1.1);
		float ny = ny1-ny2;
		pf *= settings.explosion_perlin_scale;
		b.addForce(Vec2(nx * settings.explosion_perlin_scale, ny * settings.explosion_perlin_scale));
		
		// grow trail?
		if(settings.explosion_trail_length > 0) {
			if(b.trail.size() < settings.explosion_trail_length) {
				b.trail.push_back(pos);
			}
			else {
				b.trail.pop_front();
			}
			
		}
	}
	
	for(vector<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
		(*it)->update();
	}
	//player1.update();
}
	
	
void Controller::checkBounds() {
	
	for(vector<Boid2* >::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
		Boid2& p = **it;
		if(p.position.x < 0) {
			p.position.x = w;
			p.removeTrail();
		}
		else if(p.position.x > w) {
			p.position.x = 0;
			p.removeTrail();
		}
		if(p.position.y < 0) {
			p.position.y = h;
//			p.position.set(320,320);
			p.removeTrail();
		}
		else if(p.position.y > h) {
			p.position.y = 0;
//			p.position.set(320,320);
			p.removeTrail();
		}
	}
	
}
