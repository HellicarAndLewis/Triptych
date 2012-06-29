/**     ___           ___           ___                         ___           ___     
 *     /__/\         /  /\         /  /\         _____         /  /\         /__/|    
 *    |  |::\       /  /::\       /  /::|       /  /::\       /  /::\       |  |:|    
 *    |  |:|:\     /  /:/\:\     /  /:/:|      /  /:/\:\     /  /:/\:\      |  |:|    
 *  __|__|:|\:\   /  /:/~/::\   /  /:/|:|__   /  /:/~/::\   /  /:/  \:\   __|__|:|    
 * /__/::::| \:\ /__/:/ /:/\:\ /__/:/ |:| /\ /__/:/ /:/\:| /__/:/ \__\:\ /__/::::\____
 * \  \:\~~\__\/ \  \:\/:/__\/ \__\/  |:|/:/ \  \:\/:/~/:/ \  \:\ /  /:/    ~\~~\::::/
 *  \  \:\        \  \::/          |  |:/:/   \  \::/ /:/   \  \:\  /:/      |~~|:|~~ 
 *   \  \:\        \  \:\          |  |::/     \  \:\/:/     \  \:\/:/       |  |:|   
 *    \  \:\        \  \:\         |  |:/       \  \::/       \  \::/        |  |:|   
 *     \__\/         \__\/         |__|/         \__\/         \__\/         |__|/   
 *
 *  Description: 
 *				 
 *  KinectMesh.h, created by Marek Bereza on 29/06/2012.
 */
#include "ofxOpenCv.h"
#include "contourutils.h"
#include "ofxDelaunay.h"

struct Triangle {
	ofVec2f points[3];
	ofVec2f centre;
	bool hollow;
	ofFloatColor color;
	void set(ITRIANGLE tri, XYZ *ps) {
		hollow = false;
		points[0].set(ps[tri.p1].x, ps[tri.p1].y);
		points[1].set(ps[tri.p2].x, ps[tri.p2].y);
		points[2].set(ps[tri.p3].x, ps[tri.p3].y);
		centre = (points[0] + points[1] + points[2])/3.f;
	}

		
	
};
class KinectMesh {
public:
	vector<ofVec2f> outline;
	vector<ofVec2f> insides;
	ofVboMesh mesh;
	
	int age;
	vector<Triangle> triangles;
	/**
	 * Returns false if the blob is too small to create a mesh from
	 */
	bool setup(const ofxCvBlob &blob, ofxCvGrayscaleImage &grey, unsigned char *rgb) {
		age = 0;
		int step = 20;
		int insideStep = 40;
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
		ofxDelaunay delaunay;
		for(int i = 0; i < outline.size(); i++) {
			delaunay.addPoint(outline[i].x, outline[i].y);
		}
		for(int i = 0; i < insides.size(); i++) {
			delaunay.addPoint(insides[i].x, insides[i].y);
		}
		
		
		delaunay.triangulate();
		
		XYZ *points = delaunay.getPoints();
		ITRIANGLE *tris = delaunay.getTriangles();

		int numTris = delaunay.getNumTriangles();
		
		unsigned char *depth = grey.getPixels();
		for(int i = 0; i < numTris; i++) {
			triangles.push_back(Triangle());
			triangles.back().set(tris[i], points);
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
		return true;
	}
	void draw() {
		age++;

		ofSetHexColor(0x00FF00);
		/*glLineWidth(3);
		
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < outline.size(); i++) {
			glVertex2f(outline[i].x, outline[i].y);
		}
		glEnd();
		
		glPointSize(7);
		glBegin(GL_POINTS);
		for(int i = 0; i < insides.size(); i++) {
			glVertex2f(insides[i].x, insides[i].y);
		}
		glEnd();
		glLineWidth(1);
		glPointSize(1);*/
		
		float alpha = ofMap(age, 0, 20, 1, 0, true);
		for(int i = 0; i < triangles.size(); i++) {
				
			if(triangles[i].hollow) {
				/*glColor4f(1, 1, 1, 1);
				glBegin(GL_LINE_LOOP);
				glVertex2f(triangles[i].points[0].x, triangles[i].points[0].y);
				glVertex2f(triangles[i].points[1].x, triangles[i].points[1].y);
				glVertex2f(triangles[i].points[2].x, triangles[i].points[2].y);
				glEnd();*/
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
	}
};

