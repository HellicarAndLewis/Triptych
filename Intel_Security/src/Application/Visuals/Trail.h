#ifndef ROXLU_TRAILH
#define ROXLU_TRAILH

/*
#include "Vec3.h"
#include "Vec2.h"
#include "VertexTypes.h"
#include <deque>
*/

#include <roxlu/math/Vec3.h>
#include <roxlu/math/Vec2.h>
#include <roxlu/3d/VertexTypes.h>
#include <deque>

// V = Vec type
// D = Data type (VertexP, VetexPC, etc..)
// C = Container for Data (VerticesP, VerticesPC, etc..)

inline void createTrailLineStripData(Vec2& point, float perc, VerticesP& vd) {
	vd.add(point.x, point.y, 0);
}

inline void createTrailLineStripData(Vec2& point, float perc, VerticesPC& vd) {
	VertexPC pc;
	pc.setPos(Vec3(point));
	pc.setCol(perc, perc*0.2, 1.0-perc, perc);
	vd.add(pc);
}

inline void	createTriangleStripData(Vec2& a, Vec2& b, Vec2& dir, const float maxWidth, float perc, VerticesP& vd) {
	Vec2 perp(-a.y, a.x); // or rotate 90 over Z?
	perp.normalize();
	
	float s = maxWidth * perc;
	Vec2 pa = a - perp * s;	
	Vec2 pb = a + perp * s;
	
	VertexP point_a;
	point_a.pos.set(pa.x, pa.y, 0);
	vd.add(point_a);
	
	VertexP point_b;
	point_b.pos.set(pb.x, pb.y, 0);
	vd.add(point_b);
}

inline void	createTriangleStripData(Vec2& a, Vec2& b, Vec2& dir, const float maxWidth, float perc, VerticesPC& vd) {
	
	Vec2 perp(-a.y, a.x); // or rotate 90 over Z?
	perp.normalize();
	float s = maxWidth * perc;
	Vec2 pa = a - perp * s;	
	Vec2 pb = a + perp * s;
	
	VertexPC point_a;
	point_a.setPos(pa);
	point_a.setCol(perc, perc*0.2, 1.0-perc, perc);
	vd.add(point_a);
	
	VertexPC point_b;
	point_b.setPos(pb);
	point_b.setCol(perc, perc*0.2, 1.0-perc, perc);
	vd.add(point_b);
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<class V, class D, class C>
class Trail {
public:

	template<class ITER_FIRST, class ITER_LAST>
	void copyPoints(ITER_FIRST first, ITER_LAST last);
	
	void push_back(const V p);
	void pop_front();
	void clear();
	
	size_t size();	
	size_t getNumBytesForLineStrip();
	size_t getNumBytesForTriangleStrip();
	
	void createLineStrip(C& vd);
	void createTriangleStrip(const float maxWidth, C& vd);
	
	typename std::deque<V>::iterator begin();
	typename std::deque<V>::iterator end();
	
	V& operator[](const unsigned int dx);

	std::deque<V> points;
};

template<class V, class D, class C>
inline V& Trail<V,D,C>::operator[](const unsigned int dx) {
	return points[dx];
}

template<class V, class D, class C>
inline void Trail<V, D, C>::clear() {
	points.clear();	
}

template<class V, class D, class C>
inline void Trail<V, D, C>::push_back(const V p) {
	points.push_back(p);
}

template<class V, class D, class C>
inline void Trail<V, D, C>::pop_front() {
	points.pop_front();
}


template<class V, class D, class C>
template<class ITER_FIRST, class ITER_LAST>
inline void Trail<V, D, C>::copyPoints(ITER_FIRST first, ITER_LAST last) {
	std::copy(first, last, std::back_inserter(points));
}
	

template<class V, class D, class C>
inline typename std::deque<V>::iterator Trail<V, D, C>::begin() {
	return points.begin();
}

template<class V, class D, class C>
inline typename std::deque<V>::iterator Trail<V, D, C>::end() {
	return points.end();
}

template<class V, class D, class C>
inline size_t Trail<V, D, C>::size() {
	return points.size();
}

template<class V, class D, class C>
inline size_t Trail<V, D, C>::getNumBytesForLineStrip() {
	return size() * sizeof(D);
}

template<class V, class D, class C>
inline size_t Trail<V, D, C>::getNumBytesForTriangleStrip() {
	return size() * sizeof(D) * 2;
}


template<class V, class D, class C>
inline void Trail<V,D,C>::createLineStrip(C& vd) {
	for(int i = 0; i < points.size(); ++i) {
		float p = float(i)/points.size();
		createTrailLineStripData(points[i], p, vd);		
	}
}

template<class V, class D, class C>
inline void Trail<V, D, C>::createTriangleStrip(const float maxWidth, C& vd) {
	for(int i = 0; i < points.size()-1; ++i) {
		V& a = points[i];
		V& b = points[i+1];
		V d = a - b;
		float p = float(i)/points.size();
		createTriangleStripData(a, b, d, maxWidth, p, vd);
	}
}


typedef Trail<Vec2, VertexP, VerticesP> Trail2P; // Position
typedef Trail<Vec2, VertexPC, VerticesPC> Trail2PC;

typedef Trail<Vec3, VertexP, VerticesP> Trail3P;
typedef Trail<Vec3, VertexP, VerticesPC> Trail3PC;

#endif