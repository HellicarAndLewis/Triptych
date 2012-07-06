#include "Trail.h"
#include "AppSettings.h"

Trail::Trail() 
	:start_index(0)
	,num_nodes(0)
{

}

void Trail::createVertices(VerticesPC& verts, deque<Vec3>& points, const float& agep) {
	if(points.size() < 5) {
		return;
	}
	start_index = verts.size();
	
	Vec4 col(1,1,1,1);
	VertexPC pc;
	
	/* simple strip 
	size_t size = points.size();
	for(int i = 0; i < size; ++i) {
		float p = 1.0f - float(i)/(size-1);
		col.set(p, p*0.5, 1.5-p, p);
		pc.set(points[i], col);
		verts.add(pc);
	}
	*/
	Vec3 up(0,1,0);
	float max_w = app_settings.maxParticleTrailWidth() * agep;	
	size_t size = points.size();
	int add = 0;
	for(int i = 0; i < size-1; ++i) {
		float p = float(i)/(size-1);
		
		Vec3& a = points[i];
		Vec3& b = points[i+1];
		Vec3 dir = b-a;
		Vec3 perp = cross(dir, up);
		perp.normalize();
		
		float w = max_w * p;
		perp *= w;
		Vec3 v0 = a - perp;
		Vec3 v1 = a + perp;
		
		col.set(p, p*0.5, 1.5-p, p);
		
		pc.set(v0, col);
		verts.add(pc);
		
		pc.set(v1, col);
		verts.add(pc);
		add += 2;
	}

	num_nodes = (verts.size() - start_index);
	//printf("ADD: %d, %d\n", add, num_nodes);
	
//	printf("> created: %d - %d\n", start_index, end_index);
	/*
	    float per     = 1.0 - (float)i/(float)(len-1);
      xp            = loc[i].x;
      yp            = loc[i].y;
      zp            = loc[i].z;

      if ( i < len - 2 ){
        Vec3D perp0 = loc[i].sub( loc[i+1] );
        Vec3D perp1 = perp0.cross( new Vec3D( 0, 1, 0 ) ).normalize();
        Vec3D perp2 = perp0.cross( perp1 ).normalize();
              perp1 = perp0.cross( perp2 ).normalize();

        xOff        = perp1.x * radius * agePer * per * .05;
        yOff        = perp1.y * radius * agePer * per * .05;
        zOff        = perp1.z * radius * agePer * per * .05;
        
        gl.glColor4f( per, per*.5, 1.5 - per, per);
	*/
	
}