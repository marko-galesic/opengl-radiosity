#include "struct.h"

using namespace vecmath;
using namespace entities;

Color::Color()
{
	this->_red = 0.0;
	this->_green = 0.0;
	this->_blue = 0.0;
}

Color::Color(const Color& color)
{
	this->_red = color._red;
	this->_green = color._green;
	this->_blue = color._blue;
}

Color::Color(float * color)
{
	this->_red = color[0];
	this->_green = color[1];
	this->_blue = color[2];
}

Color::Color(double * red, double * green, double * blue)
{
	this->_red = *red;
	this->_green = *green;
	this->_blue = *blue;
}

const Color& Color::operator+=(const Color& rhs )
{
	_red += rhs._red;
	_green += rhs._green;
	_blue += rhs._blue;
	return *this;
}

const Color& Color::operator-=(const Color& rhs )
{
	_red -= rhs._red;
	_green -= rhs._green;
	_blue -= rhs._blue;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Color& color)
{
    os << "(" << color._red << ", " << color._green << ", " << color._blue << ")";
    return os;
}

bool Color::isBlack()
{
	return (this->_red + this->_green + this->_blue) == 0.0;
}

Light::Light(float red, float green, float blue)
{
	this->_red = red;
	this->_green = green;
	this->_blue = blue;
}

Patch::Patch()
{
	this->_center = new Point();
	this->_normal = new Vector(0.0, 1.0, 0.0);
	this->_up = new Vector(1.0, 0.0, 0.0);
	this->_reflectance = new Color();
	this->_vertices = NULL;
	this->_numVerts = 0;
	this->_incident = new Flux();
	this->_excident = new Flux();
}
Patch::Patch(Point * center, Vector * normal, Vector * up, Color * reflectance)
{
	this->_center = center;
	this->_normal = normal;
	this->_up = up;
	this->_reflectance = reflectance;
	this->_incident = new Flux();
	this->_excident = new Flux();
}
Patch::Patch(Point * center, Vector * normal, Vector * up, Color * reflectance, Point * vertices, int numVerts)
{
	this->_center = new Point();
	this->_normal = new Vector(0.0, 1.0, 0.0);
	this->_up = new Vector(1.0, 0.0, 0.0);
	this->_reflectance = new Color();
	this->_vertices = NULL;
	this->_numVerts = 0;
	this->_incident = new Flux();
	this->_excident = new Flux();
}

LightPatch::LightPatch()
{
	this->_center = new Point();
	this->_normal = new Vector(0.0, 1.0, 0.0);
	this->_up = new Vector(1.0, 0.0, 0.0);
	this->_reflectance = new Color();
	this->_vertices = NULL;
	this->_numVerts = 0;
	this->_emission = new Color();
	this->_incident = new Flux();
	this->_excident = new Flux();
}

LightPatch::LightPatch(Point * center, Vector * normal, Vector * up, Color * reflectance, Color * emitter, Point * vertices, int numVerts)
{
	this->_center = center;
	this->_normal = normal;
	this->_up = up;
	this->_reflectance = reflectance;
	this->_vertices = vertices;
	this->_numVerts = numVerts;
	this->_emission = emitter;
	this->_incident = new Flux();
	this->_excident = new Flux();
}

Hemicube::~Hemicube()
{
	if(this->_left) delete this->_left;
	if(this->_right) delete this->_right;
	if(this->_top) delete this->_top;
	if(this->_bottom) delete this->_bottom;
	if(this->_front) delete this->_front;
}
