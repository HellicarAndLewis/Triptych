#include "ofMain.h"
#include <application/Visualizer.h>

Visualizer::Visualizer(Boids2& flockPS, Boids2& fxPS, vector<Player*>& players)
	:flock_ps(flockPS)
	,fx_ps(fxPS)
	,players(players)
	,explosion_trails(fxPS)
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

	if(!img.loadImage("twinkle.png")) {
		printf("Cannot load twinkle image.\n");
		::exit(0);
	}
	twinkle_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);

	if(!img.loadImage("mega_glow.png")) {
		printf("Cannot load mega glow texture.\n");
		::exit(0);
	}
	mega_glow_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);

	explosion_trails.setup();	
	
	// create cloaks
	for(int i = 0; i < players.size(); ++i) {
		Cloak* cl = new Cloak(players[i]->cloth, players[i]->tris);
		cl->setup();
		cloaks.push_back(cl);
	}
}

void Visualizer::update() {
	explosion_trails.update();
	
	for(vector<Cloak*>::iterator it = cloaks.begin(); it != cloaks.end(); ++it) {
		Cloak& cl = **it;
		cl.update();
	}
}



void Visualizer::draw(const Mat4& pm, const Mat4& vm, const Mat3& nm) {
	printf("draw!\n");
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	// Boids: dot
	bb.setTexture(glow_tex.getID());
	bb.bind(pm, vm);
	for(Boids2::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
		Boid2& b = **it;
		bb.draw(b.position, b.size * 5, 0.0, 1.0);
	}
	bb.unbind();
	
	// Boids: glow
	bb.setTexture(glow_tex.getID());
	bb.bind(pm, vm);
	for(Boids2::iterator it = flock_ps.begin(); it != flock_ps.end(); ++it) {
		Boid2& b = **it;
		if(b.glow_end > 0) {
			float p = float(b.glow_end - Timer::now()) / settings.boid_glow_duration_millis;
			bb.draw(b.position, b.size*20, 0.0, sin(p*PI));
		}
	}
	bb.unbind();
	
	// Twinkles (explosions)
	bb.setTexture(twinkle_tex.getID());
	bb.bind(pm, vm);
	for(Boids2::iterator it = fx_ps.begin(); it != fx_ps.end(); ++it) {
		Boid2& b = **it;

		bb.draw(b.position, b.size * 5, 0.0, sin(b.agep * PI));
	}
	bb.unbind();
	
	
	// Exploision trails
	explosion_trails.draw(pm, vm);
	
	for(vector<Cloak*>::iterator it = cloaks.begin(); it != cloaks.end(); ++it) {
		Cloak& cl = **it;
		cl.draw(pm, vm, nm);
	}
}

void Visualizer::debugDraw() {
	// test
	players[0]->boid.debugDraw();
	return;

	// FX 
	glColor3f(0,1,1);
	::draw(fx_ps);	
	
	// Flocking
	glColor3f(1,1,1);
	::draw(flock_ps);
	
	for(vector<Boid2*>::iterator it = flock_ps.particles.begin(); it != flock_ps.particles.end(); ++it) {
		Boid2& b = **it;
	
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
	
	// Clocks debug drawing.
	for(vector<Cloak*>::iterator it = cloaks.begin(); it != cloaks.end(); ++it) {
		Cloak& cl = **it;
		cl.debugDraw();
	}

	
		
}