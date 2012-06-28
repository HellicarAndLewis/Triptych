#ifndef APP_SETTINGSH
#define APP_SETTINGSH
struct AppSettings {
	AppSettings()
		:use_perlin(false)
		,particle_trail_length(20)
		,min_particle_size(0.3)
		,max_particle_size(0.7)
		,max_particle_trail_width(0.02)
	{
	}

	bool usePerlin();
	bool useParticleTrail();
	float perlinInfluence();
	float perlinScale();
	float maxParticleSize();
	float minParticleSize();
	int particleTrailLength();
	float maxParticleTrailWidth();
	
	bool use_particle_trail;
	bool use_perlin;
	int particle_trail_length;
	
	float perlin_influence;
	float perlin_scale;
	float min_particle_size;
	float max_particle_size;
	float max_particle_trail_width;
};

extern AppSettings app_settings;
	
inline int 		AppSettings::particleTrailLength()	{	return particle_trail_length; 		}
inline bool 	AppSettings::useParticleTrail()		{	return use_particle_trail;			}
inline bool 	AppSettings::usePerlin()			{	return use_perlin;					}
inline float 	AppSettings::perlinScale() 			{	return perlin_scale;				}
inline float 	AppSettings::perlinInfluence() 		{	return perlin_influence;			}
inline float 	AppSettings::minParticleSize() 		{	return min_particle_size;			}
inline float 	AppSettings::maxParticleSize() 		{	return max_particle_size;			}
inline float 	AppSettings::maxParticleTrailWidth(){	return max_particle_trail_width;	}
#endif