#include "vector.h"

#ifndef _STRUCT_
#define _STRUCT_


namespace entities
{	
	class Light
	{
		public:
			float _red;
			float _green;
			float _blue;
			Light(float red, float green, float blue);

			const Light& operator+=(const Light& rhs )
			{
				_red += rhs._red;
				_green += rhs._green;
				_blue += rhs._blue;
				return *this;
			}
		
			const Light& operator-=(const Light& rhs )
			{
				_red -= rhs._red;
				_green -= rhs._green;
				_blue -= rhs._blue;
				return *this;
			}
	};
	
	/**
	 * Color is a normalized structure for rgb color
	 */
	class Color
	{
		public:
			float _red;
			float _green;
			float _blue;

			Color();
			Color(double*, double*, double*);
			Color(const Color&);
			Color(float*);
			~Color();

			const Color& operator+=(const Color& rhs );
			const Color& operator-=(const Color& rhs );
			friend std::ostream& operator<<(std::ostream& os, const Color& color);
			bool isBlack();
	};

	/**
	 * Flow of certain color through space at a certain intensity
	 */
	class Flux
	{
		private:
			Color * _color;
		public:
			Flux(double, double, double);
			Flux(const Color& color);
			Flux();

			float getRedFlux() const;
			float getGreenFlux() const;
			float getBlueFlux() const;
			void setRedFlux(float);
			void setGreenFlux(float);
			void setBlueFlux(float);
			void incrementFlux(float, float, float);
			friend std::ostream& operator<<(std::ostream& os, const Flux& flux);
			const Flux& operator+=(const Flux& other);
			const Flux& operator/=(int other);
	};
	class Patch
	{
		public:
			Color * _reflectance;
			vecmath::Point * _center;
			vecmath::Vector * _normal;
			vecmath::Vector * _up;
			int _numVerts;
			vecmath::Point * _vertices;
			Flux * _incident;
			Flux * _excident;
		
			Patch();
			Patch(vecmath::Point * center, vecmath::Vector * normal, vecmath::Vector * up, Color * reflectance);
			Patch(vecmath::Point * center, vecmath::Vector * normal, vecmath::Vector * up, Color * reflectance, vecmath::Point * vertices, int numVerts);
	};

	class LightPatch: public Patch
	{
		public:
			Color * _emission;
			LightPatch();
			LightPatch(vecmath::Point * center, vecmath::Vector * normal, vecmath::Vector * up, Color * reflectance, Color * emitter, vecmath::Point * vertices, int numVerts);	
	};

	struct Hemicube
	{
		unsigned char * _left;
		unsigned char * _right;
		unsigned char * _top;
		unsigned char * _bottom;
		unsigned char * _front;

		~Hemicube();
	};
}
#else
#endif
