#include "point.h"
#include <stdio.h>
#include<string>
//using vector
#include <vector>
using namespace std;

Point::Point(){	//constructor
	px = 0;
	py = 0;
	pz = 0;
	dz = 0;
	dy = 0;
	dz = 0;
	s = 1;
	pR = 0;
	pG = 0;
	pB = 0;
	nx = 0;
	ny = 0;
	nz = 0;
}

/* Operator overloads for code convenience */
Point Point::operator +(const Point& other){
	Point result(px + other.px, py + other.py, pz + other.pz, 0, 0, 0);
	return result;
}

Point Point::operator -(const Point& other){
	Point result(px - other.px, py - other.py, pz - other.pz, 0, 0, 0);
	return result;
}

Point Point::operator *(const Point& other){
	float rx = (py * other.pz) - (pz * other.py);
	float ry = (pz * other.px) - (px * other.pz);
	float rz = (px * other.py) - (py * other.px);

	Point result(0, 0, 0, rx, ry, rz);
	return result;
}

Point Point::operator *(const float factor){
	Point result(px * factor, py * factor, pz * factor, 0, 0, 0);
	return result;
}


/* Class main methods */
float Point::sqrtMagnitude(){
	float mag = sqrt((nx * nx) + (ny * ny) + (nz * nz));
	return mag * mag;
}

float Point::magnitude(){
	return sqrt((nx * nx) + (ny * ny) + (nz * nz));
}

Point Point::normalized(){
	float ax = nx / magnitude();
	float by = ny / magnitude();
	float cz = nz / magnitude();
	Point result(0, 0, 0, ax, by, cz);
	return result;
}

/* Constructor and destructor methods */

Point::Point(float ax, float ay, float az, float bx, float by, float bz){
	px = ax;
	py = ay;
	pz = az;
	nx = bx;
	ny = by;
	nz = bz;
}

Point::~Point(){}