#ifndef ROXLU_APPSETTINGSH
#define ROXLU_APPSETTINGSH

struct Settings {
	Settings()
		:flocking_apply_perlin(true)
		,flocking_perlin_influence(36.0f)
		,flocking_perlin_scale(0.66f)
		,boid_trail_duration_millis(583.33f)
		,boid_glow_duration_millis(833.34f)
		,must_record_kinect(false)

	{
	}
	
	
	bool flocking_apply_perlin;
	float flocking_perlin_influence;
	float flocking_perlin_scale;
	float flocking_center_energy;
	float flocking_sphere_size; // the size of the sphere in which flocking may happen
	
	float boid_trail_duration_millis;
	float boid_glow_duration_millis;
	
	int explosion_trail_length;
	float explosion_random_x_vel;
	float explosion_random_y_vel;
	float explosion_min_lifespan;
	float explosion_max_lifespan;
	float explosion_perlin_influence;
	float explosion_perlin_scale;
	bool must_record_kinect;

};

extern Settings settings;
#endif