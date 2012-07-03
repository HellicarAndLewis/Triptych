#include "Application.h"

Application::Application(const int w, const int h)
	:flock(flock_ps.particles, 50, 5)
	,viz(flock_ps, fx_ps, players)
	,control(flock_ps, fx_ps, players, w, h)
{
}

void Application::setup() {
	// create players
	int max_players = 1;
	for(int i = 0; i < max_players; ++i) {
		Player* p = new Player(flock_ps);
		p->setup();
		players.push_back(p);
	}
		
	// setup input controller	
	control.setup();
	
	viz.setup();
}

void Application::update() {
	flock.setMaxSpeed(settings.flocking_max_speed);
	flock.update();
	
	flock_ps.update(0.3);
	fx_ps.update(0.3);
	
	control.update();
	
	
	flock_ps.removeDeadParticles();
	fx_ps.removeDeadParticles();
	
	viz.update();
}

void Application::draw(const Mat4& pm, const Mat4& vm, const Mat3& nm) {
	viz.draw(pm, vm, nm);
}

void Application::debugDraw() {
	viz.debugDraw();
}


