#ifndef APP_SETTINGSH
#define APP_SETTINGSH
struct AppSettings {
	AppSettings()
		:use_perlin(false)
	{
	}

	bool usePerlin();
	float perlinInfluence();
	float perlinScale();
	float maxParticleSize();
	float minParticleSize();
	
	bool use_perlin;
	float perlin_influence;
	float perlin_scale;
	float min_particle_size;
	float max_particle_size;
};

extern AppSettings app_settings;

inline bool 	AppSettings::usePerlin()			{	return use_perlin;			}
inline float 	AppSettings::perlinScale() 			{	return perlin_scale;		}
inline float 	AppSettings::perlinInfluence() 		{	return perlin_influence;	}
inline float 	AppSettings::minParticleSize() 		{	return min_particle_size;	}
inline float 	AppSettings::maxParticleSize() 		{	return max_particle_size;	}
#endif