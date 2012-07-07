#ifndef ROXLU_TRAILH
#define ROXLU_TRAILH


/**
 * Templated Trail/Ribbon code.
 *
 * Using templates makes reading c++ code difficult. Therefore it needs some
 * explanation. This Trail class works as std::deque internally. You add 
 * points to it and let the class generate i.e. a triangle strip. It wants
 * a container which can store "Data type" (D) elements. It can create
 * strips for 2D or 3D, with colors, texture coordinates etc.. 
 *
 * I use it like this:
 * -------------------
 * 1. create member: Trail3P (3D, with only position vertices)
 * 2. add points
 * 3. create the strip each frame.
 *
 * You can pass a functor to the createTriangleStrip() function which 
 * operator() is called for each segment it creates. This operator must
 * return the width of the trail.
 *
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


// colorizer
struct TrailTriangleVerticesPCColor {
	void operator()(const float perc, Vec4& color) {
		color.set(perc, perc*0.2, 1.0-perc, perc);
	}
};	

template<class T>
class TrailTriangleVerticesPC {
public:

	T& colorizer;
	
	TrailTriangleVerticesPC(T& colorizer)
		:colorizer(colorizer)
	{
	}
	
	void operator()(Vec3& a, Vec3& b, Vec3& dir, const float width, float perc, VerticesPC& vd) {
		Vec3 up(0,1,0);
		Vec3 crossed = cross(up, dir);
		crossed.normalize();
		crossed *= width;
		
		Vec3 pa = a-crossed;
		Vec3 pb = b+crossed;
		
		// generate color
		Vec4 col;
		colorizer(perc, col);
		
		// a
		VertexPC point_a;
		point_a.setPos(pa);
		point_a.setCol(col);
		vd.add(point_a);
		
		// b
		VertexPC point_b;
		point_b.setPos(pb);
		point_b.setCol(col);
		vd.add(point_b);
	}
	
	void operator()(Vec2& a, Vec2& b, Vec2& dir, const float width, float perc, VerticesPC& vd) {
		Vec2 perp(-a.y, a.x); // or rotate 90 over Z?
		perp.normalize();
		perp *= width;
		
		Vec2 pa = a - perp;	
		Vec2 pb = a + perp;
		
		VertexPC point_a;
		point_a.setPos(pa);
		point_a.setCol(perc, perc*0.2, 1.0-perc, perc);
		vd.add(point_a);
		
		VertexPC point_b;
		point_b.setPos(pb);
		point_b.setCol(perc, perc*0.2, 1.0-perc, perc);
		vd.add(point_b);
	}
	
};

class TrailTriangleVerticesP {
public:

	void operator()(Vec3& a, Vec3& b, Vec3& dir, const float width, float perc, VerticesP& vd) {
		Vec3 up(0,1,0);
		Vec3 crossed = cross(up, dir);
		crossed.normalize();
		crossed *= width;
		
		vd.add(VertexP(a-crossed));
		vd.add(VertexP(b+crossed));	
	}
	
	void operator()(Vec2& a, Vec2& b, Vec2& dir, const float width, float perc, VerticesP& vd) {
		Vec2 perp(-a.y, a.x); // or rotate 90 over Z?
		perp.normalize();
		perp *= width;

		Vec2 pa = a - perp;	
		Vec2 pb = a + perp;
		
		VertexP point_a;
		point_a.pos.set(pa.x, pa.y, 0);
		vd.add(point_a);
		
		VertexP point_b;
		point_b.pos.set(pb.x, pb.y, 0);
		vd.add(point_b);
	}
};


class TrailStaticWidth {
public:
	float operator()(Vec3& a, Vec3& b, Vec3& dir, const float maxWidth, float perc) {
		return maxWidth;
	}
	float operator()(Vec2& a, Vec2& b, Vec2& dir, const float maxWidth, float perc) {
		return maxWidth;
	}
};

class TrailPercentageWidth {
public:
	float operator()(Vec3& a, Vec3& b, Vec3& dir, const float maxWidth, float perc) {
		return maxWidth * perc;
	}
	float operator()(Vec2& a, Vec2& b, Vec2& dir, const float maxWidth, float perc) {
		return maxWidth * perc;
	}
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<class V, class D, class C>
class Trail {
public:

	template<class ITER_FIRST, class ITER_LAST>
	void copyPoints(ITER_FIRST first, ITER_LAST last);
	
	void push_back(const V p);
	void pop_front();
	void clear();
	void limitLength(const unsigned int length);
	
	size_t size();	
	size_t getNumBytesForLineStrip();
	size_t getNumBytesForTriangleStrip();
	
	void createLineStrip(C& vd);
	
	// W = WidthFunction
	// T = Triangulator
	template<class W, class T>
	void createTriangleStrip(const float maxWidth, C& vd, W widthFunctor, T triFunctor);
	void createTriangleStrip(const float maxWidth, C& vd);
	
	typename std::deque<V>::iterator begin();
	typename std::deque<V>::iterator end();
	
	V& operator[](const unsigned int dx);

	std::deque<V> points;
};

template<class V, class D, class C>
inline void Trail<V,D,C>::limitLength(const unsigned int length) {
	if(points.size() > length) {
		points.pop_front();
	}
}

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
inline void Trail<V,D,C>::createTriangleStrip(const float maxWidth, C& vd) {
	TrailPercentageWidth wf;
	TrailTriangleVerticesP trip;
	createTriangleStrip<TrailPercentageWidth, TrailTriangleVerticesP>(maxWidth, vd, wf, trip);	
}

template<class V, class D, class C>
template<class W, class T>
inline void Trail<V, D, C>::createTriangleStrip(const float maxWidth, C& vd, W widthFunctor, T triFunctor) {
	float w = 0.0f;
	float p = 0.0f;
	for(int i = 0; i < points.size()-1; ++i) {
		V& a = points[i];
		V& b = points[i+1];
		V d = a - b;
		p = float(i)/points.size();
		w = widthFunctor(a,b,d,maxWidth, p);
		triFunctor(a,b,d,w,p, vd);
		//createTriangleStripData(a, b, d, w, p, vd);
	}
}


typedef Trail<Vec2, VertexP, VerticesP> Trail2P; // Position
typedef Trail<Vec2, VertexPC, VerticesPC> Trail2PC; // Position and Color

typedef Trail<Vec3, VertexP, VerticesP> Trail3P;
typedef Trail<Vec3, VertexPC, VerticesPC> Trail3PC;

#endif