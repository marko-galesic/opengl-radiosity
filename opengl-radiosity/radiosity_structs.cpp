#include "vectormath.h"
#include "radiosity_structs.h"

radiosity_structs::quad::quad()
{
	this->vertices[0] = vectormath::point(0, 0, 0);
	this->vertices[1] = vectormath::point(0, 0, 0);
	this->vertices[2] = vectormath::point(0, 0, 0);
	this->vertices[3] = vectormath::point(0, 0, 0);
}

radiosity_structs::light::light(float red, float green, float blue)
{
	this->_red = red;
	this->_green = green;
	this->_blue = blue;
}

radiosity_structs::patch::patch(vectormath::point * center, vectormath::vector * normal, vectormath::vector * up, float r, float g, float b)
{
	this->_center = center;
	this->_normal = normal;
	this->_up = up;
	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(r, g, b);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
}

radiosity_structs::patch::patch(vectormath::point * center, vectormath::vector * normal, vectormath::vector * up, float r, float g, float b, vectormath::point * vertices)
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

radiosity_structs::patch::patch(vectormath::point * center, vectormath::vector * normal, vectormath::vector * up, float r, float g, float b, radiosity_structs::quad * quads, int numberOfQuads, float wireframe_r, float wireframe_g, float wireframe_b)
{
	this->_center = center;
	this->_normal = normal;
	this->_up = up;
	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(r, g, b);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
	this->quads = quads;
	this->_numberOfQuads = numberOfQuads;
	this->_wireframe_r = wireframe_r;
	this->_wireframe_g = wireframe_g;
	this->_wireframe_b = wireframe_b;
}

radiosity_structs::patch::patch(float r, float g, float b, radiosity_structs::quad * quads, int numberOfQuads, float wireframe_r, float wireframe_g, float wireframe_b)
{
	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(r, g, b);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
	this->quads = quads;
	this->_numberOfQuads = numberOfQuads;
	this->_wireframe_r = wireframe_r;
	this->_wireframe_g = wireframe_g;
	this->_wireframe_b = wireframe_b;
}

radiosity_structs::quad::quad(vectormath::point * v0, vectormath::point * v1, vectormath::point * v2, vectormath::point * v3)
{
	this->vertices[0] = *v0;
	this->vertices[1] = *v1;
	this->vertices[2] = *v2;
	this->vertices[3] = *v3;

	this->_center = center_of_quad(this);
	this->_normal = vectormath::normalized_vector(normal_of_quad(this));
	this->_up = vectormath::normalized_vector(up_vector_quad(this));
}

radiosity_structs::quad::quad(vectormath::point * v0, vectormath::point * v1, vectormath::point * v2, vectormath::point * v3, float r, float g, float b, float wireframe_r, float wireframe_g, float wireframe_b)
{
	this->vertices[0] = *v0;
	this->vertices[1] = *v1;
	this->vertices[2] = *v2;
	this->vertices[3] = *v3;

	this->_center = center_of_quad(this);
	this->_normal = vectormath::normalized_vector(normal_of_quad(this));
	this->_up = vectormath::normalized_vector(up_vector_quad(this));

	this->_emission = new light(0.0, 0.0, 0.0);
	this->_reflectance = new light(r, g, b);
	this->_incident = new light(0.0, 0.0, 0.0);
	this->_excident = new light(0.0, 0.0, 0.0);
}

// https://en.wikipedia.org/wiki/Centroid
vectormath::point * radiosity_structs::quad::center_of_quad(radiosity_structs::quad * q)
{
	float centroid_x = (q->vertices[0]._x + q->vertices[1]._x + q->vertices[2]._x + q->vertices[3]._x) / 4;
	float centroid_y = (q->vertices[0]._y + q->vertices[1]._y + q->vertices[2]._y + q->vertices[3]._y) / 4;
	float centroid_z = (q->vertices[0]._z + q->vertices[1]._z + q->vertices[2]._z + q->vertices[3]._z) / 4;

	return new vectormath::point(centroid_x, centroid_y, centroid_z);
}

vectormath::vector * radiosity_structs::quad::normal_of_quad(radiosity_structs::quad * q)
{
	vectormath::vector * normal = vectormath::crossproduct(&(q->vertices[1] - q->vertices[0]), &(q->vertices[2] - q->vertices[0]));

	return normal;
}

vectormath::vector * radiosity_structs::quad::up_vector_quad(radiosity_structs::quad * q)
{
	vectormath::vector * up = vectormath::crossproduct(q->_normal, &(q->vertices[3] - q->vertices[2]));
	return up;
}