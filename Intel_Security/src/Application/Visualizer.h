#ifndef ROXLU_VISUALIZERH
#define ROXLU_VISUALIZERH

#include <roxlu/Roxlu.h>
#include <pbd/PBD.h>

#include <application/KinectInput.h>
#include <application/Visualizer.h>
#include <application/BoidTypes.h>

#include <application/visuals/TrailsDrawer.h>
#include <application/visuals/KinectDrawer.h>
#include <application/visuals/BoidDrawer.h>
#include <application/Blur.h> // test

class DebugTrail {
public:
	void operator()(const float perc, Vec4& col) {
		col.set(1.0-perc, perc*0.5, perc*0.2, perc);
	}
};

struct BoidPartitioner {
	bool operator()(Boid* boid) {
		return boid->position.z < 0;
	}
};

class Visualizer {
public:
	Visualizer(Boids& flockPS, Boids& fxPS, KinectInput& kinect);
	void setup();
	void update();
	//void draw(const Mat4& pm, const Mat4& vm, const Mat3& nm);
	void draw(const float* pm, const float* vm, const float* nm, const float* rightVec, const float* upVec);
	void drawBoids(Boids::iterator begin, Boids::iterator end,const float* pm, const float* vm, const float* nm, const float* rightVec, const float* upVec);
	void drawGlows(Boids::iterator begin, Boids::iterator end,const float* pm, const float* vm, const float* nm, const float* rightVec, const float* upVec);
	void debugDraw();
	
	Boids& flock_ps;
	Boids& fx_ps; // effects!
	TrailsDrawer trails_drawer;
	KinectInput& kinect_input;
	KinectDrawer kinect_drawer;
	BoidDrawer boid_drawer;
	
	Billboard bb;
	Texture glow_tex;
	Texture twinkle_tex;
	Texture mega_glow_tex;
	
	// TESTING WITH TRAILS
	Trail3PC	test_trail;

	
private:
};

#endif