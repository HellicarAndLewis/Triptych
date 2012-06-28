#ifndef ROXLU_TRAILH
#define ROXLU_TRAILH

#include "Roxlu.h"
#include <deque>

using std::deque;

class Trail {
public:
	Trail();
	void createVertices(VerticesPC& vd, deque<Vec3>& points, const float& agep);
	
	int start_index;
	int num_nodes;
};


#endif