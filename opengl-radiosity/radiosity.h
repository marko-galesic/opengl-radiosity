#pragma once
#include "radiosity_structs.h"

namespace radiosity
{
	enum HEMICUBE_VIEW { LEFT, RIGHT, TOP, BOTTOM, FRONT };

	void initialize_patches();

	void enter_patch_quads(radiosity_structs::patch * p);
	void enter_patch_wireframe_color(radiosity_structs::patch * p);
	void enter_patch_quads_wireframe_overlay(radiosity_structs::patch * p);
	void enter_patch_color(radiosity_structs::patch * p);
	void enter_patch(radiosity_structs::patch * p);

	void render_scene();
	void render_hemicube_view(vectormath::point * center, vectormath::vector * up, vectormath::vector * direction, HEMICUBE_VIEW view);

	unsigned char * get_hemicube_pixels(HEMICUBE_VIEW view);

	unsigned char * apply_multiplier(unsigned char * buffer, HEMICUBE_VIEW view);

	radiosity_structs::hemicube * apply_multiplier_hemicube(radiosity_structs::hemicube * h);

	radiosity_structs::light * get_total_light_of_view(radiosity_structs::hemicube * h, HEMICUBE_VIEW view);

	radiosity_structs::light * calculaute_incident_light(vectormath::point * center, vectormath::vector * normal, vectormath::vector * up);

	void render_patch(radiosity_structs::patch * p);

	void calculate_excident(radiosity_structs::patch * p);

	void check_whether_patch_has_component_max(radiosity_structs::patch * p, radiosity_structs::light * totalLight);

	void radiosity();
}