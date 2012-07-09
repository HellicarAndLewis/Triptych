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
		,must_record_kinect(false)
		,boid_draw_glows(false)
		,boid_create_trails(false)
		,draw_axis(false)
		,draw_flock(false)
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
	float boid_scale;
	float boid_trail_width;
	bool boid_create_trails;
	
	int explosion_trail_length;
	float explosion_random_x_vel;
	float explosion_random_y_vel;
	float explosion_min_lifespan;
	float explosion_max_lifespan;
	float explosion_perlin_influence;
	float explosion_perlin_scale;
	
	float kinect_scale;
	float attract_to_user_energy;
	float attract_to_user_radius;

	bool must_record_kinect;
	bool draw_axis;
	bool draw_flock;
	
};

extern Settings settings;
#endif