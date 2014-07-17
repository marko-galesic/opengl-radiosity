#include "radiosity.h"

// TODO : levels for debug output
void radiosity()
{
	Light * totalLight = new Light(0.0, 0.0, 0.0);
		for(int i = 0; i < ITERATIONS; i++)
		{
			printf("Iteration %i\n",(i + 1));
			for(int p = 0; p < NUMBER_OF_PATCHES; p++)
			{
				#ifdef RENDERING_DEBUG
				if (DEBUG)
				{				
					cout << "\r" << "Calculating incident lighting\t" << ((float)p / (float)NUMBER_OF_PATCHES)  * 100 << "%"<< flush;
					printf("\nPatch %i\n", p);
				}		
				#endif
				renderPatch(patches[p]);
			}
			cout << endl;
			for(int p = 0; p < NUMBER_OF_PATCHES; p++)
			{
				#ifdef RENDERING_DEBUG
				cout << "\r" << "Calculating excident lighting\t" << ((float)p / (float)NUMBER_OF_PATCHES)  * 100 << "%"<< flush;			
				if (DEBUG) printf("\nPatch %i\n", p);
				#endif						
				calculateExcident(patches[p]);	

				#ifdef RENDERING_DEBUG
				if (DEBUG) cout << "\nColor After Excident Calculation: " << patches[p]._excident->_red << " " << patches[p]._excident->_green << " " << patches[p]._excident->_blue << endl;
				#endif
			}
			cout << endl;
						
		}
		/*
		for(int p = 0; p < NUMBER_OF_PATCHES; p++)
		{
			checkWhetherPatchHasComponentMax(&patches[p], totalLight);	
		}
		
		printLight(totalLight);		

		for(int p = 0; p < NUMBER_OF_PATCHES; p++)
		{
			//patches[p]._excident->_red /= totalLight->_red;
			//patches[p]._excident->_green /= totalLight->_green;
			//patches[p]._excident->_blue /= totalLight->_blue;

			if (patches[p]._excident->_red != 0.0 || patches[p]._excident->_green != 0.0 || patches[p]._excident->_blue)
			{
				if (DEBUG) cout << patches[p]._excident->_red << " " << patches[p]._excident->_green << " " << patches[p]._excident->_blue << endl;
			}
		}
		*/
}
void renderPatch(Patch * p)
{
	// Render bottom
	p->_incident = calculauteIncidentLight(p->_center, p->_normal, p->_up);

	#ifdef RENDERING_DEBUG
	if (DEBUG) cout << "\nColor After Incident Calculation: " << p->_incident << ;
	#endif
}

void calculateExcident(Patch * p)
{
	p->_excident->setRedFlux(p->_incident->getRedFlux() * p->_reflectance->_red + p->_excident->getRedFlux());
	p->_excident->setGreenFlux(p->_incident->getGreenFlux() * p->_reflectance->_green + p->_excident->getGreenFlux());
	p->_excident->setBlueFlux(p->_incident->getBlueFlux() * p->_reflectance->_blue + p->_excident->getBlueFlux());
}

