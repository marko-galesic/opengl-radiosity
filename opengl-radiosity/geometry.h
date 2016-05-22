#pragma once
#include "radiosity_structs.h"
#include "vectormath.h"
#include <math.h>

namespace demo_geometry
{
	namespace quads
	{
		radiosity_structs::quad * divide_quad(radiosity_structs::quad * q, const int numberOfDivisions)
		{
			if (numberOfDivisions == 0) return q;
			
			vectormath::point * midPoint_Alpha = vectormath::mid_point(&(q->vertices[0]), &(q->vertices[1]));
			vectormath::point * midPoint_Beta = vectormath::mid_point(&(q->vertices[0]), &(q->vertices[2]));
			vectormath::point * midPoint_Epsilon = vectormath::mid_point(&(q->vertices[2]), &(q->vertices[3]));
			vectormath::point * midPoint_Pi = vectormath::mid_point(&(q->vertices[1]), &(q->vertices[3]));
			vectormath::point * midPoint_Theta = vectormath::mid_point(midPoint_Alpha, midPoint_Epsilon);

			radiosity_structs::quad subdivided_quads[] =
			{
				radiosity_structs::quad
				(
					&(q->vertices[0]),
					midPoint_Alpha,
					midPoint_Theta,
					midPoint_Beta
				),
				radiosity_structs::quad
				(
					midPoint_Alpha,
					&(q->vertices[1]),
					midPoint_Pi,
					midPoint_Theta
				),
				radiosity_structs::quad
				(
					midPoint_Beta,
					midPoint_Theta,
					midPoint_Epsilon,
					&(q->vertices[2])
				),
				radiosity_structs::quad
				(
					midPoint_Theta,
					midPoint_Pi,
					&(q->vertices[3]),
					midPoint_Epsilon
				)
			};

			if (numberOfDivisions == 1) {
				return subdivided_quads;
			}
			else {
				radiosity_structs::quad * quads_from_subdividedquads = new radiosity_structs::quad[(int)pow(4, numberOfDivisions)];
				radiosity_structs::quad * subdivided_quads_0 = divide_quad(&(subdivided_quads[0]), numberOfDivisions - 1);

				for (int sub_quad = 0; sub_quad < (int)pow(4, numberOfDivisions - 1); sub_quad++)
				{
					quads_from_subdividedquads[sub_quad] = subdivided_quads_0[sub_quad];
				}

				radiosity_structs::quad * subdivided_quads_1 = divide_quad(&(subdivided_quads[1]), numberOfDivisions - 1);

				for (int sub_quad = (int)pow(4, numberOfDivisions - 1); sub_quad < (int)pow(4, numberOfDivisions - 1) * 2; sub_quad++)
				{
					quads_from_subdividedquads[sub_quad] = subdivided_quads_1[sub_quad - (int)pow(4, numberOfDivisions - 1)];
				}

				radiosity_structs::quad * subdivided_quads_2 = divide_quad(&(subdivided_quads[2]), numberOfDivisions - 1);

				for (int sub_quad = (int)pow(4, numberOfDivisions - 1) * 2; sub_quad < (int)pow(4, numberOfDivisions - 1) * 3; sub_quad++)
				{
					quads_from_subdividedquads[sub_quad] = subdivided_quads_1[sub_quad - (int)pow(4, numberOfDivisions - 1) * 2];
				}

				radiosity_structs::quad * subdivided_quads_3 = divide_quad(&(subdivided_quads[3]), numberOfDivisions - 1);

				for (int sub_quad = (int)pow(4, numberOfDivisions - 1) * 3; sub_quad < (int)pow(4, numberOfDivisions); sub_quad++)
				{
					quads_from_subdividedquads[sub_quad] = subdivided_quads_1[sub_quad - (int)pow(4, numberOfDivisions - 1) * 3];
				}

				return quads_from_subdividedquads;
			}
		}

		radiosity_structs::quad skyPatchQuads[] = 
		{
			radiosity_structs::quad
			(
				&vectormath::point(1.0f, 1.0f, 1.0f),
				&vectormath::point(-1.0f, 1.0f, 1.0f),
				&vectormath::point(-1.0f, 1.0f, -1.0),
				&vectormath::point(1.0f, 1.0f, -1.0f)
			)
		};

