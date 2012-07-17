#include <application/Application.h>

Application::Application(const int w, const int h)
	:flock(flock_ps.particles, 3.0240, 5)
	,control(flock_ps, fx_ps, kinect, w, h)
	,viz(flock_ps, fx_ps, kinect, control, w, h)
{
}

void Application::setup() {
	kinect.setup();
	control.setup();
	viz.setup();
}


void Application::update() {

	
	// update kinect and tell drawer if we detected someone...
	/*
	bool user_detected = kinect.update() && kinect.num_blobs > 0;
	viz.kinect_drawer.setUserDetected(user_detected);
	*/
	flock.update();
	flock_ps.update(0.3);

	fx_ps.update(0.3);
	control.update();

	flock_ps.removeDeadParticles();
	fx_ps.removeDeadParticles();
	viz.update();
}


void Application::draw(
	 const float* pm // projection matrix
	,const float* vm // view matrix
	,const float* nm // normal matrix
	,const float* rightVec // right vec for 
	,const float* upVec
)
{
	viz.draw(pm, vm, nm, rightVec, upVec);
}

void Application::debugDraw() {
	viz.debugDraw();
}

void Application::resize(int w, int h) {
	viz.resize(w, h);
}


