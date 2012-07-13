#include "ofMain.h"
#include "Visualizer.h"

Visualizer::Visualizer(Boids& flockPS, Boids& fxPS, KinectInput& kinect, Controller& controller)
	:flock_ps(flockPS)
	,fx_ps(fxPS)
	,controller(controller)
	,trails_drawer(flockPS)
	,kinect_input(kinect)
{
}

void Visualizer::setup() {
	
	
	glEnable(GL_TEXTURE_2D);
	ofImage img;
	if(!img.loadImage("particle_glow.png")) {	
		printf("Cannot load particle glow image.\n");
		::exit(0);
	}
	
	glow_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);

	trails_drawer.setup();	
	
	kinect_drawer.setup();	
	
	boid_drawer.setup();
}

void Visualizer::update() {

	trails_drawer.update();
	if(kinect_input.update()) {
		deque<KinectMesh>& meshes = kinect_input.getKinectMeshes();
		if(meshes.size() > 0) {
			kinect_drawer.update(meshes[0]);
		}
	}

}

// pm = projection matrix, vm = view matrix, nm = normal matrix
void Visualizer::draw(
	 const float* pm
	,const float* vm
	,const float* nm
	,const float* rightVec
	,const float* upVec
) 
{
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_DEPTH_TEST);

	glDepthMask(GL_TRUE);
	kinect_drawer.draw(pm, vm);
	
	glDepthMask(GL_FALSE);
	if(settings.boid_draw_glows) {
		drawGlows(flock_ps.begin(), flock_ps.end(), pm, vm, nm, rightVec, upVec);
	}

	trails_drawer.draw(pm, vm);
	glDepthMask(GL_TRUE);

	
	if(settings.draw_flock) {
		drawBoids(flock_ps.begin(), flock_ps.end(), pm, vm, nm, rightVec, upVec);
	}
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
}

void Visualizer::drawGlows(
		 Boids::iterator begin
		,Boids::iterator end
		,const float* pm
		,const float* vm
		,const float* nm
		,const float* rightVec
		,const float* upVec
)
{

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	float now = float(Timer::millis());
	
	bb.setTexture(glow_tex.getID());
	bb.bind(pm, vm, rightVec, upVec);
	for(Boids::iterator it = begin; it != end; ++it) {
		Boid& b = **it;
		if(!b.is_visible || b.mode == B_MODE_DEFAULT) {
			continue;
		}
		float a = now/b.attack_end;
		bb.draw(b.position, b.size * settings.boid_glow_size, 0.0, a);
	}
	bb.unbind();
}


void Visualizer::drawBoids(
		 Boids::iterator begin
		,Boids::iterator end
		,const float* pm
		,const float* vm
		,const float* nm
		,const float* rightVec
		,const float* upVec
	)
{

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	Mat3 cs;
	Vec3 up(0,1,0);

	for(Boids::iterator it = begin; it != end; ++it) {
		Boid& b = **it;
		if(!b.is_visible) {
			continue;
		}
		
		boid_drawer.direction = b.velocity.getNormalized();
		boid_drawer.position = b.position;
		boid_drawer.size = b.size  * settings.boid_scale;
		cs.makeCoordinateSystem(boid_drawer.direction);
		boid_drawer.draw(pm, vm, nm, cs);
	}
	
}


void Visualizer::debugDraw() {
	if(!settings.draw_flock) {
		return;
	}
	

	// FX 
	glColor3f(0,1,1);
	::draw(fx_ps);	
	
	// Flocking
	if(settings.draw_flock) {
		glColor3f(1,1,1);
		::draw(flock_ps);
	}
	
	// debug draw boid with coordinates
	Mat3 cs;
	Vec3 up(0,1,0);
	for(Boids::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
		Boid& b = **it;
		if(!b.is_visible) {
			continue;
		}
		boid_drawer.direction = b.velocity.getNormalized();
		boid_drawer.position = b.position;
		cs.makeCoordinateSystem(boid_drawer.direction);
		boid_drawer.debugDraw(cs);
	}

	kinect_drawer.debugDraw();
	
	deque<KinectMesh>& meshes = kinect_input.getKinectMeshes();
	if(meshes.size() > 0) {
		kinect_drawer.debugDrawMesh(meshes[0]);
	}

	// draw attackers
	if(settings.debugdraw_attackers) {
		glColor3f(1,0,1);
		glPointSize(8.0f);
		glBegin(GL_POINTS);
		for(Boids::iterator it = controller.attackers.begin(); it != controller.attackers.end(); ++it) {
			Boid& b = **it;
			if(!b.is_visible) {
				continue;
			}
			glVertex3fv(b.position.getPtr());
		}
		glEnd();
		glPointSize(1.0f);
	}

	// draw trails
	trails_drawer.debugDraw();
	
	// draw interactive points
	glPointSize(5.0f);
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for(int i = 0; i < kinect_input.interactive_points.size(); ++i) {
		Vec3& p = kinect_input.interactive_points[i];
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
	
}