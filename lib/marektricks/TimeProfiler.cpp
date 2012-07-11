/**
 *  Line.cpp
 *
 *  Created by Marek Bereza on 16/08/2011.
 */

#include "TimeProfiler.h"



tricks::util::NamedTimer::NamedTimer(string name) {
	this->name = name;
}
string tricks::util::NamedTimer::getName() {
	return name;
}

void tricks::util::NamedTimer::start() {
	time = ofGetElapsedTimef();
}


void tricks::util::NamedTimer::stop() {
	TimeProfiler::report(name, ofGetElapsedTimef() - time);
}

map<string,float> tricks::util::TimeProfiler::times;
void tricks::util::TimeProfiler::draw() {
	map<string,float>::iterator it = times.begin();
	int pos = 0;
	while(it!=times.end()) {
		int x = 20;
		int y = 20 + pos * 20;
		ofDrawBitmapString((*it).first, x, y);
		string timeStr = ofToString((*it).second*1000.0,0) + "ms"; 
		ofDrawBitmapString(timeStr, x+190, y); 
		string fpsStr = ofToString(1.f/(*it).second,0) + "fps"; 
		ofDrawBitmapString(fpsStr, x + 300, y);
		pos++;
		it++;
	}
	int x = 20;
	int y = 20 + pos * 20;
	
	string timeStr = ofToString(ofGetFrameRate(),0) + "fps"; 
	ofDrawBitmapString("Framerate: " + timeStr, x, y);

}
void tricks::util::TimeProfiler::print() {
	map<string,float>::iterator it = times.begin();
	while(it!=times.end()) {
		printf("Time profiler\n");
		printf("%s:   %.2f ms\n", (*it).first.c_str(), (*it).second);
	
		it++;
	}
}
void tricks::util::TimeProfiler::report(string name, float time) {
	if(times.find(name)!=times.end()) {
		times[name] = times[name]*0.95 + time*0.05;
	} else {
		times[name] = time;
	}
}

