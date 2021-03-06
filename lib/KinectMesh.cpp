  /**
 *  KinectMesh.cpp
 *
 *  Created by Marek Bereza on 02/07/2012.
 */

#include "KinectMesh.h"
#include "ofxSimpleGuiToo.h"

int KinectMesh::borderResolution = 20;
int KinectMesh::fillResolution = 40;
p2t::CDT *KinectMesh::delaunay = NULL;
bool KinectMesh::perVertexColour = false;
float KinectMesh::edgeColourSharpening = 0.5;

void KinectMesh::setupGui() {
	
	gui.addTitle("Mesh");
	gui.addSlider("Border Res", borderResolution, 5, 50);
	gui.addSlider("Fill Res", fillResolution, 5, 50);
	gui.addToggle("Per Vertex Colour", perVertexColour);
	gui.addSlider("Edge Colour Sharpening", edgeColourSharpening, 0, 1);
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

bool KinectMesh::setup(const ofxCvBlob &blob, KinectThresholder &thresholder) {
	ofxCvGrayscaleImage &grey = thresholder.getOutline(); 
	unsigned char *rgb = thresholder.getPixels();

	float t = ofGetElapsedTimef();
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);

	depth = 0;//thresholder.getDepth(blob);
	int step = borderResolution;
	int insideStep = fillResolution;
	
	// make sure nothing crazy happens here.
	step = std::max<int>(step, 5);
	insideStep = std::max<int>(insideStep, 5);
	
	
	if(blob.pts.size()/step<3) {
		return false;
	}
	
	for(int i = 0; i < blob.pts.size(); i+= step) {
		outline.push_back(ofVec2f(blob.pts[i].x, blob.pts[i].y));
	}
	
	
	ofRectangle r = blob.boundingRect;
	int ryOffset = 0;
	for(int i = 0; i < r.x + r.width; i+=insideStep) {
		
		if(ryOffset==0) {
			ryOffset = insideStep/2;
		}
		else {
			ryOffset = 0;
		}
		
		for(int j = 0; j < r.y + r.height; j+= insideStep) {
			ofVec2f jitter(
				 ofNoise(i*t*0.006,j*t*0.006,t*0.01)*20
				,ofSignedNoise(t*0.0007*i, t*0.0007*j)*20.f
			);
				
			ofVec2f p = ofVec2f(i, j+ryOffset) + jitter;
			
			if(tricks::math::pointInsidePolygon(p, outline)) {
				insides.push_back(p);
			}
		}
	}
	
	// triangulate!!
	if(delaunay!=NULL) {
		delete delaunay;
	}
	
	vector<p2t::Point*> b;
	b.reserve(4+outline.size()+insides.size()+4);
	b.push_back(new p2t::Point(0,0));
	b.push_back(new p2t::Point(640,0));
	b.push_back(new p2t::Point(640,480));
	b.push_back(new p2t::Point(0,480));
	
	video_w = 640;
	video_h = 480;
		
	delaunay = new p2t::CDT(b);
	
	for(int i = 0; i < outline.size(); i++) {
		b.push_back(new p2t::Point(outline[i].x, outline[i].y));
		delaunay->AddPoint(b.back());
	}
	for(int i = 0; i < insides.size(); i++) {
		b.push_back(new p2t::Point(insides[i].x, insides[i].y));
		delaunay->AddPoint(b.back());
	}
	
	
	delaunay->Triangulate();
	
	vector<p2t::Triangle*> tris = delaunay->GetTriangles();
	int numTris = tris.size();
	unsigned char *depth = grey.getPixels();
	for(int i = 0; i < numTris; i++) {
		
		if(triangleTouchesCorner(tris[i])) {
			continue;
		}
		
		triangles.push_back(KinectTriangle());
		triangles.back().set(tris[i]);
		
		ofVec2f c = triangles.back().centre;
		int pos = ((int)c.x)+ ((int)c.y)*grey.getWidth();
		
		if(depth[pos]>0) {
			triangles.back().hollow = false;
			
			// store a triangle we can use as vbo (as it's unwrapped we actually don't need the indices)
			p2t::Point* p;
			int cdx = pos * 3;
			float inv_col = 1.0f/256.0f;
			p = tris[i]->GetPoint(0); addVertex(KinectVertex(-0.5 + p->x/video_w, 0.5+(-p->y/video_h), 0.0, rgb[cdx+0] * inv_col, rgb[cdx+1] * inv_col, rgb[cdx+2] * inv_col));
			p = tris[i]->GetPoint(1); addVertex(KinectVertex(-0.5 + p->x/video_w, 0.5+(-p->y/video_h), 0.0, rgb[cdx+0] * inv_col, rgb[cdx+1] * inv_col, rgb[cdx+2] * inv_col));
			p = tris[i]->GetPoint(2); addVertex(KinectVertex(-0.5 + p->x/video_w, 0.5+(-p->y/video_h), 0.0, rgb[cdx+0] * inv_col, rgb[cdx+1] * inv_col, rgb[cdx+2] * inv_col));
		
			if(perVertexColour) {
				for(int i = 0; i < 3; i++) {
					ofVec2f c0 = triangles.back().points[i];
					int pos0 = ((int)c0.x)+ ((int)c0.y)*grey.getWidth();
					
					ofColor color0(rgb[pos0*3], rgb[pos0*3+1], rgb[pos0*3+2]);
					triangles.back().colors[i].set(color0.r/255.f, color0.g/255.f, color0.b/255.f);
				}
				triangles.back().sharpenEdges(edgeColourSharpening);
				
				
			} else {
				ofColor color(rgb[pos*3], rgb[pos*3+1], rgb[pos*3+2]);
				triangles.back().colors[0].set(color.r/255.f, color.g/255.f, color.b/255.f);
				triangles.back().colors[1] = triangles.back().colors[0];
				triangles.back().colors[2] = triangles.back().colors[0];
			}
			
		} else {
			triangles.back().hollow = true;
		}
	}
	
	for(int i = 0; i < triangles.size(); i++) {
		if(!triangles[i].hollow) {
			addTriangle(triangles[i]);
		}
	}
	vector<p2t::Point*>::iterator it = b.begin();
	while(it!=b.end()) {
		delete (*it);
		it++;
	}
	return true;
}



void KinectMesh::draw() {
	mesh.draw();
}

void KinectMesh::addTriangle(const KinectTriangle &tri) {
	addTriangle(tri.points[0], tri.points[1], tri.points[2], tri.colors[0], tri.colors[1], tri.colors[2]);
}

void KinectMesh::addTriangle(const ofVec2f &a, const ofVec2f &b, const ofVec2f &c, 
							 const ofFloatColor &c0, const ofFloatColor &c1, const ofFloatColor &c2) {
	
	mesh.addColor(c0);
	mesh.addVertex(a);
	mesh.addColor(c1);
	mesh.addVertex(b);
	mesh.addColor(c2);
	mesh.addVertex(c);

}


