#pragma once

#include "ofMain.h"

#include <application/Application.h>
#include <application/Settings.h>
#include <buttons/Buttons.h>
#include <pbd/PBD.h>
#include "Room.h"

#define USE_FLOCK_GUI
#define USE_APP

class testApp : public ofBaseApp{
	public:
		testApp();
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
		
		void operator()(const int n);
		
		bool show_gui;
		bool debug;
		
		EasyCam cam;
		Axis ax;

#ifdef USE_APP
		Application app;
#endif

#ifdef USE_FLOCK_GUI		
		buttons::Buttons flock_gui;
#endif

		Particles2 ps;
		Room room;
};
