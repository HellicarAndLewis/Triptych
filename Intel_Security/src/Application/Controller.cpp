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
	if(num_points == 0) {
		return;
	}
	
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
				

				int amode = 1;
				if(amode == 0) { // testing with modes.
					// attact to point.
					if(ls > 0.3f) {
						float F = 1.0f - (1.0/ls);
						dir.normalize();
						dir *= F * settings.boid_attack_energy;
						//b.addForce(dir);
					}
					else {
						b.attack_end = now-1;
					}
				}
				else if(amode == 1) {
					if(ls > settings.attract_to_user_radius) {
						float F = 1.0f/ls;	
						dir.normalize();
						dir *= F * settings.attract_to_user_energy;
						dir *= -1;
						b.addForce(dir);
					}
					else {
						b.attack_end = now-1;
					}
				}
				
				//++dx_point;
			
			}
		}
		else if(b.mode == B_MODE_ATTACK && b.attack_end < now) {
			b.mode = B_MODE_DEFAULT;
			//b.attack_start = 0;
			b.attack_end = 0;
			b.attack_delay = now + random(settings.boid_attack_delay_min, settings.boid_attack_delay_max);
		}
	}

}
	
	
void Controller::checkBounds() {
	// Make sure the boids stay in a sphere.
	float ls;
	float range_sq = settings.flocking_sphere_size * settings.flocking_sphere_size;
	for(Boids::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
		Boid& p = **it;
		// @todo when boids "Attract" we need to enable this.. 
		if(p.mode == B_MODE_ATTACK) { 
			//continue;
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
