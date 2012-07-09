#include "ofMain.h"
#include "Visualizer.h"

Visualizer::Visualizer(Boids& flockPS, Boids& fxPS, KinectInput& kinect)
	:flock_ps(flockPS)
	,fx_ps(fxPS)
	,trails_drawer(flockPS)
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
	if(!img.loadImage("nebula.png")) {
		printf("Cannot load mega glow texture.\n");
		::exit(0);
	}
	mega_glow_tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGB);

	
	trails_drawer.setup();	
	
	kinect_drawer.setup();
	
	boid_drawer.setup();
}

void Visualizer::update() {
	trails_drawer.update();
		
	if(kinect_input.update()) {
		deque<KinectMesh>& meshes = kinect_input.getKinectMeshes();
		kinect_drawer.update(meshes[0]);
	}
}



// pm = projection matrix, vm = view matrix, nm = normal matrix
void Visualizer::draw(const float* pm, const float* vm, const float* nm, const float* rightVec, const float* upVec) {
			
	// z-sort (not necessary, depth buffer takes care of this)
	//Boids::iterator boids_behind = std::partition(flock_ps.begin(), flock_ps.end(), BoidPartitioner());
	


	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// BEHIND KINECT
	if(settings.boid_draw_glows) {
		drawGlows(flock_ps.begin(), boids_behind, pm, vm, nm, rightVec, upVec);
	}
	
	// KINECT
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	kinect_drawer.draw(pm, vm);
	if(settings.draw_flock) {
		drawBoids(flock_ps.begin(), flock_ps.end(), pm, vm, nm, rightVec, upVec);
	}
	
	glDepthMask(GL_FALSE);
	if(settings.boid_draw_glows) {
		drawGlows(flock_ps.begin(), flock_ps.end(), pm, vm, nm, rightVec, upVec);
	}
	trails_drawer.draw(pm, vm);
	glDepthMask(GL_TRUE);

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
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	bb.setTexture(glow_tex.getID());
	bb.bind(pm, vm, rightVec, upVec);
	for(Boids::iterator it = begin; it != end; ++it) {
		Boid& b = **it;
		bb.draw(b.position, b.size * 0.12, 0.0, 1.0);
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
		boid_drawer.direction = b.velocity.getNormalized();
		boid_drawer.position = b.position;
		
		boid_drawer.size = b.size  * settings.boid_scale;
		cs.makeCoordinateSystem(boid_drawer.direction, up);
		boid_drawer.draw(pm, vm, nm, cs);
	}
}


void Visualizer::debugDraw() {

	// FX 
	glColor3f(0,1,1);
	::draw(fx_ps);	
	
	// Flocking
	if(settings.draw_flock) {
		glColor3f(1,1,1);
		::draw(flock_ps);
	}
	
	
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
	

	kinect_drawer.debugDraw();
	
	deque<KinectMesh>& meshes = kinect_input.getKinectMeshes();
	if(meshes.size() > 0) {
		kinect_drawer.debugDrawMesh(meshes[0]);
	}


	// draw trails
	trails_drawer.debugDraw();
	
	
	glColor3f(1,1,1);
	glLineWidth(0.1);
	glBegin(GL_LINES);
	for(Boids::iterator ita = flock_ps.begin(); ita != flock_ps.end();) {
		Boid& a = **ita;
		for(Boids::iterator itb = ita++; itb != flock_ps.end(); ++itb) {
			Boid& b = **itb;
			float sq = (b.position - a.position).lengthSquared();
			if(sq < 1.0f) {
				glVertex3fv(a.position.getPtr());
				glVertex3fv(b.position.getPtr());
			}
		}
		ita++;
	}	
	
	glEnd();
	
	
	/*		
		test_trail.push_back(connection_boid->position);
		
		
		// test: generate a triangle strip, with vertices+color and use a custom color generator.
		VerticesPC trail_verts;
		DebugTrail col;
		TrailTriangleVerticesPC<DebugTrail> vert_gen(col);
		TrailPercentageWidth wf;
		
		test_trail.createTriangleStrip(0.1f, trail_verts, wf, vert_gen);
		glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0; i < trail_verts.size(); ++i) {
				glColor4fv(trail_verts[i].col.getPtr());
				glVertex3fv(trail_verts[i].pos.getPtr());
			}
		glEnd();
		test_trail.limitLength(50);
		
		
		glPointSize(10.0f);
		glColor3f(1,1,0);
		glBegin(GL_POINTS);
			glVertex3fv(connection_boid->position.getPtr());
		glEnd();
		
	}
	*/	
}