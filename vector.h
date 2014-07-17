#ifndef _VECTOR_
#define _VECTOR_
#include <iostream>

namespace vecmath
{
	struct Vector
	{	

		float _x;
		float _y;
		float _z;
		float _length;

		Vector();
		Vector(float x, float y, float z);
		const Vector operator^(const Vector& other) const;
		const Vector& operator^=(const Vector& rhs);
		friend std::ostream& operator<<(std::ostream& os, const Vector& vector);
	};
	
	struct Point
	{
		float _x;
		float _y;
		float _z;

		Point();
		Point(float x, float y, float z);
		const Point operator^(const Point& other) const;
		const Vector& operator+(const Vector& other) const;
		const Vector operator-(const Point& other) const;
		const Point& operator^=(const Point& other);
		friend std::ostream& operator<<(std::ostream& os, const Point& point);
	};
	Vector * crossproduct(Vector*, Vector*);
	Vector * getVector(Point * a, Point * b);
	float dotproduct(Vector a, Vector b);
	Vector * add(Point * a, Vector * b);
	void normalize(Vector * a);
	float length(Vector * a);
}
#else
#endif