Flux * calculauteIncidentLight(Point * center, Vector * normal, Vector * up)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	int totalPixels = WINDOW_HEIGHT * WINDOW_WIDTH - (WINDOW_HEIGHT * WINDOW_WIDTH) / 4;
	
	Flux * totalIncident = new Flux(0.0, 0.0, 0.0);
	/* 
	 * Render Hemicube
	 */
	// Hemicube left frustrum
	Vector * leftDirection = new Vector(*center + (*up ^ *normal));
	renderHemicubeView(center, up, leftDirection, LEFT);
	

	// Hemicube right frustrum
	Vector * rightDirection = new Vector(*center + (*normal ^ *up));
	renderHemicubeView(center, up, rightDirection, RIGHT);
	

	// Hemicube top frustrum
	Vector * topDirection = new Vector(*center + *up);
	Vector * reverseNormal = new Vector(-normal->_x, -normal->_y, -normal->_z);
	renderHemicubeView(center, reverseNormal, topDirection, TOP);
	

	// Hemicube bottom frustrum
	Vector * bottomDirection = new Vector(*center + (*leftDirection ^ *normal));
	renderHemicubeView(center, normal, bottomDirection, BOTTOM);

	// Hemicube front 
	Vector * forwardDirection = new Vector(*center + *normal);
	renderHemicubeView(center, up, forwardDirection, FRONT);

	Hemicube * h = new Hemicube();
	h->_left = getHemicubePixels(LEFT);
	h->_right = getHemicubePixels(RIGHT);
	h->_top = getHemicubePixels(TOP);
	h->_bottom = getHemicubePixels(BOTTOM);
	h->_front = getHemicubePixels(FRONT);
	Hemicube * result = applyMultiplierHemicube(h);
	
	Flux * topView = getTotalLightOfView(result, TOP);
	Flux * bottomView = getTotalLightOfView(result, BOTTOM);
	Flux * leftView = getTotalLightOfView(result, LEFT);
	Flux * rightView = getTotalLightOfView(result, RIGHT);
	Flux * frontView = getTotalLightOfView(result, FRONT);
	
	*totalIncident += *topView;
	*totalIncident += *bottomView;
	*totalIncident += *leftView;
	*totalIncident += *rightView;
	*totalIncident += *frontView;
	
	#ifdef RENDERING_DEBUG
	if (DEBUG) printf("Total incident Light (%f, %f, %f)\n",totalLight->_red, totalLight->_green, totalLight->_blue);
	#endif

	*totalIncident /= totalPixels;

	#ifdef RENDERING_DEBUG
	if (DEBUG) printf("Total incident Light (%f, %f, %f) (normalized)\n",totalLight->_red, totalLight->_green, totalLight->_blue);
	#endif

	delete h;
	delete topView;
	delete bottomView;
	delete leftView;
	delete rightView;
	delete frontView;
	return totalIncident;
}

Flux * getTotalLightOfView(Hemicube * h, HEMICUBE_VIEW view)
{
	Flux * totalLightOfView = new Flux(0.0, 0.0, 0.0);
	float WIDTH, HEIGHT, pixelX, pixelY;
	const char * viewName;
	unsigned char * viewBuffer;
	switch(view)
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

	for(int i = 0; i < WIDTH * HEIGHT * 3; i+=3)
	{
		pixelX++;

		// TODO Change this from getting light to getting the index of a patch
		totalLightOfView->incrementFlux((int)viewBuffer[i] / 255.0, (int)viewBuffer[i + 1] / 255.0, (int)viewBuffer[i + 2] / 255.0);

		if (pixelX == WIDTH)
		{
			pixelX = 0;
			pixelY++;
		}
	}
	//delete viewName;
	return totalLightOfView;
}

void checkWhetherPatchHasComponentMax(Patch * p, Light * totalLight)
{
	if (p->_excident->getRedFlux() > totalLight->_red )
	{
		totalLight->_red = p->_excident->getRedFlux();
	}

	if (p->_excident->getGreenFlux() > totalLight->_green )
	{
		totalLight->_green = p->_excident->getGreenFlux();
	}	

	if (p->_excident->getBlueFlux() > totalLight->_blue )
	{
		totalLight->_blue = p->_excident->getBlueFlux();
	}
}

void add(Light * a, Light * b)
{
	a->_red += b->_red;
	a->_green += b->_green;
	a->_blue += b->_blue;
}

void printLight(Light * l)
{
	//cout << l->_red << " " << l->_green << " " << l->_blue << "\n";
}


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
		#ifdef RENDERING_DEBUG
		if (DEBUG) printf("Patch %i Excident\t: %f %f %f\n", p, patches[p]._excident->_red, patches[p]._excident->_green, patches[p]._excident->_blue);
		#endif
		
		glColor3f(patches[p]->_excident->getRedFlux(), patches[p]->_excident->getGreenFlux(), patches[p]->_excident->getBlueFlux());
		
		glBegin(GL_POLYGON);
		for(int v = 0; v < patches[p]->_numVerts; v++)
		{
			glVertex3f(patches[p]->_vertices[v]._x, patches[p]->_vertices[v]._y, patches[p]->_vertices[v]._z);
		}
		glEnd();
	}
}

