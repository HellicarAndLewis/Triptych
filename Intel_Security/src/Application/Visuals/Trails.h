#ifndef ROXLU_TRAILSH
#define ROXLU_TRAILSH

#include <vector>

#include <roxlu/Roxlu.h>
#include <application/visuals/Trail.h>


// T = Trail type
// C = Container type: VerticesP, VerticesPC, etc..

template<class T>
struct TrailInfo {
	TrailInfo<T>() 
		:start_index(0)
		,num_elements(0)
	{
	}
	
	int start_index;
	int num_elements;
	T* trail;
};

template<class T, class C>
class Trails {
public:
	void addTrail(T* trail);

	size_t getNumBytesForLineStrips();
	size_t getNumBytesForTriangleStrips();

	void createLineStrips(C& vd);
	void createTriangleStrips(const float maxWidth, C& vd);

	void clear();

	typedef typename std::vector<TrailInfo<T> >::iterator iterator;
	typedef TrailInfo<T> Info;
	
	typename std::vector<TrailInfo<T> >::iterator begin();
	typename std::vector<TrailInfo<T> >::iterator end();
	
	std::vector<TrailInfo<T> > trails;
};

// Trails does not own the trails! so delete them yourself.
template<class T, class C>
inline void Trails<T, C>::addTrail(T* tr) {
	TrailInfo<T> ti;
	ti.trail = tr;
	trails.push_back(ti);	
}


template<class T, class C>
inline typename std::vector<TrailInfo<T> >::iterator Trails<T,C>::begin() {
	return trails.begin();
}

template<class T, class C>
inline typename std::vector<TrailInfo<T> >::iterator Trails<T,C>::end() {
	return trails.end();
}

template<class T, class C>
inline void Trails<T,C>::clear() {
	trails.clear();
}

template<class T, class C>
inline size_t Trails<T, C>::getNumBytesForLineStrips() {
	size_t result = 0;
	for(typename std::vector<TrailInfo<T> >::iterator it = trails.begin(); it != trails.end(); ++it) {
		TrailInfo<T>& ti = *it;
		result += ti.trail->getNumBytesForLineStrip();
	}
	return result;
}

template<class T, class C>
inline size_t Trails<T, C>::getNumBytesForTriangleStrips() {
	size_t result = 0;
	for(typename std::vector<TrailInfo<T> >::iterator it = trails.begin(); it != trails.end(); ++it) {
		TrailInfo<T>& ti = *it;
		result += ti.trail->getNumBytesForTriangleStrip();
	}
	return result;
}

template<class T, class C>
inline void Trails<T,C>::createLineStrips(C& vd) {
	vd.clear();
	int prev = 0;
	for(typename std::vector<TrailInfo<T> >::iterator it = trails.begin(); it != trails.end(); ++it) {
		TrailInfo<T>& ti = *it;
		ti.start_index = vd.size();
		ti.trail->createLineStrip(vd);
		ti.num_elements = vd.size() - ti.start_index;
	}
}

template<class T, class C>
inline void Trails<T,C>::createTriangleStrips(const float maxWidth, C& vd) {
	vd.clear();
	int prev = 0;
	for(typename std::vector<TrailInfo<T> >::iterator it = trails.begin(); it != trails.end(); ++it) {
		TrailInfo<T>& ti = *it;
		ti.start_index = vd.size();
		ti.trail->createTriangleStrip(maxWidth, vd);
		ti.num_elements = vd.size() - ti.start_index;
	}
}

typedef Trails<Trail<Vec2, VertexPC, VerticesPC>, VerticesPC > Trails2PC; // Position, Color

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inline void drawTrailVertex(Vec2 v) {
	glVertex2fv(v.getPtr());
}


template<class V, class D, class C>
inline void draw(Trail<V, D, C>& trail) {
	glColor3f(1,1,1);
	glBegin(GL_LINE_STRIP);
	for(deque<Vec2>::iterator it = trail.begin(); it != trail.end(); ++it) {
		drawTrailVertex(*it);
	}
	glEnd();
}

template<class T, class C>
inline void draw(Trails<T,C>& trails) {
	
	for(typename vector<TrailInfo<T> >::iterator it = trails.begin(); it != trails.end(); ++it) {
		TrailInfo<T>& ti = *it;
		draw(*ti.trail);
	}
}

#endif