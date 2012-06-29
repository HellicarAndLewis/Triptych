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
	if(pm.left_hand != NULL && pm.right_hand != NULL) {
		su.setup(pm.left_hand, pm.left_hand, pm.right_hand);
		su.addParticles(num_particles);
	}
}


void SphereSystem::update() {
	su.update();
}

void SphereSystem::draw(const Mat4& pm, const Mat4& vm, const Vec3& right, const Vec3& up) {
	su.draw(pm, vm, right, up);
}

void SphereSystem::debugDraw() {
	if(app_settings.debug_draw_spheres) {
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
	}
	su.debugDraw();
}

void SphereSystem::removeTrails() {
	su.removeTrails();
}
void SphereSystem::resetParticleSizes() {
	su.resetParticleSizes();
}