void renderHemicubeView(Point * center, Vector * up, Vector * direction, HEMICUBE_VIEW view)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch(view)
	{
		case FRONT:
			glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 5.0);
			break;
		case TOP:
			glFrustum(-1.0, 1.0, -1.0, 0.0, 1.0, 5.0);
			break;
		case LEFT:	
			glFrustum(0.0, 1.0, -1.0, 1.0, 1.0, 5.0);
			break;
		case RIGHT:
			glFrustum(-1.0, 0.0, -1.0, 1.0, 1.0, 5.0);
			break;
		case BOTTOM:
			glFrustum(-1.0, 1.0, 0.0, 1.0, 1.0, 5.0);
			break;
	}
	
	glMatrixMode(GL_MODELVIEW);

	switch(view)
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
	renderScene();
}

bool blah = true;
unsigned char * applyMultiplier(unsigned char * buffer, HEMICUBE_VIEW view)
{
	float 	WIDTH, 
		HEIGHT, 
		VIEW_X, 
		VIEW_Y, 
		PIXEL_HEIGHT_IN_DEGREES, 
		PIXEL_WIDTH_IN_DEGREES, 
		START_ANGLE_PX_CAMERA_X, 
		START_ANGLE_PX_CAMERA_Y, 
		START_ANGLE_PX_PATCH_X, 
		START_ANGLE_PX_PATCH_Y;
	const char * viewName;
	switch(view)
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

	float yawPxPatch = START_ANGLE_PX_PATCH_X;		// Yaw (in degrees) between the current pixel's center and the normal of the Patch
	float pitchPxPatch = START_ANGLE_PX_PATCH_Y;		// Pitch (in degrees) between the current pixel's center and the normal of the Patch

	float pixelX = 0;
	float pixelY = 0;
	float twoPi = 2.0 * 3.14159265359;
	for(int i = 0; i < WIDTH * HEIGHT * 3; i+=3)
	{	
		float perspectiveCompensationX = (yawPxCamera / 360.0) * twoPi;
		float perspectiveCompensationY = (pitchPxCamera / 360.0) * twoPi;

		float lambertCompensationX = (yawPxPatch / 360.0) * twoPi;
		float lambertCompensationY = (pitchPxPatch / 360.0) * twoPi;
		 
		float perspectiveCompnesation = cos(perspectiveCompensationX) * cos(perspectiveCompensationY);
		float lambertCompensation = cos(lambertCompensationX) * cos(lambertCompensationY);
		
		// TODO : unit conversion from 0 - 255 to 0 - 1 (?)
		float redComponent = (int)buffer[i] * perspectiveCompnesation * lambertCompensation;
		float greenComponent = (int)buffer[i + 1] * perspectiveCompnesation * lambertCompensation;
		float blueComponent = (int)buffer[i + 2] * perspectiveCompnesation * lambertCompensation;
		
		buffer[i] = redComponent;
		buffer[i + 1] = greenComponent;
		buffer[i + 2] = blueComponent;

		yawPxCamera -= PIXEL_WIDTH_IN_DEGREES;
		switch(view)
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
			switch(view)
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
Hemicube * applyMultiplierHemicube(Hemicube * h)
{
	Hemicube * result = new Hemicube();
	result->_left = applyMultiplier(h->_left, LEFT);
	result->_right = applyMultiplier(h->_right, RIGHT);
	result->_top = applyMultiplier(h->_top, TOP);
	result->_bottom = applyMultiplier(h->_bottom, BOTTOM);
	result->_front = applyMultiplier(h->_front, FRONT);

	return result;
}
unsigned char * getHemicubePixels(HEMICUBE_VIEW view)
{
	float 	WIDTH, 
		HEIGHT, 
		VIEW_X, 
		VIEW_Y, 
		PIXEL_HEIGHT_IN_DEGREES, 
		PIXEL_WIDTH_IN_DEGREES, 
		START_ANGLE_PX_CAMERA_X, 
		START_ANGLE_PX_CAMERA_Y, 
		START_ANGLE_PX_PATCH_X, 
		START_ANGLE_PX_PATCH_Y;
	const char * viewName;
	unsigned char * buffer;
	switch(view)
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
	buffer = (unsigned char *)malloc(WIDTH * HEIGHT * 3);

	glReadPixels(VIEW_X, VIEW_Y, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	return buffer; 
}


void initializePatches()
{
	#ifdef PARSING_DEBUG
	if(DEBUG) cout << "Generating patches from obj file... \n";
	#endif

	int numVerts = 0;
	NUMBER_OF_PATCHES = shapes.size();

	Color * emission = NULL;
	Color * reflectance = NULL;
	Flux * excident = NULL;
	Point * vertices = NULL;
	Vector * normal = NULL;
	Vector * right = NULL;
	Vector * upVector = NULL;
	Point * center = NULL;
	for (size_t patch = 0; patch < NUMBER_OF_PATCHES; patch++)
	{	
		if (emission->isBlack())
		{
			emission = new Color(shapes[patch].material.emission);
			excident = new Flux(*emission);
		}
		else
		{
			excident = new Flux();
		}

		reflectance = new Color(shapes[patch].material.diffuse);

		numVerts = shapes[patch].mesh.positions.size() / 3;
		vertices = new Point[numVerts];

		for(size_t v = 0; v < numVerts; v++)
		{
			float x = shapes[patch].mesh.positions[3*v+0];
			float y = shapes[patch].mesh.positions[3*v+1];
			float z = shapes[patch].mesh.positions[3*v+2];
			
			vertices[v]._x = x;
			vertices[v]._y = y;
			vertices[v]._z = z;
		}
		
		// Calculate normal
		normal = new Vector((vertices[1] - vertices[0]) ^ (vertices[2] - vertices[1]));

		// Calculate up vector
		right = new Vector(*normal ^ *Y_AXIS);
		upVector = new Vector(*right ^ *normal);

		if(upVector->_x == 0.0 && upVector->_y == 0.0 && upVector->_z == 0.0)
		{
			upVector->_x = 0;
			upVector->_y = 0;
			upVector->_z = -1;
		}

		// Calculate center Point
		float centerX = 0.0;
		float centerY = 0.0;
		float centerZ = 0.0;			
		for(int v = 0; v < numVerts; v++)
		{
			centerX += vertices[v]._x;
			centerY += vertices[v]._y;
			centerZ += vertices[v]._z;
		}

		centerX /= numVerts;
		centerY /= numVerts;
		centerZ /= numVerts;
		
		(centerX, centerY, centerZ);

		#ifdef PARSING_DEBUG
		printf("Patch %i\n", (int)patch);
		if(DEBUG) cout << "Emission\t: " << emission;
		if(DEBUG) cout << "Reflectance\t: " << reflectance;
		if(DEBUG) cout << "Excident\t: " << excident;
		if(DEBUG) cout << "Normal\t\t: " << normal;
		if(DEBUG) cout << "Up\t\t: " << upVector;
		if(DEBUG) cout << "Center\t\t: " << center;
		#endif
		
		if (emission->isBlack())
		{
			patches.push_back(new Patch(center, normal, upVector, reflectance, vertices, numVerts));
		}
		else
		{
			patches.push_back(new LightPatch(center, normal, upVector, reflectance, emission, vertices, numVerts));
		}
	}
	#ifdef PARSING_DEBUG
	if(DEBUG) cout << NUMBER_OF_PATCHES << " patches" << endl;
	#endif	
}

/**
 * Launch program of radiosity solution.
 *
 * @param argv[1] *.obj file to run radiosity on
 * @param argv[2] number of iterations we plan to run radiosity with
 * @param argv[3] Debug flag
 */
int main(int argc, char* argv[])
{
	// Get the filename of the obj file we will run radiosity on
	string inputfile = argv[1];

	// Get the number of iterations to do progressive radiosity
	ITERATIONS = atoi(argv[2]);

	// Are we going to go into debug mode?
	if(atoi(argv[3]) == 1) DEBUG = true;

	string err = tinyobj::LoadObj(shapes, inputfile.c_str());

	if(!err.empty())
	{
		cerr << err << endl;
		exit(1);
	}

	cout << "Running radiosity on " << inputfile << " with " << ITERATIONS << " iterations.\n";

	initializePatches();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	glutInitWindowPosition(100,100);
    	glutCreateWindow("OpenGL - First window demo");
	glutDisplayFunc(renderFunction);
	glutMainLoop();
	return 0;
}
