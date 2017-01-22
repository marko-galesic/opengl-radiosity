#include <GL/freeglut.h>
#include <iostream>
#include "radiosity_application_constants.h"
#include "radiosity.h"
#include "geometry.h"
#include "radiosity_structs.h"

void radiosity::initialize_patches()
{
	using demo_geometry::skyPatch;
	using demo_geometry::bottomPatch;
	using demo_geometry::leftPatch;
	using demo_geometry::rightPatch;
	using demo_geometry::backPatch;

	skyPatch->_emission = new radiosity_structs::light(1.0, 1.0, 1.0);
	skyPatch->_excident = demo_geometry::skyPatch->_emission;
	bottomPatch->_excident = bottomPatch->_emission;
	leftPatch->_excident = leftPatch->_emission;
	rightPatch->_excident = rightPatch->_emission;
	backPatch->_excident = backPatch->_emission;
}

void radiosity::enter_patch_quads(radiosity_structs::patch * p)
{
	for (int quad = 0; quad < p->_numberOfQuads; quad++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < 4; v++)
		{
			glVertex3f
				(
				p->quads[quad].vertices[v]._x,
				p->quads[quad].vertices[v]._y,
				p->quads[quad].vertices[v]._z
				);
		}
		glEnd();
	}
}

void radiosity::enter_patch_wireframe_color(radiosity_structs::patch * p)
{
	glColor3f(p->_wireframe_r, p->_wireframe_g, p->_wireframe_b);
}

