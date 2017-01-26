#include <iostream>
#include <GL\freeglut.h>
#include "radiosity_application_constants.h"
#include "radiosity.h"
 
// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx = 0.0f, lz = 0.0f;

// XZ position of the camera
float x = 0.0f, z = 0.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void render_function()
{
	using radiosity_application_constants::running;

	if (running)
	{
		radiosity::radiosity();
		running = false;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, radiosity_application_constants::WINDOW_WIDTH, radiosity_application_constants::WINDOW_HEIGHT);
	gluPerspective(45.0f, radiosity_application_constants::WINDOW_WIDTH / radiosity_application_constants::WINDOW_HEIGHT, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); 
	if (deltaMove)
	{
		computePos(deltaMove);
	}
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
	radiosity::render_scene();
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {

	if (key == 27)
		exit(0);
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_UP: deltaMove = 0.5f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize
		(
			radiosity_application_constants::WINDOW_WIDTH, 
			radiosity_application_constants::WINDOW_HEIGHT
		);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Radiosity");
	radiosity::initialize_patches();

	glutDisplayFunc(render_function);
	glutIdleFunc(render_function);

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glutMotionFunc(mouseMove);
	glutMotionFunc(mouseMove);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}