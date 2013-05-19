#pragma once

#include <assert.h>
#include "core.h"

template <class FLTYPE> class Sphere3
{
  public:
	cyclone::Vector3 c;
	FLTYPE r;
	inline Sphere3( void ){}
	inline Sphere3(cyclone::Vector3 const & cc,FLTYPE const & rr){c=cc;r=rr;}
	inline Sphere3(cyclone::Vector3 const & cc){c=cc;r=0;}
	
	inline int Circumscribe(cyclone::Vector3 &v0,cyclone::Vector3 &v1,cyclone::Vector3 &v2,cyclone::Vector3 &v3);
	bool operator < (Sphere3 const & s) const {
		if(c!=s.c) return c<s.c;
		else return r<s.r;}
	bool operator == (Sphere3 const & s) const{
		return (c==s.c && r==s.r);}


	inline bool Intersect(cyclone::Vector3 const & p) const {
		if((c-p).LengthSquared() < r*r) return true;
		else return false;
	}

	inline bool Intersect(Sphere3<FLTYPE> const & s) const {
		if((c-s.c).magnitude()< r+s.r) return true;
		else return false;
	}
    inline bool Intersect(cyclone::Vector3 &p1,cyclone::Vector3 &p2)
    {//circle to line segment
        float top = (c.x - p1.x)*(p2.x - p1.x)  + 
                    (c.y - p1.y)*(p2.y - p1.y)  +
                    (c.z - p1.z)*(p2.z - p1.z);
        float bottom = (p2.x - p1.x)*(p2.x - p1.x)  + 
                       (p2.y - p1.y)*(p2.y - p1.y)  +
                       (p2.z - p1.z)*(p2.z - p1.z);
        float result = top/bottom;
        if(fabsf(result) >= 0 && fabsf(result) <= 1.0f )
        {
            Vector3 point = p1 + (p2 - p1) * result;
            if(point.distance(c) <= r)
                return true;
        }
        return false;
    }

	inline FLTYPE Distance( Sphere3<FLTYPE> const & s ) const{
			return (c-s.c).Length()-(r+s.r);
	}
	// It is not faster that Distance!!!
		inline FLTYPE SquaredDistance( Sphere3<FLTYPE> const & s ) const{
			return (c-s.c).Length()-(r+s.r)*(c-s.c).Length()-(r+s.r);
	}

}; // End Class


/** Algorithms Working with spheres and points */

template <class FLTYPE>
inline FLTYPE Distance( Sphere3<FLTYPE> const & s,cyclone::Vector3 const & p ){
    return Length(s.c-p)-s.r;
}
template <class FLTYPE>
inline FLTYPE SquaredDistance( Sphere3<FLTYPE> const & s,cyclone::Vector3 const & p ){
    return LengthSquared(s.c-p)-s.r*s.r;
}

template <class FLTYPE>
inline FLTYPE Distance( Sphere3<FLTYPE> const & s1,Sphere3<FLTYPE> const & s2 ){
    return Length(s1.c-s2.c)-(s1.r+s2.r);
}
template <class FLTYPE>
inline FLTYPE SquaredDistance( Sphere3<FLTYPE> const & s,Sphere3<FLTYPE> const & p ){
    return Length(s1.c-s2.c)-(s1.r+s2.r)*Length(s1.c-s2.c)-(s1.r+s2.r);
}

typedef Sphere3<short>  Sphere3s;
typedef Sphere3<int>    Sphere3i;
typedef Sphere3<float>  Sphere3f;
typedef Sphere3<double> Sphere3d;

