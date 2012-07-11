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
 *                This helps you time stuff that's happening in your code.
 *                Create NamedTimers and call start and stop when you want to time an event.
 *                Then call TimeProfiler::draw() to draw a report of how long everything is taking 
 *				 
 
  ReusableTimer lets you create a timer that you can keep using
  e.g.
  
  ReusableTimer t;
  
  t.start("Computer Vision");
  
  (vision code)...
  
  t.start("Particle system");
  
  (particle code)...
  
  t.stop();
  
 *  TimeProfiler.h, created by Marek Bereza on 16/08/2011.
 */

#pragma once
#include "ofMain.h"
namespace tricks {
	
	namespace util {
		
		
		
		class TimeProfiler {
		public:
			static void draw();
			static void print();
			static void report(string name, float time);
			static map<string,float> times;
		};
		
		
		
		class NamedTimer {
		public:
			NamedTimer(string name);
			string getName();
		
			void start();
			void stop();
		protected:
			string name;
		};
		
		
		class ScopedTimer: public: NamedTimer {
		public:
			ScopedTimer(string name): NamedTimer(name) {
				start();
			}
			~ScopedTimer() {
				stop();
			}
		};
		
		
		class ReusableTimer: public NamedTimer {
			ReusableTimer(): NamedTimer("") {
				running = false;
			}
			void start(string name) {
				if(running) {
					stop();
				}
				
				this->name = name;
				start();
				running = false;
			}
		};
		
		
	}
}