#include "ParticleMesh.h"

ParticleMesh::ParticleMesh(Particles& ps)
	:ps(ps)
	,use_indices(true)
{
}

void ParticleMesh::setup() {
	ofRegisterMouseEvents(this);

	// load the mesh we use to deform.
	importModel("grid.rox", "Grid");
//	importModel("grid.rox", "mesh");
	setupShader();
	
	// load animations
	//anim.load(File::toDataPath("hand.anim"));
	if(anim.load(File::toDataPath("marek3.txt"))) {
		anim.setFPS(30);
		anim.play();
	}
}


void ParticleMesh::importModel(const string& filename,  const string& group) {
	
	
	// load ROX file.
	ROX obj;
	obj.import(File::toDataPath(filename));
	obj.print();
	if(!obj.contains(group)) {
		printf("Cannot find: %s\n", group.c_str());
		exit(0);
		return;
	}
	
	// create particles for loaded vertices.
	VertexPN pn;
	vd = obj[group];
	Vec3 center;
	for(int i = 0; i < vd.getNumVertices(); ++i) {
		ps.addParticle(ps.createParticle(vd.getVertex(i)));
		start_positions.push_back(vd.getVertex(i));
		start_forces.push_back(vd.getVertex(i).normalize() * 0.5);
		
		if(use_indices) {
			pn.set(vd.getVertex(i), vd.getNormal(i));
			vertices.add(pn);
		}
	}	
	
	
	for(int i = 0; i < vd.getNumTriangles(); ++i) {
		Triangle& tr = vd.triangles[i];
		ps.addSpring(ps.createSpring(ps[tr.va], ps[tr.vb]));
		ps.addSpring(ps.createSpring(ps[tr.vb], ps[tr.vc]));
		ps.addSpring(ps.createSpring(ps[tr.vc], ps[tr.va]));
		
		if(use_indices) {
			vertex_indices.add(tr.va);
			vertex_indices.add(tr.vb);
			vertex_indices.add(tr.vc);
		}
		else {
			// no indices.
			pn.set(vd.getVertex(tr.va), vd.getNormal(tr.va));
			vertices.add(pn);
			
			pn.set(vd.getVertex(tr.vb), vd.getNormal(tr.vb));
			vertices.add(pn);
			
			pn.set(vd.getVertex(tr.vc), vd.getNormal(tr.vc));
			vertices.add(pn);
		}	
	}
	
	
	vector<int>vert_group;
	if(vd.getVertexGroup("hand.R", vert_group)) {
			Vec3 pos = ps[vert_group[0]]->position;
			pos.z += 0.5f;
			right_hand = ps.addParticle(ps.createParticle(pos));
			right_hand->disable();
			for(int i = 0; i < vert_group.size(); ++i) {
					ps.addSpring(ps.createSpring(right_hand, ps[vert_group[i]]));
			}
	}
	if(vd.getVertexGroup("hand.L", vert_group)) {
		Vec3 pos = ps[vert_group[0]]->position;
		pos.z += 0.5f;
		left_hand = ps.addParticle(ps.createParticle(pos));
		left_hand->disable();
		
		for(int i = 0; i < vert_group.size(); ++i) {
			ps.addSpring(ps.createSpring(left_hand, ps[vert_group[i]]));
		}
	}
	if(vd.getVertexGroup("foot.L", vert_group)) {
		Vec3 pos = ps[vert_group[0]]->position;
		pos.z += 0.5f;
		left_foot = ps.addParticle(ps.createParticle(pos));
		left_foot->disable();
		
		for(int i = 0; i < vert_group.size(); ++i) {
			ps.addSpring(ps.createSpring(left_foot, ps[vert_group[i]]));
		}
	}
	if(vd.getVertexGroup("foot.R", vert_group)) {
		Vec3 pos = ps[vert_group[0]]->position;
		pos.z += 0.5f;
		right_foot = ps.addParticle(ps.createParticle(pos));
		right_foot->disable();
		
		for(int i = 0; i < vert_group.size(); ++i) {
			ps.addSpring(ps.createSpring(right_foot, ps[vert_group[i]]));
		}
	}
	if(vd.getVertexGroup("top_row", vert_group)) {
		for(int i = 0; i < vert_group.size(); ++i) {
		//	ps[vert_group[i]]->disable();
		}
	}

	if(left_hand == NULL || right_hand == NULL || left_foot == NULL || right_foot == NULL) {
		printf("Cannot load the mesh file correctly.\n");
		exit(0);
	}
	else {
		printf("loaded!\n");
	}
}