		radiosity_structs::quad backPatchQuads[] =
		{
			radiosity_structs::quad
			(
				&vectormath::point(-1.0f, -1.0f, -1.0f),
				&vectormath::point(-1.0f, 1.0f, -1.0f),
				&vectormath::point(1.0f, 1.0f, -1.0f),
				&vectormath::point(1.0f, -1.0f, -1.0f)
			)
		};

		radiosity_structs::quad leftPatchQuads[] =
		{
			radiosity_structs::quad
			(
				&vectormath::point(-1.0f, -1.0f, 1.0f),
				&vectormath::point(-1.0f, 1.0f, 1.0f),
				&vectormath::point(-1.0f, 1.0f, -1.0f),
				&vectormath::point(-1.0f, -1.0f, -1.0f)
			)
		};

		radiosity_structs::quad rightPatchQuads[] =
		{
			radiosity_structs::quad
			(
				&vectormath::point(1.0f, 1.0f, 1.0f),
				&vectormath::point(1.0f, 1.0f, -1.0f),
				&vectormath::point(1.0f, -1.0f, -1.0f),
				&vectormath::point(1.0f, -1.0f, 1.0f)
			)
		};

		radiosity_structs::quad bottomPatchQuads[] =
		{
			radiosity_structs::quad
			(
				&vectormath::point(-1.0f, -1.0f, 1.0f),
				&vectormath::point(-1.0f, -1.0f, -1.0f),
				&vectormath::point(1.0f, -1.0f, -1.0f),
				&vectormath::point(1.0f, -1.0f, 1.0f)
			)
		};
	}

	radiosity_structs::patch * skyPatch = 
		new radiosity_structs::patch
		(
			new vectormath::point(0.0, 1.0, 0.0), 
			new vectormath::vector(0.0, -1.0, 0.0), 
			new vectormath::vector(0.0, 0.0, 1.0), 
			1.0, 1.0, 1.0,
			quads::divide_quad
			(
				quads::skyPatchQuads, 
				0
			),
			1,
			0.0, 1.0, 0.0
		);
	radiosity_structs::patch * backPatch = 
		new radiosity_structs::patch
		(
			new vectormath::point(0.0, 0.0, -1.0), 
			new vectormath::vector(0.0, 0.0, 1.0), 
			new vectormath::vector(0.0, 1.0, 0.0), 
			0.8, 0.8, 0.8,
			quads::divide_quad
			(
				quads::backPatchQuads, 
				1
			),
			4,
			1.0, 0.0, 0.0
		);
	radiosity_structs::patch * leftPatch = 
		new radiosity_structs::patch
		(
			new vectormath::point(-1.0, 0.0, 0.0), 
			new vectormath::vector(1.0, 0.0, 0.0), 
			new vectormath::vector(0.0, 1.0, 0.0), 
			1.0, 0.3, 0.3,
			quads::divide_quad
			(
				quads::leftPatchQuads, 
				0
			),
			1,
			0.0, 1.0, 0.0
		);
	radiosity_structs::patch * rightPatch = 
		new radiosity_structs::patch
		(
			new vectormath::point(1.0, 0.0, 0.0), 
			new vectormath::vector(-1.0, 0.0, 0.0), 
			new vectormath::vector(0.0, 1.0, 0.0), 
			0.3, 1.0, 0.3,
			quads::divide_quad
			(
				quads::rightPatchQuads, 
				0
			),
			1,
			0.0, 1.0, 0.0
		);
	radiosity_structs::patch * bottomPatch = 
		new radiosity_structs::patch
		(
			new vectormath::point(0.0, -1.0, 0.0), 
			new vectormath::vector(0.0, 1.0, 0.0), 
			new vectormath::vector(0.0, 0.0, -1.0), 
			0.8, 0.8, 0.8,
			quads::divide_quad
			(
				quads::bottomPatchQuads, 
				0
			),
			1,
			0.0, 1.0, 0.0
		);
}