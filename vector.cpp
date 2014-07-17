#include <cmath>
#include "vector.h"
#include "struct.h"

using namespace vecmath;
using namespace entities;

Vector::Vector()
{
	this->_x = 0;
	this->_y = 0;
	this->_z = 0;
	this->_length = 0;
}
Vector::Vector(float x, float y, float z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_length = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
}

const Vector& Vector::operator^=(const Vector& other)
{
	this->_x = this->_y * other._z - this->_z * other._y;
	this->_y = this->_z * other._x - this->_x * other._z;
	this->_z = this->_x * other._y - this->_y * other._x;

	return *this;
}

const Vector& Point::operator+(const Vector& other) const
{
	Vector * c = new Vector();

	c->_x = this->_x + other._x;
	c->_y = this->_y + other._y;
	c->_z = this->_z + other._z;

	return *c;
}

const Vector Vector::operator^(const Vector& other) const
{
	Vector result = *this;
	result ^= other;
	return result;
}

std::ostream& operator<<(std::ostream& os, const Vector& vector)
{
	os << "(" << vector._x << ", " << vector._y << ", " << vector._z << ")";
	return os;
}

Vector * vecmath::crossproduct(Vector * a, Vector * b)
{
	Vector * result = new Vector();

	result->_x = a->_y * b->_z - a->_z * b->_y;
	result->_y = a->_z * b->_x - a->_x * b->_z;
	result->_z = a->_x * b->_y - a->_y * b->_x;

	return result;
}

Point::Point()
{
	this->_x = 0.0;
	this->_y = 0.0;
	this->_z = 0.0;
}

Point::Point(float x, float y, float z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

float vecmath::dotproduct(Vector a, Vector b)
{
	return a._x * b._x + a._y * b._y + a._z * b._z;
}

Vector * vecmath::add(Point * a, Vector * b)
{
	Vector * c = new Vector();

	c->_x = a->_x + b->_x;
	c->_y = a->_y + b->_y;
	c->_z = a->_z + b->_z;

	return c;
}

float vecmath::length(Vector * a)
{
	return sqrt(pow(a->_x, 2) + pow(a->_y, 2) + pow(a->_z, 2));
}

void vecmath::normalize(Vector * a)
{
	float length = vecmath::length(a);

	a->_x /= length;
	a->_y /= length;
	a->_z /= length;
}


const Vector Point::operator-(const Point& other) const
{
	Vector * c = new Vector(0.0,0.0,0.0);

	c->_x = this->_x - other._x;	
	c->_y = this->_y - other._y;	
	c->_z = this->_z - other._z;
	
	return *c;
}

std::ostream& operator<<(std::ostream& os, const Point& point)
{
	os << "(" << point._x << ", " << point._y << ", " << point._z << ")";
	return os;
}

Vector * vecmath::getVector(Point * a, Point * b)
{
	Vector * c = new Vector(0.0,0.0,0.0);

	c->_x = b->_x - a->_x;	
	c->_y = b->_y - a->_y;	
	c->_z = b->_z - a->_z;	 

	normalize(c);

	return c;	
}