void ParticleMesh::update() {
	//ps.addForce(0,-0.1,0.3);
	ps.update(0.2);
	
	anim.update();
	
	anim.getPosition("hand_left", &left_hand->position.x);
	anim.getPosition("hand_right", &right_hand->position.x);
//	anim.getPosition("right_hand", &right_hand->position.x);
	anim.getPosition("bottom", &left_foot->position.x);
//	anim.getPosition("left_foot", &left_foot->position.x);
//	anim.getPosition("right_foot", &right_foot->position.x);
	left_hand->position *= 8;
	right_hand->position *= 8;
	// update vertices positions.
	for(int i = 0; i < ps.size(); ++i) {
		Particle& p = *ps[i];
		vertices[i].setPos(p.position);
	}
	
	// update normals
	for(int i = 0; i < vd.getNumTriangles(); ++i) {
		Triangle& tr = vd.triangles[i];
		Vec3& a = ps[tr.va]->position;
		Vec3& b = ps[tr.vb]->position;
		Vec3& c = ps[tr.vc]->position;
		Vec3 ab = b-a;
		Vec3 ac = c-a;
		Vec3 n = cross(ab,ac).normalize();
		vertices[tr.va].setNorm(n);
		vertices[tr.vb].setNorm(n);
		vertices[tr.vc].setNorm(n);
	}
	if(!use_indices) {
		for(int i = 0; i < vd.getNumTriangles(); ++i) {
			Triangle& tri = vd.triangles[i];
		
			Vec3& a = ps[tri.va]->position;
			Vec3& b = ps[tri.vb]->position;
			Vec3& c = ps[tri.vc]->position;
			
			vertices[tri.va].setPos(a);
			vertices[tri.vb].setPos(b);
			vertices[tri.vc].setPos(c);
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.numBytes(), vertices.getPtr());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleMesh::draw(const Mat4& pm, const Mat4& vm) {
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glBindVertexArrayAPPLE(vao);
	shader.enable();
	shader.uniformMat4fv("u_modelview_matrix", vm.getPtr());
	shader.uniformMat4fv("u_projection_matrix", pm.getPtr());
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader.uniform1i("u_mode", 0);
	drawTriangles();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shader.uniform1i("u_mode", 1);
	drawTriangles();
	//glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, (void*)0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	shader.disable();
	glBindVertexArrayAPPLE(0);
}

void ParticleMesh::debugDraw() {
	glColor3f(1,1,1);
	ps.draw();
	
	
	//left_hand->setPosition(p);
	
	glColor3f(0.3,0.9,0.0);
//	glBegin(GL_POINTS);
		//for(vector<Vec3>::iterator it = start_positions.begin(); it != start_positions.end(); ++it) {
		/*
		for(int i = 0; i < start_positions.size(); ++i) {
			Particle& p = *ps[i];
			Vec3& curr = p.position;
			Vec3& start = start_positions[i];
			Vec3 dir = start - curr;
			float ls = dir.lengthSquared();
			if(ls > 0.01) {
				dir.normalize();
				dir *= 1;
				p.addForce(dir);
			}
			//glVertex3fv(start.getPtr());
		}
		*/
//	glEnd();
//	printf("%f, %f, %f\n", p[0], p[1], p[2]);
	 for(int i = 0; i < start_forces.size(); ++i) {
			Particle& p = *ps[i];
			//Vec3& curr = p.position;
			Vec3& f = start_forces[i];
			//p.addForce(f);
			
			//glVertex3fv(start.getPtr());
		}

}


void ParticleMesh::mouseMoved(ofMouseEventArgs& args) {
}

void ParticleMesh::mouseDragged(ofMouseEventArgs& args) {
}

void ParticleMesh::mousePressed(ofMouseEventArgs& args) {

	switch(mode) {
		case 0: {
			ps.addParticle(ps.createParticle(args.x, args.y));
			break;
		}
		default:break;
	}
}

void ParticleMesh::mouseReleased(ofMouseEventArgs& args) {
}

// setup the shader
void ParticleMesh::setupShader() {
	glGenVertexArraysAPPLE(1, &vao); 
	glBindVertexArrayAPPLE(vao);
	
	if(use_indices) {
		glGenBuffers(1, &vbo_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indices.numBytes(), vertex_indices.getPtr(), GL_DYNAMIC_DRAW);
	}
	
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
	shader.load("interactive_mesh");
	shader.a("a_pos", 0).a("a_norm",1);
	shader.link();
	shader.u("u_modelview_matrix").u("u_projection_matrix").u("u_mode");
	
	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // norm
	
	glBufferData(GL_ARRAY_BUFFER, vertices.numBytes(), vertices.getPtr(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid*)offsetof(VertexPN, pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid*)offsetof(VertexPN, norm));
	
	glBindVertexArrayAPPLE(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}