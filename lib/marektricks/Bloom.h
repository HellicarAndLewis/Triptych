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
 *  Description: A bloom shader. The simplest way to use it is to just call
 *				 begin() and end() around the parts you want bloomed.
 * 
 *				 However, if you want to draw the result to an FBO, pass in
 *				 drawToFbo as true to setup() and use getOutput to get the fbo
 *				 
 *  Bloom.h, created by Marek Bereza on 05/10/2011.
 */

#pragma once
#include "ofMain.h"
namespace tricks { 
	namespace gl { 
		namespace effects {
			class Bloom {
			public:
				
				void setup(bool drawToFbo = false);
				void resize(int width, int height);
				void begin();
				void end();
				
				// only use this if you set it up with drawFbo = true
				ofFbo *getOutput();
				
				float amount;
				float brightness;
			private:
				bool drawToFbo;
				
				void loadShader();

				ofFbo output;
				ofFbo out1;
				ofFbo out2;
				
				ofShader shader;
				
				
				ofVec2f blurX;
				ofVec2f blurY;

			};
		}
	}
}