#include <cmath>

namespace Vectormath
{
	struct Vector
	{
		float _x;
		float _y;
		float _z;
		float _length;

		Vector();
		Vector(float x, float y, float z);
	};
	
	struct point
	{
		float _x;
		float _y;
		float _z;

		point();
		point(float x, float y, float z);
	};
	Vector * crossproduct(Vector*, Vector*);
	Vector * getVector(point * a, point * b);
	float dotproduct(Vector a, Vector b);
	Vector * add(point * a, Vector * b);
	void normalize(Vector * a);
	float length(Vector * a);
}
