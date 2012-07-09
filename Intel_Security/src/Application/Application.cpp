#include <application/Application.h>

Application::Application(const int w, const int h)
	:flock(flock_ps.particles, 50, 5)
	,viz(flock_ps, fx_ps, kinect)
	,control(flock_ps, fx_ps, w, h)
{
}

void Application::setup() {

	kinect.setup();
	control.setup();
	viz.setup();
}

void Application::update() {
	kinect.update();

	//flock.setMaxSpeed(settings.flocking_max_speed);
	flock.update();
	flock_ps.update(0.3);
	fx_ps.update(0.3);
	
	control.update();
	flock_ps.limitSpeed(0.4);
	
	flock_ps.removeDeadParticles();
	fx_ps.removeDeadParticles();
	
	viz.update();
}

void Application::draw(const float* pm, const float* vm, const float* nm, const float* rightVec, const float* upVec) {
	viz.draw(pm, vm, nm, rightVec, upVec);
}

void Application::debugDraw() {
	viz.debugDraw();
}