void radiosity::enter_patch_quads_wireframe_overlay(radiosity_structs::patch * p)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(4);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1, -1);

	radiosity::enter_patch_wireframe_color(p);
	radiosity::enter_patch_quads(p);

	glDisable(GL_POLYGON_OFFSET_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void radiosity::enter_patch_color(radiosity_structs::patch * p)
{
	glColor3f(p->_excident->_red, p->_excident->_green, p->_excident->_blue);
}

void radiosity::enter_patch(radiosity_structs::patch * p)
{
	radiosity::enter_patch_quads_wireframe_overlay(p);

	radiosity::enter_patch_color(p);
	radiosity::enter_patch_quads(p);
}

void radiosity::render_scene()
{
	using demo_geometry::bottomPatch;
	using demo_geometry::backPatch;
	using demo_geometry::leftPatch;
	using demo_geometry::rightPatch;
	using demo_geometry::skyPatch;

	using radiosity::enter_patch;

	// Bottom patch
	enter_patch(bottomPatch);

	// Back patch
	enter_patch(backPatch);

	// Left patch
	enter_patch(leftPatch);

	// Right patch
	enter_patch(rightPatch);

	// Sky patch
	enter_patch(skyPatch);
}

void radiosity::render_hemicube_view(vectormath::point * center, vectormath::vector * up, vectormath::vector * direction, radiosity::HEMICUBE_VIEW view)
{
	using radiosity_application_constants::WINDOW_WIDTH;
	using radiosity_application_constants::WINDOW_HEIGHT;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch (view)
	{
	case radiosity::HEMICUBE_VIEW::FRONT:
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 5.0);
		break;
	case radiosity::HEMICUBE_VIEW::TOP:
		glFrustum(-1.0, 1.0, -1.0, 0.0, 1.0, 5.0);
		break;
	case radiosity::HEMICUBE_VIEW::LEFT:
		glFrustum(0.0, 1.0, -1.0, 1.0, 1.0, 5.0);
		break;
	case radiosity::HEMICUBE_VIEW::RIGHT:
		glFrustum(-1.0, 0.0, -1.0, 1.0, 1.0, 5.0);
		break;
	case radiosity::HEMICUBE_VIEW::BOTTOM:
		glFrustum(-1.0, 1.0, 0.0, 1.0, 1.0, 5.0);
		break;
	}

	glMatrixMode(GL_MODELVIEW);

	switch (view)
	{
	case FRONT:
		glViewport(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		break;
	case TOP:
		glViewport(WINDOW_WIDTH / 4, WINDOW_HEIGHT * 3 / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4);
		break;
	case LEFT:
		glViewport(0, WINDOW_HEIGHT * 1 / 4, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2);
		break;
	case RIGHT:
		glViewport(WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 1 / 4, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2);
		break;
	case BOTTOM:
		glViewport(WINDOW_WIDTH / 4, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4);
		break;
	}

	glLoadIdentity();
	gluLookAt(center->_x, center->_y, center->_z, direction->_x, direction->_y, direction->_z, up->_x, up->_y, up->_z);
	radiosity::render_scene();
}

unsigned char * radiosity::get_hemicube_pixels(radiosity::HEMICUBE_VIEW view)
{
	using radiosity_application_constants::WINDOW_WIDTH;
	using radiosity_application_constants::WINDOW_HEIGHT;

	float 	WIDTH = 0,
			HEIGHT = 0,
			VIEW_X = 0,
			VIEW_Y = 0,
			PIXEL_HEIGHT_IN_DEGREES = 0,
			PIXEL_WIDTH_IN_DEGREES = 0,
			START_ANGLE_PX_CAMERA_X = 0,
			START_ANGLE_PX_CAMERA_Y = 0,
			START_ANGLE_PX_PATCH_X = 0,
			START_ANGLE_PX_PATCH_Y = 0;
	const char * viewName;
	unsigned char * buffer;
	switch (view)
	{
	case radiosity::HEMICUBE_VIEW::FRONT:
		viewName = "FRONT";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 2;
		VIEW_X = WINDOW_WIDTH / 4;
		VIEW_Y = WINDOW_HEIGHT / 4;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / (HEIGHT / 2.0);
		PIXEL_WIDTH_IN_DEGREES = 45.0 / (WIDTH / 2.0);
		START_ANGLE_PX_CAMERA_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	case radiosity::HEMICUBE_VIEW::TOP:
		viewName = "TOP";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 4;
		VIEW_X = WINDOW_WIDTH / 4;
		VIEW_Y = WINDOW_HEIGHT * 3 / 4;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / HEIGHT;
		PIXEL_WIDTH_IN_DEGREES = 45.0 / (WIDTH / 2.0);
		START_ANGLE_PX_CAMERA_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 45.0 + PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	case radiosity::HEMICUBE_VIEW::LEFT:
		viewName = "LEFT";
		WIDTH = WINDOW_WIDTH / 4;
		HEIGHT = WINDOW_HEIGHT / 2;
		VIEW_X = 0;
		VIEW_Y = WINDOW_HEIGHT * 1 / 4;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / (HEIGHT / 2.0);
		PIXEL_WIDTH_IN_DEGREES = 45.0 / WIDTH;
		START_ANGLE_PX_CAMERA_X = PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 90.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	case radiosity::HEMICUBE_VIEW::RIGHT:
		viewName = "RIGHT";
		WIDTH = WINDOW_WIDTH / 4;
		HEIGHT = WINDOW_HEIGHT / 2;
		VIEW_X = WINDOW_WIDTH * 3 / 4;
		VIEW_Y = WINDOW_HEIGHT * 1 / 4;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / (HEIGHT / 2.0);
		PIXEL_WIDTH_IN_DEGREES = 45.0 / WIDTH;
		START_ANGLE_PX_CAMERA_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 45.0 + PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	case radiosity::HEMICUBE_VIEW::BOTTOM:
		viewName = "BOTTOM";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 4;
		VIEW_X = WINDOW_WIDTH / 4;
		VIEW_Y = 0;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / HEIGHT;
		PIXEL_WIDTH_IN_DEGREES = 45.0 / (WIDTH / 2.0);
		START_ANGLE_PX_CAMERA_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 90.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	}
	buffer = (unsigned char *)malloc(WIDTH * HEIGHT * 3);

	glReadPixels(VIEW_X, VIEW_Y, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	return buffer;
}

unsigned char * radiosity::apply_multiplier(unsigned char * buffer, radiosity::HEMICUBE_VIEW view)
{
	using radiosity_application_constants::WINDOW_WIDTH;
	using radiosity_application_constants::WINDOW_HEIGHT;

	using radiosity::HEMICUBE_VIEW;

	float 	WIDTH = 0,
		HEIGHT = 0,
		VIEW_X = 0,
		VIEW_Y = 0,
		PIXEL_HEIGHT_IN_DEGREES = 0,
		PIXEL_WIDTH_IN_DEGREES = 0,
		START_ANGLE_PX_CAMERA_X = 0,
		START_ANGLE_PX_CAMERA_Y = 0,
		START_ANGLE_PX_PATCH_X = 0,
		START_ANGLE_PX_PATCH_Y = 0;
	const char * viewName;
	switch (view)
	{
	case FRONT:
		viewName = "FRONT";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 2;
		VIEW_X = WINDOW_WIDTH / 4;
		VIEW_Y = WINDOW_HEIGHT / 4;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / (HEIGHT / 2.0);
		PIXEL_WIDTH_IN_DEGREES = 45.0 / (WIDTH / 2.0);
		START_ANGLE_PX_CAMERA_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	case TOP:
		viewName = "TOP";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 4;
		VIEW_X = WINDOW_WIDTH / 4;
		VIEW_Y = WINDOW_HEIGHT * 3 / 4;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / HEIGHT;
		PIXEL_WIDTH_IN_DEGREES = 45.0 / (WIDTH / 2.0);
		START_ANGLE_PX_CAMERA_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 45.0 + PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	case LEFT:
		viewName = "LEFT";
		WIDTH = WINDOW_WIDTH / 4;
		HEIGHT = WINDOW_HEIGHT / 2;
		VIEW_X = 0;
		VIEW_Y = WINDOW_HEIGHT * 1 / 4;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / (HEIGHT / 2.0);
		PIXEL_WIDTH_IN_DEGREES = 45.0 / WIDTH;
		START_ANGLE_PX_CAMERA_X = PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 90.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	case RIGHT:
		viewName = "RIGHT";
		WIDTH = WINDOW_WIDTH / 4;
		HEIGHT = WINDOW_HEIGHT / 2;
		VIEW_X = WINDOW_WIDTH * 3 / 4;
		VIEW_Y = WINDOW_HEIGHT * 1 / 4;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / (HEIGHT / 2.0);
		PIXEL_WIDTH_IN_DEGREES = 45.0 / WIDTH;
		START_ANGLE_PX_CAMERA_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 45.0 + PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 45.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	case BOTTOM:
		viewName = "BOTTOM";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 4;
		VIEW_X = WINDOW_WIDTH / 4;
		VIEW_Y = 0;
		PIXEL_HEIGHT_IN_DEGREES = 45.0 / HEIGHT;
		PIXEL_WIDTH_IN_DEGREES = 45.0 / (WIDTH / 2.0);
		START_ANGLE_PX_CAMERA_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_CAMERA_Y = PIXEL_HEIGHT_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_X = 45.0 - PIXEL_WIDTH_IN_DEGREES / 2.0;
		START_ANGLE_PX_PATCH_Y = 90.0 - PIXEL_HEIGHT_IN_DEGREES / 2.0;
		break;
	}

	/*
	* Apply "Mulitiplier Hemicube"
	*/
	float yawPxCamera = START_ANGLE_PX_CAMERA_X;	// Yaw (in degrees) between the current pixel's center and the direction of the camera
	float pitchPxCamera = START_ANGLE_PX_CAMERA_Y;	// Pitch (in degrees) between the current pixel's center and the direction of the camera

	float yawPxPatch = START_ANGLE_PX_PATCH_X;		// Yaw (in degrees) between the current pixel's center and the normal of the patch
	float pitchPxPatch = START_ANGLE_PX_PATCH_Y;		// Pitch (in degrees) between the current pixel's center and the normal of the patch

	float pixelX = 0;
	float pixelY = 0;
	float twoPi = 2.0 * 3.14159265359;
	for (int i = 0; i < WIDTH * HEIGHT * 3; i += 3)
	{
		float perspectiveCompensationX = (abs(yawPxCamera) / 360.0) * twoPi;
		float perspectiveCompensationY = (abs(pitchPxCamera) / 360.0) * twoPi;

		float lambertCompensationX = (abs(yawPxPatch) / 360.0) * twoPi;
		float lambertCompensationY = (abs(pitchPxPatch) / 360.0) * twoPi;

		float perspectiveCompnesation = cos(perspectiveCompensationX) * cos(perspectiveCompensationY);
		float lambertCompensation = cos(lambertCompensationX) * cos(lambertCompensationY);

		float redComponent = (int)buffer[i] * perspectiveCompnesation * lambertCompensation;
		float greenComponent = (int)buffer[i + 1] * perspectiveCompnesation * lambertCompensation;
		float blueComponent = (int)buffer[i + 2] * perspectiveCompnesation * lambertCompensation;

		buffer[i] = redComponent;
		buffer[i + 1] = greenComponent;
		buffer[i + 2] = blueComponent;

		yawPxCamera -= PIXEL_WIDTH_IN_DEGREES;
		switch (view)
		{
		case RIGHT:
			yawPxPatch += PIXEL_WIDTH_IN_DEGREES;
			break;
		default:
			yawPxPatch -= PIXEL_WIDTH_IN_DEGREES;
			break;
		}

		pixelX++;

		if (pixelX == WIDTH)
		{
			pixelX = 0;
			pixelY++;
			yawPxCamera = START_ANGLE_PX_CAMERA_X;
			yawPxPatch = START_ANGLE_PX_PATCH_X;
			pitchPxCamera -= PIXEL_HEIGHT_IN_DEGREES;
			switch (view)
			{
			case FRONT:
				pitchPxPatch -= PIXEL_HEIGHT_IN_DEGREES;
				break;
			case LEFT:
				pitchPxPatch -= PIXEL_HEIGHT_IN_DEGREES;
				break;
			case RIGHT:
				pitchPxPatch -= PIXEL_HEIGHT_IN_DEGREES;
				break;
			case BOTTOM:
				pitchPxPatch -= PIXEL_HEIGHT_IN_DEGREES;
				break;
			case TOP:
				pitchPxPatch += PIXEL_WIDTH_IN_DEGREES;
				break;
			}
		}
	}

	//png.close();
	return buffer;
}

radiosity_structs::hemicube * radiosity::apply_multiplier_hemicube(radiosity_structs::hemicube * h)
{
	using radiosity::HEMICUBE_VIEW;
	using radiosity::apply_multiplier;

	radiosity_structs::hemicube * result = new radiosity_structs::hemicube();
	result->_left = apply_multiplier(h->_left, LEFT);
	result->_right = apply_multiplier(h->_right, RIGHT);
	result->_top = apply_multiplier(h->_top, TOP);
	result->_bottom = apply_multiplier(h->_bottom, BOTTOM);
	result->_front = apply_multiplier(h->_front, FRONT);

	return result;
}

radiosity_structs::light * radiosity::get_total_light_of_view(radiosity_structs::hemicube * h, radiosity::HEMICUBE_VIEW view)
{
	using radiosity_application_constants::WINDOW_WIDTH;
	using radiosity_application_constants::WINDOW_HEIGHT;

	using radiosity::HEMICUBE_VIEW;

	radiosity_structs::light * totalLightOfView = new radiosity_structs::light(0.0, 0.0, 0.0);
	float WIDTH = 0, HEIGHT = 0, pixelX = 0, pixelY = 0;
	const char * viewName;
	unsigned char * viewBuffer = NULL;
	switch (view)
	{
	case FRONT:
		viewName = "FRONT";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 2;
		viewBuffer = h->_front;
		break;
	case TOP:
		viewName = "TOP";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 4;
		viewBuffer = h->_top;
		break;
	case LEFT:
		viewName = "LEFT";
		WIDTH = WINDOW_WIDTH / 4;
		HEIGHT = WINDOW_HEIGHT / 2;
		viewBuffer = h->_left;
		break;
	case RIGHT:
		viewName = "RIGHT";
		WIDTH = WINDOW_WIDTH / 4;
		HEIGHT = WINDOW_HEIGHT / 2;
		viewBuffer = h->_right;
		break;
	case BOTTOM:
		viewName = "BOTTOM";
		WIDTH = WINDOW_WIDTH / 2;
		HEIGHT = WINDOW_HEIGHT / 4;
		viewBuffer = h->_bottom;
		break;
	}

	for (int i = 0; i < WIDTH * HEIGHT * 3; i += 3)
	{
		pixelX++;

		totalLightOfView->_red += (int)viewBuffer[i] / 255.0;

		totalLightOfView->_green += (int)viewBuffer[i + 1] / 255.0;

		totalLightOfView->_blue += (int)viewBuffer[i + 2] / 255.0;

		if (pixelX == WIDTH)
		{
			pixelX = 0;
			pixelY++;
		}
	}
	return totalLightOfView;
}

radiosity_structs::light * radiosity::calculaute_incident_light(vectormath::point * center, vectormath::vector * normal, vectormath::vector * up)
{
	using radiosity_application_constants::WINDOW_WIDTH;
	using radiosity_application_constants::WINDOW_HEIGHT;

	using radiosity::render_hemicube_view;
	using radiosity::get_hemicube_pixels;
	using radiosity::get_total_light_of_view;
	using radiosity::HEMICUBE_VIEW;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	int totalPixels = WINDOW_HEIGHT * WINDOW_WIDTH - (WINDOW_HEIGHT * WINDOW_WIDTH) / 4;

	radiosity_structs::light * totalLight = new radiosity_structs::light(0.0, 0.0, 0.0);
	/*
	* Render hemicube
	*/
	// Hemicube left frustrum
	vectormath::vector * leftDirection = add(center, crossproduct(up, normal));
	render_hemicube_view(center, up, leftDirection, LEFT);


	// Hemicube right frustrum
	vectormath::vector * rightDirection = add(center, crossproduct(normal, up));
	render_hemicube_view(center, up, rightDirection, RIGHT);


	// Hemicube top frustrum
	vectormath::vector * topDirection = add(center, up);
	vectormath::vector * reverseNormal = new vectormath::vector(-normal->_x, -normal->_y, -normal->_z);
	render_hemicube_view(center, reverseNormal, topDirection, TOP);


	// Hemicube bottom frustrum
	vectormath::vector * bottomDirection = add(center, crossproduct(leftDirection, normal));
	render_hemicube_view(center, normal, bottomDirection, BOTTOM);

	// Hemicube front 
	vectormath::vector * forwardDirection = add(center, normal);
	render_hemicube_view(center, up, forwardDirection, FRONT);

	radiosity_structs::hemicube * h = new radiosity_structs::hemicube();
	h->_left = get_hemicube_pixels(LEFT);
	h->_right = get_hemicube_pixels(RIGHT);
	h->_top = get_hemicube_pixels(TOP);
	h->_bottom = get_hemicube_pixels(BOTTOM);
	h->_front = get_hemicube_pixels(FRONT);
	radiosity_structs::hemicube * result = apply_multiplier_hemicube(h);

	radiosity_structs::light * topView = get_total_light_of_view(result, TOP);
	radiosity_structs::light * bottomView = get_total_light_of_view(result, BOTTOM);
	radiosity_structs::light * leftView = get_total_light_of_view(result, LEFT);
	radiosity_structs::light * rightView = get_total_light_of_view(result, RIGHT);
	radiosity_structs::light * frontView =get_total_light_of_view(result, FRONT);


	totalLight->_red += topView->_red;
	totalLight->_green += topView->_green;
	totalLight->_blue += topView->_blue;

	totalLight->_red += bottomView->_red;
	totalLight->_green += bottomView->_green;
	totalLight->_blue += bottomView->_blue;

	totalLight->_red += leftView->_red;
	totalLight->_green += leftView->_green;
	totalLight->_blue += leftView->_blue;

	totalLight->_red += rightView->_red;
	totalLight->_green += rightView->_green;
	totalLight->_blue += rightView->_blue;

	totalLight->_red += frontView->_red;
	totalLight->_green += frontView->_green;
	totalLight->_blue += frontView->_blue;

	totalLight->_red /= totalPixels;
	totalLight->_green /= totalPixels;
	totalLight->_blue /= totalPixels;


	return totalLight;
}

void radiosity::render_patch(radiosity_structs::patch * p)
{
	// Render bottom	
	radiosity_structs::light * incident = calculaute_incident_light(p->_center, p->_normal, p->_up);
	p->_incident->_red = incident->_red;
	p->_incident->_green = incident->_green;
	p->_incident->_blue = incident->_blue;
}

void radiosity::calculate_excident(radiosity_structs::patch * p)
{
	p->_excident->_red = p->_incident->_red * p->_reflectance->_red + p->_excident->_red;
	p->_excident->_green = p->_incident->_green * p->_reflectance->_green + p->_excident->_green;
	p->_excident->_blue = p->_incident->_blue * p->_reflectance->_blue + p->_excident->_blue;
}

void radiosity::check_whether_patch_has_component_max(radiosity_structs::patch * p, radiosity_structs::light * totalLight)
{
	if (p->_excident->_red > totalLight->_red)
	{
		totalLight->_red = p->_excident->_red;
	}

	if (p->_excident->_green > totalLight->_green)
	{
		totalLight->_green = p->_excident->_green;
	}

	if (p->_excident->_blue > totalLight->_blue)
	{
		totalLight->_blue = p->_excident->_blue;
	}
}

void radiosity::radiosity()
{
	using demo_geometry::bottomPatch;
	using demo_geometry::backPatch;
	using demo_geometry::leftPatch;
	using demo_geometry::rightPatch;
	using demo_geometry::skyPatch;
	using radiosity_application_constants::iterations;

	using radiosity::render_patch;
	using radiosity::calculate_excident;
	using radiosity::check_whether_patch_has_component_max;

	radiosity_structs::light * totalLight = new radiosity_structs::light(0.0, 0.0, 0.0);
	for (int i = 0; i < iterations; i++)
	{
		std::cout << "Iteration " << i + 1 << "\n";
		// Render skypatch	
		render_patch(skyPatch);

		// Render left	
		render_patch(leftPatch);

		// Render right	
		render_patch(rightPatch);

		// Render back	
		render_patch(backPatch);

		// Render bottom	
		render_patch(bottomPatch);

		calculate_excident(skyPatch);
		calculate_excident(leftPatch);
		calculate_excident(rightPatch);
		calculate_excident(backPatch);
		calculate_excident(bottomPatch);

	}

	check_whether_patch_has_component_max(skyPatch, totalLight);
	check_whether_patch_has_component_max(leftPatch, totalLight);
	check_whether_patch_has_component_max(rightPatch, totalLight);
	check_whether_patch_has_component_max(backPatch, totalLight);
	check_whether_patch_has_component_max(bottomPatch, totalLight);

	skyPatch->_excident->_red /= totalLight->_red;
	skyPatch->_excident->_green /= totalLight->_green;
	skyPatch->_excident->_blue /= totalLight->_blue;

	leftPatch->_excident->_red /= totalLight->_red;
	leftPatch->_excident->_green /= totalLight->_green;
	leftPatch->_excident->_blue /= totalLight->_blue;

	rightPatch->_excident->_red /= totalLight->_red;
	rightPatch->_excident->_green /= totalLight->_green;
	rightPatch->_excident->_blue /= totalLight->_blue;

	backPatch->_excident->_red /= totalLight->_red;
	backPatch->_excident->_green /= totalLight->_green;
	backPatch->_excident->_blue /= totalLight->_blue;

	bottomPatch->_excident->_red /= totalLight->_red;
	bottomPatch->_excident->_green /= totalLight->_green;
	bottomPatch->_excident->_blue /= totalLight->_blue;
}