#ifndef ROXLU_CONTROLLERH
#define ROXLU_CONTROLLERH

/**
 * The controller class controls the boid interaction. 
 * Some things where easier to add in BoidTypes, so some code which
 * influences the boids is there (i.e. the BoidForceAdder functor).
 *
 */
 
#include <pbd/PBD.h>
#include <application/BoidTypes.h>
#include <application/KinectInput.h>
#include <vector>

using std::vector;

class Controller {
public:
	Controller(Boids& flockPS, Boids& fxPS, KinectInput& kinect, int w, int h);
	void setup();
	void update();
	void checkBounds();
	void setVisibleBoidsPercentage(float perc); 
	void setAttackPercentage(float perc);
	KinectInput& kinect_input;
	Boids& flock_ps;
	Boids& fx_ps;
	int w;
	int h;
	
	vector<Boid*> attackers;
	
};

#endif