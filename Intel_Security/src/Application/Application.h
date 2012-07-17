#ifndef ROXLU_APPLICATIONH
#define ROXLU_APPLICATIONH

#include <roxlu/Roxlu.h>
#include <pbd/PBD.h>
#include <application/BoidTypes.h>
#include <application/Settings.h>
#include <application/Visualizer.h>
#include <application/Controller.h>
#include <application/KinectInput.h>

class Application {
public:
	Application(const int w, const int h);
	void setup();
	void update();
	void draw(const float* pm, const float* vm, const float* nm, const float* rightVec, const float* upVec);
	void debugDraw();
	void resize(int w, int h);
	
	KinectInput kinect;
	Boids flock_ps;
	Boids fx_ps; 
	BoidFlocking flock;
	
	Controller control;
	Visualizer viz;
};

#endif