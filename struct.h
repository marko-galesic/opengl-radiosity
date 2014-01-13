#include "vector.h"

using namespace Vectormath;

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
		Vector * _normal;
		Vector * _up;
		point * _vertices;
		int numVerts;
		
		patch();
		patch(point * center, Vector * normal, Vector * up, float r, float g, float b);
		patch(point * center, Vector * normal, Vector * up, float r, float g, float b, point * vertices);
	};

	struct hemicube
	{
		unsigned char * _left;
		unsigned char * _right;
		unsigned char * _top;
		unsigned char * _bottom;
		unsigned char * _front;

		~hemicube();
	};
}
