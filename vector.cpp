#include <cmath>
#include "vector.h"

using namespace Vectormath;

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

Vector * Vectormath::crossproduct(Vector * a, Vector * b)
{
	Vector * result = new Vector();

	result->_x = a->_y * b->_z - a->_z * b->_y;
	result->_y = a->_z * b->_x - a->_x * b->_z;
	result->_z = a->_x * b->_y - a->_y * b->_x;

	return result;
}

point::point()
{
	this->_x = 0.0;
	this->_y = 0.0;
	this->_z = 0.0;
}

point::point(float x, float y, float z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

float Vectormath::dotproduct(Vector a, Vector b)
{
	return a._x * b._x + a._y * b._y + a._z * b._z;
}

Vector * Vectormath::add(point * a, Vector * b)
{
	Vector * c = new Vector();

	c->_x = a->_x + b->_x;
	c->_y = a->_y + b->_y;
	c->_z = a->_z + b->_z;

	return c;
}
