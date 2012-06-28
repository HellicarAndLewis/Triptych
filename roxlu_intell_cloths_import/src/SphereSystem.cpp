#include "SphereSystem.h"

SphereSystem::SphereSystem(ParticleMesh& pm)
	:pm(pm)
	,su(particle_bb)
{
}

SphereSystem::~SphereSystem() {
}

void SphereSystem::setup() {
	// load particle texture
	ofImage particle_img;
	if(!particle_img.loadImage("particle_glow.png")) {
		printf("Cannot load particle image.\n");
		::exit(0);
	}
	particle_tex.setPixels(particle_img.getPixels(), particle_img.getWidth(), particle_img.getHeight(), GL_RGBA);
	particle_bb.setTexture(particle_tex.getID());
	printf("id: %d\n", particle_tex.getID());
	
	// load the sphere vertices
	OBJ sphere_loader;
	sphere_loader.import(File::toDataPath("sphere.obj"));
	sphere_vd = sphere_loader["sphere"];
	
	// create some test particles
	
	int num_particles = 200;
	if(pm.left_hand != NULL) {
		/*
		left_hand_ps.addParticle(pm.left_hand);
		
		Vec3 pos;
		Vec3 lhpos = pm.left_hand->position;
		for(int i = 0; i < num_particles; ++i) {
			pos = lhpos + randomVec3() * 2;
			Particle* p = left_hand_ps.createParticle(pos);
			left_hand_ps.addParticle(p);
			left_hand_ps.addSpring(left_hand_ps.createSpring(p, pm.left_hand));
		}
		*/
		su.setup(pm.left_hand);
		su.addParticles(num_particles);
	}
	
	
}


void SphereSystem::update() {
	su.update();
	/*	
	float s = ofGetElapsedTimef();
	float nf = s * 0.01;
	float x = noise1(s*nf);
	
	float f = 10;
	left_hand_ps.addForce(x*f,0,0);
	left_hand_ps.repel(0.02);
	*/
	
	// make sure the particles are all connected to the bounding sphere
	/*
	float radius = 1.5f;
	if(pm.left_hand != NULL) {
		Vec3& center = pm.left_hand->position;
		for(vector<Particle*>::iterator it = left_hand_ps.particles.begin(); it != left_hand_ps.particles.end(); ++it) {
			Particle& p = *(*it);
			Vec3& pos = p.position;
			Vec3 dir = center - pos;
			float ls = dir.lengthSquared();
			if(ls > 0.01){ 
				float l = radius - dir.length();
				dir.normalize();
				dir *= radius;
				// p.position = center + dir;
				p.addForce(dir);
			}		
		}
	}
	*/
	//left_hand_ps.update(0.03);
	
	//s = 0.3 + 0.3 * (1.0 + sin(s*1.4));

	//ChangeSpringStrength springs(s);
	//for_each(left_hand_ps.springs.begin(), left_hand_ps.springs.end(), springs);
	
	
}

void SphereSystem::draw(const Mat4& pm, const Mat4& vm, const Vec3& right, const Vec3& up) {
	su.draw(pm, vm, right, up);
}

void SphereSystem::debugDraw() {
	if(pm.left_hand != NULL) {
		Vec3& pos = pm.left_hand->position;
		glColor3f(1,1,1);
		glPushMatrix();
			glTranslatef(pos.x, pos.y, pos.z);
			sphere_vd.debugDraw();
		glPopMatrix();
	}
	if(pm.right_hand != NULL) {
		Vec3& pos = pm.right_hand->position;
		glColor3f(1,1,1);
		glPushMatrix();
			glTranslatef(pos.x, pos.y, pos.z);
			sphere_vd.debugDraw();
		glPopMatrix();
	}
	su.debugDraw();
//	left_hand_ps.draw();
}

void SphereSystem::removeTrails() {
	su.removeTrails();
}
void SphereSystem::resetParticleSizes() {
	su.resetParticleSizes();
}

