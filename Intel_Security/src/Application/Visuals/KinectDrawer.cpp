#include "KinectDrawer.h"

KinectDrawer::KinectDrawer() 
	:allocated_bytes(0)
	,user_detected(false)
{

}

void KinectDrawer::setup() {
	vao.create();
	vao.bind();
	glGenBuffers(1, &vbo);

	shader.load("kinect_drawer");
	shader.a("a_pos",0).a("a_col",1);
	shader.link();
	shader.u("u_modelview_matrix").u("u_projection_matrix").u("u_model");
	
	vao.unbind();	
}

void KinectDrawer::update(KinectMesh& kmesh) {
	// calc necessary bytes for buffer.
	size_t necessary = kmesh.vertices.size() * sizeof(KinectVertex); // unwelded triangles
	num_vertices = kmesh.vertices.size();

	if(num_vertices <= 0) {
		return;
	}
	// just tmp, no more vertices
	if(num_vertices > 1024*1024) {
		printf("To much vertices found.\n");
		return; 
	}
	
	// recreate buffer.
	if(necessary > allocated_bytes) {
		size_t to_allocate = 0;
		while(to_allocate < necessary) {
			to_allocate = std::max<size_t>(allocated_bytes * 2, 8192);
			allocated_bytes = to_allocate;
		}
	
		vao.bind();
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, to_allocate, NULL, GL_DYNAMIC_DRAW); eglGetError();
			glEnableVertexAttribArray(0); 
			glEnableVertexAttribArray(1); 
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(KinectVertex), (GLvoid*)offsetof(KinectVertex, pos));
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(KinectVertex), (GLvoid*)offsetof(KinectVertex, col));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		vao.unbind();
	}

	if(allocated_bytes < necessary) {
		printf("Not enough allocated...\n");
		return;
	}

	// update buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, necessary, &kmesh.vertices[0].pos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void KinectDrawer::draw(const float* pm, const float* vm) {
	if(!user_detected) {
		return;
	}
	
	if(num_vertices <= 50 || num_vertices > 1024*1024) { // 50 just arbitrary...
		return;
	}
	
	glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	
	shader.enable();
	roxlu::Mat4 model = roxlu::Mat4::scaling(settings.kinect_scale, settings.kinect_scale, settings.kinect_scale);
	
	shader.uniformMat4fv("u_projection_matrix", pm);
	shader.uniformMat4fv("u_modelview_matrix", vm);
	shader.uniformMat4fv("u_model", model.getPtr());

	vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	vao.unbind();	
	
	shader.disable();
	
	glEnable(GL_TEXTURE_2D);
}


void KinectDrawer::debugDraw() {
}

void KinectDrawer::debugDrawMesh(KinectMesh& m) {
	glBegin(GL_TRIANGLES);
	for(vector<KinectTriangle>::iterator it = m.triangles.begin(); it != m.triangles.end(); ++it) {
		KinectTriangle& tri = *it;
		if(tri.hollow) {
			continue;
		}
		
		glColor3fv(&tri.colors[0].r);;
		glVertex2fv(tri.points[0].getPtr());
		glVertex2fv(tri.points[1].getPtr());
		glVertex2fv(tri.points[2].getPtr());
		//printf("%f, f\n", tri.points[0][0], tri.points[0][1
	}
	glEnd();
	
	// outline
	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
		for(int i = 0; i < m.outline.size(); ++i) {	
			ofVec2f v = m.outline[i];
			v *= 0.01;
			glVertex2f(v.x, -v.y);
		}
	glEnd();
}
