#include "struct.h"


using namespace radiosity_structs;

light::light(float red, float green, float blue)
{
	this->_red = red;
	this->_green = green;
	this->_blue = blue;
}

patch::patch(point * center, vector * normal, vector * up, float r, float g, float b)
{
	this->_center = center;
	this->_normal = normal;
	this->_up = up;
	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(r, g, b);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
}

patch::patch(point * center, vector * normal, vector * up, float r, float g, float b, point * vertices)
{
	this->_center = center;
	this->_normal = normal;
	this->_up = up;
	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(r, g, b);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
	this->vertices = vertices;
}
