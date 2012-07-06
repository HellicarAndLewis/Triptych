#ifndef ROXLU_CONTROLLERH
#define ROXLU_CONTROLLERH

/*
#include "PBD.h"
#include "BoidTypes.h"
#include "Player.h"
*/

#include <pbd/PBD.h>
#include <application/BoidTypes.h>
#include <application/KinectInput.h>

class Controller {
public:
	Controller(Boids& flockPS, Boids& fxPS, int w, int h);
	void setup();
	void update();
	void checkBounds();

	Boids& flock_ps;
	Boids& fx_ps;
	int w;
	int h;

};

#endif