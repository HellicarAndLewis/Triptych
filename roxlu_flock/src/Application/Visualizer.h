#ifndef ROXLU_VISUALIZERH
#define ROXLU_VISUALIZERH

/*

#include "Roxlu.h"
#include "PBD.h"
#include "Visualizer.h"
#include "Boidtypes.h"
#include "Player.h"
#include "ExplosionTrails.h"
#include "Cloak.h"
*/

#include <roxlu/Roxlu.h>
#include <pbd/PBD.h>

#include <application/KinectInput.h>
#include <application/Visualizer.h>
#include <application/BoidTypes.h>
#include <application/Player.h>
#include <application/visuals/ExplosionTrails.h>
#include <application/visuals/Cloak.h>
#include <application/visuals/KinectDrawer.h>


class Visualizer {
public:
	Visualizer(Boids2& flockPS, Boids2& fxPS, vector<Player*>& players, KinectInput& kinect);
	void setup();
	void update();
	void draw(const Mat4& pm, const Mat4& vm, const Mat3& nm);
	void debugDraw();


	vector<Player*>& players;
	vector<Cloak*> cloaks;
	
	Boids2& flock_ps;
	Boids2& fx_ps; // effects!
	ExplosionTrails explosion_trails;
	KinectInput& kinect_input;
	KinectDrawer kinect_drawer;
	
	Billboard bb;
	Texture glow_tex;
	Texture twinkle_tex;
	Texture mega_glow_tex;
	
private:
};

#endif