//#include "Player.h"
#include <application/Player.h>

Player::Player(Boids2& flockPS) 
	:flock_ps(flockPS)
	,cloth_top_left(NULL)
	,cloth_top_right(NULL)
	,left_hand(NULL)
	,right_hand(NULL)
	,bottom(NULL)
{
}

void Player::setup() {
	// load test input.
	if(!anim.load(File::toDataPath("test_input.txt"))) {
		printf("Cannot load animation data.\n");
		::exit(0);
	}

	if(!anim.hasGroup("hand_left")) {
		printf("Animation file has no hand_left group.\n");
		::exit(0);
	}
	if(!anim.hasGroup("hand_right")) {
		printf("Animation file has no hand_right group.\n");
		::exit(0);
	}
	
	if(!anim.hasGroup("bottom")) {
		printf("Animation file has no bottom group.\n");
		::exit(0);

	}
	anim.setFPS(12);
	anim.play();
	
	// create the boids for interaction.
	left_hand = flock_ps.createParticle(Vec2(0.0f,0.0f));
	left_hand->disable();
	left_hand->size = 10.0f;
	flock_ps.addParticle(left_hand);
	
	right_hand = flock_ps.createParticle(Vec2(0.0f, 0.0f));
	right_hand->disable();
	right_hand->size = 10.0f;
	flock_ps.addParticle(right_hand);
	
	bottom = flock_ps.createParticle(Vec2(0.0f, 0.0f));
	bottom->disable();
	bottom->size = 10.0f;
	flock_ps.addParticle(bottom);

	// create cloth;
	createCloth3(cloth, 10,10,150,700, tris, 0.1);
	cloth_top_left = cloth[0];
	cloth_top_left->disable();
	cloth_top_right = cloth[10];
	cloth_top_right->disable();
	

	boid.setup(); 	// test
}

void Player::update() {
	float uw = 640;
	float uh = 480;
	
	// update player position.
	anim.update();
	anim.getPosition("hand_left", &left_hand->position.x);
	left_hand->position.x *= uw;
	left_hand->position.y *= uw;
	flock_ps.repel<Player>(left_hand, settings.player_repel_radius, settings.player_repel_energy, *this);
	
	
	anim.getPosition("hand_right", &right_hand->position.x);
	right_hand->position.x *= uw;
	right_hand->position.y *= uh;
	flock_ps.repel(right_hand, settings.player_repel_radius, settings.player_repel_energy);
	
	anim.getPosition("bottom", &bottom->position.x);
	bottom->position.x *= uw;
	bottom->position.y *= uh;
	flock_ps.repel(bottom, settings.player_repel_radius, settings.player_repel_energy);
	
	for(int i = 0; i < 20; ++i) {
		flock_ps.repel((Boid2*)cloth[i], settings.player_repel_radius, settings.player_repel_energy);
	}
	
	// update cloth positions
	cloth_top_left->position = left_hand->position;
	cloth_top_right->position = right_hand->position;
	cloth_top_right->position.z = -15 + sin(Timer::millis()*0.001) * 5;
	cloth_top_right->position.x += 150 + sin(Timer::millis()*0.001) * 100;
	cloth_top_left->position.z = -15 + sin(Timer::millis()*0.001) * 6;
	cloth_top_left->position.y -= 450;	
	cloth.addForce(Vec3(0.0f,1.0f, sin(float(Timer::now())*15.0f)));
	cloth.update(0.5);

	boid.position = cloth_top_left->position;
	boid.update(); 	// test
}
