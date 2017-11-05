// Some security because more than one
// class implements this header
#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <math.h>

class Vector3f{

public:
	float x, y, z;

	Vector3f operator +(const Vector3f&);
	Vector3f operator -(const Vector3f&);
	Vector3f operator *(const Vector3f&);
	Vector3f operator *(const float);
	
	float sqrtMagnitude();
	float magnitude();
	Vector3f normalized();

	Vector3f();
	Vector3f(float x, float y, float z);
	~Vector3f();

};

#endif
