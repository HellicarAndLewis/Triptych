#ifndef ROXLU_CONTROLLERH
#define ROXLU_CONTROLLERH

/*
#include "PBD.h"
#include "BoidTypes.h"
#include "Player.h"
*/
#include <pbd/PBD.h>
#include <application/BoidTypes.h>
#include <application/Player.h>

class Controller {
public:
	Controller(Boids2& flockPS, Boids2& fxPS, vector<Player*>& players, int w, int h);
	void setup();
	void update();
	void checkBounds();
	
	vector<Player*>& players;
	Boids2& flock_ps;
	Boids2& fx_ps;
	int w;
	int h;
};

#endif