#include "KinectRecorder.h"


KinectRecorder::KinectRecorder() 
	:is_playing(false)
	,is_loaded(false)
	,next_frame_millis(0)
	,index(0)

{
	setFPS(10);
}


KinectRecorder::~KinectRecorder() {
	for(int i = 0; i < frames.size(); ++i) {
		delete frames[i];
	}
}

void KinectRecorder::addFrame(KinectVertices& vertices) {
	KinectRecorderFrame* f = new KinectRecorderFrame();
	f->vertices = vertices;
	frames.push_back(f);
	printf("New kinect frame: %zu\n", frames.size());
}

bool KinectRecorder::save(const std::string& filepath) {
	std::ofstream ofs;
	ofs.open(filepath.c_str(), std::ios::out | std::ios::binary);
	if(!ofs.is_open()) {
		printf("cannot open: %s\n", filepath.c_str());
		return false;
	}
	
	size_t num_frames = frames.size();
	ofs.write((char*)& num_frames, sizeof(size_t));
	
	for(vector<KinectRecorderFrame*>::iterator it = frames.begin(); it != frames.end(); ++it) {
		KinectRecorderFrame& frame = **it;
		
		// store the vertices
		KinectVertices& vertices = frame.vertices;
		size_t num_vertices = vertices.size();
		ofs.write((char*)&num_vertices, sizeof(size_t));
		for(int i = 0; i < vertices.size(); ++i) {
			KinectVertex& kv = vertices[i];
			ofs.write((char*)kv.pos, sizeof(float) * 3);
			ofs.write((char*)kv.col, sizeof(float) * 3);
		}
	}
	
	ofs.close();
	printf("Saved.\n");
	return true;
}

bool KinectRecorder::load(const std::string& filepath) {
	std::ifstream ifs;
	ifs.open(filepath.c_str(), std::ios::out | std::ios::binary);
	if(!ifs.is_open()) {
		printf("cannot open: %s\n", filepath.c_str());
		return false;
	}
	
	size_t num_frames = 0;
	ifs.read((char*)&num_frames, sizeof(size_t));

	for(int i = 0; i < num_frames; ++i) {
		KinectVertices verts;
		size_t num_vertices;
		ifs.read((char*)&num_vertices, sizeof(size_t));

		for(int j = 0; j <  num_vertices; ++j) {
			KinectVertex kv;
			ifs.read((char*)kv.pos, sizeof(float) * 3);
			ifs.read((char*)kv.col, sizeof(float) * 3);
			verts.add(kv);
		}

		addFrame(verts);
	}
	is_loaded = true;
	return true;
}

bool KinectRecorder::update() {
	if(!is_playing || !is_loaded || frames.size() == 0) {
		return false;
	}
	
	uint64_t n = roxlu::Timer::now();
	if(n > next_frame_millis) {
		index = ++index % frames.size();
		next_frame_millis = n + fps;
	}
	return true;
}
	
