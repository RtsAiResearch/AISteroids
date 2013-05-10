#pragma once

#include "Point3.h"
#include <GL/glut.h>
#include <math.h>
#include <float.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define randflt() (((float) rand())/((float) RAND_MAX))
#define randint(x,y) (rand()%((y)-(x)+1)+(x))


#define COLOR_WHITE    0
#define COLOR_RED	   1
#define COLOR_GREEN    2
#define COLOR_BLUE	   3
#define COLOR_BLACK    4
#define COLOR_GREY	   5
#define COLOR_CYAN	   6
#define COLOR_YELLOW   7
#define COLOR_MAGENTA  8
#define COLOR_SILVER   9
#define COLOR_MAROON  10 
#define COLOR_DKGREEN 11
#define COLOR_DKBLUE  12
#define COLOR_PURPLE  13
#define COLOR_OLIVE   14
#define COLOR_TEAL    15

extern Point3f colorArray[16];

#define DEGREES_PER_RADIAN	57.29577951f
#define RADIANS_PER_DEGREE	0.017453292f

#define BULLET_SPEED 150.0f

enum
{
    DIR_LEFT  = 0x01,
    DIR_RIGHT = 0x02,
    DIR_UP    = 0x04,
    DIR_DOWN  = 0x08
};

//forward declarations
void glInitColorArray();
void glPrintf(int color, GLuint x, GLuint y, GLfloat scale, char* format, ...);
float FPS(int lastFrameTime);

#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))

//inline math functions
//---------------------------------------------------------
inline float TODEGREES(float radians)
{
    return (radians * DEGREES_PER_RADIAN);
}

//---------------------------------------------------------
inline float TORADIANS(float degrees)
{
    return (degrees * RADIANS_PER_DEGREE);
}

//---------------------------------------------------------
inline bool ISZERO( float n )
{
    return ( fabs(n) < FLT_EPSILON );
}

//---------------------------------------------------------
inline Point3f CROSS(Point3f &a,Point3f &b)
{
	Point3f temp;
	temp.x() = a.y()*b.z()-a.z()*b.y();
	temp.y() = a.z()*b.x()-a.x()*b.z();
	temp.z() = a.x()*b.y()-a.y()*b.x();
	return temp;
}

//---------------------------------------------------------
inline float DOT(Point3f &a, Point3f &b)
{
	return a.x()*b.x()+a.y()*b.y()+a.z()*b.z();
}

//---------------------------------------------------------
inline Point3f ROT2D(Point3f &a, float angle)
{
	Point3f result;
	float angleR = TORADIANS(angle);
	float sinAngle;
	float cosAngle;
	
	sinAngle = sinf( angleR );
	cosAngle = cosf( angleR );
	
	result.x() = ( cosAngle * a.x() ) - ( sinAngle * a.y() );
	result.y() = ( sinAngle * a.x() ) + ( cosAngle * a.y() );
	result.z() = a.z();
	
	return result;
}

//---------------------------------------------------------
inline float CLAMPDIR360(float dir)
{
	float outDirAbs = fabsf(dir);
	float outDir;
	
	if( outDirAbs > 360.0f )
	{
		if( outDirAbs > 720.0f )
		{
			outDir = fmodf(dir, 360.0f);
		}
		else
		{
			outDir = ( dir > 0.0f ) ? dir - 360.0f : dir + 360.0f;
		}
		outDirAbs = fabsf(outDir);
	}
	else
	{
		if(dir < 0.0f)
			outDir = dir + 360.0f;
		else
			outDir = dir;
	}
	return outDir;
}

//---------------------------------------------------------
inline float CLAMP(float value, float floor,float ceiling)
{
    if(value< floor)
        return floor;
    else if(value > ceiling)
        return ceiling;
    return value;
}

//---------------------------------------------------------
inline void CLAMPVECTORLENGTH(Point3f& value, float floor,float ceiling)
{//ensure length of vector is between floor and ceiling 
	float vecLength = value.Length();
	float adjust = -1.0f;
	if(vecLength < floor)
		adjust = floor;
	else if(vecLength > ceiling)
		adjust = ceiling;

	if(adjust != -1.0f)
	{
		value.Normalize();
		value *= adjust;
	}
}

