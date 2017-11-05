#include "vector3f.h"

/* Operator overloads for code convenience */
Vector3f Vector3f::operator +(const Vector3f& other){
	Vector3f result(x + other.x, y + other.y, z + other.z);
	return result;
}

Vector3f Vector3f::operator -(const Vector3f& other){
	Vector3f result(x - other.x, y - other.y, z - other.z);
	return result;
}

Vector3f Vector3f::operator *(const Vector3f& other){
	float rx = (y * other.z) - (z * other.y);
	float ry = (z * other.x) - (x * other.z);
	float rz = (x * other.y) - (y * other.x);

	Vector3f result(rx, ry, rz);
	return result;
}

Vector3f Vector3f::operator *(const float factor){
	Vector3f result(x * factor, y * factor, z * factor);
	return result;
}


/* Class main methods */
float Vector3f::sqrtMagnitude(){
	float mag = sqrt((x * x) + (y * y) + (z * z));
	return mag * mag;
}

float Vector3f::magnitude(){
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3f Vector3f::normalized(){
	float nx = x / magnitude();
	float ny = y / magnitude();
	float nz = z / magnitude();
	Vector3f result(nx, ny, nz);
	return result;
}

/* Constructor and destructor methods */
Vector3f::Vector3f(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3f::Vector3f(float ax, float ay, float az){
	x = ax;
	y = ay;
	z = az;
}

Vector3f::~Vector3f(){}

