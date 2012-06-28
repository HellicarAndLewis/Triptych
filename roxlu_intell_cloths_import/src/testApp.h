#pragma once

#include "ofMain.h"
#include "Roxlu.h"
#include "PBD.h"
#include "Buttons.h"
#include "SphereSystem.h"
#include "ParticleMesh.h"
#include "AppSettings.h"

using namespace pbd;

class testApp : public ofBaseApp{
	public:
		testApp();
		void operator()(int i);
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		bool debug;
		bool show_gui;
		
	//	Billboard bb;

		EasyCam cam;
		Axis ax;

		Particles ps;
		ParticleMesh creator;
		SphereSystem ss;
		buttons::Buttons gui;
		buttons::Radio<testApp>* modes;

};
