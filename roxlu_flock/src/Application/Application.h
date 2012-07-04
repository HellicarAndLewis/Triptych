#ifndef ROXLU_APPLICATIONH
#define ROXLU_APPLICATIONH

/*
#include "BoidTypes.h"
#include "PBD.h"
#include "Settings.h"
#include "Visualizer.h"
#include "Controller.h"
#include "Player.h"
*/

#include <application/BoidTypes.h>
#include <application/Settings.h>
#include <application/Visualizer.h>
#include <application/Controller.h>
#include <application/Player.h>
#include <pbd/PBD.h>
#include <roxlu/Roxlu.h>


class Application {
public:
	Application(const int w, const int h);
	void setup();
	void update();
	void draw(const Mat4& pm, const Mat4& vm, const Mat3& nm);
	void debugDraw();
	
	vector<Player*> players;
	Boids2 flock_ps;
	Boids2 fx_ps; 
	BoidFlocking2 flock;
	Visualizer viz;
	Controller control;
};
#endif