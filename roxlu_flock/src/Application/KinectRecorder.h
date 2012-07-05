#ifndef ROXLU_KINECTRECORDERH
#define ROXLU_KINECTRECORDERH

#include "KinectTriangle.h"
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <roxlu/experimental/Timer.h>

struct KinectRecorderFrame {
	KinectVertices vertices;
};

enum KinectRecorderTypes {
	KINECT_VERTICES
};

class KinectRecorder {
public:
	KinectRecorder();
	~KinectRecorder();
	void addFrame(KinectVertices& vertices);	
	bool save(const std::string& filepath);
	bool load(const std::string& filepath);	
	bool update();
	void play();
	void stop();
	KinectRecorderFrame& getCurrentFrame();
	std::vector<KinectRecorderFrame*> frames;

	void setFPS(uint64_t fps);

	bool is_loaded;
	bool is_playing;	
	uint64_t fps;
	int index;
	uint64_t next_frame_millis;
	uint64_t num_frames;
};

inline void KinectRecorder::setFPS(uint64_t f) {
	this->fps = float(1.0f/f) * 1000;
}

inline void KinectRecorder::play() {
	is_playing = true;
}

inline void KinectRecorder::stop() {
	is_playing = false;
}

inline KinectRecorderFrame& KinectRecorder::getCurrentFrame() {
	return *frames[index];
}
#endif