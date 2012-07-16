#ifndef ROXLU_APPSETTINGSH
#define ROXLU_APPSETTINGSH

#ifdef _WIN32
#define USE_LIGHT_RAYS
#endif

/**
Set the input mode for the input; during development we 
used a "Fake" kinect one. Choose from:

	- K_INPUT_NONE
	- K_INPUT_RECORDING
	- K_INPUT_KINECT
*/
#define KINECT_INPUT_MODE	K_INPUT_KINECT


struct Settings {
	Settings()
	//	:flocking_apply_perlin(true)
	//	,flocking_perlin_influence(36.0f)
	//	,flocking_perlin_scale(0.66f)
		:flocking_center_energy(0.2f)
		,flocking_sphere_size(5.2225f)

	//	,boid_trail_duration_millis(583.33f)
	//	,boid_glow_duration_millis(833.34f)
		,boid_draw_glows(false)
		,boid_glow_size(0.065f)
		,boid_scale(0.085f)
		,boid_trail_width(0.0622)
		,boid_trail_length(6)
		,boid_create_trails(true)
	//	,boid_attack_energy(0.2f)
		,boid_attack_delay_min(150.0f)
		,boid_attack_delay_max(700.0f)
		,boid_attack_duration_min(550.0f)
		,boid_attack_duration_max(4150.0f)
		,boid_shader_normal_mix(0.87f)
		,boid_shader_specular(9.9f)
		,boid_shader_fog_max(8.0f)
		,boid_shader_fog_min(1.0f)
		,boids_percentage_visible(0.8175f)
		,boids_percentage_attackers(1.0f)
		
		,kinect_scale(4.80f)
		,repel_from_user_radius(0.7125f)
		,repel_from_user_energy(0.1550f)
		,attract_to_user_energy(0.1250f)
		,attract_to_user_radius(5.5875f)
		
		,debugdraw_attackers(true)
		,draw_room(true)
		,must_record_kinect(false)
		,draw_axis(false)		
		,draw_flock(false)
		,rotate_scene(false)
	{
	}
		
//	bool flocking_apply_perlin;
//	float flocking_perlin_influence;
//	float flocking_perlin_scale;
	float flocking_center_energy;
	float flocking_sphere_size; // the size of the sphere in which flocking may happen
	
//	float boid_trail_duration_millis;
//	float boid_glow_duration_millis;
	bool boid_draw_glows;
	float boid_glow_size;
	float boid_scale;
	float boid_trail_width;
	int boid_trail_length;
	bool boid_create_trails; 
	//float boid_attack_energy; // when a boid is in attack mode, this is how "responsive" it is 
	float boid_attack_delay_min; // minimum, how long a boid waits before going from "idle" mode to "attack" mode
	float boid_attack_delay_max; // maximum, how long a boid waits before going from "idle" mode to "attack" mode
	float boid_attack_duration_min; // minimum, how long a boid will be in "attack mode"
	float boid_attack_duration_max; // maximum, how long a boid will be in "attack mode"
	float boid_shader_normal_mix; // shader
	float boid_shader_specular; // shader
	float boid_shader_fog_max; 
	float boid_shader_fog_min;
	float boids_percentage_visible;
	float boids_percentage_attackers;
	
	float kinect_scale;
	float repel_from_user_energy;
	float repel_from_user_radius;
	float attract_to_user_energy; // when a boid is further away then "attract_to_user_radius' this energy is used
	float attract_to_user_radius; // when a boid is further away then this radius it will attract to the user
	bool debugdraw_attackers;
	bool draw_room;

	bool must_record_kinect;
	bool draw_axis;
	bool draw_flock;
	bool rotate_scene;
	
};

extern Settings settings;
#endif