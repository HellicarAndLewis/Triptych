#include "BoidDrawer.h"
#include <string>
using namespace std;


BoidDrawer::BoidDrawer() 
	:size(1.0)
{
}

void BoidDrawer::setup() {
	// Load boid data from obj file.
	OBJ obj;
	if(!obj.import(File::toDataPath("boid.obj"))) {
		ofSystemAlertDialog("Cannot load boid obj file.");
		::exit(0);
	}
	obj.print();
	
	vd = obj["boid"];
	if(vd.getNumVertices() == 0) {
		ofSystemAlertDialog("No vertices found in obj file.");
		::exit(0);
	}
	if(vd.getNumNormals() == 0) {
		ofSystemAlertDialog("No normals found. Make sure to export with normals.");
		::exit(0);
	}
	if(vd.getNumTexCoords() == 0) {
		ofSystemAlertDialog("No texture coordinates found for boid!\n");
		::exit(0);
	}
	
	printf("Num vertices in boid: %zu\n", vd.getNumVertices());	
	
	
	for(int i = 0; i < vd.getNumTriangles(); ++i) {
		Triangle& tri = vd.triangles[i];
		Vec3& va = vd[tri.va];
		Vec3& vb = vd[tri.vb];
		Vec3& vc = vd[tri.vc];
		Vec3 ab = vb - va;
		Vec3 ac = vc - va;
//		Vec3 norm = cross(ab, ac);
//		norm.normalize();
		
		Vec2& ta = vd.texcoords[tri.tc_a];
		Vec2& tb = vd.texcoords[tri.tc_b];
		Vec2& tc = vd.texcoords[tri.tc_c];
		/*
		ta.y = 1.0-ta.y;
		tb.y = 1.0-tb.y;
		tc.y = 1.0-tc.y;
		*/
		vertices.add(VertexPTN(va, ta, vd.normals[tri.na]));		
		vertices.add(VertexPTN(vb, tb, vd.normals[tri.nb]));		
		vertices.add(VertexPTN(vc, tc, vd.normals[tri.nc]));		
//		vertices.add(VertexPTN(va, ta, norm));		
//		vertices.add(VertexPTN(vb, tb, norm));		
//		vertices.add(VertexPTN(vc, tc, norm));		

		/*
		vertices.add(VertexPN(vd[tri.va], vd.getNormal(tri.va)));		
		vertices.add(VertexPN(vd[tri.vb], vd.getNormal(tri.vb)));
		vertices.add(VertexPN(vd[tri.vc], vd.getNormal(tri.vc)));
		*/
		
	}
	printf("Boid drawer..\n");
	shader.load("boid_drawer");
	shader.a("a_pos", 0).a("a_tex", 1).a("a_norm", 2);
			
	shader.link();

	shader	.u("u_view_matrix")
			.u("u_model_matrix")
			.u("u_projection_matrix")
			.u("u_orientation_matrix")
			.u("u_normal_matrix")
			.u("u_texture");
	
	vao.create();
	vao.bind();
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.numBytes(), vertices.getPtr(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // tex
	glEnableVertexAttribArray(2); // norm
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), (GLvoid*)offsetof(VertexPTN, pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), (GLvoid*)offsetof(VertexPTN, tex));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), (GLvoid*)offsetof(VertexPTN, norm));
	vao.unbind();
	shader.disable();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// load texture.
	glEnable(GL_TEXTURE_2D);
	ofImage img;
	if(!img.loadImage("boid_diffuse.png")) {
		printf("Cannot load boid_diffuse.png texture");
		::exit(0);
	}
	tex.setPixels(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);

}

//void BoidDrawer::draw(const float* pm, const float* vm, const float* nm) {
void BoidDrawer::draw(const float* pm, const float* vm, const float* nm, const Mat3& cs) {
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	
	// @todo 
	Mat4 m1 = Mat4::rotation(cs);
	Mat4 m2 = Mat4::translation(position);
	Mat4 model = m2 * m1;
	model.scale(size);
	
	vao.bind();
		shader.enable();
		
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		tex.bind();
		shader.uniform1i("u_texture", 2);
		
		shader.uniformMat4fv("u_view_matrix", vm);
		shader.uniformMat4fv("u_model_matrix", model.getPtr());
		shader.uniformMat4fv("u_projection_matrix", pm);
		shader.uniformMat3fv("u_normal_matrix", nm);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
		shader.disable();
	vao.unbind();
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void BoidDrawer::debugDraw(const Mat3& cs) {
	float s = 0.3f;
	Vec3 ax;
	glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3fv(position.getPtr());  ax = cs.getXAxis(); glVertex3fv((position+ax*s).getPtr());
		glColor3f(0,1,0); glVertex3fv(position.getPtr());  ax = cs.getYAxis(); glVertex3fv((position+ax*s).getPtr());
		glColor3f(0,0,1); glVertex3fv(position.getPtr());  ax = cs.getZAxis(); glVertex3fv((position+ax*s).getPtr());
	glEnd();

	//glDisable(GL_CULL_FACE);
	
	return;
	
	
	glEnable(GL_DEPTH_TEST);
	glColor3f(1,1,0);
	glPointSize(1.0);
	glPushMatrix();
		static int t = 0;
		++t;
		glRotatef(t, 0,1,0.5);
		vd.debugDraw();

		glBegin(GL_TRIANGLES);
		for(int i =0; i < vd.getNumVertices(); ++i) {
			glVertex3fv(vd[i].getPtr());
		}
		glEnd();
		glColor3f(1,1,1);
	glPopMatrix();
}

void BoidDrawer::update() {
}
