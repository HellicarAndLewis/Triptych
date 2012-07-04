#ifndef ROXLU_PLAYERH
#define ROXLU_PLAYERH

/*
#include "SimpleAnim.h"
#include "BoidTypes.h"
#include "Settings.h"
#include "Boid.h"
*/

#include <roxlu/experimental/SimpleAnim.h>
#include <application/BoidTypes.h>
#include <application/Settings.h>
#include <application/visuals/Boid.h>

// A user in front of the installation (or test mode).

class Player {
public:
	Player(Boids2& flockPS);
	
	void setup();
	void update();
	
	void operator()(Boid2& a, Boid2& b, Vec2& dir, float f, float ls, int what);

	Boids2& flock_ps;
	Boid2* left_hand;
	Boid2* right_hand;
	Boid2* bottom;
	SimpleAnim anim; // test input
	
	//Particles2 cloth;
	Particles3 cloth;
	Particle3* cloth_top_left;
	Particle3* cloth_top_right;
	vector<Triangle> tris;
	
	Boid boid; // test
};

inline void Player::operator()(Boid2& a, Boid2& b, Vec2& dir, float f, float ls, int what) {
	if(what == CB_REPEL_PARTICLE) {
		if(f > 1.6) {
			b.grow_trail_end = Timer::now() + (uint64_t)settings.boid_trail_duration_millis;
		}
	}
}


#endif