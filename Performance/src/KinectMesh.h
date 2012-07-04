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
#include "poly2tri.h"


struct Triangle {
	ofVec2f points[3];
	ofVec2f centre;
	bool hollow;
	ofFloatColor color;
	void set(p2t::Triangle *t) {
		hollow = false;
/*		points[0].set(ps[tri.p1].x, ps[tri.p1].y);
		points[1].set(ps[tri.p2].x, ps[tri.p2].y);
		points[2].set(ps[tri.p3].x, ps[tri.p3].y);
 */
		for(int i = 0; i < 3; i++) {
			points[i].set(t->GetPoint(i)->x, t->GetPoint(i)->y);
		}
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


	static int borderResolution;
	static int fillResolution;
	static void setupGui();
	
	/**
	 * Returns false if the blob is too small to create a mesh from
	 */
	bool setup(const ofxCvBlob &blob, ofxCvGrayscaleImage &grey, unsigned char *rgb);
	void draw();
	void setTint(int tint);
	static ofShader *shader;
private:
	
	int tint;
	void addTriangle(ofVec3f a, ofVec3f b, ofVec3f c, ofFloatColor color);
	void addTriangle(const Triangle &tri);
	static p2t::CDT *delaunay;
	bool triangleTouchesCorner(p2t::Triangle *t);
};

