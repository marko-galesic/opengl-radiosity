#pragma once
#include "vectormath.h"

namespace radiosity_structs
{
	struct light
	{
		float _red;
		float _green;
		float _blue;
		light(float red, float green, float blue);
	};
	
	struct quad
	{
		vectormath::point vertices[4];
		vectormath::point * _center;
		vectormath::vector * _normal;
		vectormath::vector * _up;

		light * _emission;
		light * _reflectance;
		light * _incident;
		light * _excident;

		quad();
		quad(vectormath::point * v0, vectormath::point * v1, vectormath::point * v2, vectormath::point * v3);
		quad(vectormath::point * v0, vectormath::point * v1, vectormath::point * v2, vectormath::point * v3, float r, float g, float b, float wireframe_r, float wireframe_g, float wireframe_b);

		vectormath::point * center_of_quad(radiosity_structs::quad * q);
		vectormath::vector * normal_of_quad(radiosity_structs::quad * q);
		vectormath::vector * up_vector_quad(radiosity_structs::quad * q);
	};

	struct patch
	{
		light * _emission;
		light * _reflectance;
		light * _incident;
		light * _excident;

		vectormath::point * _center;
		vectormath::vector * _normal;
		vectormath::vector * _up;
		vectormath::point * vertices;
		quad * quads;

		int _numberOfQuads;

		float _wireframe_r;
		float _wireframe_g;
		float _wireframe_b;

		patch(vectormath::point * center, vectormath::vector * normal, vectormath::vector * up, float r, float g, float b);
		patch(vectormath::point * center, vectormath::vector * normal, vectormath::vector * up, float r, float g, float b, quad * q, int numberOfQuads, float wireframe_r, float wireframe_g, float wireframe_b);
		patch(float r, float g, float b, quad * q, int numberOfQuads, float wireframe_r, float wireframe_g, float wireframe_b);
		patch(vectormath::vector * normal, vectormath::vector * up, float r, float g, float b, quad * q, int numberOfQuads, float wireframe_r, float wireframe_g, float wireframe_b);
		patch(vectormath::point * center, vectormath::vector * normal, vectormath::vector * up, float r, float g, float b, vectormath::point * vertices);
	};

	struct hemicube
	{
		unsigned char * _left;
		unsigned char * _right;
		unsigned char * _top;
		unsigned char * _bottom;
		unsigned char * _front;
	};
}