#include "testApp.h"
#include "ofAppGlutWindow.h"
#include <roxlu/core/platform/Platform.h>

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window

#ifdef ROXLU_PLATFORM == ROXLU_APPLE
//	window.setGlutDisplayString("rgba double samples>=4 depth");
#endif

	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
