#ifndef __VECTOR3_H__
#define __VECTOR3_H__

class Vector3
{
public:
	union { // union of x,y,z to array v
		struct {
			float x, y, z;
		}; // anonymous struct
		float v[3]; // will be used for glcolor3fv(v);
	}; // anonymous union
	Vector3(float, float, float);
	Vector3(void);
	void randomize(void);
	Vector3 directionTo(Vector3);
	Vector3 scale(float);
	Vector3 add(Vector3);
	Vector3 add(float);
	Vector3 cross(Vector3);
	Vector3 normalize();
	float dot(Vector3);
	float length(void);
	float angle(Vector3);
};

#endif