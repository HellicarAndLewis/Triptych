#include "Controller.h"


Controller::Controller(
	 Boids& flockPS
	,Boids& fxPS
	,KinectInput& kinect
	,int w
	,int h
) 
:flock_ps(flockPS)
,fx_ps(fxPS)
,kinect_input(kinect)
,w(w)
,h(h)
{
}

void Controller::setup() {
	// add some boids
	int num = 300;
	float s = 5.0f;
	Boids::Vec pos;
	for(int i = 0; i < num; ++i) {
		pos = randomVec3() * s;
		Boid* p = new Boid(pos);
		p->size = random(1.0f, 10.0f);
		p->aging = false;
		//p->velocity = randomVec3()* 2.0;
		flock_ps.addParticle(p);
	}
	
	// copy some boids to the attack list
	int num_attackers = num *0.8;
	/*
	for(int i = 0; i < num_attackers; ++i) {
		pos = randomVec3() * s;
		Boid* p = new Boid(pos);
		p->size = random(1.0f, 10.0f);
		p->aging = false;
		//p->disable();
		attackers.push_back(p);
		fx_ps.addParticle(p);

	}
	*/
	
	std::copy(flock_ps.begin(), flock_ps.begin()+num_attackers, std::back_inserter(attackers));
//	attack_particle = flock_ps.createParticle(Vec3());
}


void Controller::update() {
	

	checkBounds();
	
	// create trails.
	if(settings.boid_create_trails) {
		for(Boids::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
			Boid& b = **it;
			if(b.mode == B_MODE_ATTACK) {
				b.trail.push_back(b.position);
				b.trail.limitLength(settings.boid_trail_length);
			}
			else {
				b.trail.clear();
			}
		}
	}
	
	// check if we need to activate some attackers.
	uint64_t now = Timer::millis();
	size_t num_points = kinect_input.interactive_points.size();
	size_t dx_point = 0;
	size_t attackers_per_point = ceil(attackers.size()/num_points);
//	printf("Attackers per point: %zu\n", attackers_per_point);
	
	vector<Vec3>::iterator kinect_it = kinect_input.interactive_points.begin();
	vector<Vec3>::iterator kinect_it_end = kinect_input.interactive_points.end();
	for(Boids::iterator it = attackers.begin(); it != attackers.end(); ++it) {
		Boid& b = **it;
		//b.mode = B_MODE_ATTACK;
		if(b.mode != B_MODE_ATTACK && b.attack_delay < now) {
			b.mode = B_MODE_ATTACK;
			//b.attack_start = now + 5000; // start after 2 seconds from now
			b.attack_end = now + random(settings.boid_attack_duration_min,settings.boid_attack_duration_max); // end after 5 seconds from now
		}
		else if(b.mode == B_MODE_ATTACK && b.attack_end > now) {
			int dx = (++dx_point/attackers_per_point) % num_points;
			if(dx < num_points) {
				Vec3& point = kinect_input.interactive_points[dx];
				Vec3 dir = point - b.position;
				float ls = dir.lengthSquared();
				
				if(ls > 0.3f) {
					float F = 1.0f - (1.0/ls);
					dir.normalize();
					dir *= F * settings.boid_attack_energy;
					b.addForce(dir);
				}
				else {
//					b.mode = B_MODE_DEFAULT;
					b.attack_end = now-1;
				}
				++dx_point;
			
			}
		}
		else if(b.mode == B_MODE_ATTACK && b.attack_end < now) {
			b.mode = B_MODE_DEFAULT;
			//b.attack_start = 0;
			b.attack_end = 0;
			b.attack_delay = now + random(settings.boid_attack_delay_min, settings.boid_attack_delay_max);
		}
	}
	

	
	
	
	// test fx px.
	/*
	for(Boids::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
		Boid& b = **it;
		Boid* p = fx_ps.addParticle(b.position);
		p->velocity = -b.velocity;
		p->lifespan = random(5.0f,7.0f);
		//break;
		
	}
	
	float s = 0.01;
	for(Boids::iterator it = fx_ps.begin(); it != fx_ps.end(); ++it) {
		Boid& b = **it;
		float nx = noise3(b.position.x+0.1 * s, b.position.y * s, b.position.z * s);
		float ny = noise3(b.position.x * s, b.position.y+0.1* s, b.position.z * s);
		float nz = noise3(b.position.x * s, b.position.y * s, b.position.z+0.1 * s);
		b.addForce(Vec3(nx,ny,nz));
		
	}
	*/
//	fx_ps.addForce(Vec3(0,0.1,0));
	
	/*
	vector<Boid*> new_particles;
	float pf;
	for(Boids::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
		Boid& b = **it;
		Boids::Vec& pos = b.position;

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
				Boid* exp = new Boid(npos);
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
	for(Boids::iterator it = fx_ps.begin(); it != fx_ps.end(); ++it) {  
		Boid& b = **it;
		Boids::Vec& pos = b.position;
		
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
	*/
}
	
	
void Controller::checkBounds() {
	// Make sure the boids stay in a sphere.
	float ls;
	float range_sq = settings.flocking_sphere_size * settings.flocking_sphere_size;
	for(Boids::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
		Boid& p = **it;
		if(p.mode == B_MODE_ATTACK) {
			continue;
		}
		
		Boids::Vec dir = -p.position;
		ls = dir.lengthSquared();
		if(ls > range_sq)  {
			float F = 1.0 - (1.0f/ls);
			dir.normalize();
			p.addForce(dir * (F * settings.flocking_center_energy) );
		}
	}
	
}
