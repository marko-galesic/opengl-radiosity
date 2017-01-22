#pragma once
#include "radiosity_structs.h"
#include "vectormath.h"
#include <math.h>

namespace demo_geometry
{
	radiosity_structs::quad * divide_quad(radiosity_structs::quad * q, const int numberOfDivisions);

	extern radiosity_structs::patch * skyPatch;
	extern radiosity_structs::patch * backPatch;
	extern radiosity_structs::patch * leftPatch;
	extern radiosity_structs::patch * rightPatch;
	extern radiosity_structs::patch * bottomPatch;
}