#include "calculations.h"
#include "stdafx.h"

float randFloat(float s, float t)
{
	return s + (t - s) * ((float)rand()) / (float)RAND_MAX;
}

float absf(float value)
{
	return (value >= 0) ? value : -value;
}

Vector3 crossproduct(Vector3 a, Vector3 b){
	Vector3 normal;
	normal.x = a.y*b.z - a.z*b.y;
	normal.y = a.z*b.x - a.x*b.z;
	normal.z = a.x*b.y - a.y*b.x;
	return normal;
}

float dotproduct(Vector3 a, Vector3 b){
	float dot = a.x*b.x + a.y*b.y + a.z + b.z;
	return dot;
}

float magnitude(Vector3 a){
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}