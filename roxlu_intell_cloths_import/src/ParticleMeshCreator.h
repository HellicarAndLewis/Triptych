#ifndef ROXLU_PARTICLEMESH_CREATORH
#define ROXLU_PARTICLEMESH_CREATORH

#include "ofMain.h"
#include "Roxlu.h"
#include "PBD.h"
#include "SimpleAnim.h"

/* 
Export animation:
-----------------------
import bpy

# export the animation frame by frame for selected object
ofs = open("/Users/diederickhuijbers/Documents/programming/c++/of/TripTych/apps/diederick/006_intell_2d_cloth_shaper/bin/data/left_hand.anim", "w")
start = 0
stop = 30
bpy.context.scene.frame_current = start
for i in range(start, stop):
    pos = bpy.context.active_object.location
    bpy.context.scene.frame_set(i)
    data = "%f,%f,%f\n" % (pos.y, pos.z, pos.x)
    ofs.write(data)
    
bpy.context.scene.frame_current = start
ofs.close()
-----------------------
*/

using namespace pbd;

class ParticleMeshCreator {
public:
	ParticleMeshCreator(Particles& ps);
	void importModel(const string& filename, const string& group);
	void setup();
	void update();
	void draw(const Mat4& pm, const Mat4& vm);
	void debugDraw();

	void mouseMoved(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	
	int mode;
	SimpleAnim anim;
	Particles& ps;
	Particle* left_hand;
	Particle* right_hand;
	Particle* left_foot;
	Particle* right_foot;
	vector<Vec3> start_positions;
	vector<Vec3> start_forces;
	VerticesPN vertices;
	VertexIndices vertex_indices;
	VertexData vd;
private:
	void setupShader();
	Shader shader;
	GLuint vbo;
	GLuint vao;
	GLuint vbo_indices;
};

#endif