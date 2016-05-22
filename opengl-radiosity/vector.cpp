#include <math.h>
#include "vectormath.h"

vectormath::vector::vector()
{
	this->_x = 0;
	this->_y = 0;
	this->_z = 0;
	this->_length = 0;
}
vectormath::vector::vector(float x, float y, float z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_length = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
}

vectormath::vector * vectormath::crossproduct(vector * a, vector * b)
{
	vector * result = new vector();

	result->_x = a->_y * b->_z - a->_z * b->_y;
	result->_y = a->_z * b->_x - a->_x * b->_z;
	result->_z = a->_x * b->_y - a->_y * b->_x;

	return result;
}

vectormath::point::point()
{
	this->_x = 0.0;
	this->_y = 0.0;
	this->_z = 0.0;
}

vectormath::point::point(float x, float y, float z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

float vectormath::dotproduct(vector a, vector b)
{
	return a._x * b._x + a._y * b._y + a._z * b._z;
}

vectormath::vector * vectormath::add(point * a, vector * b)
{
	vector * c = new vector();

	c->_x = a->_x + b->_x;
	c->_y = a->_y + b->_y;
	c->_z = a->_z + b->_z;

	return c;
}

vectormath::point * vectormath::mid_point(vectormath::point * a, vectormath::point * b)
{
	vectormath::point mid_point;

	float mid_x = (a->_x + b->_x) / 2;
	float mid_y = (a->_y + b->_y) / 2;
	float mid_z = (a->_z + b->_z) / 2;

	return &mid_point;
}

vectormath::vector * vectormath::normalized_vector(vectormath::vector * a)
{
	vectormath::vector * normalizedVector;

	float magnitude = sqrt(pow(a->_x, 2) + pow(a->_y, 2) + pow(a->_z, 2));

	normalizedVector = new vectormath::vector(a->_x / magnitude, a->_y / magnitude, a->_z / magnitude);

	return normalizedVector;
}

vectormath::vector vectormath::operator-(const vectormath::point &p1, const vectormath::point &p2)
{
	return vectormath::vector(p1._x - p2._x, p1._y - p2._y, p1._z - p2._z);
}