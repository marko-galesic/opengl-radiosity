#include <iostream>
#include <GL\freeglut.h>
#include "radiosity_application_constants.h"
#include "radiosity.h"
 
void render_function()
{
	using radiosity_application_constants::running;
	using radiosity_application_constants::WINDOW_WIDTH;
	using radiosity_application_constants::WINDOW_HEIGHT;

	if (running)
	{
		radiosity::radiosity();
		running = false;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0, 1.0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	radiosity::render_scene();
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize
		(
			radiosity_application_constants::WINDOW_WIDTH, 
			radiosity_application_constants::WINDOW_HEIGHT
		);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Radiosity");
	radiosity::initialize_patches();
	glutDisplayFunc(render_function);
	glutMainLoop();
	return 0;
}