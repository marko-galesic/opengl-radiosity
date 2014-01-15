#include "radiosity.h"

/*
 * Rendering functions
 */
void renderFunction()
{
	
	if(running)
	{
		radiosity();
		running = false;
	}
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0, 1.0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	renderScene();	
	glFlush();
}

// TODO : should renderScene get passed patches * as a parameter?

void renderScene()
{
	for(int p = 0; p < NUMBER_OF_PATCHES; p++)
	{
		glColor3f(patches[p]._excident->_red, patches[p]._excident->_green, patches[p]._excident->_blue);
		
		if (DEBUG) cout << "Color : " << patches[p]._excident->_red << " " << patches[p]._excident->_green << " " << patches[p]._excident->_blue << endl;
		glBegin(GL_POLYGON);
		for(int v = 0; v < patches[p].numVerts; v++)
		{
			if (DEBUG) cout << patches[p]._vertices[v]._x << " " << patches[p]._vertices[v]._y << " " << patches[p]._vertices[v]._z << endl;
			glVertex3f(patches[p]._vertices[v]._x, patches[p]._vertices[v]._y, patches[p]._vertices[v]._z);
		}
		glEnd();
	}
}

void renderBakingScene()
{
	for(int p = 0; p < NUMBER_OF_PATCHES; p++)
	{
		glColor3f((float)patches[p]._id->_red / 255.0, (float)patches[p]._id->_green / 255.0, (float)patches[p]._id->_blue / 255.0);

		cout << "Color : " << patches[p]._id->_red << " " << patches[p]._id->_green << " " << patches[p]._id->_blue << endl;
		glBegin(GL_POLYGON);
		for(int v = 0; v < patches[p].numVerts; v++)
		{
			if (DEBUG) cout << patches[p]._vertices[v]._x << " " << patches[p]._vertices[v]._y << " " << patches[p]._vertices[v]._z << endl;
			glVertex3f(patches[p]._vertices[v]._x, patches[p]._vertices[v]._y, patches[p]._vertices[v]._z);
		}
		glEnd();
	}
}

