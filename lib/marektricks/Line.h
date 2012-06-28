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
 *  Line.h, created by Marek Bereza on 16/08/2011.
 */

#pragma once
#include "ofMain.h"
namespace tricks {
	
	namespace math {
		class Line {
		public:
			
			ofVec2f start;
			ofVec2f end;
			Line() {
				
			}
			
			Line(ofVec2f start, ofVec2f end) {
				this->start = start;
				this->end   = end;
			}
			
			float length() {
				return start.distance(end);
			}
			
			float lengthSquared() {
				return start.distanceSquared(end);
			}
			
			float angle() {
				return ofRadToDeg(atan2(end.y - start.y, end.x - start.x));

			}
			
			void reverse() {
				ofVec2f temp = this->start;
				this->start = this->end;
				this->end = temp;
			}
			
			void draw() {
				ofLine(start, end);
			}
			bool operator< (const Line &line) const {
				float ax = this->end.x - this->start.x;
				float ay = this->end.y - this->start.y;
				
				float bx = line.end.x - line.start.x;
				float by = line.end.y - line.start.y;

				return (ax*ax + ay*ay) < (bx*bx + by*by);

			}
		};
		
		class LineEquation {
		public:
			
			float m;
			float c;
			
			
			void setFrom2Points(ofPoint a, ofPoint b) {
				ofPoint g = b - a;
				float m = g.y / g.x;
				
				// y = mx + c		
				// so c = y - mx
				float c = a.y - m*a.x;
				
				
				set(m, c);
			}
			
			void setFrom2Points(ofVec2f a, ofVec2f b) {
				ofVec2f g = b - a;
				float m = g.y / g.x;
				
				// y = mx + c		
				// so c = y - mx
				float c = a.y - m*a.x;
				
				
				set(m, c);
			}
			
			
			float distanceToPoint(const ofVec2f &p) {
				ofVec2f a = ofVec2f(1, getY(1));
				ofVec2f b = ofVec2f(2, getY(2));
				// from http://www.softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm#Distance to 2-Point Line
				return ABS(((a.y - b.y)*p.x + (b.x - a.x)*p.y + (a.x*b.y - b.x*a.y))/
						   sqrt((b.x-a.x)*(b.x-a.x) + (b.y - a.y)*(b.y - a.y))
						   );
			}
			
			/**
			 * Returns the index of the closest point in the array.
			 */
			int closestPoint(const vector<ofVec2f> &p) {
				ofVec2f a = ofVec2f(1, getY(1));
				ofVec2f b = ofVec2f(2, getY(2));
				
				
				// from http://www.softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm#Distance to 2-Point Line
				float amby = a.y - b.y;
				float bmax = b.x - a.x;
				
				float axbymbxay = (a.x*b.y - b.x*a.y);
				float minDist = FLT_MAX;
				int minI = 0;
				for(int i = 0; i < p.size(); i++) {

					float dist = ABS((amby*p[i].x + bmax*p[i].y + axbymbxay));
					if(dist<minDist) {
						minDist = dist;
						minI = i;
					}
				}
				
				return minI;
			}
			
			
			void set(float m, float c) {
				this->m = m;
				this->c = c;
			}
			
			float getY(float x) {
				return m*x + c;
			}
			
			float getX(float y) {
				return (y-c)/m;
			}
		};
	}
}