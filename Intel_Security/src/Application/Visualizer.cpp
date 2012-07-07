#include "ofMain.h"
#include "Visualizer.h"

Visualizer::Visualizer(Boids& flockPS, Boids& fxPS, KinectInput& kinect)
	:flock_ps(flockPS)
	,fx_ps(fxPS)
	,explosion_trails(fxPS)
	,kinect_input(kinect)
{
}

void Visualizer::setup() {
	glEnable(GL_TEXTURE_2D);
	
	// load glow image.
	ofImage img;
	if(!img.loadImage("particle_glow.png")) {	
		printf("Cannot load particle glow image.\n");
		::exit(0);
	}
	glow_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);


	// load image for twinkle fx
	if(!img.loadImage("twinkle.png")) {
		printf("Cannot load twinkle image.\n");
		::exit(0);
	}
	twinkle_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);

	// load mega glow, shown when boid explodes
	if(!img.loadImage("mega_glow.png")) {
		printf("Cannot load mega glow texture.\n");
		::exit(0);
	}
	mega_glow_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);

	
	explosion_trails.setup();	
	
	kinect_drawer.setup();
	
	boid_drawer.setup();
}

void Visualizer::update() {
	explosion_trails.update();
		
	if(kinect_input.update()) {
		deque<KinectMesh>& meshes = kinect_input.getKinectMeshes();
		kinect_drawer.update(meshes[0]);
	}
}



// pm = projection matrix, vm = view matrix, nm = normal matrix
void Visualizer::draw(const float* pm, const float* vm, const float* nm) {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	kinect_drawer.draw(pm, vm);
	
	/*
	Vec3 up(0,1,0);
	Mat3 cs; // coordinate system
	for(Particles3::iterator it = ps.begin(); it != ps.end(); ++it) {
		Particle3& p = **it;
		boid.direction = p.velocity.getNormalized();
		boid.position = p.position;
		cs.makeCoordinateSystem(boid.direction, up);
		boid.debugDraw(cs);
	}
	*/
	
	// Boids
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
//	glEnable(GL_DEPTH_TEST);
	Mat3 cs;
	Vec3 up(0,1,0);
	for(Boids::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
		Boid& b = **it;
		boid_drawer.direction = b.velocity.getNormalized();
		boid_drawer.position = b.position;
		boid_drawer.size = b.size  * 0.1;
		cs.makeCoordinateSystem(boid_drawer.direction, up);
		boid_drawer.draw(pm, vm, nm, cs);
		//boid_drawer.debugDraw(cs);
		//bb.draw(b.position, b.size * 5, 0.0, 1.0);
	}
	
	/*
	void BoidDrawer::draw(const float* pm, const float* vm, const float* nm, const float* coordinateSystem) {
	*/
	
	// Boids: dot
	/*
	bb.setTexture(glow_tex.getID());
	bb.bind(pm, vm);
	for(Boids::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
		Boid& b = **it;
		bb.draw(b.position, b.size * 5, 0.0, 1.0);
	}
	bb.unbind();
	
	// Boids: glow
	bb.setTexture(glow_tex.getID());
	bb.bind(pm, vm);
	for(Boids::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
		Boid& b = **it;
		if(b.glow_end > 0) {
			float p = float(b.glow_end - Timer::now()) / settings.boid_glow_duration_millis;
			bb.draw(b.position, b.size*20, 0.0, sin(p*PI));
		}
	}
	bb.unbind();
	
	// Twinkles (explosions)
	bb.setTexture(twinkle_tex.getID());
	bb.bind(pm, vm);
	for(Boids::iterator it = fx_ps.begin(); it != fx_ps.end(); ++it) {
		Boid& b = **it;

		bb.draw(b.position, b.size * 5, 0.0, sin(b.agep * PI));
	}
	bb.unbind();
	
	
	// Exploision trails
	explosion_trails.draw(pm, vm);
	*/
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	

}

void Visualizer::debugDraw() {
	// FX 
	glColor3f(0,1,1);
	::draw(fx_ps);	
	
	// Flocking
	glColor3f(1,1,1);
	::draw(flock_ps);
	
	/*
	for(Boids::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
		Boid& b = **it;
	
		// Boid trails
		glBegin(GL_LINE_STRIP);
		for(deque<Vec2>::iterator trit = b.trail.begin(); trit != b.trail.end(); ++trit) {
			::drawVertex(*trit);
		}
		glEnd();
		
		
		// Glowing particles
		if(b.glow_end > 0) {
			glColor3f(0,1,0);	
			glBegin(GL_POINTS);
				::drawVertex(b.position);
			glEnd();
			glColor3f(1,1,1);
		}
	}
	
	// Explosion trails
	explosion_trails.debugDraw();
	*/
	
	Mat3 cs;
	Vec3 up(0,1,0);
	for(Boids::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
		Boid& b = **it;
		boid_drawer.direction = b.velocity.getNormalized();
		boid_drawer.position = b.position;
		cs.makeCoordinateSystem(boid_drawer.direction, up);
		boid_drawer.debugDraw(cs);
		//bb.draw(b.position, b.size * 5, 0.0, 1.0);
	}
	

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	kinect_drawer.debugDraw();
	
	deque<KinectMesh>& meshes = kinect_input.getKinectMeshes();
	if(meshes.size() > 0) {
		kinect_drawer.debugDrawMesh(meshes[0]);
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
}