/**
 *  Line.cpp
 *
 *  Created by Marek Bereza on 16/08/2011.
 */

#include "Line.h"



NamedTimer::NamedTimer(string name) {
	this->name = name;
}
string NamedTimer::getName() {
	return name;
}

void NamedTimer::start() {
	time = ofGetElapsedTimef();
}


void NamedTimer::stop() {
	TimeProfiler::report(name, ofGetElapsedTimef() - time);
}

map<string,float> TimeProfiler::times;
void TimeProfiler::draw() {

}
void TimeProfiler::print() {
	map<string,float>::iterator it = times.begin();
	while(it!=times.end()) {
		printf("Time profiler\n");
		printf("%s:   %.2f ms\n", (*it).first.c_str(), (*it).second);
	
		it++;
	}
}
void TimeProfiler::report(string name, float time) {
	times[name] = time;
}