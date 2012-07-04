#ifndef ROXLU_BOID_TYPESH
#define ROXLU_BOID_TYPESH

/*
#include "PBD.h"
#include "Vec2.h"
#include <deque>
#include "Settings.h"
#include "Trail.h"
*/

#include <deque>

#include <pbd/PBD.h>
#include <application/Settings.h>
#include <application/visuals/Trail.h>
#include <roxlu/math/Vec2.h>


template<class T>
struct BoidParticle : public Particle<T> {
	BoidParticle(const T& pos, float m = 1.0);
	void update(const float dt);
	void removeTrail();
	
//	std::deque<T> trail;
	Trail2PC trail;
	uint64_t grow_trail_end;
	uint64_t glow_end;
};


template<class T>
BoidParticle<T>::BoidParticle(const T& pos, float m)
	:Particle<T>(pos, m)
	,grow_trail_end(0)
	,glow_end(0)
{
}


template<class T>
void BoidParticle<T>::update(const float dt) {

}

template<class T>
inline void BoidParticle<T>::removeTrail() {
	trail.clear();
}


typedef Particles<Vec2, BoidParticle<Vec2>, Spring<Vec2> >	Boids2;
typedef BoidParticle<Vec2> 	Boid2;
typedef Flocking<Vec2, BoidParticle<Vec2> > BoidFlocking2;

#endif