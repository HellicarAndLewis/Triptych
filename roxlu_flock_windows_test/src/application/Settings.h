#ifndef ROXLU_APPSETTINGSH
#define ROXLU_APPSETTINGSH

struct Settings {
	Settings()
		:flocking_apply_perlin(true)
		,flocking_perlin_influence(36.0f)
		,flocking_perlin_scale(0.66f)
		,flocking_max_speed(13.0f)
		,player_repel_radius(333.33f)
		,player_repel_energy(23000.00f)
		,boid_trail_duration_millis(583.33f)
		,boid_glow_duration_millis(833.34f)

	{
	}
	
	
	bool flocking_apply_perlin;
	float flocking_perlin_influence;
	float flocking_perlin_scale;
	float flocking_max_speed;
	float player_repel_radius;
	float player_repel_energy;
	float boid_trail_duration_millis;
	float boid_glow_duration_millis;
	
	int explosion_trail_length;
	float explosion_random_x_vel;
	float explosion_random_y_vel;
	float explosion_min_lifespan;
	float explosion_max_lifespan;
	float explosion_perlin_influence;
	float explosion_perlin_scale;

};

extern Settings settings;
#endif