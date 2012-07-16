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
	num_attackers = num;
		
	std::copy(flock_ps.begin(), flock_ps.begin()+num_attackers, std::back_inserter(attackers));
}


void Controller::update() {
	checkBounds();
	
	// create trails.
	if(settings.boid_create_trails) {
		for(Boids::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
			Boid& b = **it;
			
			if(!b.is_visible) {
				continue;
			}
			
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
	size_t attackers_per_point = (size_t)ceil(float(attackers.size())/num_points);
	
	for(Boids::iterator it = attackers.begin(); it != attackers.end(); ++it) {
		Boid& b = **it;
		
		if(!b.is_visible) {
			continue;
		}
		
		if(b.mode != B_MODE_ATTACK && b.attack_delay < now) {
			b.mode = B_MODE_ATTACK;
			b.attack_end = now + random(settings.boid_attack_duration_min,settings.boid_attack_duration_max); // end after 5 seconds from now
		}
		else if(b.mode == B_MODE_ATTACK && b.attack_end > now) {
			int dx = (++dx_point/attackers_per_point) % num_points;
			if(dx < num_points) {
				Vec3& point = kinect_input.interactive_points[dx];
				Vec3 dir = point - b.position;
				float ls = dir.lengthSquared();
				
		
				int amode = -1;				
				if(ls > settings.attract_to_user_radius) {
					amode = 0;
				}	
				else if(ls < settings.repel_from_user_radius) {
					amode = 1;
				}

				if(amode == 0) { 
					// attract
					float len = dir.length();
					float F = 1.0f - (1.0/len);
					dir/=len;
					dir *= F * settings.attract_to_user_energy;
					b.addForce(dir);
				}
				else if(amode == 1) {
					// repel				
					float len = dir.length();
					float F = 1.0f/len;
					dir /= len;
					dir *= F * settings.repel_from_user_energy;
					dir *= -1;
					b.addForce(dir);
				}
			}
		}
		else if(b.mode == B_MODE_ATTACK && b.attack_end < now) {
			b.mode = B_MODE_DEFAULT;
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
		
		if(!p.is_visible) {
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

// set how many boids should be visible. used to tweak the interaction.
void Controller::setVisibleBoidsPercentage(float perc) {
	int num = clamp<float>(perc,0.0f,1.0f) * flock_ps.size();
	for(int i = 0; i < flock_ps.size(); ++i) {
		flock_ps[i]->is_visible = i < num;
	}
}


// how many of the visible boids should be attackers
void Controller::setAttackPercentage(float perc) {
	int c = 0;
	for(int i = 0; i < flock_ps.size(); ++i) {
		flock_ps[i]->mode = B_MODE_DEFAULT;
		if(flock_ps[i]->is_visible) {
			++c;
		}
	}
	float p = clamp<float>(perc, 0.0f, 1.0f);
	int attack_total = p * c;
	int attack_num = 0;
	attackers.clear();
	for(int i = 0; i < flock_ps.size(); ++i) {
		if(attack_num >= attack_total) {
			break;
		}
		if(flock_ps[i]->is_visible) {
			flock_ps[i]->mode = B_MODE_ATTACK;
			attackers.push_back(flock_ps[i]);
		}
	}
}
