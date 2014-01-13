#include "struct.h"
#include <iostream>
using namespace radiosity_structs;

light::light(float red, float green, float blue)
{
	this->_red = red;
	this->_green = green;
	this->_blue = blue;
}

patch::patch(point * center, Vector * normal, Vector * up, float r, float g, float b)
{
	this->_center = center;
	this->_normal = normal;
	this->_up = up;
	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(r, g, b);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
}

patch::patch()
{
	this->_center = new point(0.0, 0.0, 0.0);
	this->_normal = new Vector(0.0, 0.0, 1.0);
	this->_up = new Vector(0.0, 1.0, 0.0);
	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(1.0, 1.0, 1.0);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
	this->_vertices = new point[1];
}

patch::patch(point * center, Vector * normal, Vector * up, float r, float g, float b, point * vertices)
{
	this->_center = center;
	this->_normal = normal;
	this->_up = up;
	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(r, g, b);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
	this->_vertices = vertices;
}

hemicube::~hemicube()
{
	if(this->_left) delete this->_left;
	if(this->_right) delete this->_right;
	if(this->_top) delete this->_top;
	if(this->_bottom) delete this->_bottom;
	if(this->_front) delete this->_front;
}