//---------------------------------------------------------
inline float CLAMPDIR180(float dir)
{
	float outDirAbs = fabsf(dir);
	float outDir;
	
	if( outDirAbs > 360.0f )
	{
		if( outDirAbs > 720.0f )
		{
			outDir = fmodf(dir, 360.0f);
		}
		else
		{
			outDir = ( dir > 0.0f ) ? dir - 360.0f : dir + 360.0f;
		}
		outDirAbs = fabsf(outDir);
	}
	else
	{
		outDir = dir;
	}
	
	if(outDirAbs > 180.0f)
	{
		if(outDir > 0.0f)
		{
			outDir -= 360.0f;
		}
		else
		{
			outDir += 360.0f;
		}
	}
	return outDir;
}

//---------------------------------------------------------
inline float GETRELATIVEANGLE(float firstAngle, float secondAngle)
{
	float result = CLAMPDIR180((M_PI*2) - firstAngle + secondAngle);

	return result;
}

//---------------------------------------------------------
inline bool ISLEFT(float angle, float angle2)
{
	bool result = GETRELATIVEANGLE(angle,angle2) > 0.0f;

	return result;
}

//---------------------------------------------------------
inline float CALCDIR( float fromX, float fromY, float toX, float toY )
{
    float deltaX = toX - fromX;
    float deltaY = toY - fromY;
	
	// Zero degree angle along the +X axis
	float result = atan2f( deltaY, deltaX );
	result = CLAMPDIR180( TODEGREES(result) );
	return result;
}	

//---------------------------------------------------------
inline float CALCDIR( Point3f &vect )
{
	// Zero degree angle along the +X axis
	float result = atan2f( vect.y(),vect.x());
	result = CLAMPDIR180( TODEGREES(result) );
	return result;
}	

//---------------------------------------------------------
inline float CALCDIR360( Point3f &vect )
{
    // Zero degree angle along the +X axis
    float result = atan2f( vect.y(),vect.x());
    result = CLAMPDIR360( TODEGREES(result) );
    return result;
}	

#define QUAD_ONE   0
#define QUAD_TWO   1
#define QUAD_THREE 2
#define QUAD_FOUR  3
//---------------------------------------------------------
inline int GETQUADRANT(Point3f &vec)
{
    // Zero degree angle along the +X axis
    float dir = atan2f( vec.y(), vec.x() );
    dir = CLAMPDIR360( TODEGREES(dir) );
    return ((int)(dir/90.0f));
}

#define SECTOR_SIZE 20.0f
//---------------------------------------------------------
inline int GETSECTOR(Point3f &vec)
{
    // Zero degree angle along the +X axis
    float dir = atan2f( vec.y(), vec.x() );
    dir = CLAMPDIR360( TODEGREES(dir) );
    return ((int)(dir/SECTOR_SIZE));
}

//---------------------------------------------------------
inline void LINEINTERSECT(Point3f &pt1, Point3f &pt2,Point3f &pt3, Point3f &pt4, float &lambda1,float &lambda2, Point3f &retval)
{
	float denom =  ((pt4.y() - pt3.y())*(pt2.x() - pt1.x())) - ((pt4.x() - pt3.x())*(pt2.y() - pt1.y()));
	float nume_a = ((pt4.x() - pt3.x())*(pt1.y() - pt3.y())) - ((pt4.y() - pt3.y())*(pt1.x() - pt3.x()));
	float nume_b = ((pt2.x() - pt1.x())*(pt1.y() - pt3.y())) - ((pt2.y() - pt1.y())*(pt1.x() - pt3.x()));
	if(ISZERO(denom))
	{
		retval = pt1;
		if(ISZERO(nume_a) && ISZERO(nume_b))
			lambda1 = lambda2 = 0.0f;
		else
			lambda1 = lambda2 = FLT_MAX;
		return;
	}
	//calculate lambda for line 1
	lambda1 = nume_a / denom;
	lambda2 = nume_b / denom;

	// Get the intersection point along line 1.
	Point3f delta21 = pt2 - pt1;
	retval = pt1 + lambda1*(delta21);
}

//---------------------------------------------------------
inline float GETNORMALDIR( float myDirection, float otherDirection )
{
    float result = CLAMPDIR180( 360.0f - myDirection + otherDirection );
    
    return result;
}

//---------------------------------------------------------
inline Point3f UNITFROMANGLE(float angle)
{
    return Point3f(cos(M_PI*angle/180.0),sin(M_PI*angle/180.0),0);
}

//---------------------------------------------------------------------------
inline Point3f UNITCROSS(Point3f &vec1,Point3f &vec2)
{
    Point3f cross = CROSS(vec1,vec2);
    float length = cross.Length();
    if ( length > 1e-06f )
        return cross/length;
    else
        return Point3f(0.0f,0.0f,0.0f);
}

//---------------------------------------------------------
inline float LERP(float t, float a, float b)
{
    return a + (b - a)*t;
}

