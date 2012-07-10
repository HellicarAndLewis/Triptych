#ifndef ROXLU_APPSETTINGSH
#define ROXLU_APPSETTINGSH

struct Settings {
	Settings()
		:flocking_apply_perlin(true)
		,flocking_perlin_influence(36.0f)
		,flocking_perlin_scale(0.66f)
		,boid_trail_duration_millis(583.33f)
		,boid_glow_duration_millis(833.34f)
		,boid_trail_width(0.06)
		,boid_trail_length(50)
		,must_record_kinect(false)
		,boid_draw_glows(false)
		,boid_create_trails(false)
		,draw_axis(false)
		,draw_flock(false)
		,debugdraw_attackers(true)
		,draw_room(true)
		,rotate_scene(false)
		,boid_attack_delay_min(2000.0f)
		,boid_attack_delay_max(15000.0f)
		,boid_attack_duration_min(0.0f)
		,boid_attack_duration_max(15000.0f)
		,boid_glow_size(1.0f)
		,boids_percentage_visible(1.0f)
		,boids_percentage_attackers(1.0f)
	{
	}
		
	bool flocking_apply_perlin;
	float flocking_perlin_influence;
	float flocking_perlin_scale;
	float flocking_center_energy;
	float flocking_sphere_size; // the size of the sphere in which flocking may happen
	
	float boid_trail_duration_millis;
	float boid_glow_duration_millis;
	bool boid_draw_glows;
	float boid_glow_size;
	float boid_scale;
	float boid_trail_width;
	int boid_trail_length;
	bool boid_create_trails;
	float boid_attack_energy;
	float boid_attack_delay_min;
	float boid_attack_delay_max;
	float boid_attack_duration_min;
	float boid_attack_duration_max;
	float boid_shader_normal_mix; // shader
	float boid_shader_specular; // shader
	float boids_percentage_visible;
	float boids_percentage_attackers;
	
	float kinect_scale;
	float attract_to_user_energy;
	float attract_to_user_radius;
	bool debugdraw_attackers;
	bool draw_room;

	bool must_record_kinect;
	bool draw_axis;
	bool draw_flock;
	bool rotate_scene;
	
};

extern Settings settings;
#endif