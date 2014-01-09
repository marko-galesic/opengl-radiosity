#include "vector.h"

using namespace vectormath;

namespace radiosity_structs
{
	struct light
	{
		float _red;
		float _green;
		float _blue;
		light(float red, float green, float blue);
	};

	struct patch
	{
		light * _emission;
		light * _reflectance;
		light * _incident;
		light * _excident;
		point * _center;
		vector * _normal;
		vector * _up;
		point * vertices;

		patch(point * center, vector * normal, vector * up, float r, float g, float b);
		patch(point * center, vector * normal, vector * up, float r, float g, float b, point * vertices);
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
