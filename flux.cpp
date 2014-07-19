#include "struct.h"

using namespace entities;

Flux::Flux()
{
	this->_color = new Color();
}

Flux::Flux(const Color& color)
{
	this->_color = new Color(color);
}

Flux::Flux(double red, double green, double blue)
{
	this->_color = new Color(&red, &green, &blue);
}

float Flux::getRedFlux() const
{
	return this->_color->_red;
}

float Flux::getGreenFlux() const
{
	return this->_color->_green;
}

float Flux::getBlueFlux() const
{
	return this->_color->_blue;
}

void Flux::setRedFlux(float red)
{
	this->_color->_red = red;
}

void Flux::setGreenFlux(float green)
{
	this->_color->_green = green;
}

void Flux::setBlueFlux(float blue)
{
	this->_color->_blue = blue;
}

void Flux::incrementFlux(float red, float green, float blue)
{
	this->_color->_red += red;
	this->_color->_green += green;
	this->_color->_blue += blue;
}

std::ostream& operator<<(std::ostream& os, const Flux& flux)
{
	Flux * temp = new Flux(flux);
	os << "(" << temp->getRedFlux() << ", " << temp->getGreenFlux() << ", " << temp->getGreenFlux() << ")";
    	return os;
}

const Flux& Flux::operator+=(const Flux& other)
{
	this->incrementFlux(other.getRedFlux(), other.getGreenFlux(), other.getBlueFlux());
	return *this;
}

const Flux& Flux::operator/=(int other)
{
	this->setRedFlux(this->getRedFlux() / (float)other);
	this->setGreenFlux(this->getGreenFlux() / (float)other);
	this->setBlueFlux(this->getBlueFlux() / (float)other);

	return *this;
}
