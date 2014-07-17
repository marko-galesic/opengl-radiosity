#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>
#include <ostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include "tiny_obj_loader.h"
#include "struct.h"
#include "vector.h"

#define PARSING_DEBUG
//#define RENDERING_DEBUG
using namespace std;
using namespace entities;
using namespace vecmath;

int WINDOW_HEIGHT = 512;
int WINDOW_WIDTH = 512;

enum HEMICUBE_VIEW { LEFT, RIGHT, TOP, BOTTOM, FRONT };

const Vector * Y_AXIS = new Vector(0.0, 1.0, 0.0);

void checkWhetherPatchHasComponentMax(Patch * p, Light * totalLight);
void renderHemicubeView(Point * center, Vector * up, Vector * direction, HEMICUBE_VIEW view);
Flux * calculauteIncidentLight(Point * center, Vector * normal, Vector * up);
void getHemicubePixels();
Hemicube * applyMultiplierHemicube(Hemicube * h);
unsigned char * applyMultiplier(unsigned char * buffer, HEMICUBE_VIEW view);
unsigned char * getHemicubePixels(HEMICUBE_VIEW view);
Flux * getTotalLightOfView(Hemicube * h, HEMICUBE_VIEW view);
void calculateExcident(Patch * p);
void renderPatch(Patch * p);
void renderScene();
void printLight(Light * l);
int ITERATIONS = 0;
int NUMBER_OF_PATCHES = 0;
bool running = true;
vector<tinyobj::shape_t> shapes;
vector<Patch*> patches;
bool DEBUG = false;
