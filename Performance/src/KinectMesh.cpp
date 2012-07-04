/**
 *  KinectMesh.cpp
 *
 *  Created by Marek Bereza on 02/07/2012.
 */

#include "KinectMesh.h"
#include "ofxSimpleGuiToo.h"

int KinectMesh::borderResolution = 20;
int KinectMesh::fillResolution = 40;
ofShader *KinectMesh::shader = NULL;
p2t::CDT *KinectMesh::delaunay = NULL;

void KinectMesh::setupGui() {
	if(shader==NULL) {
		shader = new ofShader();
		shader->load("mesh.vert", "mesh.frag");
	}
	gui.addTitle("Mesh");
	gui.addSlider("Border Res", borderResolution, 5, 50);
	gui.addSlider("Fill Res", fillResolution, 5, 50);
}

bool KinectMesh::triangleTouchesCorner(p2t::Triangle *t) {
	for(int i = 0; i < 3; i++) {
		if(
		   (t->GetPoint(i)->x==0 && t->GetPoint(i)->y==0)
		   ||
		   (t->GetPoint(i)->x==640 && t->GetPoint(i)->y==0)
		   ||
		   (t->GetPoint(i)->x==640 && t->GetPoint(i)->y==480)
		   ||
		   (t->GetPoint(i)->x==0 && t->GetPoint(i)->y==480)
		   ) {
			return true;
		}
	}
	return false;
}
void KinectMesh::setTint(int tint) {
	this->tint = tint;
}
bool KinectMesh::setup(const ofxCvBlob &blob, ofxCvGrayscaleImage &grey, unsigned char *rgb) {

	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	age = 0;
	int step = borderResolution;
	int insideStep = fillResolution;
	
	// make sure nothing crazy happens here.
	if(step<5) step = 5;
	if(insideStep<5) insideStep = 5;
	
	if(blob.pts.size()/step<3) return false;
	for(int i = 0; i < blob.pts.size(); i+= step) {
		outline.push_back(ofVec2f(blob.pts[i].x, blob.pts[i].y));
	}
	ofRectangle r = blob.boundingRect;
	int ryOffset = 0;
	for(int i = 0; i < r.x + r.width; i+=insideStep) {
		if(ryOffset==0) ryOffset = insideStep/2;
		else ryOffset = 0;
		
		for(int j = 0; j < r.y + r.height; j+= insideStep) {
			//				ofVec2f jitter = ofVec2f(1, 0);//ofRandom(-insideStep/4, insideStep/4), ofRandom(-insideStep/4, insideStep/4));
			
			ofVec2f jitter(ofNoise(i*ofGetElapsedTimef()*0.006, j*ofGetElapsedTimef()*0.006, ofGetElapsedTimef()*0.01)*20, ofSignedNoise(ofGetElapsedTimef()*0.0007*i, ofGetElapsedTimef()*0.0007*j)*20.f);
			//				jitter.rotate(ofSignedNoise(ofGetElapsedTimef()/1000.f)*180);
			ofVec2f p = ofVec2f(i, j+ryOffset) + jitter;
			
			
			if(tricks::math::pointInsidePolygon(p, outline)) {
				insides.push_back(p);
			}
		}
	}
	
	
	// triangulate!!
	//ofxDelaunay delaunay;
	if(delaunay!=NULL) {
		delete delaunay;
	}
		vector<p2t::Point*> bounding;
		bounding.push_back(new p2t::Point(0,0));
		bounding.push_back(new p2t::Point(640,0));
		bounding.push_back(new p2t::Point(640,480));
		bounding.push_back(new p2t::Point(0,480));
		
		delaunay = new p2t::CDT(bounding);
	//}
	//delaunay->reset();
	
	for(int i = 0; i < outline.size(); i++) {
//		delaunay->addPoint(outline[i].x, outline[i].y);
		delaunay->AddPoint(new p2t::Point(outline[i].x, outline[i].y));
	}
	for(int i = 0; i < insides.size(); i++) {
		
//		delaunay->addPoint(insides[i].x, insides[i].y);
		delaunay->AddPoint(new p2t::Point(insides[i].x, insides[i].y));
	}
	
	
	delaunay->Triangulate();
	
	//XYZ *points = delaunay->getPoints();
	//ITRIANGLE *tris = delaunay->getTriangles();
	vector<p2t::Triangle*> tris = delaunay->GetTriangles();
	
	
	int numTris = tris.size();
	
	unsigned char *depth = grey.getPixels();
	for(int i = 0; i < numTris; i++) {
		
		if(triangleTouchesCorner(tris[i])) {
			continue;
		}
		triangles.push_back(Triangle());
		triangles.back().set(tris[i]);
		ofVec2f c = triangles.back().centre;
		int pos = ((int)c.x)+ ((int)c.y)*grey.getWidth();
		if(depth[pos]>0) {
			triangles.back().hollow = false;
			ofColor color(rgb[pos*3], rgb[pos*3+1], rgb[pos*3+2]);
			triangles.back().color.set(color.r/255.f, color.g/255.f, color.b/255.f);
		} else {
			triangles.back().hollow = true;
		}
	}
	
	for(int i = 0; i < triangles.size(); i++) {
		if(!triangles[i].hollow) {
			addTriangle(triangles[i]);
		}
	}
	return true;
}



void KinectMesh::draw() {
	age++;
	
	/*if(tint>0) {
		glMatrixMode(GL_COLOR_MATRIX);

		if(tint%3==1) {
			glScalef(1,0,0);
		
		} else if(tint%3==2) {
			glScalef(0, 1, 0);
		} else if(tint%3==0) {
			glScalef(0, 0, 1);
		}
		glMatrixMode(GL_MODELVIEW);
	}
	*/
	
	shader->setUniform1i("tint", tint);
	
	mesh.draw();

	/*if(tint>0) {
		glMatrixMode(GL_COLOR_MATRIX);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
	
	}*/
	
	/*
	float alpha = 1;//ofMap(age, 0, 20, 1, 0, true);
	for(int i = 0; i < triangles.size(); i++) {
		
		if(triangles[i].hollow) {
			glColor4f(1, 1, 1, 1);
			glBegin(GL_LINE_LOOP);
			glVertex2f(triangles[i].points[0].x, triangles[i].points[0].y);
			glVertex2f(triangles[i].points[1].x, triangles[i].points[1].y);
			glVertex2f(triangles[i].points[2].x, triangles[i].points[2].y);
			glEnd();
		} else {
			float bri = (triangles[i].color.r+triangles[i].color.g+triangles[i].color.b)/3.f;
			
			glColor4f(0.5*bri,0.5*bri, bri, alpha);
			glBegin(GL_TRIANGLES);
			glVertex2f(triangles[i].points[0].x, triangles[i].points[0].y);
			glVertex2f(triangles[i].points[1].x, triangles[i].points[1].y);
			glVertex2f(triangles[i].points[2].x, triangles[i].points[2].y);
			glEnd();
		}
	}
	 */
}




void KinectMesh::addTriangle(const Triangle &tri) {
	addTriangle(tri.points[0], tri.points[1], tri.points[2], tri.color);
}

void KinectMesh::addTriangle(ofVec3f a, ofVec3f b, ofVec3f c, ofFloatColor color) {
	mesh.addColor(color);
	mesh.addVertex(a);
	mesh.addColor(color);
	mesh.addVertex(b);
	mesh.addColor(color);
	mesh.addVertex(c);
}


