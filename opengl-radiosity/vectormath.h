#pragma once
namespace vectormath
{
	struct vector
	{
		float _x;
		float _y;
		float _z;
		float _length;

		vector();
		vector(float x, float y, float z);
	};

	struct point
	{
		float _x;
		float _y;
		float _z;

		point();
		point(float x, float y, float z);

		friend vector operator-(const point &v1, const point &v2);
	};

	vector * crossproduct(vector*, vector*);
	float dotproduct(vector a, vector b);
	vector * add(point * a, vector * b);
	point * mid_point(point * a, point * b);
	vector * normalized_vector(vector * v);
}