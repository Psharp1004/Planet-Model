//Assn3 object will have information such as
//container for individual particle information
//  std::vector<float> particlesInd;
//current particle position: px, py, pz
//current particle direction: dx, dy, dz
//particle rotation angle const: rx, ry, rz
//particle rotation angle variable: rx2, ry2, rz2
//particle color: pR, pG, pB
//particle shape: sphere or cube (0, 1)
//particle size: k
//particle age: n ms, -1 for persistent, change transparency
//(px, py, pz, dx, dy, dz, rx, ry, rz, pR, pG, pB, 0/1, k, n)

#ifndef __POINT_H__
	//guard against cyclic dependancy
#define __POINT_H__

#include <cstdlib>
#include "glheader.h"
#include <math.h>

class Point{
public:
	//Point();	//constructor

	public:
	float px;
	float py;
	float pz;
	float dx;
	float dy;
	float dz;
	float s;
	float pR;
	float pG;
	float pB;
	float nx; 
	float ny;
	float nz;

	Point operator +(const Point&); //PointA+PointB
	Point operator -(const Point&); //PointA-PointB
	Point operator *(const Point&); //PointA*PointB, cross-product
	Point operator *(const float);  //PointA*constant

	float sqrtMagnitude(); //sqrt of magnitude
	float magnitude(); //sqrt of dot-product
	Point normalized(); //normalize normal vector to unit normal vector

	Point();
	Point(float ax, float ay, float az, float bx, float by, float bz);
	~Point();
};

#endif


//added operators +, -, *. functions sqrtMagnitude, magnitude, normalized.
//allowed creation of Point with initial px, py, pz, nx, ny, nz 
